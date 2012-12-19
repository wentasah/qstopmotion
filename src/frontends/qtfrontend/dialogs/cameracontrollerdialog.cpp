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

    PreferencesTool *preferences = frontend->getPreferences();
    int  defaultIndex;
    GrabberControlCapabilities *capabilities;

    capabilities = grabberController->getBrightnessCaps();
    if (capabilities->isAutomatic()) {
        brightnessCheckBox->show();
        brightnessCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticbrightness", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            brightnessLabel->show();
        }
        brightnessComboBox->show();
        stepBrightness = fillComboBox(brightnessComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        brightnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "brigthtness", defaultIndex));
    }

    capabilities = grabberController->getContrastCaps();
    if (capabilities->isAutomatic()) {
        contrastCheckBox->show();
        contrastCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticcontrast", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            contrastLabel->show();
        }
        contrastComboBox->show();
        stepContrast = fillComboBox(contrastComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        contrastComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "contrast", defaultIndex));
    }

    capabilities = grabberController->getSaturationCaps();
    if (capabilities->isAutomatic()) {
        saturationCheckBox->show();
        saturationCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticsaturation", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            saturationLabel->show();
        }
        saturationComboBox->show();
        stepSaturation = fillComboBox(saturationComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        saturationComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "saturation", defaultIndex));
    }

    capabilities = grabberController->getHueCaps();
    if (capabilities->isAutomatic()) {
        hueCheckBox->show();
        hueCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automatichue", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            hueLabel->show();
        }
        hueComboBox->show();
        stepHue = fillComboBox(hueComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        hueComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "hue", defaultIndex));
    }

    capabilities = grabberController->getGammaCaps();
    if (capabilities->isAutomatic()) {
        gammaCheckBox->show();
        gammaCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticgamma", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            gammaLabel->show();
        }
        gammaComboBox->show();
        stepGamma = fillComboBox(gammaComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        gammaComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "gamma", defaultIndex));
    }

    capabilities = grabberController->getSharpnessCaps();
    if (capabilities->isAutomatic()) {
        sharpnessCheckBox->show();
        sharpnessCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticsharpness", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            sharpnessLabel->show();
        }
        sharpnessComboBox->show();
        stepSharpness = fillComboBox(sharpnessComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        sharpnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "sharpness", defaultIndex));
    }

    capabilities = grabberController->getBacklightCaps();
    if (capabilities->isAutomatic()) {
        backlightCheckBox->show();
        backlightCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticbacklight", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            backlightLabel->show();
        }
        backlightComboBox->show();
        stepBacklight = fillComboBox(backlightComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        backlightComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "backlight", defaultIndex));
    }

    capabilities = grabberController->getWhiteCaps();
    if (capabilities->isAutomatic()) {
        whiteCheckBox->show();
        whiteCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticwhite", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            whiteLabel->show();
        }
        whiteComboBox->show();
        stepWhite = fillComboBox(whiteComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        whiteComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "white", defaultIndex));
    }

    capabilities = grabberController->getGainCaps();
    if (capabilities->isAutomatic()) {
        gainCheckBox->show();
        gainCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticgain", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            gainLabel->show();
        }
        gainComboBox->show();
        stepGain = fillComboBox(gainComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        gainComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "gain", defaultIndex));
    }

    capabilities = grabberController->getColorCaps();
    if (capabilities->isAutomatic()) {
        colorCheckBox->show();
        colorCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticcolor", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            colorLabel->show();
        }
        colorComboBox->show();
        stepColor = fillComboBox(colorComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        colorComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "color", defaultIndex));
    }

    capabilities = grabberController->getExposureCaps();
    if (capabilities->isAutomatic()) {
        exposureCheckBox->show();
        exposureCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticexposure", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            exposureLabel->show();
        }
        exposureComboBox->show();
        stepExposure = fillComboBox(exposureComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        exposureComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "exposure", defaultIndex));
    }

    capabilities = grabberController->getZoomCaps();
    if (capabilities->isAutomatic()) {
        zoomCheckBox->show();
        zoomCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticzoom", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            zoomLabel->show();
        }
        zoomComboBox->show();
        stepZoom = fillComboBox(zoomComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        zoomComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "zoom", defaultIndex));
    }

    capabilities = grabberController->getFocusCaps();
    if (capabilities->isAutomatic()) {
        focusCheckBox->show();
        focusCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticfocus", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            focusLabel->show();
        }
        focusComboBox->show();
        stepFocus = fillComboBox(focusComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        focusComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "focus", defaultIndex));
    }

    capabilities = grabberController->getPanCaps();
    if (capabilities->isAutomatic()) {
        panCheckBox->show();
        panCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticpan", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            panLabel->show();
        }
        panLabel->show();
        panComboBox->show();
        stepPan = fillComboBox(panComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        panComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "pan", defaultIndex));
    }

    capabilities = grabberController->getTiltCaps();
    if (capabilities->isAutomatic()) {
        tiltCheckBox->show();
        tiltCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automatictilt", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            tiltLabel->show();
        }
        tiltLabel->show();
        tiltComboBox->show();
        stepTilt = fillComboBox(tiltComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        tiltComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "tilt", defaultIndex));
    }

    capabilities = grabberController->getIrisCaps();
    if (capabilities->isAutomatic()) {
        irisCheckBox->show();
        irisCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticiris", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            irisLabel->show();
        }
        irisComboBox->show();
        stepIris = fillComboBox(irisComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        irisComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "iris", defaultIndex));
    }

    capabilities = grabberController->getRollCaps();
    if (capabilities->isAutomatic()) {
        rollCheckBox->show();
        rollCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticroll", false));
    }
    if (capabilities->isCapability()) {
        if (!capabilities->isAutomatic()) {
            rollLabel->show();
        }
        rollComboBox->show();
        stepRoll = fillComboBox(rollComboBox, capabilities);
        defaultIndex = (capabilities->getDefault() - capabilities->getMinimum()) / capabilities->getStep();
        rollComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "roll", defaultIndex));
    }

    qDebug() << "CameraControllerDialog::init --> End";
}


