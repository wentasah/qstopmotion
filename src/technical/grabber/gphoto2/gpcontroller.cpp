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
 ******************************************************************************/

#include "gpcontroller.h"

#include <QDebug>

/**************************************************************************
 * Implementation of the grabber controller functions using the
 * gphoto2 library.
 **************************************************************************/

GphotoController::GphotoController(int cap)
    : GrabberController(cap)
{
    qDebug() << "GphotoController::Constructor --> Start";

    grabber = NULL;
    grabberDevice = NULL;

    qDebug() << "GphotoController::Constructor --> End";
}


GphotoController::~GphotoController()
{
    qDebug() << "GphotoController::Destructor --> Start (Empty)";

    qDebug() << "GphotoController::Destructor --> End";
}


ImageGrabber* GphotoController::getImageGrabber()
{
    return grabber;
}


bool GphotoController::initialization(ImageGrabber* ig, ImageGrabberDevice* igd)
{
    qDebug() << "GphotoController::init --> Start";

    grabber = (GphotoGrabber*)ig;
    grabberDevice = igd;

    // qDebug() << "GphotoController::init --> Open the device ...";

    // Enumerating device capabilities

    qDebug() << "GphotoController::init --> End (Not Successful)";

    return false;
}


bool GphotoController::setUp()
{
    qDebug() << "GphotoController::setUp --> Start (Empty)";

    return true;
}


bool GphotoController::tearDown()
{
    qDebug() << "GphotoController::tearDown --> Start (Empty)";

    return true;
}


void GphotoController::getControlData(GrabberControlCapabilities *caps, long flags)
{
    /*
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
        qDebug() << "GphotoController::setCameraCapabilities --> Not supported control type";
        break;
    }
    */
}


void GphotoController::getControlFlag(GrabberControlCapabilities *caps)
{
    /*
    int                 ret = 0;
    struct v4l2_control ctrl;

    ctrl.id = queryctrl.id;
    ctrl.value = 0;
    ret = xioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if (ret != 0) {
        return;
    }
    if (ctrl.value == 1) {
        caps->setFlags(GrabberControlCapabilities::control_Auto);
    }
    else {
        caps->setFlags(GrabberControlCapabilities::control_Manual);
    }
    */

    return;
}


/*
 * sets the value for control id
 */
int GphotoController::setCtrlValue(GrabberControlCapabilities *caps, int value)
{
    qDebug() << "GphotoController::setCtrlValue --> Start";

    int ret = 0;
/*
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
            qDebug() << "GphotoController::setCtrlValue --> control id: " << ctrl.id << " failed to set (error " << ret << ")";
        }
    }

    // update real value
    // get_ctrl(hdevice, control_list, id, NULL);
*/
    qDebug() << "GphotoController::setCtrlValue --> End";
    return (ret);
}

/*
 * Gets the value for control id
 * and updates control flags and widgets
 */
int GphotoController::getCtrlValue(GrabberControlCapabilities *caps, int &value)
{
    qDebug() << "GphotoController::setCtrlValue --> Start";

    int ret = 0;
/*
    if (caps->getControlClass() == V4L2_CTRL_CLASS_USER) {
        // using VIDIOC_G_CTRL for user class controls

        struct v4l2_control ctrl;
        ctrl.id = caps->getControlId();
        ctrl.value = 0;
        ret = xioctl(fd, VIDIOC_G_CTRL, &ctrl);
        if (ret) {
            qDebug() << "GphotoController::setCtrlValue --> control id: " << ctrl.id << " failed to get value (error " << ret << ")";
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
        ret = xioctl(fd, VIDIOC_G_EXT_CTRLS, &ctrls);
        if (ret) {
            qDebug() << "GphotoController::setCtrlValue --> control id: " << ctrl.id << " failed to get value (error " << ret << ")";
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
*/
    qDebug() << "GphotoController::setCtrlValue --> End";
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
bool GphotoController::getAutomaticBrightness()
{
    return false;
}


void GphotoController::setAutomaticBrightness(bool ab)
{
    Q_ASSERT( 1 );
}

int GphotoController::getBrightness()
{
    return 0;
}


void GphotoController::setBrightness(int b)
{
    qDebug() << "GphotoController::setBrightness --> Start");

    setCtrlValue(getBrightnessCaps(), b);

    qDebug() << "GphotoController::setBrightness --> End");
}
*/

