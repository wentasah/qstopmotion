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

#ifndef GPHOTOCONTROLLER_H
#define GPHOTOCONTROLLER_H

// Include files of the gphoto library
#include <gphoto2/gphoto2.h>

#include "technical/grabber/grabbercontroller.h"


// set ioctl retries to 4 - linux uvc as increased timeout from 1000 to 3000 ms
#define IOCTL_RETRY 4

/**
 * Interface to the controller of a device.
 *
 * @author Ralf Lange
 */
class GphotoController : public GrabberController
{
public:

    // static struct uvc_xu_control_mapping xu_mappings[];

    /**
     * Constructs and initializes the object.
     * @param caps The grabber controller capabilities.
     */
    GphotoController(int caps);

    /**
     * Destructor
     */
    ~GphotoController();

    /**
     * Initialize the grabber controller.
     * @param id The ID of the device.
     * @return True if initialization is successful.
     */
    bool init(const QString &id);

    /**************************************************************************
     **************************************************************************
     * Camera quality capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Brightness
     **************************************************************************/

    /**
     * Get the current automatic brightness value of the device.
     * @return True if the automatic brightness is on.
     */
    // bool getAutomaticBrightness();

    /**
     * Set the automatic brightness value of the device.
     * @param ab True if the automatic brightness will be switched on.
     */
    // void setAutomaticBrightness(bool ab);

    /**
     * Get the current brightness value of the device.
     * @return The current brightness value.
     */
    // int getBrightness();

    /**
     * Set the brightness value of the device.
     * @param b The new brightness value
     */
    // void setBrightness(int b);

    /**************************************************************************
     * Contrast
     **************************************************************************/

    /**
     * Get the current automatic contrast value of the device.
     * @return True if the automatic contrast is on.
     */
    // bool getAutomaticContrast();

    /**
     * Set the automatic contrast value of the device.
     * @param ac True if the automatic contrast will be switched on.
     */
    // void setAutomaticContrast(bool ac);

    /**
     * Get the current contrast value of the device.
     * @return The current contrast value.
     */
    // int getContrast();

    /**
     * Set the contrast value of the device.
     * @param c The new contrast value
     */
    // void setContrast(int c);

    /**************************************************************************
     * Saturation
     **************************************************************************/

    /**
     * Get the current automatic saturation value of the device.
     * @return True if the automatic saturation is on.
     */
    // bool getAutomaticSaturation();

    /**
     * Set the automatic saturation value of the device.
     * @param as True if the automatic saturation will be switched on.
     */
    // void setAutomaticSaturation(bool as);

    /**
     * Get the current saturation value of the device.
     * @return The current saturation value.
     */
    // int getSaturation();

    /**
     * Set the saturation value of the device.
     * @param s The new saturation value
     */
    // void setSaturation(int s);

    /**************************************************************************
     * Hue
     **************************************************************************/

    /**
     * Get the current automatic hue value of the device.
     * @return True if the automatic hue is on.
     */
    // bool getAutomaticHue();

    /**
     * Set the automatic hue value of the device.
     * @param ah True if the automatic hue will be switched on.
     */
    // void setAutomaticHue(bool ah);

    /**
     * Get the current hue value of the device.
     * @return The current hue value.
     */
    // int getHue();

    /**
     * Set the hue value of the device.
     * @param h The new hue value
     */
    // void setHue(int h);

    /**************************************************************************
     * Gamma
     **************************************************************************/

    /**
     * Get the current automatic gamma value of the device.
     * @return True if the automatic gamma is on.
     */
    // bool getAutomaticGamma();

    /**
     * Set the automatic gamma value of the device.
     * @param ag True if the automatic gamma will be switched on.
     */
    // void setAutomaticGamma(bool ag);

    /**
     * Get the current gamma value of the device.
     * @return The current gamma value.
     */
    // int getGamma();

    /**
     * Set the gamma value of the device.
     * @param g The new gamma value
     */
    // void setGamma(int g);

    /**************************************************************************
     * Sharpness
     **************************************************************************/

    /**
     * Get the current automatic sharpness value of the device.
     * @return True if the automatic sharpness is on.
     */
    // bool getAutomaticSharpness();

    /**
     * Set the automatic sharpness value of the device.
     * @param as True if the automatic sharpness will be switched on.
     */
    // void setAutomaticSharpness(bool as);

    /**
     * Get the current sharpness value of the device.
     * @return The current sharpness value.
     */
    // int getSharpness();

    /**
     * Set the sharpness value of the device.
     * @param s The new sharpness value
     */
    // void setSharpness(int s);

    /**************************************************************************
     * Backlight Compensation
     **************************************************************************/

    /**
     * Get the current automatic backlight compensation value of the device.
     * @return True if the automatic backlight compensation is on.
     */
    // bool getAutomaticBacklight();

    /**
     * Set the automatic backlight compensation value of the device.
     * @param ab True if the automatic backlight compensation will be switched on.
     */
    // void setAutomaticBacklight(bool ab);

    /**
     * Get the current backlight compensation value of the device.
     * @return The current backlight compensation value.
     */
    // int getBacklight();

    /**
     * Set the backlight compensation value of the device.
     * @param b The new backlight compensation value
     */
    // void setBacklight(int b);

    /**************************************************************************
     * White Balance
     **************************************************************************/

    /**
     * Get the current automatic white balance value of the device.
     * @return True if the automatic white balance is on.
     */
    // bool getAutomaticWhite();

    /**
     * Set the automatic white balance value of the device.
     * @param ae True if the automatic white balance will be switched on.
     */
    // void setAutomaticWhite(bool ae);

    /**
     * Get the current white balance value of the device.
     * @return The current white balance value.
     */
    // int getWhite();

