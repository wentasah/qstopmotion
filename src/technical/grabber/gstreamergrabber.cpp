/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#include "gstreamergrabber.h"

#include "technical/util.h"

#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>


// GMainLoop *GstreamerGrabber::loop = 0;
#include <gst/interfaces/propertyprobe.h>


GstreamerGrabber::GstreamerGrabber(GstreamerSource as, const QString &filePath)
    : ImageGrabber(filePath)
{
    qDebug("GstreamerGrabber::Constructor --> Start");

    switch (as) {
    case 1:
#ifdef Q_WS_X11
        activeSource = GstreamerGrabber::Video4LinuxSource;
#endif
#ifdef Q_WS_WIN
        activeSource = GstreamerGrabber::DirectShowSource;
#endif
        break;
    case 2:
        activeSource = GstreamerGrabber::Iee1394Source;
        break;
    default:
        activeSource = GstreamerGrabber::TestSource;
        break;
    }

    isInitSuccess = false;
    pipeline = 0;
    source = 0;
    filter1 = 0;
    filter2 = 0;
    filter3 = 0;
    filter4 = 0;
    filter5 = 0;
    queue1 = 0;
    queue2 = 0;
    queue3 = 0;
    queue4 = 0;
    sink = 0;

    gst_init(0,0);

    qDebug("GstreamerGrabber::Constructor --> End");
}


GstreamerGrabber::~GstreamerGrabber()
{
    qDebug("GstreamerGrabber::Destructor --> Start");

    qDebug("GstreamerGrabber::Destructor --> End");
}


