/******************************************************************************
 *  Copyright (C) 2012-2015 by                                                *
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
 ******************************************************************************
 *  Base on the following sources                                             *
 ******************************************************************************
 *           guvcview              http://guvcview.sourceforge.net            *
 *                                                                            *
 *           Paulo Assis <pj.assis@gmail.com>                                 *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 2 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
 *                                                                            *
 ******************************************************************************/

#include "technical/grabber/imagegrabberdevice.h"

#include "v4l2controller.h"

#include <QDebug>

#include <libv4l2.h>
#include <fcntl.h>
#include <errno.h>

/**************************************************************************
 * Default implementation of the grabber controller functions.
 **************************************************************************/

V4L2Controller::V4L2Controller(int cap)
    : GrabberController(cap)
{
    qDebug() << "V4L2Controller::Constructor --> Start";

    // errno = -1;

    grabber = NULL;
    grabberDevice = NULL;

    qDebug() << "V4L2Controller::Constructor --> End";
}


V4L2Controller::~V4L2Controller()
{
    qDebug() << "V4L2Controller::Destructor --> Start (Empty)";

    // qDebug() << "V4L2Controller::Destructor --> End";
}


ImageGrabber* V4L2Controller::getImageGrabber()
{
    return grabber;
}


bool V4L2Controller::initialization(ImageGrabber* ig, ImageGrabberDevice* igd)
{
    qDebug() << "V4L2Controller::initialization --> Start";

    grabber = (V4L2Grabber*)ig;
    grabberDevice = igd;

    qDebug() << "V4L2Controller::initialization --> Open the device ...";

    // fd = v4l2_open(grabberDevice->getDeviceId().toAscii(), O_RDWR | O_NONBLOCK, 0);
    fd = grabber->getFd();
    if (fd < 0)
    {
        qDebug() << "V4L2Controller::initialization --> No open grabber!";
        v4l2_close(fd);
        return false;
    }

    // fd = open(grabberDevice->getDeviceId().toAscii(), O_RDWR | O_NONBLOCK);
    // if (fd < 0) {
    //     qDebug() << "V4L2Controller::initialization --> Error opening standard interface for " << grabberDevice->getDeviceId();
    //     close(fd);
    //     return false;
    // }

    // Enumerating device capabilities

    memset (&queryctrl, 0, sizeof (queryctrl));

    if (setBaseCapabilities() == false) {
        qDebug() << "V4L2Controller::initialization --> Cannot enumerate base device capabilities (" << errno << ")";
        return false;
    }

    if (setPrivateCapabilities() == false) {
        qDebug() << "V4L2Controller::initialization --> Cannot enumerate private device capabilities (" << errno << ")";
        return false;
    }

    qDebug() << "V4L2Controller::initialization --> End (Successful)";

    return true;
}


bool V4L2Controller::setUp()
{
    qDebug() << "V4L2Controller::setUp --> Start (Empty)";

    return true;
}


bool V4L2Controller::tearDown()
{
    qDebug() << "V4L2Controller::tearDown --> Start (Empty)";

    return true;
}


/*
 * don't use xioctl for control query when using V4L2_CTRL_FLAG_NEXT_CTRL
 */
int V4L2Controller::query_ioctl(int hdevice, int current_ctrl, struct v4l2_queryctrl *ctrl)
{
    int ret = 0;
    int tries = 4;
    do {
        if(ret)
            ctrl->id = current_ctrl | V4L2_CTRL_FLAG_NEXT_CTRL;
        ret = v4l2_ioctl(hdevice, VIDIOC_QUERYCTRL, ctrl);
    } while (ret && tries-- &&
             ((errno == EIO || errno == EPIPE || errno == ETIMEDOUT)));

    return(ret);
}


