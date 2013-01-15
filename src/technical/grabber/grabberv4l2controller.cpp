/******************************************************************************
 *  Copyright (C) 2012-2013 by                                                *
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

#include "grabberv4l2controller.h"

#include <QtCore/QtDebug>

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberV4L2Controller::GrabberV4L2Controller(int cap)
    : GrabberController(cap)
{
    qDebug("GrabberV4L2Controller::Constructor --> Start");

    // pCameraControl = NULL;
    // pQualityControl = NULL;

    qDebug("GrabberV4L2Controller::Constructor --> End");
}


GrabberV4L2Controller::~GrabberV4L2Controller()
{
    qDebug("GrabberV4L2Controller::Destructor --> Start (Empty)");

    // qDebug("GrabberV4L2Controller::Destructor --> End");
}


bool GrabberV4L2Controller::init(const QString id)
{
    qDebug("GrabberV4L2Controller::init --> Start");


    qDebug("GrabberV4L2Controller::init --> Enumerating video input devices ...\n");

    qDebug("GrabberV4L2Controller::init --> End (Successful)");

    return false;
}


bool GrabberV4L2Controller::setControlCapabilities()
{
    qDebug("GrabberV4L2Controller::setCameraCapabilities --> Start");
/*
    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Exposure, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getExposureCaps()->setMinimum(min);
        getExposureCaps()->setMaximum(max);
        getExposureCaps()->setStep(step);
        getExposureCaps()->setDefault(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getExposureCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getExposureCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Exposure property information.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Zoom, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getZoomCaps()->setMinimum(min);
        getZoomCaps()->setMaximum(max);
        getZoomCaps()->setStep(step);
        getZoomCaps()->setDefault(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Zoom property information.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Focus, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getFocusCaps()->setMinimum(min);
        getFocusCaps()->setMaximum(max);
        getFocusCaps()->setStep(step);
        getFocusCaps()->setDefault(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Focus property information.\n");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Pan, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getPanCaps()->setMinimum(min);
        getPanCaps()->setMaximum(max);
        getPanCaps()->setStep(step);
        getPanCaps()->setDefault(def);
        getPanCaps()->setFlags(flags);
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Pan property information.\n");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Tilt, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getTiltCaps()->setMinimum(min);
        getTiltCaps()->setMaximum(max);
        getTiltCaps()->setStep(step);
        getTiltCaps()->setDefault(def);
        getTiltCaps()->setFlags(flags);
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Tilt property information.\n");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Iris, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getIrisCaps()->setMinimum(min);
        getIrisCaps()->setMaximum(max);
        getIrisCaps()->setStep(step);
        getIrisCaps()->setDefault(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getIrisCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getIrisCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Iris property information.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Roll, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getRollCaps()->setMinimum(min);
        getRollCaps()->setMaximum(max);
        getRollCaps()->setStep(step);
        getRollCaps()->setDefault(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getRollCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getRollCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setCameraCapabilities --> Unable to retrieve Roll property information.");
    }
*/
    qDebug("GrabberV4L2Controller::setCameraCapabilities --> End");

    return true;
}


bool GrabberV4L2Controller::setQualityCapabilities()
{
    qDebug("GrabberV4L2Controller::setQualityCapabilities --> Start");
/*
    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Brightness, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getBrightnessCaps()->setMinimum(min);
        getBrightnessCaps()->setMaximum(max);
        getBrightnessCaps()->setStep(step);
        getBrightnessCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getBrightnessCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getBrightnessCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Brightness property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Contrast, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getContrastCaps()->setMinimum(min);
        getContrastCaps()->setMaximum(max);
        getContrastCaps()->setStep(step);
        getContrastCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getContrastCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getContrastCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Contrast property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Saturation, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getSaturationCaps()->setMinimum(min);
        getSaturationCaps()->setMaximum(max);
        getSaturationCaps()->setStep(step);
        getSaturationCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getSaturationCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getSaturationCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Saturation property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Hue, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getHueCaps()->setMinimum(min);
        getHueCaps()->setMaximum(max);
        getHueCaps()->setStep(step);
        getHueCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getHueCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getHueCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Hue property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Gamma, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getGammaCaps()->setMinimum(min);
        getGammaCaps()->setMaximum(max);
        getGammaCaps()->setStep(step);
        getGammaCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getGammaCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getGammaCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Gamma property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Sharpness, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getSharpnessCaps()->setMinimum(min);
        getSharpnessCaps()->setMaximum(max);
        getSharpnessCaps()->setStep(step);
        getSharpnessCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getSharpnessCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getSharpnessCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Sharpness property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_BacklightCompensation, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getBacklightCaps()->setMinimum(min);
        getBacklightCaps()->setMaximum(max);
        getBacklightCaps()->setStep(step);
        getBacklightCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getBacklightCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getBacklightCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Backlight Compensation property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_WhiteBalance, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getWhiteCaps()->setMinimum(min);
        getWhiteCaps()->setMaximum(max);
        getWhiteCaps()->setStep(step);
        getWhiteCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getWhiteCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getWhiteCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve White Balance property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Gain, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getGainCaps()->setMinimum(min);
        getGainCaps()->setMaximum(max);
        getGainCaps()->setStep(step);
        getGainCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getGainCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getGainCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Gain property information.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_ColorEnable, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getColorCaps()->setMinimum(min);
        getColorCaps()->setMaximum(max);
        getColorCaps()->setStep(step);
        getColorCaps()->setDefault(def);
        if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO) {
            getColorCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getColorCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberV4L2Controller::setQualityCapabilities --> Unable to retrieve Color Enable property information.");
    }
*/
    qDebug("GrabberV4L2Controller::setQualityCapabilities --> End");

    return true;
}


