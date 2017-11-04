/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#include "recordingtab.h"

#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QMediaPlayer>
#include <QUrl>

#include "technical/grabber/imagegrabber.h"


RecordingTab::RecordingTab(Frontend *f,
                           ToolBar  *tb,
                           QWidget  *parent) :
    QWidget(parent)
{
    frontend               = f;
    toolBar                = tb;
    cameraOn               = false;

    cameraGroupBox         = 0;
    videoSourceLabel       = 0;
    videoSourceCombo       = 0;
    resolutionLabel        = 0;
    resolutionCombo        = 0;
    cameraButton           = 0;

    recordingGroupBox      = 0;
    recordingModeCombo     = 0;

    captureGroupBox        = 0;
    mixModeCombo           = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;

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
    timelapseTimer         = NULL;
    cameraTimer            = NULL;
    beepEffect             = NULL;
    clickEffect            = NULL;

    mixAccel               = 0;
    diffAccel              = 0;
    playbackAccel          = 0;

    this->setObjectName("RecordingTab");

    PreferencesTool *pref = frontend->getPreferences();
    if (pref->getIntegerPreference("preferences", "capturebutton", captureFunction) == false) {
        captureFunction = PreferencesTool::captureButtonAfter;
    }

    makeGUI();
    retranslateStrings();

    createAccelerators();
}


RecordingTab::~RecordingTab()
{
    if (timelapseTimer != NULL) {
        delete timelapseTimer;
        timelapseTimer = NULL;
    }
    if (cameraTimer != NULL) {
        delete cameraTimer;
        cameraTimer = NULL;
    }
    if (beepEffect != NULL) {
        delete beepEffect;
        beepEffect = NULL;
    }
    if (clickEffect != NULL) {
        delete clickEffect;
        clickEffect = NULL;
    }
}