int V4L2Controller::xioctl(int fd, int IOCTL_X, void *arg)
{
    // qDebug() << "V4L2Controller::xioctl --> Start");

    int ret = 0;
    int tries = IOCTL_RETRY;

    do {
        ret = v4l2_ioctl(fd, IOCTL_X, arg);
    } while (ret && tries-- &&
             ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

    if (ret && (tries <= 0)) {
        qDebug() << "V4L2Controller::xioctl --> v4l2_ioctl (" << IOCTL_X << ") retried " << IOCTL_RETRY << " times - giving up: " << strerror(errno) << ")";
    }

    // qDebug() << "V4L2Controller::xioctl --> End");
    return (ret);
}


void V4L2Controller::enumerate_menu ()
{
    qDebug() << "V4L2Controller::enumerate_menu --> Start";

    qDebug() << "V4L2Controller::enumerate_menu --> Menu items:";

    memset (&querymenu, 0, sizeof (querymenu));
    querymenu.id = queryctrl.id;

    for (querymenu.index = queryctrl.minimum; querymenu.index <= queryctrl.maximum; querymenu.index++) {
        if (0 == v4l2_ioctl (fd, VIDIOC_QUERYMENU, &querymenu)) {
            qDebug() << "V4L2Controller::enumerate_menu --> Item: " << (char*)(querymenu.name);
        }
    }

    qDebug() << "V4L2Controller::enumerate_menu --> End";
}


void V4L2Controller::getControlData(GrabberControlCapabilities *caps, long flags)
{
    caps->setControlId(queryctrl.id);
    caps->setControlType(queryctrl.type);
    caps->setFlags(flags);

    switch (queryctrl.type) {
    case V4L2_CTRL_TYPE_MENU:
        // enumerate_menu ();
        break;
    case V4L2_CTRL_TYPE_INTEGER:
        caps->setMinimum(queryctrl.minimum);
        caps->setMaximum(queryctrl.maximum);
        caps->setStep(queryctrl.step);
        caps->setDefault(queryctrl.default_value);
        caps->setFlags(GrabberControlCapabilities::control_Manual);
        break;
    // case V4L2_CTRL_TYPE_INTEGER_MENU:   // Not defined on Debian systems
    case V4L2_CTRL_TYPE_INTEGER64:
    case V4L2_CTRL_TYPE_BOOLEAN:
    case V4L2_CTRL_TYPE_BUTTON:
    case V4L2_CTRL_TYPE_CTRL_CLASS:
    case V4L2_CTRL_TYPE_STRING:
    // case V4L2_CTRL_TYPE_BITMASK:   // Not defined on Debian systems
    default:
        qDebug() << "V4L2Controller::setCameraCapabilities --> Not supported control type";
        break;
    }
}


void V4L2Controller::getControlFlag(GrabberControlCapabilities *caps)
{
    int                 ret = 0;
    struct v4l2_control ctrl;

    ctrl.id = queryctrl.id;
    ctrl.value = 0;
    ret = v4l2_ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if (ret != 0) {
        return;
    }
    if (ctrl.value == 1) {
        caps->setFlags(GrabberControlCapabilities::control_Auto);
    }
    else {
        caps->setFlags(GrabberControlCapabilities::control_Manual);
    }
    return;
}


bool V4L2Controller::setBaseCapabilities()
{
    qDebug() << "V4L2Controller::setBaseCapabilities --> Start";

    int                         ret = 0;
    int                         currentctrl = 0;
    GrabberControlCapabilities *caps = NULL;
    int                         flags = 0;

    queryctrl.id = 0 | V4L2_CTRL_FLAG_NEXT_CTRL;
    ret = query_ioctl(fd, currentctrl, &queryctrl);
    if (ret == 0) {
        // The driver supports the V4L2_CTRL_FLAG_NEXT_CTRL flag

        qDebug() << "V4L2Controller::setBaseCapabilities --> Start V4L2_CTRL_FLAG_NEXT_CTRL";

        queryctrl.id = 0 | V4L2_CTRL_FLAG_NEXT_CTRL;
        currentctrl = 0;

        while ((ret = query_ioctl(fd, currentctrl, &queryctrl)), ret ? errno != EINVAL : 1) {

            // Prevent infinite loop for buggy V4L2_CTRL_FLAG_NEXT_CTRL implementations
            if(ret && queryctrl.id <= currentctrl) {
                qDebug() << "buggy V4L2_CTRL_FLAG_NEXT_CTRL flag implementation (workaround enabled)";
                // increment the control id manually
                currentctrl++;
                queryctrl.id = currentctrl;
                goto next_control;
            }
            else if ((queryctrl.id == V4L2_CTRL_FLAG_NEXT_CTRL) || (!ret && queryctrl.id == currentctrl)) {
                qDebug() << "buggy V4L2_CTRL_FLAG_NEXT_CTRL flag implementation (failed enumeration for id=" << queryctrl.id;
                // stop control enumeration
                break;
            }

            currentctrl = queryctrl.id;

            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                qDebug() << "V4L2Controller::setBaseCapabilities --> Disabling control " << queryctrl.id;
                goto next_control;
            }

            qDebug() << "V4L2Controller::setBaseCapabilities --> Base Control: " << (char*)(queryctrl.name);

            if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
                // This is a menu control
                enumerate_menu ();
            }

            caps = NULL;
            flags = GrabberControlCapabilities::control_none;

            switch (queryctrl.id) {
            case V4L2_CID_BRIGHTNESS:
                caps = getBrightnessCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_AUTOBRIGHTNESS:
                getControlFlag(getBrightnessCaps());
                break;
            case V4L2_CID_CONTRAST:
                caps = getContrastCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_SATURATION:
                caps = getSaturationCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_HUE:
                caps = getHueCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_HUE_AUTO:
                getControlFlag(getHueCaps());
                break;
            case V4L2_CID_GAMMA:
                caps = getGammaCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_SHARPNESS:
                caps = getSharpnessCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_BACKLIGHT_COMPENSATION:
                caps = getBacklightCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
                caps = getWhiteCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_AUTO_WHITE_BALANCE:
                getControlFlag(getWhiteCaps());
                break;
            case V4L2_CID_GAIN:
                caps = getGainCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_AUTOGAIN:
                getControlFlag(getGainCaps());
                break;
            case V4L2_CID_COLOR_KILLER:
                caps = getColorCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_EXPOSURE:
                caps = getExposureCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_EXPOSURE_ABSOLUTE:         // "Exposure (Absolute)"
                caps = getExposureCaps();
                flags = GrabberController::controller_Absolute;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_EXPOSURE_AUTO:             // "Exposure, Auto"
                getControlFlag(getExposureCaps());
                break;
            case V4L2_CID_ZOOM_ABSOLUTE:             // "Zool, Absolute"
                caps = getZoomCaps();
                flags = GrabberController::controller_Absolute;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_ZOOM_RELATIVE:             // "Zool, Relative"
                caps = getZoomCaps();
                flags = GrabberController::controller_Relative;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_ZOOM_CONTINUOUS:           // "Zool, Continuous"
                caps = getZoomCaps();
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_FOCUS_ABSOLUTE:            // "Focus (absolute)"
            // case V4L2_CID_FOCUS_LOGITECH:            // "Focus (absolute)"
                caps = getFocusCaps();
                flags = GrabberController::controller_Absolute;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_FOCUS_RELATIVE:            // "Focus (relative)"
                caps = getFocusCaps();
                flags = GrabberController::controller_Relative;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_FOCUS_AUTO:                // "Focus, Auto"
                getControlFlag(getExposureCaps());
                break;
            case V4L2_CID_PAN_ABSOLUTE:
                caps = getPanCaps();
                flags = GrabberController::controller_Absolute;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_PAN_RELATIVE:
                caps = getPanCaps();
                flags = GrabberController::controller_Relative;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_TILT_ABSOLUTE:
                caps = getTiltCaps();
                flags = GrabberController::controller_Absolute;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            case V4L2_CID_TILT_RELATIVE:
                caps = getTiltCaps();
                flags = GrabberController::controller_Relative;
                getControlData(caps, flags);
                caps->setControlClass(V4L2_CTRL_ID2CLASS(caps->getControlId()));
                break;
            default:
                qDebug() << "V4L2Controller::setBaseCapabilities --> Not Supported Base Control: " << (char*)(queryctrl.name) << " (" << queryctrl.id << ")" << (queryctrl.id & 0xFFFF0000);
            }


next_control:
            queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;

        }

        qDebug() << "V4L2Controller::setBaseCapabilities --> End V4L2_CTRL_FLAG_NEXT_CTRL";

        return true;
    }

    // The driver not supports the V4L2_CTRL_FLAG_NEXT_CTRL flag

    qDebug() << "V4L2Controller::setBaseCapabilities --> Start V4L2_CID_BASE";

    for (currentctrl = V4L2_CID_BASE; currentctrl < V4L2_CID_LASTP1; currentctrl++) {
        queryctrl.id = currentctrl;
        ret = v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl);

        if (ret) {
            continue;
        }
        if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
            continue;
        }

        qDebug() << "V4L2Controller::setBaseCapabilities --> Base Control: " << (char*)(queryctrl.name);

        if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
            // This is a menu control
            enumerate_menu ();
        }

        caps = NULL;
        flags = GrabberControlCapabilities::control_none;

        switch (queryctrl.id) {
        case V4L2_CID_BRIGHTNESS:
            caps = getBrightnessCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_AUTOBRIGHTNESS:
            getControlFlag(getBrightnessCaps());
            break;
        case V4L2_CID_CONTRAST:
            caps = getContrastCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_SATURATION:
            caps = getSaturationCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_HUE:
            caps = getHueCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_HUE_AUTO:
            getControlFlag(getHueCaps());
            break;
        case V4L2_CID_GAMMA:
            caps = getGammaCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_SHARPNESS:
            caps = getSharpnessCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_BACKLIGHT_COMPENSATION:
            caps = getBacklightCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
            caps = getWhiteCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_AUTO_WHITE_BALANCE:
            getControlFlag(getWhiteCaps());
            break;
        case V4L2_CID_GAIN:
            caps = getGainCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_AUTOGAIN:
            getControlFlag(getGainCaps());
            break;
        case V4L2_CID_COLOR_KILLER:
            caps = getColorCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_EXPOSURE:                  // "Exposure (Absolute)"
            caps = getExposureCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        default:
            qDebug() << "V4L2Controller::setBaseCapabilities --> Not Supported Base Control: " << (char*)(queryctrl.name) << " (" << queryctrl.id << ")" << (queryctrl.id & 0xFFFF0000);
        }

    }

    qDebug() << "V4L2Controller::setBaseCapabilities --> End V4L2_CID_BASE";

    qDebug() << "V4L2Controller::setBaseCapabilities --> End";

    return true;
}


