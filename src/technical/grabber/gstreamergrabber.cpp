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
    sink = 0;

    qDebug("GstreamerGrabber::Constructor --> End");
}


GstreamerGrabber::~GstreamerGrabber()
{
    qDebug("GstreamerGrabber::Destructor --> Start");

    qDebug("GstreamerGrabber::Destructor --> End");
}


bool GstreamerGrabber::initialization()
{
    qDebug("GstreamerGrabber::initialization --> Start");

    bool isCameraReady = true;
/*
    GList               *walk;
    GList               *type_list;
    GstTypeFindFactory  *findFactory;
    GstElementFactory   *elementFactory;
    gchar              **extensions;
    GstCaps             *caps;

    gst_init(0,0);
    walk = type_list = gst_type_find_factory_get_list();
    while (walk)
    {
        gchar *capsString;
        
        findFactory = (GstTypeFindFactory *)walk->data;
        QString factoryName(gst_plugin_feature_get_name(GST_PLUGIN_FEATURE (findFactory)));
        extensions = gst_type_find_factory_get_extensions(findFactory);
        if (!extensions) {
            qDebug() << "Factory: " << factoryName << " No Extension!";

            walk = g_list_next(walk);
            continue;
        }
        elementFactory = gst_element_factory_find(extensions[0]);
        if (!elementFactory) {
            qDebug() << "Factory: " << factoryName << " Extension: " << extensions[0] << " failed!";

            walk = g_list_next(walk);
            continue;
        }
        QString factoryKlass(gst_element_factory_get_klass(elementFactory));
        QString factoryDescr(gst_element_factory_get_description(elementFactory));
        
        qDebug() << "Factory: " << factoryName << " Klass: " << factoryKlass << " Description: " << factoryDescr;
        
        caps = gst_type_find_factory_get_caps(findFactory);
        capsString = gst_caps_to_string(caps);
        GST_LOG("caps are %", GST_PTR_FORMAT, caps);
        
        g_free(capsString);
        
        walk = g_list_next(walk);
    }

    g_list_free(type_list);
*/

    if (!isCameraReady) {
        qDebug("GstreamerGrabber::initialization --> End (false)");

        return false;
    }

    this->isProcess = true;

    qDebug("GstreamerGrabber::initialization --> End (true)");

    return true;
}


