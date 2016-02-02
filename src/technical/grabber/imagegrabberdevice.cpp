/******************************************************************************
 *  Copyright (C) 2011-2015 by                                                *
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

#include "imagegrabberdevice.h"

#include <QDebug>


ImageGrabberDevice::ImageGrabberDevice()
{
    qDebug() << "ImageGrabberDevice::Constructor(Default) --> Start";

    deviceId.clear();
    deviceName.clear();
    deviceSource = ImageGrabberDevice::noSource;
    deviceCap = ImageGrabberDevice::video_x_none;

    resolutions.clear();

    controller = NULL;

    qDebug() << "ImageGrabberDevice::Constructor(Default) --> End";
}


ImageGrabberDevice::ImageGrabberDevice(const int index,
                                       const QString id,
                                       const QString name,
                                       imageGrabberSources source,
                                       imageGrabberDeviceCapabilities cap)
{
    qDebug() << "ImageGrabberDevice::Constructor --> Start";

    deviceIndex = index;
    deviceId.clear();
    deviceId.append(id);
    deviceName.clear();
    deviceName.append(name);
    deviceSource = source;
    deviceCap = cap;

    controller = NULL;

    qDebug() << "ImageGrabberDevice::Constructor --> End";
}


ImageGrabberDevice::~ImageGrabberDevice()
{
    qDebug() << "ImageGrabberDevice::Destructor --> Start";

    if (controller != NULL)
    {
        delete controller;
        controller = NULL;
    }

    qDebug() << "ImageGrabberDevice::Destructor --> End";
}


const int ImageGrabberDevice::getDeviceIndex()
{
    return deviceIndex;
}


void ImageGrabberDevice::setDeviceIndex(const int index)
{
    deviceIndex = index;
}


const QString ImageGrabberDevice::getDeviceId()
{
    return deviceId;
}


void ImageGrabberDevice::setDeviceId(const QString id)
{
    deviceId.append(id);
}


const QString ImageGrabberDevice::getDeviceName()
{
    return deviceName;
}


void ImageGrabberDevice::setDeviceName(const QString name)
{
    deviceName.append(name);
}


ImageGrabberDevice::imageGrabberSources ImageGrabberDevice::getDeviceSource()
{
    return deviceSource;
}


void ImageGrabberDevice::setDeviceSource(ImageGrabberDevice::imageGrabberSources source)
{
    deviceSource = source;
}


ImageGrabberDevice::imageGrabberDeviceCapabilities ImageGrabberDevice::getDeviceCapability()
{
    return deviceCap;
}


void ImageGrabberDevice::setDeviceCapability(ImageGrabberDevice::imageGrabberDeviceCapabilities cap)
{
    deviceCap = cap;
}

/**************************************************************************
 **************************************************************************
 * Camera resolution
 **************************************************************************
 **************************************************************************/

void ImageGrabberDevice::addResolution(GrabberResolution r)
{
    for (int i = 0; i < resolutions.size(); i++) {
        if ((r.getWidth() == resolutions[i].getWidth()) &&
            (r.getHeight() == resolutions[i].getHeight()) &&
            (r.getFormat() == resolutions[i].getFormat())) {
            return;
        }
    }
    resolutions.append(r);
}


QVector<GrabberResolution> ImageGrabberDevice::getResolutions()
{
    return resolutions;
}


int ImageGrabberDevice::getActiveResolution()
{
    return -1;
}


void ImageGrabberDevice::setActiveResolution(int /*ac*/)
{
    Q_ASSERT( 1 );
}

/**************************************************************************
 **************************************************************************
 * Camera controller
 **************************************************************************
 **************************************************************************/

bool ImageGrabberDevice::isController() const
{
    if (controller != NULL) {
        return true;
    }

    return false;
}


GrabberController* ImageGrabberDevice::getController()
{
    return controller;
}


void ImageGrabberDevice::setController(GrabberController *c)
{
    controller = c;
}
