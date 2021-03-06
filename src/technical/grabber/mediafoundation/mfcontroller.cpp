/******************************************************************************
 *  Copyright (C) 2012-2016 by                                                *
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

#include "mfcontroller.h"
#include "mfutil.h"

#include <QtCore/QtDebug>

#define SLEEP_TIME 200

/**************************************************************************
 * Implementation of the grabber controller functions using the
 * Microsoft Media Foundation API.
 **************************************************************************/

MfController::MfController(int cap)
    : GrabberController(cap)
{
    qDebug() << "MfController::Constructor --> Start";

    grabber = NULL;
    grabberDevice = NULL;
    cameraControl = NULL;
    qualityControl = NULL;

    qDebug() << "MfController::Constructor --> End";
}


MfController::~MfController()
{
    qDebug() << "MfController::Destructor --> Start";

    SafeRelease(&qualityControl);
    SafeRelease(&cameraControl);

    qDebug() << "MfController::Destructor --> End";
}


ImageGrabber* MfController::getImageGrabber()
{
    return grabber;
}


bool MfController::initialization(ImageGrabber* ig, ImageGrabberDevice* igd)
{
    qDebug() << "MfController::init --> Start";

    grabber = (MfGrabber*)ig;
    grabberDevice = igd;

    bool            ret = false;
    IMFMediaSource* mediaSource = grabber->getSource();

    if(NULL == mediaSource) {
        return false;
    }

    // unsigned int shift = sizeof(Parametr);

    HRESULT hr = mediaSource->QueryInterface(IID_PPV_ARGS(&qualityControl));
    if(S_OK == hr) {
        if (setQualityCapabilities()) {
            ret = true;
        }
        SafeRelease(&qualityControl);
    }
    else {
        qDebug() << "MfController::init --> ERROR: Unable to access IAMCameraControl interface.";
    }

    hr = mediaSource->QueryInterface(IID_PPV_ARGS(&cameraControl));

    if(S_OK == hr) {
        if (setControlCapabilities()) {
            ret = true;
        }
        SafeRelease(&cameraControl);
    }
    else {
        qDebug() << "MfController::init --> ERROR: Unable to access IAMCameraControl interface.";
    }

    qDebug() << "MfController::init --> End (Successful)";

    return ret;
}


bool MfController::setUp()
{
    qDebug() << "MfController::setUp --> Start";

    bool            ret = false;
    HRESULT         hr;
    IMFMediaSource* mediaSource = grabber->getSource();

    if(NULL == mediaSource) {
        return false;
    }

    // unsigned int shift = sizeof(Parametr);

    hr = mediaSource->QueryInterface(IID_PPV_ARGS(&qualityControl));
    if(S_OK == hr) {
        ret = true;
    }
    else {
        qDebug() << "MfController::setUp --> ERROR: Unable to access IAMCameraControl interface.";
    }

    hr = mediaSource->QueryInterface(IID_PPV_ARGS(&cameraControl));
    if(S_OK == hr) {
        ret = true;
    }
    else {
        qDebug() << "MfController::setUp --> ERROR: Unable to access IAMCameraControl interface.";
    }

    qDebug() << "MfController::setUp --> End";

    return true;
}


bool MfController::tearDown()
{
    qDebug() << "MfController::tearDown --> Start";

    SafeRelease(&qualityControl);
    SafeRelease(&cameraControl);

    qDebug() << "MfController::tearDown --> End";

    return true;
}


