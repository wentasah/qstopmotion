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

#ifndef GSTREAMER_GSTDEVICEGRABBER_H
#define GSTREAMER_GSTDEVICEGRABBER_H

#include "technical/grabber/imagegrabber.h"

// Include files of the gstreamer library
#include <gst/gst.h>


/**
 * Video grabbers using the gstreamer library and the Linux dv1394 source.
 *
 * @author Ralf Lange
 */
class GstreamerDeviceGrabber : public ImageGrabber
{
    Q_OBJECT
public:

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    GstreamerDeviceGrabber(Frontend *f);

    /**
     * Destructor
     */
    ~GstreamerDeviceGrabber();

    /**
     * Initialization of the Command line grabber
     * @param devices The vector of initialized devices.
     * @return true on success, false otherwise
     */
    bool initialization(QVector<ImageGrabberDevice*> &devices);

    /**
     * Starts the grabber if it is marked to be runned in deamon mode.
     * @return true on success, false otherwise
     */
    bool setUp();

    /**
     * Get the live image from the camera
     */
    const QImage getLiveImage();

    /**
     * Get the raw image from the camera
     */
    const QImage getRawImage();

    /**
     * Grabs one picture from the device.
     * @return true on success, false otherwise
     */
    bool grab();

    /**
     * Shut downs the grabber process either if it is runned in deamon
     * mode or "single grab" mode.
     * @return true on success, false otherwise
     */
    bool tearDown();

    /**
     * Call back function for the message loop of gstreamer.
     */
    static gboolean bus_callback(GstBus     *bus,
                                 GstMessage *message,
                                 gpointer    data);

    /**
     * Pad to select the video stream from the demux to the decoder
     */
    static void on_pad_added (GstElement *element,
                              GstPad     *pad,
                              gpointer    data);

    static void cb_typefound (GstElement *typefind,
                              guint       probability,
                              GstCaps    *caps,
                              gpointer    data);

    static gboolean link_elements_with_filter (GstElement *element1,
                                               GstElement *element2);

private:
    /**
     * Get the actual image from the gstreamer application interface
     */
    const QImage getImage();

private:
    int         activeSource;
    bool        isInitSuccess;
    bool        firstImage;

    GstElement *pipeline;
    GstElement *source;
    GstElement *filter1;
    GstElement *filter2;
    GstElement *filter3;
    GstElement *filter4;
    GstElement *queue1;
    GstElement *queue2;
    GstElement *queue3;
    GstElement *queue4;
    GstElement *sink;
    // GstBus     *bus;
    // GMainLoop  *loop;

    QString filePath;
    QImage liveImage;
    QImage rawImage;

};

#endif
