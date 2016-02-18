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

#include "v4l2grabber.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

// #include <sys/ioctl.h>
#include <sys/mman.h>
#include <libv4l2.h>
#include <fcntl.h>
#include <errno.h>

#include "v4l2controller.h"

#include "technical/util.h"
#include "technical/grabber/imageconverter.h"


V4L2Grabber::V4L2Grabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug() << "V4L2Grabber::Constructor --> Start";

    fd = -1;
    errno = -1;
    dst_buf = NULL;
    frameData = NULL;

    qDebug() << "V4L2Grabber::Constructor --> End";
}


V4L2Grabber::~V4L2Grabber()
{
    qDebug() << "V4L2Grabber::Destructor --> Start";

    if (fd >= 0) {
        // Close the device file
        // close(fd);
        v4l2_close(fd);
        fd = -1;
    }

    qDebug() << "V4L2Grabber::Destructor --> End";
}


bool V4L2Grabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug() << "V4L2Grabber::initialization --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              value;
    int              deviceCount = 0;

    // Enumerate over all /dev/videoN (N = 0...x, x big enought)
    for (int i = 0; i < 10; i++) {
        QString              deviceName = QString("%1%2")
                                          .arg(QLatin1String("/dev/video"))
                                          .arg(i);
        ImageGrabberDevice*  device = NULL;
        V4L2Controller*      deviceController = NULL;
        bool                 ret;

        // fd = open(deviceName.toAscii(), O_RDONLY);
        fd = v4l2_open(deviceName.toLatin1(), O_RDWR | O_NONBLOCK, 0);
        if(fd < 0){
            qDebug() << "V4L2Grabber::initialization --> Error: Can't open device >" << deviceName << "<";
            continue;
        }

        device = new ImageGrabberDevice();
        device->setDeviceIndex(i);
        ret = readDeviceInfo(fd, device);
        if (ret) {
            // Add the new device to the device list
            devices.append(device);
            deviceCount++;

            // Add the Controller to the new device
            if (pref->getIntegerPreference("preferences", "v4l2controller", value) == false) {
                value = false;
            }
            if ((int)true == value) {
                deviceController = new V4L2Controller(0);
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

        // close(fd);
        v4l2_close(fd);
        fd = -1;
    }

    qDebug() << "V4L2Grabber::initialization --> device count: " << devices.size();

    if (0 == deviceCount) {
        qDebug() << "V4L2Grabber::initialization --> End (false)";

        return false;
    }

    qDebug() << "V4L2Grabber::initialization --> End (true)";

    return true;
}


bool V4L2Grabber::setUp()
{
    qDebug() << "V4L2Grabber::setUp --> Start";

    int                          videoSource = frontend->getProject()->getVideoSource();
    ImageGrabberDevice*          videoDevice = frontend->getDevice(videoSource);
    int                          deviceIndex = videoDevice->getDeviceIndex();
    QString                      deviceName = QString("%1%2")
                                              .arg(QLatin1String("/dev/video"))
                                              .arg(deviceIndex);
    int                          resIndex = frontend->getProject()->getResolution();
    GrabberResolution            resolution = frontend->getResolution(resIndex);
    struct v4l2_requestbuffers   req;
    enum v4l2_buf_type           type;
    unsigned int                 pixelFormat;

    width = resolution.getWidth();
    height = resolution.getHeight();

    switch (resolution.getFormat()) {
    case GrabberResolution::bgr24Format:
        pixelFormat = V4L2_PIX_FMT_BGR24;
        break;
#ifdef V4L2_PIX_FMT_XBGR32
    case GrabberResolution::xbgr32Format:
        pixelFormat = V4L2_PIX_FMT_XBGR32;
        break;
#endif
#ifdef V4L2_PIX_FMT_ABGR32
    case GrabberResolution::abgr32Format:
        pixelFormat = V4L2_PIX_FMT_ABGR32;
        break;
#endif
    case GrabberResolution::yuv2Format:
        pixelFormat = V4L2_PIX_FMT_YUYV;
        break;
    case GrabberResolution::uyvyFormat:
        pixelFormat = V4L2_PIX_FMT_UYVY;
        break;
#ifdef V4L2_PIX_FMT_YVU420M
    case GrabberResolution::yv12Format:
        pixelFormat = V4L2_PIX_FMT_YVU420M;
        break;
#endif
#ifdef V4L2_PIX_FMT_YUV420M
    case GrabberResolution::i420Format:
        pixelFormat = V4L2_PIX_FMT_YUV420M;
        break;
#endif
#ifdef V4L2_PIX_FMT_NV12M
    case GrabberResolution::nv12Format:
        pixelFormat = V4L2_PIX_FMT_NV12M;
        break;
#endif
    case GrabberResolution::mjpegFormat:
        pixelFormat = V4L2_PIX_FMT_MJPEG;
        break;
    default:
        goto Error;
    }

    //-----------------------------
    // Open video device
    //-----------------------------

    // fd = open(deviceName.toAscii(), O_RDONLY);
    fd = v4l2_open(deviceName.toLatin1(), O_RDWR | O_NONBLOCK, 0);
    if(fd < 0){
        qDebug() << "V4L2Grabber::setUp --> Error: Can't open device >" << deviceName << "<";
        return false;
    }

    //-----------------------------
    // Find usable MediaType
    //-----------------------------

    memset(&format, 0, sizeof format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = width;
    format.fmt.pix.height = height;
    format.fmt.pix.field = V4L2_FIELD_ANY;
    format.fmt.pix.pixelformat = pixelFormat;


    if (xioctl(fd, VIDIOC_S_FMT, &format) < 0) {
        qDebug() << "V4L2Grabber::setUp --> Error: Can't set capture format";
        goto Error;
    }
    if (format.fmt.pix.pixelformat != pixelFormat) {
        qDebug() << "V4L2Grabber::setUp --> Error: Libv4l didn't accept RGB24 format. Can't proceed.";
        goto Error;
    }
    if ((format.fmt.pix.width != width) || (format.fmt.pix.height != height)) {
        width = format.fmt.pix.width;
        height = format.fmt.pix.height;
        qDebug() << "V4L2Grabber::setUp --> Warning: driver is sending image at " << width << "x" << height;
    }

    //-----------------------------
    // Create image buffer
    //-----------------------------

    dst_buf = (unsigned char*)malloc(format.fmt.pix.sizeimage);
    frameData = new unsigned char[width * height * 4];  // New frame data for ARGB 8bit

    memset(&req, 0, sizeof req);
    req.count = N_BUFFERS;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (xioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        qDebug() << "V4L2Grabber::setUp --> Error: Unable to allocate buffers";
        goto Error;
    }

    for (unsigned int i = 0; i < N_BUFFERS; i++) {
        memset(&buf, 0, sizeof buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            qDebug() << "V4L2Grabber::setUp --> Error: Unable to query buffer";
            goto Error;
        }

        buffers[i].length = buf.length;
        // buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        buffers[i].start = (void*)v4l2_mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (buffers[i].start == MAP_FAILED) {
            qDebug() << "V4L2Grabber::setUp --> Error: Unable to map buffer (" << strerror(errno) << ")";  // Error see mmap man page
            goto Error;
        }
    }
    
    for (unsigned int i = 0; i < N_BUFFERS; ++i) {
        memset(&buf, 0, sizeof buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            qDebug() << "V4L2Grabber::setUp --> Error: Unable to query buffer";
            goto Error;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        qDebug() << "V4L2Grabber::setUp --> Error: Unable to switch streaming on";
        goto Error;
    }

    qDebug() << "V4L2Grabber::setUp --> End";

    return true;

Error:
    if (dst_buf != NULL) {
        free(dst_buf);
        dst_buf = NULL;
    }
    if (fd > -1) {
        // close(fd);
        v4l2_close(fd);
        fd = -1;
    }
    
    return false;
}


bool V4L2Grabber::grab()
{
    return true;
}


const QImage V4L2Grabber::getImage()
{
    int      ret = 0;
    int      imageFormat = QImage::Format_ARGB32;
    QImage   image;

    qDebug() << "V4L2Grabber::getImage --> Start";
    
    // Dequeue a buffer.
    memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    ret = xioctl(fd, VIDIOC_DQBUF, &buf);
    if (ret < 0) {
        qDebug() << "V4L2Grabber::getImage --> Error: Unable to dequeue buffer (" << errno << ")";
        return image;
    }

    unsigned char*  pixels = NULL;
    unsigned int    nBytes = 0;

    // Save the image.
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_BGR24) {
        // Read a image in RGB24 format ==> TextureFormat::Format_RGB_8

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_bgr24_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
    // if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_xxx) {
        // Read a image in AYUV (AYCbCr BT.601)
        // 4:4:4 Format, 32 Bits per Pixel

        // nBytes = buf.bytesused;
        // pixels = (unsigned char*)malloc(nBytes);
        // memcpy(pixels, buffers[buf.index].start, nBytes);

        // convert_ayuv_to_abgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    // }
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
        // Read a image in YUY2 (YCbY2) format ==> TextureFormat::Format_YUY2
        // 4:2:2 Format, 16 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_yuy2_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_UYVY) {
        // Read a image in UYVY (CbYCrY)
        // 4:2:2 Format, 16 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_uyvy_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
#ifdef V4L2_PIX_FMT_YVU420M
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_YVU420M) {
        // Read a image in YV12 (YCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_yv12_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
#endif
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV420) {
        // Read a image in I420 (YCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_i420_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
#ifdef V4L2_PIX_FMT_NV12M
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_NV12M) {
        // Read a image in NV12 (NCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_nv12_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
#endif
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_NV12) {
        // Read a image in NV12 (NCr12)
        // 4:2:0 Format, 12 Bits per Pixel

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        convert_nv12_to_xbgr32_buffer(pixels, frameData, width, height, nBytes, 0L);
    }
    if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) {
        // Read a image in MJPG format

        nBytes = buf.bytesused;
        pixels = (unsigned char*)malloc(nBytes);
        memcpy(pixels, buffers[buf.index].start, nBytes);

        // Not supported?

        convert_mjpeg422_to_xbgr32_buffer(pixels, frameData, width, height/*, nBytes, 0L*/);

        // Example code: v4l2handcontrol
        // uchar jpegBuf1[buf.bytesused + 420];
        // if (format.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) {
        //     if (mjpegToJpeg(mem[buf.index], jpegBuf1, (int) buf.bytesused) == EXIT_SUCCESS)
        //     image.loadFromData(jpegBuf1, buf.bytesused+420);
        // }
    }

    // Requeue the buffer.
    ret = xioctl(fd, VIDIOC_QBUF, &buf);
    if (ret < 0) {
        qDebug() << "V4L2Grabber::getImage --> Error: Unable to requeue buffer (" << errno << ")";
        return image;
    }

    // Create image
    image = QImage(QSize(width, height), (QImage::Format)imageFormat);
    // unsigned char* dst = const_cast<unsigned char*>(image.bits());        // Up to Qt 4.6.x
    unsigned char* dst = const_cast<unsigned char*>(image.constBits());   // From Qt 4.7
    const unsigned char* src = const_cast<unsigned char*>(frameData);
    const unsigned char* const srcEnd = src + width * height * 4;

    while (src < srcEnd) {
        *dst++ = *src++;
    }

    if (firstImage) {
        firstImage = false;
    }
    
    qDebug() << "V4L2Grabber::getImage --> End";
	
    return image;
}


