/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
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

#include <QtCore/QtDebug>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


RecordingTab::RecordingTab(Frontend *f,
                           RunAnimationHandler *rah,
                           CameraHandler *ch,
                           QWidget *parent) :
    QWidget(parent)
{
    frontend               = f;
    runAnimationHandler    = rah;
    cameraHandler          = ch;

    cameraGroupBox         = 0;
    videoSourceCombo       = 0;
    cameraButton           = 0;

    captureGroupBox        = 0;
    viewingModeChooseCombo = 0;
    unitModeChooseCombo    = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;

    playAccel              = 0;
    mixAccel               = 0;
    diffAccel              = 0;
    playbackAccel          = 0;

    captureTimer = new QTimer(this);
    connect(captureTimer, SIGNAL(timeout()), cameraHandler, SLOT(captureFrame()));

    this->setObjectName("RecordingTab");

    makeGUI();

    createAccelerators();
}


void RecordingTab::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;
    QVector<QString> deviceNames = frontend->getDeviceNames();
    int deviceSize = deviceNames.size();

    // ========================================================================
    // Camera group box
    cameraGroupBox = new QGroupBox(tr("Camera"));
    // cameraGroupBox->setFlat(true);

    videoSourceCombo = new QComboBox();
    videoSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoSourceCombo, SIGNAL(activated(int)), this, SLOT(changeVideoSource(int)));
    for (int deviceIndex = 0 ; deviceIndex < deviceSize ; deviceIndex++) {
        videoSourceCombo->addItem(deviceNames[deviceIndex]);
    }

    cameraButton = new QPushButton;
    iconFile.append(QLatin1String("cameraon.png"));
    cameraButton->setIcon(QPixmap(iconFile));
    // cameraButton->setFlat(true);
    cameraButton->setFocusPolicy(Qt::NoFocus);
    cameraHandler->setCameraButton(cameraButton);
    connect(cameraButton, SIGNAL(clicked()), cameraHandler, SLOT(toggleCamera()));
    // cameraButton->setEnabled(false);

    QVBoxLayout *cameraLayout = new QVBoxLayout;
    cameraLayout->setMargin(4);
    // cameraLayout->setSpacing(2);
    // cameraLayout->addStretch(1);
    cameraLayout->addWidget(videoSourceCombo);
    cameraLayout->addWidget(cameraButton);
    cameraLayout->addStretch(10);
    cameraGroupBox->setLayout(cameraLayout);

    // ========================================================================
    // Capture group box
    captureGroupBox = new QGroupBox(tr("Capture"));
    // secondGroupBox->setFlat(true);

    viewingModeChooseCombo = new QComboBox();
    viewingModeChooseCombo->setFocusPolicy(Qt::NoFocus);
    connect(viewingModeChooseCombo, SIGNAL(activated(int)), this, SLOT(changeViewingMode(int)));

    unitModeChooseCombo = new QComboBox();
    unitModeChooseCombo->setFocusPolicy(Qt::NoFocus);
    unitModeChooseCombo->setEnabled(false);
    connect(unitModeChooseCombo, SIGNAL(activated(int)), this, SLOT(changeUnitMode(int)));

    mixCountSliderCaption = new QLabel();

    mixCountSlider = new QSlider(Qt::Horizontal);
    mixCountSlider->setMinimum(0);
    mixCountSlider->setMaximum(5);
    mixCountSlider->setPageStep(1);
    mixCountSlider->setValue(2);
    mixCountSlider->setTickPosition(QSlider::TicksBelow);
    mixCountSlider->setFocusPolicy(Qt::NoFocus);
    connect(mixCountSlider, SIGNAL(valueChanged(int)), this, SLOT(changeMixCount(int)));

    QVBoxLayout *captureLayout = new QVBoxLayout;
    captureLayout->setMargin(4);
    // captureLayout->setSpacing(2);
    // captureLayout->addStretch(1);
    captureLayout->addWidget(viewingModeChooseCombo);
    captureLayout->addWidget(unitModeChooseCombo);
    captureLayout->addWidget(mixCountSliderCaption);
    captureLayout->addWidget(mixCountSlider);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);
    captureGroupBox->hide();

    // ========================================================================
    // Tab layout
    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(cameraGroupBox);
    tabLayout->addWidget(captureGroupBox);
    // tabLayout->addWidget(thirdGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);
}


void RecordingTab::initialize()
{
    qDebug() << "RecordingTab::initialize --> Start";

    // PreferencesTool *pref = PreferencesTool::get();
    int videoSource = frontend->getProject()->getAnimationProject()->getVideoSource();
    this->videoSourceCombo->setCurrentIndex(videoSource);

    int viewingMode = frontend->getProject()->getAnimationProject()->getViewingMode();
    changeViewingMode(viewingMode);

    int unitMode = frontend->getProject()->getAnimationProject()->getUnitMode();
    this->unitModeChooseCombo->setCurrentIndex(unitMode);

    qDebug() << "RecordingTab::initialize --> End";
}