void RecordingTab::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;
    QVector<QString> deviceNames = frontend->getDeviceNames();
    int deviceSize = deviceNames.size();

    // ========================================================================
    // Camera group box
    cameraGroupBox = new QGroupBox("cameraGroupBox");
    // cameraGroupBox->setFlat(true);

    videoSourceLabel = new QLabel("videoSourceLabel");
    videoSourceCombo = new QComboBox();
    videoSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoSourceCombo, SIGNAL(activated(int)), this, SLOT(changeVideoSource(int)));
    for (int deviceIndex = 0 ; deviceIndex < deviceSize ; deviceIndex++) {
        videoSourceCombo->addItem(deviceNames[deviceIndex].toLatin1());
    }

    resolutionLabel = new QLabel("resolutionLabel");
    resolutionCombo = new QComboBox();
    connect(resolutionCombo, SIGNAL(activated(int)), this, SLOT(changeResolution(int)));
    // resolutionLabel->hide();
    // resolutionCombo->hide();

    QVBoxLayout *cameraLayout = new QVBoxLayout;
    cameraLayout->setMargin(4);
    // cameraLayout->setSpacing(2);
    cameraLayout->addWidget(videoSourceLabel);
    cameraLayout->addWidget(videoSourceCombo);
    cameraLayout->addSpacing(10);
    cameraLayout->addWidget(resolutionLabel);
    cameraLayout->addWidget(resolutionCombo);
    // cameraLayout->addStretch(10);
    cameraGroupBox->setLayout(cameraLayout);

    // ========================================================================
    // Recording group box
    recordingGroupBox = new QGroupBox("recordingGroupBox");
    // recordingGroupBox->setFlat(true);

    recordingModeCombo = new QComboBox();
    recordingModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(recordingModeCombo, SIGNAL(activated(int)), this, SLOT(changeRecordingMode(int)));

    QVBoxLayout *recordingLayout = new QVBoxLayout;
    recordingLayout->setMargin(4);
    // recordingLayout->setSpacing(2);
    recordingLayout->addWidget(recordingModeCombo);
    recordingLayout->addStretch(10);
    recordingGroupBox->setLayout(recordingLayout);

    // ========================================================================
    // Capture group box
    captureGroupBox = new QGroupBox("captureGroupBox");
    // captureGroupBox->setFlat(true);

    mixModeCombo = new QComboBox();
    mixModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(mixModeCombo, SIGNAL(activated(int)), this, SLOT(changeMixMode(int)));

    mixCountSliderCaption = new QLabel();

    mixCountSlider = new QwtSlider();
    mixCountSlider->setOrientation(Qt::Horizontal);
    mixCountSlider->setScalePosition(QwtSlider::LeadingScale);
    mixCountSlider->setGroove(true);
    mixCountSlider->setScale(0.0, 5.0);
    mixCountSlider->setTotalSteps(5);
    mixCountSlider->setSingleSteps(1);
    connect(mixCountSlider, SIGNAL(sliderReleased()), this, SLOT(changeMixCount()));

    QVBoxLayout *captureLayout = new QVBoxLayout;
    captureLayout->setMargin(4);
    // captureLayout->setSpacing(2);
    captureLayout->addWidget(mixModeCombo);
    captureLayout->addSpacing(10);
    captureLayout->addWidget(mixCountSliderCaption);
    captureLayout->addWidget(mixCountSlider);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);
    // captureGroupBox->hide();

    // ========================================================================
    // Time-lapse group box
    timelapseGroupBox = new QGroupBox("timelapseGroupBox");
    // timelapseGroupBox->setFlat(true);

    unitModeComboCaption = new QLabel();

    unitModeCombo = new QComboBox();
    unitModeCombo->setFocusPolicy(Qt::NoFocus);
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

    // "Beep n seconds before picture:"
    beepCheckBox = new QCheckBox("beepCheckBox");
    beepCheckBox->setChecked(false);
    connect(beepCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeBeep(int)));

    beepCountSliderValue = new QLabel("0");
    beepCountSliderCaption = new QLabel("beepCountSliderCaption");

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
    timelapseGroupBox->hide();

    // ========================================================================
    // Camera button layout
    cameraButton = new QPushButton;
    iconFile.append(QLatin1String("cameraon.png"));
    QIcon ic(iconFile);
    cameraButton->setIcon(ic);
    cameraButton->setIconSize(ic.availableSizes().first());
    // cameraButton->setFlat(true);
    cameraButton->setFocusPolicy(Qt::NoFocus);
    cameraButton->setMinimumHeight(80);
    connect(cameraButton, SIGNAL(clicked()), this, SLOT(cameraButtonClicked()));
    // cameraButton->setEnabled(false);

    // ========================================================================
    // Tab layout
    tabLayout->setMargin(0);
    // tabLayout->setSpacing(2);
    tabLayout->addWidget(cameraGroupBox);
    tabLayout->addSpacing(10);
    tabLayout->addWidget(recordingGroupBox);
    tabLayout->addSpacing(10);
    tabLayout->addWidget(timelapseGroupBox);
    tabLayout->addSpacing(10);
    tabLayout->addWidget(captureGroupBox);
    tabLayout->addSpacing(10);
    tabLayout->addWidget(cameraButton);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    connect(toolBar->getCaptureButton(), SIGNAL(clicked()), this, SLOT(captureFrame()));
}


void RecordingTab::retranslateStrings()
{
    cameraGroupBox->setTitle(tr("Camera"));
    videoSourceLabel->setText(tr("Video Source:"));
    resolutionLabel->setText(tr("Resolution:"));

    recordingGroupBox->setTitle(tr("Recording mode"));
    recordingModeCombo->clear();
    recordingModeCombo->addItem(tr("Single frame capture"));
    recordingModeCombo->addItem(tr("Time-lapse capture"));

    captureGroupBox->setTitle(tr("Capture"));
    mixModeCombo->clear();
    mixModeCombo->addItem(tr("Mix"));
    mixModeCombo->addItem(tr("Diff"));
    mixModeCombo->addItem(tr("Playback"));

    QString infoText =
        tr("<h4>Toggle camera on/off (C)</h4> "
           "<p>Click this button to toggle the camera on and off</p> ");
    cameraButton->setToolTip(infoText);
    cameraButton->setWhatsThis(infoText);

    mixCountSliderCaption->setText(tr("Number of images:"));
    infoText =
        tr("<h4>Number of images</h4> "
           "<p>By changing the value in this slidebar you can specify how many images "
           "backwards in the animation which should be mixed on top of the camera or "
           "if you are in playback mode: how many images to play. </p> "
           "<p>By mixing the previous image(s) onto the camera you can more easily see "
           "how the next shot will be in relation to the other, therby making a smoother "
           "stop motion animation!</p>");
    mixCountSliderCaption->setWhatsThis(infoText);
    mixCountSlider->setWhatsThis(infoText);

    timelapseGroupBox->setTitle(tr("Time-lapse"));

    unitModeComboCaption->setText(tr("Unit:"));

    unitModeCombo->clear();
    unitModeCombo->addItem(tr("Seconds"));
    unitModeCombo->addItem(tr("Minutes"));
    unitModeCombo->addItem(tr("Hours"));
    unitModeCombo->addItem(tr("Days"));
    unitModeCombo->setCurrentIndex(getUnitMode());

    changeUnitMode(getUnitMode());

    // beepCountSliderCaption->setText(tr("Beep ?? seconds before picture:"));
    beepCheckBox->setText(tr("Beep"));
    beepCountSliderCaption->setText(tr("seconds before picture:"));
}


