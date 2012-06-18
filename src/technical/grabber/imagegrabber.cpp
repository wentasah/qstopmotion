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

#include "imagegrabber.h"

#include <QtCore/QtDebug>

ImageGrabber::ImageGrabber(Frontend *f)
{
    qDebug("ImageGrabber::Constructor --> Start");

    grabberThread = 0;

    isInitialized = false;
    isInited = false;
    isProcess = false;

    frontend = f;

    qDebug("ImageGrabber::Constructor --> End");
}


ImageGrabber::~ImageGrabber()
{
    qDebug("ImageGrabber::Destructor --> Start");

    clearDevices();

    qDebug("ImageGrabber::Destructor --> End");
}


void ImageGrabber::clearDevices()
{
    qDebug("ImageGrabber::clearDevices --> Start");

    unsigned int deviceSize = devices.size();
    for (unsigned int deviceIndex = 0; deviceIndex < deviceSize; ++deviceIndex) {
        delete devices[deviceIndex];
        devices[deviceIndex] = NULL;
    }
    devices.clear();

    qDebug("ImageGrabber::clearDevices --> End");
}


void ImageGrabber::initialization()
{
    qDebug("ImageGrabber::initialization --> Start");

    clearDevices();
    initializationSubclass();

    qDebug("ImageGrabber::initialization --> End");
}


void ImageGrabber::init()
{
    qDebug() << "ImageGrabber::init --> Start";

    // If the grabber is running in it's own process we use a timer.
    if (isGrabberProcess()) {
        initSubclass();
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
        grabberThread = new ImageGrabberThread(this);
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

    qDebug() << "ImageGrabber::init --> End";
}


void ImageGrabber::finalize()
{
    qDebug() << "ImageGrabber::finalize --> Start";

    if (isGrabberProcess()) {
        tearDown();
    }
    else {
        if (grabberThread != 0) {
            grabberThread->terminate();
            grabberThread->wait();
            delete grabberThread;
            grabberThread = 0;
        }
    }

    qDebug() << "ImageGrabber::finalize --> End";
}


const QVector<ImageGrabberDevice*> ImageGrabber::getDevices()
{
    return devices;
}


ImageGrabberDevice* ImageGrabber::getDevice(int deviceIndex)
{
    return devices[deviceIndex];
}


const QVector<QString> ImageGrabber::getDeviceNames()
{
    QVector<QString> deviceNames;

    for (int deviceIndex = 0 ; deviceIndex < devices.size() ; deviceIndex++) {
        deviceNames.append(devices[deviceIndex]->getDeviceName());
    }

    return deviceNames;
}


bool ImageGrabber::isGrabberInitialized() const
{
    return isInitialized;
}


bool ImageGrabber::isGrabberInited() const
{
    return isInited;
}


bool ImageGrabber::isGrabberProcess() const
{
    return isProcess;
}

