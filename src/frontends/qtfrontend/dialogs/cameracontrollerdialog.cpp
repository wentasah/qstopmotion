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

#include "cameracontrollerdialog.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QUrl>

#include "frontends/qtfrontend/preferences/controlerwidget.h"
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

    makeGUI();
    retranslateStrings();

    qDebug() << "CameraControllerDialog::Constructor --> End";
}


void CameraControllerDialog::makeGUI()
{
    qDebug() << "CameraControllerDialog::makeGUI --> Start";

    setWindowTitle("windowTitle");
    setMinimumSize(250, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------

    qualityGroupBox = new QGroupBox("qualityGroupBox");
    QVBoxLayout *qualityLayout = new QVBoxLayout;
    qualityGroupBox->setLayout(qualityLayout);

    brightnessCheckBox = new QCheckBox("brightnessCheckBox");
    brightnessCheckBox->setChecked(false);
    connect(brightnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBrightness(int)));
    brightnessCheckBox->hide();

    brightnessLabel = new QLabel("brightnessLabel");
    brightnessSlider = new QwtSlider();
    connect(brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(changeBrightness()));
    brightnessLabel->hide();
    brightnessSlider->hide();

    contrastCheckBox = new QCheckBox("contrastCheckBox");
    contrastCheckBox->setChecked(false);
    connect(contrastCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoContrast(int)));
    contrastCheckBox->hide();

    contrastLabel = new QLabel("contrastLabel");
    contrastSlider = new QwtSlider();
    connect(contrastSlider, SIGNAL(sliderReleased()), this, SLOT(changeContrast()));
    contrastLabel->hide();
    contrastSlider->hide();

    saturationCheckBox = new QCheckBox("saturationCheckBox");
    saturationCheckBox->setChecked(false);
    connect(saturationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSaturation(int)));
    saturationCheckBox->hide();

    saturationLabel = new QLabel("saturationLabel");
    saturationSlider = new QwtSlider();
    connect(saturationSlider, SIGNAL(sliderReleased()), this, SLOT(changeSaturation()));
    saturationLabel->hide();
    saturationSlider->hide();

    hueCheckBox = new QCheckBox("hueCheckBox");
    hueCheckBox->setChecked(false);
    connect(hueCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoHue(int)));
    hueCheckBox->hide();

    hueLabel = new QLabel("hueLabel");
    hueSlider = new QwtSlider();
    connect(hueSlider, SIGNAL(sliderReleased()), this, SLOT(changeHue()));
    hueLabel->hide();
    hueSlider->hide();

    gammaCheckBox = new QCheckBox("gammaCheckBox");
    gammaCheckBox->setChecked(false);
    connect(gammaCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGamma(int)));
    gammaCheckBox->hide();

    gammaLabel = new QLabel("gammaLabel");
    gammaSlider = new QwtSlider();
    connect(gammaSlider, SIGNAL(sliderReleased()), this, SLOT(changeGamma()));
    gammaLabel->hide();
    gammaSlider->hide();

    sharpnessCheckBox = new QCheckBox("sharpnessCheckBox");
    sharpnessCheckBox->setChecked(false);
    connect(sharpnessCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoSharpness(int)));
    sharpnessCheckBox->hide();

    sharpnessLabel = new QLabel("sharpnessLabel");
    sharpnessSlider = new QwtSlider();
    connect(sharpnessSlider, SIGNAL(sliderReleased()), this, SLOT(changeSharpness()));
    sharpnessLabel->hide();
    sharpnessSlider->hide();

    backlightCheckBox = new QCheckBox("backlightCheckBox");
    backlightCheckBox->setChecked(false);
    connect(backlightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoBacklight(int)));
    backlightCheckBox->hide();

    backlightLabel = new QLabel("backlightLabel");
    backlightSlider = new QwtSlider();
    connect(backlightSlider, SIGNAL(sliderReleased()), this, SLOT(changeBacklight()));
    backlightLabel->hide();
    backlightSlider->hide();

    whiteCheckBox = new QCheckBox("whiteCheckBox");
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));
    whiteCheckBox->hide();

    whiteLabel = new QLabel("whiteLabel");
    whiteSlider = new QwtSlider();
    connect(whiteSlider, SIGNAL(sliderReleased()), this, SLOT(changeWhite()));
    whiteLabel->hide();
    whiteSlider->hide();

    gainCheckBox = new QCheckBox("gainCheckBox");
    gainCheckBox->setChecked(false);
    connect(gainCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoGain(int)));
    gainCheckBox->hide();

    gainLabel = new QLabel("gainLabel");
    gainSlider = new QwtSlider();
    connect(gainSlider, SIGNAL(sliderReleased()), this, SLOT(changeGain()));
    gainLabel->hide();
    gainSlider->hide();

    colorCheckBox = new QCheckBox("colorCheckBox");
    colorCheckBox->setChecked(false);
    connect(colorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoColor(int)));
    colorCheckBox->hide();

    colorLabel = new QLabel("colorLabel");
    colorSlider = new QwtSlider();
    connect(colorSlider, SIGNAL(sliderReleased()), this, SLOT(changeColor()));
    colorLabel->hide();
    colorSlider->hide();

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------

    controlGroupBox = new QGroupBox("controlGroupBox");
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlGroupBox->setLayout(controlLayout);

    exposureCheckBox = new QCheckBox("exposureCheckBox");
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));
    exposureCheckBox->hide();

    exposureLabel = new QLabel("exposureLabel");
    exposureSlider = new QwtSlider();
    connect(exposureSlider, SIGNAL(sliderReleased()), this, SLOT(changeExposure()));
    exposureLabel->hide();
    exposureSlider->hide();

    zoomCheckBox = new QCheckBox("zoomCheckBox");
    zoomCheckBox->setChecked(false);
    connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoZoom(int)));
    zoomCheckBox->hide();

    zoomLabel = new QLabel("zoomLabel");
    zoomSlider = new QwtSlider();
    connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(changeZoom()));
    zoomLabel->hide();
    zoomSlider->hide();

    focusCheckBox = new QCheckBox("focusCheckBox");
    focusCheckBox->setChecked(false);
    connect(focusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoFocus(int)));
    focusCheckBox->hide();

    focusLabel = new QLabel("focusLabel");
    focusSlider = new QwtSlider();
    connect(focusSlider, SIGNAL(sliderReleased()), this, SLOT(changeFocus()));
    focusLabel->hide();
    focusSlider->hide();

    panCheckBox = new QCheckBox("panCheckBox");
    panCheckBox->setChecked(false);
    connect(panCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoPan(int)));
    panCheckBox->hide();

    panLabel = new QLabel("panLabel");
    panSlider = new QwtSlider();
    connect(panSlider, SIGNAL(sliderReleased()), this, SLOT(changePan()));
    panLabel->hide();
    panSlider->hide();

    tiltCheckBox = new QCheckBox("tiltCheckBox");
    tiltCheckBox->setChecked(false);
    connect(tiltCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoTilt(int)));
    tiltCheckBox->hide();

    tiltLabel = new QLabel("tiltLabel");
    tiltSlider = new QwtSlider();
    connect(tiltSlider, SIGNAL(sliderReleased()), this, SLOT(changeTilt()));
    tiltLabel->hide();
    tiltSlider->hide();

    irisCheckBox = new QCheckBox("irisCheckBox");
    irisCheckBox->setChecked(false);
    connect(irisCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoIris(int)));
    irisCheckBox->hide();

    irisLabel = new QLabel("irisLabel");
    irisSlider = new QwtSlider();
    connect(irisSlider, SIGNAL(sliderReleased()), this, SLOT(changeIris()));
    irisLabel->hide();
    irisSlider->hide();

    rollCheckBox = new QCheckBox("rollCheckBox");
    rollCheckBox->setChecked(false);
    connect(rollCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoRoll(int)));
    rollCheckBox->hide();

    rollLabel = new QLabel("rollLabel");
    rollSlider = new QwtSlider();
    connect(rollSlider, SIGNAL(sliderReleased()), this, SLOT(changeRoll()));
    rollLabel->hide();
    rollSlider->hide();

    resetButton = new QPushButton("resetButton");
    resetButton->setDefault(false);
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));

    helpButton = new QPushButton(tr("Help"), this);
    helpButton->setDefault(false);
    connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
    helpButton->setShortcut(QKeySequence::HelpContents);
    connect(helpButton, SIGNAL(triggered()), this, SLOT(help()));

    closeButton = new QPushButton("closeButton");
    closeButton->setDefault(true);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(helpButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(resetButton);
    bottomLayout->addLayout(buttonLayout);

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

    mainLayout->addWidget(qualityGroupBox);
    mainLayout->addWidget(controlGroupBox);
    mainLayout->addStretch(10);
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug() << "CameraControllerDialog::makeGUI --> End";
}