void RecordingTab::initialize()
{
    qDebug() << "RecordingTab::initialize --> Start";

    enableCaptureGroupBox(true);
    enableTimelapseGroupBox(true);

    /*
    int videoSource = frontend->getProject()->getAnimationProject()->getVideoSource();
    this->videoSourceCombo->setCurrentIndex(videoSource);

    for (index = 0; index < grabberController->getResolutions().size(); index++) {
        resolution = grabberController->getResolutions().at(index);
        resolutionComboBox->addItem(QString("%1%2%3")
                                    .arg(resolution.getWidth())
                                    .arg(QLatin1String(" x "))
                                    .arg(resolution.getHeight()));
    }

    if (preferences->getIntegerPreference(deviceId, "resolutionwidth", width) == true) {
        if (preferences->getIntegerPreference(deviceId, "resolutionheight", height) == false) {
            // Internal problem
            qDebug() << "CameraControllerDialog::initialize --> Resolution height not found!";
        }
        // Search resolution in the list of possible resolutions
        for (index = 0; index < grabberController->getResolutions().size(); index++) {
            resolution = grabberController->getResolutions().at(index);
            if ((resolution.getWidth() == width) &&
                    (resolution.getHeight() == height)) {
                resolutionIndex = index;
                break;
            }
        }
    }
    if (-1 == resolutionIndex) {
        // No predifined resolution - Use the maximum possible resolution
        for (index = 0; index < grabberController->getResolutions().size(); index++) {
            resolution = grabberController->getResolutions().at(index);
            if (resolution.getWidth() < (unsigned int)width) {
                continue;
            }
            if (resolution.getWidth() > (unsigned int)width) {
                width = resolution.getWidth();
                height = resolution.getHeight();
                resolutionIndex = index;
                continue;
            }
            if (resolution.getHeight() > (unsigned int)height) {
                height = resolution.getHeight();
                resolutionIndex = index;
            }
        }
    }
    grabberController->setActiveResolution(resolutionIndex);
    resolutionComboBox->setCurrentIndex(resolutionIndex);

    int mixingMode = frontend->getProject()->getAnimationProject()->getMixingMode();
    changeMixingMode(mixingMode);

    int unitMode = frontend->getProject()->getAnimationProject()->getUnitMode();
    this->unitModeChooseCombo->setCurrentIndex(unitMode);
    */
    qDebug() << "RecordingTab::initialize --> End";
}


bool RecordingTab::isCameraOn()
{
    return cameraOn;
}


void RecordingTab::checkCameraOff()
{
    if (cameraOn) {
        // The camera is on --> switch off.
        cameraButtonClicked();
    }
}


int RecordingTab::getVideoSource()
{
    return videoSourceCombo->currentIndex();
}


bool RecordingTab::setVideoSource(int index)
{
    Q_ASSERT(index >= 0);

    if (index >= videoSourceCombo->count()) {
        return false;
    }
    videoSourceCombo->setCurrentIndex(index);

    resolutionCombo->clear();
    QVector<QString> resNames = frontend->getResolutionNames(index);
    int resSize = resNames.size();
    if (0 < resSize) {
        for (int resIndex = 0 ; resIndex < resSize ; resIndex++) {
            resolutionCombo->addItem(resNames[resIndex]);
        }
    }
    else {
        resolutionCombo->addItem(tr("Not Supported"));
    }
    resolutionCombo->setCurrentIndex(0);

    return true;
}


int RecordingTab::getResolution()
{
    return resolutionCombo->currentIndex();
}


bool RecordingTab::setResolution(int index)
{
    if (index < 0) {
        return false;
    }

    if (index >= resolutionCombo->count()) {
        return false;
    }
    resolutionCombo->setCurrentIndex(index);

    return true;
}


int RecordingTab::getRecordingMode()
{
    return recordingModeCombo->currentIndex();
}


void RecordingTab::setRecordingMode(int mode)
{
    Q_ASSERT(mode >= 0);
    Q_ASSERT(mode < 2);

    recordingModeCombo->setCurrentIndex(mode);
    changeRecordingMode(mode);
}