bool V4L2Controller::setPrivateCapabilities()
{
    qDebug() << "V4L2Controller::setPrivateCapabilities --> Start";

    int ret = 0;

    for (queryctrl.id = V4L2_CID_PRIVATE_BASE; ; queryctrl.id++) {
        ret = v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl);
        if(ret) {
            break;
        }
        if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
            continue;
        }

        qDebug() << "V4L2Controller::setPrivateCapabilities --> Private Control: " << (char*)(queryctrl.name);
/*
        caps = NULL;
        flags = GrabberControlCapabilities::control_none;

        switch (queryctrl.id) {
        case V4L2_CID_ZOOM:
            caps = getZoomCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_FOCUS:
            caps = getFocusCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_PAN:
            caps = getPanCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_TILT:
            caps = getTiltCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_ITIS:
            caps = getIrisCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        case V4L2_CID_ROLL:
            caps = getRollCaps();
            getControlData(caps, flags);
            caps->setControlClass(V4L2_CTRL_CLASS_USER);
            break;
        default:
            qDebug() << "V4L2Controller::setPrivateCapabilities --> Not Supported Private Control: " << (char*)(queryctrl.name);
        }
*/
        if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
            enumerate_menu ();
        }

    }

    qDebug() << "V4L2Controller::setPrivateCapabilities --> End";

    return true;
}