bool MfController::setControlCapabilities()
{
    qDebug() << "MfController::setCameraCapabilities --> Start";

    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Exposure, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Exposure control supported.";

        getExposureCaps()->setMinimum(min);
        getExposureCaps()->setMaximum(max);
        getExposureCaps()->setStep(step);
        getExposureCaps()->setDefault(def);
        if (flags & CameraControl_Flags_Auto) {
            getExposureCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getExposureCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Exposure control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Zoom, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Zoom control supported.";

        getZoomCaps()->setMinimum(min);
        getZoomCaps()->setMaximum(max);
        getZoomCaps()->setStep(step);
        getZoomCaps()->setDefault(def);
        if (flags & CameraControl_Flags_Auto) {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Zoom control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Focus, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Focus control supported.";

        getFocusCaps()->setMinimum(min);
        getFocusCaps()->setMaximum(max);
        getFocusCaps()->setStep(step);
        getFocusCaps()->setDefault(def);
        if (flags & CameraControl_Flags_Auto) {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Focus control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Pan, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Pan control supported";

        getPanCaps()->setMinimum(min);
        getPanCaps()->setMaximum(max);
        getPanCaps()->setStep(step);
        getPanCaps()->setDefault(def);
        getPanCaps()->setFlags(flags);
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Pan control not supported";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Tilt, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Tilt control supported";

        getTiltCaps()->setMinimum(min);
        getTiltCaps()->setMaximum(max);
        getTiltCaps()->setStep(step);
        getTiltCaps()->setDefault(def);
        getTiltCaps()->setFlags(flags);
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Tilt control not supported";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Iris, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Iris control supported.";

        getIrisCaps()->setMinimum(min);
        getIrisCaps()->setMaximum(max);
        getIrisCaps()->setStep(step);
        getIrisCaps()->setDefault(def);
        if (flags & CameraControl_Flags_Auto) {
            getIrisCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getIrisCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Iris control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = cameraControl->GetRange(CameraControl_Roll, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setCameraCapabilities --> Roll control supported.";

        getRollCaps()->setMinimum(min);
        getRollCaps()->setMaximum(max);
        getRollCaps()->setStep(step);
        getRollCaps()->setDefault(def);
        if (flags & CameraControl_Flags_Auto) {
            getRollCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getRollCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setCameraCapabilities --> Roll control not supported.";
    }

    qDebug() << "MfController::setCameraCapabilities --> End";

    return true;
}


bool MfController::setQualityCapabilities()
{
    qDebug() << "MfController::setQualityCapabilities --> Start";

    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Brightness, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Brightness control supported.";

        getBrightnessCaps()->setMinimum(min);
        getBrightnessCaps()->setMaximum(max);
        getBrightnessCaps()->setStep(step);
        getBrightnessCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getBrightnessCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getBrightnessCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Brightness control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Contrast, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Contrast control supported.";

        getContrastCaps()->setMinimum(min);
        getContrastCaps()->setMaximum(max);
        getContrastCaps()->setStep(step);
        getContrastCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getContrastCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getContrastCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Contrast control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Saturation, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Saturation control supported.";

        getSaturationCaps()->setMinimum(min);
        getSaturationCaps()->setMaximum(max);
        getSaturationCaps()->setStep(step);
        getSaturationCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getSaturationCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getSaturationCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Saturation control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Hue, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Hue control supported.";

        getHueCaps()->setMinimum(min);
        getHueCaps()->setMaximum(max);
        getHueCaps()->setStep(step);
        getHueCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getHueCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getHueCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Hue control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Gamma, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Gamma control supported.";

        getGammaCaps()->setMinimum(min);
        getGammaCaps()->setMaximum(max);
        getGammaCaps()->setStep(step);
        getGammaCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getGammaCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getGammaCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Gamma control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Sharpness, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Sharpness control supported.";

        getSharpnessCaps()->setMinimum(min);
        getSharpnessCaps()->setMaximum(max);
        getSharpnessCaps()->setStep(step);
        getSharpnessCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getSharpnessCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getSharpnessCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Sharpness control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_BacklightCompensation, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Backlight Compensation control supported.";

        getBacklightCaps()->setMinimum(min);
        getBacklightCaps()->setMaximum(max);
        getBacklightCaps()->setStep(step);
        getBacklightCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getBacklightCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getBacklightCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Backlight Compensation control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_WhiteBalance, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> White Balance control supported.";

        getWhiteCaps()->setMinimum(min);
        getWhiteCaps()->setMaximum(max);
        getWhiteCaps()->setStep(step);
        getWhiteCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getWhiteCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getWhiteCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> White Balance control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_Gain, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Gain control supported.";

        getGainCaps()->setMinimum(min);
        getGainCaps()->setMaximum(max);
        getGainCaps()->setStep(step);
        getGainCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getGainCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getGainCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Gain control not supported.";
    }

    Sleep(SLEEP_TIME);
    hr = qualityControl->GetRange(VideoProcAmp_ColorEnable, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug() << "MfController::setQualityCapabilities --> Color Enable control supported.";

        getColorCaps()->setMinimum(min);
        getColorCaps()->setMaximum(max);
        getColorCaps()->setStep(step);
        getColorCaps()->setDefault(def);
        if (flags & VideoProcAmp_Flags_Auto) {
            getColorCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getColorCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug() << "MfController::setQualityCapabilities --> Color Enable control not supported.";
    }

    qDebug() << "MfController::setQualityCapabilities --> End";

    return true;
}

/**************************************************************************
 **************************************************************************
 * Camera quality capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from –10,000 to 10,000.
 **************************************************************************/

bool MfController::getAutomaticBrightness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long bright_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Brightness, &bright_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticBrightness(bool ab)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long bright_value = 0L;

    if (ab) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        bright_value = getBrightnessCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Brightness, bright_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticBrightness --> ERROR: Unable to set Brightness property value to " << ab << ". (Error 0x" << hr << ")";
    }
}


int MfController::getBrightness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long bright_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Brightness, &bright_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)bright_value;
}


void MfController::setBrightness(int b)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long bright_value = b;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Brightness, (long)bright_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setBrightness --> ERROR: Unable to set Brightness property value to " << bright_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Contrast
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 10,000.
 **************************************************************************/
/*
bool MfController::getAutomaticContrast()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long contrast_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Contrast, &contrast_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticContrast(bool ac)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long contrast_value = 0L;

    if (ac) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        contrast_value = getContrastCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Contrast, contrast_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticContrast --> ERROR: Unable to set Contrast property value to " << ac << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getContrast()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long contrast_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Contrast, &contrast_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)contrast_value;
}


void MfController::setContrast(int c)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long contrast_value = c;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Contrast, contrast_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setContrast --> ERROR: Unable to set Contrast property value to " << contrast_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Saturation
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 10,000.
 **************************************************************************/
/*
bool MfController::getAutomaticSaturation()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long saturation_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Saturation, &saturation_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticSaturation(bool as)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long saturation_value = 0L;

    if (as) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        saturation_value = getSaturationCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Saturation, saturation_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticSaturation --> ERROR: Unable to set Saturation property value to " << as << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getSaturation()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long saturation_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Saturation, &saturation_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)saturation_value;
}


void MfController::setSaturation(int s)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long saturation_value = s;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Saturation, saturation_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setSaturation --> ERROR: Unable to set Saturation property value to " << saturation_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Hue
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from –180,000 to 180,000.
 **************************************************************************/
/*
bool MfController::getAutomaticHue()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long hue_value;

    hr = qualityControl->Get(VideoProcAmp_Hue, &hue_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticHue(bool ah)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long hue_value = 0L;

    if (ah) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        hue_value = getHueCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Hue, hue_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticHue --> ERROR: Unable to set Hue property value to " << ah << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getHue()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long hue_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Hue, &hue_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)hue_value;
}


void MfController::setHue(int h)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long hue_value = h;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Hue, hue_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setHue --> ERROR: Unable to set Hue property value to " << hue_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gamma
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 1 to 500.
 **************************************************************************/
/*
bool MfController::getAutomaticGamma()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gamma_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Gamma, &gamma_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticGamma(bool ag)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gamma_value = 0L;

    if (ag) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        gamma_value = getGammaCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Gamma, gamma_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticGamma --> ERROR: Unable to set Gamma property value to " << ag << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getGamma()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gamma_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Gamma, &gamma_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)gamma_value;
}


void MfController::setGamma(int g)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long gamma_value = g;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Gamma, gamma_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setGamma --> ERROR: Unable to set Gamma property value to " << gamma_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Sharpness
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 100.
 **************************************************************************/
/*
bool MfController::getAutomaticSharpness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long sharpness_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Sharpness, &sharpness_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticSharpness(bool as)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long sharpness_value = 0L;

    if (as) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        sharpness_value = getSharpnessCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Sharpness, sharpness_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticSharpness --> ERROR: Unable to set Sharpness property value to " << as << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getSharpness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long sharpness_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Sharpness, &sharpness_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)sharpness_value;
}


void MfController::setSharpness(int s)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long sharpness_value = s;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Sharpness, sharpness_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setSharpness --> ERROR: Unable to set Sharpness property value to " << sharpness_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Backlight Compensation
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Possible values are 0 (off) and 1 (on).
 **************************************************************************/
/*
bool MfController::getAutomaticBacklight()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long backlight_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_BacklightCompensation, &backlight_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticBacklight(bool ab)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long backlight_value = 0L;

    if (ab) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        backlight_value = getBacklightCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_BacklightCompensation, backlight_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticBacklight --> ERROR: Unable to set BacklightCompensation property value to " << ab << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getBacklight()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long backlight_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_BacklightCompensation, &backlight_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)backlight_value;
}


void MfController::setBacklight(int b)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long backlight_value = b;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_BacklightCompensation, backlight_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setBacklight --> ERROR: Unable to set BacklightCompensation property value to " << backlight_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * White Balance
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The range of values depends on the device.
 **************************************************************************/
/*
bool MfController::getAutomaticWhite()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long white_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_WhiteBalance, &white_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticWhite(bool aw)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long white_value = 0L;

    if (aw) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        white_value = getWhiteCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_WhiteBalance, white_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticWhite --> ERROR: Unable to set WhiteBalance property value to " << aw << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getWhite()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long white_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_WhiteBalance, &white_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)white_value;
}


void MfController::setWhite(int w)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long white_value = w;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_WhiteBalance, white_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setWhite --> ERROR: Unable to set WhiteBalance property value to " << white_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gain
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The range of values depends on the device.
 **************************************************************************/
/*
bool MfController::getAutomaticGain()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gain_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Gain, &gain_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticGain(bool ag)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gain_value = 0L;

    if (ag) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        gain_value = getGainCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_Gain, gain_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticGain --> ERROR: Unable to set Gain property value to " << ag << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getGain()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gain_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_Gain, &gain_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)gain_value;
}


void MfController::setGain(int g)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long gain_value = g;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_Gain, gain_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setGain --> ERROR: Unable to set Gain property value to " << gain_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Color Enable
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The possible values are 0 (off) and 1 (on).
 **************************************************************************/
/*
bool MfController::getAutomaticColor()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long color_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_ColorEnable, &color_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticColor(bool ac)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long color_value = 0L;

    if (ac) {
        flags = VideoProcAmp_Flags_Auto;
    }
    else {
        color_value = getColorCaps()->getDefault();
        flags = VideoProcAmp_Flags_Manual;
    }

    hr = qualityControl->Set(VideoProcAmp_ColorEnable, color_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticColor --> ERROR: Unable to set ColorEnable property value to " << ac << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getColor()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long color_value = 0L;

    hr = qualityControl->Get(VideoProcAmp_ColorEnable, &color_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)color_value;
}


void MfController::setColor(int c)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long color_value = c;

    if (qualityControl == NULL) {
        return;
    }
    hr = qualityControl->Set(VideoProcAmp_ColorEnable, color_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setColor --> ERROR: Unable to set ColorEnable property value to " << color_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -x to +y, for example: -3 = 1/8s, -2 = 1/4s, -1 = 1/2s, 0 = 1s, 1 = 2s, 2 = 4s, ...
 **************************************************************************/

bool MfController::getAutomaticExposure()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long exposure_value = 0L;

    hr = cameraControl->Get(CameraControl_Exposure, &exposure_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticExposure(bool ae)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long exposure_value = 0L;

    if (ae) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        exposure_value = getExposureCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Exposure, exposure_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticExposure --> ERROR: Unable to set Exposure property value to " << ae << ". (Error 0x" << hr << ")";
    }
}


