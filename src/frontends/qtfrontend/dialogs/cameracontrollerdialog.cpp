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

#include "cameracontrollerdialog.h"
#include "technical/preferencestool.h"

#include <QtCore/QtDebug>
#include <QtCore/QUrl>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


CameraControllerDialog::CameraControllerDialog(Frontend *f,
                                               ImageGrabberDevice *device,
                                               QWidget *parent)
    : QDialog(parent)
{
    qDebug("CameraControllerDialog::Constructor --> Start");

    frontend = f;
    grabberDevice = device;
    deviceId = grabberDevice->getDeviceId();
    deviceId.remove(QChar(' '));   // Remove all spaces
    grabberController = device->getController();
    stepBrightness = -1;
    stepContrast = -1;
    stepSaturation = -1;
    stepHue = -1;
    stepGamma = -1;
    stepSharpness = -1;
    stepBacklight = -1;
    stepWhite = -1;
    stepGain = -1;
    stepColor = -1;
    stepExposure = -1;
    stepZoom = -1;
    stepFocus = -1;
    stepPan = -1;
    stepTilt = -1;
    stepIris = -1;
    stepRoll = -1;

    this->setWindowTitle(tr("qStopMotion Camera Controller"));
    this->setMinimumSize(200, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    qualityGroupBox = new QGroupBox(tr("Video Quality"));
    QVBoxLayout *qualityLayout = new QVBoxLayout;
    qualityGroupBox->setLayout(qualityLayout);

    brightnessCheckBox = new QCheckBox(tr("Automatic Pan"));
    brightnessCheckBox->setChecked(false);
    connect(brightnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBrightness(int)));
    brightnessCheckBox->hide();

    brightnessLabel = new QLabel(tr("Brightness:"));
    brightnessComboBox = new QComboBox();
    connect(brightnessComboBox, SIGNAL(activated(int)), this, SLOT(changeBrightness(int)));
    brightnessLabel->hide();
    brightnessComboBox->hide();

    contrastCheckBox = new QCheckBox(tr("Automatic Contrast"));
    contrastCheckBox->setChecked(false);
    connect(contrastCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoContrast(int)));
    contrastCheckBox->hide();

    contrastLabel = new QLabel(tr("Contrast:"));
    contrastComboBox = new QComboBox();
    connect(contrastComboBox, SIGNAL(activated(int)), this, SLOT(changeContrast(int)));
    contrastLabel->hide();
    contrastComboBox->hide();

    saturationCheckBox = new QCheckBox(tr("Automatic Saturation"));
    saturationCheckBox->setChecked(false);
    connect(saturationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSaturation(int)));
    saturationCheckBox->hide();

    saturationLabel = new QLabel(tr("Saturation:"));
    saturationComboBox = new QComboBox();
    connect(saturationComboBox, SIGNAL(activated(int)), this, SLOT(changeSaturation(int)));
    saturationLabel->hide();
    saturationComboBox->hide();

    hueCheckBox = new QCheckBox(tr("Automatic Hue"));
    hueCheckBox->setChecked(false);
    connect(hueCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoHue(int)));
    hueCheckBox->hide();

    hueLabel = new QLabel(tr("Hue:"));
    hueComboBox = new QComboBox();
    connect(hueComboBox, SIGNAL(activated(int)), this, SLOT(changeHue(int)));
    hueLabel->hide();
    hueComboBox->hide();

    gammaCheckBox = new QCheckBox(tr("Automatic Gamma"));
    gammaCheckBox->setChecked(false);
    connect(gammaCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGamma(int)));
    gammaCheckBox->hide();

    gammaLabel = new QLabel(tr("Gamma:"));
    gammaComboBox = new QComboBox();
    connect(gammaComboBox, SIGNAL(activated(int)), this, SLOT(changeGamma(int)));
    gammaLabel->hide();
    gammaComboBox->hide();

    sharpnessCheckBox = new QCheckBox(tr("Automatic Sharpness"));
    sharpnessCheckBox->setChecked(false);
    connect(sharpnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSharpness(int)));
    sharpnessCheckBox->hide();

    sharpnessLabel = new QLabel(tr("Sharpness:"));
    sharpnessComboBox = new QComboBox();
    connect(sharpnessComboBox, SIGNAL(activated(int)), this, SLOT(changeSharpness(int)));
    sharpnessLabel->hide();
    sharpnessComboBox->hide();

    backlightCheckBox = new QCheckBox(tr("Automatic Backlight Compensation"));
    backlightCheckBox->setChecked(false);
    connect(backlightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBacklight(int)));
    backlightCheckBox->hide();

    backlightLabel = new QLabel(tr("Backlight Compensation:"));
    backlightComboBox = new QComboBox();
    connect(backlightComboBox, SIGNAL(activated(int)), this, SLOT(changeBacklight(int)));
    backlightLabel->hide();
    backlightComboBox->hide();

    whiteCheckBox = new QCheckBox(tr("Automatic White Balance"));
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));
    whiteCheckBox->hide();

    whiteLabel = new QLabel(tr("White Balance:"));
    whiteComboBox = new QComboBox();
    connect(whiteComboBox, SIGNAL(activated(int)), this, SLOT(changeWhite(int)));
    whiteLabel->hide();
    whiteComboBox->hide();

    gainCheckBox = new QCheckBox(tr("Automatic Gain"));
    gainCheckBox->setChecked(false);
    connect(gainCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGain(int)));
    gainCheckBox->hide();

    gainLabel = new QLabel(tr("Gain:"));
    gainComboBox = new QComboBox();
    connect(gainComboBox, SIGNAL(activated(int)), this, SLOT(changeGain(int)));
    gainLabel->hide();
    gainComboBox->hide();

    colorCheckBox = new QCheckBox(tr("Automatic Color Enable"));
    colorCheckBox->setChecked(false);
    connect(colorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoColor(int)));
    colorCheckBox->hide();

    colorLabel = new QLabel(tr("Color Enable:"));
    colorComboBox = new QComboBox();
    connect(colorComboBox, SIGNAL(activated(int)), this, SLOT(changeColor(int)));
    colorLabel->hide();
    colorComboBox->hide();

    controlGroupBox = new QGroupBox(tr("Camera Control"));
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlGroupBox->setLayout(controlLayout);

    exposureCheckBox = new QCheckBox(tr("Automatic Exposure"));
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));
    exposureCheckBox->hide();

    exposureLabel = new QLabel(tr("Exposure:"));
    exposureComboBox = new QComboBox();
    connect(exposureComboBox, SIGNAL(activated(int)), this, SLOT(changeExposure(int)));
    exposureLabel->hide();
    exposureComboBox->hide();

    zoomCheckBox = new QCheckBox(tr("Automatic Zoom"));
    zoomCheckBox->setChecked(false);
    connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoZoom(int)));
    zoomCheckBox->hide();

    zoomLabel = new QLabel(tr("Zoom:"));
    zoomComboBox = new QComboBox();
    connect(zoomComboBox, SIGNAL(activated(int)), this, SLOT(changeZoom(int)));
    zoomLabel->hide();
    zoomComboBox->hide();

    focusCheckBox = new QCheckBox(tr("Automatic Focus"));
    focusCheckBox->setChecked(false);
    connect(focusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoFocus(int)));
    focusCheckBox->hide();

    focusLabel = new QLabel(tr("Focus:"));
    focusComboBox = new QComboBox();
    connect(focusComboBox, SIGNAL(activated(int)), this, SLOT(changeFocus(int)));
    focusLabel->hide();
    focusComboBox->hide();

    panCheckBox = new QCheckBox(tr("Automatic Pan"));
    panCheckBox->setChecked(false);
    connect(panCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoPan(int)));
    panCheckBox->hide();

    panLabel = new QLabel(tr("Pan:"));
    panComboBox = new QComboBox();
    connect(panComboBox, SIGNAL(activated(int)), this, SLOT(changePan(int)));
    panLabel->hide();
    panComboBox->hide();

    tiltCheckBox = new QCheckBox(tr("Automatic Tilt"));
    tiltCheckBox->setChecked(false);
    connect(tiltCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoTilt(int)));
    tiltCheckBox->hide();

    tiltLabel = new QLabel(tr("Tilt:"));
    tiltComboBox = new QComboBox();
    connect(tiltComboBox, SIGNAL(activated(int)), this, SLOT(changeTilt(int)));
    tiltLabel->hide();
    tiltComboBox->hide();

    irisCheckBox = new QCheckBox(tr("Automatic Iris"));
    irisCheckBox->setChecked(false);
    connect(irisCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoIris(int)));
    irisCheckBox->hide();

    irisLabel = new QLabel(tr("Iris:"));
    irisComboBox = new QComboBox();
    connect(irisComboBox, SIGNAL(activated(int)), this, SLOT(changeIris(int)));
    irisLabel->hide();
    irisComboBox->hide();

    rollCheckBox = new QCheckBox(tr("Automatic Roll"));
    rollCheckBox->setChecked(false);
    connect(rollCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoRoll(int)));
    rollCheckBox->hide();

    rollLabel = new QLabel(tr("Roll:"));
    rollComboBox = new QComboBox();
    connect(rollComboBox, SIGNAL(activated(int)), this, SLOT(changeRoll(int)));
    rollLabel->hide();
    rollComboBox->hide();

    closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addLayout(topLayout);

    controlLayout->addWidget(brightnessCheckBox);
    qualityLayout->addWidget(brightnessLabel);
    qualityLayout->addWidget(brightnessComboBox);
    controlLayout->addWidget(contrastCheckBox);
    qualityLayout->addWidget(contrastLabel);
    qualityLayout->addWidget(contrastComboBox);
    controlLayout->addWidget(saturationCheckBox);
    qualityLayout->addWidget(saturationLabel);
    qualityLayout->addWidget(saturationComboBox);
    controlLayout->addWidget(hueCheckBox);
    qualityLayout->addWidget(hueLabel);
    qualityLayout->addWidget(hueComboBox);
    controlLayout->addWidget(gammaCheckBox);
    qualityLayout->addWidget(gammaLabel);
    qualityLayout->addWidget(gammaComboBox);
    controlLayout->addWidget(sharpnessCheckBox);
    qualityLayout->addWidget(sharpnessLabel);
    qualityLayout->addWidget(sharpnessComboBox);
    controlLayout->addWidget(backlightCheckBox);
    qualityLayout->addWidget(backlightLabel);
    qualityLayout->addWidget(backlightComboBox);
    qualityLayout->addWidget(whiteCheckBox);
    qualityLayout->addWidget(whiteLabel);
    qualityLayout->addWidget(whiteComboBox);
    qualityLayout->addWidget(gainCheckBox);
    qualityLayout->addWidget(gainLabel);
    qualityLayout->addWidget(gainComboBox);
    qualityLayout->addWidget(colorCheckBox);
    qualityLayout->addWidget(colorLabel);
    qualityLayout->addWidget(colorComboBox);

    controlLayout->addWidget(exposureCheckBox);
    controlLayout->addWidget(exposureLabel);
    controlLayout->addWidget(exposureComboBox);
    controlLayout->addWidget(zoomCheckBox);
    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(zoomComboBox);
    controlLayout->addWidget(focusCheckBox);
    controlLayout->addWidget(focusLabel);
    controlLayout->addWidget(focusComboBox);
    controlLayout->addWidget(panCheckBox);
    controlLayout->addWidget(panLabel);
    controlLayout->addWidget(panComboBox);
    controlLayout->addWidget(tiltCheckBox);
    controlLayout->addWidget(tiltLabel);
    controlLayout->addWidget(tiltComboBox);
    controlLayout->addWidget(irisCheckBox);
    controlLayout->addWidget(irisLabel);
    controlLayout->addWidget(irisComboBox);
    controlLayout->addWidget(rollCheckBox);
    controlLayout->addWidget(rollLabel);
    controlLayout->addWidget(rollComboBox);

    mainLayout->addWidget(qualityGroupBox);
    mainLayout->addWidget(controlGroupBox);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug("CameraControllerDialog::Constructor --> End");
}


