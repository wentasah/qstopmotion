/******************************************************************************
 *  Copyright (C) 2012-2014 by                                                *
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

#include "directshowcontroller.h"
#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

/**************************************************************************
 * Implementation of the controller using the Microsoft DirectShow API.
 **************************************************************************/

DirectShowController::DirectShowController(int cap)
    : GrabberController(cap)
{
    qDebug("DirectShowController::Constructor --> Start");

    pFilter = NULL;
    pCameraControl = NULL;
    pQualityControl = NULL;
    pStreamConfig = NULL;

    qDebug("DirectShowController::Constructor --> End");
}


DirectShowController::~DirectShowController()
{
    qDebug("DirectShowController::Destructor --> Start");
/*
    if (NULL != pStreamConfig) {
        pStreamConfig->Release();
    }
    if (NULL != pQualityControl) {
        pQualityControl->Release();
    }
    if (NULL != pCameraControl) {
        pCameraControl->Release();
    }
    if (NULL != pFilter) {
        pFilter->Release();
    }
*/
    qDebug("DirectShowController::Destructor --> End");
}


bool DirectShowController::init(const QString &id)
{
    qDebug("DirectShowController::init --> Start");

    bool     ret = false;
    HRESULT  hr;

    qDebug("DirectShowController::init --> Enumerating video input devices ...\n");

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum,
                          (void **)&pSysDevEnum);
    if(FAILED(hr)) {
        qDebug("DirectShowController::Constructor --> Error end: Unable to create system device enumerator.\n");
        return false;
    }

    // Obtain a class enumerator for the video input device category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

    if (S_OK == hr) {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((false == ret) && S_OK == (pEnumCat->Next(1, &pMoniker, &cFetched))) {
            IPropertyBag *pPropBag;

            hr = pMoniker->BindToStorage(NULL,
                                         NULL,
                                         IID_IPropertyBag,
                                         (void **)&pPropBag);
            if(FAILED(hr)) {
                // Relese the moniker
                pMoniker->Release();

                continue;
            }

            //---------------------------------------------------------
            // Read the device name
            //---------------------------------------------------------

            QString deviceId;
            VARIANT varName;
            VariantInit(&varName);

            hr = pPropBag->Read(L"FriendlyName", &varName, NULL);

            if (SUCCEEDED(hr)) {
                deviceId = QString((QChar*)(varName.bstrVal), ::SysStringLen(varName.bstrVal));
                VariantClear(&varName);

                // Display the name in your UI somehow.
                qDebug() << "DirectShowController::init --> Found device: " << deviceId;
            }

            if (0 != id.compare(deviceId)) {
                // Release the properties
                pPropBag->Release();

                // Relese the moniker
                pMoniker->Release();

                continue;
            }

            if (NULL != pFilter) {
                // More than one moniker for the same device

                // Release the properties
                pPropBag->Release();

                // Relese the moniker
                pMoniker->Release();

                continue;
            }
            // To create an instance of the filter, do the following:
            hr = pMoniker->BindToObject(NULL,
                                        NULL,
                                        IID_IBaseFilter,
                                        (void**)&pFilter);

            if (FAILED(hr)) {
                // Release the pFilter
                pPropBag->Release();

                // Relese the moniker
                pMoniker->Release();

                continue;
            }

            //---------------------------------------------------------
            // Save the control capabilitries
            //---------------------------------------------------------

            // Get a pointer to the IAMCameraControl interface used to control the camera
            hr = pFilter->QueryInterface(IID_IAMCameraControl, (void **)&pCameraControl);
            if(S_OK == hr) {
                if (setControlCapabilities()) {
                    ret = true;
                }
            }
            else {
                qDebug("DirectShowController::init --> ERROR: Unable to access IAMCameraControl interface.");
            }

            // Get a pointer to the IAMVideoProcAmp interface used to control the camera
            hr = pFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pQualityControl);
            if(S_OK == hr) {
                if (setQualityCapabilities()) {
                    ret = true;
                }
            }
            else {
                qDebug("DirectShowController::init --> ERROR: Unable to access IAMCameraControl interface.");
            }

            //---------------------------------------------------------
            // Read the possible resolutions
            //---------------------------------------------------------

            // get an output pin from the filter
            IEnumPins       *pPinEnum = NULL;
            PIN_DIRECTION    ThisPinDir;
            IEnumMediaTypes *pMediaTypeEnum = NULL;
            AM_MEDIA_TYPE    UsedMediaType;
            AM_MEDIA_TYPE   *pMediaType = NULL;
            VIDEOINFOHEADER *pVideoInfoHeader = NULL;
            IPin            *pCameraPin;
            int              outputWidth;
            int              outputHeight;
            bool             connected;
            // PIN_INFO         PinInfo;
            LPWSTR           pPinId;
            QString          pinId;

            hr = pFilter->EnumPins(&pPinEnum);

            if (FAILED(hr)) {
                // Release the filter
                pFilter->Release();

                // Release the pFilter
                pPropBag->Release();

                // Relese the moniker
                pMoniker->Release();

                continue;
            }

            // Enumerate over the filter pins
            while(S_OK == pPinEnum->Next(1, &pCameraPin, NULL)) {
                pMediaTypeEnum = NULL;
                connected = false;

                pCameraPin->QueryDirection(&ThisPinDir);
                if (ThisPinDir != PINDIR_OUTPUT) {
                    pCameraPin->Release();
                    continue;
                }

                hr = pCameraPin->ConnectionMediaType(&UsedMediaType);
                if (S_OK == hr) {
                    // The pin is connected and has a media type
                    connected = true;
                }

                // hr = pCameraPin->QueryPinInfo(&PinInfo);
                hr = pCameraPin->QueryId(&pPinId);
                if (SUCCEEDED(hr)) {
#ifdef UNICODE
                  // LPWSTR Unicode to QString
                  pinId = QString::fromWCharArray((const wchar_t*)pPinId, strlen(pPinId));
#else
                  // LPWSTR 8 bit to QString
                  pinId = QString::fromLocal8Bit((const char*)pPinId);
#endif
                  // an reverse
                  // std::wstring wstr = pinID.toStdWString();
                  // LPWSTR ptr = wstr.c_str();

                  CoTaskMemFree(pPinId);
                }

                hr = pCameraPin->EnumMediaTypes(&pMediaTypeEnum);
                if (S_OK != hr) {
                    pCameraPin->Release();
                    continue;
                }

                // Enumerate over the media types
                pMediaType = NULL;
                pVideoInfoHeader = NULL;

                while (S_OK == pMediaTypeEnum->Next(1, &pMediaType, NULL))
                {
                    if ((pMediaType->formattype == FORMAT_VideoInfo) &&
                        (pMediaType->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
                        (pMediaType->pbFormat != NULL))
                    {
                        pVideoInfoHeader = (VIDEOINFOHEADER*)pMediaType->pbFormat;
                        outputWidth = pVideoInfoHeader->bmiHeader.biWidth;
                        outputHeight = pVideoInfoHeader->bmiHeader.biHeight;

                        qDebug() << "DirectShowController::init --> Resolution: " << outputWidth << ":" << outputHeight;

                        addResolution(GrabberResolution(pinId, outputWidth, outputHeight, connected));
                    }
                    // _DeleteMediaType(pMediaType);
                }

                pMediaTypeEnum->Release();
                pCameraPin->Release();
            }
            // Release the pin enumerator
            pPinEnum->Release();

            //---------------------------------------------------------
            // Release the data structures
            //---------------------------------------------------------

            // Release the storage
            pPropBag->Release();

            // Relese the moniker
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();

    qDebug("DirectShowController::init --> End (Successful)");

    return ret;
}


bool DirectShowController::setControlCapabilities()
{
    qDebug("DirectShowController::setCameraCapabilities --> Start");

    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Exposure, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Exposure control supported.");

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
        qDebug("DirectShowController::setCameraCapabilities --> Exposure control not supported.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Zoom, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Zoom control supported.");

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
        qDebug("DirectShowController::setCameraCapabilities --> Zoom control not supported.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Focus, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Focus control supported.");

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
        qDebug("DirectShowController::setCameraCapabilities --> Focus control not supported.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Pan, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Pan control supported");

        getPanCaps()->setMinimum(min);
        getPanCaps()->setMaximum(max);
        getPanCaps()->setStep(step);
        getPanCaps()->setDefault(def);
        getPanCaps()->setFlags(flags);
    }
    else {
        qDebug("DirectShowController::setCameraCapabilities --> Pan control not supported");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Tilt, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Tilt control supported");

        getTiltCaps()->setMinimum(min);
        getTiltCaps()->setMaximum(max);
        getTiltCaps()->setStep(step);
        getTiltCaps()->setDefault(def);
        getTiltCaps()->setFlags(flags);
    }
    else {
        qDebug("DirectShowController::setCameraCapabilities --> Tilt control not supported");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Iris, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Iris control supported.");

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
        qDebug("DirectShowController::setCameraCapabilities --> Iris control not supported.");
    }

    Sleep(1000);
    hr = pCameraControl->GetRange(CameraControl_Roll, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setCameraCapabilities --> Roll control supported.");

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
        qDebug("DirectShowController::setCameraCapabilities --> Roll control not supported.");
    }

    qDebug("DirectShowController::setCameraCapabilities --> End");

    return true;
}