int RecordingTab::getMixMode()
{
    return mixModeCombo->currentIndex();
}


void RecordingTab::setMixMode(int mode)
{
    Q_ASSERT(mode >= 0);
    Q_ASSERT(mode < 3);

    mixModeCombo->setCurrentIndex(mode);
}


int RecordingTab::getMixCount()
{
    if (mixModeCombo->currentIndex() == 0) {
        return (int)(mixCountSlider->value());
    }
    else {
        return frontend->getProject()->getMixCount();
    }
}


void RecordingTab::setMixCount(int count)
{
    Q_ASSERT(mixModeCombo->currentIndex() == 0);

    mixCountSlider->setValue((double)count);
}


int RecordingTab::getPlaybackCount()
{
    if (mixModeCombo->currentIndex() == 2) {
        return (int)(mixCountSlider->value());
    }
    else {
        return frontend->getProject()->getPlaybackCount();
    }
}


void RecordingTab::setPlaybackCount(int count)
{
    Q_ASSERT(mixModeCombo->currentIndex() == 2);

    mixCountSlider->setValue((double)count);
}


void RecordingTab::setMixModeMixing()
{
    changeMixMode(0);
}


void RecordingTab::setMixModeDiffing()
{
    changeMixMode(1);
}


void RecordingTab::setMixModePlayback()
{
    changeMixMode(2);
}


int RecordingTab::getUnitMode()
{
    return unitModeCombo->currentIndex();
}


void RecordingTab::setUnitMode(int mode)
{
    Q_ASSERT(mode >= 0);
    Q_ASSERT(mode < 4);

    unitModeCombo->setCurrentIndex(mode);
    changeUnitMode(mode);
}


int RecordingTab::getUnitCount()
{
    if (unitModeCombo->currentIndex() == 0) {
        return (int)(unitCountSlider->value());
    }
    else {
        return frontend->getProject()->getUnitCount();
    }
}


void RecordingTab::setUnitCount(int count)
{
    unitCountSlider->setValue((double)count);
}


bool RecordingTab::getBeepState()
{
    bool checked;

    checked = beepCheckBox->isChecked();

    return checked;
}


void RecordingTab::setBeepState(bool checked)
{
    beepCheckBox->setChecked(checked);
    if (checked) {
        changeBeep(Qt::Checked);
    }
    else {
        changeBeep(Qt::Unchecked);
    }
}


int RecordingTab::getBeepCount()
{
    if (unitModeCombo->currentIndex() == 0) {
        return (int)(beepCountSlider->value());
    }
    else {
        return frontend->getProject()->getBeepCount();
    }
}


void RecordingTab::setBeepCount(int count)
{
    beepCountSlider->setValue((double)count);
}


/*
void RecordingTab::resizeEvent(QResizeEvent *event)
{
    qDebug() << "RecordingTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug() << "RecordingTab::resizeEvent --> End");
}
*/


void RecordingTab::changeVideoSource(int index)
{
    qDebug() << "RecordingTab::changeSource --> Start";

    frontend->getProject()->getAnimationProject()->setVideoSource(index);
    frontend->setVideoSource(index);

    qDebug() << "RecordingTab::changeSource --> End";
}


void RecordingTab::changeResolution(int index)
{
    qDebug() << "RecordingTab::changeResolution --> Start";

    frontend->getProject()->getAnimationProject()->setResolution(index);
    frontend->setResolution(index);

    qDebug() << "RecordingTab::changeResolution --> End";
}


void RecordingTab::changeRecordingMode(int index)
{
    qDebug() << "RecordingTab::changeRecordingMode --> Start";

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

    frontend->getProject()->getAnimationProject()->setRecordingMode(index);

    qDebug() << "RecordingTab::changeRecordingMode --> End";
}


void RecordingTab::changeMixMode(int newMixMode)
{
    qDebug() << "RecordingTab::changeMixMode --> Start";

    frontend->getProject()->getAnimationProject()->setMixMode(newMixMode);

    frontend->getProject()->getView()->notifyNewMixMode(newMixMode);

    switch (newMixMode) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setScale(0.0, 5.0);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getMixCount());
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        mixCountSlider->setValue(1);
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setScale(0.0, 50.0);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getPlaybackCount());
        break;
    default:
        Q_ASSERT(newMixMode < 3);
        break;
    }

    qDebug() << "RecordingTab::changeMixMode --> End";
}


