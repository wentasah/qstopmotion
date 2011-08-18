/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#include "defaulttab.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"

#include <QtCore/QtDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


DefaultTab::DefaultTab(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    qDebug("DefaultTab::Constructor --> Start");

    frontend               = f;

    cameraGroupBox         = 0;
    videoSourceCombo       = 0;
    defaultVideoSource     = 0;

    captureGroupBox        = 0;
    viewingModeChooseCombo = 0;
    defaultViewingMode     = 0;
    unitModeChooseCombo    = 0;
    defaultUnitMode        = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;
    defaultMixCount        = 0;
    fpsChooserCaption      = 0;
    fpsChooser             = 0;
    defaultFps             = 0;

    this->setObjectName("DefaultTab");

    makeGUI();

    qDebug("DefaultTab::Constructor --> End");
}


void DefaultTab::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;

    cameraGroupBox = new QGroupBox(QString(tr("Camera")));
    // cameraGroupBox->setFlat(true);

    videoSourceCombo = new QComboBox();
    videoSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoSourceCombo, SIGNAL(activated(int)), this, SLOT(changeVideoSource(int)));
    videoSourceCombo->addItem(tr("Test Source"));
#ifdef Q_WS_X11
    videoSourceCombo->addItem(tr("USB Source"));
    videoSourceCombo->addItem(tr("FireWire Source"));
#endif
#ifdef Q_WS_WIN
    videoSourceCombo->addItem(tr("DirectShow Source"));
#endif
#ifdef Q_WS_MAC
#endif

    QVBoxLayout *firstLayout = new QVBoxLayout;
    // firstLayout->setMargin(0);
    // firstLayout->setSpacing(2);
    // firstLayout->addStretch(1);
    firstLayout->addWidget(videoSourceCombo);
    firstLayout->addStretch(10);
    cameraGroupBox->setLayout(firstLayout);

    captureGroupBox = new QGroupBox(QString(tr("Capture")));
    // secondGroupBox->setFlat(true);

    viewingModeChooseCombo = new QComboBox();
    viewingModeChooseCombo->setFocusPolicy(Qt::NoFocus);
    connect(viewingModeChooseCombo, SIGNAL(activated(int)), this, SLOT(changeViewingMode(int)));

    unitModeChooseCombo = new QComboBox();
    unitModeChooseCombo->setFocusPolicy(Qt::NoFocus);
    // unitModeChooseCombo->setEnabled(false);
    connect(unitModeChooseCombo, SIGNAL(activated(int)), this, SLOT(changeUnitMode(int)));

    mixCountSliderCaption = new QLabel();

    mixCountSlider = new QSlider(Qt::Horizontal);
    mixCountSlider->setMinimum(0);
    mixCountSlider->setMaximum(5);
    mixCountSlider->setPageStep(1);
    mixCountSlider->setValue(2);
    mixCountSlider->setTickPosition(QSlider::TicksBelow);
    mixCountSlider->setFocusPolicy(Qt::NoFocus);
    connect(mixCountSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSliderValue(int)));

    fpsChooserCaption = new QLabel();

    fpsChooser = new QSpinBox();
    fpsChooser->setMinimum(1);
    fpsChooser->setMaximum(30);
    fpsChooser->setValue(1);
    fpsChooser->setFocusPolicy(Qt::NoFocus);
    fpsChooser->setValue(frontend->getPreferences()->getBasicPreference("defaultframespersecond", 10));

    QVBoxLayout *secondLayout = new QVBoxLayout;
    secondLayout->addWidget(viewingModeChooseCombo);
    secondLayout->addWidget(unitModeChooseCombo);
    secondLayout->addWidget(mixCountSliderCaption);
    secondLayout->addWidget(mixCountSlider);
    secondLayout->addWidget(fpsChooserCaption);
    secondLayout->addWidget(fpsChooser);
    secondLayout->addStretch(10);
    captureGroupBox->setLayout(secondLayout);
    // captureGroupBox->hide();

    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(cameraGroupBox);
    tabLayout->addWidget(captureGroupBox);
    // tabLayout->addWidget(thirdGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    qDebug("DefaultTab::makeGUI --> End");
}


