/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#include "dv1394grabber.h"

#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gstreamer library
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

#define APP_SINK_MAX_BUFFERS 2


GstreamerDv1394Grabber::GstreamerDv1394Grabber(Frontend *f)
    : GstreamerGrabber(f)
{
    qDebug("GstreamerDv1394Grabber::Constructor --> Start");

    filter3 = 0;

    qDebug("GstreamerDv1394Grabber::Constructor --> End");
}


GstreamerDv1394Grabber::~GstreamerDv1394Grabber()
{
    qDebug("GstreamerDv1394Grabber::Destructor --> Start (Empty)");

    // qDebug("GstreamerDv1394Grabber::Destructor --> End");
}


bool GstreamerDv1394Grabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GstreamerDv1394Grabber::initialization --> Start");

    const gchar *device_name = NULL;
    GstElementFactory *srcfactory = NULL;
    GstElement *src = NULL;
    GObjectClass *klass = NULL;
    GstPropertyProbe *probe = NULL;
    const gchar *property_name = NULL;
    const gchar *property_id = NULL;
    const GParamSpec *spec_name = NULL;
    const GParamSpec *spec_id = NULL;
    GValueArray *values_name = NULL;
    GValueArray *values_id = NULL;
    int device_size;
    ImageGrabberDevice *device = NULL;

    device_size = devices.size();

    qDebug() << "GstreamerDv1394Grabber::initialization --> Check devices of dv1394src";

    device_name = "dv1394src";
    property_name = "device-name";
    property_id = "guid";

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    srcfactory = gst_element_factory_find(device_name);
    g_return_val_if_fail(srcfactory != NULL, false);
    src = gst_element_factory_create(srcfactory, "source");
    if (!src) {
        qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't get class interface.";
        gst_object_unref(GST_OBJECT(src));
        return false;
    }

    if (GST_IS_PROPERTY_PROBE(src)) {
        probe = (GstPropertyProbe*)GST_PROPERTY_PROBE(src);
        if (!probe) {
            qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't get probe interface.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_id = gst_property_probe_get_property(probe, property_id);
        if(!spec_id) {
            qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't get device spec.";
            gst_object_unref(GST_OBJECT(src));
        }

        values_id = gst_property_probe_probe_and_get_values(probe, spec_id);
        if(!values_id) {
            qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't get device id.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_name = gst_property_probe_get_property(probe, property_name);
        if(spec_name) {
            values_name = gst_property_probe_probe_and_get_values(probe, spec_name);
        }

        if (values_id != NULL) {
            qDebug() << "GstreamerDv1394Grabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_id->n_values ; i++) {
                // Handle the device id
                GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_id_pointer, &value_id_string)) {
                    qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return false;
                }
                int device_size = devices.size();
                if (values_name != NULL) {
                    // Handle the device name
                    GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                    if (!g_value_transform(value_name_pointer, &value_name_string)) {
                        qDebug() << "GstreamerDv1394Grabber::initialization --> Fatal: Can't copy string.";
                        gst_object_unref(GST_OBJECT(src));
                        return false;
                    }
                    device = new ImageGrabberDevice(0,
                                                    QLatin1String((const char*)g_value_get_string(&value_id_string)),
                                                    QLatin1String((const char*)g_value_get_string(&value_name_string)),
                                                    ImageGrabberDevice::ieee1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                else {
                    // No device name
                    device = new ImageGrabberDevice(0,
                                                    QLatin1String((const char*)g_value_get_string(&value_id_string)),
                                                    QString(QApplication::translate("GstreamerDv1394Grabber", "Device %1")).arg(device_size),
                                                    ImageGrabberDevice::ieee1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                // Add the device to the device list
                devices.append(device);
                qDebug() << "GstreamerDv1394Grabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                if (values_name != NULL) {
                    qDebug() << "GstreamerDv1394Grabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
                }
                else {
                    // No device name
                    qDebug() << "GstreamerDv1394Grabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (empty)";
                }
            }
        }
        if (values_name != NULL) {
            g_value_array_free(values_name);
        }
        if (values_id != NULL) {
            g_value_array_free(values_id);
        }
    }

    // Reset source object
    // gst_object_unref(GST_OBJECT(src));

    qDebug() << "GstreamerDv1394Grabber::initialization --> device count: " << devices.size();

    qDebug("GstreamerDv1394Grabber::initialization --> End (true)");

    return true;
}


bool GstreamerDv1394Grabber::setUp()
{
    qDebug() << "GstreamerDv1394Grabber::setUp --> Start";

    GstBus *bus;
    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = frontend->getDevice(videoSource);

    GstCaps *src_filter = 0;

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    qDebug() << "GstreamerDv1394Grabber::setUp --> Build the pipeline: dv1394src ! queue ! dvdemux ! queue ! dvdec ! ffmpegcolorspace ! appsink";

    // Examples:
    // gst-launch dv1394src ! video/x-dv ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
    // gst-launch dv1394src ! ffdemux_dv ! ffdec_dvvideo ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
    // gst-launch dv1394src ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    source = gst_element_factory_make("dv1394src", "source=dv1394src");
    if (!source) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the source.";
        return false;
    }
    // this property needs to be set before linking the element, where the device id configured in get_caps() */
    g_object_set(G_OBJECT(source), "guid", g_ascii_strtoull(videoDevice->getDeviceId().toAscii().constData(), NULL, 0), NULL);
    // g_object_set(G_OBJECT(source), "port", 0, NULL);

    filter1 = gst_element_factory_make("dvdemux", "filter1=dvdemux");
    if (!filter1) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the filter1.";
        return false;
    }
    g_object_set(G_OBJECT(filter1), "name", "d", NULL);
    queue1 = gst_element_factory_make("queue", "queue1=queue");
    if (!queue1) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the queue1.";
        return false;
    }
    filter2 = gst_element_factory_make("dvdec", "filter2=dvdec");
    if (!filter2) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the filter2.";
        return false;
    }
    queue2 = gst_element_factory_make("queue", "queue2=queue");
    if (!queue1) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the queue1.";
        return false;
    }
    filter3 = gst_element_factory_make("ffmpegcolorspace", "filter4=ffmpegcolorspace");
    if (!filter3) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the filter3.";
        return false;
    }
    sink = gst_element_factory_make("appsink", NULL);
    if (!sink) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't create the application sink.";
        return false;
    }
    gst_app_sink_set_max_buffers(GST_APP_SINK(sink), APP_SINK_MAX_BUFFERS);
    g_object_set(G_OBJECT(sink), "sync", FALSE, NULL);

    // Set default values for RGB.
    gst_app_sink_set_caps(GST_APP_SINK(sink), gst_caps_new_simple("video/x-raw-rgb", NULL));
    // The result on Windows is:
    // video/x-raw-rgb, width=(int)320, height=(int)240, framerate=(fraction)30/1, bpp=(int)24, depth=(int)24,
    // red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321

    // Set special values for RGB
    // #define SINK_CAPS "video/x-raw-rgb, width=(int)320, height=(int)300, framerate=(fraction)30/1, bpp=(int)24, depth=(int)24"
    // gst_app_sink_set_caps((GstAppSink*)sink, gst_caps_from_string(SINK_CAPS));

    // Set special values for YUV
    // #define SINK_CAPS "video/x-raw-yuv, format=(fourcc)UYVY, width=(int)320, height=(int)300" //, framerate=(fraction)45/1"
    // gst_app_sink_set_caps((GstAppSink*)sink, gst_caps_from_string(SINK_CAPS));

    //---------------------------------------------------------------------
    // Add the elements to the bin
    //---------------------------------------------------------------------

    if (!gst_bin_add(GST_BIN (pipeline), source)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the source to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the filter1 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), queue1)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the queue1 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the filter2 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), queue2)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the queue2 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter3)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the filter3 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), sink)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't add the sink to the bin.";
        return false;
    }

    //---------------------------------------------------------------------
    // Link the elements in the bin
    //---------------------------------------------------------------------

    if (!gst_element_link(source, filter1)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't link the filter1 to source.";
        return false;
    }
    if (!gst_element_link(filter2, filter3)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't link the filter3.";
        return false;
    }
    if (!gst_element_link(filter3, sink)) {
        qDebug() << "GstreamerDv1394Grabber::setUp --> Fatal: Can't link the sink to filter3.";
        return false;
    }
    // Connect filter1 and filter2 via a pad
    // The reason is, that the demultiplexer can handle 2 streams (video and audio)
    g_signal_connect(filter1,"pad-added",G_CALLBACK(on_pad_added),filter2);

    qDebug() << "GstreamerDv1394Grabber::setUp --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    qDebug() << "GstreamerDv1394Grabber::setUp --> End";

    return true;
}


