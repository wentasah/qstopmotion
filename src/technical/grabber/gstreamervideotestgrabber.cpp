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

#include "gstreamervideotestgrabber.h"

#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gstreamer library
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

#define APP_SINK_MAX_BUFFERS 2


GstreamerVideoTestGrabber::GstreamerVideoTestGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("GstreamerVideoTestGrabber::Constructor --> Start");

    isInitSuccess = false;
    firstImage = true;
    pipeline = 0;
    source = 0;
    filter1 = 0;
    sink = 0;

    gst_init(0,0);

    qDebug("GstreamerVideoTestGrabber::Constructor --> End");
}


GstreamerVideoTestGrabber::~GstreamerVideoTestGrabber()
{
    qDebug("GstreamerVideoTestGrabber::Destructor --> Start (Empty)");

    // qDebug("GstreamerVideoTestGrabber::Destructor --> End");
}


bool GstreamerVideoTestGrabber::initializationSubclass(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GstreamerVideoTestGrabber::initialization --> Start");

    ImageGrabberDevice *device = NULL;

    qDebug() << "GstreamerVideoTestGrabber::initialization --> Add video test device";

    device = new ImageGrabberDevice("",
                                    QApplication::translate("GstreamerVideoTestGrabber", "Video test device"),
                                    ImageGrabberDevice::testSource,
                                    ImageGrabberDevice::video_x_none);
    devices.append(device);

    qDebug() << "GstreamerVideoTestGrabber::initialization --> device count: " << devices.size();

    qDebug("GstreamerVideoTestGrabber::initialization --> End (true)");

    return true;
}


bool GstreamerVideoTestGrabber::initSubclass()
{
    qDebug() << "GstreamerVideoTestGrabber::init --> Start";

    GstBus  *bus;
    GstCaps *src_filter = 0;

    pipeline = gst_pipeline_new("video_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
    gst_bus_add_watch(bus, bus_callback, NULL);
    gst_object_unref(bus);

    qDebug() << "GstreamerVideoTestGrabber::init --> Build the pipeline: videotestsrc ! ffmpegcolorspace ! jpegenc ! multifilesink location=$IMAGEFILE";

    //---------------------------------------------------------------------
    // Create the elements
    //---------------------------------------------------------------------

    source = gst_element_factory_make("videotestsrc", "source=videotestsrc");
    if (!source) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't create the source.";
        return false;
    }
    // g_object_set(source, "pattern", 18, NULL);
    filter1 = gst_element_factory_make("ffmpegcolorspace", "filter1=ffmpegcolorspace");
    if (!filter1) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't create the filter1.";
        return false;
    }
    sink = gst_element_factory_make("appsink", NULL);
    if (!sink) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't create the application sink.";
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
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't add the source to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), filter1)) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't add the filter1 to the bin.";
        return false;
    }
    if (!gst_bin_add(GST_BIN (pipeline), sink)) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't add the sink to the bin.";
        return false;
    }

    //---------------------------------------------------------------------
    // Link the elements in the bin
    //---------------------------------------------------------------------

    src_filter = gst_caps_new_simple ("video/x-raw-yuv",
                                      // "width", G_TYPE_INT, 800,       // SVGA width
                                      // "height", G_TYPE_INT, 600,      // SVGA height
                                      "width", G_TYPE_INT, 640,       // VGA width
                                      "height", G_TYPE_INT, 480,      // VGA height
                                      // "width", G_TYPE_INT, 320,       // QVGA width
                                      // "height", G_TYPE_INT, 240,      // QVGA height
                                      "framerate", GST_TYPE_FRACTION, 15,
                                      1, NULL);

    // if (!gst_element_link(source, filter1)) {
    if (!gst_element_link_filtered (source, filter1, src_filter)) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't link the filter1 to source.";
        return false;
    }

    gst_caps_unref (src_filter);

    if (!gst_element_link(filter1, sink)) {
        qDebug() << "GstreamerVideoTestGrabber::init --> Fatal: Can't link the sink to the filter1.";
        return false;
    }

    qDebug() << "GstreamerVideoTestGrabber::init --> Start playing";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    qDebug() << "GstreamerVideoTestGrabber::init --> End";

    return true;
}