void CameraControllerDialog::init()
{
    qDebug() << "CameraControllerDialog::init --> Start";

    PreferencesTool            *preferences = frontend->getPreferences();
    GrabberControlCapabilities *capabilities;
    bool                        checked;
    int                         value;

    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticbrightness", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        brightnessCheckBox->setChecked(checked);
        changeAutoBrightness(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            brightnessLabel->show();
        }
        brightnessComboBox->show();
        stepBrightness = fillComboBox(brightnessComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "brigthtness", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        brightnessComboBox->setCurrentIndex(value);
        changeBrightness(value, false);
    }

    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticcontrast", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        contrastCheckBox->setChecked(checked);
        changeAutoContrast(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            contrastLabel->show();
        }
        contrastComboBox->show();
        stepContrast = fillComboBox(contrastComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "contrast", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        contrastComboBox->setCurrentIndex(value);
        changeContrast(value, false);
    }

    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticsaturation", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        saturationCheckBox->setChecked(checked);
        changeAutoSaturation(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            saturationLabel->show();
        }
        saturationComboBox->show();
        stepSaturation = fillComboBox(saturationComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "saturation", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        saturationComboBox->setCurrentIndex(value);
        changeSaturation(value, false);
    }

    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automatichue", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        hueCheckBox->setChecked(checked);
        changeAutoHue(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            hueLabel->show();
        }
        hueComboBox->show();
        stepHue = fillComboBox(hueComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "hue", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        hueComboBox->setCurrentIndex(value);
        changeHue(value, false);
    }

    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticgamma", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        gammaCheckBox->setChecked(checked);
        changeAutoGamma(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            gammaLabel->show();
        }
        gammaComboBox->show();
        stepGamma = fillComboBox(gammaComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "gamma", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        gammaComboBox->setCurrentIndex(value);
    }

    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticsharpness", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        sharpnessCheckBox->setChecked(checked);
        changeAutoSharpness(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            sharpnessLabel->show();
        }
        sharpnessComboBox->show();
        stepSharpness = fillComboBox(sharpnessComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "sharpness", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        sharpnessComboBox->setCurrentIndex(value);
        changeSharpness(value, false);
    }

    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticbacklight", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        backlightCheckBox->setChecked(checked);
        changeAutoBacklight(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            backlightLabel->show();
        }
        backlightComboBox->show();
        stepBacklight = fillComboBox(backlightComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "backlight", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        backlightComboBox->setCurrentIndex(value);
        changeBacklight(value, false);
    }

    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticwhite", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        whiteCheckBox->setChecked(checked);
        changeAutoWhite(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            whiteLabel->show();
        }
        whiteComboBox->show();
        stepWhite = fillComboBox(whiteComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "white", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        whiteComboBox->setCurrentIndex(value);
        changeWhite(value, false);
    }

    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticgain", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        gainCheckBox->setChecked(checked);
        changeAutoGain(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            gainLabel->show();
        }
        gainComboBox->show();
        stepGain = fillComboBox(gainComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "gain", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        gainComboBox->setCurrentIndex(value);
        changeGain(value, false);
    }

    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticcolor", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        colorCheckBox->setChecked(checked);
        changeAutoColor(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            colorLabel->show();
        }
        colorComboBox->show();
        stepColor = fillComboBox(colorComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "color", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        colorComboBox->setCurrentIndex(value);
        changeColor(value, false);
    }

    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticexposure", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        exposureCheckBox->setChecked(checked);
        changeAutoExposure(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            exposureLabel->show();
        }
        exposureComboBox->show();
        stepExposure = fillComboBox(exposureComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "exposure", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        exposureComboBox->setCurrentIndex(value);
        changeExposure(value, false);
    }

    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticzoom", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        zoomCheckBox->setChecked(checked);
        changeAutoZoom(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            zoomLabel->show();
        }
        zoomComboBox->show();
        stepZoom = fillComboBox(zoomComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "zoom", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        zoomComboBox->setCurrentIndex(value);
        changeZoom(value, false);
    }

    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticfocus", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        focusCheckBox->setChecked(checked);
        changeAutoFocus(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            focusLabel->show();
        }
        focusComboBox->show();
        stepFocus = fillComboBox(focusComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "focus", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        focusComboBox->setCurrentIndex(value);
        changeFocus(value, false);
    }

    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticpan", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        panCheckBox->setChecked(checked);
        changeAutoPan(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            panLabel->show();
        }
        panLabel->show();
        panComboBox->show();
        stepPan = fillComboBox(panComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "pan", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        panComboBox->setCurrentIndex(value);
        changePan(value, false);
    }

    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automatictilt", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        tiltCheckBox->setChecked(checked);
        changeAutoTilt(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            tiltLabel->show();
        }
        tiltLabel->show();
        tiltComboBox->show();
        stepTilt = fillComboBox(tiltComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "tilt", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        tiltComboBox->setCurrentIndex(value);
        changeTilt(value, false);
    }

    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticiris", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        irisCheckBox->setChecked(checked);
        changeAutoIris(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            irisLabel->show();
        }
        irisComboBox->show();
        stepIris = fillComboBox(irisComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "iris", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        irisComboBox->setCurrentIndex(value);
        changeIris(value, false);
    }

    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->show();
        if (preferences->getIntegerPreference(deviceId, "automaticroll", value)) {
            if (value == 1) {
                checked = true;
            }
            else {
                checked = false;
            }
        }
        else {
            checked = false;
        }
        rollCheckBox->setChecked(checked);
        changeAutoRoll(checked, false);
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            rollLabel->show();
        }
        rollComboBox->show();
        stepRoll = fillComboBox(rollComboBox, capabilities);
        if (preferences->getIntegerPreference(deviceId, "roll", value) == false) {
            // Calculate default value
            value = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        }
        rollComboBox->setCurrentIndex(value);
        changeRoll(value, false);
    }

    qDebug() << "CameraControllerDialog::init --> End";
}


