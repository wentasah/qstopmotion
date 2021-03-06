/******************************************************************************
 *  Copyright (C) 2010-2018 by                                                *
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

#include "domain/domainfacade.h"


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
    cameraButton->setIconSize(QSize(90, 60));
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
    recordingModeCombo->addItem(tr("Single frame capture"), DomainFacade::singleFrameMode);
    recordingModeCombo->addItem(tr("Time-lapse capture"), DomainFacade::timeLapseMode);

    captureGroupBox->setTitle(tr("Capture"));
    mixModeCombo->clear();
    mixModeCombo->addItem(tr("Mix"));
    mixModeCombo->addItem(tr("Diff"));

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


enum DomainFacade::recordingMode RecordingTab::getRecordingMode()
{
    const QVariant userData = recordingModeCombo->currentData();
    if (!userData.isValid()) {
        qWarning() << "recordingModeCombo contains invalid data for index"
                   << recordingModeCombo->currentIndex()
                   << "use singleFrameMode";
        return DomainFacade::singleFrameMode;
    }

    bool ok = false;
    const int mode = userData.toInt(&ok);
    if (!ok || (mode < DomainFacade::singleFrameMode)
            || (mode > DomainFacade::timeLapseMode)) {
        qWarning() << "recordingModeCombo contains invalid data for index"
                   << recordingModeCombo->currentIndex()
                   << "current data:" << userData
                   << "use singleFrameMode";
        return DomainFacade::singleFrameMode;
    }

    const enum DomainFacade::recordingMode recMode
            = static_cast<enum DomainFacade::recordingMode>(mode);
    return recMode;
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
    Q_ASSERT(mode >= DomainFacade::mixImageMode);
    Q_ASSERT(mode < DomainFacade::lastImageMixMode);

    mixModeCombo->setCurrentIndex(mode);
    changeMixMode(mode);
}


int RecordingTab::getMixCount()
{
    if (mixModeCombo->currentIndex() == 0) {
        // Mix mode
        return (int)(mixCountSlider->value());
    }
    else {
        // Diff mode
        return frontend->getProject()->getMixCount();
    }
}


void RecordingTab::setMixCount(int count)
{
    if (DomainFacade::diffImageMode != getMixMode()) {
        // Diff mode
        mixCountSlider->setValue((double)count);
    }
}


void RecordingTab::setMixModeMixing()
{
    changeMixMode(0);
}


void RecordingTab::setMixModeDiffing()
{
    changeMixMode(1);
}


int RecordingTab::getUnitMode()
{
    return unitModeCombo->currentIndex();
}


void RecordingTab::setUnitMode(int mode)
{
    if ((mode < DomainFacade::secondsMode) || (mode >= DomainFacade::lastUnitMode)) {
        qCritical() << "try to set invalid timelapse unit mode" << mode;
        qCritical() << "use units in seconds";
        mode = DomainFacade::secondsMode;
    }
    qDebug() << "set timelapse unit mode to" << mode;
    unitModeCombo->setCurrentIndex(mode);
    changeUnitMode(mode);
}


int RecordingTab::getUnitCount()
{
    if (unitModeCombo->currentIndex() == DomainFacade::secondsMode) {
        return (int)(unitCountSlider->value());
    }
    else {
        return frontend->getProject()->getUnitCount();
    }
}

quint64 RecordingTab::getTimelapseIntervalMsec() const
{
    quint64 factor = 1000;

    switch (unitModeCombo->currentIndex()) {
    case DomainFacade::secondsMode:
        factor = 1000;
        break;
    case DomainFacade::minutesMode:
        factor = 60000;
        break;
    case DomainFacade::hoursMode:
        factor = 3600000;
        break;
    case DomainFacade::daysMode:
        factor = 86400000;
        break;
    default:
        qCritical() << "couldn't select timelapse unit mode. combobox index:" << unitModeCombo->currentIndex()
                    << "use factor 1000";
        break;
    }

    const quint64 msec = static_cast<quint64>(unitCountSlider->value()) * factor;
    return msec;
}


void RecordingTab::setUnitCount(int count)
{
    qDebug() << "set timelapse unit count to" << count;
    unitCountSlider->setValue(static_cast<double>(count));
    unitCountSliderValue->setText(QString::number(count));

    boundBeepTimeSlider();
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
    if (unitModeCombo->currentIndex() == DomainFacade::secondsMode) {
        return (int)(beepCountSlider->value());
    }
    else {
        return frontend->getProject()->getBeepCount();
    }
}


void RecordingTab::setBeepCount(int count)
{
    qDebug() << "set timelapse beep timer interval to" << count << "second(s)";
    boundBeepTimeSlider(count);
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
      case DomainFacade::singleFrameMode:
        // Singe picture
        timelapseGroupBox->hide();
        captureGroupBox->show();

        break;
      case DomainFacade::timeLapseMode:
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
    case DomainFacade::mixImageMode:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setScale(0.0, 5.0);
        mixCountSlider->setValue((double)frontend->getProject()->getAnimationProject()->getMixCount());
        break;
    case DomainFacade::diffImageMode:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        mixCountSlider->setScale(0.0, 5.0);
        mixCountSlider->setValue((double)1);
        break;
    default:
        Q_ASSERT(newMixMode < 2);
        break;
    }

    qDebug() << "RecordingTab::changeMixMode --> End";
}


void RecordingTab::changeMixCount()
{
    int newMixCount = (int)mixCountSlider->value();;
    int mixMode = mixModeCombo->currentIndex();

    switch (mixMode) {
    case DomainFacade::mixImageMode:
        if (frontend->getProject()->getAnimationProject()->getMixCount() != newMixCount) {
            frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        }
        break;
    case DomainFacade::diffImageMode:
        break;
    }

    frontend->getProject()->getView()->notifyNewMixCount(newMixCount);
}


void RecordingTab::changeUnitMode(int index)
{
    qDebug() << "RecordingTab::changeUnitMode --> Start";

    // frontend->getProject()->getAnimationProject()->setUnitMode(index);
    qDebug() << "change timelapse timer unit mode to" << index;

    switch (index) {
    case DomainFacade::secondsMode:
        // Seconds
        unitCountSliderCaption->setText(tr("Seconds between pictures"));
        unitCountSlider->setScale(1, 60);
        unitCountSlider->setTotalSteps(60);
        unitCountSlider->setValue(30);

        break;
    case DomainFacade::minutesMode:
        // Minutes
        unitCountSliderCaption->setText(tr("Minutes between pictures"));
        unitCountSlider->setScale(1, 60);
        unitCountSlider->setTotalSteps(60);
        unitCountSlider->setValue(5);

        break;
    case DomainFacade::hoursMode:
        // Hours
        unitCountSliderCaption->setText(tr("Hours between pictures"));
        unitCountSlider->setScale(1, 25);
        unitCountSlider->setTotalSteps(25);
        unitCountSlider->setValue(1);

        break;
    case DomainFacade::daysMode:
        // Days
        unitCountSliderCaption->setText(tr("Days between pictures"));
        unitCountSlider->setScale(1, 30);
        unitCountSlider->setTotalSteps(30);
        unitCountSlider->setValue(1);

        break;
    }

    const int unitCount = static_cast<int>(unitCountSlider->value());
    unitCountSliderValue->setText(QString::number(unitCount));
    if (frontend->getProject()->isActiveProject()) {
        frontend->getProject()->getAnimationProject()->setUnitMode(index);
    }
    boundBeepTimeSlider();

    qDebug() << "RecordingTab::changeUnitMode --> End";
}


void RecordingTab::changeUnitCount()
{
    qDebug() << "RecordingTab::changeUnitCount --> Start";

    int newUnitCount = static_cast<int>(unitCountSlider->value());

    if (0 == newUnitCount) {
        newUnitCount = 1;
    }

    if (frontend->getProject()->isActiveProject()) {
        frontend->getProject()->getAnimationProject()->setUnitCount(newUnitCount);
    }
    unitCountSliderValue->setText(QString::number(newUnitCount));
    qDebug() << "timelapse grab interval changed to" << newUnitCount << "sec";

    boundBeepTimeSlider();
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

    int newBeepCount = static_cast<int>(beepCountSlider->value());

    if (0 == newBeepCount) {
        newBeepCount = 1;
    }

    qDebug() << "change timelapse beep timer interval to" << newBeepCount << "second(s)";
    frontend->getProject()->getAnimationProject()->setBeepCount(newBeepCount);
    beepCountSliderValue->setText(QString::number(newBeepCount));

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
            connect(clickEffect, SIGNAL(statusChanged()),
                    this, SLOT(checkSoundEffectStatus()));
            clickEffect->setSource(QUrl::fromLocalFile(clickSoundFile));
            clickEffect->setVolume(0.25f);
        }

        if (DomainFacade::singleFrameMode == getRecordingMode()) {
            // Capture mode

            toolBar->setActualState(ToolBar::toolBarCameraOn);
        }
        else {
            // Time lapse mode

            if (beepCheckBox->isChecked()) {
                // Create timer for the time between beep and frame capture
                cameraTimer = new QTimer(this);
                cameraTimer->setSingleShot(true);
                QObject::connect(cameraTimer, SIGNAL(timeout()), this, SLOT(storeFrame()));

                QString beepSoundFile(frontend->getSoundsDirName());
                beepSoundFile.append(QLatin1String("beep.wav"));

                beepEffect = new QSoundEffect();
                connect(beepEffect, SIGNAL(statusChanged()),
                        this, SLOT(checkSoundEffectStatus()));
                beepEffect->setSource(QUrl::fromLocalFile(beepSoundFile));
                beepEffect->setVolume(0.25f);
            }

            // Create timer for the time between the beeps
            timelapseTimer = new QTimer(this);
            timelapseTimer->setSingleShot(false);
            QObject::connect(timelapseTimer, SIGNAL(timeout()), this, SLOT(sendBeep()));

            // Start the time lapse time
            quint64 msec = getTimelapseIntervalMsec();
            qDebug() << "start timelapse timer with interval" << msec << "msec";
            timelapseTimer->start(static_cast<int>(msec));
        }
    }
    else {
        // captureGroupBox->hide();
        enableCaptureGroupBox(true);
        enableTimelapseGroupBox(true);
        if (DomainFacade::singleFrameMode == getRecordingMode()) {
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
        case DomainFacade::minutesMode:
            factor = 1000;
            break;
        case DomainFacade::hoursMode:
            factor = 60000;
            break;
        case DomainFacade::daysMode:
            factor = 3600000;
            break;
        }
        timelapseTimer->setInterval(factor / newMixCount);
    }

    int viewingMode = viewingModeChooseCombo->currentIndex();
    switch (viewingMode) {
    case DomainFacade::mixImageMode:
        frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        break;
    case DomainFacade::diffImageMode:
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
    */
}


