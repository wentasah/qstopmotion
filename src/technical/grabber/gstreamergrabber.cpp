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

#include "gstreamergrabber.h"

#include "technical/grabber/imagegrabberdevice.h"
#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gstreamer library
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

#define APP_SINK_MAX_BUFFERS 2

const QString GstreamerGrabber::capturedFileName = QLatin1String("capturedfile");
const QString GstreamerGrabber::capturedFileSuffix = QLatin1String("jpg");


GstreamerGrabber::GstreamerGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("GstreamerGrabber::Constructor --> Start");

    isInitSuccess = false;
    firstImage = true;
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

    QString rootDir;

    rootDir.append(frontend->getUserDirName());
    rootDir.append("/");
    rootDir.append(capturedFileName);
    rootDir.append(".");
    rootDir.append(capturedFileSuffix);

    filePath.append(rootDir);

    qDebug("GstreamerGrabber::Constructor --> End");
}


GstreamerGrabber::~GstreamerGrabber()
{
    qDebug("GstreamerGrabber::Destructor --> Start");

    removeCaptureFiles();

    qDebug("GstreamerGrabber::Destructor --> End");
}


bool GstreamerGrabber::initializationSubclass(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GstreamerGrabber::initialization --> Start");

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

#ifdef Q_WS_X11
    qDebug() << "gstreamergrabber::initialization --> Check devices of v4l2src";

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
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get class interface.";
        gst_object_unref(GST_OBJECT(src));
        return false;
    }

    if (GST_IS_PROPERTY_PROBE(src)) {
        probe = (GstPropertyProbe*)GST_PROPERTY_PROBE(src);
        if (!probe) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get probe interface.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_id = gst_property_probe_get_property(probe, property_id);
        if(!spec_id) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get device spec.";
            gst_object_unref(GST_OBJECT(src));
        }

        values_id = gst_property_probe_probe_and_get_values(probe, spec_id);
        if(!values_id) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get device id.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_name = gst_property_probe_get_property(probe, property_name);
        if(spec_name) {
            values_name = gst_property_probe_probe_and_get_values(probe, spec_name);
        }

        if (values_id != NULL) {
            qDebug() << "gstreamergrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_id->n_values ; i++) {
                // Handle the device id
                GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_id_pointer, &value_id_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return false;
                }
                // Add the device to the device list
                int device_size = devices.size();
                if (values_name != NULL) {
                    // Handle the device name
                    GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                    if (!g_value_transform(value_name_pointer, &value_name_string)) {
                        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
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
                                                    QString(QApplication::translate("GstreamerGrabber", "Device %1")).arg(device_size),
                                                    ImageGrabberDevice::video4LinuxSource,
                                                    ImageGrabberDevice::video_x_none);
                 }
                // Add the device to the device list
                devices.append(device);
                qDebug() << "gstreamergrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                if (values_name != NULL) {
                    qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
                }
                else {
                    // No device name
                    qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (empty)";
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

#endif

#ifdef Q_WS_X11
    qDebug() << "gstreamergrabber::initialization --> Check devices of dv1394src";

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
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get class interface.";
        gst_object_unref(GST_OBJECT(src));
        return false;
    }

    if (GST_IS_PROPERTY_PROBE(src)) {
        probe = (GstPropertyProbe*)GST_PROPERTY_PROBE(src);
        if (!probe) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get probe interface.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_id = gst_property_probe_get_property(probe, property_id);
        if(!spec_id) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get device spec.";
            gst_object_unref(GST_OBJECT(src));
        }

        values_id = gst_property_probe_probe_and_get_values(probe, spec_id);
        if(!values_id) {
            qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get device id.";
            gst_object_unref(GST_OBJECT(src));
        }

        spec_name = gst_property_probe_get_property(probe, property_name);
        if(spec_name) {
            values_name = gst_property_probe_probe_and_get_values(probe, spec_name);
        }

        if (values_id != NULL) {
            qDebug() << "gstreamergrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_id->n_values ; i++) {
                // Handle the device id
                GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                if (!g_value_transform(value_id_pointer, &value_id_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return false;
                }
                int device_size = devices.size();
                if (values_name != NULL) {
                    // Handle the device name
                    GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                    if (!g_value_transform(value_name_pointer, &value_name_string)) {
                        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                        gst_object_unref(GST_OBJECT(src));
                        return false;
                    }
                    device = new ImageGrabberDevice((const char*)g_value_get_string(&value_id_string),
                                                    (const char*)g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::iee1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                else {
                    // No device name
                    device = new ImageGrabberDevice((const char*)g_value_get_string(&value_id_string),
                                                    QString(QApplication::translate("GstreamerGrabber", "Device %1")).arg(device_size),
                                                    ImageGrabberDevice::iee1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                // Add the device to the device list
                devices.append(device);
                qDebug() << "gstreamergrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                if (values_name != NULL) {
                    qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
                }
                else {
                    // No device name
                    qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (empty)";
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

#endif

#ifdef Q_WS_WIN
    qDebug() << "gstreamergrabber::initialization --> Check devices of dshowvideosrc";

    device_name = "dshowvideosrc";
    property_name = "device-name";
    property_id = "device";

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    srcfactory = gst_element_factory_find(device_name);
    g_return_val_if_fail(srcfactory != NULL, false);
    src = gst_element_factory_create(srcfactory, "source");
    if (!src) {
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't get class interface.";
        gst_object_unref(GST_OBJECT(src));
        return false;
    }

    if (GST_IS_PROPERTY_PROBE(src)) {
        probe = (GstPropertyProbe*)GST_PROPERTY_PROBE(src);

        spec_id = gst_property_probe_get_property(probe, "device");
        if(spec_id) {
            values_id = gst_property_probe_probe_and_get_values(probe, spec_id);
        }

        spec_name = gst_property_probe_get_property(probe, "device-name");
        if(spec_name) {
            values_name = gst_property_probe_probe_and_get_values(probe, spec_name);
        }

        if (values_name != NULL) {
            qDebug() << "gstreamergrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_name->n_values ; i++) {
                // Handle the device name
                GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                if (!g_value_transform(value_name_pointer, &value_name_string)) {
                    qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                    gst_object_unref(GST_OBJECT(src));
                    return false;
                }
                int device_size = devices.size();
                if (values_id != NULL) {
                    // Handle the device id
                    GValue* value_id_pointer = g_value_array_get_nth(values_id, i);
                    GValue value_id_string = {0,};
                    g_value_init(&value_id_string, G_TYPE_STRING);
                    GValue value_name_string = {0,};
                    g_value_init(&value_name_string, G_TYPE_STRING);
                    if (!g_value_transform(value_id_pointer, &value_id_string)) {
                        qDebug() << "GstreamerGrabber::initialization --> Fatal: Can't copy string.";
                        gst_object_unref(GST_OBJECT(src));
                        return false;
                    }
                    device = new ImageGrabberDevice(g_value_get_string(&value_id_string),
                                                    g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::directShowUsbSource,
                                                    ImageGrabberDevice::video_x_none);
                }
                else {
                    // No device id
                    device = new ImageGrabberDevice(g_value_get_string(&value_name_string),
                                                    g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::directShow1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                // Add the device to the device list
                devices.append(device);
                if (values_id != NULL) {
                    qDebug() << "gstreamergrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                }
                else {
                    // No device id
                    qDebug() << "gstreamergrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (empty)";
                }
                qDebug() << "gstreamergrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
            }
        }
        if (values_id != NULL) {
            g_value_array_free(values_id);
        }
        if (values_name != NULL) {
            g_value_array_free(values_name);
        }
    }

    // Floating referenz counter of source object is 0 ?????
    // Reset source object
    // gst_object_unref(GST_OBJECT(src));

#endif

    qDebug() << "gstreamergrabber::initialization --> device count: " << devices.size();

    qDebug("GstreamerGrabber::initialization --> End (true)");

    return true;
}


bool GstreamerGrabber::initSubclass()
{
    qDebug() << "gstreamergrabber::init --> Start";

    GstBus *bus;
    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = frontend->getDevice(videoSource);

    GstCaps *src_filter = 0;

    // gst_init(0,0);

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::testSource:
        qDebug() << "gstreamergrabber::init --> Test source not supported";

        return false;
    case ImageGrabberDevice::video4LinuxSource:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: v4l2src ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        //---------------------------------------------------------------------
        // Create the elements
        //---------------------------------------------------------------------

        source = gst_element_factory_make("v4l2src", NULL); // "source=v4l2src");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return false;
        }
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        g_object_set (G_OBJECT(source), "device", videoDevice->getDeviceId().toAscii().constData(), NULL);

        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return false;
        }
        /*
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);
        */
        sink = gst_element_factory_make("appsink", NULL);
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the application sink.";
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
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return false;
        }
        /*
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return false;
        }
        */
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return false;
        }

        //---------------------------------------------------------------------
        // Link the elements in the bin
        //---------------------------------------------------------------------

        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return false;
        }
        /*
        if (!gst_element_link(filter1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2 to filter1.";
            return false;
        }
        if (!gst_element_link(filter2, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter2.";
            return false;
        }
        */
        if (!gst_element_link(filter1, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter1.";
            return false;
        }

        break;
    case ImageGrabberDevice::iee1394Source:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dv1394src ! queue ! dvdemux ! queue ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        // Examples:
        // gst-launch dv1394src ! video/x-dv ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dv1394src ! ffdemux_dv ! ffdec_dvvideo ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dv1394src ! dvdemux ! dvdec ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE

        //---------------------------------------------------------------------
        // Create the elements
        //---------------------------------------------------------------------

        source = gst_element_factory_make("dv1394src", "source=dv1394src");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return false;
        }
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        g_object_set(G_OBJECT(source), "guid", g_ascii_strtoull(videoDevice->getDeviceId().toAscii().constData(), NULL, 0), NULL);
        // g_object_set(G_OBJECT(source), "port", 0, NULL);

        filter1 = gst_element_factory_make("dvdemux", "filter1=dvdemux");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return false;
        }
        g_object_set(G_OBJECT(filter1), "name", "d", NULL);
        queue1 = gst_element_factory_make("queue", "queue1=queue");
        if (!queue1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the queue1.";
            return false;
        }
        filter2 = gst_element_factory_make("dvdec", "filter2=dvdec");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return false;
        }
        queue2 = gst_element_factory_make("queue", "queue2=queue");
        if (!queue1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the queue1.";
            return false;
        }
        /*
        filter3 = gst_element_factory_make("ffdeinterlace", "filter3=ffdeinterlace");
        if (!filter3) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter3.";
            return false;
        }
        */
        filter4 = gst_element_factory_make("ffmpegcolorspace", "filter4=ffmpegcolorspace");
        if (!filter4) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter4.";
            return false;
        }
        /*
        filter5 = gst_element_factory_make("jpegenc", "filter5=jpegenc");
        if (!filter5) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter5.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);
        */
        sink = gst_element_factory_make("appsink", NULL);
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the application sink.";
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
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), queue1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the queue1 to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), queue2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the queue1 to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter4)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return false;
        }
        /*
        if (!gst_bin_add(GST_BIN (pipeline), filter5)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return false;
        }
        */
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return false;
        }

        //---------------------------------------------------------------------
        // Link the elements in the bin
        //---------------------------------------------------------------------

        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return false;
        }
        /*
        if (!gst_element_link(filter1, queue1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the queue1.";
            return false;
        }

        if (!gst_element_link(queue1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2.";
            return false;
        }
        if (!gst_element_link(filter2, filter3)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter3.";
            return false;
        }
        if (!gst_element_link(filter2, queue2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the queue1.";
            return false;
        }
        */
        if (!gst_element_link(filter2, filter4)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter4.";
            return false;
        }
        /*
        if (!gst_element_link(filter4, filter5)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter5.";
            return false;
        }
        if (!gst_element_link(filter5, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter5.";
            return false;
        }
        */
        if (!gst_element_link(filter4, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter4.";
            return false;
        }
        // Connect filter1 and filter2 via a pad
        // The reason is, that the demultiplexer can handle 2 streams (video and audio)
        g_signal_connect(filter1,"pad-added",G_CALLBACK(on_pad_added),filter2);

        break;
    case ImageGrabberDevice::directShowUsbSource:
    case ImageGrabberDevice::directShow1394Source:
        qDebug() << "gstreamergrabber::init --> Build the pipeline: dshowvideosrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

        // Examples Web-Cam:
        // gst-launch dshowvideosrc ! video/x-raw-rpg ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // gst-launch dshowvideosrc ! video/x-raw-yuv ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE

        // Examples FireWire:
        // gst-launch dshowvideosrc ! video/x-dv ! ffdemux_dv ! ffdec_dvvideo ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE
        // R.L.: Direct Show Video Source didn't work with FireWire (IEE1394) devices.
        // The application hangs when linking dshowvideosrc and ffdemux_dv.

        // Other Examples:
        // gst-launch ksvideosrc ! ...

        //---------------------------------------------------------------------
        // Create the elements
        //---------------------------------------------------------------------

        source = gst_element_factory_make("dshowvideosrc", "source=dshowvideosrc");
        if (!source) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the source.";
            return false;
        }
        // this property needs to be set before linking the element, where the device id configured in get_caps() */
        g_object_set (G_OBJECT(source), "device-name", videoDevice->getDeviceId().toAscii().constData(), NULL);
        filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
        if (!filter1) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter1.";
            return false;
        }
        /*
        filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
        if (!filter2) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the filter2.";
            return false;
        }
        sink = gst_element_factory_make("multifilesink", "sink=multifilesink");
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the sink.";
            return false;
        }
        g_object_set(G_OBJECT (sink), "location", filePath.toLatin1().constData(), NULL);
        */
        sink = gst_element_factory_make ("appsink", NULL);
        if (!sink) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't create the application sink.";
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
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the source to the bin.";
            return false;
        }
        if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter1 to the bin.";
            return false;
        }
        /*
        if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the filter2 to the bin.";
            return false;
        }
        */
        if (!gst_bin_add(GST_BIN (pipeline), sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't add the sink to the bin.";
            return false;
        }

        //---------------------------------------------------------------------
        // Link the elements in the bin
        //---------------------------------------------------------------------

        if (!gst_element_link(source, filter1)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter1 to source.";
            return false;
        }
        /*
        if (!gst_element_link(filter1, filter2)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the filter2 to filter1.";
            return false;
        }
        if (!gst_element_link(filter2, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter2.";
            return false;
        }
        */
        if (!gst_element_link(filter1, sink)) {
            qDebug() << "gstreamergrabber::init --> Fatal: Can't link the sink to filter1.";
            return false;
        }
        break;
    default:
        qDebug() << "gstreamergrabber::init --> Unknown source";

        return false;
    }

    qDebug() << "gstreamergrabber::init --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

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


