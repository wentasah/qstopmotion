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
#include "mfutil.h"
#include "technical/grabber/imageconverter.h"
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
    qDebug("MfGrabber::Constructor --> Start");

    HRESULT hr;

    mediaSource = NULL;
    sourceReader = NULL;

    frameData = NULL;
    frameLength = 0;

    width = -1;
    height = -1;
    stride = -1;

    hr = MFStartup(MF_VERSION);
    if (!SUCCEEDED(hr)) {
        // Media foundation can't be started
        qFatal("MfGrabber::Constructor --> Error: Cannot start the media foundation!");
    }

    qDebug("MfGrabber::Constructor --> End");
}


MfGrabber::~MfGrabber()
{
    qDebug("MfGrabber::Destructor --> Start");

    HRESULT hr;

    hr = MFShutdown();
    if (!SUCCEEDED(hr)) {
        // Media foundation can't be shuted down
        qFatal("MfGrabber::Destructor --> Error: Cannot shut down the media foundation!");
    }

    qDebug("MfGrabber::Destructor --> End");
}


IMFMediaSource* MfGrabber::getSource()
{
    return mediaSource;
}


bool MfGrabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("MfGrabber::initialization --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int              value;
    int              deviceCount = 0;

    HRESULT             hr = S_OK;
    IMFAttributes*      pAttributes = NULL;
    IMFActivate**       ppDevices = NULL;
    UINT32              deviceCount;
    int                 device_size;
    ImageGrabberDevice* device = NULL;
    MfController*       deviceController = NULL;

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
            device->setDeviceIndex(i);

            hr = readDeviceInfo(ppDevices[i], device);
            if (SUCCEEDED(hr)) {
                // Add the new device to the device list
                devices.append(device);
                deviceCount++;

                // Add the Controller to the new device
                if (pref->getIntegerPreference("preferences", "mediafoundationcontroller", value) == false) {
                    value = false;
                }
                if ((int)true == value) {
                    deviceController = new MfController(0);
                    device->setController(deviceController);

                    // Initalize the device controller
                    deviceController->initialization(this, device);
                }
            }
            else {
                // Delete the new device
                delete device;
                device = NULL;
            }
            SafeRelease(&mediaSource);

            // vds_Devices.push_back(vd);

            SafeRelease(&ppDevices[i]);
        }

        CoTaskMemFree(ppDevices);
        ppDevices = NULL;
        deviceCount = 0;
    }
    else {
        qDebug() << "MfGrabber::initialization --> Warning: no video input devices";

        hr = -1;
    }

    qDebug() << "MfGrabber::initialization --> device count: " << devices.size();

    if (0 == deviceCount) {
        qDebug("MfGrabber::initialization --> End (false)");

        return false;
    }

    qDebug("MfGrabber::initialization --> End (true)");

    return true;
}


