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

    mediaSource = NULL;
    sourceReader = NULL;

    frameData = NULL;
    frameLength = 0;

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
            device->setDeviceIndex(i);

            // Add the Controller to the new device
            deviceController = new MfController(0);
            device->setController(deviceController);

            hr = readDeviceInfo(ppDevices[i], i, device);
            if (SUCCEEDED(hr)) {
                // Add the new device to the device list
                devices.append(device);

                // Initalize the device controller
                deviceController->init(device->getDeviceId());
            }
            else {
                // Delete the new device controller
                delete deviceController;
                deviceController = NULL;
                device->setController(NULL);

                // Delete the new device
                delete device;
                device = NULL;
            }

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
    int                 deviceIndex = videoDevice->getDeviceIndex();

    HRESULT          hr = S_OK;
    IMFAttributes   *devAttr = NULL;
    IMFActivate    **camDevices = NULL;
    UINT32           camCount = 0;
    IMFMediaType    *mediaType = NULL;
    GUID             subtype = { 0 };

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
//-----------------------------

    IMFMediaType *pMType = NULL;
    DWORD         dwMediaTypeIndex = 0;
    GUID          MajorType;
    GUID          SubType;

    while (SUCCEEDED(hr)) {
        hr = sourceReader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                              dwMediaTypeIndex,
                                              &pMType);
        if (hr == MF_E_NO_MORE_TYPES) {
            hr = S_OK;
            break;
        }
        if (!SUCCEEDED(hr)) {
            SafeRelease(&sourceReader);
            SafeRelease(&mediaSource);
            return false;
        }

        pMType->GetGUID(MF_MT_MAJOR_TYPE, &MajorType);
        pMType->GetGUID(MF_MT_SUBTYPE, &SubType);

        QString MajorTypeString = GetGUIDNameConstNew(MajorType);
        QString SubTypeString = GetGUIDNameConstNew(SubType);

        qDebug() << "MfGrabber::setUp --> Media Type (" << dwMediaTypeIndex << "): " << MajorTypeString << " - " << SubTypeString;

        SafeRelease(&pMType);
        dwMediaTypeIndex++;
    }
/*
    hr = pMType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB24);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    // LogMediaType(pMType);

    hr = sourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           NULL,
                                           pMType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }
*/


//------------------------------
    hr = sourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           &mediaType);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    mediaType->GetGUID(MF_MT_MAJOR_TYPE, &MajorType);
    mediaType->GetGUID(MF_MT_SUBTYPE, &SubType);

    QString MajorTypeString = GetGUIDNameConstNew(MajorType);
    QString SubTypeString = GetGUIDNameConstNew(SubType);

    qDebug() << "MfGrabber::setUp --> Current Media Type: " << MajorTypeString << " - " << SubTypeString;

    hr = mediaType->GetGUID(MF_MT_SUBTYPE, &subtype);
    if (!SUCCEEDED(hr)) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    if (subtype == MFVideoFormat_YUY2) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }
    if (subtype != MFVideoFormat_RGB24) {
        SafeRelease(&sourceReader);
        SafeRelease(&mediaSource);
        return false;
    }

    qDebug() << "MfGrabber::setUp --> End";

    return true;
}

bool MfGrabber::grab()
{
    return true;
}


const QImage MfGrabber::getImage()
{
    QImage image;
    int w = -1, h = -1;
    const BYTE* data = NULL;
    int Tries = 10;

    while (data == NULL && --Tries) {
        getRawFrame(data, w, h);
    }
    if (data != NULL && w > 0 && h > 0) {
        image = QImage(QSize(w, h), QImage::Format_ARGB32);
        BYTE* dst = const_cast<BYTE*>(image.constBits());
        const BYTE* src = const_cast<BYTE*>(data);
        const BYTE* const srcEnd = src + w * h * 3;
        while (src < srcEnd) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = 0xffU;
        }
    }

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

    SafeRelease(&sourceReader);
    SafeRelease(&mediaSource);
    // SafeRelease(&mediaPlayer);

    qDebug() << "MfGrabber::tearDown --> End";

    return true;
}


