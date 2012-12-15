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

GrabberController::GrabberController(int cap)
{
    qDebug("GrabberController::Constructor --> Start");

    controllerCap = cap;

    qDebug("GrabberController::Constructor --> End");
}


GrabberController::~GrabberController()
{
    qDebug("GrabberController::Destructor --> Start (Empty)");

    // qDebug("GrabberController::Destructor --> End");
}


bool GrabberController::init(const QString id)
{
    return true;
}


int GrabberController::getControllerCapabilities()
{
    return controllerCap;
}

/**************************************************************************
 **************************************************************************
 * Camera quality capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/

bool GrabberController::isBrightness()
{
    if (brightnessCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getBrightnessCaps()
{
    return &brightnessCapabilities;
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
    if (contrastCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getContrastCaps()
{
    return &contrastCapabilities;
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
    if (saturationCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getSaturationCaps()
{
    return &saturationCapabilities;
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
    if (hueCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getHueCaps()
{
    return &hueCapabilities;
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
    if (gammaCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getGammaCaps()
{
    return &gammaCapabilities;
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
    if (sharpnessCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getSharpnessCaps()
{
    return &sharpnessCapabilities;
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
    if (backlightCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getBacklightCaps()
{
    return &backlightCapabilities;
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
 * White Balance
 **************************************************************************/

bool GrabberController::isAutomaticWhite()
{
    if (whiteCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

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
    if (whiteCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getWhiteCaps()
{
    return &whiteCapabilities;
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
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 **************************************************************************/

bool GrabberController::isAutomaticExposure()
{
    if (exposureCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

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
    if (exposureCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getExposureCaps()
{
    return &exposureCapabilities;
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
 * Zoom
 **************************************************************************/

bool GrabberController::isAutomaticZoom()
{
    if (zoomCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

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
    if (zoomCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }

    return false;
}


GrabberControlCapabilities *GrabberController::getZoomCaps()
{
    return &zoomCapabilities;
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
    if (focusCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

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
    if (focusCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getFocusCaps()
{
    return &focusCapabilities;
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
    if (panCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getPanCaps()
{
    return &panCapabilities;
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
    if (tiltCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getTiltCaps()
{
    return &tiltCapabilities;
}


int GrabberController::getTilt()
{
    return 0;
}


void GrabberController::setTilt(int /*t*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Iris
 **************************************************************************/

bool GrabberController::isAutomaticIris()
{
    if (irisCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

    return false;
}


bool GrabberController::getAutomaticIris()
{
    return false;
}


void GrabberController::setAutomaticIris(bool /*ai*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isIris()
{
    if (irisCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getIrisCaps()
{
    return &irisCapabilities;
}


int GrabberController::getIris()
{
    return 0;
}


void GrabberController::setIris(int /*i*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Roll
 **************************************************************************/

bool GrabberController::isAutomaticRoll()
{
    if (rollCapabilities.getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

    return false;
}


bool GrabberController::getAutomaticRoll()
{
    return false;
}


void GrabberController::setAutomaticRoll(bool /*ar*/)
{
    Q_ASSERT( 1 );
}

bool GrabberController::isRoll()
{
    if (rollCapabilities.getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


GrabberControlCapabilities *GrabberController::getRollCaps()
{
    return &rollCapabilities;
}


int GrabberController::getRoll()
{
    return 0;
}


void GrabberController::setRoll(int /*r*/)
{
    Q_ASSERT( 1 );
}
