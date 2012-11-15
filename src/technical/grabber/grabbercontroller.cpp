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

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberController::GrabberController(ImageGrabberDevice *d,
                                     int                 cap)
{
    qDebug("GrabberController::Constructor --> Start");

    device = d;
    controllerCap = cap;

    qDebug("GrabberController::Constructor --> End");
}


GrabberController::~GrabberController()
{
    qDebug("GrabberController::Destructor --> Start (Empty)");

    // qDebug("GrabberController::Destructor --> End");
}


ImageGrabberDevice* GrabberController::getDevice()
{
    return device;
}


int GrabberController::getControllerCapabilities()
{
    return controllerCap;
}

/**************************************************************************
 **************************************************************************
 * Camera capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/

bool GrabberController::isBrightness()
{
    return false;
}


GrabberControlCapabilities *GrabberController::getBrightnessCaps()
{
    return NULL;
}


int GrabberController::getBrightness()
{
    return 0;
}


void GrabberController::setBrightness(int /*b*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Contrast
 **************************************************************************/

bool GrabberController::isContrast()
{
    return false;
}


int GrabberController::getMaximumContrast()
{
    return 0;
}


int GrabberController::getContrast()
{
    return 0;
}


void GrabberController::setContrast(int /*c*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Saturation
 **************************************************************************/

bool GrabberController::isSaturation()
{
    return false;
}


int GrabberController::getMaximumSaturation()
{
    return 0;
}


int GrabberController::getSaturation()
{
    return 0;
}


void GrabberController::setSaturation(int /*s*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Hue
 **************************************************************************/

bool GrabberController::isHue()
{
    return false;
}


int GrabberController::getMaximumHue()
{
    return 0;
}


int GrabberController::getHue()
{
    return 0;
}


void GrabberController::setHue(int /*h*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Gamma
 **************************************************************************/

bool GrabberController::isGamma()
{
    return false;
}


int GrabberController::getMaximumGamma()
{
    return 0;
}


int GrabberController::getGamma()
{
    return 0;
}


void GrabberController::setGamma(int /*g*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Sharpness
 **************************************************************************/

bool GrabberController::isSharpness()
{
    return false;
}


int GrabberController::getMaximumSharpness()
{
    return 0;
}


int GrabberController::getSharpness()
{
    return 0;
}


void GrabberController::setSharpness(int /*s*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Backlight Compensation
 **************************************************************************/

bool GrabberController::isBacklight()
{
    return false;
}


int GrabberController::getMaximumBacklight()
{
    return 0;
}


int GrabberController::getBacklight()
{
    return 0;
}


void GrabberController::setBacklight(int /*b*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Exposure
 **************************************************************************/

bool GrabberController::isAutomaticExposure()
{
    return false;
}


bool GrabberController::getAutomaticExposure()
{
    return false;
}


void GrabberController::setAutomaticExposure(bool /*ae*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isExposure()
{
    return false;
}


int GrabberController::getMaximumExposure()
{
    return 0;
}


int GrabberController::getExposure()
{
    return 0;
}


void GrabberController::setExposure(int /*e*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * White Balance
 **************************************************************************/

bool GrabberController::isAutomaticWhite()
{
    return false;
}


bool GrabberController::getAutomaticWhite()
{
    return false;
}


void GrabberController::setAutomaticWhite(bool /*aw*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isWhite()
{
    return false;
}


int GrabberController::getMaximumWhite()
{
    return 0;
}


int GrabberController::getWhite()
{
    return 0;
}


void GrabberController::setWhite(int /*w*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Zoom
 **************************************************************************/

bool GrabberController::isAutomaticZoom()
{
    return false;
}


bool GrabberController::getAutomaticZoom()
{
    return false;
}


void GrabberController::setAutomaticZoom(bool /*az*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isZoom()
{
    return false;
}


int GrabberController::getMaximumZoom()
{
    return 0;
}


int GrabberController::getZoom()
{
    return 0;
}


void GrabberController::setZoom(int /*z*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Focus
 **************************************************************************/

bool GrabberController::isAutomaticFocus()
{
    return false;
}


bool GrabberController::getAutomaticFocus()
{
    return false;
}


void GrabberController::setAutomaticFocus(bool /*af*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isFocus()
{
    return false;
}


int GrabberController::getMaximumFocus()
{
    return 0;
}


int GrabberController::getFocus()
{
    return 0;
}


void GrabberController::setFocus(int /*f*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Pan
 **************************************************************************/

bool GrabberController::isPan()
{
    return false;
}


int GrabberController::getMaximumPan()
{
    return 0;
}


int GrabberController::getPan()
{
    return 0;
}


void GrabberController::setPan(int /*p*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Tilt
 **************************************************************************/

bool GrabberController::isTilt()
{
    return false;
}


int GrabberController::getMaximumTilt()
{
    return 0;
}


int GrabberController::getTilt()
{
    return 0;
}


void GrabberController::setTilt(int /*t*/)
{
    Q_ASSERT( 1 );
}