void RecordingTab::changeMixCount()
{
    int newMixCount = (int)mixCountSlider->value();;
    int mixMode = mixModeCombo->currentIndex();

    switch (mixMode) {
    case 0:
        if (frontend->getProject()->getAnimationProject()->getMixCount() != newMixCount) {
            frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        }
        break;
    case 1:
        break;
    case 2:
        if (frontend->getProject()->getAnimationProject()->getPlaybackCount() != newMixCount) {
            frontend->getProject()->getAnimationProject()->setPlaybackCount(newMixCount);
        }
        break;
    case 3:
        break;
    }

    frontend->getProject()->getView()->notifyNewMixCount(newMixCount);
}


void RecordingTab::changeUnitMode(int index)
{
    qDebug() << "RecordingTab::changeUnitMode --> Start";

    // frontend->getProject()->getAnimationProject()->setUnitMode(index);

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

    if (frontend->getProject()->isActiveProject()) {
        frontend->getProject()->getAnimationProject()->setUnitMode(index);
    }

    qDebug() << "RecordingTab::changeUnitMode --> End";
}


void RecordingTab::changeUnitCount()
{
    qDebug() << "RecordingTab::changeUnitCount --> Start";

    int newUnitCount = (int)unitCountSlider->value();
    int factor = 0;
    int unitMode = unitModeCombo->currentIndex();

    if (0 == newUnitCount) {
        newUnitCount = 1;
    }
    /*
    switch (unitMode) {
    case 0:
        if (60 < newUnitCount) {
            newUnitCount = 60;
        }
        break;
    case 1:
        if (60 < newUnitCount) {
            newUnitCount = 60;
        }
        break;
    case 2:
        if (24 < newUnitCount) {
            newUnitCount = 24;
        }
        break;
    case 3:
        if (30 < newUnitCount) {
            newUnitCount = 30;
        }
        break;
    }
    */
    if (frontend->getProject()->isActiveProject()) {
        frontend->getProject()->getAnimationProject()->setUnitCount(newUnitCount);
    }
    unitCountSliderValue->setText(QString("%1").arg(newUnitCount));
    /*
    if (newUnitCount == 0 || unitMode == 0) {
        if (timelapseTimer->isActive()) {
            timelapseTimer->stop();
        }
        return;
    }

    if (timelapseTimer->isActive() == false) {
        // Grab the first frame manually
        cameraHandler->captureFrame();
        // then grab at the given interval
        timelapseTimer->start(factor / newUnitCount);
    }
    else {
        timelapseTimer->setInterval(factor / newUnitCount);
    }
    */

    qDebug() << "RecordingTab::changeUnitCount --> End";
}


void RecordingTab::changeBeep(int newState)
{
    qDebug() << "RecordingTab::changeBeep --> Start";

    bool checked;

    if (Qt::Unchecked == newState) {
        checked = false;
    }
    else {
        checked = true;
    }

    beepCountSlider->setEnabled(checked);
    // beepCheckBox->setEnabled(checked);
    beepCountSliderValue->setEnabled(checked);
    beepCountSliderCaption->setEnabled(checked);

    frontend->getProject()->getAnimationProject()->setBeepState(checked);

    qDebug() << "RecordingTab::changeBeep --> End";
}


void RecordingTab::changeBeepCount()
{
    qDebug() << "RecordingTab::changeBeepCount --> Start";

    int newBeepCount = (int)beepCountSlider->value();

    if (0 == newBeepCount) {
        newBeepCount = 1;
    }

    frontend->getProject()->getAnimationProject()->setBeepCount(newBeepCount);
    beepCountSliderValue->setText(QString("%1").arg(newBeepCount));

    qDebug() << "RecordingTab::changeBeepCount --> End";
}


void RecordingTab::changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction)
{
    // The function of the camera button is changed
    captureFunction = newFunction;
}


