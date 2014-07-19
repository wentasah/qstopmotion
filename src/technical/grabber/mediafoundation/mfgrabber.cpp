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

#include "mfgrabber.h"
#include "mfcontroller.h"
#include "technical/preferencestool.h"
#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the media foundation
// #include "mfidl.h"
// #include <guiddef.h>
#include <Strmif.h>


#define APP_SINK_MAX_BUFFERS 2


/**************************************************************************
 * Implementation of the image grabber functions using the
 * Microsoft Media Foundation API.
 **************************************************************************/

MfGrabber::MfGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("MfGrabber::Constructor --> Start (Empty)");

    HRESULT hr;

    hr = MFStartup(MF_VERSION);
    if (!SUCCEEDED(hr)) {
        // Media foundation can't be started
        qFatal("MfGrabber::Constructor --> Error: Cannot start the media foundation!");
    }

    qDebug("MfGrabber::Constructor --> End");
}


MfGrabber::~MfGrabber()
{
    qDebug("MfGrabber::Destructor --> Start (Empty)");

    HRESULT hr;

    hr = MFShutdown();
    if (!SUCCEEDED(hr)) {
        // Media foundation can't be shuted down
        qFatal("MfGrabber::Destructor --> Error: Cannot shut down the media foundation!");
    }

    qDebug("MfGrabber::Destructor --> End");
}


bool MfGrabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("MfGrabber::initialization --> Start");

    // PreferencesTool *pref = frontend->getPreferences();
    // int              value;

    HRESULT          hr = S_OK;
    IMFAttributes   *pAttributes = NULL;
    IMFActivate    **ppDevices = NULL;
    UINT32           deviceCount;

    int                 device_size;
    ImageGrabberDevice *device = NULL;
    MfController       *deviceController = NULL;

    device_size = devices.size();

    qDebug() << "MfGrabber::initialization --> Check devices of mfwvideosrc";

    // Initializes the COM library on the current thread and identifies
    // the concurrency model as single-thread apartment (STA).
    // CoInitialize(NULL);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    //---------------------------------------------------------------------
    // Search all possible devices
    //---------------------------------------------------------------------

    hr = MFCreateAttributes(&pAttributes, 1);
    if (!SUCCEEDED(hr))
    {
        qDebug() << "MfGrabber::initialization --> Error: cannot create video input device attributes";
        return false;
    }

    hr = pAttributes->SetGUID(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
        );
    if (!SUCCEEDED(hr))
    {
        qDebug() << "MfGrabber::initialization --> Error: cannot set video input device attributes";
        return false;
    }

    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &deviceCount);
    if (!SUCCEEDED(hr))
    {
        qDebug() << "MfGrabber::initialization --> Error: cannot enumerate video input devices";
        return false;
    }

    if(deviceCount > 0)
    {
        for(UINT32 i = 0; i < deviceCount; i++)
        {
            device = new ImageGrabberDevice();

            hr = readDeviceInfo(ppDevices[i], i, device);
            if (SUCCEEDED(hr)) {
                // Add the Controller to the new device
                deviceController = new MfController(0);
                if (deviceController->init(device->getDeviceId()))
                {
                    device->setController(deviceController);
                }
                // Add the new device to the device list
                devices.append(device);
            }
            else {
                // Delete the new device
                delete device;
                device = NULL;
            }

            // vds_Devices.push_back(vd);

            ppDevices[i]->Release();
            ppDevices[i] = NULL;
        }

        CoTaskMemFree(ppDevices);
        ppDevices = NULL;
        deviceCount = 0;
    }
    else {
        hr = -1;
    }

    qDebug() << "MfGrabber::initialization --> device count: " << devices.size();

    qDebug("MfGrabber::initialization --> End (true)");

    return true;
}


bool MfGrabber::setUp()
{
    qDebug() << "MfGrabber::setUp --> Start";

    int     imageWidth = 640;    // Default values are VGA
    int     imageHeight = 480;
    int     videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice *videoDevice = frontend->getDevice(videoSource);

    qDebug() << "MfGrabber::tearDown --> End";

    return true;
}

bool MfGrabber::grab()
{
    return true;
}


