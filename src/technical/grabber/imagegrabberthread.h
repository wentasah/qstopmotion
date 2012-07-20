/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
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

#ifndef IMAGEGRABBERTHREAD_H
#define IMAGEGRABBERTHREAD_H

#include "technical/grabber/imagegrabber.h"

#include <QtCore/QThread>


/**
 * Thread used for polling an external program to update the camera. Only
 * used for polling, not for when the camera is running in deamon mode.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ImageGrabberThread : public QThread
{
public:
    /**
     * Constructs and initializes the object.
     * @param frameView the frame view to be used for displaying images
     * @param g the grabber to use be used for grabbing images
     */
    ImageGrabberThread(ImageGrabber *g);

    /**
     * Displays the images grabbed with the registered grabber. These
     * are displayed one after one and it looks like we are streaming
     * live from the camera.
     */
    void run();

    /**
     * Checks if last grabbing was successfull.
     * @return true if last grabbing was success, false otherwise
     */
    bool wasGrabbingSuccess();

private:
    ImageGrabber *grabber;
    bool wasGrabSuccess;
};

#endif