bool DirectShowController::setQualityCapabilities()
{
    qDebug("DirectShowController::setQualityCapabilities --> Start");

    long min;
    long max;
    long step;
    long def;
    long flags;
    HRESULT hr;

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Brightness, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Brightness control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Brightness control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Contrast, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Contrast control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Contrast control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Saturation, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Saturation control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Saturation control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Hue, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Hue control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Hue control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Gamma, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Gamma control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Gamma control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Sharpness, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Sharpness control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Sharpness control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_BacklightCompensation, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Backlight Compensation control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Backlight Compensation control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_WhiteBalance, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> White Balance control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> White Balance control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_Gain, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Gain control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Gain control not supported.");
    }

    Sleep(1000);
    hr = pQualityControl->GetRange(VideoProcAmp_ColorEnable, &min, &max, &step, &def, &flags);
    if (hr == S_OK) {
        qDebug("DirectShowController::setQualityCapabilities --> Color Enable control supported.");

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
        qDebug("DirectShowController::setQualityCapabilities --> Color Enable control not supported.");
    }

    qDebug("DirectShowController::setQualityCapabilities --> End");

    return true;
}


/**************************************************************************
 **************************************************************************
 * Camera resolution
 **************************************************************************
 **************************************************************************/

int DirectShowController::getActiveResolution()
{



    /* ========================================================================
     * R.L.: This code work not together with the gstreamer pipeline
     *
    HRESULT          hr = S_OK;
    AM_MEDIA_TYPE   *pMediaType = NULL;
    VIDEOINFOHEADER *pVideoInfoHeader = NULL;
    int              activeWidth;
    int              activeHeight;

    if (NULL == pStreamConfig) {
        //---------------------------------------------------------
        // Save the stream configuration to handle resolution
        //---------------------------------------------------------

        // Get a pointer to the IAMStreamConfig interface used to control the camera resolution
        hr = pFilter->QueryInterface(IID_IAMStreamConfig, (void **)&pStreamConfig);
        if (S_OK == hr) {
            AM_MEDIA_TYPE *pmt = NULL;
            hr = pStreamConfig->GetFormat(&pmt);
            if (SUCCEEDED(hr))
            {
                // Examine the media type for any information you need.
                // DeleteMediaType(pmt);
            }
        }
        else {
            qDebug("DirectShowController::getActiveResolution --> ERROR: Unable to access IAMStreamConfig interface.");
            return -1;
        }
    }

    hr = pStreamConfig->GetFormat(&pMediaType);
    if (SUCCEEDED(hr))
    {
        // Examine the media type for any information you need.

        if ((pMediaType->formattype == FORMAT_VideoInfo) &&
            (pMediaType->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
            (pMediaType->pbFormat != NULL))
        {
            pVideoInfoHeader = (VIDEOINFOHEADER*)pMediaType->pbFormat;
            activeWidth = pVideoInfoHeader->bmiHeader.biWidth;
            activeHeight = pVideoInfoHeader->bmiHeader.biHeight;

            qDebug() << "DirectShowController::getActiveResolution --> active Resolution: " << activeWidth << ":" << activeHeight;

            // addResolution(GrabberResolution(pinId, outputWidth, outputHeight, connected));
        }
        // DeleteMediaType(pmt);
    }
    *
    * ======================================================================== */

    return -1;
}


