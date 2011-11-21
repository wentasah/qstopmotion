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


GstreamerGrabber::GstreamerGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("GstreamerGrabber::Constructor --> Start");

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


void GstreamerGrabber::initializationSubclass()
{
    qDebug("GstreamerGrabber::initialization --> Start");

    gchar *device_name = NULL;
    GstElementFactory *srcfactory = NULL;
    GstElement *src = NULL;
    GstPropertyProbe *probe = NULL;
    gchar *property_name = NULL;
    gchar *property_id = NULL;
    GValueArray *values_name = NULL;
    GValueArray *values_id = NULL;
    int device_size;

    qDebug() << "gstreamergrabber::initialization --> Add video test device";

    // device_name = "videotestsrc";

    device_size = devices.size();
    devices.resize(device_size + 1);
    ((ImageGrabberDevice)devices[device_size]).deviceName.append(QObject::tr("Video test device"));
    ((ImageGrabberDevice)devices[device_size]).deviceId.append("");

#ifdef Q_WS_X11
    qDebug() << "gstreamergrabber::initialization --> Check devices of dv1394src";

    device_name = "dv1394src";
    property_name = "device-name";
    property_id = "guid";

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

        values_name = gst_property_probe_get_values_name (probe, property_name);
        values_id = gst_property_probe_get_values_name (probe, property_id);
        if ((values_name != NULL) && (values_id != NULL) && (values_name->n_values == values_id->n_values)) {
            qDebug() << "gstreamergrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_name->n_values ; i++) {
                // Handle the device name
                GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_name_pointer, &value_name_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return;
                }
                qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << g_value_get_string(&value_name_string) << "'";
                // Handle the device id
                GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                if (!g_value_transform(value_id_pointer, &value_id_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return;
                }
                qDebug() << "gstreamergrabber::initialization --> device id " << i << " '" << g_value_get_string(&value_id_string) << "'";
                // Add the device to the device list
                int device_size = devices.size();
                devices.resize(device_size + 1);
                ((ImageGrabberDevice)devices[device_size]).deviceName.append(g_value_get_string(&value_name_string));
                ((ImageGrabberDevice)devices[device_size]).deviceId.append(g_value_get_string(&value_id_string));
            }
            g_value_array_free(values_name);
            g_value_array_free(values_id);
        }
    }

    // Reset source object
    gst_element_set_state(src, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(src));

#endif

#ifdef Q_WS_X11
    qDebug() << "gstreamergrabber::initialization --> Check devices of v4l2src";

    device_name = "v4l2src";
    property_name = "device-name";
    property_id = "device";

    // R.L. In this time, the implementation of 'gst_property_probe_get_values_name'
    // in the 'v4l2src' source is buggy.

    device_size = devices.size();
    devices.resize(device_size + 1);
    ((ImageGrabberDevice)devices[device_size]).deviceName.append(QObject::tr("Device 1"));
    ((ImageGrabberDevice)devices[device_size]).deviceId.append("/dev/video1");

    device_size++;
    devices.resize(device_size + 1);
    ((ImageGrabberDevice)devices[device_size]).deviceName.append(QObject::tr("Device 2"));
    ((ImageGrabberDevice)devices[device_size]).deviceId.append("/dev/video2");
#endif

#ifdef Q_WS_WIN
    qDebug() << "gstreamergrabber::initialization --> Check devices of dshowvideosrc";

    device_name = "dshowvideosrc";
    property_name = "device-name";
    property_id = "device";

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
        values_name = gst_property_probe_get_values_name (probe, property_id);
        if (values_name != NULL) {
            qDebug() << "gstreamergrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_name->n_values ; i++) {
                // Handle the device name
                GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_name_pointer, &value_name_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return;
                }
                qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << g_value_get_string(&value_name_string) << "'";
                // Add the device to the device list
                int device_size = devices.size();
                devices.resize(device_size + 1);
                ((ImageGrabberDevice)devices[device_size]).deviceName.append(g_value_get_string(&value_name_string));
                ((ImageGrabberDevice)devices[device_size]).deviceId.append("");
            }
            g_value_array_free(values_name);
        }
        // Reset source object
        gst_element_set_state(src, GST_STATE_NULL);
    }

    // Floating referenz counter of source object is 0 ?????
    // gst_object_unref(GST_OBJECT(src));
    // gst_object_unref(GST_OBJECT(srcfactory));

#endif

    isInitialized = true;
    isProcess = true;

    qDebug() << "gstreamergrabber::initialization --> device count: " << devices.size();

    qDebug("GstreamerGrabber::initialization --> End (true)");

    return;
}


void GstreamerGrabber::initSubclass()
{
    qDebug() << "gstreamergrabber::init --> Start";

    GstBus *bus;

    // gst_init(0,0);

    switch ((GstreamerGrabber::GstreamerSource)frontend->getProject()->getVideoSource()) {
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

        // Examples:
        // gst-launch dshowvideosrc ! video/x-dv ! ffdemux_dv ! ffdec_dvvideo ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch ksvideosrc ! ...
        // gst-launch dshowvideosrc ! video/x-raw-rpg ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dshowvideosrc ! video/x-raw-yuv ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE

        // R.L.: Direct Show Video Source didn't work with FireWire (IEE1394) devices.
        // The application hangs when linking dshowvideosrc and ffdemux_dv.

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

    qDebug() << "gstreamergrabber::on_pad_added --> Start";

    /* We can now link this pad with the vorbis-decoder sink pad */
    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);

    qDebug() << "gstreamergrabber::on_pad_added --> Start";
}

void GstreamerGrabber::cb_typefound (GstElement *typefind,
                                     guint       probability,
                                     GstCaps    *caps,
                                     gpointer    data)
{
    // GMainLoop *loop = data;
    gchar *type;

    qDebug() << "gstreamergrabber::cb_typefound --> Start";

    type = gst_caps_to_string (caps);
    qDebug() << "Media type " << type << "found, probability " << probability;
    g_free (type);

    // since we connect to a signal in the pipeline thread context, we need
    // to set an idle handler to exit the main loop in the mainloop context.
    // Normally, your app should not need to worry about such things. */
    // g_idle_add (idle_exit_loop, loop);

    qDebug() << "gstreamergrabber::cb_typefound --> End";
}

gboolean GstreamerGrabber::link_elements_with_filter (GstElement *element1, GstElement *element2)
{
    gboolean link_ok;
    GstCaps *caps;

    qDebug() << "gstreamergrabber::link_elements_with_filter --> Start";

    caps = gst_caps_new_simple("video/x-dv",
                               "systemstream", G_TYPE_BOOLEAN, TRUE,
                               NULL);
    link_ok = gst_element_link_filtered (element1, element2, caps);
    gst_caps_unref (caps);
    if (!link_ok) {
        qDebug() << "gstreamergrabber::link_elements_with_filter --> Failed to link element1 and element2!";
    }

    qDebug() << "gstreamergrabber::link_elements_with_filter --> End";

    return link_ok;
}