void RecordingTab::cameraButtonClicked()
{
    qDebug() << "RecordingTab::cameraButtonClicked --> Start";

    if (cameraOn == false) {
        // Camera is off

        if (!frontend->getProject()->isActiveProject()) {
            // No active project
            frontend->showInformation(tr("Information"), tr("No active project. Please create a new project or open an existing project."));
            return;
        }
        int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
        if (-1 == activeSceneIndex) {
            // No scene selected
            frontend->showInformation(tr("Information"), tr("No scene and no take selected. Please select a scene and a take on the project tab."));
            return;
        }
        int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
        if (-1 == activeTakeIndex) {
            // No take selected
            frontend->showInformation(tr("Information"), tr("No take selected. Please select a take on the project tab."));
            return;
        }

        qDebug() << "RecordingTab::cameraButtonClicked --> Start playing video from webcam";

        cameraOn = frontend->startGrabber();

        if (cameraOn) {
            videoSourceCombo->setEnabled(false);
            resolutionCombo->setEnabled(false);

            QString iconFile(frontend->getIconsDirName());
            iconFile.append(QLatin1String("cameraoff.png"));
            cameraButton->setIcon(QIcon(iconFile));

            switch (captureFunction) {
            // case PreferencesTool::captureButtonBevor:
            //     break;
            // case PreferencesTool::captureButtonAfter:
            //     break;
            case PreferencesTool::captureButtonAppend:
                int newExposureIndex = frontend->getProject()->getActiveTake()->getExposureSize() - 1;

                if (newExposureIndex != frontend->getProject()->getActiveExposureIndex()) {
                    frontend->getProject()->selectExposureToUndo(frontend->getProject()->getActiveSceneIndex(),
                                                                 frontend->getProject()->getActiveTakeIndex(),
                                                                 newExposureIndex);
                }
                break;
            }
        }
    } else {
        // Camera is on
        qDebug() << "RecordingTab::cameraButtonClicked --> Stop playing video from webcam";

        QString iconFile(frontend->getIconsDirName());
        iconFile.append(QLatin1String("cameraon.png"));

        cameraButton->setIcon(QIcon(iconFile));

        frontend->stopGrabber();

        cameraOn = false;

        videoSourceCombo->setEnabled(true);
        resolutionCombo->setEnabled(true);
    }

    if (cameraOn) {
        initialize();
        // captureGroupBox->show();
        enableCaptureGroupBox(false);
        enableTimelapseGroupBox(false);

        if (frontend->getSignal()) {
            QString clickSoundFile(frontend->getSoundsDirName());
            clickSoundFile.append(QLatin1String("click.wav"));

            clickEffect = new QSoundEffect();
            clickEffect->setSource(QUrl::fromLocalFile(clickSoundFile));
            clickEffect->setVolume(0.25f);
        }

        if (0 == getRecordingMode()) {
            // Capture mode

            toolBar->setActualState(ToolBar::toolBarCameraOn);
        }
        else {
            // Time lapse mode

            int msec;
            int factor;

            if (beepCheckBox->isChecked()) {
                // Create timer for the time between beep and frame capture
                cameraTimer = new QTimer(this);
                cameraTimer->setSingleShot(true);
                QObject::connect(cameraTimer, SIGNAL(timeout()), this, SLOT(storeFrame()));

                QString beepSoundFile(frontend->getSoundsDirName());
                beepSoundFile.append(QLatin1String("beep.wav"));

                beepEffect = new QSoundEffect();
                beepEffect->setSource(QUrl::fromLocalFile(beepSoundFile));
                beepEffect->setVolume(0.25f);
            }

            // Create timer for the time between the beeps
            timelapseTimer = new QTimer(this);
            timelapseTimer->setSingleShot(false);
            QObject::connect(timelapseTimer, SIGNAL(timeout()), this, SLOT(sendBeep()));

            // Calculate the time between the beeps
            switch (unitModeCombo->currentIndex()) {
            case 0:
                // Seconds
                factor = 1000;

                break;
            case 1:
                // Minutes
                factor = 60000;

                break;
            case 2:
                // Hours
                factor = 3600000;

                break;
            case 3:
                // Days
                factor = 86400000;

                break;
            default:
                if (timelapseTimer->isActive()) {
                    timelapseTimer->stop();
                }
                break;
            }

            msec = unitCountSlider->value() * factor;

            // Start the time lapse time
            timelapseTimer->start(msec);
        }
    }
    else {
        // captureGroupBox->hide();
        enableCaptureGroupBox(true);
        enableTimelapseGroupBox(true);
        if (0 == getRecordingMode()) {
            // Capture mode

            toolBar->setActualState(ToolBar::toolBarCameraOff);
        }
        else {
            // Time lapse mode

            timelapseTimer->stop();
            delete timelapseTimer;
            timelapseTimer = NULL;

            delete clickEffect;
            clickEffect = NULL;

            if (beepCheckBox->isChecked()) {
                cameraTimer->stop();
                delete cameraTimer;
                cameraTimer = NULL;

                delete beepEffect;
                beepEffect = NULL;
            }
        }
    }

    qDebug() << "RecordingTab::cameraButtonClicked --> End";
}