void DirectShowController::setActiveResolution(int ac)
{

    /* ========================================================================
     * R.L.: This code work not together with the gstreamer pipeline
     *
    HRESULT hr = S_OK;
    GrabberResolution  newResolution;
    int iCount, iSize;
    BYTE *pSCC = NULL;
    AM_MEDIA_TYPE *pmt;

    newResolution = getResolutions().at(ac);

    if (NULL == pStreamConfig) {
        //---------------------------------------------------------
        // Save the stream configuration to handle resolution
        //---------------------------------------------------------

        // Get a pointer to the IAMStreamConfig interface used to control the camera resolution
        hr = pFilter->QueryInterface(IID_IAMStreamConfig, (void **)&pStreamConfig);
        if (S_OK == hr) {
            AM_MEDIA_TYPE *pmt = NULL;
            hr = pStreamConfig->GetFormat(&pmt);
            if (SUCCEEDED(hr))
            {
                // Examine the media type for any information you need.
                // DeleteMediaType(pmt);
            }
        }
        else {
            qDebug("DirectShowController::setActiveResolution --> ERROR: Unable to access IAMStreamConfig interface.");
            return;
        }
    }

    hr = pStreamConfig->GetNumberOfCapabilities(&iCount, &iSize);

    pSCC = new BYTE[iSize];
    if (pSCC == NULL)
    {
        // TODO: Out of memory error.
    }

    // Get the first format.
    hr = pStreamConfig->GetStreamCaps(0, &pmt, pSCC);
    if (hr == S_OK)
    {
        // TODO: Examine the format. If it's not suitable for some
        // reason, call GetStreamCaps with the next index value (up
        // to iCount). Otherwise, set the format:
        hr = pStreamConfig->SetFormat(pmt);
        if (FAILED(hr))
        {
            // TODO: Error handling.
        }
        // DeleteMediaType(pmt);
    }
    delete [] pSCC;
    *
    * ======================================================================== */
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

bool DirectShowController::getAutomaticBrightness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long bright_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Brightness, &bright_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticBrightness(bool ab)
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

    hr = pQualityControl->Set(VideoProcAmp_Brightness, bright_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticBrightness --> ERROR: Unable to set Brightness property value to " << ab << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getBrightness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long bright_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Brightness, &bright_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getBrightness --> ERROR: Unable to get Brightness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)bright_value;
}


void DirectShowController::setBrightness(int b)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long bright_value = b;

    hr = pQualityControl->Set(VideoProcAmp_Brightness, (long)bright_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setBrightness --> ERROR: Unable to set Brightness property value to " << bright_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Contrast
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 10,000.
 **************************************************************************/

bool DirectShowController::getAutomaticContrast()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long contrast_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Contrast, &contrast_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticContrast(bool ac)
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

    hr = pQualityControl->Set(VideoProcAmp_Contrast, contrast_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticContrast --> ERROR: Unable to set Contrast property value to " << ac << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getContrast()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long contrast_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Contrast, &contrast_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getContrast --> ERROR: Unable to get Contrast property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)contrast_value;
}


void DirectShowController::setContrast(int c)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long contrast_value = c;

    hr = pQualityControl->Set(VideoProcAmp_Contrast, contrast_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setContrast --> ERROR: Unable to set Contrast property value to " << contrast_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Saturation
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 10,000.
 **************************************************************************/

bool DirectShowController::getAutomaticSaturation()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long saturation_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Saturation, &saturation_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticSaturation(bool as)
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

    hr = pQualityControl->Set(VideoProcAmp_Saturation, saturation_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticSaturation --> ERROR: Unable to set Saturation property value to " << as << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getSaturation()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long saturation_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Saturation, &saturation_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getSaturation --> ERROR: Unable to get Saturation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)saturation_value;
}


void DirectShowController::setSaturation(int s)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long saturation_value = s;

    hr = pQualityControl->Set(VideoProcAmp_Saturation, saturation_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setSaturation --> ERROR: Unable to set Saturation property value to " << saturation_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Hue
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from –180,000 to 180,000.
 **************************************************************************/

bool DirectShowController::getAutomaticHue()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long hue_value;

    hr = pQualityControl->Get(VideoProcAmp_Hue, &hue_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticHue(bool ah)
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

    hr = pQualityControl->Set(VideoProcAmp_Hue, hue_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticHue --> ERROR: Unable to set Hue property value to " << ah << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getHue()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long hue_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Hue, &hue_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getHue --> ERROR: Unable to get Hue property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)hue_value;
}


void DirectShowController::setHue(int h)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long hue_value = h;

    hr = pQualityControl->Set(VideoProcAmp_Hue, hue_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setHue --> ERROR: Unable to set Hue property value to " << hue_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gamma
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 1 to 500.
 **************************************************************************/

bool DirectShowController::getAutomaticGamma()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gamma_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Gamma, &gamma_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticGamma(bool ag)
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

    hr = pQualityControl->Set(VideoProcAmp_Gamma, gamma_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticGamma --> ERROR: Unable to set Gamma property value to " << ag << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getGamma()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gamma_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Gamma, &gamma_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getGamma --> ERROR: Unable to get Gamma property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)gamma_value;
}


void DirectShowController::setGamma(int g)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long gamma_value = g;

    hr = pQualityControl->Set(VideoProcAmp_Gamma, gamma_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setGamma --> ERROR: Unable to set Gamma property value to " << gamma_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Sharpness
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Values range from 0 to 100.
 **************************************************************************/

bool DirectShowController::getAutomaticSharpness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long sharpness_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Sharpness, &sharpness_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticSharpness(bool as)
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

    hr = pQualityControl->Set(VideoProcAmp_Sharpness, sharpness_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticSharpness --> ERROR: Unable to set Sharpness property value to " << as << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getSharpness()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long sharpness_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Sharpness, &sharpness_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getSharpness --> ERROR: Unable to get Sharpness property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)sharpness_value;
}


void DirectShowController::setSharpness(int s)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long sharpness_value = s;

    hr = pQualityControl->Set(VideoProcAmp_Sharpness, sharpness_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setSharpness --> ERROR: Unable to set Sharpness property value to " << sharpness_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Backlight Compensation
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  Possible values are 0 (off) and 1 (on).
 **************************************************************************/

bool DirectShowController::getAutomaticBacklight()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long backlight_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_BacklightCompensation, &backlight_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticBacklight(bool ab)
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

    hr = pQualityControl->Set(VideoProcAmp_BacklightCompensation, backlight_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticBacklight --> ERROR: Unable to set BacklightCompensation property value to " << ab << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getBacklight()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long backlight_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_BacklightCompensation, &backlight_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getBacklight --> ERROR: Unable to get BacklightCompensation property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)backlight_value;
}


void DirectShowController::setBacklight(int b)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long backlight_value = b;

    hr = pQualityControl->Set(VideoProcAmp_BacklightCompensation, backlight_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setBacklight --> ERROR: Unable to set BacklightCompensation property value to " << backlight_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * White Balance
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The range of values depends on the device.
 **************************************************************************/

bool DirectShowController::getAutomaticWhite()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long white_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_WhiteBalance, &white_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticWhite(bool aw)
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

    hr = pQualityControl->Set(VideoProcAmp_WhiteBalance, white_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticWhite --> ERROR: Unable to set WhiteBalance property value to " << aw << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getWhite()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long white_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_WhiteBalance, &white_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getWhite --> ERROR: Unable to get WhiteBalance property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)white_value;
}


void DirectShowController::setWhite(int w)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long white_value = w;

    hr = pQualityControl->Set(VideoProcAmp_WhiteBalance, white_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setWhite --> ERROR: Unable to set WhiteBalance property value to " << white_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Gain
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The range of values depends on the device.
 **************************************************************************/

bool DirectShowController::getAutomaticGain()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gain_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Gain, &gain_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticGain(bool ag)
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

    hr = pQualityControl->Set(VideoProcAmp_Gain, gain_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticGain --> ERROR: Unable to set Gain property value to " << ag << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getGain()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long gain_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_Gain, &gain_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getGain --> ERROR: Unable to get Gain property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)gain_value;
}


void DirectShowController::setGain(int g)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long gain_value = g;

    hr = pQualityControl->Set(VideoProcAmp_Gain, gain_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setGain --> ERROR: Unable to set Gain property value to " << gain_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Color Enable
 *
 *  http://msdn.microsoft.com/library/windows/desktop/dd407328.aspx:
 *  The possible values are 0 (off) and 1 (on).
 **************************************************************************/

bool DirectShowController::getAutomaticColor()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long color_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_ColorEnable, &color_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & VideoProcAmp_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticColor(bool ac)
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

    hr = pQualityControl->Set(VideoProcAmp_ColorEnable, color_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticColor --> ERROR: Unable to set ColorEnable property value to " << ac << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getColor()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long color_value = 0L;

    hr = pQualityControl->Get(VideoProcAmp_ColorEnable, &color_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getColor --> ERROR: Unable to get ColorEnable property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)color_value;
}


void DirectShowController::setColor(int c)
{
    HRESULT hr = S_OK;
    long flags = VideoProcAmp_Flags_Manual;
    long color_value = c;

    hr = pQualityControl->Set(VideoProcAmp_ColorEnable, color_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setColor --> ERROR: Unable to set ColorEnable property value to " << color_value << ". (Error 0x" << hr << ")";
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

bool DirectShowController::getAutomaticExposure()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long exposure_value = 0L;

    hr = pCameraControl->Get(CameraControl_Exposure, &exposure_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticExposure(bool ae)
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

    hr = pCameraControl->Set(CameraControl_Exposure, exposure_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticExposure --> ERROR: Unable to set Exposure property value to " << ae << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getExposure()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long exposure_value = 0L;

    hr = pCameraControl->Get(CameraControl_Exposure, &exposure_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getExposure --> ERROR: Unable to get Exposure property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)exposure_value;
}


void DirectShowController::setExposure(int e)
{
    HRESULT hr = 0;
    long flags = CameraControl_Flags_Manual;
    long exposure_value = e;

    hr = pCameraControl->Set(CameraControl_Exposure, exposure_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setExposure --> ERROR: Unable to set Exposure property value to " << exposure_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Zoom
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from 10 to 600, and the default is spezific to the device.
 **************************************************************************/

bool DirectShowController::getAutomaticZoom()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long zoom_value = 0L;

    hr = pCameraControl->Get(CameraControl_Zoom, &zoom_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticZoom(bool az)
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

    hr = pCameraControl->Set(CameraControl_Zoom, zoom_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticZoom --> ERROR: Unable to set Zoom property value to " << az << ". (Error 0x" << hr << ")";
    }
}

int DirectShowController::getZoom()
{
    HRESULT hr = S_OK;
    long flags = 0;
    long zoom_value = 0L;

    hr = pCameraControl->Get(CameraControl_Zoom, &zoom_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getZoom --> ERROR: Unable to get Zoom property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)zoom_value;
}


void DirectShowController::setZoom(int z)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long zoom_value = z;

    hr = pCameraControl->Set(CameraControl_Zoom, zoom_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setZoom --> ERROR: Unable to set Zoom property value to " << zoom_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Focus
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The range and default value are spezific to the device.
 **************************************************************************/

bool DirectShowController::getAutomaticFocus()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long focus_value = 0L;

    hr = pCameraControl->Get(CameraControl_Focus, &focus_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticFocus(bool af)
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

    hr = pCameraControl->Set(CameraControl_Focus, focus_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticFocus --> ERROR: Unable to set Focus property value to " << af << ". (Error 0x" << hr << ")";
    }
}

int DirectShowController::getFocus()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long focus_value = 0L;

    hr = pCameraControl->Get(CameraControl_Focus, &focus_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getFocus --> ERROR: Unable to get Focus property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)focus_value;
}


void DirectShowController::setFocus(int f)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long focus_value = f;

    hr = pCameraControl->Set(CameraControl_Focus, focus_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setFocus --> ERROR: Unable to set Focus property value to " << focus_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Pan
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/

bool DirectShowController::getAutomaticPan()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long pan_value = 0L;

    hr = pCameraControl->Get(CameraControl_Pan, &pan_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticPan(bool ap)
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

    hr = pCameraControl->Set(CameraControl_Pan, pan_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticPan --> ERROR: Unable to set Pan property value to " << ap << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getPan()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long pan_value = 0L;

    hr = pCameraControl->Get(CameraControl_Pan, &pan_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getPan --> ERROR: Unable to get Pan property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)pan_value;
}


void DirectShowController::setPan(int p)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long pan_value = p;

    hr = pCameraControl->Set(CameraControl_Pan, pan_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setPan --> ERROR: Unable to set Pan property value to " << pan_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Tilt
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/

bool DirectShowController::getAutomaticTilt()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long tilt_value = 0L;

    hr = pCameraControl->Get(CameraControl_Tilt, &tilt_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticTilt(bool at)
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

    hr = pCameraControl->Set(CameraControl_Tilt, tilt_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticTilt --> ERROR: Unable to set Tilt property value to " << at << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getTilt()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long tilt_value = 0L;

    hr = pCameraControl->Get(CameraControl_Tilt, &tilt_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getTilt --> ERROR: Unable to get Tilt property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)tilt_value;
}


void DirectShowController::setTilt(int t)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long tilt_value = t;

    hr = pCameraControl->Set(CameraControl_Tilt, tilt_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setTilt --> ERROR: Unable to set Zoom property value to " << tilt_value << ". (Error 0x" << hr << ")";
    }
}
/**************************************************************************
 * Iris
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range in units of fstop * 10.
 **************************************************************************/

bool DirectShowController::getAutomaticIris()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long iris_value = 0L;

    hr = pCameraControl->Get(CameraControl_Iris, &iris_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticIris(bool ai)
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

    hr = pCameraControl->Set(CameraControl_Iris, iris_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticIris --> ERROR: Unable to set Iris property value to " << ai << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getIris()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long iris_value = 0L;

    hr = pCameraControl->Get(CameraControl_Iris, &iris_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getIris --> ERROR: Unable to get Iris property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)iris_value;
}


void DirectShowController::setIris(int i)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long iris_value = i;

    hr = pCameraControl->Set(CameraControl_Iris, (long)iris_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setIris --> ERROR: Unable to set Iris property value to " << iris_value << ". (Error 0x" << hr << ")";
    }
}

/**************************************************************************
 * Roll
 *
 *  msdn.microsoft.com/de-de/library/aa925325.aspx:
 *  The value range from -180 to +180, and the default is zero.
 **************************************************************************/

bool DirectShowController::getAutomaticRoll()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long roll_value = 0L;

    hr = pCameraControl->Get(CameraControl_Roll, &roll_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getAutomaticRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return false;
    }

    if (flags & CameraControl_Flags_Auto) {
        return true;
    }

    return false;
}


void DirectShowController::setAutomaticRoll(bool ar)
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

    hr = pCameraControl->Set(CameraControl_Roll, roll_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setAutomaticRoll --> ERROR: Unable to set Roll property value to " << ar << ". (Error 0x" << hr << ")";
    }
}


int DirectShowController::getRoll()
{
    HRESULT hr = S_OK;
    long flags = 0L;
    long roll_value = 0L;

    hr = pCameraControl->Get(CameraControl_Roll, &roll_value, &flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::getRoll --> ERROR: Unable to get Roll property value. (Error 0x" << hr << ")";
        return -1;
    }

    return (int)roll_value;
}


void DirectShowController::setRoll(int r)
{
    HRESULT hr = S_OK;
    long flags = CameraControl_Flags_Manual;
    long roll_value = r;

    hr = pCameraControl->Set(CameraControl_Roll, roll_value, flags);
    Sleep(1000);
    if (hr != S_OK) {
        qDebug() << "DirectShowController::setRoll --> ERROR: Unable to set Roll property value to " << roll_value << ". (Error 0x" << hr << ")";
    }
}
