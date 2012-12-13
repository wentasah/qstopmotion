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

#ifndef IMAGEGRABBERFACADE_H
#define IMAGEGRABBERFACADE_H

#include "frontends/frontend.h"
#include "technical/grabber/grabbercontroller.h"
#include "technical/grabber/imagegrabber.h"
#include "technical/grabber/imagegrabberdevice.h"
#include "technical/grabber/imagegrabberthread.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtGui/QImage>


/**
 * Abstract class for the different video grabbers used by the SDLVideoView
 * widget.
 *
 * @author Bjoern Erik Nilsen, Fredrik Berg Kjoelstad, Ralf Lange
 */
class ImageGrabberFacade : public QObject
{
    Q_OBJECT
public:

    /**
     * Enum with all possible image output formats
     */
    enum imageFormat {
        jpegFormat,            // JPEG format
        tiffFormat,            // TIFF format
        bmpFormat              // BMP format
    };

    /**
     * Enum with all possible image sizes
     */
    enum imageSize {
        defaultSize,          // Source size
        qvgaSize,             // 320x240 (QVGA)
        vgaSize,              // 640x480 (VGA)
        svgaSize,             // 800x600 (SVGA)
        paldSize,             // 704x576 (PAL D)
        hdreadySize,          // 1280x720 (HD Ready)
        fullhdSize            // 1900x1080 (Full HD)
    };

    /**
     * Enum with all possible image adjustments
     */
    enum imageAdjustment {
        leftUp,               // Left up corner
        centerUp,             // Center up
        rightUp,              // Right up corner
        leftMiddle,           // Left middle
        centerMiddle,         // Center middle
        rightMiddle,          // Right middle
        leftDown,             // Left down corner
        centerDown,           // Center down
        rightDown             // Right down corner
    };

    /**
     * Constructs and initializes the object.
     * @param filePath path to the output file grabbed from a device
     */
    ImageGrabberFacade(Frontend *f);

    /**
     * Destructor
     */
    ~ImageGrabberFacade();

    /**
     * A new device is set to use as video grabber
     */
    void newDeviceSet();

    /**
     * Get the possible video devices.
     * @return Vector with the devices.
     */
    const QVector<ImageGrabberDevice*> getDevices();

    /**
     * Get one video devices.
     * @param deviceIndex The index of the device.
     * @return The video devices.
     */
    ImageGrabberDevice *getDevice(int deviceIndex);

    /**
     * Get the possible video devices.
     * @return Vector with the device names.
     */
    const QVector<QString> getDeviceNames();

    /**
     * Checks if the grabber is initialized.
     * @return true if it is initialized, false otherwise
     */
    bool isGrabberInitialized() const;

    /**
     * Checks if the grabber is inited.
     * @return true if it is inited, false otherwise
     */
    bool isGrabberInited() const;

    /**
     * Initialization of the grabber.
     */
    void initialization();

    /**
     * Initializing the grabber.
     * @return true on success, false otherwise
     */
    void init();

    /**
     * Finalizing the grabber.
     * @return true on success, false otherwise
     */
    void finalize();

    /**
     * Get the live image from the camera
     * @return The live image.
     */
    const QImage getLiveImage();

    /**
     * Get the raw image from the camera
     * @return The raw image.
     */
    const QImage getRawImage();

private:

    /**
     * Clear the device list
     */
    void clearDevices();

protected:
    /**
     * Frontend of the application
     */
    Frontend *frontend;

    bool isInitialized;
    bool isInited;
    QVector<ImageGrabberDevice*> devices;
    ImageGrabberThread *grabberThread;

    ImageGrabber *gstreamerVideoTestGrabber;
    ImageGrabber *gstreamerV4L2Grabber;
    ImageGrabber *gstreamerDv1394Grabber;
    ImageGrabber *gstreamerDirectShowUsbGrabber;
    ImageGrabber *gphotoGrabber;

};

#endif
