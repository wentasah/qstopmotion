/******************************************************************************
 *  Copyright (C) 2010-2014 by                                                *
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

#include "directshowusbgrabber.h"
#include "directshowcontroller.h"
#include "technical/preferencestool.h"
#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gstreamer library
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

#define APP_SINK_MAX_BUFFERS 2


GstreamerDirectShowUsbGrabber::GstreamerDirectShowUsbGrabber(Frontend *f)
    : GstreamerGrabber(f)
{
    qDebug("GstreamerDirectShowUsbGrabber::Constructor --> Start (Empty)");

    // qDebug("GstreamerDirectShowUsbGrabber::Constructor --> End");
}


GstreamerDirectShowUsbGrabber::~GstreamerDirectShowUsbGrabber()
{
    qDebug("GstreamerDirectShowUsbGrabber::Destructor --> Start (Empty)");

    // qDebug("GstreamerDirectShowUsbGrabber::Destructor --> End");
}


bool GstreamerDirectShowUsbGrabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GstreamerDirectShowUsbGrabber::initialization --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

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
    DirectShowController *deviceController = NULL;

    device_size = devices.size();

    qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Check devices of dshowvideosrc";

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
        qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Fatal: Can't create the source.";
    }
    g_return_val_if_fail(src != NULL, false);

    klass = G_OBJECT_GET_CLASS(src);
    if(!g_object_class_find_property(klass, property_id)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Fatal: Can't get class interface.";
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
            qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Check device count";
            for (unsigned int i = 0 ; i < values_name->n_values ; i++) {
                // Handle the device name
                GValue* value_name_pointer = g_value_array_get_nth(values_name, i);
                GValue value_name_string = {0,};
                g_value_init(&value_name_string, G_TYPE_STRING);
                GValue value_id_string = {0,};
                g_value_init(&value_id_string, G_TYPE_STRING);
                if (!g_value_transform(value_name_pointer, &value_name_string)) {
                    qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Fatal: Can't copy string.";
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
                        qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Fatal: Can't copy string.";
                        gst_object_unref(GST_OBJECT(src));
                        return false;
                    }
                    device = new ImageGrabberDevice(0,
                                                    g_value_get_string(&value_id_string),
                                                    g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::directShowUsbSource,
                                                    ImageGrabberDevice::video_x_none);
                }
                else {
                    // No device id
                    device = new ImageGrabberDevice(0,
                                                    g_value_get_string(&value_name_string),
                                                    g_value_get_string(&value_name_string),
                                                    ImageGrabberDevice::directShow1394Source,
                                                    ImageGrabberDevice::video_x_none);
                }
                // Add the device to the device list
                devices.append(device);

                // Create grabber controller
                if (pref->getIntegerPreference("preferences", "gstreamerdirectshowusbcontroller", value) == false) {
                    value = false;
                }
                if ((int)true == value) {
                    deviceController = new DirectShowController(0);
                    if (deviceController->initialization(this, device))
                    {
                        device->setController(deviceController);

                        if (deviceController->setResolutions(device) == false) {
                            qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> Cannot enumerate resolutions (" << errno << ")";
                            // return false;
                        }

                    }
                    else
                    {
                        delete deviceController;
                        deviceController = NULL;
                    }
                }

                if (values_id != NULL) {
                    qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (" << g_value_get_string(&value_id_string) << ")";
                }
                else {
                    // No device id
                    qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> device id " << i << " '" << devices[device_size]->getDeviceId() << "' (empty)";
                }
                qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> device name " << i << " '" << devices[device_size]->getDeviceName() << "' (" << g_value_get_string(&value_name_string) << ")";
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

    qDebug() << "GstreamerDirectShowUsbGrabber::initialization --> device count: " << devices.size();

    qDebug("GstreamerDirectShowUsbGrabber::initialization --> End (true)");

    return true;
}


bool GstreamerDirectShowUsbGrabber::setUp()
{
    qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Start";

    int     imageWidth = 640;    // Default values are VGA
    int     imageHeight = 480;
    GstBus *bus;
    int     videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = frontend->getDevice(videoSource);

    GstCaps *src_filter = 0;

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Build the pipeline: dshowvideosrc ! ffmpegcolorspace ! appsink";

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
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't create the source.";
        return false;
    }
    // this property needs to be set before linking the element, where the device id configured in get_caps() */
    g_object_set (G_OBJECT(source), "device-name", videoDevice->getDeviceId().toAscii().constData(), NULL);
    filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
    if (!filter1) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't create the filter1.";
        return false;
    }
    /*
    filter2 = gst_element_factory_make("jpegenc", "filter2=jpegenc");
    if (!filter2) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't create the filter2.";
        return false;
    }
    */
    sink = gst_element_factory_make ("appsink", NULL);
    if (!sink) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't create the application sink.";
        return false;
    }
    gst_app_sink_set_max_buffers(GST_APP_SINK(sink), APP_SINK_MAX_BUFFERS);
    g_object_set(G_OBJECT(sink), "sync", FALSE, NULL);

    // Set default values for RGB.
    switch (frontend->getProject()->getImageSize()) {
    case ImageGrabber::qvgaSize:    // QVGA
        imageWidth = 320;
        imageHeight = 240;
        break;
    case ImageGrabber::vgaSize:     // VGA
        imageWidth = 640;
        imageHeight = 480;
        break;
    case ImageGrabber::svgaSize:    // SVGA
        imageWidth = 800;
        imageHeight = 600;
        break;
    case ImageGrabber::paldSize:    // PAL D
        imageWidth = 704;
        imageHeight = 576;
        break;
    case ImageGrabber::hdreadySize: // HD Ready
        imageWidth = 1280;
        imageHeight = 720;
        break;
    case ImageGrabber::fullhdSize:  // Full HD
        imageWidth = 1900;
        imageHeight = 1080;
        break;
    default: // Camera image size
        // imageWidth = imageWidth;
        // imageHeight = imageHeight;
        break;
    }

    // gst_app_sink_set_caps(GST_APP_SINK(sink), gst_caps_new_simple("video/x-raw-rgb", NULL));
    gst_app_sink_set_caps(GST_APP_SINK(sink), gst_caps_new_simple("video/x-raw-rgb",
                                                                  "width", G_TYPE_INT, imageWidth,
                                                                  "height", G_TYPE_INT, imageHeight,
                                                                  NULL));
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
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't add the source to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't add the filter1 to the bin.";
        return false;
    }
    /*
    if (!gst_bin_add(GST_BIN (pipeline), filter2)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't add the filter2 to the bin.";
        return false;
    }
    */
    if (!gst_bin_add(GST_BIN (pipeline), sink)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't add the sink to the bin.";
        return false;
    }

    //---------------------------------------------------------------------
    // Link the elements in the bin
    //---------------------------------------------------------------------
    /*
    src_filter = gst_caps_new_simple ("video/x-raw-yuv",
                                      // "width", G_TYPE_INT, 800,       // SVGA width
                                      // "height", G_TYPE_INT, 600,      // SVGA height
                                      "width", G_TYPE_INT, 640,       // VGA width
                                      "height", G_TYPE_INT, 480,      // VGA height
                                      // "width", G_TYPE_INT, 320,       // QVGA width
                                      // "height", G_TYPE_INT, 240,      // QVGA height
                                      "framerate", GST_TYPE_FRACTION, 15,
                                      1, NULL);

    if (!gst_element_link_filtered (source, filter1, src_filter)) {
        qDebug() << "GstreamerVideoTestGrabber::setUp --> Fatal: Can't link the filter1 to source.";
        return false;
    }

    gst_caps_unref (src_filter);
    */
    if (!gst_element_link(source, filter1)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't link the filter1 to source.";
        return false;
    }
    /*
    if (!gst_element_link(filter1, filter2)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't link the filter2 to filter1.";
        return false;
    }
    */
    if (!gst_element_link(filter1, sink)) {
        qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Fatal: Can't link the sink to filter1.";
        return false;
    }

    qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    qDebug() << "GstreamerDirectShowUsbGrabber::setUp --> End";

    return true;
}