bool MfGrabber::setUp()
{
    qDebug() << "MfGrabber::setUp --> Start";

    int                 videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice* videoDevice = frontend->getDevice(videoSource);
    int                 deviceIndex = videoDevice->getDeviceIndex();
    HRESULT             hr = S_OK;
    IMFAttributes*      devAttr = NULL;
    IMFActivate**       camDevices = NULL;
    UINT32              camCount = 0;
    IMFMediaType*       mediaType = NULL;
    IMFMediaType*       newMediaType = NULL;
    DWORD               dwMediaTypeIndex = 0;
    UINT32              typeWidth;
    UINT32              typeHeight;
    GUID                typeMajorType;
    GUID                typeSubType;
    int                 resIndex = -1;


    hr = MFCreateAttributes(&devAttr, 1);
    if (!SUCCEEDED(hr)) {
        return false;
    }

    hr = devAttr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                          MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&devAttr);
        return false;
    }

    hr = MFEnumDeviceSources(devAttr, &camDevices, &camCount);
    if (!SUCCEEDED(hr)) {
        qDebug() << "MfGrabber::setUp --> Cannot create the video capture device";
        SafeRelease(&devAttr);
        return false;
    }

    SafeRelease(&devAttr);

    if (deviceIndex > int(camCount)) {
        return false;
    }

    hr = camDevices[deviceIndex]->ActivateObject(__uuidof(IMFMediaSource),
                                                 (void**)&mediaSource);
    if (!SUCCEEDED(hr)) {
        return false;
    }

    hr = MFCreateSourceReaderFromMediaSource(mediaSource, NULL, &sourceReader);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&mediaSource);
        return false;
    }

    hr = MFCreateMediaType(&newMediaType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    //-----------------------------
    // Find usable MediaType
    //-----------------------------

    resIndex = frontend->getProject()->getResolution();
    GrabberResolution resolution = frontend->getResolution(resIndex);

    while (SUCCEEDED(hr)) {
        hr = sourceReader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                              dwMediaTypeIndex,
                                              &mediaType);
        if (hr == MF_E_NO_MORE_TYPES) {
            hr = S_OK;
            break;
        }
        if (!SUCCEEDED(hr)) {
            SafeRelease(&sourceReader);
            SafeRelease(&mediaSource);
            return false;
        }

        mediaType->GetGUID(MF_MT_MAJOR_TYPE, &typeMajorType);
        mediaType->GetGUID(MF_MT_SUBTYPE, &typeSubType);

        hr = MFGetAttributeSize(mediaType, MF_MT_FRAME_SIZE, &typeWidth, &typeHeight);
        if (FAILED(hr)) {
            SafeRelease(&mediaType);
            SafeRelease(&sourceReader);
            SafeRelease(&mediaSource);
            return false;
        }

        if ((resolution.getWidth() == typeWidth) &&
            (resolution.getHeight() == typeHeight)) {
            switch (resolution.getFormat()) {
            case GrabberResolution::bgr24Format:
                if (typeSubType == MFVideoFormat_RGB24) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::ayuvFormat:
                if (typeSubType == MFVideoFormat_AYUV) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::yuv2Format:
                if (typeSubType == MFVideoFormat_YUY2) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::uyvyFormat:
                if (typeSubType == MFVideoFormat_UYVY) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::yv12Format:
                if (typeSubType == MFVideoFormat_YV12) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::i420Format:
                if (typeSubType == MFVideoFormat_I420) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::nv12Format:
                if (typeSubType == MFVideoFormat_NV12) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            case GrabberResolution::mjpegFormat:
                if (typeSubType == MFVideoFormat_MJPG) {
                    mediaType->CopyAllItems(newMediaType);
                }
                break;
            }
        }


        QString majorTypeString = GetGUIDNameConst(typeMajorType);
        QString subTypeString = GetGUIDNameConst(typeSubType);

        LONG typeStride = (LONG)MFGetAttributeUINT32(mediaType, MF_MT_DEFAULT_STRIDE, 1);

        qDebug() << "MfGrabber::setUp --> Media Type (" << dwMediaTypeIndex << "): "
                 << majorTypeString << " - " << subTypeString
                 << " Size: " << typeWidth << "x" << typeHeight
                 << " Stride: " << typeStride;

        LogMediaType(mediaType);


        SafeRelease(&mediaType);
        dwMediaTypeIndex++;
    }

    //------------------------------
    // Use the found media type
    //------------------------------

    hr = sourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           NULL,
                                           newMediaType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&newMediaType);
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    SafeRelease(&newMediaType);

    hr = sourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           &mediaType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    hr = mediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&mediaType);
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    hr = mediaType->GetGUID(MF_MT_SUBTYPE, &subType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&mediaType);
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    hr = MFGetAttributeSize(mediaType, MF_MT_FRAME_SIZE, &width, &height);
    if (FAILED(hr)) {
        SafeRelease(&mediaType);
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    stride = (LONG)MFGetAttributeUINT32(mediaType, MF_MT_DEFAULT_STRIDE, 1);

    QString majorTypeString = GetGUIDNameConst(majorType);
    QString subTypeString = GetGUIDNameConst(subType);

    qDebug() << "MfGrabber::setUp --> Current Media Type: "
             << majorTypeString << " - " << subTypeString
             << " Size: " << width << "x" << height
             << " Stride: " << stride;

    LogMediaType(mediaType);

    if (subType == MFVideoFormat_RGB24) {
        return true;
    }

    if (subType == MFVideoFormat_AYUV) {
        return true;
    }

    if (subType == MFVideoFormat_YUY2) {
        return true;
    }

    if (subType == MFVideoFormat_I420) {
        return true;
    }

    SafeRelease(&mediaType);
    SafeRelease(&sourceReader);
    SafeRelease(&mediaSource);

    qDebug() << "MfGrabber::setUp --> End";

    return false;
}

bool MfGrabber::grab()
{
    return true;
}


const QImage MfGrabber::getImage()
{
    QImage      image;
    const BYTE* data = NULL;
    int         Tries = 10;

    while (data == NULL && --Tries) {
        getRawFrame(data);
    }

    if (data != NULL && width > 0 && height > 0) {
        image = QImage(QSize(width, height), QImage::Format_ARGB32);
        BYTE* dst = const_cast<BYTE*>(image.constBits());
        const BYTE* src = const_cast<BYTE*>(data);
        const BYTE* const srcEnd = src + width * height * 4;

        while (src < srcEnd) {
            *dst++ = *src++;
        }
    }

    if (firstImage) {
        firstImage = false;
    }

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

    SafeRelease(&sourceReader);
    SafeRelease(&mediaSource);

    qDebug() << "MfGrabber::tearDown --> End";

    return true;
}


