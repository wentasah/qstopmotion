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

    this->setWindowTitle(tr("qStopMotion Camera Controller"));
    this->setMinimumSize(200, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    brightnessLabel = new QLabel(tr("Brightness:"));
    brightnessComboBox = new QComboBox();
    connect(brightnessComboBox, SIGNAL(activated(int)), this, SLOT(changeBrightness(int)));

    contrastLabel = new QLabel(tr("Contrast:"));
    contrastComboBox = new QComboBox();
    connect(contrastComboBox, SIGNAL(activated(int)), this, SLOT(changeContrast(int)));

    saturationLabel = new QLabel(tr("Saturation:"));
    saturationComboBox = new QComboBox();
    connect(saturationComboBox, SIGNAL(activated(int)), this, SLOT(changeSaturation(int)));

    hueLabel = new QLabel(tr("Hue:"));
    hueComboBox = new QComboBox();
    connect(hueComboBox, SIGNAL(activated(int)), this, SLOT(changeHue(int)));

    gammaLabel = new QLabel(tr("Gamma:"));
    gammaComboBox = new QComboBox();
    connect(gammaComboBox, SIGNAL(activated(int)), this, SLOT(changeGamma(int)));

    sharpnessLabel = new QLabel(tr("Sharpness:"));
    sharpnessComboBox = new QComboBox();
    connect(sharpnessComboBox, SIGNAL(activated(int)), this, SLOT(changeSharpness(int)));

    backlightLabel = new QLabel(tr("Backlight Compensation:"));
    backlightComboBox = new QComboBox();
    connect(backlightComboBox, SIGNAL(activated(int)), this, SLOT(changeBacklight(int)));

    exposureCheckBox = new QCheckBox(tr("Automatic Exposure"));
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));

    exposureLabel = new QLabel(tr("Exposure:"));
    exposureComboBox = new QComboBox();
    connect(exposureComboBox, SIGNAL(activated(int)), this, SLOT(changeExposure(int)));

    whiteCheckBox = new QCheckBox(tr("Automatic White Balance"));
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));

    whiteLabel = new QLabel(tr("White Balance:"));
    whiteComboBox = new QComboBox();
    connect(whiteComboBox, SIGNAL(activated(int)), this, SLOT(changeWhite(int)));

    zoomCheckBox = new QCheckBox(tr("Automatic Zoom"));
    zoomCheckBox->setChecked(false);
    connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoZoom(int)));

    zoomLabel = new QLabel(tr("Zoom:"));
    zoomComboBox = new QComboBox();
    connect(zoomComboBox, SIGNAL(activated(int)), this, SLOT(changeZoom(int)));

    focusCheckBox = new QCheckBox(tr("Automatic Focus"));
    focusCheckBox->setChecked(false);
    connect(focusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoFocus(int)));

    focusLabel = new QLabel(tr("Focus:"));
    focusComboBox = new QComboBox();
    connect(focusComboBox, SIGNAL(activated(int)), this, SLOT(changeFocus(int)));

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
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug("CameraControllerDialog::Constructor --> End");
}


void CameraControllerDialog::init()
{
    qDebug() << "CameraControllerDialog::init --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();

    if (grabberController->isBrightness()) {
        stepBrightness = fillComboBox(brightnessComboBox, grabberController->getMaximumBrightness());
        brightnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "brigthtness", brightnessComboBox->count()/2));
    }
    else {
        brightnessLabel->hide();
        brightnessComboBox->hide();
    }

    if (grabberController->isContrast()) {
        stepContrast = fillComboBox(contrastComboBox, grabberController->getMaximumContrast());
        contrastComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "contrast", contrastComboBox->count()/2));
    }
    else {
        contrastLabel->hide();
        contrastComboBox->hide();
    }

    if (grabberController->isSaturation()) {
        stepSaturation = fillComboBox(saturationComboBox, grabberController->getMaximumSaturation());
        saturationComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "saturation", saturationComboBox->count()/2));
    }
    else {
        saturationLabel->hide();
        saturationComboBox->hide();
    }

    if (grabberController->isHue()) {
        stepHue = fillComboBox(hueComboBox, grabberController->getMaximumHue());
        hueComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "hue", hueComboBox->count()/2));
    }
    else {
        hueLabel->hide();
        hueComboBox->hide();
    }

    if (grabberController->isGamma()) {
        stepGamma = fillComboBox(gammaComboBox, grabberController->getMaximumGamma());
        gammaComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "gamma", gammaComboBox->count()/2));
    }
    else {
        gammaLabel->hide();
        gammaComboBox->hide();
    }

    if (grabberController->isSharpness()) {
        stepSharpness = fillComboBox(sharpnessComboBox, grabberController->getMaximumSharpness());
        sharpnessComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "sharpness", sharpnessComboBox->count()/2));
    }
    else {
        sharpnessLabel->hide();
        sharpnessComboBox->hide();
    }

    if (grabberController->isBacklight()) {
        stepBacklight = fillComboBox(backlightComboBox, grabberController->getMaximumSharpness());
        backlightComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "backlight", backlightComboBox->count()/2));
    }
    else {
        backlightLabel->hide();
        backlightComboBox->hide();
    }

    if (grabberController->isAutomaticExposure()) {
        exposureLabel->hide();
        exposureCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticexposure", false));
    }
    else {
        exposureCheckBox->hide();
    }
    if (grabberController->isExposure()) {
        stepExposure = fillComboBox(exposureComboBox, grabberController->getMaximumExposure());
        exposureComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "exposure", exposureComboBox->count()/2));
    }
    else {
        exposureLabel->hide();
        exposureComboBox->hide();
    }

    if (grabberController->isAutomaticWhite()) {
        whiteLabel->hide();
        whiteCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticwhite", false));
    }
    else {
        whiteCheckBox->hide();
    }
    if (grabberController->isWhite()) {
        stepWhite = fillComboBox(whiteComboBox, grabberController->getMaximumWhite());
        whiteComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "white", whiteComboBox->count()/2));
    }
    else {
        whiteLabel->hide();
        whiteComboBox->hide();
    }

    if (grabberController->isAutomaticZoom()) {
        zoomLabel->hide();
        zoomCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticzoom", false));
    }
    else {
        zoomCheckBox->hide();
    }
    if (grabberController->isZoom()) {
        stepZoom = fillComboBox(zoomComboBox, grabberController->getMaximumZoom());
        zoomComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "zoom", zoomComboBox->count()/2));
    }
    else {
        zoomLabel->hide();
        zoomComboBox->hide();
    }

    if (grabberController->isAutomaticFocus()) {
        focusLabel->hide();
        focusCheckBox->setChecked(preferences->getIntegerPreference(deviceId, "automaticfocus", false));
    }
    else {
        focusCheckBox->hide();
    }
    if (grabberController->isFocus()) {
        stepFocus = fillComboBox(focusComboBox, grabberController->getMaximumFocus());
        focusComboBox->setCurrentIndex(preferences->getIntegerPreference(deviceId, "focus", focusComboBox->count()/2));
    }
    else {
        focusLabel->hide();
        focusComboBox->hide();
    }

    qDebug() << "CameraControllerDialog::init --> End";
}


void CameraControllerDialog::changeBrightness(int index)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start";

    QString deviceId = grabberController->getDevice()->getDeviceId();
    PreferencesTool *preferences = frontend->getPreferences();
    int value = index * stepBrightness;
    int maxValue = grabberController->getMaximumBrightness();

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
