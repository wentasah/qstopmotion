/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include <QtCore/QtDebug>


ImageGrabberFacade::ImageGrabberFacade(Frontend *f)
{
    qDebug("ImageGrabberFacade::Constructor --> Start");

    frontend = f;

    grabberThread = NULL;

    isInitialized = false;
    isInited = false;
    isProcess = false;

    gstreamerVideoTestGrabber = NULL;
    gstreamerV4L2Grabber = NULL;
    gstreamerDv1394Grabber = NULL;
    gstreamerDirectShowUsbGrabber = NULL;

    qDebug("ImageGrabberFacade::Constructor --> End");
}


ImageGrabberFacade::~ImageGrabberFacade()
{
    qDebug("ImageGrabberFacade::Destructor --> Start");

    clearDevices();

    if (gstreamerVideoTestGrabber != NULL) {
        delete gstreamerVideoTestGrabber;
        gstreamerVideoTestGrabber = NULL;
    }
    if (gstreamerV4L2Grabber != NULL) {
        delete gstreamerV4L2Grabber;
        gstreamerV4L2Grabber = NULL;
    }
    if (gstreamerDv1394Grabber != NULL) {
        delete gstreamerDv1394Grabber;
        gstreamerDv1394Grabber = NULL;
    }
    if (gstreamerDirectShowUsbGrabber != NULL) {
        delete gstreamerDirectShowUsbGrabber;
        gstreamerDirectShowUsbGrabber = NULL;
    }

    qDebug("ImageGrabberFacade::Destructor --> End");
}


void ImageGrabberFacade::clearDevices()
{
    qDebug("ImageGrabberFacade::clearDevices --> Start");

    unsigned int deviceSize = devices.size();
    for (unsigned int deviceIndex = 0; deviceIndex < deviceSize; ++deviceIndex) {
        delete devices[deviceIndex];
        devices[deviceIndex] = NULL;
    }
    devices.clear();

    qDebug("ImageGrabberFacade::clearDevices --> End");
}


void ImageGrabberFacade::initialization()
{
    qDebug("ImageGrabberFacade::initialization --> Start");

    clearDevices();

    gstreamerVideoTestGrabber = new GstreamerVideoTestGrabber(frontend);
    gstreamerV4L2Grabber = new GstreamerV4L2Grabber(frontend);
    gstreamerDv1394Grabber = new GstreamerDv1394Grabber(frontend);
    gstreamerDirectShowUsbGrabber = new GstreamerDirectShowUsbGrabber(frontend);

    if (gstreamerVideoTestGrabber->initializationSubclass(devices)) {
        isInitialized = true;
    }
    if (gstreamerV4L2Grabber->initializationSubclass(devices)) {
        isInitialized = true;
    }
    if (gstreamerDv1394Grabber->initializationSubclass(devices)) {
        isInitialized = true;
    }
    if (gstreamerDirectShowUsbGrabber->initializationSubclass(devices)) {
        isInitialized = true;
    }

    isProcess = true;

    qDebug("ImageGrabberFacade::initialization --> End");
}


void ImageGrabberFacade::init()
{
    qDebug() << "ImageGrabberFacade::init --> Start";

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::testSource:
        isInited = gstreamerVideoTestGrabber->initSubclass();

        if (!isGrabberInited()) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }

        break;
    case ImageGrabberDevice::video4LinuxSource:
        isInited = gstreamerV4L2Grabber->initSubclass();

        if (!isGrabberInited()) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }

        break;
    case ImageGrabberDevice::ieee1394Source:
        isInited = gstreamerDv1394Grabber->initSubclass();

        if (!isGrabberInited()) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }

        break;
    case ImageGrabberDevice::directShowUsbSource:
    case ImageGrabberDevice::directShow1394Source:
        isInited = gstreamerDirectShowUsbGrabber->initSubclass();

        if (!isGrabberInited()) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }

        break;
    default:

        break;
    }

    /*
    // If the grabber is running in it's own process we use a timer.
    if (isGrabberProcess()) {
        isInited = gstreamerVideoTestGrabber->initSubclass();

        if (!isGrabberInited()) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }
    }
    // Otherwise a thread is needed
    else {
        grabberThread = new ImageGrabberThread(gstreamerVideoTestGrabber);

        grabberThread->start();
        grabberThread->wait(500);

        if (grabberThread->wasGrabbingSuccess() == false) {
            frontend->showWarning(tr("Check image grabber"),
                                  tr("Grabbing failed. This may happen if you try\n"
                                     "to grab from an invalid device. Please check\n"
                                     "your grabber settings in the preferences menu."));
            return;
        }
    }
    */

    qDebug() << "ImageGrabberFacade::init --> End";
}


void ImageGrabberFacade::finalize()
{
    qDebug() << "ImageGrabberFacade::finalize --> Start";

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    isInitialized = false;
    isInited = false;
    isProcess = false;

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::testSource:
        gstreamerVideoTestGrabber->tearDown();
        break;
    case ImageGrabberDevice::video4LinuxSource:
        gstreamerV4L2Grabber->tearDown();
        break;
    case ImageGrabberDevice::ieee1394Source:
        gstreamerDv1394Grabber->tearDown();
        break;
    case ImageGrabberDevice::directShowUsbSource:
    case ImageGrabberDevice::directShow1394Source:
        gstreamerDirectShowUsbGrabber->tearDown();
        break;
    default:
        break;
    }

    /*
    if (isGrabberProcess()) {
        gstreamerVideoTestGrabber->tearDown();
    }
    else {
        if (grabberThread != 0) {
            grabberThread->terminate();
            grabberThread->wait();
            delete grabberThread;
            grabberThread = 0;
        }
    }
    */

    qDebug() << "ImageGrabberFacade::finalize --> End";
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


bool ImageGrabberFacade::isGrabberInitialized() const
{
    return isInitialized;
}


bool ImageGrabberFacade::isGrabberInited() const
{
    return isInited;
}


bool ImageGrabberFacade::isGrabberProcess() const
{
    return isProcess;
}


bool ImageGrabberFacade::isController() const
{
    return false;
}


GrabberController* ImageGrabberFacade::getController()
{
    return NULL;
}


const QImage ImageGrabberFacade::getLiveImage()
{
    QImage liveImage;

    int videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = getDevice(videoSource);

    switch (videoDevice->getDeviceSource()) {
    case ImageGrabberDevice::testSource:
        liveImage = gstreamerVideoTestGrabber->getLiveImage();
        break;
    case ImageGrabberDevice::video4LinuxSource:
        liveImage = gstreamerV4L2Grabber->getLiveImage();
        break;
    case ImageGrabberDevice::ieee1394Source:
        liveImage = gstreamerDv1394Grabber->getLiveImage();
        break;
    case ImageGrabberDevice::directShowUsbSource:
    case ImageGrabberDevice::directShow1394Source:
        liveImage = gstreamerDirectShowUsbGrabber->getLiveImage();
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
    case ImageGrabberDevice::testSource:
        rawImage = gstreamerVideoTestGrabber->getRawImage();
        break;
    case ImageGrabberDevice::video4LinuxSource:
        rawImage = gstreamerV4L2Grabber->getRawImage();
        break;
    case ImageGrabberDevice::ieee1394Source:
        rawImage = gstreamerDv1394Grabber->getRawImage();
        break;
    case ImageGrabberDevice::directShowUsbSource:
    case ImageGrabberDevice::directShow1394Source:
        rawImage = gstreamerDirectShowUsbGrabber->getRawImage();
        break;
    default:
        break;
    }

    return rawImage;
}
