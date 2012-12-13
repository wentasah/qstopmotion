/******************************************************************************
 *  Copyright (C) 2012-2012 by                                                *
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

#ifndef GRABBERDIRECTSHOWCONTROLLER_H
#define GRABBERDIRECTSHOWCONTROLLER_H

#include "technical/grabber/grabbercontroller.h"

#include <dshow.h>
#include <Ks.h>				// Required by KsMedia.h
#include <KsMedia.h>		// For KSPROPERTY_CAMERACONTROL_FLAGS_*

/**
 * Interface to the controller of a device.
 *
 * @author Ralf Lange
 */
class GrabberDirectShowController : public GrabberController
{
public:

    /**
     * Constructs and initializes the object.
     * @param caps The grabber controller capabilities.
     */
    GrabberDirectShowController(int caps);

    /**
     * Destructor
     */
    ~GrabberDirectShowController();

    /**
     * Initialize the grabber controller.
     * @param id The ID of the device.
     * @return True if initialization is successful.
     */
    bool init(const QString id);

    /**************************************************************************
     **************************************************************************
     * Camera capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Brightness
     **************************************************************************/

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
     * Zoom
     **************************************************************************/

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
     * Get the current tilt value of the device.
     * @return The current tilt value.
     */
    int getTilt();

    /**
     * Set the tilt value of the device.
     * @param t The new tilt value
     */
    void setTilt(int t);

private:
    IAMCameraControl *pCameraControl;

    bool setCapabilities();

};

#endif
