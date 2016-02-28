/******************************************************************************
 *  Copyright (C) 2005-2016 by                                                *
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

#include <QDebug>
#include <QHBoxLayout>
#include <QProgressDialog>
#include <QUrl>

#include "technical/preferencestool.h"


CameraControllerDialog::CameraControllerDialog(Frontend *f,
                                               ImageGrabberDevice *device,
                                               QWidget *parent)
    : QDialog(parent)
{
    qDebug() << "CameraControllerDialog::Constructor --> Start";

    frontend = f;
    grabberDevice = device;
    deviceId = grabberDevice->getDeviceId();
    deviceId.remove(QLatin1Char(' '));   // Remove all spaces (Windows)
    deviceId.remove(QLatin1Char('/'));   // Remove all slashs (Linux)
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

    makeGUI();
    retranslateStrings();

    qDebug() << "CameraControllerDialog::Constructor --> End";
}


void CameraControllerDialog::makeGUI()
{
    qDebug() << "CameraControllerDialog::makeGUI --> Start";


    setWindowTitle("windowTitle");
    setMinimumSize(200, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    qualityGroupBox = new QGroupBox("qualityGroupBox");
    QVBoxLayout *qualityLayout = new QVBoxLayout;
    qualityGroupBox->setLayout(qualityLayout);
    qualityCount = 0;

    brightnessCheckBox = new QCheckBox("brightnessCheckBox");
    brightnessCheckBox->setChecked(false);
    connect(brightnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBrightness(int)));
    brightnessCheckBox->hide();

    brightnessLabel = new QLabel("brightnessLabel");
    brightnessSlider = new QSlider(Qt::Horizontal);
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness(int)));
    brightnessLabel->hide();
    brightnessSlider->hide();

    contrastCheckBox = new QCheckBox("contrastCheckBox");
    contrastCheckBox->setChecked(false);
    connect(contrastCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoContrast(int)));
    contrastCheckBox->hide();

    contrastLabel = new QLabel("contrastLabel");
    contrastSlider = new QSlider(Qt::Horizontal);
    connect(contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
    contrastLabel->hide();
    contrastSlider->hide();

    saturationCheckBox = new QCheckBox("saturationCheckBox");
    saturationCheckBox->setChecked(false);
    connect(saturationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSaturation(int)));
    saturationCheckBox->hide();

    saturationLabel = new QLabel("saturationLabel");
    saturationSlider = new QSlider(Qt::Horizontal);
    connect(saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSaturation(int)));
    saturationLabel->hide();
    saturationSlider->hide();

    hueCheckBox = new QCheckBox("hueCheckBox");
    hueCheckBox->setChecked(false);
    connect(hueCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoHue(int)));
    hueCheckBox->hide();

    hueLabel = new QLabel("hueLabel");
    hueSlider = new QSlider(Qt::Horizontal);
    connect(hueSlider, SIGNAL(valueChanged(int)), this, SLOT(changeHue(int)));
    hueLabel->hide();
    hueSlider->hide();

    gammaCheckBox = new QCheckBox("gammaCheckBox");
    gammaCheckBox->setChecked(false);
    connect(gammaCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGamma(int)));
    gammaCheckBox->hide();

    gammaLabel = new QLabel("gammaLabel");
    gammaSlider = new QSlider(Qt::Horizontal);
    connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(changeGamma(int)));
    gammaLabel->hide();
    gammaSlider->hide();

    sharpnessCheckBox = new QCheckBox("sharpnessCheckBox");
    sharpnessCheckBox->setChecked(false);
    connect(sharpnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSharpness(int)));
    sharpnessCheckBox->hide();

    sharpnessLabel = new QLabel("sharpnessLabel");
    sharpnessSlider = new QSlider(Qt::Horizontal);
    connect(sharpnessSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSharpness(int)));
    sharpnessLabel->hide();
    sharpnessSlider->hide();

    backlightCheckBox = new QCheckBox("backlightCheckBox");
    backlightCheckBox->setChecked(false);
    connect(backlightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBacklight(int)));
    backlightCheckBox->hide();

    backlightLabel = new QLabel("backlightLabel");
    backlightSlider = new QSlider(Qt::Horizontal);
    connect(backlightSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBacklight(int)));
    backlightLabel->hide();
    backlightSlider->hide();

    whiteCheckBox = new QCheckBox("whiteCheckBox");
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));
    whiteCheckBox->hide();

    whiteLabel = new QLabel("whiteLabel");
    whiteSlider = new QSlider(Qt::Horizontal);
    connect(whiteSlider, SIGNAL(valueChanged(int)), this, SLOT(changeWhite(int)));
    whiteLabel->hide();
    whiteSlider->hide();

    gainCheckBox = new QCheckBox("gainCheckBox");
    gainCheckBox->setChecked(false);
    connect(gainCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGain(int)));
    gainCheckBox->hide();

    gainLabel = new QLabel("gainLabel");
    gainSlider = new QSlider(Qt::Horizontal);
    connect(gainSlider, SIGNAL(valueChanged(int)), this, SLOT(changeGain(int)));
    gainLabel->hide();
    gainSlider->hide();

    colorCheckBox = new QCheckBox("colorCheckBox");
    colorCheckBox->setChecked(false);
    connect(colorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoColor(int)));
    colorCheckBox->hide();

    colorLabel = new QLabel("colorLabel");
    colorSlider = new QSlider(Qt::Horizontal);
    connect(colorSlider, SIGNAL(valueChanged(int)), this, SLOT(changeColor(int)));
    colorLabel->hide();
    colorSlider->hide();

    controlGroupBox = new QGroupBox("controlGroupBox");
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlGroupBox->setLayout(controlLayout);
    controlCount = 0;

    exposureCheckBox = new QCheckBox("exposureCheckBox");
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));
    exposureCheckBox->hide();

    exposureLabel = new QLabel("exposureLabel");
    exposureSlider = new QSlider(Qt::Horizontal);
    connect(exposureSlider, SIGNAL(valueChanged(int)), this, SLOT(changeExposure(int)));
    exposureLabel->hide();
    exposureSlider->hide();

    zoomCheckBox = new QCheckBox("zoomCheckBox");
    zoomCheckBox->setChecked(false);
    connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoZoom(int)));
    zoomCheckBox->hide();

    zoomLabel = new QLabel("zoomLabel");
    zoomSlider = new QSlider(Qt::Horizontal);
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(changeZoom(int)));
    zoomLabel->hide();
    zoomSlider->hide();

    focusCheckBox = new QCheckBox("focusCheckBox");
    focusCheckBox->setChecked(false);
    connect(focusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoFocus(int)));
    focusCheckBox->hide();

    focusLabel = new QLabel("focusLabel");
    focusSlider = new QSlider(Qt::Horizontal);
    connect(focusSlider, SIGNAL(valueChanged(int)), this, SLOT(changeFocus(int)));
    focusLabel->hide();
    focusSlider->hide();

    panCheckBox = new QCheckBox("panCheckBox");
    panCheckBox->setChecked(false);
    connect(panCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoPan(int)));
    panCheckBox->hide();

    panLabel = new QLabel("panLabel");
    panSlider = new QSlider(Qt::Horizontal);
    connect(panSlider, SIGNAL(valueChanged(int)), this, SLOT(changePan(int)));
    panLabel->hide();
    panSlider->hide();

    tiltCheckBox = new QCheckBox("tiltCheckBox");
    tiltCheckBox->setChecked(false);
    connect(tiltCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoTilt(int)));
    tiltCheckBox->hide();

    tiltLabel = new QLabel("tiltLabel");
    tiltSlider = new QSlider(Qt::Horizontal);
    connect(tiltSlider, SIGNAL(valueChanged(int)), this, SLOT(changeTilt(int)));
    tiltLabel->hide();
    tiltSlider->hide();

    irisCheckBox = new QCheckBox("irisCheckBox");
    irisCheckBox->setChecked(false);
    connect(irisCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoIris(int)));
    irisCheckBox->hide();

    irisLabel = new QLabel("irisLabel");
    irisSlider = new QSlider(Qt::Horizontal);
    connect(irisSlider, SIGNAL(valueChanged(int)), this, SLOT(changeIris(int)));
    irisLabel->hide();
    irisSlider->hide();

    rollCheckBox = new QCheckBox("rollCheckBox");
    rollCheckBox->setChecked(false);
    connect(rollCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoRoll(int)));
    rollCheckBox->hide();

    rollLabel = new QLabel("rollLabel");
    rollSlider = new QSlider(Qt::Horizontal);
    connect(rollSlider, SIGNAL(valueChanged(int)), this, SLOT(changeRoll(int)));
    rollLabel->hide();
    rollSlider->hide();

    resetButton = new QPushButton("resetButton");
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));

    closeButton = new QPushButton("closeButton");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(resetButton);
    // bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);

    mainLayout = new QVBoxLayout;
    // mainLayout->addLayout(topLayout);

    qualityLayout->addWidget(brightnessCheckBox);
    qualityLayout->addWidget(brightnessLabel);
    qualityLayout->addWidget(brightnessSlider);
    qualityLayout->addWidget(contrastCheckBox);
    qualityLayout->addWidget(contrastLabel);
    qualityLayout->addWidget(contrastSlider);
    qualityLayout->addWidget(saturationCheckBox);
    qualityLayout->addWidget(saturationLabel);
    qualityLayout->addWidget(saturationSlider);
    qualityLayout->addWidget(hueCheckBox);
    qualityLayout->addWidget(hueLabel);
    qualityLayout->addWidget(hueSlider);
    qualityLayout->addWidget(gammaCheckBox);
    qualityLayout->addWidget(gammaLabel);
    qualityLayout->addWidget(gammaSlider);
    qualityLayout->addWidget(sharpnessCheckBox);
    qualityLayout->addWidget(sharpnessLabel);
    qualityLayout->addWidget(sharpnessSlider);
    qualityLayout->addWidget(backlightCheckBox);
    qualityLayout->addWidget(backlightLabel);
    qualityLayout->addWidget(backlightSlider);
    qualityLayout->addWidget(whiteCheckBox);
    qualityLayout->addWidget(whiteLabel);
    qualityLayout->addWidget(whiteSlider);
    qualityLayout->addWidget(gainCheckBox);
    qualityLayout->addWidget(gainLabel);
    qualityLayout->addWidget(gainSlider);
    qualityLayout->addWidget(colorCheckBox);
    qualityLayout->addWidget(colorLabel);
    qualityLayout->addWidget(colorSlider);
    qualityLayout->addStretch();

    controlLayout->addWidget(exposureCheckBox);
    controlLayout->addWidget(exposureLabel);
    controlLayout->addWidget(exposureSlider);
    controlLayout->addWidget(zoomCheckBox);
    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(zoomSlider);
    controlLayout->addWidget(focusCheckBox);
    controlLayout->addWidget(focusLabel);
    controlLayout->addWidget(focusSlider);
    controlLayout->addWidget(panCheckBox);
    controlLayout->addWidget(panLabel);
    controlLayout->addWidget(panSlider);
    controlLayout->addWidget(tiltCheckBox);
    controlLayout->addWidget(tiltLabel);
    controlLayout->addWidget(tiltSlider);
    controlLayout->addWidget(irisCheckBox);
    controlLayout->addWidget(irisLabel);
    controlLayout->addWidget(irisSlider);
    controlLayout->addWidget(rollCheckBox);
    controlLayout->addWidget(rollLabel);
    controlLayout->addWidget(rollSlider);
    controlLayout->addStretch();

    mainLayout->addWidget(qualityGroupBox);
    mainLayout->addWidget(controlGroupBox);
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug() << "CameraControllerDialog::makeGUI --> End";
}


void CameraControllerDialog::retranslateStrings()
{
    qDebug() << "CameraControllerDialog::retranslateStrings --> Start";

    setWindowTitle(tr("qStopMotion Camera Controller"));

    qualityGroupBox->setTitle(tr("Video Quality"));

    brightnessCheckBox->setText(tr("Automatic Brightness"));
    brightnessLabel->setText(tr("Brightness:"));
    contrastCheckBox->setText(tr("Automatic Contrast"));
    contrastLabel->setText(tr("Contrast:"));
    saturationCheckBox->setText(tr("Automatic Saturation"));
    saturationLabel->setText(tr("Saturation:"));
    hueCheckBox->setText(tr("Automatic Hue"));
    hueLabel->setText(tr("Hue:"));
    gammaCheckBox->setText(tr("Automatic Gamma"));
    gammaLabel->setText(tr("Gamma:"));
    sharpnessCheckBox->setText(tr("Automatic Sharpness"));
    sharpnessLabel->setText(tr("Sharpness:"));
    backlightCheckBox->setText(tr("Automatic Backlight Compensation"));
    backlightLabel->setText(tr("Backlight Compensation:"));
    whiteCheckBox->setText(tr("Automatic White Balance"));
    whiteLabel->setText(tr("White Balance:"));
    gainCheckBox->setText(tr("Automatic Gain"));
    gainLabel->setText(tr("Gain:"));
    colorCheckBox->setText(tr("Automatic Color Enable"));
    colorLabel->setText(tr("Color Enable:"));

    controlGroupBox->setTitle(tr("Camera Control"));

    exposureCheckBox->setText(tr("Automatic Exposure"));
    exposureLabel->setText(tr("Exposure:"));
    zoomCheckBox->setText(tr("Automatic Zoom"));
    zoomLabel->setText(tr("Zoom:"));
    focusCheckBox->setText(tr("Automatic Focus"));
    focusLabel->setText(tr("Focus:"));
    panCheckBox->setText(tr("Automatic Pan"));
    panLabel->setText(tr("Pan:"));
    tiltCheckBox->setText(tr("Automatic Tilt"));
    tiltLabel->setText(tr("Tilt:"));
    irisCheckBox->setText(tr("Automatic Iris"));
    irisLabel->setText(tr("Iris:"));
    rollCheckBox->setText(tr("Automatic Roll"));
    rollLabel->setText(tr("Roll:"));

    resetButton->setText(tr("&Reset to Default"));
    closeButton->setText(tr("&Close"));

    qDebug() << "CameraControllerDialog::retranslateStrings --> End";
}


void CameraControllerDialog::initialize()
{
    qDebug() << "CameraControllerDialog::initialize --> Start";

    PreferencesTool            *preferences = frontend->getPreferences();
    GrabberControlCapabilities *capabilities;
    bool                        checked;
    int                         value;

    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        brightnessCheckBox->show();
        brightnessCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        brightnessLabel->show();
        brightnessSlider->show();
        brightnessSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "brightness", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepBrightness = configureSlider(brightnessSlider, capabilities, value);
    }

    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        contrastCheckBox->show();
        contrastCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        contrastLabel->show();
        contrastSlider->show();
        contrastSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "contrast", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepContrast = configureSlider(contrastSlider, capabilities, value);
    }

    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        saturationCheckBox->show();
        saturationCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        saturationLabel->show();
        saturationSlider->show();
        saturationSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "saturation", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepSaturation = configureSlider(saturationSlider, capabilities, value);
    }

    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        hueCheckBox->show();
        hueCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        hueLabel->show();
        hueSlider->show();
        hueSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "hue", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepHue = configureSlider(hueSlider, capabilities, value);
    }

    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        gammaCheckBox->show();
        gammaCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        gammaLabel->show();
        gammaSlider->show();
        gammaSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "gamma", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepGamma = configureSlider(gammaSlider, capabilities, value);
    }

    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        sharpnessCheckBox->show();
        sharpnessCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        sharpnessLabel->show();
        sharpnessSlider->show();
        sharpnessSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "sharpness", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepSharpness = configureSlider(sharpnessSlider, capabilities, value);
    }

    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        backlightCheckBox->show();
        backlightCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        backlightLabel->show();
        backlightSlider->show();
        backlightSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "backlight", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepBacklight = configureSlider(backlightSlider, capabilities, value);
    }

    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        whiteCheckBox->show();
        whiteCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        whiteLabel->show();
        whiteSlider->show();
        whiteSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "white", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepWhite = configureSlider(whiteSlider, capabilities, value);
    }

    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        gainCheckBox->show();
        gainCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        gainLabel->show();
        gainSlider->show();
        gainSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "gain", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepGain = configureSlider(gainSlider, capabilities, value);
    }

    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        qualityCount++;
        colorCheckBox->show();
        colorCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        qualityCount++;
        colorLabel->show();
        colorSlider->show();
        colorSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "color", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepColor = configureSlider(colorSlider, capabilities, value);
    }

    // TODO: This will not work
    mainLayout->setStretchFactor(qualityGroupBox, qualityCount);

    if (qualityCount == 0) {
        qualityGroupBox->hide();
    }

    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        exposureCheckBox->show();
        exposureCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        exposureLabel->show();
        exposureSlider->show();
        exposureSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "exposure", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepExposure = configureSlider(exposureSlider, capabilities, value);
    }

    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        zoomCheckBox->show();
        zoomCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        zoomLabel->show();
        zoomSlider->show();
        zoomSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "zoom", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepZoom = configureSlider(zoomSlider, capabilities, value);
    }

    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        focusCheckBox->show();
        focusCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        focusLabel->show();
        focusSlider->show();
        focusSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "focus", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepFocus = configureSlider(focusSlider, capabilities, value);
    }

    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        panCheckBox->show();
        panCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        panLabel->show();
        panSlider->show();
        panSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "pan", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepPan = configureSlider(panSlider, capabilities, value);
    }

    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        tiltCheckBox->show();
        tiltCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        tiltLabel->show();
        tiltSlider->show();
        tiltSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "tilt", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepTilt = configureSlider(tiltSlider, capabilities, value);
    }

    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        irisCheckBox->show();
        irisCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        irisLabel->show();
        irisSlider->show();
        irisSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "iris", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepIris = configureSlider(irisSlider, capabilities, value);
    }

    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        controlCount++;
        rollCheckBox->show();
        rollCheckBox->setEnabled(false);
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
    }
    if (capabilities->isCapability()) {
        controlCount++;
        rollLabel->show();
        rollSlider->show();
        rollSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "roll", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        stepRoll = configureSlider(rollSlider, capabilities, value);
    }

    // TODO: This will not work
    mainLayout->setStretchFactor(controlGroupBox, controlCount);

    if (controlCount == 0) {
        controlGroupBox->hide();
    }

    closeButton->setFocus();

    qDebug() << "CameraControllerDialog::initialize --> End";
}


bool CameraControllerDialog::setUp()
{
    qDebug() << "CameraControllerDialog::setUp --> Start";

    PreferencesTool            *preferences = frontend->getPreferences();
    GrabberControlCapabilities *capabilities;
    bool                        checked;
    int                         value;
    int                         progressMax = 17;
    int                         progressValue = 0;

    QProgressDialog             progress(tr("Restore Camera Settings..."), QString(), 0, progressMax);

    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    grabberController->setUp();

    progress.setValue(progressValue++);
    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->setEnabled(true);
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
        changeAutoBrightness(checked, false);
    }
    if (capabilities->isCapability()) {
        brightnessSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "brightness", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeBrightness(value, false);
    }
    qDebug() << "CameraControllerDialog::setUp --> Brightness enabeled:" << brightnessSlider->isEnabled();

    progress.setValue(progressValue++);
    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->setEnabled(true);
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
        changeAutoContrast(checked, false);
    }
    if (capabilities->isCapability()) {
        contrastSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "contrast", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeContrast(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->setEnabled(true);
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
        changeAutoSaturation(checked, false);
    }
    if (capabilities->isCapability()) {
        saturationSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "saturation", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeSaturation(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->setEnabled(true);
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
        changeAutoHue(checked, false);
    }
    if (capabilities->isCapability()) {
        hueSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "hue", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeHue(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->setEnabled(true);
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
        changeAutoGamma(checked, false);
    }
    if (capabilities->isCapability()) {
        gammaSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "gamma", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeGamma(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->setEnabled(true);
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
        changeAutoSharpness(checked, false);
    }
    if (capabilities->isCapability()) {
        sharpnessSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "sharpness", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeSharpness(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->setEnabled(true);
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
        changeAutoBacklight(checked, false);
    }
    if (capabilities->isCapability()) {
        backlightSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "backlight", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeBacklight(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->setEnabled(true);
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
        changeAutoWhite(checked, false);
    }
    if (capabilities->isCapability()) {
        whiteSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "white", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeWhite(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->setEnabled(true);
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
        changeAutoGain(checked, false);
    }
    if (capabilities->isCapability()) {
        gainSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "gain", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeGain(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->setEnabled(true);
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
        changeAutoColor(checked, false);
    }
    if (capabilities->isCapability()) {
        colorSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "color", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeColor(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->setEnabled(true);
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
        changeAutoExposure(checked, false);
    }
    if (capabilities->isCapability()) {
        exposureSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "exposure", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeExposure(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->setEnabled(true);
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
        changeAutoZoom(checked, false);
    }
    if (capabilities->isCapability()) {
        zoomSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "zoom", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeZoom(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->setEnabled(true);
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
        changeAutoFocus(checked, false);
    }
    if (capabilities->isCapability()) {
        focusSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "focus", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeFocus(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->setEnabled(true);
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
        changeAutoPan(checked, false);
    }
    if (capabilities->isCapability()) {
        panSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "pan", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changePan(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->setEnabled(true);
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
        changeAutoTilt(checked, false);
    }
    if (capabilities->isCapability()) {
        tiltSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "tilt", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeTilt(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->setEnabled(true);
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
        changeAutoIris(checked, false);
    }
    if (capabilities->isCapability()) {
        irisSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "iris", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeIris(value, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->setEnabled(true);
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
        changeAutoRoll(checked, false);
    }
    if (capabilities->isCapability()) {
        rollSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "roll", value) == false) {
            // Calculate default value
            value = capabilities->getDefault();
        }
        changeRoll(value, false);
    }

    progress.setValue(progressValue);
    progress.hide();

    qDebug() << "CameraControllerDialog::setUp --> End";

    return true;
}


bool CameraControllerDialog::tearDown()
{
    qDebug() << "CameraControllerDialog::tearDown --> Start";

    GrabberControlCapabilities *capabilities;

    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        brightnessSlider->setEnabled(false);
    }

    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        contrastSlider->setEnabled(false);
    }

    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        saturationSlider->setEnabled(false);
    }

    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        hueSlider->setEnabled(false);
    }

    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        gammaSlider->setEnabled(false);
    }

    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        sharpnessSlider->setEnabled(false);
    }

    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        backlightSlider->setEnabled(false);
    }

    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        whiteSlider->setEnabled(false);
    }

    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        gainSlider->setEnabled(false);
    }

    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        colorSlider->setEnabled(false);
    }

    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        exposureSlider->setEnabled(false);
    }

    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        zoomSlider->setEnabled(false);
    }

    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        focusSlider->setEnabled(false);
    }

    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        panSlider->setEnabled(false);
    }

    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        tiltSlider->setEnabled(false);
    }

    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        irisSlider->setEnabled(false);
    }

    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->setEnabled(false);
    }
    if (capabilities->isCapability()) {
        rollSlider->setEnabled(false);
    }

    grabberController->tearDown();

    qDebug() << "CameraControllerDialog::tearDown --> End";

    return true;
}

/**************************************************************************
 **************************************************************************
 * Camera capabilities
 **************************************************************************
 **************************************************************************/

