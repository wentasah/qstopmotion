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

#ifndef GRABBERCONTROLLER_H
#define GRABBERCONTROLLER_H

#include "technical/grabber/grabbercontrolcapabilities.h"
#include "technical/grabber/grabberresolution.h"

#include <QtCore/QString>
#include <QtCore/QVector>

/**
 * Interface to the controller of a device.
 *
 * @author Ralf Lange
 */
class GrabberController
{
public:

    /**
     * Device controller capabilities
     */
    enum grabberControllerFlags {
        controller_none,
        controller_Auto,
        controller_Manual,
        controller_Relative,
        controller_Absolute,
        controller_Continuous
    };

    /**
     * Constructs and initializes the object.
     * @param caps The grabber controller capabilities.
     */
    GrabberController(int caps);

    /**
     * Destructor
     */
    virtual ~GrabberController();

    /**
     * Initialize the grabber controller.
     * @param id The ID of the device.
     * @return True if initialization is successful.
     */
    bool init(const QString &id);

    /**
     * Get the capabilities of the controller.
     * @return The capabilities of the controller.
     */
    int getControllerCapabilities();

    /**************************************************************************
     **************************************************************************
     * Camera resolution
     **************************************************************************
     **************************************************************************/

    /**
     * Add a resolution to the controller.
     * @param r The new grabber resolution.
     */
    void addResolution(GrabberResolution r);

    /**
     * Get the possible resolutions of the controller.
     * @return The resolutions of the controller.
     */
    QVector<GrabberResolution> getResolutions();

    /**
     * Get the active resolution of the controller.
     * @return The index of the active resolution.
     */
    virtual int getActiveResolution();

    /**
     * Set the active resolution of the controller.
     * @param ac The index of the new active resolution.
     */
    virtual void setActiveResolution(int ac);

    /**************************************************************************
     **************************************************************************
     * Camera quality capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Brightness
     **************************************************************************/

    /**
     * Get the capabilities of the brightness control.
     * @return The brightness control capabilities.
     */
    GrabberControlCapabilities *getBrightnessCaps();

    /**
     * Get the current automatic brightness value of the device.
     * @return True if the automatic brightness is on.
     */
    virtual bool getAutomaticBrightness();

    /**
     * Set the automatic brightness value of the device.
     * @param ab True if the automatic brightness will be switched on.
     */
    virtual void setAutomaticBrightness(bool ab);

    /**
     * Get the current brightness value of the device.
     * @return The current brightness value.
     */
    virtual int getBrightness();

    /**
     * Set the brightness value of the device.
     * @param b The new brightness value
     */
    virtual void setBrightness(int b);

    /**************************************************************************
     * Contrast
     **************************************************************************/

    /**
     * Get the capabilities of the contrast control.
     * @return The contrast control capabilities.
     */
    GrabberControlCapabilities *getContrastCaps();

    /**
     * Get the current automatic contrast value of the device.
     * @return True if the automatic contrast is on.
     */
    virtual bool getAutomaticContrast();

    /**
     * Set the automatic contrast value of the device.
     * @param ac True if the automatic contrast will be switched on.
     */
    virtual void setAutomaticContrast(bool ac);

    /**
     * Get the current contrast value of the device.
     * @return The current contrast value.
     */
    virtual int getContrast();

    /**
     * Set the contrast value of the device.
     * @param c The new contrast value
     */
    virtual void setContrast(int c);

    /**************************************************************************
     * Saturation
     **************************************************************************/

    /**
     * Get the capabilities of the saturation control.
     * @return The saturation control capabilities.
     */
    GrabberControlCapabilities *getSaturationCaps();

    /**
     * Get the current automatic saturation value of the device.
     * @return True if the automatic saturation is on.
     */
    virtual bool getAutomaticSaturation();

    /**
     * Set the automatic saturation value of the device.
     * @param as True if the automatic saturation will be switched on.
     */
    virtual void setAutomaticSaturation(bool as);

