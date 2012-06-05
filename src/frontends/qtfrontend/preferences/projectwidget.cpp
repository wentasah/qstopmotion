/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include "projectwidget.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"

#include <QtCore/QtDebug>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ProjectWidget::ProjectWidget(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    qDebug("ProjectWidget::Constructor --> Start");

    frontend               = f;

    recordingGroupBox      = 0;
    recordingModeCombo     = 0;

    cameraGroupBox         = 0;
    videoSourceCombo       = 0;
    defaultVideoSource     = 0;

    captureGroupBox        = 0;
    mixModeCombo           = 0;
    defaultMixMode         = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;
    defaultMixCount        = 0;
    defaultPlaybackCount   = 0;
    fpsChooserCaption      = 0;
    fpsChooser             = 0;
    defaultFps             = 0;

    // autoGroupBox           = 0;
    // unitModeCombo          = 0;
    // defaultUnitMode        = 0;

    this->setObjectName("ProjectWidget");

    makeGUI();

    qDebug("ProjectWidget::Constructor --> End");
}


void ProjectWidget::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;

    recordingGroupBox = new QGroupBox(tr("Recording"));
    // recordingGroupBox->setFlat(true);

    recordingModeCombo = new QComboBox();
    recordingModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(recordingModeCombo, SIGNAL(activated(int)), this, SLOT(changeRecordingMode(int)));
    recordingModeCombo->addItem(tr("Single frame capture"));
    // recordingModeCombo->addItem(tr("Automated recording"));

    QVBoxLayout *recordingLayout = new QVBoxLayout;
    // recordingLayout->setMargin(0);
    // recordingLayout->setSpacing(2);
    // recordingLayout->addStretch(1);
    recordingLayout->addWidget(recordingModeCombo);
    recordingLayout->addStretch(10);
    recordingGroupBox->setLayout(recordingLayout);

    cameraGroupBox = new QGroupBox(tr("Camera"));
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

    QVBoxLayout *cameraLayout = new QVBoxLayout;
    // cameraLayout->setMargin(0);
    // cameraLayout->setSpacing(2);
    // cameraLayout->addStretch(1);
    cameraLayout->addWidget(videoSourceCombo);
    cameraLayout->addStretch(10);
    cameraGroupBox->setLayout(cameraLayout);

    captureGroupBox = new QGroupBox(tr("Capture"));
    // secondGroupBox->setFlat(true);

    mixModeCombo = new QComboBox();
    mixModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(mixModeCombo, SIGNAL(activated(int)), this, SLOT(changeMixMode(int)));
    mixModeCombo->addItem(tr("Mix"));
    mixModeCombo->addItem(tr("Diff"));
    mixModeCombo->addItem(tr("Playback"));
    mixCountSliderCaption = new QLabel(tr("Number of images:"));
    QString infoText =
        tr("<h4>Number of images</h4> "
           "<p>By changing the value in this slidebar you can specify how many images "
           "backwards in the animation which should be mixed on top of the camera or "
           "if you are in playback mode: how many images to play. </p> "
           "<p>By mixing the previous image(s) onto the camera you can more easily see "
           "how the next shot will be in relation to the other, therby making a smoother "
           "stop motion animation!</p>");
    mixCountSliderCaption->setWhatsThis(infoText);

    mixCountSlider = new QSlider(Qt::Horizontal);
    mixCountSlider->setMinimum(0);
    mixCountSlider->setMaximum(5);
    mixCountSlider->setPageStep(1);
    mixCountSlider->setValue(2);
    mixCountSlider->setTickPosition(QSlider::TicksBelow);
    mixCountSlider->setFocusPolicy(Qt::NoFocus);
    connect(mixCountSlider, SIGNAL(valueChanged(int)), this, SLOT(changeMixCount(int)));
    mixCountSlider->setWhatsThis(infoText);

    fpsChooserCaption = new QLabel(tr("FPS chooser"));
    infoText =
        tr("<h4>FPS chooser</h4> "
           "<p>By changing the value in this "
           "chooser you set which speed the "
           "animation in the <b>FrameView</b> "
           "should run at.</p> "
           "<p>To start an animation press the "
           "<b>Run Animation</b> button.</p>");
    fpsChooserCaption->setWhatsThis(infoText);

    fpsChooser = new QSpinBox();
    fpsChooser->setMinimum(1);
    fpsChooser->setMaximum(30);
    fpsChooser->setValue(1);
    fpsChooser->setFocusPolicy(Qt::NoFocus);
    fpsChooser->setWhatsThis(infoText);

    QVBoxLayout *captureLayout = new QVBoxLayout;
    captureLayout->addWidget(mixModeCombo);
    captureLayout->addWidget(mixCountSliderCaption);
    captureLayout->addWidget(mixCountSlider);
    captureLayout->addWidget(fpsChooserCaption);
    captureLayout->addWidget(fpsChooser);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);

    /*
    autoGroupBox = new QGroupBox(tr("Auto"));
    // autoGroupBox->setFlat(true);

    unitModeCombo = new QComboBox();
    unitModeCombo->setFocusPolicy(Qt::NoFocus);
    // unitModeCombo->setEnabled(false);
    connect(unitModeCombo, SIGNAL(activated(int)), this, SLOT(changeUnitMode(int)));
    unitModeCombo->addItem("");
    unitModeCombo->addItem(tr("Pr sec"));
    unitModeCombo->addItem(tr("Pr min"));
    unitModeCombo->addItem(tr("Pr hr"));

    QVBoxLayout *autoLayout = new QVBoxLayout;
    autoLayout->addWidget(unitModeCombo);
    autoGroupBox->setLayout(autoLayout);
    */

    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(recordingGroupBox);
    tabLayout->addWidget(cameraGroupBox);
    tabLayout->addWidget(captureGroupBox);
    // tabLayout->addWidget(autoGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    qDebug("ProjectWidget::makeGUI --> End");
}