/**************************************************************************
 **************************************************************************
 * Camera quality capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticBrightness()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticBrightness(bool ab)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getBrightness()
{
    return 0;
}


void GrabberV4L2Controller::setBrightness(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Contrast
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticContrast()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticContrast(bool ac)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getContrast()
{
    return 0;
}


void GrabberV4L2Controller::setContrast(int c)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Saturation
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticSaturation()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticSaturation(bool as)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getSaturation()
{
    return 0;
}


void GrabberV4L2Controller::setSaturation(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Hue
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticHue()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticHue(bool ah)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getHue()
{
    return 0;
}


void GrabberV4L2Controller::setHue(int h)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gamma
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticGamma()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticGamma(bool ag)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getGamma()
{
    return 0;
}


void GrabberV4L2Controller::setGamma(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticSharpness()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticSharpness(bool as)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getSharpness()
{
    return 0;
}


void GrabberV4L2Controller::setSharpness(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticBacklight()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticBacklight(bool ab)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getBacklight()
{
    return 0;
}


void GrabberV4L2Controller::setBacklight(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticWhite()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getWhite()
{
    return 0;
}


void GrabberV4L2Controller::setWhite(int w)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gain
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticGain()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticGain(bool ag)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getGain()
{
    return 0;
}


void GrabberV4L2Controller::setGain(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Color Enable
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticColor()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticColor(bool ac)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getColor()
{
    return 0;
}


void GrabberV4L2Controller::setColor(int c)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticExposure()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isExposure()
{
    return false;
}


void GrabberV4L2Controller::setExposure(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Zoom
 **************************************************************************/

bool GrabberV4L2Controller::getAutomaticZoom()
{
    /*
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }
*/
    return false;
}


void GrabberV4L2Controller::setAutomaticZoom(bool az)
{
    /*
    HRESULT hr = 0;
    long flags;
    long z = 0;

    if (az) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        z = getZoomCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Zoom, z, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr;
    }
    */
}

int GrabberV4L2Controller::getZoom()
{
    /*
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
        return -1;
    }
*/
    return 0; // (int)z;
}


void GrabberV4L2Controller::setZoom(int z)
{
    /*
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Zoom, (long)z, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr;
    }
    */
}

/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticFocus()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}

int GrabberV4L2Controller::getFocus()
{
    return 0;
}


void GrabberV4L2Controller::setFocus(int f)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Pan
 **************************************************************************/

bool GrabberV4L2Controller::getAutomaticPan()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticPan(bool ap)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getPan()
{
    return 0;
}


void GrabberV4L2Controller::setPan(int p)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Tilt
 **************************************************************************/

bool GrabberV4L2Controller::getAutomaticTilt()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticTilt(bool at)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getTilt()
{
    return 0;
}


void GrabberV4L2Controller::setTilt(int t)
{
    Q_ASSERT( 1 );
}
/**************************************************************************
 * Iris
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticIris()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticIris(bool ae)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isIris()
{
    return false;
}


void GrabberV4L2Controller::setIris(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Roll
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticRoll()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticRoll(bool ar)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isRoll()
{
    return false;
}


void GrabberV4L2Controller::setRoll(int e)
{
    Q_ASSERT( 1 );
}
*/