const QImage V4L2Grabber::getLiveImage()
{
    // liveImage.load(filePath);

    liveImage = getImage();

    return liveImage;
}


const QImage V4L2Grabber::getRawImage()
{
    // rawImage.load(filePath);

    rawImage = getImage();

    return rawImage;
}


bool V4L2Grabber::tearDown()
{
    enum v4l2_buf_type   type;

    qDebug() << "V4L2Grabber::tearDown --> Start";
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
        qDebug() << "V4L2Grabber::tearDown --> Error: Unable to switch streaming off";
        // goto Error;
    }

    for (unsigned int i = 0; i < N_BUFFERS; ++i) {
        // munmap(buffers[i].start, buffers[i].length);
        v4l2_munmap(buffers[i].start, buffers[i].length);
    }

    if (frameData != NULL) {
        delete frameData;
        frameData = NULL;
    }
    if (dst_buf != NULL) {
        free(dst_buf);
        dst_buf = NULL;
    }
    if (fd > -1) {
        // close(fd);
        v4l2_close(fd);
        fd = -1;
    }

    qDebug() << "V4L2Grabber::tearDown --> End";

    return true;
}


int V4L2Grabber::getFd()
{
    return fd;
}


bool V4L2Grabber::readDeviceInfo(int fd, ImageGrabberDevice *device)
{
    qDebug() << "V4L2Grabber::readDeviceInfo --> Start";

    struct v4l2_capability   video_cap;     // include/linux/videodev.h

    if(xioctl(fd, VIDIOC_QUERYCAP, &video_cap) == -1) {
        qDebug() << "V4L2Grabber::initialization --> Error: Can't get capabilities";
        return false;
    }

    qDebug() << "V4L2Grabber::initialization --> Driver:" << (char*)(video_cap.driver);
    qDebug() << "V4L2Grabber::initialization --> Name:" << (char*)(video_cap.card);

    device->setDeviceId(QLatin1String((const char*)video_cap.card));
    device->setDeviceName(QLatin1String((const char*)video_cap.card) + " (V4L2)");
    device->setDeviceSource(ImageGrabberDevice::video4Linux2Source);

    enumerateCaptureFormats(fd, device);

    qDebug() << "V4L2Grabber::readDeviceInfo --> End";

    return true;
}


