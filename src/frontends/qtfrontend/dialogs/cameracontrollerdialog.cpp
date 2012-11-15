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
                                               GrabberController *controller,
                                               QWidget *parent)
    : QDialog(parent)
{
    qDebug("CameraControllerDialog::Constructor --> Start");

    frontend = f;
    grabberController = controller;
    stepBrightness = -1;
    stepContrast = -1;
    stepSaturation = -1;
    stepHue = -1;
    stepGamma = -1;
    stepSharpness = -1;
    stepBacklight = -1;
    stepExposure = -1;
    stepWhite = -1;
    stepZoom = -1;
    stepFocus = -1;
    stepPan = -1;
    stepTilt = -1;

    this->setWindowTitle(tr("qStopMotion Camera Controller"));
    this->setMinimumSize(200, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

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

    exposureCheckBox = new QCheckBox(tr("Automatic Exposure"));
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));
    exposureCheckBox->hide();

    exposureLabel = new QLabel(tr("Exposure:"));
    exposureComboBox = new QComboBox();
    connect(exposureComboBox, SIGNAL(activated(int)), this, SLOT(changeExposure(int)));
    exposureLabel->hide();
    exposureComboBox->hide();

    whiteCheckBox = new QCheckBox(tr("Automatic White Balance"));
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));
    whiteCheckBox->hide();

    whiteLabel = new QLabel(tr("White Balance:"));
    whiteComboBox = new QComboBox();
    connect(whiteComboBox, SIGNAL(activated(int)), this, SLOT(changeWhite(int)));
    whiteLabel->hide();
    whiteComboBox->hide();

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

    closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addLayout(topLayout);
    mainLayout->addWidget(brightnessLabel);
    mainLayout->addWidget(brightnessComboBox);
    mainLayout->addWidget(contrastLabel);
    mainLayout->addWidget(contrastComboBox);
    mainLayout->addWidget(saturationLabel);
    mainLayout->addWidget(saturationComboBox);
    mainLayout->addWidget(hueLabel);
    mainLayout->addWidget(hueComboBox);
    mainLayout->addWidget(gammaLabel);
    mainLayout->addWidget(gammaComboBox);
    mainLayout->addWidget(sharpnessLabel);
    mainLayout->addWidget(sharpnessComboBox);
    mainLayout->addWidget(backlightLabel);
    mainLayout->addWidget(backlightComboBox);
    mainLayout->addWidget(exposureCheckBox);
    mainLayout->addWidget(exposureLabel);
    mainLayout->addWidget(exposureComboBox);
    mainLayout->addWidget(whiteCheckBox);
    mainLayout->addWidget(whiteLabel);
    mainLayout->addWidget(whiteComboBox);
    mainLayout->addWidget(zoomCheckBox);
    mainLayout->addWidget(zoomLabel);
    mainLayout->addWidget(zoomComboBox);
    mainLayout->addWidget(focusCheckBox);
    mainLayout->addWidget(focusLabel);
    mainLayout->addWidget(focusComboBox);
    mainLayout->addWidget(panLabel);
    mainLayout->addWidget(panComboBox);
    mainLayout->addWidget(tiltLabel);
    mainLayout->addWidget(tiltComboBox);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug("CameraControllerDialog::Constructor --> End");
}


