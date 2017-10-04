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

#include "projectwidget.h"

#include <QDebug>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QVBoxLayout>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"


ProjectWidget::ProjectWidget(Frontend *f, bool type, QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "ProjectWidget::Constructor --> Start";

    Q_ASSERT(type == 1);   // Use this widget only in general dialog!

    frontend               = f;
    tabType                = type;

    recordingGroupBox      = 0;
    recordingModeCombo     = 0;

    grabberGroupBox        = 0;
    grabberSourceCombo     = 0;
    defaultGrabberSource   = 0;

    captureGroupBox        = 0;
    mixModeCombo           = 0;
    defaultMixMode         = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;
    defaultMixCount        = 0;
    defaultPlaybackCount   = 0;

    timelapseGroupBox      = 0;
    unitModeComboCaption   = 0;
    unitModeCombo          = 0;
    unitCountSliderValue   = 0;
    unitCountSliderCaption = 0;
    unitCountSlider        = 0;
    beepCheckBox           = 0;
    beepCountSliderValue   = 0;
    beepCountSliderCaption = 0;
    beepCountSlider        = 0;
    defaultUnitMode        = 0;
    defaultUnitCount       = 0;
    defaultBeepCheck       = false;
    defaultBeepCount       = 0;

    this->setObjectName("ProjectWidget");

    makeGUI();

    qDebug() << "ProjectWidget::Constructor --> End";
}


