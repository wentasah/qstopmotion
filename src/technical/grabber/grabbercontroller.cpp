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

#include "grabbercontroller.h"

#include <QtCore/QtDebug>

GrabberController::GrabberController(ImageGrabberDevice *d,
                                     int                 cap)
{
    qDebug("GrabberController::Constructor --> Start");

    // device = d;
    controllerCap = cap;

    qDebug("GrabberController::Constructor --> End");
}


GrabberController::~GrabberController()
{
    qDebug("GrabberController::Destructor --> Start");

    qDebug("GrabberController::Destructor --> End");
}


ImageGrabberDevice* GrabberController::getDevice()
{
    return device;
}


int GrabberController::getControllerCapabilities()
{
    return controllerCap;
}
