/******************************************************************************
 *  Copyright (C) 2010-2012 by                                                *
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

#include "frameviewinterface.h"

#include <QtCore/QDir>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QX11Info>

FrameViewInterface::FrameViewInterface(Frontend *f, QWidget *parent, int fps)
    :QWidget(parent)
{
    qDebug("FrameViewInterface::Constructor --> Start");

    setAttribute(Qt::WA_PaintOnScreen);      // Paint directly on the screen, no composition management (only on X11 systems)
    // setAttribute(Qt::WA_PaintUnclipped);     // All painter operations unclipped
    setAttribute(Qt::WA_NoSystemBackground); // There is no background image
    // setAttribute(Qt::WA_OpaquePaintEvent);   // No erase of the image before updated

    frontend = f;

    this->framesPerSecond = fps;

    isPlayingVideo = false;

    widthConst  = 4;
    heightConst = 3;
    mixCount = 2;
    mixMode = 0;

    setNormalRatio();

    frontend->getView()->attatch(this);

    qDebug("FrameViewInterface::Constructor --> FrameView is attatched to the ViewFacade");

    connect(&grabTimer, SIGNAL(timeout()), this, SLOT(redraw()));
    connect(&playbackTimer, SIGNAL(timeout()), this, SLOT(nextPlayBack()));

    qDebug("FrameViewInterface::Constructor --> End");
}


FrameViewInterface::~FrameViewInterface()
{
    qDebug("FrameViewInterface::Destructor --> Start");

    frontend = NULL;

    qDebug("FrameViewInterface::Destructor --> End");
}


Frontend* FrameViewInterface::getFrontend()
{
    Q_ASSERT(frontend != NULL);

    return frontend;
}

/*
void FrameViewInterface::setFrontend(Frontend* f)
{
    frontend = f;
}
*/

void FrameViewInterface::setWidescreenRatio()
{
    qDebug("FrameViewInterface::setWidescreenRatio --> Start");

    widthConst  = 16;
    heightConst = 9;
    float minWidth = 300 * (16 / 9);
    this->setMinimumSize((int)minWidth, 300);
    this->update();

    qDebug("FrameViewInterface::setWidescreenRatio --> End");
}


void FrameViewInterface::setNormalRatio()
{
    qDebug("FrameViewInterface::setNormalRatio --> Start");

    widthConst  = 4;
    heightConst = 3;
    this->setMinimumSize(400, 300);
    this->update();

    qDebug("FrameViewInterface::setNormalRatio --> End");
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void FrameViewInterface::updateNewProject()
{
    qDebug("FrameViewInterface::updateNewProject --> Nothing");

    // this->newProject();

    // qDebug("FrameViewInterface::updateNewProject --> End");
}


void FrameViewInterface::updateDescriptionsUpdated()
{
    qDebug("FrameViewInterface::updateDescriptionsUpdated --> Nothing");

    // this->descriptionsUpdated();

    // qDebug("FrameViewInterface::updateDescriptionsUpdated --> End");
}


void FrameViewInterface::updateMixMode(int newMixMode)
{
    qDebug("FrameViewInterface::updateMixMode --> Start");

    this->setMixMode(newMixMode);

    qDebug("FrameViewInterface::updateMixMode --> End");
}


void FrameViewInterface::updateMixCount(int newMixCount)
{
    qDebug("FrameViewInterface::updateMixCount --> Start");

    this->mixCount = newMixCount;

    qDebug("FrameViewInterface::updateMixCount --> End");
}


void FrameViewInterface::updateFramesPerSecond(int newFps)
{
    qDebug("FrameViewInterface::updateFramesPerSecond --> Start");

    this->setFps(newFps);

    qDebug("FrameViewInterface::updateFramesPerSecond --> End");
}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void FrameViewInterface::updateAddScene(int)
{
    qDebug("FrameViewInterface::updateAddScene --> Empty");
}

void FrameViewInterface::updateInsertScene(int)
{
    qDebug("FrameViewInterface::updateInsertScene --> Empty");
}

// void FrameViewInterface::updateSetNewActiveScene(int) {}

void FrameViewInterface::updateActivateScene()
{
    qDebug("FrameViewInterface::updateActivateScene --> Start");

    activateScene();

    qDebug("FrameViewInterface::updateActivateScene --> End");
}

void FrameViewInterface::updateMoveScene(int, int) {}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void FrameViewInterface::updateAddTake(int, int)
{
    qDebug("FrameViewInterface::updateAddTake --> Empty");
}

void FrameViewInterface::updateInsertTake(int, int)
{
    qDebug("FrameViewInterface::updateInsertTake --> Empty");
}

// void FrameViewInterface::updateSetNewActiveTake(int) {}


void FrameViewInterface::updateActivateTake()
{
    qDebug("FrameViewInterface::updateActivateTake --> Start");

    qDebug("FrameViewInterface::updateActivateTake --> End");
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void FrameViewInterface::updateRemoveExposure(int, int, int) {}


void FrameViewInterface::updateRemoveExposures(int, int) {}


void FrameViewInterface::updateMoveExposures(int, int, int) {}


/**************************************************************************
 * Frameview functions
 **************************************************************************/

bool FrameViewInterface::cameraOn()
{
    qDebug("FrameViewInterface::cameraOn --> Start");

    this->initCompleted();

    this->isPlayingVideo = true;
    grabTimer.start(150);

    qDebug("FrameViewInterface::cameraOn --> End");
    return true;
}


void FrameViewInterface::cameraOff()
{
    qDebug("FrameViewInterface::cameraOff --> Start");

    grabTimer.stop();
    playbackTimer.stop();

    clearImageBuffer();

    this->isPlayingVideo = false;
    this->update();

    qDebug("FrameViewInterface::cameraOff --> End");
}


int FrameViewInterface::getMixMode() const
{
    return mixMode;
}


bool FrameViewInterface::setMixMode(int mode)
{
    qDebug("FrameViewInterface::setMixMode --> Start");

    // Going into playback mode.
    if (mode == 2 && this->mixMode != 2) {
        grabTimer.stop();
        playbackTimer.start(1000 / framesPerSecond);
    }
    // Going out of playback mode.
    else if (mode != 2 && mixMode == 2) {
        playbackTimer.stop();
        grabTimer.start(150);
    }

    mixMode = mode;

    qDebug("FrameViewInterface::setMixMode --> End (true)");
    return true;
}


void FrameViewInterface::setMixCount(int mixCount)
{
    this->mixCount = mixCount;
}


void FrameViewInterface::setFps(int fps)
{
    this->framesPerSecond = fps;
    if (playbackTimer.isActive()) {
        playbackTimer.setInterval(1000 / fps);
    }
}

/**************************************************************************
 * Private functions
 **************************************************************************/

void FrameViewInterface::activateScene()
{
    qDebug("FrameViewInterface::activateScene --> Start");

    // activateTake();

    qDebug("FrameViewInterface::activateScene --> End");
}

void FrameViewInterface::activateTake()
{
    qDebug("FrameViewInterface::activateTake --> Start");

    // activateExposure();

    qDebug("FrameViewInterface::activateTake --> End");
}
