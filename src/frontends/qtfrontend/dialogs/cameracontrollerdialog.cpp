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

    brightnessLabel = new QLabel(tr("Brightness:"));
    brightnessComboBox = new QComboBox();
    connect(brightnessComboBox, SIGNAL(activated(int)), this, SLOT(changeBrightness(int)));
    brightnessLabel->hide();
    brightnessComboBox->hide();

    contrastLabel = new QLabel(tr("Contrast:"));
    contrastComboBox = new QComboBox();
    connect(contrastComboBox, SIGNAL(activated(int)), this, SLOT(changeContrast(int)));
    contrastLabel->hide();
    contrastComboBox->hide();

    saturationLabel = new QLabel(tr("Saturation:"));
    saturationComboBox = new QComboBox();
    connect(saturationComboBox, SIGNAL(activated(int)), this, SLOT(changeSaturation(int)));
    saturationLabel->hide();
    saturationComboBox->hide();

    hueLabel = new QLabel(tr("Hue:"));
    hueComboBox = new QComboBox();
    connect(hueComboBox, SIGNAL(activated(int)), this, SLOT(changeHue(int)));
    hueLabel->hide();
    hueComboBox->hide();

    gammaLabel = new QLabel(tr("Gamma:"));
    gammaComboBox = new QComboBox();
    connect(gammaComboBox, SIGNAL(activated(int)), this, SLOT(changeGamma(int)));
    gammaLabel->hide();
    gammaComboBox->hide();

    sharpnessLabel = new QLabel(tr("Sharpness:"));
    sharpnessComboBox = new QComboBox();
    connect(sharpnessComboBox, SIGNAL(activated(int)), this, SLOT(changeSharpness(int)));
    sharpnessLabel->hide();
    sharpnessComboBox->hide();

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

    panLabel = new QLabel(tr("Pan:"));
    panComboBox = new QComboBox();
    connect(panComboBox, SIGNAL(activated(int)), this, SLOT(changePan(int)));
    panLabel->hide();
    panComboBox->hide();

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

    qualityLayout->addWidget(brightnessLabel);
    qualityLayout->addWidget(brightnessComboBox);
    qualityLayout->addWidget(contrastLabel);
    qualityLayout->addWidget(contrastComboBox);
    qualityLayout->addWidget(saturationLabel);
    qualityLayout->addWidget(saturationComboBox);
    qualityLayout->addWidget(hueLabel);
    qualityLayout->addWidget(hueComboBox);
    qualityLayout->addWidget(gammaLabel);
    qualityLayout->addWidget(gammaComboBox);
    qualityLayout->addWidget(sharpnessLabel);
    qualityLayout->addWidget(sharpnessComboBox);
    qualityLayout->addWidget(backlightLabel);
    qualityLayout->addWidget(backlightComboBox);
    qualityLayout->addWidget(whiteCheckBox);
    qualityLayout->addWidget(whiteLabel);
    qualityLayout->addWidget(whiteComboBox);

    controlLayout->addWidget(exposureCheckBox);
    controlLayout->addWidget(exposureLabel);
    controlLayout->addWidget(exposureComboBox);
    controlLayout->addWidget(zoomCheckBox);
    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(zoomComboBox);
    controlLayout->addWidget(focusCheckBox);
    controlLayout->addWidget(focusLabel);
    controlLayout->addWidget(focusComboBox);
    controlLayout->addWidget(panLabel);
    controlLayout->addWidget(panComboBox);
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

    if (grabberController->isBrightness()) {
        brightnessLabel->show();
        brightnessComboBox->show();
        stepBrightness = fillComboBox(brightnessComboBox, grabberController->getBrightnessCaps());
        brightnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "brigthtness", brightnessComboBox->count()/2));
    }

    if (grabberController->isContrast()) {
        contrastLabel->show();
        contrastComboBox->show();
        stepContrast = fillComboBox(contrastComboBox, grabberController->getContrastCaps());
        contrastComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "contrast", contrastComboBox->count()/2));
    }

    if (grabberController->isSaturation()) {
        saturationLabel->show();
        saturationComboBox->show();
        stepSaturation = fillComboBox(saturationComboBox, grabberController->getSaturationCaps());
        saturationComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "saturation", saturationComboBox->count()/2));
    }

    if (grabberController->isHue()) {
        hueLabel->show();
        hueComboBox->show();
        stepHue = fillComboBox(hueComboBox, grabberController->getHueCaps());
        hueComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "hue", hueComboBox->count()/2));
    }

    if (grabberController->isGamma()) {
        gammaLabel->show();
        gammaComboBox->show();
        stepGamma = fillComboBox(gammaComboBox, grabberController->getGammaCaps());
        gammaComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "gamma", gammaComboBox->count()/2));
    }

    if (grabberController->isSharpness()) {
        sharpnessLabel->show();
        sharpnessComboBox->show();
        stepSharpness = fillComboBox(sharpnessComboBox, grabberController->getSharpnessCaps());
        sharpnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "sharpness", sharpnessComboBox->count()/2));
    }

    if (grabberController->isBacklight()) {
        backlightLabel->show();
        backlightComboBox->show();
        stepBacklight = fillComboBox(backlightComboBox, grabberController->getBacklightCaps());
        backlightComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "backlight", backlightComboBox->count()/2));
    }

    if (grabberController->isAutomaticWhite()) {
        whiteCheckBox->show();
        whiteCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticwhite", false));
    }
    else {
        if (grabberController->isWhite()) {
            whiteLabel->show();
            whiteComboBox->show();
            stepWhite = fillComboBox(whiteComboBox, grabberController->getWhiteCaps());
            whiteComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "white", whiteComboBox->count()/2));
        }
    }

    if (grabberController->isAutomaticExposure()) {
        exposureCheckBox->show();
        exposureCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticexposure", false));
    }
    else {
        if (grabberController->isExposure()) {
            exposureLabel->show();
            exposureComboBox->show();
            stepExposure = fillComboBox(exposureComboBox, grabberController->getExposureCaps());
            exposureComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "exposure", exposureComboBox->count()/2));
        }
    }

    if (grabberController->isAutomaticZoom()) {
        zoomCheckBox->show();
        zoomCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticzoom", false));
    }
    else {
        if (grabberController->isZoom()) {
            zoomLabel->show();
            zoomComboBox->show();
            stepZoom = fillComboBox(zoomComboBox, grabberController->getZoomCaps());
            zoomComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "zoom", zoomComboBox->count()/2));
        }
    }

    if (grabberController->isAutomaticFocus()) {
        focusCheckBox->show();
        focusCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticfocus", false));
    }
    else {
        if (grabberController->isFocus()) {
            focusLabel->show();
            focusComboBox->show();
            stepFocus = fillComboBox(focusComboBox, grabberController->getFocusCaps());
            focusComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "focus", focusComboBox->count()/2));
        }
    }

    if (grabberController->isPan()) {
        panLabel->show();
        panComboBox->show();
        stepPan = fillComboBox(panComboBox, grabberController->getPanCaps());
        panComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "pan", panComboBox->count()/2));
    }

    if (grabberController->isTilt()) {
        tiltLabel->show();
        tiltComboBox->show();
        stepTilt = fillComboBox(tiltComboBox, grabberController->getTiltCaps());
        tiltComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "tilt", tiltComboBox->count()/2));
    }
    else {
    }

    if (grabberController->isAutomaticIris()) {
        irisCheckBox->show();
        irisCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticiris", false));
    }
    else {
        if (grabberController->isIris()) {
            irisLabel->show();
            irisComboBox->show();
            stepIris = fillComboBox(irisComboBox, grabberController->getIrisCaps());
            irisComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "iris", irisComboBox->count()/2));
        }
    }

    if (grabberController->isAutomaticRoll()) {
        rollCheckBox->show();
        rollCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticroll", false));
    }
    else {
        if (grabberController->isRoll()) {
            rollLabel->show();
            rollComboBox->show();
            stepRoll = fillComboBox(rollComboBox, grabberController->getRollCaps());
            rollComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "roll", rollComboBox->count()/2));
        }
    }

    qDebug() << "CameraControllerDialog::init --> End";
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
