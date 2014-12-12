/******************************************************************************
 *  Copyright (C) 2011-2014 by                                                *
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
#include "technical/grabber/grabbercontroller.h"
#include "technical/grabber/grabberresolution.h"

#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtGui/QImage>

class GrabberController;

/**
 * Class containing all the informations of a device.
 *
 * @author Ralf Lange
 */
class ImageGrabberDevice
{
public:

    /**
     * All possible video sources.
     */
    enum imageGrabberSources {
        noSource,                      // 0
        testSource,                    // 1
        video4LinuxSource,             // 2
        ieee1394Source,                // 3
        gphoto2Source,                 // 4
        mediaFoundationSource          // 5
    };

    /**
     * Video device capabilities
     */
    enum imageGrabberDeviceCapabilities {
        video_x_none,                  // 0
        video_x_raw_rgb,               // 1
        video_x_raw_yuv,               // 2
        video_x_dv                     // 3
    };

    /**
     * Default constructor of the object.
     */
    ImageGrabberDevice();

    /**
     * Constructs and initializes the object.
     * @param index The index of the device.
     * @param id The id of the device.
     * @param name The name of the device.
     * @param source The source of the device.
     * @param cap The capability of the device.
     */
    ImageGrabberDevice(const int index,
                       const QString id,
                       const QString name,
                       imageGrabberSources source,
                       imageGrabberDeviceCapabilities cap);

    /**
     * Destructor
     */
    ~ImageGrabberDevice();

    /**
     * Get the index of the device.
     * @return The index of the device.
     */
    const int getDeviceIndex();

    /**
     * Set the index of the device.
     * @param index The new index of the device.
     */
    void setDeviceIndex(const int index);

    /**
     * Get the id of the device.
     * @return The id of the device.
     */
    const QString getDeviceId();

    /**
     * Set the id of the device.
     * @param id The new id of the device.
     */
    void setDeviceId(const QString id);

    /**
     * Get the name of the device.
     * @return The name of the device.
     */
    const QString getDeviceName();

    /**
     * Set the name of the device.
     * @param name The new name of the device.
     */
    void setDeviceName(const QString name);

    /**
     * Get the source of the device.
     * @return The source of the device.
     */
    imageGrabberSources getDeviceSource();

    /**
     * Set the source of the device.
     * @param source The new source of the device.
     */
    void setDeviceSource(imageGrabberSources source);

    /**
     * Get the capability of the device.
     * @return The capability of the device.
     */
    imageGrabberDeviceCapabilities getDeviceCapability();

    /**
     * Set the capability of the device.
     * @param cap The new capability of the device.
     */
    void setDeviceCapability(imageGrabberDeviceCapabilities cap);

    /**************************************************************************
     **************************************************************************
     * Camera resolution
     **************************************************************************
     **************************************************************************/

    /**
     * Add a resolution to the controller.
     * @param r The new grabber resolution.
     */
    void addResolution(GrabberResolution r);

    /**
     * Get the possible resolutions of the controller.
     * @return The resolutions of the controller.
     */
    QVector<GrabberResolution> getResolutions();

    /**
     * Get the active resolution of the controller.
     * @return The index of the active resolution.
     */
    virtual int getActiveResolution();

    /**
     * Set the active resolution of the controller.
     * @param ac The index of the new active resolution.
     */
    virtual void setActiveResolution(int ac);

    /**************************************************************************
     **************************************************************************
     * Camera controller
     **************************************************************************
     **************************************************************************/

    /**
     * Has the grabber a controller interface?
     * @return True if there is a controller interface.
     */
    bool isController() const;

    /**
     * Get the controller of the grabber.
     * @return The controller of the grabber.
     */
    GrabberController *getController();

    /**
     * Set the controller of the grabber.
     * @param The controller of the grabber.
     */
    void setController(GrabberController *c);

private:
    int                             deviceIndex;
    QString                         deviceId;
    QString                         deviceName;
    imageGrabberSources             deviceSource;
    imageGrabberDeviceCapabilities  deviceCap;

    QVector<GrabberResolution>      resolutions;

    GrabberController              *controller;

};

#endif
