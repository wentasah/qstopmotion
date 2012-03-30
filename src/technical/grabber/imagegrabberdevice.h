/******************************************************************************
 *  Copyright (C) 2011-2011 by                                                *
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

#ifndef IMAGEGRABBERDEVICE_H
#define IMAGEGRABBERDEVICE_H

#include "frontends/frontend.h"
#include "technical/grabber/imagegrabberthread.h"

#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtGui/QImage>


/**
 * All possible video sources.
 */
enum ImageGrabberVideoSources {
    TestSource,
    Video4LinuxSource,
    Iee1394Source,
    DirectShowSource
};

/**
 * Video device capabilities
 */
enum ImageGrabberDeviceCapabilities {
    video_x_none,
    video_x_raw_rgb,
    video_x_raw_yuv,
    video_x_dv
};

/**
 * Class containing all the informations of a device.
 *
 * @author Ralf Lange
 */
class ImageGrabberDevice
{
public:

    /**
     * Constructs and initializes the object.
     * @param id The id of the device.
     * @param name The name of the device.
     * @param source The source of the device.
     * @param cap The capability of the device.
     */
    ImageGrabberDevice(const QString id,
                       const QString name,
                       ImageGrabberVideoSources source,
                       ImageGrabberDeviceCapabilities cap);

    /**
     * Destructor
     */
    ~ImageGrabberDevice();

    /**
     * Get the id of the device.
     * @return The id of the device.
     */
    const QString getDeviceId();

    /**
     * Get the name of the device.
     * @return The name of the device.
     */
    const QString getDeviceName();

    /**
     * Get the source of the device.
     * @return The source of the device.
     */
    ImageGrabberVideoSources getDeviceSource();

    /**
     * Get the capability of the device.
     * @return The capability of the device.
     */
    ImageGrabberDeviceCapabilities getDeviceCapability();

private:
    QString   deviceId;
    // GSTValue *deviceIdValue;
    QString   deviceName;
    ImageGrabberVideoSources deviceSource;
    ImageGrabberDeviceCapabilities deviceCap;
};

#endif