bool GstreamerDv1394Grabber::tearDown()
{
    qDebug() << "GstreamerDv1394Grabber::tearDown --> Start";

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

    // g_main_loop_unref(GstreamerDv1394Grabber::loop);

    qDebug() << "GstreamerDv1394Grabber::tearDown --> End";

    return true;
}


void GstreamerDv1394Grabber::on_pad_added (GstElement *element,
                                     GstPad     *pad,
                                     gpointer    data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    qDebug() << "GstreamerDv1394Grabber::on_pad_added --> Start";

    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);

    qDebug() << "GstreamerDv1394Grabber::on_pad_added --> Start";
}


void GstreamerDv1394Grabber::cb_typefound (GstElement *typefind,
                                     guint       probability,
                                     GstCaps    *caps,
                                     gpointer    data)
{
    // GMainLoop *loop = data;
    gchar *type;

    qDebug() << "GstreamerDv1394Grabber::cb_typefound --> Start";

    type = gst_caps_to_string (caps);
    qDebug() << "Media type " << type << "found, probability " << probability;
    g_free (type);

    // since we connect to a signal in the pipeline thread context, we need
    // to set an idle handler to exit the main loop in the mainloop context.
    // Normally, your app should not need to worry about such things. */
    // g_idle_add (idle_exit_loop, loop);

    qDebug() << "GstreamerDv1394Grabber::cb_typefound --> End";
}
