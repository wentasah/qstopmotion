/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include "controlerwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QRect>
#include <QSpacerItem>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"


ControlerWidget::ControlerWidget(Frontend *f, QWidget *parent) : QWidget(parent)
{
    qDebug() << "ControlerWidget::Constructor --> Start";

    frontend                 = f;

    infoText                 = 0;

    // Image preferences
    qualityPrefs             = 0;
    brightnessCheckBox       = 0;
    contrastCheckBox         = 0;
    saturationCheckBox       = 0;
    hueCheckBox              = 0;
    gammaCheckBox            = 0;
    sharpnessCheckBox        = 0;
    backlightCheckBox        = 0;
    whiteCheckBox            = 0;
    gainCheckBox             = 0;
    colorCheckBox            = 0;

    // Live view preferences
    controlPrefs             = 0;
    exposureCheckBox         = 0;
    zoomCheckBox             = 0;
    focusCheckBox            = 0;
    panCheckBox              = 0;
    tiltCheckBox             = 0;
    irisCheckBox             = 0;
    rollCheckBox             = 0;

    actualBrightness  = BRIGHTNESSDEFAULT;
    actualContrast    = CONTRASTDEFAULT;
    actualSaturation  = SATURATIONDEFAULT;
    actualHue         = HUEDEFAULT;
    actualGamma       = GAMMADEFAULT;
    actualSharpness   = SHARPNESSDEFAULT;
    actualBacklight   = BACKLIGHTDEFAULT;
    actualWhite       = WHITEDEFAULT;
    actualGain        = GAINDEFAULT;
    actualColor       = COLORDEFAULT;

    actualExposure    = EXPOSUREDEFAULT;
    actualZoom        = ZOOMDEFAULT;
    actualFocus       = FOCUSDEFAULT;
    actualPan         = PANDEFAULT;
    actualTilt        = TILTDEFAULT;
    actualIris        = IRISDEFAULT;
    actualRoll        = ROLLDEFAULT;

    this->setObjectName("ControlerWidget");

    makeGUI();

    qDebug() << "ControlerWidget::Constructor --> End";
}


