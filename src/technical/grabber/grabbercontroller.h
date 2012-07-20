/******************************************************************************
 *  Copyright (C) 2012-2012 by                                                *
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

#ifndef GRABBERCONTROLLER_H
#define GRABBERCONTROLLER_H

#include "technical/grabber/imagegrabberdevice.h"

#include <QtCore/QString>
#include <QtCore/QVector>


/**
 * Interface to the controller of a device.
 *
 * @author Ralf Lange
 */
class GrabberController
{
public:

    /**
     * Device controller capabilities
     */
    enum grabberControllerCapabilities {
        controller_none,
        controller_1,
        controller_2,
        controller_3
    };

    /**
     * Constructs and initializes the object.
     * @param d The image grabber device of the controller.
     * @param caps The grabber controller capabilities.
     */
    GrabberController(ImageGrabberDevice *d,
                      int                 caps);

    /**
     * Destructor
     */
    ~GrabberController();

    /**
     * Get the device of the controller.
     * @return The device of the controller.
     */
    ImageGrabberDevice* getDevice();

    /**
     * Get the capabilities of the controller.
     * @return The capabilities of the controller.
     */
    int getControllerCapabilities();

private:
    ImageGrabberDevice *device;
    int                 controllerCap;
};

#endif