void RecordingTab::sendBeep()
{
    qDebug() << "RecordingTab::sendBeep --> Start";

    if (beepCheckBox->isChecked()) {
        beepEffect->play();

        // Start the timer for the frame capture
        const int ms = beepCountSlider->value() * 1000;
        qDebug() << "start timelapse beep timer with interval" << ms << "msec";
        cameraTimer->start(ms);
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
    if (!cameraOn) {
        // skip capturing when camera hasn't been activated yet
        return;
    }

    qDebug() << "RecordingTab::captureFrame --> Start";

    if (DomainFacade::singleFrameMode == getRecordingMode()) {
        // Capture mode

        toolBar->setActualState(ToolBar::toolBarNothing);
    }

    storeFrame();

    if (DomainFacade::singleFrameMode == getRecordingMode()) {
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


void RecordingTab::checkSoundEffectStatus()
{
    const QSoundEffect *effect = qobject_cast<const QSoundEffect *>(sender());
    if (!effect) {
        qCritical() << "slot checkSoundEffectStatus() was called from unexpected sender:"
                    << sender();
        return;
    }

    if (effect->status() == QSoundEffect::Error) {
        qCritical() << "an error was occured for sound effect" << effect->source();
        qCritical() << "verify the sound file's location";
    }
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
    case DomainFacade::qvgaImageSize:    // QVGA
        destWidth = 320;
        destHeight = 240;
        break;
    case DomainFacade::vgaImageSize:     // VGA
        destWidth = 640;
        destHeight = 480;
        break;
    case DomainFacade::svgaImageSize:    // SVGA
        destWidth = 800;
        destHeight = 600;
        break;
    case DomainFacade::paldImageSize:    // PAL D
        destWidth = 704;
        destHeight = 576;
        break;
    case DomainFacade::hdreadyImageSize: // HD Ready
        destWidth = 1280;
        destHeight = 720;
        break;
    case DomainFacade::fullhdImageSize:  // Full HD
        destWidth = 1900;
        destHeight = 1080;
        break;
    default: // Camera image size
        destWidth = imageWidth;
        destHeight = imageHeight;
        break;
    }

    switch (trans) {
    case DomainFacade::ScaleImage:
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

    case DomainFacade::ClipImage:
        // Clip the image to the output size
        switch (frontend->getProject()->getImageAdjustment()) {
        case DomainFacade::leftUp:
        case DomainFacade::leftMiddle:
        case DomainFacade::leftDown:
            x = 0;
            break;
        case DomainFacade::centerUp:
        case DomainFacade::centerMiddle:
        case DomainFacade::centerDown:
            x = (int)((imageWidth-destWidth)/2);
            break;
        case DomainFacade::rightUp:
        case DomainFacade::rightMiddle:
        case DomainFacade::rightDown:
            x = (int)(imageWidth-destWidth);
            break;
        }

        switch (frontend->getProject()->getImageAdjustment()) {
        case DomainFacade::leftUp:
        case DomainFacade::centerUp:
        case DomainFacade::rightUp:
            y = 0;
            break;
        case DomainFacade::leftMiddle:
        case DomainFacade::centerMiddle:
        case DomainFacade::rightMiddle:
            y = (int)((imageHeight-destHeight)/2);
            break;
        case DomainFacade::leftDown:
        case DomainFacade::centerDown:
        case DomainFacade::rightDown:
            y = (int)(imageHeight-destHeight);
            break;
        }

        outputImage = image.copy(x, y, destWidth, destHeight);

        break;

    case DomainFacade::ZoomImage:
        // Zoom the image

        double zoomValue = frontend->getProject()->getZoomValue();

        // 1. Step: Clip the image to the output size

        newImageWidth = imageWidth - ((imageWidth - destWidth) * zoomValue / 100.0);
        newImageHeight = imageHeight - ((imageHeight - destHeight) * zoomValue / 100.0);

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

void RecordingTab::boundBeepTimeSlider(int newBeepIntervalSec)
{
    const quint64 timelapseIntervalMs = getTimelapseIntervalMsec();
    quint64 beepIntervalMs = static_cast<quint64>(beepCountSlider->value()) * 1000;
    if (newBeepIntervalSec == -1) {
        beepIntervalMs = static_cast<quint64>(newBeepIntervalSec) * 1000;
    }

    // Rescale beep count slider according to timelapse grabbing interval
    qint64 maxBeepInterval = timelapseIntervalMs / 1000 - 1;
    if (maxBeepInterval <= 0) {
        qWarning() << "timelapse interval is less than 1 second. set beep interval to 1sec";
        maxBeepInterval = 1;
    }
    if (maxBeepInterval > 100) {
        maxBeepInterval = 100;
    }
    beepCountSlider->setScale(1, maxBeepInterval);
    beepCountSlider->setTotalSteps(static_cast<uint>(maxBeepInterval));

    // Reduce beep interval if it greater than timelaplse grabbing interval
    int newBeepCount = -1;
    if (beepIntervalMs >= timelapseIntervalMs) {
        qDebug() << "reduce timelapse beep interval to 1000msec because current value greater than grabbing interval:"
                 << beepIntervalMs << timelapseIntervalMs;
        newBeepCount = 1;
    } else if (newBeepIntervalSec == -1) {
        newBeepCount = newBeepIntervalSec;
    }
    if (newBeepCount > 0) {
        beepCountSlider->setValue(static_cast<double>(newBeepCount));
        beepCountSliderValue->setText(QString::number(newBeepCount));
        if (frontend->getProject() && frontend->getProject()->getAnimationProject()) {
            frontend->getProject()->getAnimationProject()->setBeepCount(newBeepCount);
        }
    }
}