int MfController::getExposure()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long exposure_value = 0L;

    hr = cameraControl->Get(CameraControl_Exposure, &exposure_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)exposure_value;
}


void MfController::setExposure(int e)
{
    HRESULT hr = 0;
    long flags = CameraControl_Flags_Manual;
    long exposure_value = e;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Exposure, exposure_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setExposure --> ERROR: Unable to set Exposure property value to " << exposure_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Zoom
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from 10 to 600, and the default is spezific to the device.
 **************************************************************************/

bool MfController::getAutomaticZoom()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long zoom_value = 0L;

    hr = cameraControl->Get(CameraControl_Zoom, &zoom_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticZoom(bool az)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long zoom_value = 0L;

    if (az) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        zoom_value = getZoomCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Zoom, zoom_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticZoom --> ERROR: Unable to set Zoom property value to " << az << ". (Error 0x" << hr << ")";
    }
}

int MfController::getZoom()
{
    HRESULT hr = S_OK;
    long flags = 0;
    long zoom_value = 0L;

    hr = cameraControl->Get(CameraControl_Zoom, &zoom_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)zoom_value;
}


void MfController::setZoom(int z)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long zoom_value = z;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Zoom, zoom_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setZoom --> ERROR: Unable to set Zoom property value to " << zoom_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Focus
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The range and default value are spezific to the device.
 **************************************************************************/