bool GstreamerDirectShowUsbGrabber::tearDown()
{
    qDebug() << "GstreamerDirectShowUsbGrabber::tearDown --> Start";

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

    // g_main_loop_unref(GstreamerDirectShowUsbGrabber::loop);

    qDebug() << "GstreamerDirectShowUsbGrabber::tearDown --> End";

    return true;
}


void GstreamerDirectShowUsbGrabber::on_pad_added (GstElement *element,
                                     GstPad     *pad,
                                     gpointer    data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    qDebug() << "GstreamerDirectShowUsbGrabber::on_pad_added --> Start";

    /* We can now link this pad with the vorbis-decoder sink pad */
    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);

    qDebug() << "GstreamerDirectShowUsbGrabber::on_pad_added --> Start";
}


void GstreamerDirectShowUsbGrabber::cb_typefound (GstElement *typefind,
                                     guint       probability,
                                     GstCaps    *caps,
                                     gpointer    data)
{
    // GMainLoop *loop = data;
    gchar *type;

    qDebug() << "GstreamerDirectShowUsbGrabber::cb_typefound --> Start";

    type = gst_caps_to_string (caps);
    qDebug() << "Media type " << type << "found, probability " << probability;
    g_free (type);

    // since we connect to a signal in the pipeline thread context, we need
    // to set an idle handler to exit the main loop in the mainloop context.
    // Normally, your app should not need to worry about such things. */
    // g_idle_add (idle_exit_loop, loop);

    qDebug() << "GstreamerDirectShowUsbGrabber::cb_typefound --> End";
}


gboolean GstreamerDirectShowUsbGrabber::link_elements_with_filter (GstElement *element1, GstElement *element2)
{
    gboolean link_ok;
    GstCaps *caps;

    qDebug() << "GstreamerDirectShowUsbGrabber::link_elements_with_filter --> Start";

    caps = gst_caps_new_simple("video/x-dv",
                               "systemstream", G_TYPE_BOOLEAN, TRUE,
                               NULL);
    link_ok = gst_element_link_filtered (element1, element2, caps);
    gst_caps_unref (caps);
    if (!link_ok) {
        qDebug() << "GstreamerDirectShowUsbGrabber::link_elements_with_filter --> Failed to link element1 and element2!";
    }

    qDebug() << "GstreamerDirectShowUsbGrabber::link_elements_with_filter --> End";

    return link_ok;
}
