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

#include <dshow.h>
#include <Ks.h>				// Required by KsMedia.h
#include <KsMedia.h>		// For KSPROPERTY_CAMERACONTROL_FLAGS_*

#include <QtCore/QtDebug>

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberDirectShowController::GrabberDirectShowController(ImageGrabberDevice *d,
                                     int                 cap)
    : GrabberController(d, cap)
{
    qDebug("GrabberDirectShowController::Constructor --> Start (Empty)");

    // qDebug("GrabberDirectShowController::Constructor --> End");
}


GrabberDirectShowController::~GrabberDirectShowController()
{
    qDebug("GrabberDirectShowController::Destructor --> Start (Empty)");

    // qDebug("GrabberDirectShowController::Destructor --> End");
}


bool GrabberDirectShowController::init()
{
    qDebug("GrabberDirectShowController::init --> Start");

    HRESULT hr;

    printf("Enumerating video input devices ...\n");

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum,
                          (void **)&pSysDevEnum);
    if(FAILED(hr))
    {
        qDebug("GrabberDirectShowController::Constructor --> Error end: Unable to create system device enumerator.\n");
        return false;
    }

    // Obtain a class enumerator for the video input device category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

    if(hr == S_OK)
    {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
        {
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(NULL,
                                         NULL,
                                         IID_IPropertyBag,
                                         (void **)&pPropBag);
            if(SUCCEEDED(hr))
            {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    // Display the name in your UI somehow.
                    qDebug() << "GrabberDirectShowController::init --> Found device: " << varName.bstrVal;
                }
                VariantClear(&varName);

                // To create an instance of the filter, do the following:
                IBaseFilter *pFilter;
                hr = pMoniker->BindToObject(NULL,
                                            NULL,
                                            IID_IBaseFilter,
                                            (void**)&pFilter);

                // Save the control capabilitries
                // process_filter(pFilter);

                // Remember to release pFilter later.
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();

    qDebug("GrabberDirectShowController::init --> End (Successful)");

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
/*
bool GrabberDirectShowController::getAutomaticZoom()
{
    return false;
}


void GrabberDirectShowController::setAutomaticZoom(bool az)
{
    Q_ASSERT( 1 );
}

int GrabberDirectShowController::getZoom()
{
    return 0;
}


void GrabberDirectShowController::setZoom(int z)
{
    Q_ASSERT( 1 );
}
*/
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