void CameraControllerDialog::changeAutoBrightness(int newState)
{
    changeAutoBrightness(newState, true);
}


void CameraControllerDialog::changeAutoBrightness(int newState, bool save)
{
    qDebug() << "CameraControllerDialog::changeAutoBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        brightnessSlider->setEnabled(false);
        grabberController->setAutomaticBrightness(true);
    }
    else {
        brightnessSlider->setEnabled(true);
        grabberController->setAutomaticBrightness(false);
    }
    if (save) {
        preferences->setIntegerPreference(deviceId, "automaticbrightness", newState);
    }

    qDebug() << "CameraControllerDialog::changeAutoBrightness --> End";
}


void CameraControllerDialog::changeBrightness(int value)
{
    brightnessSlider->setToolTip(QString::number(value));
    changeBrightness(value, true);
}


void CameraControllerDialog::changeBrightness(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setBrightness(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "brightness", value);
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
        contrastSlider->setEnabled(false);
        grabberController->setAutomaticContrast(true);
    }
    else {
        contrastSlider->setEnabled(true);
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
    int value = grabberController->getContrastCaps()->getMinimum() + (index * stepContrast);
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
        saturationSlider->setEnabled(false);
        grabberController->setAutomaticSaturation(true);
    }
    else {
        saturationSlider->setEnabled(true);
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
    int value = grabberController->getSaturationCaps()->getMinimum() + (index * stepSaturation);
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
        hueSlider->setEnabled(false);
        grabberController->setAutomaticHue(true);
    }
    else {
        hueSlider->setEnabled(true);
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
    int value = grabberController->getHueCaps()->getMinimum() + (index * stepHue);
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
        gammaSlider->setEnabled(false);
        grabberController->setAutomaticGamma(true);
    }
    else {
        gammaSlider->setEnabled(true);
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
    int value = grabberController->getGammaCaps()->getMinimum() + (index * stepGamma);
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
        sharpnessSlider->setEnabled(false);
        grabberController->setAutomaticSharpness(true);
    }
    else {
        sharpnessSlider->setEnabled(true);
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
    int value = grabberController->getSharpnessCaps()->getMinimum() + (index * stepSharpness);
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
        backlightSlider->setEnabled(false);
        grabberController->setAutomaticBacklight(true);
    }
    else {
        backlightSlider->setEnabled(true);
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
    int value = grabberController->getBacklightCaps()->getMinimum() + (index * stepBacklight);
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
        whiteSlider->setEnabled(false);
        grabberController->setAutomaticWhite(true);
    }
    else {
        whiteSlider->setEnabled(true);
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
    int value = grabberController->getWhiteCaps()->getMinimum() + (index * stepWhite);
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
        gainSlider->setEnabled(false);
        grabberController->setAutomaticGain(true);
    }
    else {
        gainSlider->setEnabled(true);
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
    int value = grabberController->getGainCaps()->getMinimum() + (index * stepGain);
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
        colorSlider->setEnabled(false);
        grabberController->setAutomaticColor(true);
    }
    else {
        colorSlider->setEnabled(true);
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
    int value = grabberController->getColorCaps()->getMinimum() + (index * stepColor);
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
        exposureSlider->setEnabled(false);
        grabberController->setAutomaticExposure(true);
    }
    else {
        exposureSlider->setEnabled(true);
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
    int value = grabberController->getExposureCaps()->getMinimum() + (index * stepExposure);
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
        zoomSlider->setEnabled(false);
        grabberController->setAutomaticZoom(true);
    }
    else {
        zoomSlider->setEnabled(true);
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
    int value = grabberController->getZoomCaps()->getMinimum() + (index * stepZoom);
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
        focusSlider->setEnabled(false);
        grabberController->setAutomaticFocus(true);
    }
    else {
        focusSlider->setEnabled(true);
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
    int value = grabberController->getFocusCaps()->getMinimum() + (index * stepFocus);
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
        panSlider->setEnabled(false);
        grabberController->setAutomaticPan(true);
    }
    else {
        panSlider->setEnabled(true);
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
    int value = grabberController->getPanCaps()->getMinimum() + (index * stepPan);
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
        tiltSlider->setEnabled(false);
        grabberController->setAutomaticTilt(true);
    }
    else {
        tiltSlider->setEnabled(true);
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
    int value = grabberController->getTiltCaps()->getMinimum() + (index * stepTilt);
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
        irisSlider->setEnabled(false);
        grabberController->setAutomaticIris(true);
    }
    else {
        irisSlider->setEnabled(true);
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
    int value = grabberController->getIrisCaps()->getMinimum() + (index * stepIris);
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
        rollSlider->setEnabled(false);
        grabberController->setAutomaticRoll(true);
    }
    else {
        rollSlider->setEnabled(true);
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
    int value = grabberController->getRollCaps()->getMinimum() + (index * stepRoll);
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


int CameraControllerDialog::configureSlider(QSlider *slider, GrabberControlCapabilities *controlCaps, int value)
{
    slider->setMinimum(controlCaps->getMinimum());
    slider->setMaximum(controlCaps->getMaximum());
    slider->setSingleStep(controlCaps->getStep());
    slider->setPageStep(controlCaps->getStep()/10);
    slider->setValue(value);

    slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    slider->setTracking(true);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setFocusPolicy(Qt::NoFocus);

    qDebug() << "CameraControllerDialog::configureSlider --> Values: Min:" << slider->minimum() << " Step:" << slider->singleStep() << " Max:" << slider->maximum();

    return controlCaps->getStep();
}


void CameraControllerDialog::reset()
{
    qDebug() << "CameraControllerDialog::reset --> Start";

    GrabberControlCapabilities *capabilities;
    int                         value;
    int                         progressMax = 17;
    int                         progressValue = 0;
    QProgressDialog             progress(tr("Reset Camera Settings..."), tr("Abort Reset"), 0, progressMax);

    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        brightnessSlider->setValue(value);
        changeBrightness(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getContrastCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        contrastSlider->setValue(value);
        changeContrast(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        saturationSlider->setValue(value);
        changeSaturation(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getHueCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        hueSlider->setValue(value);
        changeHue(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getGammaCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        gammaSlider->setValue(value);
        changeGamma(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        sharpnessSlider->setValue(value);
        changeSharpness(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        backlightSlider->setValue(value);
        changeBacklight(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getGainCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        gainSlider->setValue(value);
        changeGain(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getColorCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        colorSlider->setValue(value);
        changeColor(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        whiteSlider->setValue(value);
        changeWhite(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getExposureCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        exposureSlider->setValue(value);
        changeExposure(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getZoomCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        zoomSlider->setValue(value);
        changeZoom(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getFocusCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        focusSlider->setValue(value);
        changeFocus(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getPanCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        panSlider->setValue(value);
        changePan(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getTiltCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        tiltSlider->setValue(value);
        changeTilt(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getIrisCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        irisSlider->setValue(value);
        changeIris(value, true);
    }

    progress.setValue(progressValue++);
    if (progress.wasCanceled()) {
        return;
    }
    capabilities = grabberController->getRollCaps();
    if (capabilities->isCapability()) {
        value = capabilities->getDefault();
        rollSlider->setValue(value);
        changeRoll(value, true);
    }

    progress.setValue(progressMax);
    progress.hide();

    qDebug() << "CameraControllerDialog::reset --> End";
}


void CameraControllerDialog::close()
{
    qDebug() << "CameraControllerDialog::close --> Start";

    setFocus();

    // Disable the controlls an the tear down the grabber controller
    tearDown();

    this->hide();

    qDebug() << "CameraControllerDialog::close --> End";
}