void ProjectWidget::makeGUI()
{
    qDebug() << "ProjectWidget::makeGUI --> Start";

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // ========================================================================
    // Camera group box
    grabberGroupBox = new QGroupBox;
    grabberGroupBox->setTitle(tr("Image grabber settings"));
    // grabberGroupBox->setFlat(true);

    grabberSourceCombo = new QComboBox();
    grabberSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(grabberSourceCombo, SIGNAL(activated(int)), this, SLOT(changeGrabberSource(int)));
#ifdef Q_OS_LINUX
    grabberSourceCombo->addItem(tr("Video 4 Linux 2 (USB WebCam)"));
    grabberSourceCombo->addItem(tr("gphoto (USB Compact Camera)"));
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    grabberSourceCombo->addItem(tr("Media Foundation"));
#endif

    QVBoxLayout *grabberLayout = new QVBoxLayout;
    grabberLayout->addWidget(grabberSourceCombo);
    grabberLayout->addStretch(10);
    grabberGroupBox->setLayout(grabberLayout);
    mainLayout->addWidget(grabberGroupBox);

    // ========================================================================
    // Recording group box
    recordingGroupBox = new QGroupBox(tr("Recording"));

    recordingModeCombo = new QComboBox();
    recordingModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(recordingModeCombo, SIGNAL(activated(int)), this, SLOT(changeRecordingMode(int)));
    recordingModeCombo->addItem(tr("Single frame capture"));
    recordingModeCombo->addItem(tr("Time-lapse capture"));

    QVBoxLayout *recordingLayout = new QVBoxLayout;
    // recordingLayout->setMargin(0);
    // recordingLayout->setSpacing(2);
    // recordingLayout->addStretch(1);
    recordingLayout->addWidget(recordingModeCombo);
    recordingLayout->addStretch(10);
    recordingGroupBox->setLayout(recordingLayout);
    mainLayout->addWidget(recordingGroupBox);

    // ========================================================================
    // Capture group box
    captureGroupBox = new QGroupBox(tr("Capture"));

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

    mixCountSlider = new QwtSlider();
    mixCountSlider->setOrientation(Qt::Horizontal);
    mixCountSlider->setScalePosition(QwtSlider::LeadingScale);
    mixCountSlider->setGroove(true);
    mixCountSlider->setScale(0.0, 5.0);
    mixCountSlider->setTotalSteps(5);
    mixCountSlider->setSingleSteps(1);
    connect(mixCountSlider, SIGNAL(sliderReleased()), this, SLOT(changeMixCount()));
    mixCountSlider->setWhatsThis(infoText);

    QVBoxLayout *captureLayout = new QVBoxLayout;
    captureLayout->addWidget(mixModeCombo);
    captureLayout->addWidget(mixCountSliderCaption);
    captureLayout->addWidget(mixCountSlider);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);
    mainLayout->addWidget(captureGroupBox);

    // ========================================================================
    // Time-lapse group box
    timelapseGroupBox = new QGroupBox(tr("Time-lapse"));

    unitModeComboCaption = new QLabel(tr("Unit:"));

    unitModeCombo = new QComboBox();
    unitModeCombo->setFocusPolicy(Qt::NoFocus);
    unitModeCombo->addItem(tr("Seconds"));
    unitModeCombo->addItem(tr("Minutes"));
    unitModeCombo->addItem(tr("Hours"));
    unitModeCombo->addItem(tr("Days"));
    unitModeCombo->setCurrentIndex(defaultUnitMode);
    connect(unitModeCombo, SIGNAL(activated(int)), this, SLOT(changeUnitMode(int)));

    // "n Seconds between pictures"
    unitCountSliderValue = new QLabel("0");
    unitCountSliderCaption = new QLabel("unitCountSliderCaption");

    unitCountSlider = new QwtSlider();
    unitCountSlider->setOrientation(Qt::Horizontal);
    unitCountSlider->setScalePosition(QwtSlider::LeadingScale);
    unitCountSlider->setGroove(true);
    unitCountSlider->setSingleSteps(1);
    connect(unitCountSlider, SIGNAL(sliderReleased()), this, SLOT(changeUnitCount()));

    // beepCountSliderCaption->setText(tr("Beep ?? seconds before picture:"));
    beepCheckBox = new QCheckBox(tr("Beep"));
    beepCheckBox->setChecked(false);
    connect(beepCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeBeep(int)));

    beepCountSliderValue = new QLabel("0");
    beepCountSliderCaption = new QLabel(tr("seconds before picture:"));

    beepCountSlider = new QwtSlider();
    beepCountSlider->setOrientation(Qt::Horizontal);
    beepCountSlider->setScalePosition(QwtSlider::LeadingScale);
    beepCountSlider->setGroove(true);
    beepCountSlider->setSingleSteps(1);
    connect(beepCountSlider, SIGNAL(sliderReleased()), this, SLOT(changeBeepCount()));

    QHBoxLayout *unitCountCaptionLayout = new QHBoxLayout;
    unitCountCaptionLayout->addWidget(unitCountSliderValue);
    unitCountCaptionLayout->addSpacing(1);
    unitCountCaptionLayout->addWidget(unitCountSliderCaption);
    unitCountCaptionLayout->addStretch(1);

    QHBoxLayout *beepCountCaptionLayout = new QHBoxLayout;
    beepCountCaptionLayout->addWidget(beepCheckBox);
    beepCountCaptionLayout->addSpacing(1);
    beepCountCaptionLayout->addWidget(beepCountSliderValue);
    beepCountCaptionLayout->addSpacing(1);
    beepCountCaptionLayout->addWidget(beepCountSliderCaption);
    beepCountCaptionLayout->addStretch(1);

    QVBoxLayout *timelapseLayout = new QVBoxLayout;
    timelapseLayout->setMargin(4);
    // timelapseLayout->setSpacing(2);
    timelapseLayout->addWidget(unitModeComboCaption);
    timelapseLayout->addWidget(unitModeCombo);
    timelapseLayout->addSpacing(10);
    timelapseLayout->addLayout(unitCountCaptionLayout);
    timelapseLayout->addWidget(unitCountSlider);
    timelapseLayout->addSpacing(10);
    timelapseLayout->addLayout(beepCountCaptionLayout);
    timelapseLayout->addWidget(beepCountSlider);
    timelapseGroupBox->setLayout(timelapseLayout);

    mainLayout->addWidget(timelapseGroupBox);

    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "ProjectWidget::makeGUI --> End";
}


void ProjectWidget::initialize()
{
    qDebug() << "ProjectWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              intValue;
    bool             boolValue;

    if (pref->getIntegerPreference("preferences", "defaultrecordingmode", intValue) == false) {
        intValue = 0;
    }
    defaultRecordingMode = intValue;

    if (pref->getIntegerPreference("preferences", "defaultgrabbersource", intValue) == false) {
        intValue = 0;
    }
    defaultGrabberSource = intValue;

    if (pref->getIntegerPreference("preferences", "defaultmixingmode", intValue) == false) {
        intValue = 0;
    }
    defaultMixMode = intValue;

    if (pref->getIntegerPreference("preferences", "defaultmixcount", intValue) == false) {
        intValue = 0;
    }
    defaultMixCount = intValue;

    if (pref->getIntegerPreference("preferences", "defaultplaybackcount", intValue) == false) {
        intValue = 0;
    }
    defaultPlaybackCount = intValue;

    if (pref->getIntegerPreference("preferences", "defaultunitmode", intValue) == false) {
        intValue = 0;
    }
    defaultUnitMode = intValue;

    if (pref->getIntegerPreference("preferences", "defaultunitcount", intValue) == false) {
        intValue = 0;
    }
    defaultUnitCount = intValue;

    if (pref->getBooleanPreference("preferences", "defaultbeepcheck", boolValue) == false) {
        boolValue = 0;
    }
    defaultBeepCheck = boolValue;

    if (pref->getIntegerPreference("preferences", "defaultbeepcount", intValue) == false) {
        intValue = 0;
    }
    defaultBeepCount = intValue;

    reset();

    qDebug() << "ProjectWidget::initialize --> End";
}