HRESULT MfGrabber::readDeviceInfo(IMFActivate *pActivate, ImageGrabberDevice *device)
{
    qDebug() << "MfGrabber::readDeviceInfo --> Start";

    HRESULT hr = S_OK;

    if (pActivate) {
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
        device->setDeviceName(QString::fromUtf16((const ushort*)FriendlyName) + " (MMF)");
        device->setDeviceSource(ImageGrabberDevice::mediaFoundationSource);

        hr = pActivate->ActivateObject(__uuidof(IMFMediaSource), (void**)&mediaSource );
        if (!SUCCEEDED(hr)) {
            // Error activating the source

            if(FriendlyName) {
                CoTaskMemFree(FriendlyName);
            }

            return hr;
        }

        enumerateCaptureFormats(mediaSource, device);

        // SafeRelease(&mediaSource);

        // Frees allocated memory
        if(FriendlyName) {
            CoTaskMemFree(FriendlyName);
        }
    }

    qDebug() << "MfGrabber::readDeviceInfo --> End";

    return hr;
}


HRESULT MfGrabber::enumerateCaptureFormats(IMFMediaSource *pSource, ImageGrabberDevice *device)
{
    qDebug() << "MfGrabber::enumerateCaptureFormats --> Start";

    HRESULT                    hr = S_OK;
    IMFPresentationDescriptor* pPD = NULL;
    IMFStreamDescriptor*       pSD = NULL;
    IMFMediaTypeHandler*       pHandler = NULL;
    IMFMediaType*              pType = NULL;
    BOOL                       fSelected;
    DWORD                      cTypes = 0;

    hr = pSource->CreatePresentationDescriptor(&pPD);
    if (FAILED(hr)) {
        goto done;
    }

    hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);
    if (FAILED(hr)) {
        goto done;
    }

    hr = pSD->GetMediaTypeHandler(&pHandler);
    if (FAILED(hr)) {
        goto done;
    }

    hr = pHandler->GetMediaTypeCount(&cTypes);
    if (FAILED(hr)) {
        goto done;
    }

    for (DWORD i = 0; i < cTypes; i++) {
        PROPVARIANT  val;
        unsigned int outputWidth = 0;
        unsigned int outputHeight = 0;
        unsigned int outputFormat = 0;
        QString      GUID_name;
        UINT32       uHigh = 0;
        UINT32       uLow = 0;

        PropVariantInit(&val);

        hr = pHandler->GetMediaTypeByIndex(i, &pType);
        if (FAILED(hr)) {
            goto done;
        }

        // LogMediaType(pType);

        // MediaType MT = FormatReader::Read(pType);
        hr = pType->GetItem(MF_MT_FRAME_SIZE, &val);
        if (FAILED(hr)) {
            goto done;
        }

        // LogUINT32AsUINT64New(val, uHigh, uLow);
        Unpack2UINT32AsUINT64(val.uhVal.QuadPart, &uHigh, &uLow);
        outputWidth = uHigh;
        outputHeight = uLow;

        hr = PropVariantClear(&val);

        // Read the Videoundertyp-GUID
        hr = pType->GetItem(MF_MT_SUBTYPE, &val);
        if (FAILED(hr)) {
            goto done;
        }

        // GUID is in val.puuid
        GUID_name = GetGUIDNameConst(*val.puuid);
        if (*val.puuid == MFVideoFormat_RGB24) {
            outputFormat = GrabberResolution::bgr24Format;
        }
        else if (*val.puuid == MFVideoFormat_AYUV) {
            outputFormat = GrabberResolution::ayuvFormat;
        }
        else if (*val.puuid == MFVideoFormat_YUY2) {
            outputFormat = GrabberResolution::yuv2Format;
        }
        else if (*val.puuid == MFVideoFormat_UYVY) {
            outputFormat = GrabberResolution::uyvyFormat;
        }
        else if (*val.puuid == MFVideoFormat_YV12) {
            outputFormat = GrabberResolution::yv12Format;
        }
        else if (*val.puuid == MFVideoFormat_I420) {
            outputFormat = GrabberResolution::i420Format;
        }
        else if (*val.puuid == MFVideoFormat_NV12) {
            outputFormat = GrabberResolution::nv12Format;
        }
        // MJPEG is not supported!
        // else if (*val.puuid == MFVideoFormat_MJPG) {
        //     outputFormat = GrabberResolution::mjpegFormat;
        // }
        else {
            qDebug() << "MfGrabber::enumerateCaptureFormats --> Not supported format (" << i << "): " << outputWidth << " x " << outputHeight << " [" << GUID_name << "]";
            goto nextType;
        }

        device->addResolution(GrabberResolution(outputWidth, outputHeight, outputFormat, false));

nextType:
        hr = PropVariantClear(&val);

        SafeRelease(&pType);
    }

