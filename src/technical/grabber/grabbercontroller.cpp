/******************************************************************************
 *  Copyright (C) 2012-2015 by                                                *
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

#include <QDebug>


/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberController::GrabberController(int cap)
{
    qDebug() << "GrabberController::Constructor --> Start";

    controllerCap = cap;

    qDebug() << "GrabberController::Constructor --> End";
}


GrabberController::~GrabberController()
{
    qDebug() << "GrabberController::Destructor --> Start (Empty)";

    // qDebug() << "GrabberController::Destructor --> End";
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

GrabberControlCapabilities *GrabberController::getBrightnessCaps()
{
    return &brightnessCapabilities;
}


bool GrabberController::getAutomaticBrightness()
{
    return false;
}


void GrabberController::setAutomaticBrightness(bool /*ab*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getContrastCaps()
{
    return &contrastCapabilities;
}


bool GrabberController::getAutomaticContrast()
{
    return false;
}


void GrabberController::setAutomaticContrast(bool /*ac*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getSaturationCaps()
{
    return &saturationCapabilities;
}


bool GrabberController::getAutomaticSaturation()
{
    return false;
}


void GrabberController::setAutomaticSaturation(bool /*as*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getHueCaps()
{
    return &hueCapabilities;
}


bool GrabberController::getAutomaticHue()
{
    return false;
}


void GrabberController::setAutomaticHue(bool /*ah*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getGammaCaps()
{
    return &gammaCapabilities;
}


bool GrabberController::getAutomaticGamma()
{
    return false;
}


void GrabberController::setAutomaticGamma(bool /*ag*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getSharpnessCaps()
{
    return &sharpnessCapabilities;
}


bool GrabberController::getAutomaticSharpness()
{
    return false;
}


void GrabberController::setAutomaticSharpness(bool /*as*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getBacklightCaps()
{
    return &backlightCapabilities;
}


bool GrabberController::getAutomaticBacklight()
{
    return false;
}


void GrabberController::setAutomaticBacklight(bool /*ab*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getWhiteCaps()
{
    return &whiteCapabilities;
}


bool GrabberController::getAutomaticWhite()
{
    return false;
}


void GrabberController::setAutomaticWhite(bool /*aw*/)
{
    Q_ASSERT( 1 );
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
 * Gain
 **************************************************************************/

GrabberControlCapabilities *GrabberController::getGainCaps()
{
    return &gainCapabilities;
}


bool GrabberController::getAutomaticGain()
{
    return false;
}


void GrabberController::setAutomaticGain(bool /*ag*/)
{
    Q_ASSERT( 1 );
}


int GrabberController::getGain()
{
    return 0;
}


void GrabberController::setGain(int /*g*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Color Enable
 **************************************************************************/

GrabberControlCapabilities *GrabberController::getColorCaps()
{
    return &colorCapabilities;
}


bool GrabberController::getAutomaticColor()
{
    return false;
}


void GrabberController::setAutomaticColor(bool /*ac*/)
{
    Q_ASSERT( 1 );
}


int GrabberController::getColor()
{
    return 0;
}


void GrabberController::setColor(int /*c*/)
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

GrabberControlCapabilities *GrabberController::getExposureCaps()
{
    return &exposureCapabilities;
}


bool GrabberController::getAutomaticExposure()
{
    return false;
}


void GrabberController::setAutomaticExposure(bool /*ae*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getZoomCaps()
{
    return &zoomCapabilities;
}


bool GrabberController::getAutomaticZoom()
{
    return false;
}


void GrabberController::setAutomaticZoom(bool /*az*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getFocusCaps()
{
    return &focusCapabilities;
}


bool GrabberController::getAutomaticFocus()
{
    return false;
}


void GrabberController::setAutomaticFocus(bool /*af*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getPanCaps()
{
    return &panCapabilities;
}


bool GrabberController::getAutomaticPan()
{
    return false;
}


void GrabberController::setAutomaticPan(bool /*ap*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getTiltCaps()
{
    return &tiltCapabilities;
}


bool GrabberController::getAutomaticTilt()
{
    return false;
}


void GrabberController::setAutomaticTilt(bool /*at*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getIrisCaps()
{
    return &irisCapabilities;
}


bool GrabberController::getAutomaticIris()
{
    return false;
}


void GrabberController::setAutomaticIris(bool /*ai*/)
{
    Q_ASSERT( 1 );
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

GrabberControlCapabilities *GrabberController::getRollCaps()
{
    return &rollCapabilities;
}


bool GrabberController::getAutomaticRoll()
{
    return false;
}


void GrabberController::setAutomaticRoll(bool /*ar*/)
{
    Q_ASSERT( 1 );
}


int GrabberController::getRoll()
{
    return 0;
}


void GrabberController::setRoll(int /*r*/)
{
    Q_ASSERT( 1 );
}