void GstreamerGrabber::initialization()
{
    qDebug("GstreamerGrabber::initialization --> Start");

    gchar *device_name = NULL;
    GstElementFactory *srcfactory = NULL;
    GstElement *src = NULL;
    GstPropertyProbe *probe = NULL;
    gchar *prop_name = NULL;
    GValueArray *va = NULL;

    // TODO: Handle more than one device per source
    // device_name = "videotestsrc";
#ifdef Q_WS_X11
    // device_name = "v4l2src";
    device_name = "dv1394src";
#endif
#ifdef Q_WS_WIN
    device_name = "dshowvideosrc";
#endif

    srcfactory = gst_element_factory_find(device_name);
    g_return_if_fail(srcfactory != NULL);
    src = gst_element_factory_create(srcfactory, "source");
    if (!src) {
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_if_fail(src != NULL);

    if (GST_IS_PROPERTY_PROBE(src)) {
        gst_element_set_state (src, GST_STATE_READY);
        // wait until it’s up and running or failed
        if (gst_element_get_state (src, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Source failed to go into READY state.";
            gst_object_unref(GST_OBJECT(src));
            return;
        }
        probe = GST_PROPERTY_PROBE(src);

        if (!g_strcmp0 (device_name, "dv1394src")) {
            prop_name = "guid";
        }
        else {
            if (!g_strcmp0 (device_name, "v4l2src")) {
                prop_name = "device";
            }
            else {
                prop_name = "device-name";
            }
        }

        va = gst_property_probe_get_values_name (probe, prop_name);
        if (va != NULL) {
            for (unsigned int i = 0 ; i < va->n_values ; i++) {
                GValue* v0 = g_value_array_get_nth(va, i);
                GValue valstr = {0,};
                g_value_init(&valstr, G_TYPE_STRING);
                if (!g_value_transform(v0, &valstr)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                }
                qDebug() << "gstreamergrabber::initialization --> device " << i << " '" << g_value_get_string(&valstr) << "'";
            }
        }
    }
    else {
        qDebug() << "gstreamergrabber::initialization --> device 0";
    }
    qDebug() << "gstreamergrabber::initialization --> Check device count";

    // END: Handle more than one device per source

    // const GList* pva = gst_property_probe_get_properties(probe);

    // Reset source object
    gst_element_set_state(src, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(src));

    isInitialized = true;
    isProcess = true;

    qDebug("GstreamerGrabber::initialization --> End (true)");

    return;
}


void GstreamerGrabber::init()
{
    qDebug() << "gstreamergrabber::init --> Start";

    GstBus *bus;

    // gst_init(0,0);

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    switch (activeSource) {
    case TestSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: videotestsrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        source = gst_element_factory_make("videotestsrc", "source=videotestsrc");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return;
        }
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        // Add the elements to the bin
        if (!gst_bin_add(GST_BIN (pipeline), source)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return;
        }

        // Link the elements in the bin
        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return;
        }
        if (!gst_element_link(filter1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2.";
            return;
        }
        if (!gst_element_link(filter2, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink.";
            return;
        }

        break;
    case Video4LinuxSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: v4l2src ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        // Create the elements
        source = gst_element_factory_make("v4l2src", NULL); // "source=v4l2src");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return;
        }
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        // g_object_set (G_OBJECT(source), "device", device_id, NULL);

        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        // Add the elements to the bin
        if (!gst_bin_add(GST_BIN (pipeline), source)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return;
        }

        // Link the elements in the bin
        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return;
        }
        if (!gst_element_link(filter1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2.";
            return;
        }
        if (!gst_element_link(filter2, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink.";
            return;
        }

        break;
    case Iee1394Source:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dv1394src ! queue ! dvdemux ! queue ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        // Examples:
        // gst-launch dv1394src ! video/x-dv ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dv1394src ! ffdemux_dv ! ffdec_dvvideo ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dv1394src ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE

        // Create the elements
        source = gst_element_factory_make("dv1394src", "source=dv1394src");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return;
        }
        // g_object_set(G_OBJECT(source), "port", 0, NULL);
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        // g_object_set(G_OBJECT(source), "guid", g_ascii_strtoull(device_id, NULL, 0), NULL);

        filter1 = gst_element_factory_make("dvdemux", "filter1=dvdemux");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return;
        }
        g_object_set(G_OBJECT(filter1), "name", "d", NULL);
        queue1 = gst_element_factory_make("queue", "queue1=queue");
        if (!queue1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the queue1.";
            return;
        }
        filter2 = gst_element_factory_make("dvdec", "filter2=dvdec");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return;
        }
        queue2 = gst_element_factory_make("queue", "queue2=queue");
        if (!queue1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the queue1.";
            return;
        }
/*
        filter3 = gst_element_factory_make("ffdeinterlace", "filter3=ffdeinterlace");
        if (!filter3) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter3.";
            return;
        }
*/
        filter4 = gst_element_factory_make("ffmpegcolorspace", "filter4=ffmpegcolorspace");
        if (!filter4) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter4.";
            return;
        }
        filter5 = gst_element_factory_make("jpegenc", "filter5=jpegenc");
        if (!filter5) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter5.";
            return;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        // Add the elements to the bin
        if (!gst_bin_add(GST_BIN (pipeline), source)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), queue1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the queue1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), queue2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the queue1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter4)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter5)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return;
        }

        // Link the elements in the bin
        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return;
        }
        /*
        if (!gst_element_link(filter1, queue1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the queue1.";
            return;
        }

        if (!gst_element_link(queue1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2.";
            return;
        }
        if (!gst_element_link(filter2, filter3)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter3.";
            return;
        }
        if (!gst_element_link(filter2, queue2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the queue1.";
            return;
        }
        */
        if (!gst_element_link(filter2, filter4)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter4.";
            return;
        }
        if (!gst_element_link(filter4, filter5)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter5.";
            return;
        }
        if (!gst_element_link(filter5, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink.";
            return;
        }
        // Connect filter1 and filter2 via a pad
        // The reason is, that the demultiplexer can handle 2 streams (video and audio)
        g_signal_connect(filter1,"pad-added",G_CALLBACK(on_pad_added),filter2);

        break;
    case DirectShowSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dshowvideosrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        // Create the elements
        source = gst_element_factory_make("dshowvideosrc", "source=dshowvideosrc");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return;
        }
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        // g_object_set (G_OBJECT(source), "device-name", device_id, NULL);
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        // Add the elements to the bin
        if (!gst_bin_add(GST_BIN (pipeline), source)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return;
        }
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return;
        }

        // Link the elements in the bin
        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return;
        }
        if (!gst_element_link(filter1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2.";
            return;
        }
        if (!gst_element_link(filter2, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink.";
            return;
        }

        break;
    default:
        qDebug() << "gstreamergrabber::init --> Unknown source";

        return;
    }

    qDebug() << "gstreamergrabber::init --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    isInited = TRUE;

    qDebug() << "gstreamergrabber::init --> End";

    return;
}