void CameraControllerDialog::changeAutoBrightness(int newState)
{
    changeAutoBrightness(newState, true);
}


void CameraControllerDialog::changeAutoBrightness(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        brightnessComboBox->setEnabled(false);
        grabberController->setAutomaticBrightness(true);
    }
    else {
        brightnessComboBox->setEnabled(true);
        grabberController->setAutomaticBrightness(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticbrightness", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoBrightness --> End";
}


void CameraControllerDialog::changeBrightness(int index)
{
    changeBrightness(index, true);
}


void CameraControllerDialog::changeBrightness(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    long value = index * stepBrightness;
    long maxValue = grabberController->getBrightnessCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBrightness(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "brigthtness", index);
    }

    qDebug() << "CameraControllerDialog::changeBrightness --> End";
}


void CameraControllerDialog::changeAutoContrast(int newState)
{
    changeAutoContrast(newState, true);
}


void CameraControllerDialog::changeAutoContrast(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoContrast --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        contrastComboBox->setEnabled(false);
        grabberController->setAutomaticContrast(true);
    }
    else {
        contrastComboBox->setEnabled(true);
        grabberController->setAutomaticContrast(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticcontrast", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoContrast --> End";
}


void CameraControllerDialog::changeContrast(int index)
{
    changeContrast(index, true);
}


void CameraControllerDialog::changeContrast(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeContrast --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepContrast;
    int maxValue = grabberController->getContrastCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setContrast(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "contrast", index);
    }

    qDebug() << "CameraControllerDialog::changeContrast --> End";
}