void CameraControllerDialog::retranslateStrings()
{
    qDebug() << "CameraControllerDialog::retranslateStrings --> Start";

    setWindowTitle(tr("qStopMotion Camera Controller"));

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------

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

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------

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
    bool                        boolValue;
    int                         intValue;

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------
    // Brightness Controls
    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticbrightness", boolValue) == false) {
            boolValue = false;
        }
        brightnessCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        brightnessSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "brightness", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(brightnessSlider, capabilities, intValue);
    }

    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticcontrast", boolValue) == false) {
            boolValue = false;
        }
        contrastCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        contrastSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "contrast", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(contrastSlider, capabilities, intValue);
    }

    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticsaturation", boolValue) == false) {
            boolValue = false;
        }
        saturationCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        saturationSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "saturation", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(saturationSlider, capabilities, intValue);
    }

    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automatichue", boolValue) == false) {
            boolValue = false;
        }
        hueCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        hueSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "hue", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(hueSlider, capabilities, intValue);
    }

    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticgamma", boolValue) == false) {
            boolValue = false;
        }
        gammaCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        gammaSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "gamma", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(gammaSlider, capabilities, intValue);
    }

    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticsharpness", boolValue) == false) {
            boolValue = false;
        }
        sharpnessCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        sharpnessSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "sharpness", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(sharpnessSlider, capabilities, intValue);
    }

    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticbacklight", boolValue) == false) {
            boolValue = false;
        }
        backlightCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        backlightSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "backlight", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(backlightSlider, capabilities, intValue);
    }

    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticwhite", boolValue) == false) {
            boolValue = false;
        }
        whiteCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        whiteSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "white", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(whiteSlider, capabilities, intValue);
    }

    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticgain", boolValue) == false) {
            boolValue = false;
        }
        gainCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        gainSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "gain", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(gainSlider, capabilities, intValue);
    }

    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticcolor", boolValue) == false) {
            boolValue = false;
        }
        colorCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        colorSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "color", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(colorSlider, capabilities, intValue);
    }

    if (qualityCount == 0) {
        qualityGroupBox->hide();
    }

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------

    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticexposure", boolValue) == false) {
            boolValue = false;
        }
        exposureCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        exposureSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "exposure", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(exposureSlider, capabilities, intValue);
    }

    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticzoom", boolValue) == false) {
            boolValue = false;
        }
        zoomCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        zoomSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "zoom", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(zoomSlider, capabilities, intValue);
    }

    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticfocus", boolValue) == false) {
            boolValue = false;
        }
        focusCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        focusSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "focus", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(focusSlider, capabilities, intValue);
    }

    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticpan", boolValue) == false) {
            boolValue = false;
        }
        panCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        panSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "pan", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(panSlider, capabilities, intValue);
    }

    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automatictilt", boolValue) == false) {
            boolValue = false;
        }
        tiltCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        tiltSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "tilt", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(tiltSlider, capabilities, intValue);
    }

    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticiris", boolValue) == false) {
            boolValue = false;
        }
        irisCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        irisSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "iris", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(irisSlider, capabilities, intValue);
    }

    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->setEnabled(false);
        if (preferences->getBooleanPreference(deviceId, "automaticroll", boolValue) == false) {
            boolValue = false;
        }
        rollCheckBox->setChecked(boolValue);
    }
    if (capabilities->isCapability()) {
        rollSlider->setEnabled(false);
        if (preferences->getIntegerPreference(deviceId, "roll", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        configureSlider(rollSlider, capabilities, intValue);
    }

    if (controlCount == 0) {
        controlGroupBox->hide();
    }

    closeButton->setFocus();

    qDebug() << "CameraControllerDialog::initialize --> End";
}


void CameraControllerDialog::enableControls()
{
    qDebug() << "CameraControllerDialog::enableControls --> Start";

    PreferencesTool            *preferences = frontend->getPreferences();
    GrabberControlCapabilities *capabilities;
    bool                        showControl;

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------
    qualityCount = 0;
    // Brightness control
    capabilities = grabberController->getBrightnessCaps();
    if (preferences->getBooleanPreference("preferences", "brightnesscontrol", showControl) == false) {
        showControl = ControlerWidget::BRIGHTNESSDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            brightnessCheckBox->show();
        }
        else {
            brightnessCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            brightnessLabel->show();
            brightnessSlider->show();
        }
        else {
            brightnessLabel->hide();
            brightnessSlider->hide();
        }
    }

    // Contrast control
    capabilities = grabberController->getContrastCaps();
    if (preferences->getBooleanPreference("preferences", "contrastcontrol", showControl) == false) {
        showControl = ControlerWidget::CONTRASTDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            contrastCheckBox->show();
        }
        else {
            contrastCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            contrastLabel->show();
            contrastSlider->show();
        }
        else {
            contrastLabel->hide();
            contrastSlider->hide();
        }
    }

    // Saturation control
    capabilities = grabberController->getSaturationCaps();
    if (preferences->getBooleanPreference("preferences", "saturationcontrol", showControl) == false) {
        showControl = ControlerWidget::SATURATIONDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            saturationCheckBox->show();
        }
        else {
            saturationCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            saturationLabel->show();
            saturationSlider->show();
        }
        else {
            saturationLabel->hide();
            saturationSlider->hide();
        }
    }

    // Hue control
    capabilities = grabberController->getHueCaps();
    if (preferences->getBooleanPreference("preferences", "huecontrol", showControl) == false) {
        showControl = ControlerWidget::HUEDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            hueCheckBox->show();
        }
        else {
            hueCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            hueLabel->show();
            hueSlider->show();
        }
        else {
            hueLabel->hide();
            hueSlider->hide();
        }
    }

    // Gamma control
    capabilities = grabberController->getGammaCaps();
    if (preferences->getBooleanPreference("preferences", "gammacontrol", showControl) == false) {
        showControl = ControlerWidget::HUEDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            gammaCheckBox->show();
        }
        else {
            gammaCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            gammaLabel->show();
            gammaSlider->show();
        }
        else {
            gammaLabel->hide();
            gammaSlider->hide();
        }
    }

    // Sharpness control
    capabilities = grabberController->getSharpnessCaps();
    if (preferences->getBooleanPreference("preferences", "sharpnesscontrol", showControl) == false) {
        showControl = ControlerWidget::SHARPNESSDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            sharpnessCheckBox->show();
        }
        else {
            sharpnessCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            sharpnessLabel->show();
            sharpnessSlider->show();
        }
        else {
            sharpnessLabel->hide();
            sharpnessSlider->hide();
        }
    }

    // Backlight control
    capabilities = grabberController->getBacklightCaps();
    if (preferences->getBooleanPreference("preferences", "backlightcontrol", showControl) == false) {
        showControl = ControlerWidget::BACKLIGHTDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            backlightCheckBox->show();
        }
        else {
            backlightCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            backlightLabel->show();
            backlightSlider->show();
        }
        else {
            backlightLabel->hide();
            backlightSlider->hide();
        }
    }

    // White control
    capabilities = grabberController->getWhiteCaps();
    if (preferences->getBooleanPreference("preferences", "whitecontrol", showControl) == false) {
        showControl = ControlerWidget::WHITEDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            whiteCheckBox->show();
        }
        else {
            whiteCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            whiteLabel->show();
            whiteSlider->show();
        }
        else {
            whiteLabel->hide();
            whiteSlider->hide();
        }
    }

    // Gain control
    capabilities = grabberController->getGainCaps();
    if (preferences->getBooleanPreference("preferences", "gaincontrol", showControl) == false) {
        showControl = ControlerWidget::GAINDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            gainCheckBox->show();
        }
        else {
            gainCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            gainLabel->show();
            gainSlider->show();
        }
        else {
            gainLabel->hide();
            gainSlider->hide();
        }
    }

    // Color control
    capabilities = grabberController->getColorCaps();
    if (preferences->getBooleanPreference("preferences", "colorcontrol", showControl) == false) {
        showControl = ControlerWidget::COLORDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            qualityCount++;
            colorCheckBox->show();
        }
        else {
            colorCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            qualityCount++;
            colorLabel->show();
            colorSlider->show();
        }
        else {
            colorLabel->hide();
            colorSlider->hide();
        }
    }

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------
    controlCount = 0;
    // Exposure control
    capabilities = grabberController->getExposureCaps();
    if (preferences->getBooleanPreference("preferences", "exposurecontrol", showControl) == false) {
        showControl = ControlerWidget::EXPOSUREDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            exposureCheckBox->show();
        }
        else {
            exposureCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            exposureLabel->show();
            exposureSlider->show();
        }
        else {
            exposureLabel->hide();
            exposureSlider->hide();
        }
    }

    // Zoom control
    capabilities = grabberController->getZoomCaps();
    if (preferences->getBooleanPreference("preferences", "zoomcontrol", showControl) == false) {
        showControl = ControlerWidget::ZOOMDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            zoomCheckBox->show();
        }
        else {
            zoomCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            zoomLabel->show();
            zoomSlider->show();
        }
        else {
            zoomLabel->hide();
            zoomSlider->hide();
        }
    }

    // Focus control
    capabilities = grabberController->getFocusCaps();
    if (preferences->getBooleanPreference("preferences", "focuscontrol", showControl) == false) {
        showControl = ControlerWidget::FOCUSDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            focusCheckBox->show();
        }
        else {
            focusCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            focusLabel->show();
            focusSlider->show();
        }
        else {
            focusLabel->hide();
            focusSlider->hide();
        }
    }

    // Pan control
    capabilities = grabberController->getPanCaps();
    if (preferences->getBooleanPreference("preferences", "pancontrol", showControl) == false) {
        showControl = ControlerWidget::PANDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            panCheckBox->show();
        }
        else {
            panCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            panLabel->show();
            panSlider->show();
        }
        else {
            panLabel->hide();
            panSlider->hide();
        }
    }

    // Tilt control
    capabilities = grabberController->getTiltCaps();
    if (preferences->getBooleanPreference("preferences", "tiltcontrol", showControl) == false) {
        showControl = ControlerWidget::TILTDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            tiltCheckBox->show();
        }
        else {
            tiltCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            tiltLabel->show();
            tiltSlider->show();
        }
        else {
            tiltLabel->hide();
            tiltSlider->hide();
        }
    }

    // Iris control
    capabilities = grabberController->getIrisCaps();
    if (preferences->getBooleanPreference("preferences", "iriscontrol", showControl) == false) {
        showControl = ControlerWidget::IRISDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            irisCheckBox->show();
        }
        else {
            irisCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            irisLabel->show();
            irisSlider->show();
        }
        else {
            irisLabel->hide();
            irisSlider->hide();
        }
    }

    // Roll control
    capabilities = grabberController->getRollCaps();
    if (preferences->getBooleanPreference("preferences", "rollcontrol", showControl) == false) {
        showControl = ControlerWidget::ROLLDEFAULT;
    }
    if (capabilities->isAutomatic()) {
        if (showControl) {
            controlCount++;
            rollCheckBox->show();
        }
        else {
            rollCheckBox->hide();
        }
    }
    if (capabilities->isCapability()) {
        if (showControl) {
            controlCount++;
            rollLabel->show();
            rollSlider->show();
        }
        else {
            rollLabel->hide();
            rollSlider->hide();
        }
    }

    // Show/hide control group boxes
    if (qualityCount == 0) {
        qualityGroupBox->hide();
    }
    else {
        qualityGroupBox->show();
    }

    if (controlCount == 0) {
        controlGroupBox->hide();
    }
    else {
        controlGroupBox->show();
    }

    qDebug() << "CameraControllerDialog::enableControls --> Stop";
}