/*
 * sets the value for control id
 */
int V4L2Controller::setCtrlValue(GrabberControlCapabilities *caps, int value)
{
    qDebug() << "V4L2Controller::setCtrlValue --> Start";

    int ret = 0;

    if (caps->getControlClass() == V4L2_CTRL_CLASS_USER) {
        // using VIDIOC_G_CTRL for user class controls

        struct v4l2_control ctrl;

        ctrl.id = caps->getControlId();
        ctrl.value = value;
        ret = xioctl(fd, VIDIOC_S_CTRL, &ctrl);
    }
    else {
        // using VIDIOC_G_EXT_CTRLS on single controls

        struct v4l2_ext_controls ctrls = {0};
        struct v4l2_ext_control ctrl = {0};
        ctrl.id = caps->getControlId();
        switch (caps->getControlType())
        {
            // case V4L2_CTRL_TYPE_STRING:
            //     ctrl.size = control->value;
            //     ctrl.string = control->string;
            //     break;
            case V4L2_CTRL_TYPE_INTEGER64:
                ctrl.value64 = (long)value;
                break;
            default:
                ctrl.value = value;
                break;
        }
        ctrls.ctrl_class = caps->getControlClass();
        ctrls.count = 1;
        ctrls.controls = &ctrl;
        ret = xioctl(fd, VIDIOC_S_EXT_CTRLS, &ctrls);
        if (ret) {
            qDebug() << "V4L2Controller::setCtrlValue --> control id: " << ctrl.id << " failed to set (error " << ret << ")";
        }
    }

    // update real value
    // get_ctrl(hdevice, control_list, id, NULL);

    qDebug() << "V4L2Controller::setCtrlValue --> End";
    return (ret);
}

/*
 * Gets the value for control id
 * and updates control flags and widgets
 */