bool V4L2Grabber::enumerateCaptureFormats(int fd, ImageGrabberDevice *device)
{
    qDebug() << "V4L2Grabber::enumerateCaptureFormats --> Start";

    enum v4l2_buf_type       type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    struct v4l2_fmtdesc      fmt;
    struct v4l2_frmsizeenum  frmsize;
    // struct v4l2_frmivalenum  frmival;
    unsigned int             outputWidth;
    unsigned int             outputHeight;

    fmt.index = 0;
    fmt.type = type;

    while (xioctl(fd, VIDIOC_ENUM_FMT, &fmt) >= 0) {

        qDebug() << "GrabberV4L2Controller::setResolutions --> Format Description: " << (char*)(fmt.description);

        frmsize.pixel_format = fmt.pixelformat;
        frmsize.index = 0;
        while (xioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0) {
            switch (frmsize.type) {
            case V4L2_FRMSIZE_TYPE_DISCRETE:
                outputWidth = frmsize.discrete.width;
                outputHeight = frmsize.discrete.height;
                break;
            case V4L2_FRMSIZE_TYPE_STEPWISE:
                outputWidth = frmsize.stepwise.max_width;
                outputHeight = frmsize.stepwise.max_height;
                break;
            case V4L2_FRMSIZE_TYPE_CONTINUOUS:
                continue;
                break;
            default:
                continue;
                break;
            }
            
            switch(frmsize.pixel_format) {
            case V4L2_PIX_FMT_BGR24:
              // Depth: 24, Description: BGR 24bit
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::bgr24Format, false));
              break;
#ifdef V4L2_PIX_FMT_XBGR32
            case V4L2_PIX_FMT_XBGR32:
              // Depth: 24, Description: BGR 32bit
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::xbgr32Format, false));
              break;