bool CameraControllerDialog::setUp()
{
    qDebug() << "CameraControllerDialog::setUp --> Start";

    PreferencesTool            *preferences = frontend->getPreferences();
    GrabberControlCapabilities *capabilities;
    bool                        boolValue;
    int                         intValue;
    int                         progressMax = 17;
    int                         progressValue = 0;

    QProgressDialog             progress(tr("Restore Camera Settings..."), QString(), 0, progressMax);

    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    grabberController->setUp();

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------

    progress.setValue(progressValue++);
    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticbrightness", boolValue) == false) {
            boolValue = false;
        }
        changeAutoBrightness(boolValue, false);
    }
    if (capabilities->isCapability()) {
        brightnessSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "brightness", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeBrightness(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticcontrast", boolValue) == false) {
            boolValue = false;
        }
        changeAutoContrast(boolValue, false);
    }
    if (capabilities->isCapability()) {
        contrastSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "contrast", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeContrast(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticsaturation", boolValue) == false) {
            boolValue = false;
        }
        changeAutoSaturation(boolValue, false);
    }
    if (capabilities->isCapability()) {
        saturationSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "saturation", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeSaturation(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automatichue", boolValue) == false) {
            boolValue = false;
        }
        changeAutoHue(boolValue, false);
    }
    if (capabilities->isCapability()) {
        hueSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "hue", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeHue(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticgamma", boolValue) == false) {
            boolValue = false;
        }
        changeAutoGamma(boolValue, false);
    }
    if (capabilities->isCapability()) {
        gammaSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "gamma", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeGamma(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticsharpness", boolValue) == false) {
            boolValue = false;
        }
        changeAutoSharpness(boolValue, false);
    }
    if (capabilities->isCapability()) {
        sharpnessSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "sharpness", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeSharpness(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticbacklight", boolValue) == false) {
            boolValue = false;
        }
        changeAutoBacklight(boolValue, false);
    }
    if (capabilities->isCapability()) {
        backlightSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "backlight", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeBacklight(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticwhite", boolValue) == false) {
            boolValue = false;
        }
        changeAutoWhite(boolValue, false);
    }
    if (capabilities->isCapability()) {
        whiteSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "white", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeWhite(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticgain", boolValue) == false) {
            boolValue = false;
        }
        changeAutoGain(boolValue, false);
    }
    if (capabilities->isCapability()) {
        gainSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "gain", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeGain(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticcolor", boolValue) == false) {
            boolValue = false;
        }
        changeAutoColor(boolValue, false);
    }
    if (capabilities->isCapability()) {
        colorSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "color", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeColor(intValue, false);
    }

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------

    progress.setValue(progressValue++);
    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticexposure", boolValue) == false) {
            boolValue = false;
        }
        changeAutoExposure(boolValue, false);
    }
    if (capabilities->isCapability()) {
        exposureSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "exposure", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeExposure(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticzoom", boolValue) == false) {
            boolValue = false;
        }
        changeAutoZoom(boolValue, false);
    }
    if (capabilities->isCapability()) {
        zoomSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "zoom", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeZoom(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticfocus", boolValue) == false) {
            boolValue = false;
        }
        changeAutoFocus(boolValue, false);
    }
    if (capabilities->isCapability()) {
        focusSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "focus", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeFocus(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticpan", boolValue) == false) {
            boolValue = false;
        }
        changeAutoPan(boolValue, false);
    }
    if (capabilities->isCapability()) {
        panSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "pan", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changePan(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automatictilt", boolValue) == false) {
            boolValue = false;
        }
        changeAutoTilt(boolValue, false);
    }
    if (capabilities->isCapability()) {
        tiltSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "tilt", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeTilt(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticiris", boolValue) == false) {
            boolValue = false;
        }
        changeAutoIris(boolValue, false);
    }
    if (capabilities->isCapability()) {
        irisSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "iris", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeIris(intValue, false);
    }

    progress.setValue(progressValue++);
    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->setEnabled(true);
        if (preferences->getBooleanPreference(deviceId, "automaticroll", boolValue) == false) {
            boolValue = false;
        }
        changeAutoRoll(boolValue, false);
    }
    if (capabilities->isCapability()) {
        rollSlider->setEnabled(true);
        if (preferences->getIntegerPreference(deviceId, "roll", intValue) == false) {
            // Calculate default value
            intValue = capabilities->getDefault();
        }
        changeRoll(intValue, false);
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

    // -------------------------------------------------------------------------
    // Quality controls
    // -------------------------------------------------------------------------

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

    // -------------------------------------------------------------------------
    // Control controls
    // -------------------------------------------------------------------------

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


void CameraControllerDialog::changeBrightness()
{
    int value = (int)(brightnessSlider->value());

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


void CameraControllerDialog::changeContrast()
{
    int value = (int)(contrastSlider->value());

    contrastSlider->setToolTip(QString::number(value));
    changeContrast(value, true);
}


void CameraControllerDialog::changeContrast(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeContrast --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setContrast(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "contrast", value);
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


void CameraControllerDialog::changeSaturation()
{
    int value = (int)(saturationSlider->value());

    saturationSlider->setToolTip(QString::number(value));
    changeSaturation(value, true);
}


void CameraControllerDialog::changeSaturation(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeSaturation --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setSaturation(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "saturation", value);
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


void CameraControllerDialog::changeHue()
{
    int value = (int)(hueSlider->value());

    hueSlider->setToolTip(QString::number(value));
    changeHue(value, true);
}


void CameraControllerDialog::changeHue(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeHue --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setHue(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "hue", value);
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


void CameraControllerDialog::changeGamma()
{
    int value = (int)(gammaSlider->value());

    gammaSlider->setToolTip(QString::number(value));
    changeGamma(value, true);
}


void CameraControllerDialog::changeGamma(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeGamma --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setGamma(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "gamma", value);
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


void CameraControllerDialog::changeSharpness()
{
    int value = (int)(sharpnessSlider->value());

    sharpnessSlider->setToolTip(QString::number(value));
    changeSharpness(value, true);
}


void CameraControllerDialog::changeSharpness(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeSharpness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setSharpness(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "sharpness", value);
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


void CameraControllerDialog::changeBacklight()
{
    int value = (int)(backlightSlider->value());

    backlightSlider->setToolTip(QString::number(value));
    changeBacklight(value, true);
}


void CameraControllerDialog::changeBacklight(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeBacklight --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setBacklight(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "backlight", value);
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


void CameraControllerDialog::changeWhite()
{
    int value = (int)(whiteSlider->value());

    whiteSlider->setToolTip(QString::number(value));
    changeWhite(value, true);
}


void CameraControllerDialog::changeWhite(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeWhite --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setWhite(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "white", value);
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


void CameraControllerDialog::changeGain()
{
    int value = (int)(gainSlider->value());

    gainSlider->setToolTip(QString::number(value));
    changeGain(value, true);
}


void CameraControllerDialog::changeGain(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeGain --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setGain(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "gain", value);
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


void CameraControllerDialog::changeColor()
{
    int value = (int)(colorSlider->value());

    colorSlider->setToolTip(QString::number(value));
    changeColor(value, true);
}


void CameraControllerDialog::changeColor(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeColor --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setColor(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "color", value);
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


void CameraControllerDialog::changeExposure()
{
    int value = (int)(exposureSlider->value());

    exposureSlider->setToolTip(QString::number(value));
    changeExposure(value, true);
}


void CameraControllerDialog::changeExposure(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeExposure --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setExposure(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "exposure", value);
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


void CameraControllerDialog::changeZoom()
{
    int value = (int)(zoomSlider->value());

    zoomSlider->setToolTip(QString::number(value));
    changeZoom(value, true);
}


void CameraControllerDialog::changeZoom(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeZoo --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setZoom(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "zoom", value);
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


void CameraControllerDialog::changeFocus()
{
    int value = (int)(focusSlider->value());

    focusSlider->setToolTip(QString::number(value));
    changeFocus(value, true);
}


void CameraControllerDialog::changeFocus(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeFocus --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setFocus(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "focus", value);
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


void CameraControllerDialog::changePan()
{
    int value = (int)(panSlider->value());

    panSlider->setToolTip(QString::number(value));
    changePan(value, true);
}


void CameraControllerDialog::changePan(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changePan --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setPan(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "pan", value);
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


void CameraControllerDialog::changeTilt()
{
    int value = (int)(tiltSlider->value());

    tiltSlider->setToolTip(QString::number(value));
    changeTilt(value, true);
}


void CameraControllerDialog::changeTilt(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeTilt --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setTilt(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "tilt", value);
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


void CameraControllerDialog::changeIris()
{
    int value = (int)(irisSlider->value());

    irisSlider->setToolTip(QString::number(value));
    changeIris(value, true);
}


void CameraControllerDialog::changeIris(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeIris --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setIris(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "iris", value);
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


void CameraControllerDialog::changeRoll()
{
    int value = (int)(rollSlider->value());

    rollSlider->setToolTip(QString::number(value));
    changeRoll(value, true);
}


void CameraControllerDialog::changeRoll(int value, bool save)
{
    qDebug() << "CameraControllerDialog::changeRoll --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    grabberController->setRoll(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "roll", value);
    }

    qDebug() << "CameraControllerDialog::changeRoll --> End";
}


void CameraControllerDialog::configureSlider(QwtSlider *slider, GrabberControlCapabilities *controlCaps, int value)
{
    slider->setOrientation(Qt::Horizontal);
    slider->setScalePosition(QwtSlider::LeadingScale);
    slider->setGroove(true);

    slider->setScale((double)controlCaps->getMinimum(),(double)controlCaps->getMaximum());
    // slider->setSingleStep(controlCaps->getStep());
    // slider->setPageStep(controlCaps->getStep()/10);
    slider->setValue(value);

    slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    slider->setTracking(true);
    // slider->setTickInterval(10);
    // slider->setTickPosition(QwtSlider::TicksBelow);
    slider->setFocusPolicy(Qt::NoFocus);
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


void CameraControllerDialog::help()
{
    qDebug() << "CameraControllerDialog::help --> Start";

    frontend->openOnlineHelp("#control");

    qDebug() << "CameraControllerDialog::help --> End";
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