/*
void ProjectWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "ProjectWidget::resizeEvent --> Start";

    QWidget::resizeEvent(event);

    qDebug() << "ProjectWidget::resizeEvent --> End";
}
*/

void ProjectWidget::apply()
{
    qDebug() << "ProjectWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              index;

    switch (grabberSourceCombo->currentIndex()) {
    case 0:
#ifdef Q_OS_LINUX
        index = ImageGrabberDevice::video4Linux2Source;
#endif
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
        index = ImageGrabberDevice::mediaFoundationSource;
#endif
        break;
    case 1:
#ifdef Q_OS_LINUX
        index = ImageGrabberDevice::gphoto2Source;
#endif
        break;
    }

    if (defaultGrabberSource != index) {
        defaultGrabberSource = index;
        pref->setIntegerPreference("preferences", "defaultgrabbersource", defaultGrabberSource);
    }

    int newRecordingMode = recordingModeCombo->currentIndex();
    if (defaultRecordingMode != newRecordingMode) {
        defaultRecordingMode = newRecordingMode;
        pref->setIntegerPreference("preferences", "defaultrecordingmode", defaultRecordingMode);
    }

    int newMixMode = mixModeCombo->currentIndex();
    if (defaultMixMode != newMixMode) {
        defaultMixMode = newMixMode;
        pref->setIntegerPreference("preferences", "defaultmixmode", defaultMixMode);
    }

    int newMixCount = (int)mixCountSlider->value();
    switch (newMixMode) {
    case 0:
        if (defaultMixCount != newMixCount) {
            defaultMixCount = newMixCount;
            pref->setIntegerPreference("preferences", "defaultmixcount", defaultMixCount);
        }
        break;
    case 1:
        break;
    case 2:
        if (defaultPlaybackCount != newMixCount) {
            defaultPlaybackCount = newMixCount;
            pref->setIntegerPreference("preferences", "defaultplaybackcount", defaultPlaybackCount);
        }
        break;
    case 3:
        break;
    }

    int newUnitMode = unitModeCombo->currentIndex();
    if (defaultUnitMode != newUnitMode) {
        defaultUnitMode = newUnitMode;
        pref->setIntegerPreference("preferences", "defaultunitmode", defaultUnitMode);
    }

    int newUnitCount = (int)unitCountSlider->value();
    if (defaultUnitCount != newUnitCount) {
        defaultUnitCount = newUnitCount;
        pref->setIntegerPreference("preferences", "defaultunitcount", defaultUnitCount);
    }

    bool newBeepCheck = beepCheckBox->isChecked();
    if (defaultBeepCheck != newBeepCheck) {
        defaultBeepCheck = newBeepCheck;
        pref->setBooleanPreference("preferences", "defaultbeepcheck", defaultBeepCheck);
    }

    int newBeepCount = (int)beepCountSlider->value();
    if (defaultBeepCount != newBeepCount) {
        defaultBeepCount = newBeepCount;
        pref->setIntegerPreference("preferences", "defaultbeepcount", defaultBeepCount);
    }

    qDebug() << "ProjectWidget::apply --> End";
}


void ProjectWidget::reset()
{
    qDebug() << "ProjectWidget::reset --> Start";

    setImageGrabberSource(defaultGrabberSource);

    recordingModeCombo->setCurrentIndex(defaultRecordingMode);
    changeRecordingMode(defaultRecordingMode);

    mixModeCombo->setCurrentIndex(defaultMixMode);
    changeMixMode(defaultMixMode);

    unitModeCombo->setCurrentIndex(defaultUnitMode);
    changeUnitMode(defaultUnitMode);

    unitCountSlider->setValue((double)defaultUnitCount);
    changeUnitCount();

    beepCheckBox->setChecked(defaultBeepCheck);
    changeBeep(defaultBeepCheck);

    beepCountSlider->setValue((double)defaultBeepCount);
    changeBeepCount();

    qDebug() << "ProjectWidget::reset --> End";
}