void DefaultTab::initialize()
{
    qDebug("DefaultTab::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    defaultVideoSource = pref->getBasicPreference("defaultvideosource", 0);
    videoSourceCombo->setCurrentIndex(defaultVideoSource);
    changeVideoSource(defaultVideoSource);

    defaultViewingMode = pref->getBasicPreference("defaultviewingmode", 0);
    viewingModeChooseCombo->setCurrentIndex(defaultViewingMode);
    changeViewingMode(defaultViewingMode);

    defaultUnitMode = pref->getBasicPreference("defaultunitmode", 0);
    unitModeChooseCombo->setCurrentIndex(defaultUnitMode);
    changeUnitMode(defaultUnitMode);

    qDebug("DefaultTab::initialize --> End");
}

/*
void DefaultTab::resizeEvent(QResizeEvent *event)
{
    qDebug("DefaultTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("DefaultTab::resizeEvent --> End");
}
*/

void DefaultTab::apply()
{
    qDebug("DefaultTab::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    int newVideoSource = videoSourceCombo->currentIndex();
    if (defaultVideoSource != newVideoSource)
    {
        pref->setBasicPreference("defaultvideosource", newVideoSource);
        defaultVideoSource = newVideoSource;
    }

    int newViewingMode = viewingModeChooseCombo->currentIndex();
    if (defaultViewingMode != newViewingMode)
    {
        pref->setBasicPreference("defaultviewingmode", newViewingMode);
        defaultViewingMode = newViewingMode;
    }

    int newUnitMode = unitModeChooseCombo->currentIndex();
    if (defaultUnitMode != newUnitMode)
    {
        pref->setBasicPreference("defaultunitmode", newUnitMode);
        defaultUnitMode = newUnitMode;
    }

    int newMixCount = mixCountSlider->value();
    if (defaultMixCount != newMixCount)
    {
        switch (newViewingMode) {
        case 0:
            pref->setBasicPreference("defaultmixcount", newMixCount);
            break;
        case 1:
            break;
        case 2:
            pref->setBasicPreference("defaultplaybackcount", newMixCount);
            break;
        case 3:
            break;
        }

        defaultMixCount = newMixCount;
    }

    int newFps = fpsChooser->value();
    if (defaultFps != newFps)
    {
        pref->setBasicPreference("defaultframespersecond", newFps);
        defaultFps = newFps;
    }

    qDebug("DefaultTab::apply --> End");
}


void DefaultTab::reset()
{
    qDebug("DefaultTab::reset --> Start");

    changeVideoSource(defaultVideoSource);
    changeViewingMode(defaultViewingMode);
    changeUnitMode(defaultUnitMode);
    this->mixCountSlider->setValue(defaultMixCount);
    this->fpsChooser->setValue(defaultFps);

    qDebug("DefaultTab::reset --> End");
}


void DefaultTab::retranslateStrings()
{
    viewingModeChooseCombo->clear();
    viewingModeChooseCombo->addItem(tr("Mix"));
    viewingModeChooseCombo->addItem(tr("Diff"));
    viewingModeChooseCombo->addItem(tr("Playback"));
    viewingModeChooseCombo->addItem(tr("Auto"));
    viewingModeChooseCombo->setCurrentIndex(this->defaultViewingMode);

    unitModeChooseCombo->clear();
    unitModeChooseCombo->addItem("");
    unitModeChooseCombo->addItem(tr("Pr sec"));
    unitModeChooseCombo->addItem(tr("Pr min"));
    unitModeChooseCombo->addItem(tr("Pr hr"));
    unitModeChooseCombo->setCurrentIndex(this->defaultUnitMode);

    mixCountSliderCaption->setText(tr("Number of images:"));
    QString infoText =
        tr("<h4>Number of images</h4> "
           "<p>By changing the value in this slidebar you can specify how many images "
           "backwards in the animation which should be mixed on top of the camera or "
           "if you are in playback mode: how many images to play. </p> "
           "<p>By mixing the previous image(s) onto the camera you can more easily see "
           "how the next shot will be in relation to the other, therby making a smoother "
           "stop motion animation!</p>");
    mixCountSliderCaption->setWhatsThis(infoText);
    mixCountSlider->setWhatsThis(infoText);

    fpsChooserCaption->setText(tr("FPS chooser"));
    infoText =
        tr("<h4>FPS chooser</h4> "
           "<p>By changing the value in this "
           "chooser you set which speed the "
           "animation in the <b>FrameView</b> "
           "should run at.</p> "
           "<p>To start an animation press the "
           "<b>Run Animation</b> button.</p>");
    fpsChooserCaption->setWhatsThis(infoText);
    fpsChooser->setWhatsThis(infoText);
}


void DefaultTab::updateSliderValue(int /*sliderValue*/)
{
    /*
    if (sliderValue != 0) {
        int factor = 0;
        int index = unitModeChooseCombo->currentIndex();
        switch (index) {
        case 1:
            factor = 1000;
            break;
        case 2:
            factor = 60000;
            break;
        case 3:
            factor = 3600000;
            break;
        }
        captureTimer->setInterval(factor / sliderValue);
    }
    */
}


void DefaultTab::changeVideoSource(int index)
{
    this->videoSourceCombo->setCurrentIndex(index);
}


void DefaultTab::changeViewingMode(int index)
{
    switch (index) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(5);
        mixCountSlider->setValue(frontend->getPreferences()->getBasicPreference("defaultmixcount", 2));
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(50);
        mixCountSlider->setValue(frontend->getPreferences()->getBasicPreference("defaultplaybackcount", 5));
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        break;
    case 3:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(10);
        mixCountSlider->setValue(1);
        unitModeChooseCombo->setEnabled(true);
        unitModeChooseCombo->setCurrentIndex(0);
        break;
    default:
        Q_ASSERT(index <= 3);

        break;
    }
}


void DefaultTab::changeUnitMode(int /*index*/)
{
    /*
    int sliderValue = mixCountSlider->value();
    if (sliderValue == 0 || index == 0) {
        return;
    }

    int factor = 0;
    switch (index) {
    case 1:
        factor = 1000;
        break;
    case 2:
        factor = 60000;
        break;
    case 3:
        factor = 3600000;
        break;
    default:
        break;
    }
    */
}
