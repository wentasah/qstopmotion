/******************************************************************************
 *  Copyright (C) 2012-2014 by                                                *
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

#ifndef MEDIAFOUNDATION_MFCONTROLLER_H
#define MEDIAFOUNDATION_MFCONTROLLER_H

#include "technical/grabber/grabbercontroller.h"
#include "technical/grabber/mediafoundation/mfgrabber.h"

// Include files of the media foundation
#include <strmif.h>
#include <mfapi.h>

/**
 * Interface to the controller of a device using the
 * Microsoft Media Foundation API.
 *
 * @author Ralf Lange
 */
class MfController : public GrabberController
{
public:

    /**
     * Constructs and initializes the object.
     * @param caps The grabber controller capabilities.
     */
    MfController(int caps);

    /**
     * Destructor
     */
    ~MfController();

    /**
     * Get the image grabber of the controller.
     *
     * @brief getImageGrabber
     * @return The image grabber or null.
     */
    ImageGrabber* getImageGrabber();

    /**
     * Initialize the grabber controller.
     * @param ig The image grabber of the controller.
     * @param igd The image grabber device of the controller.
     * @return True if initialization is successful.
     */
    bool initialization(ImageGrabber* ig, ImageGrabberDevice* igd);

    /**
     * Set up the controller.
     * @return true on success, false otherwise
     */
    bool setUp();

    /**
     * Tear down the controller.
     * @return true on success, false otherwise
     */
    bool tearDown();

    /**************************************************************************
     **************************************************************************
     * Camera capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Brightness
     **************************************************************************/

    /**
     * Has the camera a automatic brightness control capability?
     * @return True if the camera has a automatic brightness control apability.
     */
    // bool isAutomaticBrightness();

    /**
     * Get the current automatic brightness value of the device.
     * @return True if the automatic brightness is on.
     */
    bool getAutomaticBrightness();

    /**
     * Set the automatic brightness value of the device.
     * @param ae True if the automatic brightness will be switched on.
     */
    void setAutomaticBrightness(bool ab);

    /**
     * Has the camera a brightness control capability?
     * @return True if the camera has a brightness control apability.
     */
    // bool isBrightness();

    /**
     * Get the maximum value of the brightness.
     * @return The maximum value of the brightness.
     */
    // int getMaximumBrightness();

    /**
     * Get the current brightness value of the device.
     * @return The current brightness value.
     */
    int getBrightness();

    /**
     * Set the brightness value of the device.
     * @param b The new brightness value
     */
    void setBrightness(int b);

    /**************************************************************************
     * Contrast
     **************************************************************************/

    /**
     * Has the camera a contrast control capability?
     * @return True if the camera has a contrast control apability.
     */
    bool isContrast();

    /**
     * Get the maximum value of the contrast.
     * @return The maximum value of the contrast.
     */
    int getMaximumContrast();

    /**
     * Get the current contrast value of the device.
     * @return The current contrast value.
     */
    int getContrast();

    /**
     * Set the contrast value of the device.
     * @param c The new contrast value
     */
    void setContrast(int c);

    /**************************************************************************
     * Saturation
     **************************************************************************/

    /**
     * Has the camera a saturation control capability?
     * @return True if the camera has a saturation control apability.
     */
    bool isSaturation();

    /**
     * Get the maximum value of the saturation.
     * @return The maximum value of the saturation.
     */
    int getMaximumSaturation();

    /**
     * Get the current saturation value of the device.
     * @return The current saturation value.
     */
    int getSaturation();

    /**
     * Set the saturation value of the device.
     * @param s The new saturation value
     */
    void setSaturation(int s);

    /**************************************************************************
     * Hue
     **************************************************************************/

    /**
     * Has the camera a hue control capability?
     * @return True if the camera has a hue control apability.
     */
    bool isHue();

    /**
     * Get the maximum value of the hue.
     * @return The maximum value of the hue.
     */
    int getMaximumHue();

    /**
     * Get the current hue value of the device.
     * @return The current hue value.
     */
    int getHue();

    /**
     * Set the hue value of the device.
     * @param h The new hue value
     */
    void setHue(int h);

    /**************************************************************************
     * Gamma
     **************************************************************************/