bool MfController::getAutomaticFocus()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long focus_value = 0L;

    hr = cameraControl->Get(CameraControl_Focus, &focus_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticFocus(bool af)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long focus_value = 0L;

    if (af) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        focus_value = getFocusCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Focus, focus_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticFocus --> ERROR: Unable to set Focus property value to " << af << ". (Error 0x" << hr << ")";
    }
}

int MfController::getFocus()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long focus_value = 0L;

    hr = cameraControl->Get(CameraControl_Focus, &focus_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)focus_value;
}


void MfController::setFocus(int f)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long focus_value = f;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Focus, focus_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setFocus --> ERROR: Unable to set Focus property value to " << focus_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Pan
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/
/*
bool MfController::getAutomaticPan()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long pan_value = 0L;

    hr = cameraControl->Get(CameraControl_Pan, &pan_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticPan(bool ap)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long pan_value = 0L;

    if (ap) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        pan_value = getPanCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Pan, pan_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticPan --> ERROR: Unable to set Pan property value to " << ap << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getPan()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long pan_value = 0L;

    hr = cameraControl->Get(CameraControl_Pan, &pan_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)pan_value;
}


void MfController::setPan(int p)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long pan_value = p;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Pan, pan_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setPan --> ERROR: Unable to set Pan property value to " << pan_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Tilt
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/
/*
bool MfController::getAutomaticTilt()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long tilt_value = 0L;

    hr = cameraControl->Get(CameraControl_Tilt, &tilt_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticTilt(bool at)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long tilt_value = 0L;

    if (at) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        tilt_value = getTiltCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Tilt, tilt_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticTilt --> ERROR: Unable to set Tilt property value to " << at << ". (Error 0x" << hr << ")";
    }
}
*/

