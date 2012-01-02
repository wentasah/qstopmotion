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

#include "frontends/qtfrontend/toolbar.h"

#include <QtCore/QtDebug>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ToolBar::ToolBar(Frontend *f,
                 CameraHandler *cameraHandler,
                 QWidget *parent) :
    QWidget(parent),
    cameraHandler(cameraHandler)
{
    frontend            = f;
    runAnimationTimer   = NULL;
    framesIcon          = 0;
    overlaySlider       = 0;
    cameraIcon          = 0;
    toBeginButton       = 0;
    previousFrameButton = 0;
    captureButton       = 0;
    playButton          = 0;
    nextFrameButton     = 0;
    toEndButton         = 0;
    fps                 = 0;
    exposureCount       = 0;

    frameNr = 0;
    isLooping = false;

    setObjectName("ToolBar");

    makeGUI();

    runAnimationTimer = new QTimer(this);
    QObject::connect(runAnimationTimer, SIGNAL(timeout()), this, SLOT(playNextFrame()));
}


void ToolBar::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    framesIcon = new QLabel();
    iconFile.append(QLatin1String("frames.png"));
    framesIcon->setPixmap(QPixmap(iconFile));

    overlaySlider = new QSlider(Qt::Horizontal);

    cameraIcon = new QLabel();
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("camera.png"));
    cameraIcon->setPixmap(QPixmap(iconFile));

    toBeginButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("tobegin.png"));
    iconFile.append(QLatin1String("firstexposure.png"));
    toBeginButton->setIcon(QPixmap(iconFile));
    // toBeginButton->setFlat(true);
    // toBeginButton->setFocusPolicy( Qt::NoFocus );
    connect(toBeginButton, SIGNAL(clicked()), this, SLOT(selectToBeginFrame()));
    toBeginButton->setEnabled(false);

    previousFrameButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("previousframe.png"));
    iconFile.append(QLatin1String("previousexposure.png"));
    previousFrameButton->setIcon(QPixmap(iconFile));
    // previousFrameButton->setFlat(true);
    connect(previousFrameButton, SIGNAL(clicked()), this, SLOT(selectPreviousFrame()));
    previousFrameButton->setEnabled(false);

    captureButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("capture.png"));
    captureButton->setIcon(QPixmap(iconFile));
    // captureButton->setFlat(true);
    // captureButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(captureButton, SIGNAL(clicked()), cameraHandler, SLOT(captureFrame()));

    playButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("play.png"));
    playButton->setIcon(QPixmap(iconFile));
    // playButton->setFlat(true);
    connect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));

    nextFrameButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("nextframe.png"));
    iconFile.append(QLatin1String("nextexposure.png"));
    nextFrameButton->setIcon(QPixmap(iconFile));
    // nextFrameButton->setFlat(true);
    // nextFrameButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(nextFrameButton, SIGNAL(clicked()), this, SLOT(selectNextFrame()));
    nextFrameButton->setEnabled(false);

    toEndButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("toend.png"));
    iconFile.append(QLatin1String("lastexposure.png"));
    toEndButton->setIcon(QPixmap(iconFile));
    // toEndButton->setFlat(true);
    // toEndButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(toEndButton, SIGNAL(clicked()), this, SLOT(selectToEndFrame()));
    toEndButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(2);
    // buttonLayout->addStretch(1);
    buttonLayout->addWidget(framesIcon);
    buttonLayout->addWidget(overlaySlider);
    buttonLayout->addWidget(cameraIcon);
    buttonLayout->addStretch(10);
    buttonLayout->addWidget(toBeginButton);
    buttonLayout->addWidget(previousFrameButton);
    buttonLayout->addWidget(captureButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(nextFrameButton);
    buttonLayout->addWidget(toEndButton);
    buttonLayout->addStretch(10);
    setLayout(buttonLayout);

}