#endif
#ifdef V4L2_PIX_FMT_ABGR32
            case V4L2_PIX_FMT_ABGR32:
              // Depth: 24, Description: BGR 32bit
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::abgr32Format, false));
              break;
#endif
            case V4L2_PIX_FMT_YUYV:
              // Depth: 9, Description: YUV 4:1:0
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::yuv2Format, false));
              break;
            case V4L2_PIX_FMT_UYVY:
              // Depth: 16, Description: YUV 4:2:2
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::uyvyFormat, false));
              break;
#ifdef V4L2_PIX_FMT_YVU420M
            case V4L2_PIX_FMT_YVU420M:
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::yv12Format, false));
              break;
#endif
#ifdef V4L2_PIX_FMT_YUV420M
            case V4L2_PIX_FMT_YUV420M:
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::i420Format, false));
              break;
#endif
#ifdef V4L2_PIX_FMT_NV12M
            case V4L2_PIX_FMT_NV12M:
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::nv12Format, false));
              break;
#endif
            case V4L2_PIX_FMT_MJPEG:
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::mjpegFormat, false));
              break;
            default:
              device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::unknownFormat, false));
              break;
            }

            frmsize.index++;
        }

        // qDebug() << "GrabberV4L2Controller::setResolutions --> Private Control: " << (char*)(queryctrl.name);

        fmt.index++;
    }

    qDebug() << "V4L2Grabber::enumerateCaptureFormats --> End";

    return true;
}


int V4L2Grabber::xioctl(int fd, int IOCTL_X, void *arg)
{
    // qDebug() << "V4L2Grabber::xioctl --> Start";

    int ret = 0;
    int tries = IOCTL_RETRY;

    do {
        ret = v4l2_ioctl(fd, IOCTL_X, arg);
    } while (ret && tries-- &&
             ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

    if (ret && (tries <= 0)) {
        qDebug() << "V4L2Grabber::xioctl --> ioctl (" << IOCTL_X << ") retried " << IOCTL_RETRY << " times - giving up: " << strerror(errno) << ")";
    }

    // qDebug() << "V4L2Grabber::xioctl --> End";
    return (ret);
}