    /**
     * Has the camera a gamma control capability?
     * @return True if the camera has a gamma control apability.
     */
    bool isGamma();

    /**
     * Get the maximum value of the gamma.
     * @return The maximum value of the gamma.
     */
    int getMaximumGamma();

    /**
     * Get the current gamma value of the device.
     * @return The current gamma value.
     */
    int getGamma();

    /**
     * Set the gamma value of the device.
     * @param g The new gamma value
     */
    void setGamma(int g);

    /**************************************************************************
     * Sharpness
     **************************************************************************/

    /**
     * Has the camera a sharpness control capability?
     * @return True if the camera has a sharpness control apability.
     */
    bool isSharpness();

    /**
     * Get the maximum value of the sharpness.
     * @return The maximum value of the sharpness.
     */
    int getMaximumSharpness();

    /**
     * Get the current sharpness value of the device.
     * @return The current sharpness value.
     */
    int getSharpness();

    /**
     * Set the sharpness value of the device.
     * @param s The new sharpness value
     */
    void setSharpness(int s);

    /**************************************************************************
     * Backlight Compensation
     **************************************************************************/

    /**
     * Has the camera a backlight compensation control capability?
     * @return True if the camera has a backlight compensation control apability.
     */
    bool isBacklight();

    /**
     * Get the maximum value of the backlight compensation.
     * @return The maximum value of the backlight compensation.
     */
    int getMaximumBacklight();

    /**
     * Get the current backlight compensation value of the device.
     * @return The current backlight compensation value.
     */
    int getBacklight();

    /**
     * Set the backlight compensation value of the device.
     * @param b The new backlight compensation value
     */
    void setBacklight(int b);

    /**************************************************************************
     * White Balance
     **************************************************************************/

    /**
     * Has the camera a automatic white balance control capability?
     * @return True if the camera has a automatic white balance control apability.
     */
    bool isAutomaticWhite();

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
     * Has the camera a white balance control capability?
     * @return True if the camera has a white balance control apability.
     */
    bool isWhite();

    /**
     * Get the maximum value of the white balance.
     * @return The maximum value of the white balance.
     */
    int getMaximumWhite();

    /**
     * Get the current white balance value of the device.
     * @return The current white balance value.
     */
    int getWhite();

    /**
     * Set the white balance value of the device.
     * @param w The new white balance value
     */
    void setWhite(int w);

    /**************************************************************************
     * Gain
     **************************************************************************/

    /**
     * Has the camera a gain control capability?
     * @return True if the camera has a gain control apability.
     */
    // bool isGain();

    /**
     * Get the maximum value of the gain.
     * @return The maximum value of the gain.
     */
    // int getMaximumGain();

    /**
     * Get the current gain value of the device.
     * @return The current gain value.
     */
    int getGain();

    /**
     * Set the gain value of the device.
     * @param g The new gain value
     */
    void setGain(int g);

    /**************************************************************************
     * Color
     **************************************************************************/

    /**
     * Has the camera a color control capability?
     * @return True if the camera has a color control apability.
     */
    // bool isColor();

    /**
     * Get the maximum value of the color.
     * @return The maximum value of the color.
     */
    // int getMaximumColor();

    /**
     * Get the current color value of the device.
     * @return The current color value.
     */
    int getColor();

    /**
     * Set the color value of the device.
     * @param c The new color value
     */
    void setColor(int c);

    /**************************************************************************
     **************************************************************************
     * Camera control capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Exposure
     **************************************************************************/

    /**
     * Has the camera a automatic exposure control capability?
     * @return True if the camera has a automatic exposure control apability.
     */
    bool isAutomaticExposure();

    /**
     * Get the current automatic exposure value of the device.
     * @return True if the automatic exposure is on.
     */
    bool getAutomaticExposure();

    /**
     * Set the automatic exposure value of the device.
     * @param ae True if the automatic exposure will be switched on.
     */
    void setAutomaticExposure(bool ae);

    /**
     * Has the camera a exposure control capability?
     * @return True if the camera has a exposure control apability.
     */
    bool isExposure();

    /**
     * Get the maximum value of the exposure.
     * @return The maximum value of the exposure.
     */
    int getMaximumExposure();

    /**
     * Get the current exposure value of the device.
     * @return The current exposure value.
     */
    int getExposure();

