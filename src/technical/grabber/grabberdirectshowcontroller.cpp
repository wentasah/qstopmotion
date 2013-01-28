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

#include "grabberdirectshowcontroller.h"

#include <QtCore/QtDebug>

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberDirectShowController::GrabberDirectShowController(int cap)
    : GrabberController(cap)
{
    qDebug("GrabberDirectShowController::Constructor --> Start");

    pCameraControl = NULL;
    pQualityControl = NULL;

    qDebug("GrabberDirectShowController::Constructor --> End");
}


GrabberDirectShowController::~GrabberDirectShowController()
{
    qDebug("GrabberDirectShowController::Destructor --> Start (Empty)");

    // qDebug("GrabberDirectShowController::Destructor --> End");
}


bool GrabberDirectShowController::init(const QString &id)
{
    qDebug("GrabberDirectShowController::init --> Start");

    bool ret = false;
    HRESULT hr;

    qDebug("GrabberDirectShowController::init --> Enumerating video input devices ...\n");

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum,
                          (void **)&pSysDevEnum);
    if(FAILED(hr)) {
        qDebug("GrabberDirectShowController::Constructor --> Error end: Unable to create system device enumerator.\n");
        return false;
    }

    // Obtain a class enumerator for the video input device category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

    if (hr == S_OK) {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((ret == false) && (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)) {
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(NULL,
                                         NULL,
                                         IID_IPropertyBag,
                                         (void **)&pPropBag);
            if(SUCCEEDED(hr)) {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                QString deviceId((QChar*)(varName.bstrVal), ::SysStringLen(varName.bstrVal));
                VariantClear(&varName);

                if (SUCCEEDED(hr)) {
                    // Display the name in your UI somehow.
                    qDebug() << "GrabberDirectShowController::init --> Found device: " << deviceId;
                }

                if (id.compare(deviceId) == 0) {
                    // To create an instance of the filter, do the following:
                    IBaseFilter *pFilter;
                    hr = pMoniker->BindToObject(NULL,
                                                NULL,
                                                IID_IBaseFilter,
                                                (void**)&pFilter);

                    // Save the control capabilitries

                    // Get a pointer to the IAMCameraControl interface used to control the camera
                    hr = pFilter->QueryInterface(IID_IAMCameraControl, (void **)&pCameraControl);
                    if(hr == S_OK) {
                        if (setControlCapabilities())
                        {
                            ret = true;
                        }
                    }
                    else {
                        qDebug("GrabberDirectShowController::init --> ERROR: Unable to access IAMCameraControl interface.");
                    }

                    // Get a pointer to the IAMVideoProcAmp interface used to control the camera
                    hr = pFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pQualityControl);
                    if(hr == S_OK) {
                        if (setQualityCapabilities())
                        {
                            ret = true;
                        }
                    }
                    else {
                        qDebug("GrabberDirectShowController::init --> ERROR: Unable to access IAMCameraControl interface.");
                    }

                    // Remember to release pFilter later.
                    pPropBag->Release();
                }
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();

    qDebug("GrabberDirectShowController::init --> End (Successful)");

    return ret;
}


bool GrabberDirectShowController::setControlCapabilities()
{
    qDebug("GrabberDirectShowController::setCameraCapabilities --> Start");

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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Exposure property information.");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Zoom property information.");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Focus property information.\n");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Pan property information.\n");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Tilt property information.\n");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Iris property information.");
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
        qDebug("GrabberDirectShowController::setCameraCapabilities --> Unable to retrieve Roll property information.");
    }

    qDebug("GrabberDirectShowController::setCameraCapabilities --> End");

    return true;
}


bool GrabberDirectShowController::setQualityCapabilities()
{
    qDebug("GrabberDirectShowController::setQualityCapabilities --> Start");

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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Brightness property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Contrast property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Saturation property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Hue property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Gamma property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Sharpness property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Backlight Compensation property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve White Balance property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Gain property information.");
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
        qDebug("GrabberDirectShowController::setQualityCapabilities --> Unable to retrieve Color Enable property information.");
    }

    qDebug("GrabberDirectShowController::setQualityCapabilities --> End");

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

