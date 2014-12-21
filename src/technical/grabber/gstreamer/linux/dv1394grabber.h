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

#ifndef GSTREAMER_LINUX_DV1394GRABBER_H
#define GSTREAMER_LINUX_DV1394GRABBER_H

#include "technical/grabber/imagegrabberdevice.h"
#include "technical/grabber/gstreamer/gstgrabber.h"


// Include files of the gstreamer library
#include <gst/gst.h>


/**
 * Video grabbers using the dv1394 library of Linux.
 *
 * @author Ralf Lange
 */
class GstreamerDv1394Grabber : public GstreamerGrabber
{
    Q_OBJECT
public:

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    GstreamerDv1394Grabber(Frontend *f);

    /**
     * Destructor
     */
    ~GstreamerDv1394Grabber();

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
     * Shut downs the grabber process either if it is runned in deamon
     * mode or "single grab" mode.
     * @return true on success, false otherwise
     */
    bool tearDown();

private:
    /**
     * Pad to select the video stream from the demux to the decoder
     */
    static void on_pad_added (GstElement *element,
                              GstPad     *pad,
                              gpointer    data);

    /**
     *
     */
    static void cb_typefound (GstElement *typefind,
                              guint       probability,
                              GstCaps    *caps,
                              gpointer    data);

private:
    int         activeSource;
    bool        isInitSuccess;
    bool        firstImage;

    GstElement *filter3;
    GstElement *queue1;
    GstElement *queue2;

};

#endif