void CameraControllerDialog::changeAutoSaturation(int newState)
{
    changeAutoSaturation(newState, true);
}


void CameraControllerDialog::changeAutoSaturation(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoSaturation --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        saturationComboBox->setEnabled(false);
        grabberController->setAutomaticSaturation(true);
    }
    else {
        saturationComboBox->setEnabled(true);
        grabberController->setAutomaticSaturation(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticsaturation", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoSaturation --> End";
}


void CameraControllerDialog::changeSaturation(int index)
{
    changeSaturation(index, true);
}


void CameraControllerDialog::changeSaturation(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeSaturation --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSaturation;
    int maxValue = grabberController->getSaturationCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setSaturation(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "saturation", index);
    }

    qDebug() << "CameraControllerDialog::changeSaturation --> End";
}


void CameraControllerDialog::changeAutoHue(int newState)
{
    changeAutoHue(newState, true);
}


void CameraControllerDialog::changeAutoHue(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoHue --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        hueComboBox->setEnabled(false);
        grabberController->setAutomaticHue(true);
    }
    else {
        hueComboBox->setEnabled(true);
        grabberController->setAutomaticHue(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automatichue", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoHue --> End";
}


void CameraControllerDialog::changeHue(int index)
{
    changeHue(index, true);
}


void CameraControllerDialog::changeHue(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeHue --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepHue;
    int maxValue = grabberController->getHueCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setHue(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "hue", index);
    }

    qDebug() << "CameraControllerDialog::changeHue --> End";
}