int V4L2Controller::getCtrlValue(GrabberControlCapabilities *caps, int &value)
{
    qDebug() << "V4L2Controller::setCtrlValue --> Start";

    int ret = 0;

    if (caps->getControlClass() == V4L2_CTRL_CLASS_USER) {
        // using VIDIOC_G_CTRL for user class controls

        struct v4l2_control ctrl;
        ctrl.id = caps->getControlId();
        ctrl.value = 0;
        ret = v4l2_ioctl(fd, VIDIOC_G_CTRL, &ctrl);
        if (ret) {
            qDebug() << "V4L2Controller::setCtrlValue --> control id: " << ctrl.id << " failed to get value (error " << ret << ")";
        }
        else {
            value = ctrl.value;
        }
    }
    else
    {
        // using VIDIOC_G_EXT_CTRLS on single controls

        struct v4l2_ext_controls ctrls = {0};
        struct v4l2_ext_control ctrl = {0};
        ctrl.id = caps->getControlId();
        // ctrl.size = 0;
        // if(control->control.type == V4L2_CTRL_TYPE_STRING)
        // {
        //     ctrl.size = control->control.maximum + 1;
        //     ctrl.string = control->string;
        // }
        ctrls.ctrl_class = caps->getControlClass();
        ctrls.count = 1;
        ctrls.controls = &ctrl;
        ret = v4l2_ioctl(fd, VIDIOC_G_EXT_CTRLS, &ctrls);
        if (ret) {
            qDebug() << "V4L2Controller::setCtrlValue --> control id: " << ctrl.id << " failed to get value (error " << ret << ")";
        }
        else {
            switch(caps->getControlType()) {
                // case V4L2_CTRL_TYPE_STRING:
                //     //string gets set on VIDIOC_G_EXT_CTRLS
                //     //add the maximum size to value
                //     control->value = ctrl.size;
                //     break;
                case V4L2_CTRL_TYPE_INTEGER64:
                    value = (int)ctrl.value64;
                    break;
                default:
                    value = ctrl.value;
                    // printf("control %i [0x%08x] = %i\n", i, clist[i].id, clist[i].value);
                    break;
            }
        }
    }

    // update_ctrl_flags(control_list, id);
    // update_widget_state(control_list, all_data);

    qDebug() << "V4L2Controller::setCtrlValue --> End";
    return (ret);
}

/**************************************************************************
 **************************************************************************
 * Camera quality capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Brightness
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticBrightness()
{
    return false;
}


void V4L2Controller::setAutomaticBrightness(bool ab)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getBrightness()
{
    return 0;
}


void V4L2Controller::setBrightness(int b)
{
    qDebug() << "V4L2Controller::setBrightness --> Start";

    setCtrlValue(getBrightnessCaps(), b);

    qDebug() << "V4L2Controller::setBrightness --> End";
}

/**************************************************************************
 * Contrast
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticContrast()
{
    return false;
}


void V4L2Controller::setAutomaticContrast(bool ac)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getContrast()
{
    return 0;
}


void V4L2Controller::setContrast(int c)
{
    qDebug() << "V4L2Controller::setContrast --> Start";

    setCtrlValue(getContrastCaps(), c);

    qDebug() << "V4L2Controller::setContrast --> End";
}

/**************************************************************************
 * Saturation
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticSaturation()
{
    return false;
}


void V4L2Controller::setAutomaticSaturation(bool as)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getSaturation()
{
    return 0;
}


void V4L2Controller::setSaturation(int s)
{
    qDebug() << "V4L2Controller::setSaturation --> Start";

    setCtrlValue(getSaturationCaps(), s);

    qDebug() << "V4L2Controller::setSaturation --> End";
}

/**************************************************************************
 * Hue
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticHue()
{
    return false;
}


void V4L2Controller::setAutomaticHue(bool ah)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getHue()
{
    return 0;
}


void V4L2Controller::setHue(int h)
{
    qDebug() << "V4L2Controller::setHue --> Start";

    setCtrlValue(getHueCaps(), h);

    qDebug() << "V4L2Controller::setHue --> End";
}

/**************************************************************************
 * Gamma
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticGamma()
{
    return false;
}


void V4L2Controller::setAutomaticGamma(bool ag)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getGamma()
{
    return 0;
}


void V4L2Controller::setGamma(int g)
{
    qDebug() << "V4L2Controller::setGamma --> Start";

    setCtrlValue(getGammaCaps(), g);

    qDebug() << "V4L2Controller::setGamma --> End";
}

/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticSharpness()
{
    return false;
}


void V4L2Controller::setAutomaticSharpness(bool as)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getSharpness()
{
    return 0;
}


void V4L2Controller::setSharpness(int s)
{
    qDebug() << "V4L2Controller::setSharpness --> Start";

    setCtrlValue(getSharpnessCaps(), s);

    qDebug() << "V4L2Controller::setSharpness --> End";
}

/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticBacklight()
{
    return false;
}


void V4L2Controller::setAutomaticBacklight(bool ab)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getBacklight()
{
    return 0;
}


void V4L2Controller::setBacklight(int b)
{
    qDebug() << "V4L2Controller::setBacklight --> Start";

    setCtrlValue(getBacklightCaps(), b);

    qDebug() << "V4L2Controller::setBacklight --> End";
}

/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticWhite()
{
    return false;
}


void V4L2Controller::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getWhite()
{
    return 0;
}


void V4L2Controller::setWhite(int w)
{
    qDebug() << "V4L2Controller::setWhite --> Start";

    setCtrlValue(getWhiteCaps(), w);

    qDebug() << "V4L2Controller::setWhite --> End";
}

/**************************************************************************
 * Gain
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticGain()
{
    return false;
}


void V4L2Controller::setAutomaticGain(bool ag)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getGain()
{
    return 0;
}


void V4L2Controller::setGain(int g)
{
    qDebug() << "V4L2Controller::setGain --> Start";

    setCtrlValue(getGainCaps(), g);

    qDebug() << "V4L2Controller::setGain --> End";
}

/**************************************************************************
 * Color Enable
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticColor()
{
    return false;
}


void V4L2Controller::setAutomaticColor(bool ac)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getColor()
{
    return 0;
}


void V4L2Controller::setColor(int c)
{
    qDebug() << "V4L2Controller::setColor --> Start";

    setCtrlValue(getColorCaps(), c);

    qDebug() << "V4L2Controller::setColor --> End";
}

/**************************************************************************
 **************************************************************************
 * Camera control capabilities
 **************************************************************************
 **************************************************************************/