    /**
     * Get the current saturation value of the device.
     * @return The current saturation value.
     */
    virtual int getSaturation();

    /**
     * Set the saturation value of the device.
     * @param s The new saturation value
     */
    virtual void setSaturation(int s);

    /**************************************************************************
     * Hue
     **************************************************************************/

    /**
     * Get the capabilities of the hue control.
     * @return The hue control capabilities.
     */
    GrabberControlCapabilities *getHueCaps();

    /**
     * Get the current automatic hue value of the device.
     * @return True if the automatic hue is on.
     */
    virtual bool getAutomaticHue();

    /**
     * Set the automatic hue value of the device.
     * @param ah True if the automatic hue will be switched on.
     */
    virtual void setAutomaticHue(bool ah);

    /**
     * Get the current hue value of the device.
     * @return The current hue value.
     */
    virtual int getHue();

    /**
     * Set the hue value of the device.
     * @param h The new hue value
     */
    virtual void setHue(int h);

    /**************************************************************************
     * Gamma
     **************************************************************************/

    /**
     * Get the capabilities of the gamma control.
     * @return The gamma control capabilities.
     */
    GrabberControlCapabilities *getGammaCaps();

    /**
     * Get the current automatic gamma value of the device.
     * @return True if the automatic gamma is on.
     */
    virtual bool getAutomaticGamma();

    /**
     * Set the automatic gamma value of the device.
     * @param ag True if the automatic gamma will be switched on.
     */
    virtual void setAutomaticGamma(bool ag);

    /**
     * Get the current gamma value of the device.
     * @return The current gamma value.
     */
    virtual int getGamma();

    /**
     * Set the gamma value of the device.
     * @param g The new gamma value
     */
    virtual void setGamma(int g);

    /**************************************************************************
     * Sharpness
     **************************************************************************/

    /**
     * Get the capabilities of the sharpness control.
     * @return The sharpness control capabilities.
     */
    GrabberControlCapabilities *getSharpnessCaps();

    /**
     * Get the current automatic sharpness value of the device.
     * @return True if the automatic sharpness is on.
     */
    virtual bool getAutomaticSharpness();

    /**
     * Set the automatic sharpness value of the device.
     * @param as True if the automatic sharpness will be switched on.
     */
    virtual void setAutomaticSharpness(bool as);

    /**
     * Get the current sharpness value of the device.
     * @return The current sharpness value.
     */
    virtual int getSharpness();

    /**
     * Set the sharpness value of the device.
     * @param s The new sharpness value
     */
    virtual void setSharpness(int s);

    /**************************************************************************
     * Backlight Compensation
     **************************************************************************/

    /**
     * Get the capabilities of the backlight control.
     * @return The backlight control capabilities.
     */
    GrabberControlCapabilities *getBacklightCaps();

    /**
     * Get the current automatic backlight value of the device.
     * @return True if the automatic backlight is on.
     */
    virtual bool getAutomaticBacklight();

    /**
     * Set the automatic backlight value of the device.
     * @param ab True if the automatic backlight will be switched on.
     */
    virtual void setAutomaticBacklight(bool ab);

    /**
     * Get the current backlight compensation value of the device.
     * @return The current backlight compensation value.
     */
    virtual int getBacklight();

    /**
     * Set the backlight compensation value of the device.
     * @param b The new backlight compensation value
     */
    virtual void setBacklight(int b);

    /**************************************************************************
     * White Balance
     **************************************************************************/

    /**
     * Get the capabilities of the white control.
     * @return The white control capabilities.
     */
    GrabberControlCapabilities *getWhiteCaps();

    /**
     * Get the current automatic white balance value of the device.
     * @return True if the automatic white balance is on.
     */
    virtual bool getAutomaticWhite();

    /**
     * Set the automatic white balance value of the device.
     * @param aw True if the automatic white balance will be switched on.
     */
    virtual void setAutomaticWhite(bool aw);

    /**
     * Get the current white balance value of the device.
     * @return The current white balance value.
     */
    virtual int getWhite();