void CameraControllerDialog::changeAutoGamma(int newState)
{
    changeAutoGamma(newState, true);
}


void CameraControllerDialog::changeAutoGamma(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoGamma --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        gammaComboBox->setEnabled(false);
        grabberController->setAutomaticGamma(true);
    }
    else {
        gammaComboBox->setEnabled(true);
        grabberController->setAutomaticGamma(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticgamma", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoGamma --> End";
}


void CameraControllerDialog::changeGamma(int index)
{
    changeGamma(index, true);
}


void CameraControllerDialog::changeGamma(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeGamma --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepGamma;
    int maxValue = grabberController->getGammaCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setGamma(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "gamma", index);
    }

    qDebug() << "CameraControllerDialog::changeGamma --> End";
}


void CameraControllerDialog::changeAutoSharpness(int newState)
{
    changeAutoSharpness(newState, true);
}


void CameraControllerDialog::changeAutoSharpness(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoSharpness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        sharpnessComboBox->setEnabled(false);
        grabberController->setAutomaticSharpness(true);
    }
    else {
        sharpnessComboBox->setEnabled(true);
        grabberController->setAutomaticSharpness(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticsharpness", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoSharpness --> End";
}


void CameraControllerDialog::changeSharpness(int index)
{
    changeSharpness(index, true);
}


void CameraControllerDialog::changeSharpness(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeSharpness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSharpness;
    int maxValue = grabberController->getSharpnessCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setSharpness(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "sharpness", index);
    }

    qDebug() << "CameraControllerDialog::changeSharpness --> End";
}