int MfController::getTilt()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long tilt_value = 0L;

    hr = cameraControl->Get(CameraControl_Tilt, &tilt_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)tilt_value;
}


void MfController::setTilt(int t)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long tilt_value = t;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Tilt, tilt_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setTilt --> ERROR: Unable to set Zoom property value to " << tilt_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Iris
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range in units of fstop * 10.
 **************************************************************************/

bool MfController::getAutomaticIris()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long iris_value = 0L;

    hr = cameraControl->Get(CameraControl_Iris, &iris_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticIris(bool ai)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long iris_value = 0L;

    if (ai) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        iris_value = getZoomCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Iris, iris_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticIris --> ERROR: Unable to set Iris property value to " << ai << ". (Error 0x" << hr << ")";
    }
}


int MfController::getIris()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long iris_value = 0L;

    hr = cameraControl->Get(CameraControl_Iris, &iris_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)iris_value;
}


void MfController::setIris(int i)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long iris_value = i;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Iris, (long)iris_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setIris --> ERROR: Unable to set Iris property value to " << iris_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Roll
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/

bool MfController::getAutomaticRoll()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long roll_value = 0L;

    hr = cameraControl->Get(CameraControl_Roll, &roll_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getAutomaticRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void MfController::setAutomaticRoll(bool ar)
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long roll_value = 0L;

    if (ar) {
        flags = CameraControl_Flags_Auto;
    }
    else {
        roll_value = getRollCaps()->getDefault();
        flags = CameraControl_Flags_Manual;
    }

    hr = cameraControl->Set(CameraControl_Roll, roll_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setAutomaticRoll --> ERROR: Unable to set Roll property value to " << ar << ". (Error 0x" << hr << ")";
    }
}


int MfController::getRoll()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long roll_value = 0L;

    hr = cameraControl->Get(CameraControl_Roll, &roll_value, &flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::getRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)roll_value;
}


void MfController::setRoll(int r)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long roll_value = r;

    if (cameraControl == NULL) {
        return;
    }
    hr = cameraControl->Set(CameraControl_Roll, roll_value, flags);
    Sleep(SLEEP_TIME);
    if (hr != S_OK) {
        qDebug() << "MfController::setRoll --> ERROR: Unable to set Roll property value to " << roll_value << ". (Error 0x" << hr << ")";
    }
}
