/******************************************************************************
 *  Copyright (C) 2010-2014 by                                                *
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

#ifndef MEDIAFROUNDATION_MFGRABBER_H
#define MEDIAFROUNDATION_MFGRABBER_H

#include "technical/grabber/imagegrabber.h"
#include "technical/grabber/imagegrabberdevice.h"

// Include files of the media foundation
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>


/**
 * Video grabber class useing the Microsoft Media Foundation API.
 *
 * @author Ralf Lange
 */
class MfGrabber : public ImageGrabber
{
    Q_OBJECT
public:

    /**
     * Initializes the member variables.
     * @param filePath path to the output file grabbed from a device
     */
    MfGrabber(Frontend *f);

    /**
     * Destructor
     */
    ~MfGrabber();

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

    IMFMediaSource* getSource();

private:
    HRESULT readDeviceInfo(IMFActivate *pActivate, unsigned int Num, ImageGrabberDevice *device);

    HRESULT enumerateCaptureFormats(IMFMediaSource *pSource, ImageGrabberDevice *device);

    HRESULT SetDeviceFormat(IMFMediaSource *pSource, DWORD dwFormatIndex);

    void getRawFrame(const uchar*& data);

    /**
     * Get the actual image from the gstreamer application interface.
     */
    const QImage getImage();

    int         activeSource;
    bool        isInitSuccess;
    bool        firstImage;

    QImage liveImage;
    QImage rawImage;

    IMFMediaSource*  mediaSource;
    IMFSourceReader* sourceReader;

    uchar*           frameData;
    int              frameLength;
    QSize            frameSize;
    QImage           lastFrame;

    UINT32           width;
    UINT32           height;
    LONG             stride;
    GUID             majorType;
    GUID             subType;
};

#endif
