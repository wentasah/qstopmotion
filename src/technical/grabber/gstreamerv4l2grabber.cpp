/******************************************************************************
 *  Copyright (C) 2010-2012 by                                                *
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

#include "gstreamerv4l2grabber.h"

#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gstreamer library
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

#define APP_SINK_MAX_BUFFERS 2


GstreamerV4L2Grabber::GstreamerV4L2Grabber(Frontend *f)
    : GstreamerGrabber(f)
{
    qDebug("GstreamerV4L2Grabber::Constructor --> Start (Empty)");

    // qDebug("GstreamerV4L2Grabber::Constructor --> End");
}


GstreamerV4L2Grabber::~GstreamerV4L2Grabber()
{
    qDebug("GstreamerV4L2Grabber::Destructor --> Start (Empty)");

    // qDebug("GstreamerV4L2Grabber::Destructor --> End");
}


bool GstreamerV4L2Grabber::initializationSubclass(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GstreamerV4L2Grabber::initializationSubclass --> Start");

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

    qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Check devices of v4l2src";

    device_name = "v4l2src";
    property_name = "device-name";
    property_id = "device";

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    srcfactory = gst_element_factory_find(device_name);
    g_return_val_if_fail(srcfactory != NULL, false);
    src = gst_element_factory_create(srcfactory, "source");
    if (!src) {
        qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't get class interface.";
        gst_object_unref(GST_OBJECT(src));
        return false;
    }

    if (GST_IS_PROPERTY_PROBE(src)) {
        probe = (GstPropertyProbe*)GST_PROPERTY_PROBE(src);
        if (!probe) {
            qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't get probe interface.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_id = gst_property_probe_get_property(probe, property_id);
        if(!spec_id) {
            qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't get device spec.";
            gst_object_unref(GST_OBJECT(src));
        }

        values_id = gst_property_probe_probe_and_get_values(probe, spec_id);
        if(!values_id) {
            qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't get device id.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_name = gst_property_probe_get_property(probe, property_name);
        if(spec_name) {
            values_name = gst_property_probe_probe_and_get_values(probe, spec_name);
        }

        if (values_id != NULL) {
            qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Check device count";
            for (unsigned int i = 0 ; i < values_id->n_values ; i++) {
                // Handle the device id
                GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_id_pointer, &value_id_string)) {
                    qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return false;
                }
                // Add the device to the device list
                int device_size = devices.size();
                if (values_name != NULL) {
                    // Handle the device name
                    GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                    if (!g_value_transform(value_name_pointer, &value_name_string)) {
                        qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> Fatal: Can't copy string.";
                        gst_object_unref(GST_OBJECT(src));
                        return false;
                    }
                    device = new ImageGrabberDevice((const char*)g_value_get_string(&value_id_string),
                                                    (const char*)g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::video4LinuxSource,
                                                    ImageGrabberDevice::video_x_none);
                }
                else {
                    // No device name
                    device = new ImageGrabberDevice((const char*)g_value_get_string(&value_id_string),
                                                    QString(QApplication::translate("GstreamerV4L2Grabber", "Device %1")).arg(device_size),
                                                    ImageGrabberDevice::video4LinuxSource,
                                                    ImageGrabberDevice::video_x_none);
                 }
                // Add the device to the device list
                devices.append(device);
                qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                if (values_name != NULL) {
                    qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
                }
                else {
                    // No device name
                    qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (empty)";
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

    qDebug() << "GstreamerV4L2Grabber::initializationSubclass --> device count: " << devices.size();

    qDebug("GstreamerV4L2Grabber::initializationSubclass --> End (true)");

    return true;
}


bool GstreamerV4L2Grabber::initSubclass()
{
    qDebug() << "GstreamerV4L2Grabber::initSubclass --> Start";

    GstBus *bus;
    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = frontend->getDevice(videoSource);

    GstCaps *src_filter = 0;

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    qDebug() << "GstreamerV4L2Grabber::initSubclass --> Build the pipeline: v4l2src ! ffmpegcolorspace ! appsink";

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    source = gst_element_factory_make("v4l2src", NULL); // "source=v4l2src");
    if (!source) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't create the source.";
        return false;
    }
    // this property needs to be set before linking the element, where the device id configured in get_caps() */
    g_object_set (G_OBJECT(source), "device", videoDevice->getDeviceId().toAscii().constData(), NULL);

    filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
    if (!filter1) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't create the filter1.";
        return false;
    }
    sink = gst_element_factory_make("appsink", NULL);
    if (!sink) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't create the application sink.";
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
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't add the source to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't add the filter1 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), sink)) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't add the sink to the bin.";
        return false;
    }

    //---------------------------------------------------------------------
    // Link the elements in the bin
    //---------------------------------------------------------------------

    if (!gst_element_link(source, filter1)) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't link the filter1 to source.";
        return false;
    }
    if (!gst_element_link(filter1, sink)) {
        qDebug() << "GstreamerV4L2Grabber::initSubclass --> Fatal: Can't link the sink to filter1.";
        return false;
    }

    qDebug() << "GstreamerV4L2Grabber::initSubclass --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    qDebug() << "GstreamerV4L2Grabber::initSubclass --> End";

    return true;
}


bool GstreamerV4L2Grabber::tearDown()
{
    qDebug() << "GstreamerV4L2Grabber::tearDown --> Start";

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

    // g_main_loop_unref(GstreamerV4L2Grabber::loop);

    qDebug() << "GstreamerV4L2Grabber::tearDown --> End";

    return true;
}
