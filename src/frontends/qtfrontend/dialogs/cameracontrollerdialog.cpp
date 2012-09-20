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

    this->setWindowTitle(tr("qStopMotion Camera Controller"));
    this->setMinimumSize(200, 500);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    brightnessLabel = new QLabel(tr("Brightness:"));
    brightnessComboBox = new QComboBox();
    for (int i = 0 ; i <= 127 ; i+=8) {
        brightnessComboBox->addItem(QString("%1").arg(i));
    }
    brightnessComboBox->addItem(QString("%1").arg(127));
    connect(brightnessComboBox, SIGNAL(activated(int)), this, SLOT(changeBrightness(int)));

    contrastLabel = new QLabel(tr("Contrast:"));
    contrastComboBox = new QComboBox();
    for (int i = 0 ; i <= 127 ; i+=8) {
        contrastComboBox->addItem(QString("%1").arg(i));
    }
    contrastComboBox->addItem(QString("%1").arg(127));
    connect(contrastComboBox, SIGNAL(activated(int)), this, SLOT(changeContrast(int)));

    saturationLabel = new QLabel(tr("Saturation:"));
    saturationComboBox = new QComboBox();
    for (int i = 0 ; i <= 127 ; i+=8) {
        saturationComboBox->addItem(QString("%1").arg(i));
    }
    saturationComboBox->addItem(QString("%1").arg(127));
    connect(saturationComboBox, SIGNAL(activated(int)), this, SLOT(changeSaturation(int)));

    hueLabel = new QLabel(tr("Hue:"));
    hueComboBox = new QComboBox();
    for (int i = 0 ; i <= 180 ; i+=18) {
        hueComboBox->addItem(QString("%1").arg(i));
    }
    connect(hueComboBox, SIGNAL(activated(int)), this, SLOT(changeHue(int)));

    gammaLabel = new QLabel(tr("Gamma:"));
    gammaComboBox = new QComboBox();
    for (int i = 0 ; i <= 277 ; i+=17) {
        gammaComboBox->addItem(QString("%1").arg(i));
    }
    gammaComboBox->addItem(QString("%1").arg(277));
    connect(gammaComboBox, SIGNAL(activated(int)), this, SLOT(changeGamma(int)));

    sharpnessLabel = new QLabel(tr("Sharpness:"));
    sharpnessComboBox = new QComboBox();
    for (int i = 0 ; i <= 11 ; i++) {
        sharpnessComboBox->addItem(QString("%1").arg(i));
    }
    connect(sharpnessComboBox, SIGNAL(activated(int)), this, SLOT(changeSharpness(int)));

    backlightLabel = new QLabel(tr("Backlight Compensation:"));
    backlightComboBox = new QComboBox();
    for (int i = 0 ; i <= 2 ; i++) {
        backlightComboBox->addItem(QString("%1").arg(i));
    }
    connect(backlightComboBox, SIGNAL(activated(int)), this, SLOT(changeBacklight(int)));

    exposureCheckBox = new QCheckBox(tr("Automatic Exposure"));
    exposureCheckBox->setChecked(false);
    connect(exposureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoExposure(int)));

    exposureComboBox = new QComboBox();
    for (int i = 0 ; i <= 10 ; i++) {
        exposureComboBox->addItem(QString("%1").arg(i));
    }
    connect(exposureComboBox, SIGNAL(activated(int)), this, SLOT(changeExposure(int)));

    whiteCheckBox = new QCheckBox(tr("Automatic White Balance"));
    whiteCheckBox->setChecked(false);
    connect(whiteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoWhite(int)));

    whiteComboBox = new QComboBox();
    for (int i = 0 ; i <= 10 ; i++) {
        whiteComboBox->addItem(QString("%1").arg(i));
    }
    connect(whiteComboBox, SIGNAL(activated(int)), this, SLOT(changeWhite(int)));

    zoomCheckBox = new QCheckBox(tr("Automatic Zoom"));
    zoomCheckBox->setChecked(false);
    connect(zoomCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoZoom(int)));

    zoomComboBox = new QComboBox();
    for (int i = 0 ; i <= 10 ; i++) {
        zoomComboBox->addItem(QString("%1").arg(i));
    }
    connect(zoomComboBox, SIGNAL(activated(int)), this, SLOT(changeZoom(int)));

    focusCheckBox = new QCheckBox(tr("Automatic Focus"));
    focusCheckBox->setChecked(false);
    connect(focusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeAutoFocus(int)));

    focusComboBox = new QComboBox();
    for (int i = 0 ; i <= 10 ; i++) {
        focusComboBox->addItem(QString("%1").arg(i));
    }
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
    mainLayout->addWidget(exposureComboBox);
    mainLayout->addWidget(whiteCheckBox);
    mainLayout->addWidget(whiteComboBox);
    mainLayout->addWidget(zoomCheckBox);
    mainLayout->addWidget(zoomComboBox);
    mainLayout->addWidget(focusCheckBox);
    mainLayout->addWidget(focusComboBox);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    qDebug("CameraControllerDialog::Constructor --> End");
}