void ProjectWidget::initialize()
{
    qDebug("ProjectWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    defaultRecordingMode = pref->getBasicPreference("defaultrecordingmode", 0);
    recordingModeCombo->setCurrentIndex(defaultRecordingMode);
    changeRecordingMode(defaultRecordingMode);

    defaultVideoSource = pref->getBasicPreference("defaultvideosource", 0);
    videoSourceCombo->setCurrentIndex(defaultVideoSource);
    changeVideoSource(defaultVideoSource);

    defaultMixMode = pref->getBasicPreference("defaultmixmode", 0);
    mixModeCombo->setCurrentIndex(defaultMixMode);
    changeMixMode(defaultMixMode);

    defaultMixCount = frontend->getPreferences()->getBasicPreference("defaultmixcount", 2);
    defaultPlaybackCount = frontend->getPreferences()->getBasicPreference("defaultplaybackcount", 5);

    switch (defaultMixMode) {
    case 0:
        mixCountSlider->setValue(defaultMixCount);
        break;
    case 1:
        break;
    case 2:
        mixCountSlider->setValue(defaultPlaybackCount);
        break;
    }

    defaultFps = pref->getBasicPreference("defaultframespersecond", 10);
    fpsChooser->setValue(defaultFps);

    /*
        defaultUnitMode = pref->getBasicPreference("defaultunitmode", 0);
        unitModeCombo->setCurrentIndex(defaultUnitMode);
        changeUnitMode(defaultUnitMode);
    */

    qDebug("ProjectWidget::initialize --> End");
}

/*
void ProjectWidget::resizeEvent(QResizeEvent *event)
{
    qDebug("ProjectWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ProjectWidget::resizeEvent --> End");
}
*/

void ProjectWidget::apply()
{
    qDebug("ProjectWidget::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    int newRecordingMode = recordingModeCombo->currentIndex();
    if (defaultRecordingMode != newRecordingMode)
    {
        pref->setBasicPreference("defaultrecordingmode", newRecordingMode);
        defaultRecordingMode = newRecordingMode;
    }

    int newVideoSource = videoSourceCombo->currentIndex();
    if (defaultVideoSource != newVideoSource)
    {
        pref->setBasicPreference("defaultvideosource", newVideoSource);
        defaultVideoSource = newVideoSource;
    }

    int newMixMode = mixModeCombo->currentIndex();
    if (defaultMixMode != newMixMode)
    {
        pref->setBasicPreference("defaultmixmode", newMixMode);
        defaultMixMode = newMixMode;
    }

    int newMixCount = mixCountSlider->value();
    if (defaultMixCount != newMixCount)
    {
        switch (newMixMode) {
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
/*
    int newUnitMode = unitModeChooseCombo->currentIndex();
    if (defaultUnitMode != newUnitMode)
    {
        pref->setBasicPreference("defaultunitmode", newUnitMode);
        defaultUnitMode = newUnitMode;
    }
*/
    qDebug("ProjectWidget::apply --> End");
}


void ProjectWidget::reset()
{
    qDebug("ProjectWidget::reset --> Start");

    changeRecordingMode(defaultRecordingMode);
    changeVideoSource(defaultVideoSource);
    changeMixMode(defaultMixMode);
    this->mixCountSlider->setValue(defaultMixCount);
    this->fpsChooser->setValue(defaultFps);
/*
    changeUnitMode(defaultUnitMode);
*/
    qDebug("ProjectWidget::reset --> End");
}


void ProjectWidget::changeRecordingMode(int index)
{
    this->recordingModeCombo->setCurrentIndex(index);
}


void ProjectWidget::changeVideoSource(int index)
{
    this->videoSourceCombo->setCurrentIndex(index);
}


void ProjectWidget::changeMixMode(int index)
{
    switch (index) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(5);
        mixCountSlider->setValue(defaultMixCount);
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(50);
        mixCountSlider->setValue(defaultPlaybackCount);
        break;
    default:
        Q_ASSERT(index < 3);

        break;
    }
}


void ProjectWidget::changeMixCount(int /*sliderValue*/)
{
}

/*
void ProjectWidget::changeUnitMode(int index)
{
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
}


void ProjectWidget::updateSliderValue(int sliderValue)
{
    if (sliderValue != 0) {
        int factor = 0;
        int index = unitModeCombo->currentIndex();
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
}
*/
