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

#include "windowsuvccontroller.h"

#include <QtCore/QtDebug>

/**************************************************************************
 * Implementation of the Windows UVC controller functions.
 **************************************************************************/

WindowsUvcController::WindowsUvcController(int cap)
    : GrabberController(cap)
{
    qDebug("WindowsUvcController::Constructor --> Start (Empty)");

    // qDebug("WindowsUvcController::Constructor --> End");
}


WindowsUvcController::~WindowsUvcController()
{
    qDebug("WindowsUvcController::Destructor --> Start (Empty)");

    // qDebug("WindowsUvcController::Destructor --> End");
}


/*
ImageGrabberDevice* WindowsUvcController::getDevice()
{
    return device;
}


int WindowsUvcController::getControllerCapabilities()
{
    return controllerCap;
}
*/
/**************************************************************************
 **************************************************************************
 * Camera capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/
/*
bool WindowsUvcController::isBrightness()
{
    return false;
}


int WindowsUvcController::getMaximumBrightness()
{
    return 0;
}


int WindowsUvcController::getBrightness()
{
    return 0;
}


void WindowsUvcController::setBrightness(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Contrast
 **************************************************************************/
/*
bool WindowsUvcController::isContrast()
{
    return false;
}


int WindowsUvcController::getMaximumContrast()
{
    return 0;
}


int WindowsUvcController::getContrast()
{
    return 0;
}


void WindowsUvcController::setContrast(int c)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Saturation
 **************************************************************************/
/*
bool WindowsUvcController::isSaturation()
{
    return false;
}


int WindowsUvcController::getMaximumSaturation()
{
    return 0;
}


int WindowsUvcController::getSaturation()
{
    return 0;
}


void WindowsUvcController::setSaturation(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Hue
 **************************************************************************/
/*
bool WindowsUvcController::isHue()
{
    return false;
}


int WindowsUvcController::getMaximumHue()
{
    return 0;
}


int WindowsUvcController::getHue()
{
    return 0;
}


void WindowsUvcController::setHue(int h)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gamma
 **************************************************************************/
/*
bool WindowsUvcController::isGamma()
{
    return false;
}


int WindowsUvcController::getMaximumGamma()
{
    return 0;
}


int WindowsUvcController::getGamma()
{
    return 0;
}


void WindowsUvcController::setGamma(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
bool WindowsUvcController::isSharpness()
{
    return false;
}


int WindowsUvcController::getMaximumSharpness()
{
    return 0;
}


int WindowsUvcController::getSharpness()
{
    return 0;
}


void WindowsUvcController::setSharpness(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
bool WindowsUvcController::isBacklight()
{
    return false;
}


int WindowsUvcController::getMaximumBacklight()
{
    return 0;
}


int WindowsUvcController::getBacklight()
{
    return 0;
}


void WindowsUvcController::setBacklight(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Exposure
 **************************************************************************/
/*
bool WindowsUvcController::isAutomaticExposure()
{
    return false;
}


bool WindowsUvcController::getAutomaticExposure()
{
    return false;
}


void WindowsUvcController::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}

bool WindowsUvcController::isExposure()
{
    return false;
}


int WindowsUvcController::getMaximumExposure()
{
    return 0;
}


int WindowsUvcController::getExposure()
{
    return 0;
}


void WindowsUvcController::setExposure(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool WindowsUvcController::isAutomaticWhite()
{
    return false;
}


bool WindowsUvcController::getAutomaticWhite()
{
    return false;
}


void WindowsUvcController::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}

bool WindowsUvcController::isWhite()
{
    return false;
}


int WindowsUvcController::getMaximumWhite()
{
    return 0;
}


int WindowsUvcController::getWhite()
{
    return 0;
}


void WindowsUvcController::setWhite(int w)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Zoom
 **************************************************************************/
/*
bool WindowsUvcController::isAutomaticZoom()
{
    return false;
}


bool WindowsUvcController::getAutomaticZoom()
{
    return false;
}


void WindowsUvcController::setAutomaticZoom(bool az)
{
    Q_ASSERT( 1 );
}

bool WindowsUvcController::isZoom()
{
    return false;
}


int WindowsUvcController::getMaximumZoom()
{
    return 0;
}


int WindowsUvcController::getZoom()
{
    return 0;
}


void WindowsUvcController::setZoom(int z)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool WindowsUvcController::isAutomaticFocus()
{
    return false;
}


bool WindowsUvcController::getAutomaticFocus()
{
    return false;
}


void WindowsUvcController::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}

bool WindowsUvcController::isFocus()
{
    return false;
}


int WindowsUvcController::getMaximumFocus()
{
    return 0;
}


int WindowsUvcController::getFocus()
{
    return 0;
}


void WindowsUvcController::setFocus(int f)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Pan
 **************************************************************************/
/*
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


void GrabberController::setPan(int p)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Tilt
 **************************************************************************/
/*
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


void GrabberController::setTilt(int t)
{
    Q_ASSERT( 1 );
}
*/