    /**
     * Set the white balance value of the device.
     * @param w The new white balance value
     */
    // void setWhite(int w);

    /**************************************************************************
     * Gain
     **************************************************************************/

    /**
     * Get the current automatic gain value of the device.
     * @return True if the automatic gain is on.
     */
    // bool getAutomaticGain();

    /**
     * Set the automatic gain value of the device.
     * @param ag True if the automatic gain will be switched on.
     */
    // void setAutomaticGain(bool ag);

    /**
     * Get the current gain value of the device.
     * @return The current gain value.
     */
    // int getGain();

    /**
     * Set the gain value of the device.
     * @param g The new gain value
     */
    // void setGain(int g);

    /**************************************************************************
     * Color Enable
     **************************************************************************/

    /**
     * Get the current automatic color enable value of the device.
     * @return True if the automatic color enable is on.
     */
    // bool getAutomaticColor();

    /**
     * Set the automatic color enable value of the device.
     * @param ac True if the automatic color enable will be switched on.
     */
    // void setAutomaticColor(bool ac);

    /**
     * Get the current color enable value of the device.
     * @return The current color enable value.
     */
    // int getColor();

    /**
     * Set the color enable value of the device.
     * @param c The new color enable value
     */
    // void setColor(int c);

    /**************************************************************************
     **************************************************************************
     * Camera control capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Exposure
     **************************************************************************/

    /**
     * Get the current automatic exposure value of the device.
     * @return True if the automatic exposure is on.
     */
    // bool getAutomaticExposure();

    /**
     * Set the automatic exposure value of the device.
     * @param ae True if the automatic exposure will be switched on.
     */
    // void setAutomaticExposure(bool ae);

    /**
     * Get the current exposure value of the device.
     * @return The current exposure value.
     */
    // int getExposure();

    /**
     * Set the exposure value of the device.
     * @param e The new exposure value
     */
    // void setExposure(int e);

    /**************************************************************************
     * Zoom
     **************************************************************************/

    /**
     * Get the current automatic zoom value of the device.
     * @return True if the automatic zoom is on.
     */
    // bool getAutomaticZoom();

    /**
     * Set the automatic zoom value of the device.
     * @param az True if the automatic zoom will be switched on.
     */
    // void setAutomaticZoom(bool az);

    /**
     * Get the current zoom value of the device.
     * @return The current zoom value.
     */
    int getZoom();

    /**
     * Set the zoom value of the device.
     * @param z The new zoom value
     */
    void setZoom(int z);

    /**************************************************************************
     * Focus
     **************************************************************************/

    /**
     * Get the current automatic focus value of the device.
     * @return True if the automatic focus is on.
     */
    // bool getAutomaticFocus();

    /**
     * Set the automatic focus value of the device.
     * @param af True if the automatic focus will be switched on.
     */
    // void setAutomaticFocus(bool af);

    /**
     * Get the current focus value of the device.
     * @return The current focus value.
     */
    // int getFocus();

    /**
     * Set the focus value of the device.
     * @param f The new focus value
     */
    // void setFocus(int f);

    /**************************************************************************
     * Pan
     **************************************************************************/

    /**
     * Get the current automatic pan value of the device.
     * @return True if the automatic pan is on.
     */
    // bool getAutomaticPan();

    /**
     * Set the automatic pan value of the device.
     * @param ap True if the automatic pan will be switched on.
     */
    // void setAutomaticPan(bool ap);

    /**
     * Get the current pan value of the device.
     * @return The current pan value.
     */
    // int getPan();

    /**
     * Set the pan value of the device.
     * @param p The new pan value
     */
    // void setPan(int p);

    /**************************************************************************
     * Tilt
     **************************************************************************/

    /**
     * Get the current automatic tilt value of the device.
     * @return True if the automatic tilt is on.
     */
    // bool getAutomaticTilt();

    /**
     * Set the automatic tilt value of the device.
     * @param at True if the automatic tilt will be switched on.
     */
    // void setAutomaticTilt(bool at);

    /**
     * Get the current tilt value of the device.
     * @return The current tilt value.
     */
    // int getTilt();

    /**
     * Set the tilt value of the device.
     * @param t The new tilt value
     */
    // void setTilt(int t);

    /**************************************************************************
     * Iris
     **************************************************************************/

    /**
     * Get the current automatic iris value of the device.
     * @return True if the automatic iris is on.
     */
    // bool getAutomaticIris();

    /**
     * Set the automatic iris value of the device.
     * @param ae True if the automatic iris will be switched on.
     */
    // void setAutomaticIris(bool ae);

    /**
     * Get the current iris value of the device.
     * @return The current iris value.
     */
    // int getIris();

    /**
     * Set the iris value of the device.
     * @param e The new iris value
     */
    // void setIris(int e);

    /**************************************************************************
     * Roll
     **************************************************************************/

    /**
     * Get the current automatic roll value of the device.
     * @return True if the automatic roll is on.
     */
    // bool getAutomaticRoll();

    /**
     * Set the automatic roll value of the device.
     * @param ar True if the automatic roll will be switched on.
     */
    // void setAutomaticRoll(bool ar);

    /**
     * Get the current roll value of the device.
     * @return The current roll value.
     */
    // int getRoll();

    /**
     * Set the roll value of the device.
     * @param r The new roll value
     */
    // void setRoll(int r);

private:


    void getControlData(GrabberControlCapabilities *caps, long flags = GrabberController::controller_none);
    void getControlFlag(GrabberControlCapabilities *caps);
    int  setCtrlValue(GrabberControlCapabilities *caps, int value);
    int  getCtrlValue(GrabberControlCapabilities *caps, int &value);

};

#endif