void CameraControllerDialog::init()
{
    qDebug() << "CameraControllerDialog::init --> Start";

    if (!grabberController->isBrightness()) {
        brightnessLabel->hide();
        brightnessComboBox->hide();
    }

    qDebug() << "CameraControllerDialog::init --> End";
}


void CameraControllerDialog::changeBrightness(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeBrightness --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeBrightness --> End";
}


void CameraControllerDialog::changeContrast(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeContrast --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeContrast --> End";
}


void CameraControllerDialog::changeSaturation(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeSaturation --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeSaturation --> End";
}


void CameraControllerDialog::changeHue(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeHue --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeHue --> End";
}


void CameraControllerDialog::changeGamma(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeGamma --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeGamma --> End";
}


void CameraControllerDialog::changeSharpness(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeSharpness --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeSharpness --> End";
}


void CameraControllerDialog::changeBacklight(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeBacklight --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeBacklight --> End";
}


void CameraControllerDialog::changeAutoExposure(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoExposure --> Start";

    if (newState) {
        exposureComboBox->setEnabled(false);
        // controller->setAutoExposure(true);
    }
    else {
        exposureComboBox->setEnabled(true);
        // controller->setAutoExposure(false);
    }

    qDebug() << "CameraControllerDialog::changeAutoExposure --> End";
}


void CameraControllerDialog::changeExposure(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeExposure --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeExposure --> End";
}


void CameraControllerDialog::changeAutoWhite(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoWhite --> Start";

    if (newState) {
        whiteComboBox->setEnabled(false);
        // controller->setAutoWhite(true);
    }
    else {
        whiteComboBox->setEnabled(true);
        // controller->setAutoWhite(false);
    }

    qDebug() << "CameraControllerDialog::changeAutoWhite --> End";
}


void CameraControllerDialog::changeWhite(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeWhite --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeWhite --> End";
}


void CameraControllerDialog::changeAutoZoom(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoZoo --> Start";

    if (newState) {
        zoomComboBox->setEnabled(false);
        // controller->setAutoZoom(true);
    }
    else {
        zoomComboBox->setEnabled(true);
        // controller->setAutoZoom(false);
    }

    qDebug() << "CameraControllerDialog::changeAutoZoo --> End";
}


void CameraControllerDialog::changeZoom(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeZoo --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeZoo --> End";
}


void CameraControllerDialog::changeAutoFocus(int newState)
{
    qDebug() << "CameraControllerDialog::changeAutoFocus --> Start";

    if (newState) {
        focusComboBox->setEnabled(false);
        // controller->setAutoFocus(true);
    }
    else {
        focusComboBox->setEnabled(true);
        // controller->setAutoFocus(false);
    }

    qDebug() << "CameraControllerDialog::changeAutoFocus --> End";
}


void CameraControllerDialog::changeFocus(int /*index*/)
{
    qDebug() << "CameraControllerDialog::changeFocus --> Start (Empty)";

    // qDebug() << "CameraControllerDialog::changeFocus --> End";
}
