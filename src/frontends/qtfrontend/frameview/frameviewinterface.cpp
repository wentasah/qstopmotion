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

#include "frameviewinterface.h"

#include "technical/grabber/gstreamergrabber.h"

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
    grabber = 0;
    grabThread = 0;

    this->framesPerSecond = fps;

    isPlayingVideo = false;

    widthConst  = 4;
    heightConst = 3;
    mixCount = 2;
    viewingMode = 0;

    QString rootDir;

    rootDir.append(frontend->getUserDirName());
    rootDir.append("/");
    rootDir.append("capturedfile.jpg");

    capturedImg.append(rootDir);

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

    if (grabber) {
        delete grabber;
        grabber = NULL;
    }
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


void FrameViewInterface::updateViewingMode(int newViewingMode)
{
    qDebug("FrameViewInterface::updateViewingMode --> Start");

    this->setViewingMode(newViewingMode);

    qDebug("FrameViewInterface::updateViewingMode --> End");
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

// TODO: Refactor this terrible ugly method. This one is really bad!!
bool FrameViewInterface::on()
{
    qDebug("FrameViewInterface::on --> Start");

    GstreamerGrabber::GstreamerSource activeSource = (GstreamerGrabber::GstreamerSource)frontend->getProject()->getVideoSource();
    this->grabber = new GstreamerGrabber(activeSource, capturedImg);

    grabber->initialization();
    if (grabber->isGrabberInitialized()) {
        this->initCompleted();
        this->isPlayingVideo = true;

        // If the grabber is running in it's own process we use a timer.
        if (grabber->isGrabberProcess()) {
            grabber->init();
            if (grabber->isGrabberInited()) {
                grabTimer.start(150);
            } else {
                frontend->showWarning(tr("Check image grabber"),
                                      tr("Grabbing failed. This may happen if you try\n"
                                         "to grab from an invalid device. Please check\n"
                                         "your grabber settings in the preferences menu."));
                return false;
            }
        }
        // Otherwise a thread is needed
        else {
            grabThread = new ImageGrabThread(this, grabber);
            grabThread->start();
            grabThread->wait(500);

            if (grabThread->wasGrabbingSuccess() == false) {
                frontend->showWarning(tr("Check image grabber"),
                                      tr("Grabbing failed. This may happen if you try\n"
                                         "to grab from an invalid device. Please check\n"
                                         "your grabber settings in the preferences menu."));
                return false;
            }
        }
    } else {
        frontend->hideProgress();
        frontend->showWarning(tr("Select image grabber"),
                              tr("You have to define an image grabber to use.\n"
                                 "This can be set in the preferences menu."));
        return false;
    }

    qDebug("FrameViewInterface::on --> End");
    return true;
}


void FrameViewInterface::off()
{
    qDebug("FrameViewInterface::off --> Start");

    if (grabber != 0) {
        if (grabber->isGrabberProcess()) {
            grabber->tearDown();
            grabTimer.stop();
            playbackTimer.stop();
        }
        delete grabber;
        grabber = 0;
    }

    if (grabThread != 0) {
        grabThread->terminate();
        grabThread->wait();
        delete grabThread;
        grabThread = 0;
    }

    clearImageBuffer();

    this->isPlayingVideo = false;

    this->update();

    qDebug("FrameViewInterface::off --> End");
}


int FrameViewInterface::getViewingMode() const
{
    return viewingMode;
}


bool FrameViewInterface::setViewingMode(int mode)
{
    qDebug("FrameViewInterface::setViewMode --> Start");

    // Going into playback mode.
    if (mode == 2 && this->viewingMode != 2) {
        if (grabber->isGrabberProcess()) {
            grabTimer.stop();
            playbackTimer.start(1000 / framesPerSecond);
        } else {
            qDebug("FrameViewImage::setViewMode --> End (false)");
            return false;
        }
    }
    // Going out of playback mode.
    else if (mode != 2 && this->viewingMode == 2) {
        if (grabber->isGrabberProcess()) {
            playbackTimer.stop();
            grabTimer.start(150);
        }
    }

    this->viewingMode = mode;

    qDebug("FrameViewInterface::setViewMode --> End (true)");
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