void CameraControllerDialog::changeAutoBacklight(int newState)
{
    changeAutoBacklight(newState, true);
}


void CameraControllerDialog::changeAutoBacklight(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoBacklight --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        backlightComboBox->setEnabled(false);
        grabberController->setAutomaticBacklight(true);
    }
    else {
        backlightComboBox->setEnabled(true);
        grabberController->setAutomaticBacklight(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticbacklight", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoBacklight --> End";
}


void CameraControllerDialog::changeBacklight(int index)
{
    changeBacklight(index, true);
}


void CameraControllerDialog::changeBacklight(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeBacklight --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepBacklight;
    int maxValue = grabberController->getBacklightCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBacklight(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "backlight", index);
    }

    qDebug() << "CameraControllerDialog::changeBacklight --> End";
}


void CameraControllerDialog::changeAutoWhite(int newState)
{
    changeAutoWhite(newState, true);
}


void CameraControllerDialog::changeAutoWhite(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoWhite --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        whiteComboBox->setEnabled(false);
        grabberController->setAutomaticWhite(true);
    }
    else {
        whiteComboBox->setEnabled(true);
        grabberController->setAutomaticWhite(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticwhite", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoWhite --> End";
}


void CameraControllerDialog::changeWhite(int index)
{
    changeWhite(index, true);
}


void CameraControllerDialog::changeWhite(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeWhite --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepWhite;
    int maxValue = grabberController->getWhiteCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setWhite(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "white", index);
    }

    qDebug() << "CameraControllerDialog::changeWhite --> End";
}


