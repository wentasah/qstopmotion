/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#ifndef V4L2GRABBER_H
#define V4L2GRABBER_H

#include "technical/grabber/imagegrabber.h"
#include "technical/grabber/imagegrabberdevice.h"

// Include files of v4l2
#include <linux/videodev2.h>

#define N_BUFFERS 2

/**
 * Video grabbers using the Video4Linux2 library.
 *
 * @author Ralf Lange
 */
class V4L2Grabber : public ImageGrabber
{
    Q_OBJECT
public:

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    V4L2Grabber(Frontend *f);

    /**
     * Destructor
     */
    ~V4L2Grabber();

    /**
     * Initialization of the Command line grabber
     * @param devices The vector of initialized devices.
     * @return true on success, false otherwise
     */
    bool initialization(QVector<ImageGrabberDevice*> &devices);

    /**
     * Starts the grabber if it is marked to be runned in deamon mode.
     * @return true on success, false otherwise
     */
    bool setUp();

    /**
     * Get the live image from the camera
     */
    const QImage getLiveImage();

    /**
     * Get the raw image from the camera
     */
    const QImage getRawImage();

    /**
     * Grabs one picture from the device.
     * @return true on success, false otherwise
     */
    bool grab();

    /**
     * Shut downs the grabber process either if it is runned in deamon
     * mode or "single grab" mode.
     * @return true on success, false otherwise
     */
    bool tearDown();

    /**
     * @brief getFd
     * @return The file descriptor of the open device.
     */
    int getFd();

private:
    bool readDeviceInfo(int fd, ImageGrabberDevice *device);

    bool enumerateCaptureFormats(int fd, ImageGrabberDevice *device);

    // bool SetDeviceFormat(int fd, int dwFormatIndex);

    /**
     * ioctl with a number of retries in the case of failure
     * @param fd Device descriptor
     * @param IOCTL_X ioctl reference
     * @param arg Pointer to ioctl data
     * @return ioctl result
     */
    int xioctl(int fd, int IOCTL_X, void *arg);

    /**
     * Get the actual image from the gstreamer application interface.
     */
    const QImage getImage();

private:
    struct buffer {
            void    *start;
            size_t   length;
    };

    int         activeSource;
    bool        isInitSuccess;
    bool        firstImage;

    /**
     * V4L2 file handle
     */
    int                  fd;
    unsigned int         width;
    unsigned int         height;
    struct v4l2_format   format;
    unsigned char*       dst_buf;
    unsigned char*       frameData;
    v4l2_buffer          buf;
    struct buffer        buffers[N_BUFFERS];
	  
    QImage liveImage;
    QImage rawImage;
};

#endif