/**************************************************************************
 * Contrast
 **************************************************************************/
/*
bool GphotoController::getAutomaticContrast()
{
    return false;
}


void GphotoController::setAutomaticContrast(bool ac)
{
    Q_ASSERT( 1 );
}

int GphotoController::getContrast()
{
    return 0;
}


void GphotoController::setContrast(int c)
{
    qDebug() << "GphotoController::setContrast --> Start");

    setCtrlValue(getContrastCaps(), c);

    qDebug() << "GphotoController::setContrast --> End");
}
*/

/**************************************************************************
 * Saturation
 **************************************************************************/
/*
bool GphotoController::getAutomaticSaturation()
{
    return false;
}


void GphotoController::setAutomaticSaturation(bool as)
{
    Q_ASSERT( 1 );
}

int GphotoController::getSaturation()
{
    return 0;
}


void GphotoController::setSaturation(int s)
{
    qDebug() << "GphotoController::setSaturation --> Start");

    setCtrlValue(getSaturationCaps(), s);

    qDebug() << "GphotoController::setSaturation --> End");
}
*/

/**************************************************************************
 * Hue
 **************************************************************************/
/*
bool GphotoController::getAutomaticHue()
{
    return false;
}


void GphotoController::setAutomaticHue(bool ah)
{
    Q_ASSERT( 1 );
}

int GphotoController::getHue()
{
    return 0;
}


void GphotoController::setHue(int h)
{
    qDebug() << "GphotoController::setHue --> Start");

    setCtrlValue(getHueCaps(), h);

    qDebug() << "GphotoController::setHue --> End");
}
*/

/**************************************************************************
 * Gamma
 **************************************************************************/
/*
bool GphotoController::getAutomaticGamma()
{
    return false;
}


void GphotoController::setAutomaticGamma(bool ag)
{
    Q_ASSERT( 1 );
}

int GphotoController::getGamma()
{
    return 0;
}


void GphotoController::setGamma(int g)
{
    qDebug() << "GphotoController::setGamma --> Start";

    setCtrlValue(getGammaCaps(), g);

    qDebug() << "GphotoController::setGamma --> End";
}
*/

/**************************************************************************
 * Sharpness
 **************************************************************************/
/*
bool GphotoController::getAutomaticSharpness()
{
    return false;
}


void GphotoController::setAutomaticSharpness(bool as)
{
    Q_ASSERT( 1 );
}

int GphotoController::getSharpness()
{
    return 0;
}


void GphotoController::setSharpness(int s)
{
    qDebug() << "GphotoController::setSharpness --> Start";

    setCtrlValue(getSharpnessCaps(), s);

    qDebug() << "GphotoController::setSharpness --> End";
}
*/

/**************************************************************************
 * Backlight Compensation
 **************************************************************************/
/*
bool GphotoController::getAutomaticBacklight()
{
    return false;
}


void GphotoController::setAutomaticBacklight(bool ab)
{
    Q_ASSERT( 1 );
}

int GphotoController::getBacklight()
{
    return 0;
}


void GphotoController::setBacklight(int b)
{
    qDebug() << "GphotoController::setBacklight --> Start";

    setCtrlValue(getBacklightCaps(), b);

    qDebug() << "GphotoController::setBacklight --> End";
}
*/

/**************************************************************************
 * White Balance
 **************************************************************************/
/*
bool GphotoController::getAutomaticWhite()
{
    return false;
}


void GphotoController::setAutomaticWhite(bool aw)
{
    Q_ASSERT( 1 );
}

int GphotoController::getWhite()
{
    return 0;
}


void GphotoController::setWhite(int w)
{
    qDebug() << "GphotoController::setWhite --> Start";

    setCtrlValue(getWhiteCaps(), w);

    qDebug() << "GphotoController::setWhite --> End";
}
*/