    /**
     * Set the white balance value of the device.
     * @param w The new white balance value
     */
    virtual void setWhite(int w);

    /**************************************************************************
     * Gain
     **************************************************************************/

    /**
     * Get the capabilities of the gain control.
     * @return The gain control capabilities.
     */
    GrabberControlCapabilities *getGainCaps();

    /**
     * Get the current automatic gain value of the device.
     * @return True if the automatic gain is on.
     */
    virtual bool getAutomaticGain();

    /**
     * Set the automatic gain value of the device.
     * @param ag True if the automatic gain will be switched on.
     */
    virtual void setAutomaticGain(bool ag);

    /**
     * Get the current gain value of the device.
     * @return The current gain value.
     */
    virtual int getGain();

    /**
     * Set the gain value of the device.
     * @param g The new gain value
     */
    virtual void setGain(int g);

    /**************************************************************************
     * Color Enable
     **************************************************************************/

    /**
     * Get the capabilities of the color enable control.
     * @return The color enable control capabilities.
     */
    GrabberControlCapabilities *getColorCaps();

    /**
     * Get the current automatic color enable value of the device.
     * @return True if the automatic color enable is on.
     */
    virtual bool getAutomaticColor();

    /**
     * Set the automatic color enable value of the device.
     * @param ac True if the automatic color enable will be switched on.
     */
    virtual void setAutomaticColor(bool ac);

    /**
     * Get the current color enable value of the device.
     * @return The current color enable value.
     */
    virtual int getColor();

    /**
     * Set the color enable value of the device.
     * @param c The new color enable value
     */
    virtual void setColor(int c);

    /**************************************************************************
     **************************************************************************
     * Camera control capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Exposure
     **************************************************************************/

    /**
     * Get the capabilities of the exposure control.
     * @return The exposure control capabilities.
     */
    GrabberControlCapabilities *getExposureCaps();

    /**
     * Get the current automatic exposure value of the device.
     * @return True if the automatic exposure is on.
     */
    virtual bool getAutomaticExposure();

    /**
     * Set the automatic exposure value of the device.
     * @param ae True if the automatic exposure will be switched on.
     */
    virtual void setAutomaticExposure(bool ae);

    /**
     * Get the current exposure value of the device.
     * @return The current exposure value.
     */
    virtual int getExposure();

    /**
     * Set the exposure value of the device.
     * @param e The new exposure value
     */
    virtual void setExposure(int e);

    /**************************************************************************
     * Zoom
     **************************************************************************/

    /**
     * Get the capabilities of the zoom control.
     * @return The zoom control capabilities.
     */
    GrabberControlCapabilities *getZoomCaps();

    /**
     * Get the current automatic zoom value of the device.
     * @return True if the automatic zoom is on.
     */
    virtual bool getAutomaticZoom();

    /**
     * Set the automatic zoom value of the device.
     * @param az True if the automatic zoom will be switched on.
     */
    virtual void setAutomaticZoom(bool az);

    /**
     * Get the current zoom value of the device.
     * @return The current zoom value.
     */
    virtual int getZoom();

    /**
     * Set the zoom value of the device.
     * @param z The new zoom value
     */
    virtual void setZoom(int z);

    /**************************************************************************
     * Focus
     **************************************************************************/

    /**
     * Get the capabilities of the focus control.
     * @return The focus control capabilities.
     */
    GrabberControlCapabilities *getFocusCaps();

    /**
     * Get the current automatic focus value of the device.
     * @return True if the automatic focus is on.
     */
    virtual bool getAutomaticFocus();

    /**
     * Set the automatic focus value of the device.
     * @param af True if the automatic focus will be switched on.
     */
    virtual void setAutomaticFocus(bool af);

    /**
     * Get the current focus value of the device.
     * @return The current focus value.
     */
    virtual int getFocus();

    /**
     * Set the focus value of the device.
     * @param f The new focus value
     */
    virtual void setFocus(int f);

    /**************************************************************************
     * Pan
     **************************************************************************/