    /**
     * Set the exposure value of the device.
     * @param e The new exposure value
     */
    void setExposure(int e);

    /**************************************************************************
     * Zoom
     **************************************************************************/

    /**
     * Has the camera a automatic zoom control capability?
     * @return True if the camera has a automatic zoom control apability.
     */
    bool isAutomaticZoom();

    /**
     * Get the current automatic zoom value of the device.
     * @return True if the automatic zoom is on.
     */
    bool getAutomaticZoom();

    /**
     * Set the automatic zoom value of the device.
     * @param az True if the automatic zoom will be switched on.
     */
    void setAutomaticZoom(bool az);

    /**
     * Has the camera a zoom control capability?
     * @return True if the camera has a zoom control apability.
     */
    bool isZoom();

    /**
     * Get the maximum value of the zoom.
     * @return The maximum value of the zoom.
     */
    int getMaximumZoom();

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
     * Has the camera a automatic focus control capability?
     * @return True if the camera has a automatic focus control apability.
     */
    bool isAutomaticFocus();

    /**
     * Get the current automatic focus value of the device.
     * @return True if the automatic focus is on.
     */
    bool getAutomaticFocus();

    /**
     * Set the automatic focus value of the device.
     * @param af True if the automatic focus will be switched on.
     */
    void setAutomaticFocus(bool af);

    /**
     * Has the camera a focus control capability?
     * @return True if the camera has a focus control apability.
     */
    bool isFocus();

    /**
     * Get the maximum focus of the zoom.
     * @return The maximum focus of the zoom.
     */
    int getMaximumFocus();

    /**
     * Get the current focus value of the device.
     * @return The current focus value.
     */
    int getFocus();

    /**
     * Set the focus value of the device.
     * @param f The new focus value
     */
    void setFocus(int f);

    /**************************************************************************
     * Pan
     **************************************************************************/

    /**
     * Has the camera a pan control capability?
     * @return True if the camera has a pan control apability.
     */
    bool isPan();

    /**
     * Get the maximum value of the pan.
     * @return The maximum value of the pan.
     */
    int getMaximumPan();

    /**
     * Get the current pan value of the device.
     * @return The current pan value.
     */
    int getPan();

    /**
     * Set the pan value of the device.
     * @param p The new pan value
     */
    void setPan(int p);

    /**************************************************************************
     * Tilt
     **************************************************************************/

    /**
     * Has the camera a tilt control capability?
     * @return True if the camera has a tilt control apability.
     */
    bool isTilt();

    /**
     * Get the maximum value of the tilt.
     * @return The maximum value of the tilt.
     */
    int getMaximumTilt();

    /**
     * Get the current tilt value of the device.
     * @return The current tilt value.
     */
    int getTilt();

    /**
     * Set the tilt value of the device.
     * @param t The new tilt value
     */
    void setTilt(int t);

    /**************************************************************************
     * Iris
     **************************************************************************/

    /**
     * Get the current automatic iris value of the device.
     * @return True if the automatic iris is on.
     */
    bool getAutomaticIris();

    /**
     * Set the automatic iris value of the device.
     * @param ae True if the automatic iris will be switched on.
     */
    void setAutomaticIris(bool ae);

    /**
     * Get the current iris value of the device.
     * @return The current iris value.
     */
    int getIris();

    /**
     * Set the iris value of the device.
     * @param e The new iris value
     */
    void setIris(int e);

    /**************************************************************************
     * Roll
     **************************************************************************/

    /**
     * Get the current automatic roll value of the device.
     * @return True if the automatic roll is on.
     */
    bool getAutomaticRoll();

    /**
     * Set the automatic roll value of the device.
     * @param ar True if the automatic roll will be switched on.
     */
    void setAutomaticRoll(bool ar);

    /**
     * Get the current roll value of the device.
     * @return The current roll value.
     */
    int getRoll();

    /**
     * Set the roll value of the device.
     * @param r The new roll value
     */
    void setRoll(int r);

private:

    MfGrabber*          grabber;
    ImageGrabberDevice* grabberDevice;
    IAMCameraControl*   cameraControl;
    IAMVideoProcAmp*    qualityControl;

    bool setControlCapabilities();
    bool setQualityCapabilities();

};

#endif