HRESULT MfGrabber::readDeviceInfo(IMFActivate *pActivate, unsigned int /*Num*/, ImageGrabberDevice *device)
{
    qDebug() << "MfGrabber::readDeviceInfo --> Start";

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

        SafeRelease(&pSource);

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
        PROPVARIANT  val;
        unsigned int outputWidth = 0;
        unsigned int outputHeight = 0;
        QString      GUID_name;
        UINT32       uHigh = 0, uLow = 0;

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

        hr = PropVariantClear(&val);

        qDebug() << "MfGrabber::enumerateCaptureFormats --> Format (" << i << "): " << outputWidth << " x " << outputHeight << " [" << GUID_name << "]";

        // vd_CurrentFormats.push_back(MT);
        device->getController()->addResolution(GrabberResolution(0, outputWidth, outputHeight, false));

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

    IMFPresentationDescriptor *pPD = NULL;
    IMFStreamDescriptor *pSD = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType *pType = NULL;

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

/*
void MfGrabber::getRawFrame(const uchar*& data, int& length)
{
    HRESULT         hr = S_OK;
    IMFMediaBuffer *buffer = NULL;
    IMFSample      *sample = NULL;
    GUID            subtype = { 0 };
    IMFMediaType   *mediaType = NULL;
    DWORD           dwFlags = 0x00000000U;
    BYTE           *samples = NULL;
    DWORD           nSamples = 0;

    data = NULL;
    length = -1;

    hr = sourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
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

    hr = sourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
                                           &mediaType);
    if (FAILED(hr)) {
        goto done;
    }

    hr = mediaType->GetGUID(MF_MT_SUBTYPE, &subtype);
    if (subtype != MFAudioFormat_MP3) {
        goto done;
    }

    hr = sample->ConvertToContiguousBuffer(&buffer);
    if (FAILED(hr)) {
        goto done;
    }

    hr = buffer->Lock(&samples, NULL, &nSamples);
    if (FAILED(hr)) {
        goto done;
    }

    if (nSamples > 0) {
        if (frameLength != nSamples) {
            frameLength = nSamples;
            safeDeleteArray(frameData);
            frameData = new uchar[nSamples];
        }
        length = frameLength;
        data = frameData;
        const uchar* src = samples;
        uchar* dst = const_cast<uchar*>(data);
        const uchar* const dstEnd = dst + nSamples;
        while (dst < dstEnd) {
            *dst++ = *src++;
        }
    }
    buffer->Unlock();

done:
    SafeRelease(&buffer);
    SafeRelease(&sample);
}
*/

void MfGrabber::getRawFrame(const uchar*& data, int& w, int& h)
{
    qDebug() << "MfGrabber::getRawFrame --> Start";

    HRESULT         hr = S_OK;
    IMFMediaBuffer *buffer = NULL;
    IMFSample      *sample = NULL;
    GUID            subtype = { 0 };
    IMFMediaType   *mediaType = NULL;
    DWORD           dwFlags = 0x00000000U;
    BYTE           *pixels = NULL;
    DWORD           nPixels = 0;

    data = NULL;
    w = -1;
    h = -1;

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

    hr = sourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           &mediaType);
    if (FAILED(hr)) {
        goto done;
    }

    hr = mediaType->GetGUID(MF_MT_SUBTYPE, &subtype);
    if (subtype == MFVideoFormat_RGB24) {
        // Read a image in RGB24 format ==> TextureFormat::Format_RGB_8
        hr = MFGetAttributeSize(mediaType, MF_MT_FRAME_SIZE, (UINT32*)&w, (UINT32*)&h);
        if (FAILED(hr)) {
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
        if (nPixels > 0 && w > 0 && h > 0) {
            if (frameSize != QSize(w, h)) {
                frameSize = QSize(w, h);
                safeDeleteArray(frameData);
                frameData = new uchar[nPixels];
            }
            data = frameData;
            uchar* dst = const_cast<uchar*>(data);
            const uchar* const dstEnd = dst + nPixels;
            LONG stride = (LONG)MFGetAttributeUINT32(mediaType, MF_MT_DEFAULT_STRIDE, 1);
            bool upsideDown = (stride < 0);
            if (upsideDown) {
                stride = -stride;
                if (stride != w*3) {
                    goto done;
                }
                for (int scanLine = h-1; scanLine >= 0; --scanLine) {
                    const uchar* src = pixels + scanLine * stride;
                    const uchar* const srcEnd = src + w * 3;
                    while (src < srcEnd) {
                        *dst++ = *src++;
                        *dst++ = *src++;
                        *dst++ = *src++;
                    }
                }
            }
            else {
                const uchar* src = pixels;
                while (dst < dstEnd) {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                }
            }
        }
        buffer->Unlock();
    }
    if (subtype == MFVideoFormat_AYUV) {
        // Read a image in AYUV (AYCbCr BT.601)
        // 4:4:4 Format, 32 Bits per Pixel
    }
    if (subtype == MFVideoFormat_YUY2) {
        // Read a image in YUY2 (YCbY2) format ==> TextureFormat::Format_YUY2
        // 4:2:2 Format, 16 Bits per Pixel
    }
    if (subtype == MFVideoFormat_UYVY) {
        // Read a image in UYVY (CbYCrY)
        // 4:2:2 Format, 16 Bits per Pixel
    }
    if (subtype == MFVideoFormat_YV12) {
        // Read a image in YV12 (YCr12)
        // 4:2:0 Format, 12 Bits per Pixel
    }
    if (subtype == MFVideoFormat_NV12) {
        // Read a image in NV12 (NCr12)
        // 4:2:0 Format, 12 Bits per Pixel
    }
    if (subtype == MFVideoFormat_MJPG) {
        // Read a image in MJPG format
    }

    // MFVideoFormat_I420
    // MFVideoFormat_RGB32 ==> TextureFormat::Format_RGBA_8
    // MFVideoFormat_ARGB32
done:
    SafeRelease(&buffer);
    SafeRelease(&sample);

    qDebug() << "MfGrabber::getRawFrame --> End";
}

/*
HRESULT MfGrabber::LogMediaType(IMFMediaType *pType)
{
    qDebug() << "MfGrabber::LogMediaType --> Start";

    UINT32 count = 0;

    HRESULT hr = pType->GetCount(&count);
    if (FAILED(hr))
    {
        return hr;
    }

    if (count == 0)
    {
        qDebug() << "MfGrabber::LogMediaType --> Empty media type.";
    }

    for (UINT32 i = 0; i < count; i++)
    {
        hr = LogAttributeValueByIndex(pType, i);
        if (FAILED(hr))
        {
            break;
        }
    }

    qDebug() << "MfGrabber::LogMediaType --> End";

    return hr;
}


HRESULT MfGrabber::LogAttributeValueByIndex(IMFAttributes *pAttr, DWORD index)
{
    qDebug() << "MfGrabber::LogAttributeValueByIndex --> Start";

    WCHAR *pGuidName = NULL;
    WCHAR *pGuidValName = NULL;

    GUID guid = { 0 };

    PROPVARIANT var;
    PropVariantInit(&var);

    HRESULT hr = pAttr->GetItemByIndex(index, &guid, &var);
    if (FAILED(hr))
    {
        goto done;
    }

    hr = GetGUIDName(guid, &pGuidName);
    if (FAILED(hr))
    {
        goto done;
    }

    qDebug() << "MfGrabber::LogAttributeValueByIndex --> GUID-Name: " << pGuidName;

    hr = SpecialCaseAttributeValue(guid, var);
    if (FAILED(hr))
    {
        goto done;
    }
    if (hr == S_FALSE)
    {
        switch (var.vt)
        {
        case VT_UI4:
            DBGMSG(L"%d", var.ulVal);
            break;

        case VT_UI8:
            DBGMSG(L"%I64d", var.uhVal);
            break;

        case VT_R8:
            DBGMSG(L"%f", var.dblVal);
            break;

        case VT_CLSID:
            hr = GetGUIDName(*var.puuid, &pGuidValName);
            if (SUCCEEDED(hr))
            {
                DBGMSG(pGuidValName);
            }
            break;

        case VT_LPWSTR:
            DBGMSG(var.pwszVal);
            break;

        case VT_VECTOR | VT_UI1:
            DBGMSG(L"<<byte array>>");
            break;

        case VT_UNKNOWN:
            DBGMSG(L"IUnknown");
            break;

        default:
            DBGMSG(L"Unexpected attribute type (vt = %d)", var.vt);
            break;
        }
    }

done:
    DBGMSG(L"\n");
    CoTaskMemFree(pGuidName);
    CoTaskMemFree(pGuidValName);
    PropVariantClear(&var);

    qDebug() << "MfGrabber::LogAttributeValueByIndex --> End";
    return hr;
}
*/

void MfGrabber::ConvertAYUV_to_YUY2(const BYTE *pSrcBuffer, BYTE *pDestBuffer, DWORD width, DWORD height, LONG lSrcStride, LONG lDestStride)
{
    /*
    const BYTE *pRow0 = pSrcBuffer;

#ifdef CONVERT_WITH_CHROMA_FILTERING

    // filter chroma
    for (DWORD y = 0; y < height; y += 1)
    {
        AYUVSample *pSrc = (AYUVSample*)pRow0;

        for (DWORD x = 2; x < width - 2; x += 2)
        {
            pSrc[x].Cb = (pSrc[x - 1].Cb >> 2) + ((pSrc[x].Cb + 1) >> 1) + (pSrc[x + 1].Cb >> 2);
            pSrc[x].Cr = (pSrc[x - 1].Cr >> 2) + ((pSrc[x].Cr + 1) >> 1) + (pSrc[x + 1].Cr >> 2);
        }

        pRow0 += lSrcStride;
    }

#endif

    // Convert to YUY2
    for (DWORD y = 0; y < height; y++)
    {
        AYUVSample *pSrc = (AYUVSample*)pSrcBuffer;
        DWORD *pDest = (DWORD*)pDestBuffer;

        for (DWORD x = 0; x < width / 2; x += 1)
        {
            const BYTE y0 = pSrc[x * 2].Y;
            const BYTE u0 = pSrc[x * 2].Cb;
            const BYTE v0 = pSrc[x * 2].Cr;
            const BYTE y1 = pSrc[x * 2 + 1].Y;

            pDest[x] = y0 | (u0 << 8) | (y1 << 16) | (v0 << 24);
        }

        pSrcBuffer += lSrcStride;
        pDestBuffer += lDestStride;
    }
    */
}

QString MfGrabber::GetGUIDNameConstNew(const GUID& guid)
{
    IF_EQUAL_RETURN(guid, MF_MT_MAJOR_TYPE);
    IF_EQUAL_RETURN(guid, MF_MT_MAJOR_TYPE);
    IF_EQUAL_RETURN(guid, MF_MT_SUBTYPE);
    IF_EQUAL_RETURN(guid, MF_MT_ALL_SAMPLES_INDEPENDENT);
    IF_EQUAL_RETURN(guid, MF_MT_FIXED_SIZE_SAMPLES);
    IF_EQUAL_RETURN(guid, MF_MT_COMPRESSED);
    IF_EQUAL_RETURN(guid, MF_MT_SAMPLE_SIZE);
    IF_EQUAL_RETURN(guid, MF_MT_WRAPPED_TYPE);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_NUM_CHANNELS);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_SAMPLES_PER_SECOND);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_FLOAT_SAMPLES_PER_SECOND);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_AVG_BYTES_PER_SECOND);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_BLOCK_ALIGNMENT);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_BITS_PER_SAMPLE);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_VALID_BITS_PER_SAMPLE);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_SAMPLES_PER_BLOCK);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_CHANNEL_MASK);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_FOLDDOWN_MATRIX);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_WMADRC_PEAKREF);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_WMADRC_PEAKTARGET);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_WMADRC_AVGREF);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_WMADRC_AVGTARGET);
    IF_EQUAL_RETURN(guid, MF_MT_AUDIO_PREFER_WAVEFORMATEX);
    IF_EQUAL_RETURN(guid, MF_MT_AAC_PAYLOAD_TYPE);
    IF_EQUAL_RETURN(guid, MF_MT_AAC_AUDIO_PROFILE_LEVEL_INDICATION);
    IF_EQUAL_RETURN(guid, MF_MT_FRAME_SIZE);
    IF_EQUAL_RETURN(guid, MF_MT_FRAME_RATE);
    IF_EQUAL_RETURN(guid, MF_MT_FRAME_RATE_RANGE_MAX);
    IF_EQUAL_RETURN(guid, MF_MT_FRAME_RATE_RANGE_MIN);
    IF_EQUAL_RETURN(guid, MF_MT_PIXEL_ASPECT_RATIO);
    IF_EQUAL_RETURN(guid, MF_MT_DRM_FLAGS);
    IF_EQUAL_RETURN(guid, MF_MT_PAD_CONTROL_FLAGS);
    IF_EQUAL_RETURN(guid, MF_MT_SOURCE_CONTENT_HINT);
    IF_EQUAL_RETURN(guid, MF_MT_VIDEO_CHROMA_SITING);
    IF_EQUAL_RETURN(guid, MF_MT_INTERLACE_MODE);
    IF_EQUAL_RETURN(guid, MF_MT_TRANSFER_FUNCTION);
    IF_EQUAL_RETURN(guid, MF_MT_VIDEO_PRIMARIES);
    IF_EQUAL_RETURN(guid, MF_MT_CUSTOM_VIDEO_PRIMARIES);
    IF_EQUAL_RETURN(guid, MF_MT_YUV_MATRIX);
    IF_EQUAL_RETURN(guid, MF_MT_VIDEO_LIGHTING);
    IF_EQUAL_RETURN(guid, MF_MT_VIDEO_NOMINAL_RANGE);
    IF_EQUAL_RETURN(guid, MF_MT_GEOMETRIC_APERTURE);
    IF_EQUAL_RETURN(guid, MF_MT_MINIMUM_DISPLAY_APERTURE);
    IF_EQUAL_RETURN(guid, MF_MT_PAN_SCAN_APERTURE);
    IF_EQUAL_RETURN(guid, MF_MT_PAN_SCAN_ENABLED);
    IF_EQUAL_RETURN(guid, MF_MT_AVG_BITRATE);
    IF_EQUAL_RETURN(guid, MF_MT_AVG_BIT_ERROR_RATE);
    IF_EQUAL_RETURN(guid, MF_MT_MAX_KEYFRAME_SPACING);
    IF_EQUAL_RETURN(guid, MF_MT_DEFAULT_STRIDE);
    IF_EQUAL_RETURN(guid, MF_MT_PALETTE);
    IF_EQUAL_RETURN(guid, MF_MT_USER_DATA);
    IF_EQUAL_RETURN(guid, MF_MT_AM_FORMAT_TYPE);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG_START_TIME_CODE);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG2_PROFILE);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG2_LEVEL);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG2_FLAGS);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG_SEQUENCE_HEADER);
    IF_EQUAL_RETURN(guid, MF_MT_DV_AAUX_SRC_PACK_0);
    IF_EQUAL_RETURN(guid, MF_MT_DV_AAUX_CTRL_PACK_0);
    IF_EQUAL_RETURN(guid, MF_MT_DV_AAUX_SRC_PACK_1);
    IF_EQUAL_RETURN(guid, MF_MT_DV_AAUX_CTRL_PACK_1);
    IF_EQUAL_RETURN(guid, MF_MT_DV_VAUX_SRC_PACK);
    IF_EQUAL_RETURN(guid, MF_MT_DV_VAUX_CTRL_PACK);
    IF_EQUAL_RETURN(guid, MF_MT_ARBITRARY_HEADER);
    IF_EQUAL_RETURN(guid, MF_MT_ARBITRARY_FORMAT);
    IF_EQUAL_RETURN(guid, MF_MT_IMAGE_LOSS_TOLERANT);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG4_SAMPLE_DESCRIPTION);
    IF_EQUAL_RETURN(guid, MF_MT_MPEG4_CURRENT_SAMPLE_ENTRY);
    IF_EQUAL_RETURN(guid, MF_MT_ORIGINAL_4CC);
    IF_EQUAL_RETURN(guid, MF_MT_ORIGINAL_WAVE_FORMAT_TAG);

    // Media types

    IF_EQUAL_RETURN(guid, MFMediaType_Audio);
    IF_EQUAL_RETURN(guid, MFMediaType_Video);
    IF_EQUAL_RETURN(guid, MFMediaType_Protected);
    IF_EQUAL_RETURN(guid, MFMediaType_SAMI);
    IF_EQUAL_RETURN(guid, MFMediaType_Script);
    IF_EQUAL_RETURN(guid, MFMediaType_Image);
    IF_EQUAL_RETURN(guid, MFMediaType_HTML);
    IF_EQUAL_RETURN(guid, MFMediaType_Binary);
    IF_EQUAL_RETURN(guid, MFMediaType_FileTransfer);

    IF_EQUAL_RETURN(guid, MFVideoFormat_AI44); //     FCC('AI44')
    IF_EQUAL_RETURN(guid, MFVideoFormat_ARGB32); //   D3DFMT_A8R8G8B8
    IF_EQUAL_RETURN(guid, MFVideoFormat_AYUV); //     FCC('AYUV')
    IF_EQUAL_RETURN(guid, MFVideoFormat_DV25); //     FCC('dv25')
    IF_EQUAL_RETURN(guid, MFVideoFormat_DV50); //     FCC('dv50')
    IF_EQUAL_RETURN(guid, MFVideoFormat_DVH1); //     FCC('dvh1')
    IF_EQUAL_RETURN(guid, MFVideoFormat_DVSD); //     FCC('dvsd')
    IF_EQUAL_RETURN(guid, MFVideoFormat_DVSL); //     FCC('dvsl')
    IF_EQUAL_RETURN(guid, MFVideoFormat_H264); //     FCC('H264')
    IF_EQUAL_RETURN(guid, MFVideoFormat_I420); //     FCC('I420')
    IF_EQUAL_RETURN(guid, MFVideoFormat_IYUV); //     FCC('IYUV')
    IF_EQUAL_RETURN(guid, MFVideoFormat_M4S2); //     FCC('M4S2')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MJPG);
    IF_EQUAL_RETURN(guid, MFVideoFormat_MP43); //     FCC('MP43')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MP4S); //     FCC('MP4S')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MP4V); //     FCC('MP4V')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MPG1); //     FCC('MPG1')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MSS1); //     FCC('MSS1')
    IF_EQUAL_RETURN(guid, MFVideoFormat_MSS2); //     FCC('MSS2')
    IF_EQUAL_RETURN(guid, MFVideoFormat_NV11); //     FCC('NV11')
    IF_EQUAL_RETURN(guid, MFVideoFormat_NV12); //     FCC('NV12')
    IF_EQUAL_RETURN(guid, MFVideoFormat_P010); //     FCC('P010')
    IF_EQUAL_RETURN(guid, MFVideoFormat_P016); //     FCC('P016')
    IF_EQUAL_RETURN(guid, MFVideoFormat_P210); //     FCC('P210')
    IF_EQUAL_RETURN(guid, MFVideoFormat_P216); //     FCC('P216')
    IF_EQUAL_RETURN(guid, MFVideoFormat_RGB24); //    D3DFMT_R8G8B8
    IF_EQUAL_RETURN(guid, MFVideoFormat_RGB32); //    D3DFMT_X8R8G8B8
    IF_EQUAL_RETURN(guid, MFVideoFormat_RGB555); //   D3DFMT_X1R5G5B5
    IF_EQUAL_RETURN(guid, MFVideoFormat_RGB565); //   D3DFMT_R5G6B5
    IF_EQUAL_RETURN(guid, MFVideoFormat_RGB8);
    IF_EQUAL_RETURN(guid, MFVideoFormat_UYVY); //     FCC('UYVY')
    IF_EQUAL_RETURN(guid, MFVideoFormat_v210); //     FCC('v210')
    IF_EQUAL_RETURN(guid, MFVideoFormat_v410); //     FCC('v410')
    IF_EQUAL_RETURN(guid, MFVideoFormat_WMV1); //     FCC('WMV1')
    IF_EQUAL_RETURN(guid, MFVideoFormat_WMV2); //     FCC('WMV2')
    IF_EQUAL_RETURN(guid, MFVideoFormat_WMV3); //     FCC('WMV3')
    IF_EQUAL_RETURN(guid, MFVideoFormat_WVC1); //     FCC('WVC1')
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y210); //     FCC('Y210')
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y216); //     FCC('Y216')
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y410); //     FCC('Y410')
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y416); //     FCC('Y416')
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y41P);
    IF_EQUAL_RETURN(guid, MFVideoFormat_Y41T);
    IF_EQUAL_RETURN(guid, MFVideoFormat_YUY2); //     FCC('YUY2')
    IF_EQUAL_RETURN(guid, MFVideoFormat_YV12); //     FCC('YV12')
    IF_EQUAL_RETURN(guid, MFVideoFormat_YVYU);

    IF_EQUAL_RETURN(guid, MFAudioFormat_PCM); //              WAVE_FORMAT_PCM
    IF_EQUAL_RETURN(guid, MFAudioFormat_Float); //            WAVE_FORMAT_IEEE_FLOAT
    IF_EQUAL_RETURN(guid, MFAudioFormat_DTS); //              WAVE_FORMAT_DTS
    IF_EQUAL_RETURN(guid, MFAudioFormat_Dolby_AC3_SPDIF); //  WAVE_FORMAT_DOLBY_AC3_SPDIF
    IF_EQUAL_RETURN(guid, MFAudioFormat_DRM); //              WAVE_FORMAT_DRM
    IF_EQUAL_RETURN(guid, MFAudioFormat_WMAudioV8); //        WAVE_FORMAT_WMAUDIO2
    IF_EQUAL_RETURN(guid, MFAudioFormat_WMAudioV9); //        WAVE_FORMAT_WMAUDIO3
    IF_EQUAL_RETURN(guid, MFAudioFormat_WMAudio_Lossless); // WAVE_FORMAT_WMAUDIO_LOSSLESS
    IF_EQUAL_RETURN(guid, MFAudioFormat_WMASPDIF); //         WAVE_FORMAT_WMASPDIF
    IF_EQUAL_RETURN(guid, MFAudioFormat_MSP1); //             WAVE_FORMAT_WMAVOICE9
    IF_EQUAL_RETURN(guid, MFAudioFormat_MP3); //              WAVE_FORMAT_MPEGLAYER3
    IF_EQUAL_RETURN(guid, MFAudioFormat_MPEG); //             WAVE_FORMAT_MPEG
    IF_EQUAL_RETURN(guid, MFAudioFormat_AAC); //              WAVE_FORMAT_MPEG_HEAAC
    IF_EQUAL_RETURN(guid, MFAudioFormat_ADTS); //             WAVE_FORMAT_MPEG_ADTS_AAC

    return NULL;
}

