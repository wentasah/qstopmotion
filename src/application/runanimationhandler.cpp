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

#include "runanimationhandler.h"


RunAnimationHandler::RunAnimationHandler(Frontend *f,
                                         QObject *parent,
                                         const char *name)
    : QObject(parent)
{
    frontend           = f;
    playButton         = NULL;
    pauseButton        = NULL;
    loopButton         = NULL;
    timer              = NULL;
    fps                = 0;
    exposureCount      = 0;

    frameNr = 0;
    isLooping = false;

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(playNextFrame()));
    setObjectName(name);
}


RunAnimationHandler::~RunAnimationHandler()
{
    frontend = NULL;
}


void RunAnimationHandler::setPlayButton(QPushButton * playButton)
{
    this->playButton = playButton;
}


void RunAnimationHandler::setLoopButton(QPushButton * loopButton)
{
    this->loopButton = loopButton;
}


void RunAnimationHandler::toggleRunning()
{
    if (timer->isActive()) {
        stopAnimation();
    } else {
        runAnimation();
    }
}


void RunAnimationHandler::runAnimation()
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
            timer->start(1000 / fps);
            timer->setSingleShot(false);
        }
    }
}


void RunAnimationHandler::stopAnimation()
{
    if (timer->isActive()) {
        QObject::disconnect(playButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));
        QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));

        if (playButton->isChecked()) {
            playButton->toggle();
        }

        playButton->setChecked(false);

        frontend->getProject()->setActiveExposureIndex(frameNr);

        // frontend->shutdownAudioDevice();

        frontend->clearMessage();
        timer->stop();
        frontend->getProject()->setActiveExposureIndex(0);
    }
}


void RunAnimationHandler::setPauseButton(QPushButton * pauseButton)
{
    this->pauseButton = pauseButton;
}


void RunAnimationHandler::pauseAnimation()
{
    if (timer->isActive()) {
        QObject::disconnect(playButton, SIGNAL(clicked()), this, SLOT(pauseAnimation()));
        QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(runAnimation()));

        if (playButton->isChecked()) {
            playButton->toggle();
        }

        playButton->setChecked(false);

        frontend->getProject()->setActiveExposureIndex(frameNr);

        // frontend->shutdownAudioDevice();

        frontend->clearMessage();
        timer->stop();
    }

}

// TODO: No selectPreviousScene button, no implementation
void RunAnimationHandler::selectPreviousScene()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeSceneIndex > 0) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex - 1);
    }
    */
}

// TODO: No selectPreviousTake button, no implementation
void RunAnimationHandler::selectPreviousTake()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeSceneIndex > 0) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex - 1);
    }
    */
}


void RunAnimationHandler::selectToBeginFrame()
{
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    if (activeExposureIndex > 0) {
        frontend->getProject()->setActiveExposureIndex(0);
    }
}


void RunAnimationHandler::selectPreviousFrame()
{
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    if (activeExposureIndex > 0) {
        frontend->getProject()->setActiveExposureIndex(activeExposureIndex - 1);
    }
}


void RunAnimationHandler::selectNextFrame()
{
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    int exposureCount = frontend->getProject()->getTakeExposureSize(activeSceneIndex, activeTakeIndex);
    if ((activeExposureIndex > -1) && (activeExposureIndex < exposureCount - 1)) {
        frontend->getProject()->setActiveExposureIndex(activeExposureIndex + 1);
    }
}


void RunAnimationHandler::selectToEndFrame()
{
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int exposureCount = frontend->getProject()->getTakeExposureSize(activeSceneIndex, activeTakeIndex);
    if (0 < exposureCount) {
        frontend->getProject()->setActiveExposureIndex(exposureCount - 1);
    }
}

// TODO: No selectNextTake button, no implementation
void RunAnimationHandler::selectNextTake()
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

void RunAnimationHandler::selectNextScene()
{
    /*
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int sceneCount = frontend->getProject()->getSceneSize();
    if ((activeSceneIndex > -1) && (activeSceneIndex < sceneCount - 1)) {
        frontend->getProject()->setActiveSceneIndex(activeSceneIndex + 1);
    }
    */
}


void RunAnimationHandler::toggleLooping()
{
    isLooping = !isLooping;
}


void RunAnimationHandler::playNextFrame()
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