bool GstreamerGrabber::tearDown()
{
    qDebug() << "gstreamergrabber::tearDown --> Start";

    int ret;

    ret = gst_element_set_state(pipeline, GST_STATE_NULL);
    switch (ret) {
    case GST_STATE_CHANGE_FAILURE:
        break;
    case GST_STATE_CHANGE_SUCCESS:
        break;
    case GST_STATE_CHANGE_ASYNC:
        break;
    case GST_STATE_CHANGE_NO_PREROLL:
        break;
    }

    gst_object_unref(GST_OBJECT (pipeline));

    // g_main_loop_unref(GstreamerGrabber::loop);

    qDebug() << "gstreamergrabber::tearDown --> End";

    return true;
}


bool GstreamerGrabber::grab()
{
    /*
    if (process.startDetached(prePoll) == false) {
        isInitSuccess = false;
        return false;
    }
    */
    return true;
}


gboolean GstreamerGrabber::bus_callback(GstBus * /*bus*/, GstMessage *message, gpointer /*data*/)
{
    qDebug() << "gstreamergrabber::event_loop --> Start";
    qDebug() << "Got " << GST_MESSAGE_TYPE_NAME (message) << " message";

    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS:
        qDebug() << "gstreamergrabber::event_loop --> Message EOS";

        // g_main_loop_quit(GstreamerGrabber::loop);
        break;
    case GST_MESSAGE_INFO:{
            qDebug() << "gstreamergrabber::event_loop --> Message INFO";

            GError *gerror;
            gchar *debug;

            gst_message_parse_info (message, &gerror, &debug);
            gst_object_default_error (GST_MESSAGE_SRC (message), gerror, debug);
            qDebug() << "Message: " << gerror->message;
            qDebug() << "Debug: " << debug;
            g_error_free (gerror);
            g_free (debug);

            break;
        }
    case GST_MESSAGE_WARNING:{
            qDebug() << "gstreamergrabber::event_loop --> Message WARNING";

            GError *gerror;
            gchar *debug;

            gst_message_parse_warning (message, &gerror, &debug);
            gst_object_default_error (GST_MESSAGE_SRC (message), gerror, debug);
            qDebug() << "Message: " << gerror->message;
            qDebug() << "Debug: " << debug;
            g_error_free (gerror);
            g_free (debug);

            break;
        }
    case GST_MESSAGE_ERROR:{
            qDebug() << "gstreamergrabber::event_loop --> Message ERROR";

            GError *gerror;
            gchar *debug;

            gst_message_parse_error (message, &gerror, &debug);
            gst_object_default_error (GST_MESSAGE_SRC (message), gerror, debug);
            qDebug() << "Message: " << gerror->message;
            qDebug() << "Debug: " << debug;
            g_error_free (gerror);
            g_free (debug);

            // g_main_loop_quit(GstreamerGrabber::loop);
            break;
        }
    case GST_MESSAGE_STATE_CHANGED: {
            GstState old_state, new_state;

            gst_message_parse_state_changed (message, &old_state, &new_state, NULL);
            g_print ("Element %s changed state from %s to %s.\n",
                GST_OBJECT_NAME (message->src),
                gst_element_state_get_name (old_state),
                gst_element_state_get_name (new_state));
            // qDebug() << "Element " << GST_OBJECT_NAME (message->src)
            //        << " changed state from " << gst_element_state_get_name (old_state)
            //        << " to " << gst_element_state_get_name (new_state) << "!";
            break;
        }
    default:
        // Unhandeled message
        break;
    }
    return true;
}

void GstreamerGrabber::on_pad_added (GstElement *element,
                                     GstPad     *pad,
                                     gpointer    data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    /* We can now link this pad with the vorbis-decoder sink pad */
    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);
}