const QImage GstreamerGrabber::getImage()
{
    QImage image;

    GstBuffer *buffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
    GstCaps *caps = gst_buffer_get_caps(buffer);

    gint width, height;
    GstStructure *st = gst_caps_get_structure(caps, 0); // index);
    const gchar *imageType = gst_structure_get_name(st);

    if (firstImage) {
        const gchar *bufferCaps = gst_structure_to_string(st);
        qDebug() << "gstreamergrabber::getImage --> buffer caps: " << bufferCaps;
    }

    gst_structure_get_int(st, "width", &width);
    gst_structure_get_int(st, "height", &height);

    if (0 == strcmp(imageType, "video/x-raw-rgb")) {
        // video/x-raw-rgb, format=RGB3
        // video/x-raw-rgb, format=BGR3

        guchar *rgbData;
        QImage::Format format = QImage::Format_Invalid;
        gint bpp;
        gst_structure_get_int(st, "bpp", &bpp);

        if (bpp == 24)
            format = QImage::Format_RGB888;
        else if (bpp == 32)
            format = QImage::Format_RGB32;

        if (format != QImage::Format_Invalid) {
            rgbData = GST_BUFFER_DATA (buffer); //image_data is a static unsigned char *
            image = QImage((const uchar *)rgbData,
                           width,
                           height,
                           format);
            image.bits(); //detach
        }
    }
    else {
        if (0 == strcmp(imageType, "video/x-raw-yuv")) {
            guchar *yuvData;
            guint32 fourcc;
            gst_structure_get_fourcc(st, "format", &fourcc);
            if (fourcc == GST_STR_FOURCC("I420")) {
                // YUV ... Planar
                // Identical to IYUV
                // Identical to YV12 expect that the U and V plane oder is reversed

                yuvData = GST_BUFFER_DATA (buffer); //image_data is a static unsigned char *
                // rgbData = new unsigned char[width*height*2];
                // uyvy_to_rgb24(yuvData, rgbData, width, height);

                image = QImage(width/2, height/2, QImage::Format_RGB32);

                for (int y=0; y<height; y+=2) {
                    const uchar *yLine = yuvData + y*width;
                    const uchar *uLine = yuvData + width*height + y*width/4;
                    const uchar *vLine = yuvData + width*height*5/4 + y*width/4;

                    for (int x=0; x<width; x+=2) {
                        const qreal Y = 1.164*(yLine[x]-16);
                        const int U = uLine[x/2]-128;
                        const int V = vLine[x/2]-128;

                        int b = qBound(0, int(Y + 2.018*U), 255);
                        int g = qBound(0, int(Y - 0.813*V - 0.391*U), 255);
                        int r = qBound(0, int(Y + 1.596*V), 255);

                        image.setPixel(x/2,y/2,qRgb(r,g,b));
                    }
                }
            }
            if (fourcc == GST_STR_FOURCC("YUY2")) {
                // YUV 4:2:2 Packed
                if (firstImage) {
                    qDebug() << "gstreamergrabber::getImage --> format 'video/x-raw-yuv(YUY2)' not supported!!!";
                }
            }
            if (fourcc == GST_STR_FOURCC("YV12")) {
                // YUV 4:2:0 Planar, Uncompressed format commonly used im MPEG video processing.
                if (firstImage) {
                    qDebug() << "gstreamergrabber::getImage --> format 'video/x-raw-yuv(YV12)' not supported!!!";
                }
            }
        }
        else {
            if (0 == strcmp(imageType, "image/jpeg")) {
                if (firstImage) {
                    qDebug() << "gstreamergrabber::getImage --> format 'image/jpeg' not supported!!!";
                }
            }
            else {
                if (firstImage) {
                    qDebug() << "gstreamergrabber::getImage --> format '" << imageType << "' not supported!!!";
                }
            }
        }
    }

    gst_caps_unref(caps);
    gst_buffer_unref(buffer); //If I comment out this the application stops crashing, but fills up the memory

    if (firstImage) {
        firstImage = false;
    }

    Q_ASSERT(image.isNull() == false);

    return image;
}


const QImage GstreamerGrabber::getLiveImage()
{
    // liveImage.load(filePath);

    liveImage = getImage();

    return liveImage;
}


const QImage GstreamerGrabber::getRawImage()
{
    // rawImage.load(filePath);

    rawImage = getImage();

    return rawImage;
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


void GstreamerGrabber::removeCaptureFiles()
{
    bool ret;

    QDir homeDir(frontend->getUserDirName());
    QStringList nameFilter;
    QString fileName(capturedFileName);
    fileName.append(".");
    fileName.append(capturedFileSuffix);
    nameFilter.append(fileName);

    homeDir.setNameFilters(nameFilter);
    homeDir.setFilter(QDir::Files);
    QStringList fileNames = homeDir.entryList();
    if (fileNames.count() > 0) {
        ret = homeDir.remove(fileNames[0]);
        Q_ASSERT(ret == true);
    }
}
