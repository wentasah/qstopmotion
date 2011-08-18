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
public:

    /**
     * All possible gstreamer sources.
     */
    enum GstreamerSource {
        TestSource,
        Video4LinuxSource,
        Iee1394Source,
        DirectShowSource
    };

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    GstreamerGrabber(GstreamerSource activeSource, const QString &filePath);

    /**
     * Destructor
     */
    ~GstreamerGrabber();

    /**
     * Initialization of the Command line grabber
     */
    bool initialization();

    /**
     * Starts the grabber if it is marked to be runned in deamon mode.
     * @return true on success, false otherwise
     */
    bool init();

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

    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer data);

    // static GMainLoop *loop;

private:
    // QProcess process;
    GstreamerSource activeSource;
    bool            isInitSuccess;

    GstElement *pipeline;
    GstElement *source;
    GstElement *filter1;
    GstElement *filter2;
    GstElement *filter3;
    GstElement *filter4;
    GstElement *filter5;
    GstElement *sink;
    // GstBus     *bus;
    // GMainLoop  *loop;

};

#endif