/*
void RecordingTab::changeFpuCount(int newFpuCount)
{
    if (timelapseTimer->isActive() && newMixCount != 0) {
        int factor = 0;
        int unitMode = unitModeChooseCombo->currentIndex();
        switch (unitMode) {
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
        timelapseTimer->setInterval(factor / newMixCount);
    }

    int viewingMode = viewingModeChooseCombo->currentIndex();
    switch (viewingMode) {
    case 0:
        frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        break;
    case 1:
        break;
    case 2:
        frontend->getProject()->getAnimationProject()->setPlaybackCount(newMixCount);
        break;
    case 3:
        break;
    }

    frontend->getProject()->getView()->notifyNewMixCount(newMixCount);
}
*/

void RecordingTab::createAccelerators()
{
    /*
    mixAccel = new QShortcut(QKeySequence(Qt::Key_1), this);
    connect(mixAccel, SIGNAL(activated()), this, SLOT(setMixModeMixing()));

    diffAccel = new QShortcut(QKeySequence(Qt::Key_2), this);
    connect(diffAccel, SIGNAL(activated()), this, SLOT(setMixModeDiffing()));

    playbackAccel = new QShortcut(QKeySequence(Qt::Key_3), this);
    connect(playbackAccel, SIGNAL(activated()), this, SLOT(setMixModePlayback()));
    */
}


void RecordingTab::sendBeep()
{
    qDebug() << "RecordingTab::sendBeep --> Start";

    if (beepCheckBox->isChecked()) {
        beepEffect->play();

        // Start the timer for the frame capture
        cameraTimer->start(beepCountSlider->value() * 1000);
    }
    else
    {
        // Save the frame immediately
        storeFrame();
    }


    qDebug() << "RecordingTab::sendBeep --> End";
}


void RecordingTab::captureFrame()
{
    qDebug() << "RecordingTab::captureFrame --> Start";

    if (0 == getRecordingMode()) {
        // Capture mode

        toolBar->setActualState(ToolBar::toolBarNothing);
    }

    storeFrame();

    if (0 == getRecordingMode()) {
        // Capture mode

        toolBar->setActualState(ToolBar::toolBarCameraOn);
    }

    qDebug() << "RecordingTab::captureFrame --> End";
}


void RecordingTab::storeFrame()
{
    qDebug() << "RecordingTab::storeFrame --> Start";

    QImage newImage = clipAndScale(frontend->getRawImage());

    if (!newImage.isNull()) {
        int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
        int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
        int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
        int exposureSize = frontend->getProject()->getScene(activeSceneIndex)->getTake(activeTakeIndex)->getExposureSize();
        int newExposureIndex = activeExposureIndex;

        switch (captureFunction) {
        case PreferencesTool::captureButtonBevor:
            if (-1 == newExposureIndex) {
                frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
                newExposureIndex++;
            }
            else {
                frontend->getProject()->insertExposureToUndo(newImage, activeSceneIndex, activeTakeIndex, activeExposureIndex);
            }
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, newExposureIndex);
            break;
        case PreferencesTool::captureButtonAfter:
            newExposureIndex++;
            if (newExposureIndex == exposureSize) {
                // actual exposure index is the last in the take
                frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
            }
            else {
                // actual exposure index is not the last in the take
                frontend->getProject()->insertExposureToUndo(newImage, activeSceneIndex, activeTakeIndex, newExposureIndex);
            }
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, newExposureIndex);
            break;
        case PreferencesTool::captureButtonAppend:
            frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, exposureSize);
            break;
        }

        if (frontend->getSignal()) {
            clickEffect->play();
        }
    }

    qDebug() << "RecordingTab::storeFrame --> End";
}


