/******************************************************************************
 *  Copyright (C) 2005-2016 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
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

#include "imagegrabberfacade.h"

#include <QDebug>

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  #include "technical/grabber/mediafoundation/mfgrabber.h"
#endif

#ifdef Q_OS_LINUX
  #include "technical/grabber/gphoto2/gpgrabber.h"
  #include "technical/grabber/video4linux2/v4l2grabber.h"
#endif

#include "technical/preferencestool.h"


ImageGrabberFacade::ImageGrabberFacade(Frontend *f)
{
    qDebug() << "ImageGrabberFacade::Constructor --> Start";

    frontend = f;

    grabberThread = NULL;

    isInitialized = false;
    isInited = false;

    gphotoGrabber = NULL;
    v4l2Grabber = NULL;

    mediaFoundationGrabber = NULL;

    qDebug() << "ImageGrabberFacade::Constructor --> End";
}


ImageGrabberFacade::~ImageGrabberFacade()
{
    qDebug() << "ImageGrabberFacade::Destructor --> Start";

    clearDevices();

    if (gphotoGrabber != NULL) {
        delete gphotoGrabber;
        gphotoGrabber = NULL;
    }
    if (v4l2Grabber != NULL) {
        delete v4l2Grabber;
        v4l2Grabber = NULL;
    }
    if (mediaFoundationGrabber != NULL) {
        delete mediaFoundationGrabber;
        mediaFoundationGrabber = NULL;
    }

    qDebug() << "ImageGrabberFacade::Destructor --> End";
}


void ImageGrabberFacade::newDeviceSet()
{
    // Create a new instance of the device

    // Initialize the grabber controller of the device
}


void ImageGrabberFacade::clearDevices()
{
    qDebug() << "ImageGrabberFacade::clearDevices --> Start";

    unsigned int deviceSize = devices.size();
    for (unsigned int deviceIndex = 0; deviceIndex < deviceSize; ++deviceIndex) {
        delete devices[deviceIndex];
        devices[deviceIndex] = NULL;
    }
    devices.clear();

    qDebug() << "ImageGrabberFacade::clearDevices --> End";
}


void ImageGrabberFacade::initialization()
{
    qDebug() << "ImageGrabberFacade::initialization --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

    clearDevices();

#ifdef Q_OS_LINUX
    // video4linux2 device
    if (pref->getIntegerPreference("preferences", "v4l2grabber", value) == false) {
        value = true;
    }
    if ((int)true == value) {
        v4l2Grabber = new V4L2Grabber(frontend);
        if (v4l2Grabber->initialization(devices)) {
            isInitialized = true;
        }
    }
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Microsoft Media Foundation device
    if (pref->getIntegerPreference("preferences", "mediafoundationgrabber", value) == false) {
        value = true;
    }
    if ((int)true == value) {
        mediaFoundationGrabber = new MfGrabber(frontend);
        if (mediaFoundationGrabber->initialization(devices)) {
            isInitialized = true;
        }
    }
#endif

#ifdef Q_OS_LINUX
    // gphoto2 device
    if (pref->getIntegerPreference("preferences", "gphoto2grabber", value) == false) {
        value = false;
    }
    if ((int)true == value) {
        gphotoGrabber = new GphotoGrabber(frontend);
        if (gphotoGrabber->initialization(devices)) {
            isInitialized = true;
        }
    }
#endif

    qDebug() << "ImageGrabberFacade::initialization --> End";
}


void ImageGrabberFacade::init()
{
    qDebug() << "ImageGrabberFacade::init --> Start";

    if (0 == getDeviceCount()) {
        frontend->showWarning(tr("Check image sources"),
                              tr("No usable video image source found. Please\n"
                                 "connect on video device on the computer."));
        return;
    }

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::video4Linux2Source:
        isInited = v4l2Grabber->setUp();

        break;
    case ImageGrabberDevice::gphoto2Source:
        isInited = gphotoGrabber->setUp();

        break;
    case ImageGrabberDevice::mediaFoundationSource:
        isInited = mediaFoundationGrabber->setUp();

        break;
    default:

        break;
    }

    if (!isGrabberInited()) {
        frontend->showWarning(tr("Check image grabber"),
                              tr("Image grabber initialization failed. This may happen \n"
                                 "if you try to grab from an invalid device. Please\n"
                                 "select another device on the recording tool tab."));
        return;
    }

    qDebug() << "ImageGrabberFacade::init --> End";
}


void ImageGrabberFacade::finalize()
{
    qDebug() << "ImageGrabberFacade::finalize --> Start";

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    isInitialized = false;
    isInited = false;

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::video4Linux2Source:
        v4l2Grabber->tearDown();
        break;
    case ImageGrabberDevice::gphoto2Source:
        gphotoGrabber->tearDown();
        break;
    case ImageGrabberDevice::mediaFoundationSource:
        mediaFoundationGrabber->tearDown();
        break;
    default:
        break;
    }

    qDebug() << "ImageGrabberFacade::finalize --> End";
}


const int ImageGrabberFacade::getDeviceCount()
{
    return devices.count();
}


const QVector<ImageGrabberDevice*> ImageGrabberFacade::getDevices()
{
    return devices;
}


ImageGrabberDevice* ImageGrabberFacade::getDevice(int deviceIndex)
{
    return devices[deviceIndex];
}


const QVector<QString> ImageGrabberFacade::getDeviceNames()
{
    QVector<QString> deviceNames;

    for (int deviceIndex = 0 ; deviceIndex < devices.size() ; deviceIndex++) {
        deviceNames.append(devices[deviceIndex]->getDeviceName());
    }

    return deviceNames;
}


const QVector<QString> ImageGrabberFacade::getResolutionNames(int deviceIndex)
{
    QVector<QString>           resNames;
    ImageGrabberDevice*        device = devices[deviceIndex];
    QVector<GrabberResolution> resolutions = device->getResolutions();
    int                        resSize = resolutions.size();

    for (int resIndex = 0 ; resIndex < resSize ; resIndex++) {
        resNames.append(resolutions[resIndex].getName());
    }

    return resNames;
}


const GrabberResolution ImageGrabberFacade::getResolution(int deviceIndex, int resIndex)
{
    ImageGrabberDevice*        device = devices[deviceIndex];
    QVector<GrabberResolution> resolutions = device->getResolutions();
    int                        resSize = resolutions.size();

    return resolutions[resIndex];
}


bool ImageGrabberFacade::isGrabberInitialized() const
{
    return isInitialized;
}


bool ImageGrabberFacade::isGrabberInited() const
{
    return isInited;
}


const QImage ImageGrabberFacade::getLiveImage()
{
    QImage liveImage;

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::video4Linux2Source:
        liveImage = v4l2Grabber->getLiveImage();
        break;
    case ImageGrabberDevice::gphoto2Source:
        liveImage = gphotoGrabber->getLiveImage();
        break;
    case ImageGrabberDevice::mediaFoundationSource:
        liveImage = mediaFoundationGrabber->getLiveImage();
        break;
    default:
        break;
    }

    return liveImage;
}


const QImage ImageGrabberFacade::getRawImage()
{
    QImage rawImage;

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::video4Linux2Source:
        rawImage = v4l2Grabber->getRawImage();
        break;
    case ImageGrabberDevice::gphoto2Source:
        rawImage = gphotoGrabber->getRawImage();
        break;
    case ImageGrabberDevice::mediaFoundationSource:
        rawImage = mediaFoundationGrabber->getRawImage();
        break;
    default:
        break;
    }

    return rawImage;
}
