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

    qDebug("GrabberDirectShowController::Constructor --> End");
}


GrabberDirectShowController::~GrabberDirectShowController()
{
    qDebug("GrabberDirectShowController::Destructor --> Start (Empty)");

    // qDebug("GrabberDirectShowController::Destructor --> End");
}


bool GrabberDirectShowController::init(const QString id)
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
                        if (setCapabilities())
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


bool GrabberDirectShowController::setCapabilities()
{
    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Zoom, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getZoomCaps()->setMinimum(min);
        getZoomCaps()->setMaximum(max);
        getZoomCaps()->setStep(step);
        getZoomCaps()->setDef(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getZoomCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberDirectShowController::init --> Unable to retrieve Zoom property information.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Focus, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getFocusCaps()->setMinimum(min);
        getFocusCaps()->setMaximum(max);
        getFocusCaps()->setStep(step);
        getFocusCaps()->setDef(def);
        if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO) {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Auto);
        }
        else {
            getFocusCaps()->setFlags(GrabberControlCapabilities::control_Manual);
        }
    }
    else {
        qDebug("GrabberDirectShowController::init --> Unable to retrieve Focus property information.\n");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Pan, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getPanCaps()->setMinimum(min);
        getPanCaps()->setMaximum(max);
        getPanCaps()->setStep(step);
        getPanCaps()->setDef(def);
        getPanCaps()->setFlags(flags);
    }
    else {
        qDebug("GrabberDirectShowController::init --> Unable to retrieve Pan property information.\n");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Tilt, &min, &max, &step, &def, &flags);
    if(hr == S_OK) {
        getTiltCaps()->setMinimum(min);
        getTiltCaps()->setMaximum(max);
        getTiltCaps()->setStep(step);
        getTiltCaps()->setDef(def);
        getTiltCaps()->setFlags(flags);
    }
    else {
        qDebug("GrabberDirectShowController::init --> Unable to retrieve Tilt property information.\n");
    }

    return true;
}


/**************************************************************************
 **************************************************************************
 * Camera capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/
/*
int GrabberDirectShowController::getBrightness()
{
    return 0;
}


void GrabberDirectShowController::setBrightness(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Contrast
 **************************************************************************/
/*
int GrabberDirectShowController::getContrast()
{
    return 0;
}


void GrabberDirectShowController::setContrast(int c)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Saturation
 **************************************************************************/
/*
int GrabberDirectShowController::getSaturation()
{
    return 0;
}


void GrabberDirectShowController::setSaturation(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Hue
 **************************************************************************/
/*
int GrabberDirectShowController::getHue()
{
    return 0;
}


void GrabberDirectShowController::setHue(int h)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gamma
 **************************************************************************/
/*
int GrabberDirectShowController::getGamma()
{
    return 0;
}


void GrabberDirectShowController::setGamma(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
int GrabberDirectShowController::getSharpness()
{
    return 0;
}


void GrabberDirectShowController::setSharpness(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
int GrabberDirectShowController::getBacklight()
{
    return 0;
}


void GrabberDirectShowController::setBacklight(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Exposure
 **************************************************************************/
/*
bool GrabberDirectShowController::getAutomaticExposure()
{
    return false;
}


void GrabberDirectShowController::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}


bool GrabberDirectShowController::isExposure()
{
    return false;
}


void GrabberDirectShowController::setExposure(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool GrabberDirectShowController::getAutomaticWhite()
{
    return false;
}


void GrabberDirectShowController::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}

int GrabberDirectShowController::getWhite()
{
    return 0;
}


void GrabberDirectShowController::setWhite(int w)
{
    Q_ASSERT( 1 );
}
*/
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
        qDebug() << "GrabberDirectShowController::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
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
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Zoom, (long)z, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberDirectShowController::Constructor --> ERROR: Unable to set CameraControl_Zoom property value to " << z << ". (Error 0x" << hr;
    }
}

int GrabberDirectShowController::getZoom()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberDirectShowController::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
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
        qDebug() << "GrabberDirectShowController::Constructor --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr;
    }
}

/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool GrabberDirectShowController::getAutomaticFocus()
{
    return false;
}


void GrabberDirectShowController::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}

int GrabberDirectShowController::getFocus()
{
    return 0;
}


void GrabberDirectShowController::setFocus(int f)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Pan
 **************************************************************************/

int GrabberDirectShowController::getPan()
{
    return 0;
}


void GrabberDirectShowController::setPan(int p)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 * Tilt
 **************************************************************************/

int GrabberDirectShowController::getTilt()
{
    return 0;
}


void GrabberDirectShowController::setTilt(int t)
{
    Q_ASSERT( 1 );
}
