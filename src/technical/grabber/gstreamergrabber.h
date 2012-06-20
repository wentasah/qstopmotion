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

#ifndef GSTREAMERGRABBER_H
#define GSTREAMERGRABBER_H

#include "imagegrabber.h"

#include <QtCore/QProcess>

// Include files of the gstreamer library
#include <gst/gst.h>


/**
 * Abstract class for the different video grabbers used by the VideoView
 * widgets.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class GstreamerGrabber : public ImageGrabber
{
    Q_OBJECT
public:

    /**
     * Name of the captured file
     */
    static const QString capturedFileName;

    /**
     * Suffix of the captured file
     */
    static const QString capturedFileSuffix;

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    GstreamerGrabber(Frontend *f);

    /**
     * Destructor
     */
    ~GstreamerGrabber();

    /**
     * Initialization of the Command line grabber
     *
    void initialization();
*/
    /**
     * Initialization of the Command line grabber
     */
    void initializationSubclass();

    /**
     * Starts the grabber if it is marked to be runned in deamon mode.
     * @return true on success, false otherwise
     *
    void init();
*/
    /**
     * Starts the grabber if it is marked to be runned in deamon mode.
     * @return true on success, false otherwise
     */
    void initSubclass();

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
     * Remove all capture files
     */
    void removeCaptureFiles();

    const QImage getImage();

private:
    // QProcess process;
    ImageGrabberDevice::imageGrabberVideoSources activeSource;
    bool        isInitSuccess;
    bool        firstImage;

    GstElement *pipeline;
    GstElement *source;
    GstElement *filter1;
    GstElement *filter2;
    GstElement *filter3;
    GstElement *filter4;
    GstElement *filter5;
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