void CameraControllerDialog::changeAutoGain(int newState)
{
    changeAutoGain(newState, true);
}


void CameraControllerDialog::changeAutoGain(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoGain --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        gainComboBox->setEnabled(false);
        grabberController->setAutomaticGain(true);
    }
    else {
        gainComboBox->setEnabled(true);
        grabberController->setAutomaticGain(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticgain", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoGain --> End";
}


void CameraControllerDialog::changeGain(int index)
{
    changeGain(index, true);
}


void CameraControllerDialog::changeGain(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeGain --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepGain;
    int maxValue = grabberController->getGainCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setGain(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "gain", index);
    }

    qDebug() << "CameraControllerDialog::changeGain --> End";
}


void CameraControllerDialog::changeAutoColor(int newState)
{
    changeAutoColor(newState, true);
}


void CameraControllerDialog::changeAutoColor(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoColor --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        colorComboBox->setEnabled(false);
        grabberController->setAutomaticColor(true);
    }
    else {
        colorComboBox->setEnabled(true);
        grabberController->setAutomaticColor(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticcolor", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoColor --> End";
}


void CameraControllerDialog::changeColor(int index)
{
    changeColor(index, true);
}


void CameraControllerDialog::changeColor(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeColor --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepColor;
    int maxValue = grabberController->getColorCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setColor(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "color", index);
    }

    qDebug() << "CameraControllerDialog::changeColor --> End";
}


void CameraControllerDialog::changeAutoExposure(int newState)
{
    changeAutoExposure(newState, true);
}


void CameraControllerDialog::changeAutoExposure(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoExposure --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        exposureComboBox->setEnabled(false);
        grabberController->setAutomaticExposure(true);
    }
    else {
        exposureComboBox->setEnabled(true);
        grabberController->setAutomaticExposure(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticexposure", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoExposure --> End";
}


void CameraControllerDialog::changeExposure(int index)
{
    changeExposure(index, true);
}


void CameraControllerDialog::changeExposure(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeExposure --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepExposure;
    int maxValue = grabberController->getExposureCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setExposure(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "exposure", index);
    }

    qDebug() << "CameraControllerDialog::changeExposure --> End";
}


void CameraControllerDialog::changeAutoZoom(int newState)
{
    changeAutoZoom(newState, true);
}


void CameraControllerDialog::changeAutoZoom(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoZoo --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        zoomComboBox->setEnabled(false);
        grabberController->setAutomaticZoom(true);
    }
    else {
        zoomComboBox->setEnabled(true);
        grabberController->setAutomaticZoom(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticzoom", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoZoo --> End";
}


void CameraControllerDialog::changeZoom(int index)
{
    changeZoom(index, true);
}


void CameraControllerDialog::changeZoom(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeZoo --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepZoom;
    int maxValue = grabberController->getZoomCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setZoom(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "zoom", index);
    }

    qDebug() << "CameraControllerDialog::changeZoo --> End";
}


void CameraControllerDialog::changeAutoFocus(int newState)
{
    changeAutoFocus(newState, true);
}