/**************************************************************************
 * Exposure
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticExposure()
{
    return false;
}


void V4L2Controller::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getExposure()
{
    return 0;
}


void V4L2Controller::setExposure(int e)
{
    qDebug() << "V4L2Controller::setExposure --> Start";

    setCtrlValue(getExposureCaps(), e);

    qDebug() << "V4L2Controller::setExposure --> End";
}

/**************************************************************************
 * Zoom
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticZoom()
{
    return false;
}


void V4L2Controller::setAutomaticZoom(bool az)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getZoom()
{
    int value = 0;

    getCtrlValue(getZoomCaps(), value);

    return value;
}


void V4L2Controller::setZoom(int z)
{
    qDebug() << "V4L2Controller::setZoom --> Start";

    setCtrlValue(getZoomCaps(), z);

    setCtrlValue(getZoomCaps(), z);

    qDebug() << "V4L2Controller::setZoom --> End";
}

/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticFocus()
{
    return false;
}


void V4L2Controller::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}
*/

int V4L2Controller::getFocus()
{
    return 0;
}


void V4L2Controller::setFocus(int f)
{
    qDebug() << "V4L2Controller::setFocus --> Start";

    setCtrlValue(getFocusCaps(), f);

    qDebug() << "V4L2Controller::setFocus --> End";
}

/**************************************************************************
 * Pan
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticPan()
{
    return false;
}


void V4L2Controller::setAutomaticPan(bool ap)
{
    Q_ASSERT( 1 );
}


int V4L2Controller::getPan()
{
    return 0;
}


void V4L2Controller::setPan(int p)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Tilt
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticTilt()
{
    return false;
}


void V4L2Controller::setAutomaticTilt(bool at)
{
    Q_ASSERT( 1 );
}


int V4L2Controller::getTilt()
{
    return 0;
}


void V4L2Controller::setTilt(int t)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Iris
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticIris()
{
    return false;
}


void V4L2Controller::setAutomaticIris(bool ae)
{
    Q_ASSERT( 1 );
}


bool V4L2Controller::isIris()
{
    return false;
}


void V4L2Controller::setIris(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Roll
 **************************************************************************/
/*
bool V4L2Controller::getAutomaticRoll()
{
    return false;
}


void V4L2Controller::setAutomaticRoll(bool ar)
{
    Q_ASSERT( 1 );
}


bool V4L2Controller::isRoll()
{
    return false;
}


void V4L2Controller::setRoll(int e)
{
    Q_ASSERT( 1 );
}
*/