void CameraControllerDialog::init()
{
    qDebug() << "CameraControllerDialog::init --> Start";

    if (grabberController->getDevice() == NULL) {
        // No device
        return;
    }

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    // long min;
    long max;
    // long step;

    if (grabberController->isBrightness()) {
        max = grabberController->getBrightnessCaps()->getMaximum();
        brightnessLabel->show();
        brightnessComboBox->show();
        stepBrightness = fillComboBox(brightnessComboBox, max);
        brightnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "brigthtness", brightnessComboBox->count()/2));
    }

    if (grabberController->isContrast()) {
        contrastLabel->show();
        contrastComboBox->show();
        stepContrast = fillComboBox(contrastComboBox, grabberController->getMaximumContrast());
        contrastComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "contrast", contrastComboBox->count()/2));
    }

    if (grabberController->isSaturation()) {
        saturationLabel->show();
        saturationComboBox->show();
        stepSaturation = fillComboBox(saturationComboBox, grabberController->getMaximumSaturation());
        saturationComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "saturation", saturationComboBox->count()/2));
    }

    if (grabberController->isHue()) {
        hueLabel->show();
        hueComboBox->show();
        stepHue = fillComboBox(hueComboBox, grabberController->getMaximumHue());
        hueComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "hue", hueComboBox->count()/2));
    }

    if (grabberController->isGamma()) {
        gammaLabel->show();
        gammaComboBox->show();
        stepGamma = fillComboBox(gammaComboBox, grabberController->getMaximumGamma());
        gammaComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "gamma", gammaComboBox->count()/2));
    }

    if (grabberController->isSharpness()) {
        sharpnessLabel->show();
        sharpnessComboBox->show();
        stepSharpness = fillComboBox(sharpnessComboBox, grabberController->getMaximumSharpness());
        sharpnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "sharpness", sharpnessComboBox->count()/2));
    }

    if (grabberController->isBacklight()) {
        backlightLabel->show();
        backlightComboBox->show();
        stepBacklight = fillComboBox(backlightComboBox, grabberController->getMaximumSharpness());
        backlightComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "backlight", backlightComboBox->count()/2));
    }

    if (grabberController->isAutomaticExposure()) {
        exposureCheckBox->show();
        exposureCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticexposure", false));
    }
    else {
        if (grabberController->isExposure()) {
            exposureLabel->show();
            exposureComboBox->show();
            stepExposure = fillComboBox(exposureComboBox, grabberController->getMaximumExposure());
            exposureComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "exposure", exposureComboBox->count()/2));
        }
    }

    if (grabberController->isAutomaticWhite()) {
        whiteCheckBox->show();
        whiteCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticwhite", false));
    }
    else {
        if (grabberController->isWhite()) {
            whiteLabel->show();
            whiteComboBox->show();
            stepWhite = fillComboBox(whiteComboBox, grabberController->getMaximumWhite());
            whiteComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "white", whiteComboBox->count()/2));
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
            stepZoom = fillComboBox(zoomComboBox, grabberController->getMaximumZoom());
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
            stepFocus = fillComboBox(focusComboBox, grabberController->getMaximumFocus());
            focusComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "focus", focusComboBox->count()/2));
        }
    }

    if (grabberController->isPan()) {
        panLabel->show();
        panComboBox->show();
        stepPan = fillComboBox(panComboBox, grabberController->getMaximumPan());
        panComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "pan", panComboBox->count()/2));
    }

    if (grabberController->isTilt()) {
        tiltLabel->show();
        tiltComboBox->show();
        stepTilt = fillComboBox(tiltComboBox, grabberController->getMaximumTilt());
        tiltComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "tilt", tiltComboBox->count()/2));
    }
    else {
    }

    qDebug() << "CameraControllerDialog::init --> End";
}


void CameraControllerDialog::changeBrightness(int index)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepContrast;
    int maxValue = grabberController->getMaximumContrast();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSaturation;
    int maxValue = grabberController->getMaximumSaturation();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepHue;
    int maxValue = grabberController->getMaximumHue();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepGamma;
    int maxValue = grabberController->getMaximumGamma();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepSharpness;
    int maxValue = grabberController->getMaximumSharpness();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepBacklight;
    int maxValue = grabberController->getMaximumBacklight();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBacklight(value);
    preferences->setIntegerPreference(deviceId, "backlight", index);

    qDebug() << "CameraControllerDialog::changeBacklight --> End";
}


void CameraControllerDialog::changeAutoExposure(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoExposure --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepExposure;
    int maxValue = grabberController->getMaximumExposure();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setExposure(value);
    preferences->setIntegerPreference(deviceId, "exposure", index);

    qDebug() << "CameraControllerDialog::changeExposure --> End";
}


void CameraControllerDialog::changeAutoWhite(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoWhite --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepWhite;
    int maxValue = grabberController->getMaximumWhite();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setWhite(value);
    preferences->setIntegerPreference(deviceId, "white", index);

    qDebug() << "CameraControllerDialog::changeWhite --> End";
}


void CameraControllerDialog::changeAutoZoom(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoZoo --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepZoom;
    int maxValue = grabberController->getMaximumZoom();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepFocus;
    int maxValue = grabberController->getMaximumFocus();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepPan;
    int maxValue = grabberController->getMaximumPan();

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

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepTilt;
    int maxValue = grabberController->getMaximumTilt();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setTilt(value);
    preferences->setIntegerPreference(deviceId, "tilt", index);

    qDebug() << "CameraControllerDialog::changeTilt --> End";
}


int CameraControllerDialog::fillComboBox(QComboBox *comboBox, int maxValue)
{
    int step;
    int value;

    step = maxValue / 10;
    if (step < 1) {
        step = 1;
    }
    for (value = 0 ; value <= maxValue ; value += step) {
        comboBox->addItem(QString("%1").arg(value));
    }
    if (value < maxValue) {
        comboBox->addItem(QString("%1").arg(maxValue));
    }

    return step;
}
