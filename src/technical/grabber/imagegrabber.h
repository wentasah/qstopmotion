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

#ifndef IMAGEGRABBER_H
#define IMAGEGRABBER_H

#include "frontends/frontend.h"
#include "technical/grabber/grabbercontroller.h"
#include "technical/grabber/imagegrabberdevice.h"

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
class ImageGrabber : public QObject
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
    ImageGrabber(Frontend *f);

    /**
     * Destructor
     */
    ~ImageGrabber();

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
     * Initialization of the grabber.
     */
    void initialization();

    /**
     * Abstract function for initializing the grabber.
     * @return true on success, false otherwise
     */
    void init();

    /**
     * Abstract function for initializing the grabber.
     * @return true on success, false otherwise
     */
    void finalize();

    /**
     * Abstract function for shutting down the grabber.
     * @return true on success, false otherwise
     */
    virtual bool tearDown() = 0;

    /**
     * Abstract function for grabbing an image.
     * @return true on success, false otherwise
     */
    virtual bool grab() = 0;

    /**
     * Abstract function to get the live image from the camera
     * @return The live image.
     */
    virtual const QImage getLiveImage() = 0;

    /**
     * Abstract function to get the raw image from the camera
     * @return The raw image.
     */
    virtual const QImage getRawImage() = 0;

    /**
     * Initialization of the Command line grabber
     */
    virtual bool initializationSubclass(QVector<ImageGrabberDevice*> &devices) = 0;

    /**
     * Abstract function for initializing the grabber.
     * @return true on success, false otherwise
     */
    virtual bool initSubclass() = 0;

protected:
    /**
     * Frontend of the application
     */
    Frontend *frontend;

    GrabberController *controller;

};

#endif