const QImage MfGrabber::getImage()
{
    QImage image;
/*
    GstBuffer *buffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
    GstCaps *caps = gst_buffer_get_caps(buffer);

    gint width, height;
    GstStructure *st = gst_caps_get_structure(caps, 0); // index);
    const gchar *imageType = gst_structure_get_name(st);

    if (firstImage) {
        const gchar *bufferCaps = gst_structure_to_string(st);
        qDebug() << "MfGrabber::getImage --> buffer caps: " << bufferCaps;
    }

    gst_structure_get_int(st, "width", &width);
    gst_structure_get_int(st, "height", &height);

    if (0 == strcmp(imageType, "video/x-raw-rgb")) {
        // video/x-raw-rgb, format=RGB3
        // video/x-raw-rgb, format=BGR3

        guchar *rgbData;
        QImage::Format format = QImage::Format_Invalid;
        gint bpp;
        gst_structure_get_int(st, "bpp", &bpp);

        if (bpp == 24)
            format = QImage::Format_RGB888;
        else if (bpp == 32)
            format = QImage::Format_RGB32;

        if (format != QImage::Format_Invalid) {
            rgbData = GST_BUFFER_DATA (buffer); //image_data is a static unsigned char *
            image = QImage((const uchar *)rgbData,
                           width,
                           height,
                           format);
            image.bits(); //detach
        }
    }
    else {
        if (0 == strcmp(imageType, "video/x-raw-yuv")) {
            guchar *yuvData;
            guint32 fourcc;
            gst_structure_get_fourcc(st, "format", &fourcc);
            if (fourcc == GST_STR_FOURCC("I420")) {
                // YUV ... Planar
                // Identical to IYUV
                // Identical to YV12 expect that the U and V plane oder is reversed

                yuvData = GST_BUFFER_DATA (buffer); //image_data is a static unsigned char *
                // rgbData = new unsigned char[width*height*2];
                // uyvy_to_rgb24(yuvData, rgbData, width, height);

                image = QImage(width/2, height/2, QImage::Format_RGB32);

                for (int y=0; y<height; y+=2) {
                    const uchar *yLine = yuvData + y*width;
                    const uchar *uLine = yuvData + width*height + y*width/4;
                    const uchar *vLine = yuvData + width*height*5/4 + y*width/4;

                    for (int x=0; x<width; x+=2) {
                        const qreal Y = 1.164*(yLine[x]-16);
                        const int U = uLine[x/2]-128;
                        const int V = vLine[x/2]-128;

                        int b = qBound(0, int(Y + 2.018*U), 255);
                        int g = qBound(0, int(Y - 0.813*V - 0.391*U), 255);
                        int r = qBound(0, int(Y + 1.596*V), 255);

                        image.setPixel(x/2,y/2,qRgb(r,g,b));
                    }
                }
            }
            if (fourcc == GST_STR_FOURCC("YUY2")) {
                // YUV 4:2:2 Packed
                if (firstImage) {
                    qDebug() << "GstreamerGrabber::getImage --> format 'video/x-raw-yuv(YUY2)' not supported!!!";
                }
            }
            if (fourcc == GST_STR_FOURCC("YV12")) {
                // YUV 4:2:0 Planar, Uncompressed format commonly used im MPEG video processing.
                if (firstImage) {
                    qDebug() << "GstreamerGrabber::getImage --> format 'video/x-raw-yuv(YV12)' not supported!!!";
                }
            }
        }
        else {
            if (0 == strcmp(imageType, "image/jpeg")) {
                if (firstImage) {
                    qDebug() << "GstreamerGrabber::getImage --> format 'image/jpeg' not supported!!!";
                }
            }
            else {
                if (firstImage) {
                    qDebug() << "GstreamerGrabber::getImage --> format '" << imageType << "' not supported!!!";
                }
            }
        }
    }

    gst_caps_unref(caps);
    gst_buffer_unref(buffer); //If I comment out this the application stops crashing, but fills up the memory
*/
    if (firstImage) {
        firstImage = false;
    }

    Q_ASSERT(image.isNull() == false);

    return image;
}