/**************************************************************************
 * Gain
 **************************************************************************/
/*
bool GphotoController::getAutomaticGain()
{
    return false;
}


void GphotoController::setAutomaticGain(bool ag)
{
    Q_ASSERT( 1 );
}

int GphotoController::getGain()
{
    return 0;
}


void GphotoController::setGain(int g)
{
    qDebug() << "GphotoController::setGain --> Start";

    setCtrlValue(getGainCaps(), g);

    qDebug() << "GphotoController::setGain --> End";
}
*/

/**************************************************************************
 * Color Enable
 **************************************************************************/
/*
bool GphotoController::getAutomaticColor()
{
    return false;
}


void GphotoController::setAutomaticColor(bool ac)
{
    Q_ASSERT( 1 );
}

int GphotoController::getColor()
{
    return 0;
}


void GphotoController::setColor(int c)
{
    qDebug() << "GphotoController::setColor --> Start";

    setCtrlValue(getColorCaps(), c);

    qDebug() << "GphotoController::setColor --> End";
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
bool GphotoController::getAutomaticExposure()
{
    return false;
}


void GphotoController::setAutomaticExposure(bool ae)
{
    Q_ASSERT( 1 );
}

int GphotoController::getExposure()
{
    return 0;
}


void GphotoController::setExposure(int e)
{
    qDebug() << "GphotoController::setExposure --> Start";

    setCtrlValue(getExposureCaps(), e);

    qDebug() << "GphotoController::setExposure --> End";
}
*/

/**************************************************************************
 * Zoom
 **************************************************************************/
/*
bool GphotoController::getAutomaticZoom()
{
    return false;
}


void GphotoController::setAutomaticZoom(bool az)
{
    Q_ASSERT( 1 );
}
*/

int GphotoController::getZoom()
{
    int value = 0;

    getCtrlValue(getZoomCaps(), value);

    return value;
}


void GphotoController::setZoom(int z)
{
    qDebug() << "GphotoController::setZoom --> Start";

    setCtrlValue(getZoomCaps(), z);

    setCtrlValue(getZoomCaps(), z);

    qDebug() << "GphotoController::setZoom --> End";
}

/**************************************************************************
 * Focus
 **************************************************************************/
/*
bool GphotoController::getAutomaticFocus()
{
    return false;
}


void GphotoController::setAutomaticFocus(bool af)
{
    Q_ASSERT( 1 );
}

int GphotoController::getFocus()
{
    return 0;
}


void GphotoController::setFocus(int f)
{
    qDebug() << "GphotoController::setFocus --> Start";

    setCtrlValue(getFocusCaps(), f);

    qDebug() << "GphotoController::setFocus --> End";
}
*/

/**************************************************************************
 * Pan
 **************************************************************************/
/*
bool GphotoController::getAutomaticPan()
{
    return false;
}


void GphotoController::setAutomaticPan(bool ap)
{
    Q_ASSERT( 1 );
}


int GphotoController::getPan()
{
    return 0;
}


void GphotoController::setPan(int p)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Tilt
 **************************************************************************/
/*
bool GphotoController::getAutomaticTilt()
{
    return false;
}


void GphotoController::setAutomaticTilt(bool at)
{
    Q_ASSERT( 1 );
}


int GphotoController::getTilt()
{
    return 0;
}


void GphotoController::setTilt(int t)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Iris
 **************************************************************************/
/*
bool GphotoController::getAutomaticIris()
{
    return false;
}


void GphotoController::setAutomaticIris(bool ae)
{
    Q_ASSERT( 1 );
}


bool GphotoController::isIris()
{
    return false;
}


void GphotoController::setIris(int e)
{
    Q_ASSERT( 1 );
}
*/
/**************************************************************************
 * Roll
 **************************************************************************/
/*
bool GphotoController::getAutomaticRoll()
{
    return false;
}


void GphotoController::setAutomaticRoll(bool ar)
{
    Q_ASSERT( 1 );
}


bool GphotoController::isRoll()
{
    return false;
}


void GphotoController::setRoll(int e)
{
    Q_ASSERT( 1 );
}
*/