void ProjectWidget::setImageGrabberSource(int newSource)
{
    qDebug() << "ImportWidget::setImageGrabberSource --> Start";

    switch (newSource) {
    case ImageGrabberDevice::video4Linux2Source:
        grabberSourceCombo->setCurrentIndex(0);
        break;
    case ImageGrabberDevice::gphoto2Source:
        grabberSourceCombo->setCurrentIndex(1);
        break;
    case ImageGrabberDevice::mediaFoundationSource:
        grabberSourceCombo->setCurrentIndex(0);
        break;
    }

    qDebug() << "ImportWidget::setImageGrabberSource --> End";
}


void ProjectWidget::changeRecordingMode(int index)
{
    qDebug() << "ProjectWidget::changeRecordingMode --> Start";

    switch(index) {
      case 0:
        // Singe picture
        timelapseGroupBox->hide();
        captureGroupBox->show();

        break;
      case 1:
        // Time-lapse picture
        timelapseGroupBox->show();
        captureGroupBox->hide();

        break;
    }

    qDebug() << "ProjectWidget::changeRecordingMode --> End";
}


void ProjectWidget::changeGrabberSource(int index)
{
    // this->grabberSourceCombo->setCurrentIndex(index);
}


void ProjectWidget::changeMixMode(int index)
{
    switch (index) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setScale(0.0, 5.0);
        mixCountSlider->setValue(defaultMixCount);
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setScale(0.0, 50.0);
        mixCountSlider->setValue(defaultPlaybackCount);
        break;
    default:
        Q_ASSERT(index < 3);

        break;
    }
}


void ProjectWidget::changeMixCount()
{
    // qDebug() << "ImportWidget::changeMixCount --> Empty";
}


void ProjectWidget::changeUnitMode(int index)
{
    qDebug() << "ImportWidget::changeUnitMode --> Start";

    switch (index) {
    case 0:
        // Seconds
        unitCountSliderCaption->setText(tr("Seconds between pictures"));
        unitCountSlider->setScale(0, 60);
        unitCountSlider->setTotalSteps(60);
        unitCountSlider->setValue(30);

        break;
    case 1:
        // Minutes
        unitCountSliderCaption->setText(tr("Minutes between pictures"));
        unitCountSlider->setScale(0, 60);
        unitCountSlider->setTotalSteps(60);
        unitCountSlider->setValue(5);

        break;
    case 2:
        // Hours
        unitCountSliderCaption->setText(tr("Hours between pictures"));
        unitCountSlider->setScale(0, 25);
        unitCountSlider->setTotalSteps(25);
        unitCountSlider->setValue(1);

        break;
    case 3:
        // Days
        unitCountSliderCaption->setText(tr("Days between pictures"));
        unitCountSlider->setScale(0, 30);
        unitCountSlider->setTotalSteps(30);
        unitCountSlider->setValue(1);

        break;
    }

    qDebug() << "ImportWidget::changeUnitMode --> End";
}


void ProjectWidget::changeUnitCount()
{
    qDebug() << "ImportWidget::changeUnitCount --> Start";

    int count = (int)unitCountSlider->value();

    if (0 == count) {
        count = 1;
        unitCountSlider->setValue(1.0);
    }

    unitCountSliderValue->setText(QString("%1").arg(count));

    qDebug() << "ImportWidget::changeUnitCount --> End";
}


void ProjectWidget::changeBeep(int newState)
{
    qDebug() << "ProjectWidget::changeBeep --> Start";

    bool check;

    if (Qt::Unchecked == newState) {
        check = false;
    }
    else {
        check = true;
    }

    beepCountSlider->setEnabled(check);
    // beepCheckBox->setEnabled(check);
    beepCountSliderValue->setEnabled(check);
    beepCountSliderCaption->setEnabled(check);

    qDebug() << "ProjectWidget::changeBeep --> End";
}


void ProjectWidget::changeBeepCount()
{
    qDebug() << "ImportWidget::changeBeepCount --> Start";

    int count = (int)beepCountSlider->value();

    if (0 == count) {
        count = 1;
        beepCountSlider->setValue(1.0);
    }

    beepCountSliderValue->setText(QString("%1").arg(count));

    qDebug() << "ImportWidget::changeBeepCount --> End";
}