bool GstreamerGrabber::init()
{
    qDebug() << "gstreamergrabber::init --> Start";

    GstBus *bus;

    gst_init(0,0);

    // GstreamerGrabber::loop = g_main_loop_new(NULL, FALSE);

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    switch (activeSource) {
    case TestSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: videotestsrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        source = gst_element_factory_make("videotestsrc", "source=videotestsrc");
        if (!source) {
            QDebug(QtFatalMsg) << "Can't create the source.";
            return false;
        }
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            QDebug(QtFatalMsg) << "Can't create the filter1.";
            return false;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            QDebug(QtFatalMsg) << "Can't create the filter2.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            QDebug(QtFatalMsg) << "Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        gst_bin_add_many(GST_BIN (pipeline), source, filter1, filter2, sink, NULL);

        if (!gst_element_link_many(source, filter1, filter2, sink, NULL)) {
            QDebug(QtFatalMsg) << "Can't link the elements.";
            return false;
        }

        break;
    case Video4LinuxSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: v4l2src ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        source = gst_element_factory_make("v4l2src", "source=v4l2src");
        if (!source) {
            QDebug(QtFatalMsg) << "Can't create the source.";
            return false;
        }
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            QDebug(QtFatalMsg) << "Can't create the filter1.";
            return false;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            QDebug(QtFatalMsg) << "Can't create the filter2.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            QDebug(QtFatalMsg) << "Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        gst_bin_add_many(GST_BIN (pipeline), source, filter1, filter2, sink, NULL);

        if (!gst_element_link_many(source, filter1, filter2, sink, NULL)) {
            QDebug(QtFatalMsg) << "Can't link the elements.";
            return false;
        }

        break;
    case Iee1394Source:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dv1394 port=0 ! video/x-dv ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        source = gst_element_factory_make("dv1394", "source=dv1394");
        if (!source) {
            QDebug(QtFatalMsg) << "Can't create the source.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "port", "0", NULL);

        filter1 = gst_element_factory_make("video/x-dv", "filter1=video/x-dv");
        if (!filter1) {
            QDebug(QtFatalMsg) << "Can't create the filter1.";
            return false;
        }

        filter2 = gst_element_factory_make("dvdemux", "filter2=dvdemux");
        if (!filter2) {
            QDebug(QtFatalMsg) << "Can't create the filter2.";
            return false;
        }

        filter3 = gst_element_factory_make("dvdec", "filter3=dvdec");
        if (!filter3) {
            QDebug(QtFatalMsg) << "Can't create the filter3.";
            return false;
        }

        filter4 = gst_element_factory_make("ffmpegcolorspace", "filter4=ffmpegcolorspace");
        if (!filter4) {
            QDebug(QtFatalMsg) << "Can't create the filter4.";
            return false;
        }

        filter5 = gst_element_factory_make("jpegenc", "filter5=jpegenc");
        if (!filter5) {
            QDebug(QtFatalMsg) << "Can't create the filter5.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            QDebug(QtFatalMsg) << "Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        gst_bin_add_many(GST_BIN (pipeline), source, filter1, filter2, filter3, filter4, filter5, sink, NULL);

        if (!gst_element_link_many(source, filter1, filter2, filter3, filter4, filter5, sink, NULL)) {
            QDebug(QtFatalMsg) << "Can't link the elements.";
            return false;
        }

        break;
    case DirectShowSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dshowvideosrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        source = gst_element_factory_make("dshowvideosrc", NULL); // "device-name=device_source" );
        if (!source) {
            QDebug(QtFatalMsg) << "Can't create the source.";
            return false;
        }
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            QDebug(QtFatalMsg) << "Can't create the filter1.";
            return false;
        }
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            QDebug(QtFatalMsg) << "Can't create the filter2.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            QDebug(QtFatalMsg) << "Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);

        gst_bin_add_many(GST_BIN (pipeline), source, filter1, filter2, sink, NULL);

        if (!gst_element_link_many(source, filter1, filter2, sink, NULL)) {
            QDebug(QtFatalMsg) << "Can't link the elements.";
            return false;
        }

        break;
    default:
        qDebug() << "gstreamergrabber::init --> Unknown source";

        return false;
    }

    qDebug() << "gstreamergrabber::init --> Check device count";

    if (TestSource != activeSource) {
        // TODO: Handle more than one device per source
        GstPropertyProbe* probe = GST_PROPERTY_PROBE(source);
        GValueArray* va = gst_property_probe_get_values_name(probe, "device-name");
        GValue* v0 = g_value_array_get_nth(va, 0);
        qDebug() << "gstreamergrabber::init --> device 0" << g_value_get_string(v0);
        if (va->n_values > 1) {
            // More than one device possible
            GValue* v = g_value_array_get_nth(va, 1);
            qDebug() << "gstreamergrabber::init --> device 1" << g_value_get_string(v);
        }
        // END: Handle more than one device per source
    }

    qDebug() << "gstreamergrabber::init --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // qDebug() << "gstreamergrabber::init --> run main loop";

    // g_main_loop_run(GstreamerGrabber::loop);

    qDebug() << "gstreamergrabber::init --> End";

    return true;
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
            QDebug(QtFatalMsg) << "Message: " << gerror->message;
            QDebug(QtFatalMsg) << "Debug: " << debug;
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
            QDebug(QtFatalMsg) << "Message: " << gerror->message;
            QDebug(QtFatalMsg) << "Debug: " << debug;
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
            QDebug(QtFatalMsg) << "Message: " << gerror->message;
            QDebug(QtFatalMsg) << "Debug: " << debug;
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
            // QDebug(QtFatalMsg) << "Element " << GST_OBJECT_NAME (message->src)
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