void ToolBar::initialize()
{
    /*
    PreferencesTool *pref = PreferencesTool::get();
    int active = -1;

    int numEncoders = pref->getNumberEncoders();

    encoderTable->setRowCount(numEncoders);

    if (numEncoders > 0 ) {
        QString activeId = pref->getActiveEncoder();
        int idx = 0;

        for (int i = 0; i < numEncoders; ++i) {
            QString id(QString("encoder%1").arg(i));
            QString name;
            QString desc;
            QString start;
            QString stop;
            QString filename;

            if ( pref->getEncoder(id, name, desc, start, stop, filename) ) {
                encoderTable->setItem(i, 0, new QTableWidgetItem(name) );
                encoderTable->setItem(i, 1, new QTableWidgetItem(desc) );
                startEncoderStrings.push_back(start);
                stopEncoderStrings.push_back(stop);
                outputFiles.push_back(filename);

                if ( id.compare(activeId) == 0 )
                    active = idx;

                idx++;
            }
        }
    }

    if (active > -1) {
        encoderTable->setCurrentCell(active, 0);
    }
    */
}

/*
void ToolBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
*/

void ToolBar::apply()
{
    /*
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
    */
}


void ToolBar::toggleRunning()
{
    if (runAnimationTimer->isActive()) {
        stopAnimation();
    } else {
        runAnimation();
    }
}


void ToolBar::runAnimation()
{
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    exposureCount = frontend->getProject()->getTakeExposureSize(activeSceneIndex, activeTakeIndex);
    fps = frontend->getProject()->getFramesPerSecond();

    if (frontend->getProject()->getActiveSceneIndex() >= 0) {
        if (exposureCount > 0) {
            frontend->getProject()->initAudioDevice();
            QObject::disconnect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));
            QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));

            //playButton->setToggleButton(true);
            playButton->setChecked(true);
            playButton->toggle();
            frameNr = frontend->getProject()->getActiveExposureIndex();
            frontend->showMessage(tr("Running animation"), 2000);
            runAnimationTimer->start(1000 / fps);
            runAnimationTimer->setSingleShot(false);
        }
    }
}


void ToolBar::stopAnimation()
{
    if (runAnimationTimer->isActive()) {
        QObject::disconnect(playButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));
        QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));

        if (playButton->isChecked()) {
            playButton->toggle();
        }

        playButton->setChecked(false);

        frontend->getProject()->setActiveExposureIndex(frameNr);

        // frontend->shutdownAudioDevice();

        frontend->clearMessage();
        runAnimationTimer->stop();
        frontend->getProject()->setActiveExposureIndex(0);
    }
}


void ToolBar::pauseAnimation()
{
    if (runAnimationTimer->isActive()) {
        QObject::disconnect(playButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));
        QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));

        if (playButton->isChecked()) {
            playButton->toggle();
        }

        playButton->setChecked(false);

        frontend->getProject()->setActiveExposureIndex(frameNr);

        // frontend->shutdownAudioDevice();

        frontend->clearMessage();
        runAnimationTimer->stop();
    }

}

// TODO: No selectPreviousScene button, no implementation
void ToolBar::selectPreviousScene()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeSceneIndex > 0) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex - 1);
    }
    */
}

// TODO: No selectPreviousTake button, no implementation
void ToolBar::selectPreviousTake()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeSceneIndex > 0) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex - 1);
    }
    */
}


void ToolBar::selectToBeginFrame()
{
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    if (activeExposureIndex > 0) {
        frontend->getProject()->setActiveExposureIndex(0);
    }
}


void ToolBar::selectPreviousFrame()
{
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    if (activeExposureIndex > 0) {
        frontend->getProject()->setActiveExposureIndex(activeExposureIndex - 1);
    }
}


void ToolBar::selectNextFrame()
{
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    int exposureCount = frontend->getProject()->getTakeExposureSize(activeSceneIndex, activeTakeIndex);
    if ((activeExposureIndex > -1) && (activeExposureIndex < exposureCount - 1)) {
        frontend->getProject()->setActiveExposureIndex(activeExposureIndex + 1);
    }
}


void ToolBar::selectToEndFrame()
{
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int exposureCount = frontend->getProject()->getTakeExposureSize(activeSceneIndex, activeTakeIndex);
    if (0 < exposureCount) {
        frontend->getProject()->setActiveExposureIndex(exposureCount - 1);
    }
}