done:
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    SafeRelease(&pType);

    qDebug() << "MfGrabber::enumerateCaptureFormats --> End";

    return hr;
}


HRESULT MfGrabber::SetDeviceFormat(IMFMediaSource *pSource, DWORD dwFormatIndex)
{
    qDebug() << "MfGrabber::SetDeviceFormat --> Start";

    IMFPresentationDescriptor* pPD = NULL;
    IMFStreamDescriptor*       pSD = NULL;
    IMFMediaTypeHandler*       pHandler = NULL;
    IMFMediaType*              pType = NULL;

    HRESULT hr = pSource->CreatePresentationDescriptor(&pPD);
    if (FAILED(hr))
    {
        goto done;
    }

    BOOL fSelected;
    hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);
    if (FAILED(hr))
    {
        goto done;
    }

    hr = pSD->GetMediaTypeHandler(&pHandler);
    if (FAILED(hr))
    {
        goto done;
    }

    hr = pHandler->GetMediaTypeByIndex(dwFormatIndex, &pType);
    if (FAILED(hr))
    {
        goto done;
    }

    hr = pHandler->SetCurrentMediaType(pType);

done:
    SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pHandler);
    SafeRelease(&pType);

    qDebug() << "MfGrabber::SetDeviceFormat --> End";

    return hr;
}


void MfGrabber::getRawFrame(const uchar*& data)
{
    qDebug() << "MfGrabber::getRawFrame --> Start";

    HRESULT         hr = S_OK;
    IMFMediaBuffer* buffer = NULL;
    IMFSample*      sample = NULL;
    DWORD           dwFlags = 0x00000000U;
    BYTE*           pixels = NULL;
    DWORD           nPixels = 0;

    data = NULL;

    hr = sourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                  0,
                                  NULL,
                                  &dwFlags,
                                  NULL,
                                  &sample);
    if (FAILED(hr) ||
        (sample == NULL) ||
        ((dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) != 0)) {
        goto done;
    }

    hr = sample->ConvertToContiguousBuffer(&buffer);
    if (FAILED(hr)) {
        goto done;
    }

    hr = buffer->Lock(&pixels, NULL, &nPixels);
    if (FAILED(hr)) {
        goto done;
    }

    // Prepare the buffer to convert to QImage::Format_ARGB32
    if (nPixels < 1) {
        goto unlock;
    }

    if (frameSize != QSize(width, height)) {
        frameSize = QSize(width, height);
        safeDeleteArray(frameData);
        frameData = new uchar[width * height * 4];  // New frame data for ARGB 8bit
    }
    data = frameData;

    if (subType == MFVideoFormat_RGB24) {
        // Read a image in RGB24 format ==> TextureFormat::Format_RGB_8

        convert_bgr24_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_AYUV) {
        // Read a image in AYUV (AYCbCr BT.601)
        // 4:4:4 Format, 32 Bits per Pixel

        convert_ayuv_to_abgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_YUY2) {
        // Read a image in YUY2 (YCbY2) format ==> TextureFormat::Format_YUY2
        // 4:2:2 Format, 16 Bits per Pixel

        convert_yuy2_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_UYVY) {
        // Read a image in UYVY (CbYCrY)
        // 4:2:2 Format, 16 Bits per Pixel

        convert_uyvy_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_YV12) {
        // Read a image in YV12 (YCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        convert_yv12_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_I420) {
        // Read a image in I420 (YCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        convert_i420_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_NV12) {
        // Read a image in NV12 (NCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        convert_nv12_to_xbgr32_buffer(pixels, frameData, width, height, nPixels, stride);
    }
    if (subType == MFVideoFormat_MJPG) {
        // Read a image in MJPG format

        // Not supported!
    }

    // MFVideoFormat_RGB32 ==> TextureFormat::Format_RGBA_8
    // MFVideoFormat_ARGB32

unlock:
    buffer->Unlock();

done:
    SafeRelease(&buffer);
    SafeRelease(&sample);

    qDebug() << "MfGrabber::getRawFrame --> End";
}
