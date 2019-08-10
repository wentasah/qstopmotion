/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
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

#ifndef CAMERACONTROLLERDIALOG_H
#define CAMERACONTROLLERDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <qwt_slider.h>

#include "frontends/frontend.h"
#include "technical/grabber/grabbercontroller.h"
#include "technical/grabber/imagegrabberdevice.h"


class CameraControllerDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Constructor
     */
    CameraControllerDialog(Frontend *f,
                           ImageGrabberDevice *device,
                           QWidget *parent = 0);

    /**
     * Create the GUI elements
     */
    void makeGUI();

    /**
     * Retranslate all strings of the tab after changing the language setting
     */
    void retranslateStrings();

    /**
     * Initialize the dialog
     */
    void initialize();

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

    /**
     * Enable/disable the controls selected on the control preferences.
     * @brief enableControls
     */
    void enableControls();

private slots:

    /**
     * Slot for notified the camera contoller when automatic brightness is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoBrightness(int newState);

    /**
     * Slot for notified the camera contoller when automatic brightness is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoBrightness(int newState, bool save);

    /**
     * Slot for notified the camera controller when the brightness changes,
     * so that brightness can be updated.
     */
    void changeBrightness();

    /**
     * Slot for notified the camera controller when the brightness changes,
     * so that brightness can be updated.
     * @param value the new brightness value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeBrightness(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic contrast is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoContrast(int newState);

    /**
     * Slot for notified the camera contoller when automatic contrast is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoContrast(int newState, bool save);

    /**
     * Slot for notified the camera controller when the contrast changes,
     * so that contrast can be updated.
     */
    void changeContrast();

    /**
     * Slot for notified the camera controller when the contrast changes,
     * so that contrast can be updated.
     * @param value the new contrast value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeContrast(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic saturation is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoSaturation(int newState);

    /**
     * Slot for notified the camera contoller when automatic saturation is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoSaturation(int newState, bool save);

    /**
     * Slot for notified the camera controller when the saturation changes,
     * so that saturation can be updated.
     */
    void changeSaturation();

    /**
     * Slot for notified the camera controller when the saturation changes,
     * so that saturation can be updated.
     * @param value the new saturation value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeSaturation(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic hue is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoHue(int newState);

    /**
     * Slot for notified the camera contoller when automatic hue is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoHue(int newState, bool save);

    /**
     * Slot for notified the camera controller when the hue changes,
     * so that hue can be updated.
     */
    void changeHue();

    /**
     * Slot for notified the camera controller when the hue changes,
     * so that hue can be updated.
     * @param value the new hue value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeHue(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic gamma is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoGamma(int newState);

    /**
     * Slot for notified the camera contoller when automatic gamma is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoGamma(int newState, bool save);

    /**
     * Slot for notified the camera controller when the gamma changes,
     * so that gamma can be updated.
     */
    void changeGamma();

    /**
     * Slot for notified the camera controller when the gamma changes,
     * so that gamma can be updated.
     * @param value the new gamma value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeGamma(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic sharpness is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoSharpness(int newState);

    /**
     * Slot for notified the camera contoller when automatic sharpness is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoSharpness(int newState, bool save);

    /**
     * Slot for notified the camera controller when the sharpness changes,
     * so that sharpness can be updated.
     */
    void changeSharpness();

    /**
     * Slot for notified the camera controller when the sharpness changes,
     * so that sharpness can be updated.
     * @param value the new sharpness value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeSharpness(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic backlight is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoBacklight(int newState);

    /**
     * Slot for notified the camera contoller when automatic backlight is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoBacklight(int newState, bool save);

    /**
     * Slot for notified the camera controller when the backlight compensation changes,
     * so that backlight compensation can be updated.
     */
    void changeBacklight();

    /**
     * Slot for notified the camera controller when the backlight compensation changes,
     * so that backlight compensation can be updated.
     * @param value the new backlight compensation value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeBacklight(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic gain is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoGain(int newState);

    /**
     * Slot for notified the camera contoller when automatic gain is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoGain(int newState, bool save);

    /**
     * Slot for notified the camera controller when the gain changes,
     * so that gain can be updated.
     */
    void changeGain();

    /**
     * Slot for notified the camera controller when the gain changes,
     * so that gain can be updated.
     * @param value the new gain value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeGain(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic color enable is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoColor(int newState);

    /**
     * Slot for notified the camera contoller when automatic color enable is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoColor(int newState, bool save);

    /**
     * Slot for notified the camera controller when the color enable changes,
     * so that color enable can be updated.
     */
    void changeColor();

    /**
     * Slot for notified the camera controller when the color enable changes,
     * so that color enable can be updated.
     * @param value the new white balance value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeColor(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic white balance is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoWhite(int newState);

    /**
     * Slot for notified the camera contoller when automatic white balance is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoWhite(int newState, bool save);

    /**
     * Slot for notified the camera controller when the white balance changes,
     * so that white balance can be updated.
     */
    void changeWhite();

    /**
     * Slot for notified the camera controller when the white balance changes,
     * so that white balance can be updated.
     * @param value the new white balance value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeWhite(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic exposure is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoExposure(int newState);

    /**
     * Slot for notified the camera contoller when automatic exposure is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoExposure(int newState, bool save);

    /**
     * Slot for notified the camera controller when the exposure changes,
     * so that exposure can be updated.
     */
    void changeExposure();

    /**
     * Slot for notified the camera controller when the exposure changes,
     * so that exposure can be updated.
     * @param value the new exposure value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeExposure(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic zoom is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     */
    void changeAutoZoom(int newState);

    /**
     * Slot for notified the camera contoller when automatic zoom is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoZoom(int newState, bool save);

    /**
     * Slot for notified the camera controller when the zoom changes,
     * so that zoom can be updated.
     */
    void changeZoom();

    /**
     * Slot for notified the camera controller when the zoom changes,
     * so that zoom can be updated.
     * @param value The new zoo value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeZoom(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic focus is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoFocus(int newState);

    /**
     * Slot for notified the camera contoller when automatic focus is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoFocus(int newState, bool save);

    /**
     * Slot for notified the camera controller when the focus changes,
     * so that focus can be updated.
     */
    void changeFocus();

    /**
     * Slot for notified the camera controller when the focus changes,
     * so that focus can be updated.
     * @param value the new focus value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeFocus(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic pan is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoPan(int newState);

    /**
     * Slot for notified the camera contoller when automatic pan is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoPan(int newState, bool save);

    /**
     * Slot for notified the camera controller when the pan changes,
     * so that pan can be updated.
     */
    void changePan();

    /**
     * Slot for notified the camera controller when the pan changes,
     * so that pan can be updated.
     * @param value the new pan value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changePan(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic tilt is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoTilt(int newState);

    /**
     * Slot for notified the camera contoller when automatic tilt is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoTilt(int newState, bool save);

    /**
     * Slot for notified the camera controller when the tilt changes,
     * so that tilt can be updated.
     */
    void changeTilt();

    /**
     * Slot for notified the camera controller when the tilt changes,
     * so that tilt can be updated.
     * @param value the new tilt value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeTilt(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic iris is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoIris(int newState);

    /**
     * Slot for notified the camera contoller when automatic iris is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoIris(int newState, bool save);

    /**
     * Slot for notified the camera controller when the iris changes,
     * so that iris can be updated.
     */
    void changeIris();

    /**
     * Slot for notified the camera controller when the iris changes,
     * so that iris can be updated.
     * @param value the new iris value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeIris(int value, bool save);

    /**
     * Slot for notified the camera contoller when automatic roll is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     */
    void changeAutoRoll(int newState);

    /**
     * Slot for notified the camera contoller when automatic roll is changed, so that
     * camera contoller can be updaten.
     * @param newState The new state of the check box
     * @param save If true than the new sate is saved in the preferences file.
     */
    void changeAutoRoll(int newState, bool save);

    /**
     * Slot for notified the camera controller when the roll changes,
     * so that iris can be updated.
     */
    void changeRoll();

    /**
     * Slot for notified the camera controller when the roll changes,
     * so that iris can be updated.
     * @param value the new roll value.
     * @param save If true than the new value is saved in the preferences file.
     */
    void changeRoll(int value, bool save);

    /**
     * Slot for notified the camera controller to reset to the default values.
     */
    void reset();

    /**
     * Slot for notified the camera controller to open the online help.
     *
     * @brief help
     */
    void help();

    /**
     * Slot for notified the camera controller to close the dialog.
     *
     * @brief close
     */
    void close();

