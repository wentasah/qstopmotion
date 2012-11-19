/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#include "frontends/frontend.h"
#include "technical/grabber/grabbercontroller.h"

#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>


class CameraControllerDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Constructor
     */
    CameraControllerDialog(Frontend *f, GrabberController *controller, QWidget *parent = 0);

    /**
     * Initialize the dialog
     */
    void init();

private slots:
    /**
     * Slot for notified the camera controller when the brightness changes,
     * so that brightness can be updated.
     * @param index the new brightness value.
     */
    void changeBrightness(int index);

    /**
     * Slot for notified the camera controller when the contrast changes,
     * so that contrast can be updated.
     * @param index the new contrast value.
     */
    void changeContrast(int index);

    /**
     * Slot for notified the camera controller when the saturation changes,
     * so that saturation can be updated.
     * @param index the new saturation value.
     */
    void changeSaturation(int index);

    /**
     * Slot for notified the camera controller when the hue changes,
     * so that hue can be updated.
     * @param index the new hue value.
     */
    void changeHue(int index);

    /**
     * Slot for notified the camera controller when the gamma changes,
     * so that gamma can be updated.
     * @param index the new gamma value.
     */
    void changeGamma(int index);

    /**
     * Slot for notified the camera controller when the sharpness changes,
     * so that sharpness can be updated.
     * @param index the new sharpness value.
     */
    void changeSharpness(int index);

    /**
     * Slot for notified the camera controller when the backlight compensation changes,
     * so that backlight compensation can be updated.
     * @param index the new backlight compensation value.
     */
    void changeBacklight(int index);

    /**
     * Slot for notified the camera contoller when automatic exposure is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     */
    void changeAutoExposure(int newState);

    /**
     * Slot for notified the camera controller when the exposure changes,
     * so that exposure can be updated.
     * @param index the new exposure value.
     */
    void changeExposure(int index);

    /**
     * Slot for notified the camera contoller when automatic white balance is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     */
    void changeAutoWhite(int newState);

    /**
     * Slot for notified the camera controller when the white balance changes,
     * so that white balance can be updated.
     * @param index the new white balance value.
     */
    void changeWhite(int index);

    /**
     * Slot for notified the camera contoller when automatic zoom is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     */
    void changeAutoZoom(int newState);

    /**
     * Slot for notified the camera controller when the zoom changes,
     * so that zoom can be updated.
     * @param index the new zoo value.
     */
    void changeZoom(int index);

    /**
     * Slot for notified the camera contoller when automatic focus is changed, so that
     * camera contoller can be updaten.
     * @param newStateb The new state of the check box
     */
    void changeAutoFocus(int newState);

    /**
     * Slot for notified the camera controller when the focus changes,
     * so that focus can be updated.
     * @param index the new focus value.
     */
    void changeFocus(int index);

    /**
     * Slot for notified the camera controller when the pan changes,
     * so that pan can be updated.
     * @param index the new pan value.
     */
    void changePan(int index);

    /**
     * Slot for notified the camera controller when the tilt changes,
     * so that tilt can be updated.
     * @param index the new tilt value.
     */
    void changeTilt(int index);

private:
    /**
     * Fill the combobox with the values.
     * @param comboBox The combo box to fill.
     * @param controlCaps The control capabilities.
     * @return The step lengt for the values of the combo box.
     */
    int fillComboBox(QComboBox *comboBox, GrabberControlCapabilities *controlCaps);

    Frontend     *frontend;

    GrabberController *grabberController;

    QLabel       *brightnessLabel;
    QComboBox    *brightnessComboBox;
    int           stepBrightness;

    QLabel       *contrastLabel;
    QComboBox    *contrastComboBox;
    int           stepContrast;

    QLabel       *saturationLabel;
    QComboBox    *saturationComboBox;
    int           stepSaturation;

    QLabel       *hueLabel;
    QComboBox    *hueComboBox;
    int           stepHue;

    QLabel       *gammaLabel;
    QComboBox    *gammaComboBox;
    int           stepGamma;

    QLabel       *sharpnessLabel;
    QComboBox    *sharpnessComboBox;
    int           stepSharpness;

    QLabel       *backlightLabel;
    QComboBox    *backlightComboBox;
    int           stepBacklight;

    QCheckBox    *exposureCheckBox;
    QLabel       *exposureLabel;
    QComboBox    *exposureComboBox;
    int           stepExposure;

    QCheckBox    *whiteCheckBox;
    QLabel       *whiteLabel;
    QComboBox    *whiteComboBox;
    int           stepWhite;

    QCheckBox    *zoomCheckBox;
    QLabel       *zoomLabel;
    QComboBox    *zoomComboBox;
    int           stepZoom;

    QCheckBox    *focusCheckBox;
    QLabel       *focusLabel;
    QComboBox    *focusComboBox;
    int           stepFocus;

    QLabel       *panLabel;
    QComboBox    *panComboBox;
    int           stepPan;

    QLabel       *tiltLabel;
    QComboBox    *tiltComboBox;
    int           stepTilt;

    QPushButton  *closeButton;
};

#endif
