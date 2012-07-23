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
    : GstreamerGrabber(f)
{
    qDebug("GstreamerVideoTestGrabber::Constructor --> Start (Empty)");

    // qDebug("GstreamerVideoTestGrabber::Constructor --> End");
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