bool GrabberDirectShowController::getAutomaticBrightness()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long b;

    hr = pQualityControl->Get(VideoProcAmp_Brightness, &b, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticBrightness(bool ab)
{
    HRESULT hr = 0;
    long flags;
    long b = 0;

    if (ab) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        b = getBrightnessCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Brightness, b, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticBrightness --> ERROR: Unable to set Brightness property value to " << ab << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getBrightness()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long b;

    hr = pQualityControl->Get(VideoProcAmp_Brightness, &b, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)b;
}


void GrabberDirectShowController::setBrightness(int b)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Brightness, (long)b, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setBrightness --> ERROR: Unable to set Brightness property value to " << b << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Contrast
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticContrast()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long c;

    hr = pQualityControl->Get(VideoProcAmp_Contrast, &c, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticContrast(bool ac)
{
    HRESULT hr = 0;
    long flags;
    long c = 0;

    if (ac) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        c = getContrastCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Contrast, c, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticContrast --> ERROR: Unable to set Contrast property value to " << ac << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getContrast()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long c;

    hr = pQualityControl->Get(VideoProcAmp_Contrast, &c, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)c;
}


void GrabberDirectShowController::setContrast(int c)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Contrast, (long)c, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setContrast --> ERROR: Unable to set Contrast property value to " << c << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Saturation
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticSaturation()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long s;

    hr = pQualityControl->Get(VideoProcAmp_Saturation, &s, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticSaturation(bool as)
{
    HRESULT hr = 0;
    long flags;
    long s = 0;

    if (as) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        s = getSaturationCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Saturation, s, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticSaturation --> ERROR: Unable to set Saturation property value to " << as << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getSaturation()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long s;

    hr = pQualityControl->Get(VideoProcAmp_Saturation, &s, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)s;
}


void GrabberDirectShowController::setSaturation(int s)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Saturation, (long)s, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setSaturation --> ERROR: Unable to set Saturation property value to " << s << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Hue
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticHue()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long h;

    hr = pQualityControl->Get(VideoProcAmp_Hue, &h, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticHue(bool ah)
{
    HRESULT hr = 0;
    long flags;
    long h = 0;

    if (ah) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        h = getHueCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Hue, h, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticHue --> ERROR: Unable to set Hue property value to " << ah << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getHue()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long h;

    hr = pQualityControl->Get(VideoProcAmp_Hue, &h, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)h;
}


void GrabberDirectShowController::setHue(int h)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Hue, (long)h, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setHue --> ERROR: Unable to set Hue property value to " << h << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gamma
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticGamma()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long g;

    hr = pQualityControl->Get(VideoProcAmp_Gamma, &g, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticGamma(bool ag)
{
    HRESULT hr = 0;
    long flags;
    long g = 0;

    if (ag) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        g = getGammaCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Gamma, g, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticGamma --> ERROR: Unable to set Gamma property value to " << ag << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getGamma()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long g;

    hr = pQualityControl->Get(VideoProcAmp_Gamma, &g, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)g;
}


void GrabberDirectShowController::setGamma(int g)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Gamma, (long)g, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setGamma --> ERROR: Unable to set Gamma property value to " << g << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Sharpness
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticSharpness()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long s;

    hr = pQualityControl->Get(VideoProcAmp_Sharpness, &s, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticSharpness(bool as)
{
    HRESULT hr = 0;
    long flags;
    long s = 0;

    if (as) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        s = getSharpnessCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Sharpness, s, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticSharpness --> ERROR: Unable to set Sharpness property value to " << as << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getSharpness()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long s;

    hr = pQualityControl->Get(VideoProcAmp_Sharpness, &s, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)s;
}


void GrabberDirectShowController::setSharpness(int s)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Sharpness, (long)s, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setSharpness --> ERROR: Unable to set Sharpness property value to " << s << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Backlight Compensation
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticBacklight()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long b;

    hr = pQualityControl->Get(VideoProcAmp_BacklightCompensation, &b, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticBacklight(bool ab)
{
    HRESULT hr = 0;
    long flags;
    long b = 0;

    if (ab) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        b = getBacklightCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_BacklightCompensation, b, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticBacklight --> ERROR: Unable to set BacklightCompensation property value to " << ab << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getBacklight()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long b;

    hr = pQualityControl->Get(VideoProcAmp_BacklightCompensation, &b, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)b;
}


void GrabberDirectShowController::setBacklight(int b)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_BacklightCompensation, (long)b, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setBacklight --> ERROR: Unable to set BacklightCompensation property value to " << b << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * White Balance
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticWhite()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long w;

    hr = pQualityControl->Get(VideoProcAmp_WhiteBalance, &w, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticWhite(bool aw)
{
    HRESULT hr = 0;
    long flags;
    long w = 0;

    if (aw) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        w = getWhiteCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_WhiteBalance, w, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticWhite --> ERROR: Unable to set WhiteBalance property value to " << aw << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getWhite()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long w;

    hr = pQualityControl->Get(VideoProcAmp_WhiteBalance, &w, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)w;
}


void GrabberDirectShowController::setWhite(int w)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_WhiteBalance, (long)w, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setWhite --> ERROR: Unable to set WhiteBalance property value to " << w << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gain
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticGain()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long g;

    hr = pQualityControl->Get(VideoProcAmp_Gain, &g, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticGain(bool ag)
{
    HRESULT hr = 0;
    long flags;
    long g = 0;

    if (ag) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        g = getGainCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_Gain, g, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticGain --> ERROR: Unable to set Gain property value to " << ag << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getGain()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long g;

    hr = pQualityControl->Get(VideoProcAmp_Gain, &g, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)g;
}


void GrabberDirectShowController::setGain(int g)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_Gain, (long)g, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setGain --> ERROR: Unable to set Gain property value to " << g << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Color Enable
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticColor()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long b;

    hr = pQualityControl->Get(VideoProcAmp_ColorEnable, &b, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticColor(bool ac)
{
    HRESULT hr = 0;
    long flags;
    long c = 0;

    if (ac) {
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
    }
    else {
        c = getColorCaps()->getDefault();
        flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    }

    hr = pQualityControl->Set(VideoProcAmp_ColorEnable, c, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticColor --> ERROR: Unable to set ColorEnable property value to " << ac << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getColor()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
    long c;

    hr = pQualityControl->Get(VideoProcAmp_ColorEnable, &c, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)c;
}


void GrabberDirectShowController::setColor(int c)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;

    hr = pQualityControl->Set(VideoProcAmp_ColorEnable, (long)c, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setColor --> ERROR: Unable to set ColorEnable property value to " << c << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticExposure()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long e;

    hr = pCameraControl->Get(CameraControl_Exposure, &e, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticExposure(bool ae)
{
    HRESULT hr = 0;
    long flags;
    long e = 0;

    if (ae) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        e = getExposureCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Exposure, e, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticExposure --> ERROR: Unable to set Exposure property value to " << ae << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getExposure()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long e;

    hr = pCameraControl->Get(CameraControl_Exposure, &e, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)e;
}


void GrabberDirectShowController::setExposure(int e)
{
    HRESULT hr = 0;
    // long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_RELATIVE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Exposure, (long)e, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setExposure --> ERROR: Unable to set Exposure property value to " << e << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Zoom
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticZoom()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticZoom(bool az)
{
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
        qDebug() << "GrabberDirectShowController::setAutomaticZoom --> ERROR: Unable to set Zoom property value to " << az << ". (Error 0x" << hr << ")";
    }
}

int GrabberDirectShowController::getZoom()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)z;
}


void GrabberDirectShowController::setZoom(int z)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Zoom, (long)z, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setZoom --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Focus
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticFocus()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long f;

    hr = pCameraControl->Get(CameraControl_Focus, &f, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticFocus(bool af)
{
    HRESULT hr = 0;
    long flags;
    long f = 0;

    if (af) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        f = getFocusCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Focus, f, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticFocus --> ERROR: Unable to set Focus property value to " << af << ". (Error 0x" << hr << ")";
    }
}

int GrabberDirectShowController::getFocus()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long f;

    hr = pCameraControl->Get(CameraControl_Focus, &f, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)f;
}


void GrabberDirectShowController::setFocus(int f)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Focus, (long)f, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setFocus --> ERROR: Unable to set Focus property value to " << f << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Pan
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticPan()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long p;

    hr = pCameraControl->Get(CameraControl_Pan, &p, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticPan(bool ap)
{
    HRESULT hr = 0;
    long flags;
    long p = 0;

    if (ap) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        p = getPanCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Pan, p, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticPan --> ERROR: Unable to set Pan property value to " << ap << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getPan()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long p;

    hr = pCameraControl->Get(CameraControl_Pan, &p, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)p;
}


void GrabberDirectShowController::setPan(int p)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Pan, (long)p, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setPan --> ERROR: Unable to set Pan property value to " << p << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Tilt
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticTilt()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long t;

    hr = pCameraControl->Get(CameraControl_Tilt, &t, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticTilt(bool at)
{
    HRESULT hr = 0;
    long flags;
    long t = 0;

    if (at) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        t = getTiltCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Tilt, t, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticTilt --> ERROR: Unable to set Tilt property value to " << at << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getTilt()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long t;

    hr = pCameraControl->Get(CameraControl_Tilt, &t, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)t;
}


void GrabberDirectShowController::setTilt(int t)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Tilt, (long)t, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setTilt --> ERROR: Unable to set Zoom property value to " << t << ". (Error 0x" << hr << ")";
    }
}
/**************************************************************************
 * Iris
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticIris()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long i;

    hr = pCameraControl->Get(CameraControl_Iris, &i, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticIris(bool ai)
{
    HRESULT hr = 0;
    long flags;
    long i = 0;

    if (ai) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        i = getZoomCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Iris, i, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticIris --> ERROR: Unable to set Iris property value to " << ai << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getIris()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long i;

    hr = pCameraControl->Get(CameraControl_Iris, &i, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)i;
}


void GrabberDirectShowController::setIris(int i)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Iris, (long)i, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setIris --> ERROR: Unable to set Iris property value to " << i << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Roll
 **************************************************************************/

bool GrabberDirectShowController::getAutomaticRoll()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long r;

    hr = pCameraControl->Get(CameraControl_Roll, &r, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::getAutomaticRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberDirectShowController::setAutomaticRoll(bool ar)
{
    HRESULT hr = 0;
    long flags;
    long r = 0;

    if (ar) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        r = getRollCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Roll, r, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::setAutomaticRoll --> ERROR: Unable to set Roll property value to " << ar << ". (Error 0x" << hr << ")";
    }
}


int GrabberDirectShowController::getRoll()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long r;

    hr = pCameraControl->Get(CameraControl_Roll, &r, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::getRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)r;
}


void GrabberDirectShowController::setRoll(int r)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Roll, (long)r, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::setRoll --> ERROR: Unable to set Roll property value to " << r << ". (Error 0x" << hr << ")";
    }
}