QImage RecordingTab::clipAndScale(QImage image)
{
    QImage outputImage;
    QImage clipImage;

    double destWidth = 0;
    double destHeight = 0;
    double imageWidth = image.width();
    double imageHeight = image.height();
    double newImageWidth = 0;
    double newImageHeight = 0;
    int    x = 0;
    int    y = 0;
    double widthScale;
    double heightScale;
    int    trans = frontend->getProject()->getImageTransformation();

    switch (frontend->getProject()->getVideoSize()) {
    case ImageGrabber::qvgaSize:    // QVGA
        destWidth = 320;
        destHeight = 240;
        break;
    case ImageGrabber::vgaSize:     // VGA
        destWidth = 640;
        destHeight = 480;
        break;
    case ImageGrabber::svgaSize:    // SVGA
        destWidth = 800;
        destHeight = 600;
        break;
    case ImageGrabber::paldSize:    // PAL D
        destWidth = 704;
        destHeight = 576;
        break;
    case ImageGrabber::hdreadySize: // HD Ready
        destWidth = 1280;
        destHeight = 720;
        break;
    case ImageGrabber::fullhdSize:  // Full HD
        destWidth = 1900;
        destHeight = 1080;
        break;
    default: // Camera image size
        destWidth = imageWidth;
        destHeight = imageHeight;
        break;
    }

    switch (trans) {
    case 0:
        // Scale the image to the output size

        widthScale = imageWidth / destWidth;
        heightScale = imageHeight / destHeight;

        if (widthScale > heightScale) {
            outputImage = image.scaledToWidth(destWidth);
        }
        else {
            outputImage = image.scaledToHeight(destHeight);
        }

        break;

    case 1:
        // Clip the image to the output size
        switch (frontend->getProject()->getImageAdjustment()) {
        case ImageGrabber::leftUp:
        case ImageGrabber::leftMiddle:
        case ImageGrabber::leftDown:
            x = 0;
            break;
        case ImageGrabber::centerUp:
        case ImageGrabber::centerMiddle:
        case ImageGrabber::centerDown:
            x = (int)((imageWidth-destWidth)/2);
            break;
        case ImageGrabber::rightUp:
        case ImageGrabber::rightMiddle:
        case ImageGrabber::rightDown:
            x = (int)(imageWidth-destWidth);
            break;
        }

        switch (frontend->getProject()->getImageAdjustment()) {
        case ImageGrabber::leftUp:
        case ImageGrabber::centerUp:
        case ImageGrabber::rightUp:
            y = 0;
            break;
        case ImageGrabber::leftMiddle:
        case ImageGrabber::centerMiddle:
        case ImageGrabber::rightMiddle:
            y = (int)((imageHeight-destHeight)/2);
            break;
        case ImageGrabber::leftDown:
        case ImageGrabber::centerDown:
        case ImageGrabber::rightDown:
            y = (int)(imageHeight-destHeight);
            break;
        }

        outputImage = image.copy(x, y, destWidth, destHeight);

        break;

    case 2:
        // Zoom the image

        int zoomValue = frontend->getProject()->getZoomValue();

        // 1. Step: Clip the image to the output size

        newImageWidth = imageWidth - ((imageWidth - destWidth) * zoomValue / 100);
        newImageHeight = imageHeight - ((imageHeight - destHeight) * zoomValue / 100);

        x = (int)((imageWidth-newImageWidth)/2);
        y = (int)((imageHeight-newImageHeight)/2);

        clipImage = image.copy(x, y, newImageWidth, newImageHeight);

        // 2. Step: Scale up the image

        widthScale = newImageWidth / destWidth;
        heightScale = newImageHeight / destHeight;

        if (widthScale > heightScale) {
            outputImage = clipImage.scaledToWidth(destWidth);
        }
        else {
            outputImage = clipImage.scaledToHeight(destHeight);
        }

        break;

    }

    return outputImage;
}


void RecordingTab::enableCaptureGroupBox(bool newState)
{
    qDebug() << "RecordingTab::enableCaptureGroupBox --> Start";

    mixModeCombo->setEnabled(newState);
    mixCountSliderCaption->setEnabled(newState);
    mixCountSlider->setEnabled(newState);

    qDebug() << "RecordingTab::enableCaptureGroupBox --> End";
}


void RecordingTab::enableTimelapseGroupBox(bool newState)
{
    qDebug() << "RecordingTab::enableTimelapseGroupBox --> Start";

    unitModeComboCaption->setEnabled(newState);
    unitModeCombo->setEnabled(newState);
    unitCountSliderValue->setEnabled(newState);
    unitCountSliderCaption->setEnabled(newState);
    beepCheckBox->setEnabled(newState);
    if (beepCheckBox->isChecked()) {
        beepCountSliderValue->setEnabled(newState);
        beepCountSliderCaption->setEnabled(newState);
        beepCountSlider->setEnabled(newState);
    }
    else {
        beepCountSliderValue->setEnabled(false);
        beepCountSliderCaption->setEnabled(false);
        beepCountSlider->setEnabled(false);
    }

    qDebug() << "RecordingTab::enableTimelapseGroupBox --> End";
}