bool GstreamerVideoTestGrabber::tearDown()
{
    qDebug() << "GstreamerVideoTestGrabber::tearDown --> Start";

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

    // g_main_loop_unref(GstreamerVideoTestGrabber::loop);

    qDebug() << "GstreamerVideoTestGrabber::tearDown --> End";

    return true;
}


bool GstreamerVideoTestGrabber::grab()
{
    /*
    if (process.startDetached(prePoll) == false) {
        isInitSuccess = false;
        return false;
    }
    */
    return true;
}


const QImage GstreamerVideoTestGrabber::getImage()
{
    QImage image;

    GstBuffer *buffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
    GstCaps *caps = gst_buffer_get_caps(buffer);

    gint width, height;
    GstStructure *st = gst_caps_get_structure(caps, 0); // index);
    const gchar *imageType = gst_structure_get_name(st);

    if (firstImage) {
        const gchar *bufferCaps = gst_structure_to_string(st);
        qDebug() << "GstreamerVideoTestGrabber::getImage --> buffer caps: " << bufferCaps;
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
                    qDebug() << "GstreamerVideoTestGrabber::getImage --> format 'video/x-raw-yuv(YUY2)' not supported!!!";
                }
            }
            if (fourcc == GST_STR_FOURCC("YV12")) {
                // YUV 4:2:0 Planar, Uncompressed format commonly used im MPEG video processing.
                if (firstImage) {
                    qDebug() << "GstreamerVideoTestGrabber::getImage --> format 'video/x-raw-yuv(YV12)' not supported!!!";
                }
            }
        }
        else {
            if (0 == strcmp(imageType, "image/jpeg")) {
                if (firstImage) {
                    qDebug() << "GstreamerVideoTestGrabber::getImage --> format 'image/jpeg' not supported!!!";
                }
            }
            else {
                if (firstImage) {
                    qDebug() << "GstreamerVideoTestGrabber::getImage --> format '" << imageType << "' not supported!!!";
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


const QImage GstreamerVideoTestGrabber::getLiveImage()
{
    // liveImage.load(filePath);

    liveImage = getImage();

    return liveImage;
}


const QImage GstreamerVideoTestGrabber::getRawImage()
{
    // rawImage.load(filePath);

    rawImage = getImage();

    return rawImage;
}


gboolean GstreamerVideoTestGrabber::bus_callback(GstBus * /*bus*/, GstMessage *message, gpointer /*data*/)
{
    qDebug() << "GstreamerVideoTestGrabber::bus_callback --> Start";
    qDebug() << "Got " << GST_MESSAGE_TYPE_NAME (message) << " message";

    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS:
        qDebug() << "GstreamerVideoTestGrabber::bus_callback --> Message EOS";

        // g_main_loop_quit(GstreamerVideoTestGrabber::loop);
        break;
    case GST_MESSAGE_INFO:{
            qDebug() << "GstreamerVideoTestGrabber::bus_callback --> Message INFO";

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
            qDebug() << "GstreamerVideoTestGrabber::bus_callback --> Message WARNING";

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
            qDebug() << "GstreamerVideoTestGrabber::bus_callback --> Message ERROR";

            GError *gerror;
            gchar *debug;

            gst_message_parse_error (message, &gerror, &debug);
            gst_object_default_error (GST_MESSAGE_SRC (message), gerror, debug);
            qDebug() << "Message: " << gerror->message;
            qDebug() << "Debug: " << debug;
            g_error_free (gerror);
            g_free (debug);

            // g_main_loop_quit(GstreamerVideoTestGrabber::loop);
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

    qDebug() << "GstreamerVideoTestGrabber::bus_callback --> End";
    return true;
}