    /**
     * Get the capabilities of the pan control.
     * @return The pan control capabilities.
     */
    GrabberControlCapabilities *getPanCaps();

    /**
     * Get the current automatic pan value of the device.
     * @return True if the automatic pan is on.
     */
    virtual bool getAutomaticPan();

    /**
     * Set the automatic pan value of the device.
     * @param ap True if the automatic pan will be switched on.
     */
    virtual void setAutomaticPan(bool ap);

    /**
     * Get the current pan value of the device.
     * @return The current pan value.
     */
    virtual int getPan();

    /**
     * Set the pan value of the device.
     * @param p The new pan value
     */
    virtual void setPan(int p);

    /**************************************************************************
     * Tilt
     **************************************************************************/

    /**
     * Get the capabilities of the tilt control.
     * @return The tilt control capabilities.
     */
    GrabberControlCapabilities *getTiltCaps();

    /**
     * Get the current automatic tilt value of the device.
     * @return True if the automatic tilt is on.
     */
    virtual bool getAutomaticTilt();

    /**
     * Set the automatic tilt value of the device.
     * @param at True if the automatic tilt will be switched on.
     */
    virtual void setAutomaticTilt(bool at);

    /**
     * Get the current tilt value of the device.
     * @return The current tilt value.
     */
    virtual int getTilt();

    /**
     * Set the tilt value of the device.
     * @param t The new tilt value
     */
    virtual void setTilt(int t);

    /**************************************************************************
     * Iris
     **************************************************************************/

    /**
     * Get the capabilities of the iris control.
     * @return The iris control capabilities.
     */
    GrabberControlCapabilities *getIrisCaps();

    /**
     * Get the current automatic iris value of the device.
     * @return True if the automatic iris is on.
     */
    virtual bool getAutomaticIris();

    /**
     * Set the automatic iris value of the device.
     * @param ai True if the automatic iris will be switched on.
     */
    virtual void setAutomaticIris(bool ai);

    /**
     * Get the current iris value of the device.
     * @return The current iris value.
     */
    virtual int getIris();

    /**
     * Set the iris value of the device.
     * @param i The new iris value
     */
    virtual void setIris(int i);

    /**************************************************************************
     * Roll
     **************************************************************************/

    /**
     * Get the capabilities of the roll control.
     * @return The roll control capabilities.
     */
    GrabberControlCapabilities *getRollCaps();

    /**
     * Get the current automatic roll value of the device.
     * @return True if the automatic roll is on.
     */
    virtual bool getAutomaticRoll();

    /**
     * Set the automatic roll value of the device.
     * @param ar True if the automatic roll will be switched on.
     */
    virtual void setAutomaticRoll(bool ar);

    /**
     * Get the current roll value of the device.
     * @return The current roll value.
     */
    virtual int getRoll();

    /**
     * Set the roll value of the device.
     * @param r The new roll value
     */
    virtual void setRoll(int r);

private:
    int                         controllerCap;
    QVector<GrabberResolution>  resolutions;

    // Video capabilities
    GrabberControlCapabilities  brightnessCapabilities;
    GrabberControlCapabilities  contrastCapabilities;
    GrabberControlCapabilities  saturationCapabilities;
    GrabberControlCapabilities  hueCapabilities;
    GrabberControlCapabilities  gammaCapabilities;
    GrabberControlCapabilities  sharpnessCapabilities;
    GrabberControlCapabilities  backlightCapabilities;
    GrabberControlCapabilities  whiteCapabilities;
    GrabberControlCapabilities  gainCapabilities;
    GrabberControlCapabilities  colorCapabilities;

    // Camera control capabilities
    GrabberControlCapabilities  exposureCapabilities;
    GrabberControlCapabilities  zoomCapabilities;
    GrabberControlCapabilities  focusCapabilities;
    GrabberControlCapabilities  panCapabilities;
    GrabberControlCapabilities  tiltCapabilities;
    GrabberControlCapabilities  irisCapabilities;
    GrabberControlCapabilities  rollCapabilities;
};

#endif
