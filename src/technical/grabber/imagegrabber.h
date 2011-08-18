/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#include <QtCore/QString>

/**
 * Abstract class for the different video grabbers used by the SDLVideoView
 * widget.
 *
 * @author Bjoern Erik Nilsen, Fredrik Berg Kjoelstad, Ralf Lange
 */
class ImageGrabber
{
public:
    /**
     * Constructs and initializes the object.
     * @param filePath path to the output file grabbed from a device
     */
    ImageGrabber(const QString &filePath);

    /**
     * Destructor
     */
    ~ImageGrabber();

    /**
     * Initialization of the Command line grabber
     */
    virtual bool initialization() = 0;

    /**
     * Checks if the process is running in deamon mode.
     * @return true if it runs in deamon mode, false otherwise
     */
    bool isGrabberProcess() const;

    /**
     * Abstract function for initializing the grabber.
     * @return true on success, false otherwise
     */
    virtual bool init() = 0;

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

protected:
    QString filePath;
    bool isProcess;
};

#endif