// TODO: No selectNextTake button, no implementation
void ToolBar::selectNextTake()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int sceneCount = frontend->getProject()->getSceneSize();
    if ((activeSceneIndex > -1) && (activeSceneIndex < sceneCount - 1)) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex + 1);
    }
    */
}

// TODO: No selectNextScene button, no implementation

void ToolBar::selectNextScene()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int sceneCount = frontend->getProject()->getSceneSize();
    if ((activeSceneIndex > -1) && (activeSceneIndex < sceneCount - 1)) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex + 1);
    }
    */
}


void ToolBar::toggleLooping()
{
    isLooping = !isLooping;
}


void ToolBar::playNextFrame()
{
    if (frontend->getProject()->getActiveSceneIndex() >= 0) {

        // frontend->playSound(frameNr);
        // frontend->getView()->notifyPlaySound(sceneIndex);

        if (isLooping) {
            frameNr = (frameNr < exposureCount - 1) ? frameNr + 1 : 0;
        }
        else {
            if (frameNr < exposureCount - 1) {
                ++frameNr;
            }
            else {
                this->stopAnimation();
            }
        }
    }
    else {
        stopAnimation();
    }
}
void ToolBar::modelSizeChanged(int modelSize)
{
    qDebug("ToolBar::modelSizeChanged --> Start");

    //Not <=1 because it is signed with a meaning for -1.
    if (modelSize == 0 || modelSize == 1) {
        if (previousFrameButton->isEnabled()) {
            previousFrameButton->setEnabled(false);
            nextFrameButton->setEnabled(false);
            playButton->setEnabled(false);
            // pauseButton->setEnabled(false);
            // stopButton->setEnabled(false);
            toEndButton->setEnabled(false);
            toBeginButton->setEnabled(false);
        }
    }
    else {
        if (modelSize >= 2) {
            if ( !previousFrameButton->isEnabled() && !cameraHandler->isCameraRunning() ) {
                previousFrameButton->setEnabled(true);
                nextFrameButton->setEnabled(true);
                playButton->setEnabled(true);
                // pauseButton->setEnabled(true);
                // stopButton->setEnabled(true);
                toEndButton->setEnabled(true);
                toBeginButton->setEnabled(true);
            }
        }
    }

    qDebug("ToolBar::modelSizeChanged --> End");
}

/*
void ToolBar::toBegin()
{
}


void ToolBar::previousFrame()
{
}


void ToolBar::play()
{
}


void ToolBar::capture()
{
}


void ToolBar::nextFrame()
{
}


void ToolBar::toEnd()
{
}
*/

void ToolBar::retranslateStrings()
{
    QString infoText = tr("<h4>Play animation (K, P)</h4>");
    playButton->setToolTip(infoText);
    /*
    infoText = tr("<h4>Previous scene (I)</h4>");
    previousSceneButton->setToolTip(infoText);

    infoText = tr("<h4>Previous scene (I)</h4>");
    previousTakeButton->setToolTip(infoText);
    */
    infoText = tr("<h4>First frame of the take (J, Left)</h4>");
    toBeginButton->setToolTip(infoText);

    infoText = tr("<h4>Previous frame (J, Left)</h4>");
    previousFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Previous frame (J, Left)</h4>");
    previousFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Next frame (L, Right)</h4>");
    nextFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Last frame of the take (L, Right)</h4>");
    toEndButton->setToolTip(infoText);
    /*
    infoText = tr("<h4>Next take (O)</h4>");
    nextTakeButton->setToolTip(infoText);

    infoText = tr("<h4>Next scene (O)</h4>");
    nextSceneButton->setToolTip(infoText);
    */
    infoText =
        tr("<h4>Capture Frame (Space)</h4> "
           "<p>Click on this button to <em>capture</em> a frame from the "
           "camera an put it in the animation</p> <p> This can also be "
           "done by pressing the <b>Space key</b></p>");
    captureButton->setWhatsThis(infoText);
    captureButton->setToolTip(infoText);

}