void ControlerWidget::makeGUI()
{
    qDebug() << "ControlerWidget::makeGUI --> Start";

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p>" +
        tr("Below you can set which features of the camera "
           "should be used in the camera control dialog.") +
        "</p>");

    infoText->setMinimumWidth(440);
    infoText->setMinimumHeight(55);
    infoText->setMaximumHeight(60);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Image import preferences
    qualityPrefs = new QGroupBox;
    qualityPrefs->setTitle(tr("Image quality controls"));

    brightnessCheckBox = new QCheckBox(tr("Brightness control"));
    brightnessCheckBox->setChecked(false);

    contrastCheckBox = new QCheckBox(tr("Contrast control"));
    contrastCheckBox->setChecked(false);

    saturationCheckBox = new QCheckBox(tr("Saturation control"));
    saturationCheckBox->setChecked(false);

    hueCheckBox = new QCheckBox(tr("Hue control"));
    hueCheckBox->setChecked(false);

    gammaCheckBox = new QCheckBox(tr("Gamma control"));
    gammaCheckBox->setChecked(false);

    sharpnessCheckBox = new QCheckBox(tr("Sharpness control"));
    sharpnessCheckBox->setChecked(false);

    backlightCheckBox = new QCheckBox(tr("Backlight"));
    backlightCheckBox->setChecked(false);

    whiteCheckBox = new QCheckBox(tr("White control"));
    whiteCheckBox->setChecked(false);

    gainCheckBox = new QCheckBox(tr("Gain control"));
    gainCheckBox->setChecked(false);

    colorCheckBox = new QCheckBox(tr("Color control"));
    colorCheckBox->setChecked(false);

    QGridLayout *qualityPrefsLayout = new QGridLayout;
    // qualityPrefsLayout->setColumnStretch(0, 1);
    qualityPrefsLayout->addWidget(brightnessCheckBox, 0, 0, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(contrastCheckBox, 0, 1, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(saturationCheckBox, 1, 0, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(hueCheckBox, 1, 1, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(gammaCheckBox, 2, 0, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(sharpnessCheckBox, 2, 1, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(backlightCheckBox, 3, 0, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(whiteCheckBox, 3, 1, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(gainCheckBox, 4, 0, Qt::AlignLeft);
    qualityPrefsLayout->addWidget(colorCheckBox, 4, 1, Qt::AlignLeft);
    qualityPrefs->setLayout(qualityPrefsLayout);

    // Live view preferences
    controlPrefs = new QGroupBox;
    controlPrefs->setTitle(tr("Camera controls"));

    exposureCheckBox = new QCheckBox(tr("Exposure control"));
    exposureCheckBox->setChecked(false);

    zoomCheckBox = new QCheckBox(tr("Zoom control"));
    zoomCheckBox->setChecked(false);

    focusCheckBox = new QCheckBox(tr("Focus control"));
    focusCheckBox->setChecked(false);

    panCheckBox = new QCheckBox(tr("Pan control"));
    panCheckBox->setChecked(false);

    tiltCheckBox = new QCheckBox(tr("Tilt control"));
    tiltCheckBox->setChecked(false);

    irisCheckBox = new QCheckBox(tr("Iris control"));
    irisCheckBox->setChecked(false);

    rollCheckBox = new QCheckBox(tr("Roll control"));
    rollCheckBox->setChecked(false);

    QGridLayout *controlPrefsLayout = new QGridLayout;
    // controlPrefsLayout->setColumnStretch(0, 1);
    controlPrefsLayout->addWidget(exposureCheckBox, 0, 0, Qt::AlignLeft);
    controlPrefsLayout->addWidget(zoomCheckBox, 0, 1, Qt::AlignLeft);
    controlPrefsLayout->addWidget(focusCheckBox, 1, 0, Qt::AlignLeft);
    controlPrefsLayout->addWidget(panCheckBox, 1, 1, Qt::AlignLeft);
    controlPrefsLayout->addWidget(tiltCheckBox, 2, 0, Qt::AlignLeft);
    controlPrefsLayout->addWidget(irisCheckBox, 2, 1, Qt::AlignLeft);
    controlPrefsLayout->addWidget(rollCheckBox, 3, 0, Qt::AlignLeft);
    controlPrefs->setLayout(controlPrefsLayout);

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(qualityPrefs);
    mainLayout->addWidget(controlPrefs);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "ControlerWidget::makeGUI --> End";
}


void ControlerWidget::initialize()
{
    qDebug() << "ControlerWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();

    pref->getBooleanPreference("preferences", "brightnesscontrol", actualBrightness);
    pref->getBooleanPreference("preferences", "contrastcontrol", actualContrast);
    pref->getBooleanPreference("preferences", "saturationcontrol", actualSaturation);
    pref->getBooleanPreference("preferences", "huecontrol", actualHue);
    pref->getBooleanPreference("preferences", "gammacontrol", actualGamma);
    pref->getBooleanPreference("preferences", "sharpnesscontrol", actualSharpness);
    pref->getBooleanPreference("preferences", "backlightcontrol", actualBacklight);
    pref->getBooleanPreference("preferences", "whitecontrol", actualWhite);
    pref->getBooleanPreference("preferences", "gaincontrol", actualGain);
    pref->getBooleanPreference("preferences", "colorcontrol", actualColor);

    pref->getBooleanPreference("preferences", "exposurecontrol", actualExposure);
    pref->getBooleanPreference("preferences", "zoomcontrol", actualZoom);
    pref->getBooleanPreference("preferences", "focuscontrol", actualFocus);
    pref->getBooleanPreference("preferences", "pancontrol", actualPan);
    pref->getBooleanPreference("preferences", "tiltcontrol", actualTilt);
    pref->getBooleanPreference("preferences", "iriscontrol", actualIris);
    pref->getBooleanPreference("preferences", "rollcontrol", actualRoll);

    reset();

    qDebug() << "ControlerWidget::initialize --> End";
}

/*
void ControlerWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "ControlerWidget::resizeEvent --> Start";

    QWidget::resizeEvent(event);

    qDebug() << "ControlerWidget::resizeEvent --> End";
}
*/

void ControlerWidget::apply()
{
    qDebug() << "ControlerWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    bool             boolValue;

    boolValue = brightnessCheckBox->isChecked();
    if (boolValue != actualBrightness) {
        pref->setBooleanPreference("preferences", "brightnesscontrol", boolValue);
        actualBrightness = boolValue;
    }

    boolValue = contrastCheckBox->isChecked();
    if (boolValue != actualContrast) {
        pref->setBooleanPreference("preferences", "contrastcontrol", boolValue);
        actualContrast = boolValue;
    }

    boolValue = saturationCheckBox->isChecked();
    if (boolValue != actualSaturation) {
        pref->setBooleanPreference("preferences", "saturationcontrol", boolValue);
        actualSaturation = boolValue;
    }

    boolValue = hueCheckBox->isChecked();
    if (boolValue != actualHue) {
        pref->setBooleanPreference("preferences", "huecontrol", boolValue);
        actualHue = boolValue;
    }

    boolValue = gammaCheckBox->isChecked();
    if (boolValue != actualGamma) {
        pref->setBooleanPreference("preferences", "gammacontrol", boolValue);
        actualGamma = boolValue;
    }

    boolValue = sharpnessCheckBox->isChecked();
    if (boolValue != actualSharpness) {
        pref->setBooleanPreference("preferences", "sharpnesscontrol", boolValue);
        actualSharpness = boolValue;
    }

    boolValue = backlightCheckBox->isChecked();
    if (boolValue != actualBacklight) {
        pref->setBooleanPreference("preferences", "backlightcontrol", boolValue);
        actualBacklight = boolValue;
    }

    boolValue = whiteCheckBox->isChecked();
    if (boolValue != actualWhite) {
        pref->setBooleanPreference("preferences", "whitecontrol", boolValue);
        actualWhite = boolValue;
    }

    boolValue = gainCheckBox->isChecked();
    if (boolValue != actualGain) {
        pref->setBooleanPreference("preferences", "gaincontrol", boolValue);
        actualGain = boolValue;
    }

    boolValue = colorCheckBox->isChecked();
    if (boolValue != actualColor) {
        pref->setBooleanPreference("preferences", "colorcontrol", boolValue);
        actualColor = boolValue;
    }


    boolValue = exposureCheckBox->isChecked();
    if (boolValue != actualExposure) {
        pref->setBooleanPreference("preferences", "exposurecontrol", boolValue);
        actualExposure = boolValue;
    }

    boolValue = zoomCheckBox->isChecked();
    if (boolValue != actualZoom) {
        pref->setBooleanPreference("preferences", "zoomcontrol", boolValue);
        actualZoom = boolValue;
    }

    boolValue = focusCheckBox->isChecked();
    if (boolValue != actualFocus) {
        pref->setBooleanPreference("preferences", "focuscontrol", boolValue);
        actualFocus = boolValue;
    }

    boolValue = panCheckBox->isChecked();
    if (boolValue != actualPan) {
        pref->setBooleanPreference("preferences", "pancontrol", boolValue);
        actualPan = boolValue;
    }

    boolValue = tiltCheckBox->isChecked();
    if (boolValue != actualTilt) {
        pref->setBooleanPreference("preferences", "tiltcontrol", boolValue);
        actualTilt = boolValue;
    }

    boolValue = irisCheckBox->isChecked();
    if (boolValue != actualIris) {
        pref->setBooleanPreference("preferences", "iriscontrol", boolValue);
        actualIris = boolValue;
    }

    boolValue = rollCheckBox->isChecked();
    if (boolValue != actualRoll) {
        pref->setBooleanPreference("preferences", "rollcontrol", boolValue);
        actualRoll = boolValue;
    }

    qDebug() << "ControlerWidget::apply --> End";
}


void ControlerWidget::reset()
{
    qDebug() << "ControlerWidget::reset --> Start";

    brightnessCheckBox->setChecked(actualBrightness);
    contrastCheckBox->setChecked(actualContrast);
    saturationCheckBox->setChecked(actualSaturation);
    hueCheckBox->setChecked(actualHue);
    gammaCheckBox->setChecked(actualGamma);
    sharpnessCheckBox->setChecked(actualSharpness);
    backlightCheckBox->setChecked(actualBacklight);
    whiteCheckBox->setChecked(actualWhite);
    gainCheckBox->setChecked(actualGain);
    colorCheckBox->setChecked(actualColor);

    exposureCheckBox->setChecked(actualExposure);
    zoomCheckBox->setChecked(actualZoom);
    focusCheckBox->setChecked(actualFocus);
    panCheckBox->setChecked(actualPan);
    tiltCheckBox->setChecked(actualTilt);
    irisCheckBox->setChecked(actualIris);
    rollCheckBox->setChecked(actualRoll);

    qDebug() << "ControlerWidget::reset --> End";
}