const QImage MfGrabber::getLiveImage()
{
    // liveImage.load(filePath);

    liveImage = getImage();

    return liveImage;
}


const QImage MfGrabber::getRawImage()
{
    // rawImage.load(filePath);

    rawImage = getImage();

    return rawImage;
}


bool MfGrabber::tearDown()
{
    qDebug() << "MfGrabber::tearDown --> Start";


    qDebug() << "MfGrabber::tearDown --> End";

    return true;
}


HRESULT MfGrabber::readDeviceInfo(IMFActivate *pActivate, unsigned int Num, ImageGrabberDevice *device)
{
    HRESULT hr = S_OK;

    // vd_CurrentNumber = Num;

    if(pActivate)
    {
        IMFMediaSource *pSource = NULL;
        LPWSTR          FriendlyName = NULL;
        UINT32          cchLength;

        hr = pActivate->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
                                           &FriendlyName,
                                           &cchLength);
        if (!SUCCEEDED(hr)) {
            // Error reading friendly name
            return hr;
        }
        device->setDeviceId(QString::fromUtf16((const ushort*)FriendlyName));
        device->setDeviceName(QString::fromUtf16((const ushort*)FriendlyName));
        device->setDeviceSource(ImageGrabberDevice::mediaFoundationSource);

        hr = pActivate->ActivateObject(
            __uuidof(IMFMediaSource),
            (void**)&pSource );
        if (!SUCCEEDED(hr)) {
            // Error activating the source

            if(FriendlyName) {
                CoTaskMemFree(FriendlyName);
            }

            return hr;
        }

        enumerateCaptureFormats(pSource, device);

        // buildLibraryofTypes();

        if (pSource) {
            pSource->Release();
            pSource = NULL;
        }

        // Frees allocated memory
        if(FriendlyName) {
            CoTaskMemFree(FriendlyName);
        }

    }

    return hr;
}

HRESULT MfGrabber::enumerateCaptureFormats(IMFMediaSource *pSource, ImageGrabberDevice *device)
{
    HRESULT                    hr = S_OK;
    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor       *pSD = NULL;
    IMFMediaTypeHandler       *pHandler = NULL;
    IMFMediaType              *pType = NULL;

    hr = pSource->CreatePresentationDescriptor(&pPD);
    if (FAILED(hr)) {
        goto done;
    }

    BOOL fSelected;
    hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);
    if (FAILED(hr)) {
        goto done;
    }

    hr = pSD->GetMediaTypeHandler(&pHandler);
    if (FAILED(hr)) {
        goto done;
    }

    DWORD cTypes = 0;
    hr = pHandler->GetMediaTypeCount(&cTypes);
    if (FAILED(hr)) {
        goto done;
    }

    for (DWORD i = 0; i < cTypes; i++) {
        PROPVARIANT val;
        unsigned int outputWidth = 0;
        unsigned int outputHeight = 0;

        PropVariantInit(&val);
        hr = pHandler->GetMediaTypeByIndex(i, &pType);


        if (FAILED(hr)) {
            goto done;
        }

        // MediaType MT = FormatReader::Read(pType);
        hr = pType->GetItem(MF_MT_FRAME_SIZE, &val);
        if (SUCCEEDED(hr)) {
            UINT32 uHigh = 0, uLow = 0;
            // LogUINT32AsUINT64New(val, uHigh, uLow);
            Unpack2UINT32AsUINT64(val.uhVal.QuadPart, &uHigh, &uLow);
            outputWidth = uHigh;
            outputHeight = uLow;

            PropVariantClear(&val);
        }

        // vd_CurrentFormats.push_back(MT);
        device->getController()->addResolution(GrabberResolution(0, outputWidth, outputHeight, false));

        if (pType) {
            pType->Release();
            pType=NULL;
        }
    }

done:
    if (pPD) {
        pPD->Release();
        pPD = NULL;
    }
    if (pSD) {
        pSD->Release();
        pSD = NULL;
    }
    if (pHandler) {
        pHandler->Release();
        pHandler = NULL;
    }
    if (pType) {
        pType->Release();
        pType = NULL;
    }

    return hr;
}