void CameraControllerDialog::changeAutoFocus(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoFocus --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        focusComboBox->setEnabled(false);
        grabberController->setAutomaticFocus(true);
    }
    else {
        focusComboBox->setEnabled(true);
        grabberController->setAutomaticFocus(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticfocus", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoFocus --> End";
}


void CameraControllerDialog::changeFocus(int index)
{
    changeFocus(index, true);
}


void CameraControllerDialog::changeFocus(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeFocus --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepFocus;
    int maxValue = grabberController->getFocusCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setFocus(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "focus", index);
    }

    qDebug() << "CameraControllerDialog::changeFocus --> End";
}


void CameraControllerDialog::changeAutoPan(int newState)
{
    changeAutoPan(newState, true);
}


void CameraControllerDialog::changeAutoPan(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoPan --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        panComboBox->setEnabled(false);
        grabberController->setAutomaticPan(true);
    }
    else {
        panComboBox->setEnabled(true);
        grabberController->setAutomaticPan(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticpan", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoPan --> End";
}


void CameraControllerDialog::changePan(int index)
{
    changePan(index, true);
}


void CameraControllerDialog::changePan(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changePan --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepPan;
    int maxValue = grabberController->getPanCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setPan(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "pan", index);
    }

    qDebug() << "CameraControllerDialog::changePan --> End";
}


void CameraControllerDialog::changeAutoTilt(int newState)
{
    changeAutoTilt(newState, true);
}


void CameraControllerDialog::changeAutoTilt(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoTilt --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        tiltComboBox->setEnabled(false);
        grabberController->setAutomaticTilt(true);
    }
    else {
        tiltComboBox->setEnabled(true);
        grabberController->setAutomaticTilt(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automatictilt", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoTilt --> End";
}


void CameraControllerDialog::changeTilt(int index)
{
    changeTilt(index, true);
}


void CameraControllerDialog::changeTilt(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeTilt --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepTilt;
    int maxValue = grabberController->getTiltCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setTilt(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "tilt", index);
    }

    qDebug() << "CameraControllerDialog::changeTilt --> End";
}


void CameraControllerDialog::changeAutoIris(int newState)
{
    changeAutoIris(newState, true);
}


void CameraControllerDialog::changeAutoIris(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoIris --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        irisComboBox->setEnabled(false);
        grabberController->setAutomaticIris(true);
    }
    else {
        irisComboBox->setEnabled(true);
        grabberController->setAutomaticIris(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticiris", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoIris --> End";
}


void CameraControllerDialog::changeIris(int index)
{
    changeIris(index, true);
}


void CameraControllerDialog::changeIris(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeIris --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepIris;
    int maxValue = grabberController->getIrisCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setIris(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "iris", index);
    }

    qDebug() << "CameraControllerDialog::changeIris --> End";
}


void CameraControllerDialog::changeAutoRoll(int newState)
{
    changeAutoRoll(newState, true);
}


void CameraControllerDialog::changeAutoRoll(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoRoll --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        rollComboBox->setEnabled(false);
        grabberController->setAutomaticRoll(true);
    }
    else {
        rollComboBox->setEnabled(true);
        grabberController->setAutomaticRoll(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticroll", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoRoll --> End";
}


void CameraControllerDialog::changeRoll(int index)
{
    changeRoll(index, true);
}


void CameraControllerDialog::changeRoll(int index, bool save)
{
    qDebug() << "CameraControllerDialog::changeRoll --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepRoll;
    int maxValue = grabberController->getRollCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setRoll(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "roll", index);
    }

    qDebug() << "CameraControllerDialog::changeRoll --> End";
}


int CameraControllerDialog::fillComboBox(QComboBox *comboBox, GrabberControlCapabilities *controlCaps)
{
    int value;

    for (value = controlCaps->getMinimum() ; value <= controlCaps->getMaximum() ; value += controlCaps->getStep()) {
        comboBox->addItem(QString("%1").arg(value));
    }
    if (value < controlCaps->getMaximum()) {
        comboBox->addItem(QString("%1").arg(controlCaps->getMaximum()));
    }

    return controlCaps->getStep();
}
