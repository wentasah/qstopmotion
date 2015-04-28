/******************************************************************************
 *  Copyright (C) 2014-2015 by                                                *
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

#ifndef GRABBERRESOLUTION_H
#define GRABBERRESOLUTION_H

#include <QtCore/QString>


/**
 * Resolution of a device.
 *
 * @author Ralf Lange
 */
class GrabberResolution
{
public:

    /**
     * Enum with all possible image output formats
     */
    enum resolutionFormat {
        unknownFormat,         // Not supported format
        bgr24Format,           // BGR24 format
        xbgr32Format,          // XBGR32 format
        abgr32Format,          // ABGR32 format
        ayuvFormat,            // AYUV format
        yuv2Format,            // YUV2 format
        uyvyFormat,            // UYVY format
        yv12Format,            // YV12 format
        nv12Format,            // NV12 format
        i420Format,            // I420 format
        mjpegFormat            // MJPEG format
    };

    /**
     * Default constructs and initializes the object.
     */
    GrabberResolution();

    /**
     * Constructs and initializes the object.
     * @param w The width of the resolution.
     * @param h The height of the resolution.
     * @param f The image format of the resolution.
     * @param a Is the resolution active.
     */
    GrabberResolution(unsigned int  w,
                      unsigned int  h,
                      unsigned int  f,
                      bool          a);

    /**
     * Destructor
     */
    ~GrabberResolution();

    /**
     * Get the width value of the resolution
     * @return The width value of the resolution.
     */
    unsigned int getWidth();

    /**
     * Set the width value of the resolution
     * @param w The new width value of the resolution.
     */
    void setWidth(unsigned int w);

    /**
     * Get the height value of the resolution
     * @return The height value of the resolution.
     */
    unsigned int getHeight();

    /**
     * Set the height value of the resolution
     * @param h The new height value of the resolution.
     */
    void setHeight(unsigned int h);

    /**
     * Get the image format of the resolution
     * @return The image format of the resolution.
     */
    unsigned int getFormat();

    /**
     * Set the image format of the resolution
     * @param f The new image format of the resolution.
     */
    void setFormat(unsigned int f);

    /**
     * Is the resolution active?
     * @return True if active.
     */
    bool isActive();

    /**
     * Get the name of the resolution.
     * @return The id of the resolution.
     */
    const QString getName();

private:

    unsigned int  width;
    unsigned int  height;
    unsigned int  format;
    bool          active;

};

#endif