/*
void RecordingTab::resizeEvent(QResizeEvent *event)
{
    qDebug("RecordingTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("RecordingTab::resizeEvent --> End");
}
*/

/*
void RecordingTab::apply()
{
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("encoder%1").arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("encoder%1").arg(i),
                             encoderTable->item(i, 0)->text(),
                             encoderTable->item(i, 1)->text(),
                             startEncoderStrings[i],
                             stopEncoderStrings[i],
                             outputFiles[i]);
        }
    }
    else {
        pref->setNumberEncoders(0);
        pref->setActiveEncoder(QString());
    }
}
*/


void RecordingTab::retranslateStrings()
{
    cameraGroupBox->setTitle(tr("Camera"));

    captureGroupBox->setTitle(tr("Capture"));
    viewingModeChooseCombo->clear();
    viewingModeChooseCombo->addItem(tr("Mix"));
    viewingModeChooseCombo->addItem(tr("Diff"));
    viewingModeChooseCombo->addItem(tr("Playback"));
    viewingModeChooseCombo->addItem(tr("Auto"));

    unitModeChooseCombo->clear();
    unitModeChooseCombo->addItem("");
    unitModeChooseCombo->addItem(tr("Pr sec"));
    unitModeChooseCombo->addItem(tr("Pr min"));
    unitModeChooseCombo->addItem(tr("Pr hr"));
    unitModeChooseCombo->setCurrentIndex(0);

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
}


void RecordingTab::setMixingMode()
{
    changeViewingMode(0);
}


void RecordingTab::setDiffingMode()
{
    changeViewingMode(1);
}


void RecordingTab::setPlaybackMode()
{
    changeViewingMode(2);
}


void RecordingTab::changeVideoSource(int index)
{
    qDebug() << "RecordingTab::changeSource --> Start";

    frontend->getProject()->getAnimationProject()->setVideoSource(index);

    qDebug() << "RecordingTab::changeSource --> End";
}


void RecordingTab::changeViewingMode(int newViewingMode)
{
    qDebug() << "RecordingTab::changeViewingMode --> Start";

    frontend->getProject()->getAnimationProject()->setViewingMode(newViewingMode);

    frontend->getProject()->getView()->notifyNewViewingMode(newViewingMode);

    // viewingModeChooseCombo->setCurrentIndex(newViewingMode);
    switch (newViewingMode) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(5);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getMixCount());
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        captureTimer->stop();
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        mixCountSlider->setValue(1);
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        captureTimer->stop();
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(50);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getPlaybackCount());
        unitModeChooseCombo->setEnabled(false);
        unitModeChooseCombo->setCurrentIndex(0);
        captureTimer->stop();
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
        Q_ASSERT(newViewingMode < 4);
        break;
    }

    qDebug() << "RecordingTab::changeViewingMode --> End";
}


void RecordingTab::changeUnitMode(int index)
{
    qDebug() << "RecordingTab::changeUnitMode --> Start";

    frontend->getProject()->getAnimationProject()->setUnitMode(index);

    int sliderValue = mixCountSlider->value();
    if (sliderValue == 0 || index == 0) {
        if (captureTimer->isActive()) {
            captureTimer->stop();
        }
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
        if (captureTimer->isActive()) {
            captureTimer->stop();
        }
        break;
    }

    if (captureTimer->isActive() == false) {
        // Grab the first frame manually
        cameraHandler->captureFrame();
        // then grab at the given interval
        captureTimer->start(factor / sliderValue);
    } else {
        captureTimer->setInterval(factor / sliderValue);
    }

    qDebug() << "RecordingTab::changeUnitMode --> End";
}


void RecordingTab::changeMixCount(int newMixCount)
{
    if (captureTimer->isActive() && newMixCount != 0) {
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
        captureTimer->setInterval(factor / newMixCount);
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


void RecordingTab::cameraOn(bool isOn)
{
    qDebug("RecordingTab::cameraOn --> Start");

    if (isOn) {
        runAnimationHandler->stopAnimation();
        captureGroupBox->show();
    } else {
        if (captureTimer->isActive()) {
            captureTimer->stop();
        }
        captureGroupBox->hide();
    }

    qDebug("RecordingTab::cameraOn --> Stop");
}


void RecordingTab::createAccelerators()
{
    playAccel = new QShortcut(QKeySequence(Qt::Key_K), this);
    connect(playAccel, SIGNAL(activated()), runAnimationHandler, SLOT(toggleRunning()));

    mixAccel = new QShortcut(QKeySequence(Qt::Key_1), this);
    connect(mixAccel, SIGNAL(activated()), this, SLOT(setMixingMode()));

    diffAccel = new QShortcut(QKeySequence(Qt::Key_2), this);
    connect(diffAccel, SIGNAL(activated()), this, SLOT(setDiffingMode()));

    playbackAccel = new QShortcut(QKeySequence(Qt::Key_3), this);
    connect(playbackAccel, SIGNAL(activated()), this, SLOT(setPlaybackMode()));
}



