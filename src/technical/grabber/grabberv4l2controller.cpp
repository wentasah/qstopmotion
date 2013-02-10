/******************************************************************************
 *  Copyright (C) 2012-2013 by                                                *
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

#include "grabberv4l2controller.h"

#include <sys/ioctl.h>
#include <libv4l2.h>
#include <fcntl.h>
#include <errno.h>

#include <QtCore/QtDebug>


/* some Logitech webcams have pan/tilt/focus controls */
// #define LENGTH_OF_XU_CTR (6)
#define LENGTH_OF_XU_MAP (9)

/* mapping for Pan/Tilt/Focus */
static struct uvc_xu_control_mapping xu_mappings[] =
{
    {
        .id        = V4L2_CID_PAN_RELATIVE,
        // .name      = (unsigned char[])("Pan (relative)"),
        .name      = {'P','a','n',' ','(','r','e','l','a','t','i','v','e',')',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_MOTOR_CONTROL,
        .selector  = XU_MOTORCONTROL_PANTILT_RELATIVE,
        .size      = 16,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
        .data_type = UVC_CTRL_DATA_TYPE_SIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_TILT_RELATIVE,
        // .name      = "Tilt (relative)",
        .name      = {'T','i','l','t',' ','(','r','e','l','a','t','i','v','e',')',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_MOTOR_CONTROL,
        .selector  = XU_MOTORCONTROL_PANTILT_RELATIVE,
        .size      = 16,
        .offset    = 16,
        .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
        .data_type = UVC_CTRL_DATA_TYPE_SIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_PAN_RESET,
        // .name      = "Pan Reset",
        .name      = {'P','a','n',' ','R','e','s','e','t',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_MOTOR_CONTROL,
        .selector  = XU_MOTORCONTROL_PANTILT_RESET,
        .size      = 1,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_BUTTON,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_TILT_RESET,
        // .name      = "Tilt Reset",
        .name      = {'T','i','l','t',' ','R','e','s','e','t',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_MOTOR_CONTROL,
        .selector  = XU_MOTORCONTROL_PANTILT_RESET,
        .size      = 1,
        .offset    = 1,
        .v4l2_type = V4L2_CTRL_TYPE_BUTTON,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_FOCUS_LOGITECH,
        // .name      = "Focus (absolute)",
        .name      = {'F','o','c','u','s',' ','(','a','b','s','o','l','u','t','e',')',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_MOTOR_CONTROL,
        .selector  = XU_MOTORCONTROL_FOCUS,
        .size      = 8,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_LED1_MODE_LOGITECH,
        // .name      = "LED1 Mode",
        .name      = {'L','E','D','1',' ','M','o','d','e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_USER_HW_CONTROL,
        .selector  = XU_HW_CONTROL_LED1,
        .size      = 8,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_MENU,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL, // led_menu_entry,
        .menu_count = 4,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_LED1_FREQUENCY_LOGITECH,
        // .name      = "LED1 Frequency",
        .name      = {'L','E','D','1',' ','F','r','e','q','u','e','n','c','y',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_USER_HW_CONTROL,
        .selector  = XU_HW_CONTROL_LED1,
        .size      = 8,
        .offset    = 16,
        .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_DISABLE_PROCESSING_LOGITECH,
        // .name      = "Disable video processing",
        .name      = {'D','i','s','a','b','l','e',' ','v','i','d','e','o',' ','p','r','o','c','e','s','s','i','n','g',0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_VIDEO_PIPE,
        .selector  = XU_COLOR_PROCESSING_DISABLE,
        .size      = 8,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_BOOLEAN,
        .data_type = UVC_CTRL_DATA_TYPE_BOOLEAN,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },
    {
        .id        = V4L2_CID_RAW_BITS_PER_PIXEL_LOGITECH,
        // .name      = "Raw bits per pixel",
        .name      = {'R','a','w',' ','b','i','t','s',' ','p','e','r',' ','p','i','x','e','l',0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .entity    = UVC_GUID_LOGITECH_VIDEO_PIPE,
        .selector  = XU_RAW_DATA_BITS_PER_PIXEL,
        .size      = 8,
        .offset    = 0,
        .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
        .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
        .menu_info = NULL,
        .menu_count = 0,
        .reserved = {0,0,0,0}
    },

};

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

GrabberV4L2Controller::GrabberV4L2Controller(int cap)
    : GrabberController(cap)
{
    qDebug("GrabberV4L2Controller::Constructor --> Start");

    fd = -1;
    errno = -1;

    // pCameraControl = NULL;
    // pQualityControl = NULL;

    qDebug("GrabberV4L2Controller::Constructor --> End");
}


GrabberV4L2Controller::~GrabberV4L2Controller()
{
    qDebug("GrabberV4L2Controller::Destructor --> Start (Empty)");

    if (fd >= 0) {
        // Close the device file
        close(fd);
        fd = -1;
    }

    qDebug("GrabberV4L2Controller::Destructor --> End");
}


bool GrabberV4L2Controller::init(const QString &id)
{
    qDebug("GrabberV4L2Controller::init --> Start");

    qDebug("GrabberV4L2Controller::init --> Open the device ...");

    fd = open(id.toAscii(), O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        qDebug() << "GrabberV4L2Controller::init --> Cannot open " << id;
        return false;
    }

    // Map dynamic device controls

    if (initDynCtrls(fd) < 0) {
        qDebug() << "GrabberV4L2Controller::init --> Cannot map dynamic device capabilities (" << errno << ")";
        return false;
    }

    // Enumerating device capabilities

    memset (&queryctrl, 0, sizeof (queryctrl));

    if (setBaseCapabilities() == false) {
        qDebug() << "GrabberV4L2Controller::init --> Cannot enumerate base device capabilities (" << errno << ")";
        return false;
    }

    if (setPrivateCapabilities() == false) {
        qDebug() << "GrabberV4L2Controller::init --> Cannot enumerate private device capabilities (" << errno << ")";
        return false;
    }

    qDebug("GrabberV4L2Controller::init --> End (Successful)");

    return true;
}


int GrabberV4L2Controller::xioctl(int fd, int IOCTL_X, void *arg)
{
    qDebug("GrabberV4L2Controller::xioctl --> Start");

    int ret = 0;
    int tries = IOCTL_RETRY;

    do {
        ret = v4l2_ioctl(fd, IOCTL_X, arg);
    } while (ret && tries-- &&
             ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

    if (ret && (tries <= 0)) {
        qDebug() << "GrabberV4L2Controller::xioctl --> ioctl (" << IOCTL_X << ") retried " << IOCTL_RETRY << " times - giving up: " << strerror(errno) << ")";
    }

    qDebug("GrabberV4L2Controller::xioctl --> End");
    return (ret);
}


int GrabberV4L2Controller::initDynCtrls(int fd)
{
    qDebug("GrabberV4L2Controller::initDynCtrls --> Start");

    int i   = 0;
    int err = 0;

    // after adding the controls, add the mapping now
    for (i=0; i<LENGTH_OF_XU_MAP; i++)
    {
        qDebug("GrabberV4L2Controller::initDynCtrls --> mapping control for %s", (char*)(xu_mappings[i].name));

        err = xioctl(fd, UVCIOC_CTRL_MAP, &xu_mappings[i]);
        if (err < 0)
        {
            switch(errno) {
            case ENOENT:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: Device do not support the given control or the specified extension unit could not be found.\n");
                break;
            case ENOBUFS:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: The buffer size is incorrect.\n");
                break;
            case EINVAL:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: Invalide request code.\n");
                break;
            case EBADRQC:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: The given request is not supported.\n");
                break;
            case EFAULT:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: The data pointer references to an iaccessible memory area.\n");
                break;
            case EACCES:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: Need admin previledges for adding extension controls");
                qDebug("GrabberV4L2Controller::initDynCtrls --> Error: Please run 'guvcview --add_ctrls' as root (or with sudo)");

                qDebug("GrabberV4L2Controller::initDynCtrls --> Error End");
                return -1;
            case EEXIST:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Mapping exists");
                break;
            default:
                qDebug("GrabberV4L2Controller::initDynCtrls --> Unknown error");
                break;
            }
        }
    }

    qDebug("GrabberV4L2Controller::initDynCtrls --> End");
    return 0;
}


void GrabberV4L2Controller::enumerate_menu ()
{
    qDebug("GrabberV4L2Controller::enumerate_menu --> Start");

    qDebug("GrabberV4L2Controller::enumerate_menu --> Menu items:");

    memset (&querymenu, 0, sizeof (querymenu));
    querymenu.id = queryctrl.id;

    for (querymenu.index = queryctrl.minimum; querymenu.index <= queryctrl.maximum; querymenu.index++) {
        if (0 == ioctl (fd, VIDIOC_QUERYMENU, &querymenu)) {
            qDebug() << "GrabberV4L2Controller::enumerate_menu --> Item: " << (char*)(querymenu.name);
        }
    }

    qDebug("GrabberV4L2Controller::enumerate_menu --> End");
}


void GrabberV4L2Controller::getControlData(GrabberControlCapabilities *caps)
{
    switch (queryctrl.type) {
    case V4L2_CTRL_TYPE_MENU:
        enumerate_menu ();
        break;
    case V4L2_CTRL_TYPE_INTEGER:
        caps->setMinimum(queryctrl.minimum);
        caps->setMaximum(queryctrl.maximum);
        caps->setStep(queryctrl.step);
        caps->setDefault(queryctrl.default_value);
        caps->setFlags(GrabberControlCapabilities::control_Manual);

        break;
    case V4L2_CTRL_TYPE_INTEGER_MENU:
    case V4L2_CTRL_TYPE_INTEGER64:
    case V4L2_CTRL_TYPE_BOOLEAN:
    case V4L2_CTRL_TYPE_BUTTON:
    case V4L2_CTRL_TYPE_CTRL_CLASS:
    case V4L2_CTRL_TYPE_STRING:
    case V4L2_CTRL_TYPE_BITMASK:
    default:
        qDebug() << "GrabberV4L2Controller::setCameraCapabilities --> Not supported control type";
        break;
    }
}


void GrabberV4L2Controller::getControlFlag(GrabberControlCapabilities *caps, int id)
{
    
}


bool GrabberV4L2Controller::setPrivateCapabilities()
{
    qDebug("GrabberV4L2Controller::setPrivateCapabilities --> Start");

    for (queryctrl.id = V4L2_CID_PRIVATE_BASE; ; queryctrl.id++) {
        if (0 == v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                continue;
            }

            qDebug() << "GrabberV4L2Controller::setPrivateCapabilities --> Private Control: " << (char*)(queryctrl.name);
/*
            switch (queryctrl.id) {
            case V4L2_CID_ZOOM:
                getControlData(getZoomCaps());
                break;
            case V4L2_CID_FOCUS:
                getControlData(getFocusCaps());
                break;
            case V4L2_CID_PAN:
                getControlData(getPanCaps());
                break;
            case V4L2_CID_TILT:
                getControlData(getTiltCaps());
                break;
            case V4L2_CID_ITIS:
                getControlData(getIrisCaps());
                break;
            case V4L2_CID_ROLL:
                getControlData(getRollCaps());
                break;
            default:
                qDebug() << "GrabberV4L2Controller::setPrivateCapabilities --> Not Supported Private Control: " << (char*)(queryctrl.name);
            }
*/
            if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
                enumerate_menu ();
            }
        }
        else {
            if (errno == EINVAL) {
                break;
            }

            perror ("VIDIOC_QUERYCTRL");
            exit (EXIT_FAILURE);
        }
    }

    qDebug("GrabberV4L2Controller::setPrivateCapabilities --> End");

    return true;
}


bool GrabberV4L2Controller::setBaseCapabilities()
{
    qDebug("GrabberV4L2Controller::setBaseCapabilities --> Start");

    for (queryctrl.id = V4L2_CID_BASE; queryctrl.id < V4L2_CID_LASTP1; queryctrl.id++) {
        if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                continue;
            }

            qDebug() << "GrabberV4L2Controller::setBaseCapabilities --> Base Control: " << (char*)(queryctrl.name);

            switch (queryctrl.id) {
            case V4L2_CID_BRIGHTNESS:
            	getControlData(getBrightnessCaps());
                break;
            case V4L2_CID_AUTOBRIGHTNESS:
            	getControlFlag(getBrightnessCaps(), V4L2_CID_AUTOBRIGHTNESS);
                break;
            case V4L2_CID_CONTRAST:
            	getControlData(getContrastCaps());
                break;
            case V4L2_CID_SATURATION:
            	getControlData(getSaturationCaps());
                break;
            case V4L2_CID_HUE:
            	getControlData(getHueCaps());
                break;
            case V4L2_CID_HUE_AUTO:
            	getControlFlag(getHueCaps(), V4L2_CID_HUE_AUTO);
                break;
            case V4L2_CID_GAMMA:
            	getControlData(getGammaCaps());
                break;
            case V4L2_CID_SHARPNESS:
            	getControlData(getSharpnessCaps());
                break;
            case V4L2_CID_BACKLIGHT_COMPENSATION:
            	getControlData(getBacklightCaps());
                break;
            case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
            	getControlData(getWhiteCaps());
                break;
            case V4L2_CID_AUTO_WHITE_BALANCE:
            	getControlFlag(getWhiteCaps(), V4L2_CID_AUTO_WHITE_BALANCE);
                break;
            case V4L2_CID_GAIN:
            	getControlData(getGainCaps());
                break;
            case V4L2_CID_AUTOGAIN:
            	getControlFlag(getGainCaps(), V4L2_CID_AUTOGAIN);
                break;
            case V4L2_CID_COLOR_KILLER:
            	getControlData(getColorCaps());
                break;
            case V4L2_CID_EXPOSURE:
                getControlData(getExposureCaps());
                break;
            default:
                qDebug() << "GrabberV4L2Controller::setBaseCapabilities --> Not Supported Base Control: " << (char*)(queryctrl.name);
            }

        }
        else {
            if (errno == EINVAL) {
                continue;
            }

            perror ("VIDIOC_QUERYCTRL");
            exit (EXIT_FAILURE);
        }
    }

    qDebug("GrabberV4L2Controller::setBaseCapabilities --> End");

    return true;
}


/**************************************************************************
 **************************************************************************
 * Camera quality capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/

bool GrabberV4L2Controller::getAutomaticBrightness()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticBrightness(bool ab)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getBrightness()
{
    return 0;
}


void GrabberV4L2Controller::setBrightness(int b)
{
    qDebug("GrabberV4L2Controller::setBrightness --> Start");

    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;

    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;

    if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End (VIDIOC_QUERYCTRL)");
            return;
        }
        else {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End - V4L2_CID_BRIGHTNESS is not supported");
        }
    }
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug("GrabberV4L2Controller::setBrightness --> Error End - V4L2_CID_BRIGHTNESS is disabled");
    }
    else {
        memset (&control, 0, sizeof (control));
        control.id = V4L2_CID_BRIGHTNESS;
        control.value = b;

        if (-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End (VIDIOC_S_CTRL)");
            return;
        }
    }

    // Errors ignored
    ioctl (fd, VIDIOC_S_CTRL, &control);

    qDebug("GrabberV4L2Controller::setBrightness --> End");
}

/**************************************************************************
 * Contrast
 **************************************************************************/

bool GrabberV4L2Controller::getAutomaticContrast()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticContrast(bool ac)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getContrast()
{
    return 0;
}


void GrabberV4L2Controller::setContrast(int c)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;

    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_CONTRAST;

    if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End (VIDIOC_QUERYCTRL)");
            return;
        }
        else {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End - V4L2_CID_CONTRAST is not supported");
        }
    }
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug("GrabberV4L2Controller::setBrightness --> Error End - V4L2_CID_CONTRAST is disabled");
    }
    else {
        memset (&control, 0, sizeof (control));
        control.id = V4L2_CID_CONTRAST;
        control.value = c;

        if (-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qDebug("GrabberV4L2Controller::setBrightness --> Error End (VIDIOC_S_CTRL)");
            return;
        }
    }

    // Errors ignored
    ioctl (fd, VIDIOC_S_CTRL, &control);
}

/**************************************************************************
 * Saturation
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticSaturation()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticSaturation(bool as)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getSaturation()
{
    return 0;
}


void GrabberV4L2Controller::setSaturation(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Hue
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticHue()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticHue(bool ah)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getHue()
{
    return 0;
}


void GrabberV4L2Controller::setHue(int h)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gamma
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticGamma()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticGamma(bool ag)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getGamma()
{
    return 0;
}


void GrabberV4L2Controller::setGamma(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticSharpness()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticSharpness(bool as)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getSharpness()
{
    return 0;
}


void GrabberV4L2Controller::setSharpness(int s)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticBacklight()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticBacklight(bool ab)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getBacklight()
{
    return 0;
}


void GrabberV4L2Controller::setBacklight(int b)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticWhite()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getWhite()
{
    return 0;
}


void GrabberV4L2Controller::setWhite(int w)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Gain
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticGain()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticGain(bool ag)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getGain()
{
    return 0;
}


void GrabberV4L2Controller::setGain(int g)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Color Enable
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticColor()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticColor(bool ac)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getColor()
{
    return 0;
}


void GrabberV4L2Controller::setColor(int c)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticExposure()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isExposure()
{
    return false;
}


void GrabberV4L2Controller::setExposure(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Zoom
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticZoom()
{
    return false;

    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
        return false;
    }

    if (flags & KSPROPERTY_CAMERACONTROL_FLAGS_AUTO)
    {
        return true;
    }

    return false;
}


void GrabberV4L2Controller::setAutomaticZoom(bool az)
{
    HRESULT hr = 0;
    long flags;
    long z = 0;

    if (az) {
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
    }
    else {
        z = getZoomCaps()->getDefault();
        flags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    }

    hr = pCameraControl->Set(CameraControl_Zoom, z, flags);
    if (hr != S_OK)
    {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr;
    }
}

int GrabberV4L2Controller::getZoom()
{
    HRESULT hr = 0;
    long flags; // = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
    long z;

    hr = pCameraControl->Get(CameraControl_Zoom, &z, &flags);
    if (hr != S_OK) {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to get Zoom property value. (Error 0x" << hr;
        return -1;
    }
    return (int)z;
}


void GrabberV4L2Controller::setZoom(int z)
{
    HRESULT hr = 0;
    long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

    hr = pCameraControl->Set(CameraControl_Zoom, (long)z, flags);
    if (hr != S_OK) {
        qDebug() << "GrabberV4L2Controller::Constructor --> ERROR: Unable to set Zoom property value to " << z << ". (Error 0x" << hr;
    }
}
*/
/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticFocus()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}

int GrabberV4L2Controller::getFocus()
{
    return 0;
}


void GrabberV4L2Controller::setFocus(int f)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Pan
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticPan()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticPan(bool ap)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getPan()
{
    return 0;
}


void GrabberV4L2Controller::setPan(int p)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Tilt
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticTilt()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticTilt(bool at)
{
    Q_ASSERT( 1 );
}


int GrabberV4L2Controller::getTilt()
{
    return 0;
}


void GrabberV4L2Controller::setTilt(int t)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Iris
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticIris()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticIris(bool ae)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isIris()
{
    return false;
}


void GrabberV4L2Controller::setIris(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Roll
 **************************************************************************/
/*
bool GrabberV4L2Controller::getAutomaticRoll()
{
    return false;
}


void GrabberV4L2Controller::setAutomaticRoll(bool ar)
{
    Q_ASSERT( 1 );
}


bool GrabberV4L2Controller::isRoll()
{
    return false;
}


void GrabberV4L2Controller::setRoll(int e)
{
    Q_ASSERT( 1 );
}
*/