void CameraControllerDialog::changeAutoBrightness(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        brightnessComboBox->setEnabled(false);
        grabberController->setAutomaticBrightness(true);
        preferences->setIntegerPreference(deviceId, "automaticbrightness", true);
    }
    else {
        brightnessComboBox->setEnabled(true);
        grabberController->setAutomaticBrightness(false);
        preferences->setIntegerPreference(deviceId, "automaticbrightness", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoBrightness --> End";
}


void CameraControllerDialog::changeBrightness(int index)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    // long minValue;
    // long stepValue;
    long maxValue;

    maxValue = grabberController->getBrightnessCaps()->getMaximum();
    long value = index * stepBrightness;

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBrightness(value);
    preferences->setIntegerPreference(deviceId, "brigthtness", index);

    qDebug() << "CameraControllerDialog::changeBrightness --> End";
}


void CameraControllerDialog::changeAutoContrast(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoContrast --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        contrastComboBox->setEnabled(false);
        grabberController->setAutomaticContrast(true);
        preferences->setIntegerPreference(deviceId, "automaticcontrast", true);
    }
    else {
        contrastComboBox->setEnabled(true);
        grabberController->setAutomaticContrast(false);
        preferences->setIntegerPreference(deviceId, "automaticcontrast", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoContrast --> End";
}


void CameraControllerDialog::changeContrast(int index)
{
    qDebug() << "CameraControllerDialog::changeContrast --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepContrast;
    int maxValue = grabberController->getContrastCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setContrast(value);
    preferences->setIntegerPreference(deviceId, "contrast", index);

    qDebug() << "CameraControllerDialog::changeContrast --> End";
}


void CameraControllerDialog::changeAutoSaturation(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoSaturation --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        saturationComboBox->setEnabled(false);
        grabberController->setAutomaticSaturation(true);
        preferences->setIntegerPreference(deviceId, "automaticsaturation", true);
    }
    else {
        saturationComboBox->setEnabled(true);
        grabberController->setAutomaticSaturation(false);
        preferences->setIntegerPreference(deviceId, "automaticsaturation", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoSaturation --> End";
}


void CameraControllerDialog::changeSaturation(int index)
{
    qDebug() << "CameraControllerDialog::changeSaturation --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSaturation;
    int maxValue = grabberController->getSaturationCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setSaturation(value);
    preferences->setIntegerPreference(deviceId, "saturation", index);

    qDebug() << "CameraControllerDialog::changeSaturation --> End";
}


void CameraControllerDialog::changeAutoHue(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoHue --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        hueComboBox->setEnabled(false);
        grabberController->setAutomaticHue(true);
        preferences->setIntegerPreference(deviceId, "automatichue", true);
    }
    else {
        hueComboBox->setEnabled(true);
        grabberController->setAutomaticHue(false);
        preferences->setIntegerPreference(deviceId, "automatichue", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoHue --> End";
}


void CameraControllerDialog::changeHue(int index)
{
    qDebug() << "CameraControllerDialog::changeHue --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepHue;
    int maxValue = grabberController->getHueCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setHue(value);
    preferences->setIntegerPreference(deviceId, "hue", index);

    qDebug() << "CameraControllerDialog::changeHue --> End";
}


void CameraControllerDialog::changeAutoGamma(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoGamma --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        gammaComboBox->setEnabled(false);
        grabberController->setAutomaticGamma(true);
        preferences->setIntegerPreference(deviceId, "automaticgamma", true);
    }
    else {
        gammaComboBox->setEnabled(true);
        grabberController->setAutomaticGamma(false);
        preferences->setIntegerPreference(deviceId, "automaticgamma", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoGamma --> End";
}


void CameraControllerDialog::changeGamma(int index)
{
    qDebug() << "CameraControllerDialog::changeGamma --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepGamma;
    int maxValue = grabberController->getGammaCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setGamma(value);
    preferences->setIntegerPreference(deviceId, "gamma", index);

    qDebug() << "CameraControllerDialog::changeGamma --> End";
}


void CameraControllerDialog::changeAutoSharpness(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoSharpness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        sharpnessComboBox->setEnabled(false);
        grabberController->setAutomaticSharpness(true);
        preferences->setIntegerPreference(deviceId, "automaticsharpness", true);
    }
    else {
        sharpnessComboBox->setEnabled(true);
        grabberController->setAutomaticSharpness(false);
        preferences->setIntegerPreference(deviceId, "automaticsharpness", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoSharpness --> End";
}


void CameraControllerDialog::changeSharpness(int index)
{
    qDebug() << "CameraControllerDialog::changeSharpness --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSharpness;
    int maxValue = grabberController->getSharpnessCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setSharpness(value);
    preferences->setIntegerPreference(deviceId, "sharpness", index);

    qDebug() << "CameraControllerDialog::changeSharpness --> End";
}


void CameraControllerDialog::changeAutoBacklight(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoBacklight --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        backlightComboBox->setEnabled(false);
        grabberController->setAutomaticBacklight(true);
        preferences->setIntegerPreference(deviceId, "automaticbacklight", true);
    }
    else {
        backlightComboBox->setEnabled(true);
        grabberController->setAutomaticBacklight(false);
        preferences->setIntegerPreference(deviceId, "automaticbacklight", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoBacklight --> End";
}


void CameraControllerDialog::changeBacklight(int index)
{
    qDebug() << "CameraControllerDialog::changeBacklight --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepBacklight;
    int maxValue = grabberController->getBacklightCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBacklight(value);
    preferences->setIntegerPreference(deviceId, "backlight", index);

    qDebug() << "CameraControllerDialog::changeBacklight --> End";
}


void CameraControllerDialog::changeAutoWhite(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoWhite --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        whiteComboBox->setEnabled(false);
        grabberController->setAutomaticWhite(true);
        preferences->setIntegerPreference(deviceId, "automaticwhite", true);
    }
    else {
        whiteComboBox->setEnabled(true);
        grabberController->setAutomaticWhite(false);
        preferences->setIntegerPreference(deviceId, "automaticwhite", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoWhite --> End";
}


void CameraControllerDialog::changeWhite(int index)
{
    qDebug() << "CameraControllerDialog::changeWhite --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepWhite;
    int maxValue = grabberController->getWhiteCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setWhite(value);
    preferences->setIntegerPreference(deviceId, "white", index);

    qDebug() << "CameraControllerDialog::changeWhite --> End";
}


void CameraControllerDialog::changeAutoGain(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoGain --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        gainComboBox->setEnabled(false);
        grabberController->setAutomaticGain(true);
        preferences->setIntegerPreference(deviceId, "automaticgain", true);
    }
    else {
        gainComboBox->setEnabled(true);
        grabberController->setAutomaticGain(false);
        preferences->setIntegerPreference(deviceId, "automaticgain", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoGain --> End";
}


void CameraControllerDialog::changeGain(int index)
{
    qDebug() << "CameraControllerDialog::changeGain --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepGain;
    int maxValue = grabberController->getGainCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setGain(value);
    preferences->setIntegerPreference(deviceId, "gain", index);

    qDebug() << "CameraControllerDialog::changeGain --> End";
}


void CameraControllerDialog::changeAutoColor(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoColor --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        colorComboBox->setEnabled(false);
        grabberController->setAutomaticColor(true);
        preferences->setIntegerPreference(deviceId, "automaticcolor", true);
    }
    else {
        colorComboBox->setEnabled(true);
        grabberController->setAutomaticColor(false);
        preferences->setIntegerPreference(deviceId, "automaticcolor", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoColor --> End";
}


void CameraControllerDialog::changeColor(int index)
{
    qDebug() << "CameraControllerDialog::changeColor --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepColor;
    int maxValue = grabberController->getColorCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setColor(value);
    preferences->setIntegerPreference(deviceId, "color", index);

    qDebug() << "CameraControllerDialog::changeColor --> End";
}


void CameraControllerDialog::changeAutoExposure(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoExposure --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        exposureComboBox->setEnabled(false);
        grabberController->setAutomaticExposure(true);
        preferences->setIntegerPreference(deviceId, "automaticexposure", true);
    }
    else {
        exposureComboBox->setEnabled(true);
        grabberController->setAutomaticExposure(false);
        preferences->setIntegerPreference(deviceId, "automaticexposure", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoExposure --> End";
}


void CameraControllerDialog::changeExposure(int index)
{
    qDebug() << "CameraControllerDialog::changeExposure --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepExposure;
    int maxValue = grabberController->getExposureCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setExposure(value);
    preferences->setIntegerPreference(deviceId, "exposure", index);

    qDebug() << "CameraControllerDialog::changeExposure --> End";
}


void CameraControllerDialog::changeAutoZoom(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoZoo --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        zoomComboBox->setEnabled(false);
        grabberController->setAutomaticZoom(true);
        preferences->setIntegerPreference(deviceId, "automaticzoom", true);
    }
    else {
        zoomComboBox->setEnabled(true);
        grabberController->setAutomaticZoom(false);
        preferences->setIntegerPreference(deviceId, "automaticzoom", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoZoo --> End";
}


void CameraControllerDialog::changeZoom(int index)
{
    qDebug() << "CameraControllerDialog::changeZoo --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepZoom;
    int maxValue = grabberController->getZoomCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setZoom(value);
    preferences->setIntegerPreference(deviceId, "zoom", index);

    qDebug() << "CameraControllerDialog::changeZoo --> End";
}


void CameraControllerDialog::changeAutoFocus(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoFocus --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        focusComboBox->setEnabled(false);
        grabberController->setAutomaticFocus(true);
        preferences->setIntegerPreference(deviceId, "automaticfocus", true);
    }
    else {
        focusComboBox->setEnabled(true);
        grabberController->setAutomaticFocus(false);
        preferences->setIntegerPreference(deviceId, "automaticfocus", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoFocus --> End";
}


void CameraControllerDialog::changeFocus(int index)
{
    qDebug() << "CameraControllerDialog::changeFocus --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepFocus;
    int maxValue = grabberController->getFocusCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setFocus(value);
    preferences->setIntegerPreference(deviceId, "focus", index);

    qDebug() << "CameraControllerDialog::changeFocus --> End";
}


void CameraControllerDialog::changeAutoPan(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoPan --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        panComboBox->setEnabled(false);
        grabberController->setAutomaticPan(true);
        preferences->setIntegerPreference(deviceId, "automaticpan", true);
    }
    else {
        panComboBox->setEnabled(true);
        grabberController->setAutomaticPan(false);
        preferences->setIntegerPreference(deviceId, "automaticpan", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoPan --> End";
}


void CameraControllerDialog::changePan(int index)
{
    qDebug() << "CameraControllerDialog::changePan --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepPan;
    int maxValue = grabberController->getPanCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setPan(value);
    preferences->setIntegerPreference(deviceId, "pan", index);

    qDebug() << "CameraControllerDialog::changePan --> End";
}


void CameraControllerDialog::changeAutoTilt(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoTilt --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        tiltComboBox->setEnabled(false);
        grabberController->setAutomaticTilt(true);
        preferences->setIntegerPreference(deviceId, "automatictilt", true);
    }
    else {
        tiltComboBox->setEnabled(true);
        grabberController->setAutomaticTilt(false);
        preferences->setIntegerPreference(deviceId, "automatictilt", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoTilt --> End";
}


void CameraControllerDialog::changeTilt(int index)
{
    qDebug() << "CameraControllerDialog::changeTilt --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepTilt;
    int maxValue = grabberController->getTiltCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setTilt(value);
    preferences->setIntegerPreference(deviceId, "tilt", index);

    qDebug() << "CameraControllerDialog::changeTilt --> End";
}


void CameraControllerDialog::changeAutoIris(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoIris --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        irisComboBox->setEnabled(false);
        grabberController->setAutomaticIris(true);
        preferences->setIntegerPreference(deviceId, "automaticiris", true);
    }
    else {
        irisComboBox->setEnabled(true);
        grabberController->setAutomaticIris(false);
        preferences->setIntegerPreference(deviceId, "automaticiris", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoIris --> End";
}


void CameraControllerDialog::changeIris(int index)
{
    qDebug() << "CameraControllerDialog::changeIris --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepIris;
    int maxValue = grabberController->getIrisCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setIris(value);
    preferences->setIntegerPreference(deviceId, "iris", index);

    qDebug() << "CameraControllerDialog::changeIris --> End";
}


void CameraControllerDialog::changeAutoRoll(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoRoll --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    if (newState) {
        rollComboBox->setEnabled(false);
        grabberController->setAutomaticRoll(true);
        preferences->setIntegerPreference(deviceId, "automaticroll", true);
    }
    else {
        rollComboBox->setEnabled(true);
        grabberController->setAutomaticRoll(false);
        preferences->setIntegerPreference(deviceId, "automaticroll", false);
    }

    qDebug() << "CameraControllerDialog::changeAutoRoll --> End";
}


void CameraControllerDialog::changeRoll(int index)
{
    qDebug() << "CameraControllerDialog::changeRoll --> Start";

    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepRoll;
    int maxValue = grabberController->getRollCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setRoll(value);
    preferences->setIntegerPreference(deviceId, "roll", index);

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