private:
    /**
     * Configure the slider with the values.
     * @param slider The slider to configure.
     * @param controlCaps The control capabilities.
     * @param value The actual value of the slider.
     * @return The step lengt for the values of the slider.
     */
    void configureSlider(QwtSlider *slider, GrabberControlCapabilities *controlCaps, int value);

    Frontend     *frontend;

    ImageGrabberDevice *grabberDevice;
    QString             deviceId;
    GrabberController  *grabberController;

    QGroupBox    *qualityGroupBox;
    int           qualityCount;

    QCheckBox    *brightnessCheckBox;
    QLabel       *brightnessLabel;
    QwtSlider    *brightnessSlider;

    QCheckBox    *contrastCheckBox;
    QLabel       *contrastLabel;
    QwtSlider    *contrastSlider;

    QCheckBox    *saturationCheckBox;
    QLabel       *saturationLabel;
    QwtSlider    *saturationSlider;

    QCheckBox    *hueCheckBox;
    QLabel       *hueLabel;
    QwtSlider    *hueSlider;

    QCheckBox    *gammaCheckBox;
    QLabel       *gammaLabel;
    QwtSlider    *gammaSlider;

    QCheckBox    *sharpnessCheckBox;
    QLabel       *sharpnessLabel;
    QwtSlider    *sharpnessSlider;

    QCheckBox    *backlightCheckBox;
    QLabel       *backlightLabel;
    QwtSlider    *backlightSlider;

    QCheckBox    *whiteCheckBox;
    QLabel       *whiteLabel;
    QwtSlider    *whiteSlider;

    QCheckBox    *gainCheckBox;
    QLabel       *gainLabel;
    QwtSlider    *gainSlider;

    QCheckBox    *colorCheckBox;
    QLabel       *colorLabel;
    QwtSlider    *colorSlider;

    QGroupBox    *controlGroupBox;
    int           controlCount;

    QCheckBox    *exposureCheckBox;
    QLabel       *exposureLabel;
    QwtSlider    *exposureSlider;

    QCheckBox    *zoomCheckBox;
    QLabel       *zoomLabel;
    QwtSlider    *zoomSlider;

    QCheckBox    *focusCheckBox;
    QLabel       *focusLabel;
    QwtSlider    *focusSlider;

    QCheckBox    *panCheckBox;
    QLabel       *panLabel;
    QwtSlider    *panSlider;

    QCheckBox    *tiltCheckBox;
    QLabel       *tiltLabel;
    QwtSlider    *tiltSlider;

    QCheckBox    *irisCheckBox;
    QLabel       *irisLabel;
    QwtSlider    *irisSlider;

    QCheckBox    *rollCheckBox;
    QLabel       *rollLabel;
    QwtSlider    *rollSlider;

    QVBoxLayout  *mainLayout;

    QPushButton  *resetButton;
    QPushButton  *helpButton;
    QPushButton  *closeButton;
};

#endif
