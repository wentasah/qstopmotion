/******************************************************************************
 *  Copyright (C) 2010-2017 by                                                *
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

#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QRect>
#include <QSize>
#include <QString>

#include "domain/animation/animationproject.h"


FrameViewInterface::FrameViewInterface(Frontend *f, QWidget *parent, double fps)
    :QWidget(0)
{
    qDebug() << "FrameViewInterface::Constructor --> Start";

    frontend = f;

    this->liveViewFps = fps;

    displayMode = logoMode;

    widthConst  = 4;
    heightConst = 3;
    frameViewWidth = 0;
    frameViewHeight = 0;
    mixCount = 2;
    mixMode = 0;

    setNormalRatio();

    frontend->getView()->attach(this);

    qDebug() << "FrameViewInterface::Constructor --> FrameView is attached to the ViewFacade";

    connect(&grabTimer, SIGNAL(timeout()), this, SLOT(redraw()));

    qDebug() << "FrameViewInterface::Constructor --> End";
}


FrameViewInterface::~FrameViewInterface()
{
    qDebug() << "FrameViewInterface::Destructor --> Start";

    frontend = NULL;

    qDebug() << "FrameViewInterface::Destructor --> End";
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
    qDebug() << "FrameViewInterface::setWidescreenRatio --> Start";

    widthConst  = 16;
    heightConst = 9;
    float minWidth = 300 * (16 / 9);
    this->setMinimumSize((int)minWidth, 300);
    this->update();

    qDebug() << "FrameViewInterface::setWidescreenRatio --> End";
}


void FrameViewInterface::setNormalRatio()
{
    qDebug() << "FrameViewInterface::setNormalRatio --> Start";

    widthConst  = 4;
    heightConst = 3;
    this->setMinimumSize(400, 300);
    this->update();

    qDebug() << "FrameViewInterface::setNormalRatio --> End";
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void FrameViewInterface::updateNewProject()
{
    qDebug() << "FrameViewInterface::updateNewProject --> Nothing";

    // this->newProject();

    // qDebug() << "FrameViewInterface::updateNewProject --> End";
}


void FrameViewInterface::updateOpenProject()
{
    qDebug() << "FrameViewInterface::updateOpenProject --> Nothing";

    // this->openProject();

    // qDebug() << "FrameViewInterface::updateOpenProject --> End";
}


void FrameViewInterface::updateMixMode(int newMixMode)
{
    qDebug() << "FrameViewInterface::updateMixMode --> Start";

    this->setMixMode(newMixMode);

    qDebug() << "FrameViewInterface::updateMixMode --> End";
}


void FrameViewInterface::updateMixCount(int newMixCount)
{
    qDebug() << "FrameViewInterface::updateMixCount --> Start";

    this->mixCount = newMixCount;

    qDebug() << "FrameViewInterface::updateMixCount --> End";
}


void FrameViewInterface::updateLiveViewFps(double newFps)
{
    qDebug() << "FrameViewInterface::updateLiveViewFps --> Start";

    this->setLiveViewFps(newFps);

    qDebug() << "FrameViewInterface::updateLiveViewFps --> End";
}


void FrameViewInterface::updateVideoFps(int newFps)
{
    qDebug() << "FrameViewInterface::updateVideoFps --> Start";

    this->setVideoFps(newFps);

    qDebug() << "FrameViewInterface::updateVideoFps --> End";
}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void FrameViewInterface::updateAddScene(int)
{
    qDebug() << "FrameViewInterface::updateAddScene --> Empty";
}

void FrameViewInterface::updateInsertScene(int)
{
    qDebug() << "FrameViewInterface::updateInsertScene --> Empty";
}

// void FrameViewInterface::updateSetNewActiveScene(int) {}

void FrameViewInterface::updateActivateScene()
{
    qDebug() << "FrameViewInterface::updateActivateScene --> Start";

    activateScene();

    qDebug() << "FrameViewInterface::updateActivateScene --> End";
}

void FrameViewInterface::updateMoveScene(int, int) {}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void FrameViewInterface::updateAddTake(int, int)
{
    qDebug() << "FrameViewInterface::updateAddTake --> Empty";
}

void FrameViewInterface::updateInsertTake(int, int)
{
    qDebug() << "FrameViewInterface::updateInsertTake --> Empty";
}

// void FrameViewInterface::updateSetNewActiveTake(int) {}


void FrameViewInterface::updateActivateTake()
{
    qDebug() << "FrameViewInterface::updateActivateTake --> Start";

    qDebug() << "FrameViewInterface::updateActivateTake --> End";
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void FrameViewInterface::updateRemoveExposure(int, int, int) {}


void FrameViewInterface::updateMoveExposures(int, int, int) {}


/**************************************************************************
 * Frameview functions
 **************************************************************************/

bool FrameViewInterface::cameraOn()
{
    qDebug() << "FrameViewInterface::cameraOn --> Start";

    initCompleted();

    displayMode = liveImageMode;

    grabTimer.start(1000 / liveViewFps);

    qDebug() << "FrameViewInterface::cameraOn --> End";
    return true;
}


void FrameViewInterface::cameraOff()
{
    qDebug() << "FrameViewInterface::cameraOff --> Start";

    grabTimer.stop();

    clearImageBuffer();

    // Clear the display
    displayMode = emptyMode;
    repaint();

    if (0 == frontend->getProject()->getTotalExposureSize()) {
        showLogo();
    }
    else {
        displayMode = stillImageMode;
        activateExposure();
    }
    this->update();

    qDebug() << "FrameViewInterface::cameraOff --> End";
}


int FrameViewInterface::getMixMode() const
{
    return mixMode;
}


bool FrameViewInterface::setMixMode(int mode)
{
    qDebug() << "FrameViewInterface::setMixMode --> Start";

    mixMode = mode;

    qDebug() << "FrameViewInterface::setMixMode --> End (true)";
    return true;
}


void FrameViewInterface::setMixCount(int mixCount)
{
    this->mixCount = mixCount;
}


void FrameViewInterface::setLiveViewFps(double fps)
{
    this->liveViewFps = fps;
    if (grabTimer.isActive()) {
        grabTimer.setInterval(1000 / fps);
    }
}


void FrameViewInterface::setVideoFps(int fps)
{
    this->videoFps = fps;
    if (playbackTimer.isActive()) {
        playbackTimer.setInterval(1000 / fps);
    }
}

/**************************************************************************
 * Protected functions
 **************************************************************************/

void FrameViewInterface::activateScene()
{
    qDebug() << "FrameViewInterface::activateScene --> Start";

    // activateTake();

    qDebug() << "FrameViewInterface::activateScene --> End";
}


void FrameViewInterface::activateTake()
{
    qDebug() << "FrameViewInterface::activateTake --> Start";

    // activateExposure();

    qDebug() << "FrameViewInterface::activateTake --> End";
}


QImage FrameViewInterface::clipImage(QImage image)
{
    QImage outputImage;
    QImage clipImage;

    double destWidth = 0;
    double destHeight = 0;
    double imageWidth = image.width();
    double imageHeight = image.height();
    double newImageWidth = 0;
    double newImageHeight = 0;
    double widthScale = 0;
    double heightScale = 0;
    int    x = 0;
    int    y = 0;
    int    trans = frontend->getProject()->getImageTransformation();

    if (displayMode == liveImageMode) {
        // Cliping and zooming is only necessary for live image mode

        switch (frontend->getProject()->getVideoSize()) {
        case DomainFacade::qvgaSize:    // QVGA
            destWidth = 320;
            destHeight = 240;
            break;
        case DomainFacade::vgaSize:     // VGA
            destWidth = 640;
            destHeight = 480;
            break;
        case DomainFacade::svgaSize:    // SVGA
            destWidth = 800;
            destHeight = 600;
            break;
        case DomainFacade::paldSize:    // PAL D
            destWidth = 704;
            destHeight = 576;
            break;
        case DomainFacade::hdreadySize: // HD Ready
            destWidth = 1280;
            destHeight = 720;
            break;
        case DomainFacade::fullhdSize:  // Full HD
            destWidth = 1900;
            destHeight = 1080;
            break;
        default: // Camera image size
            destWidth = imageWidth;
            destHeight = imageHeight;
            break;
        }

        switch (trans) {
        case AnimationProject::ScaleImage:
            // Scale the image to the output size

            widthScale = imageWidth / frameViewWidth;
            heightScale = imageHeight / frameViewHeight;

            if (widthScale > heightScale) {
                outputImage = image.scaledToWidth(frameViewWidth);
            }
            else {
                outputImage = image.scaledToHeight(frameViewHeight);
            }

            break;
        case AnimationProject::ClipImage:
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

            clipImage = image.copy(x, y, destWidth, destHeight);

            widthScale = imageWidth / frameViewWidth;
            heightScale = imageHeight / frameViewHeight;

            if (widthScale > heightScale) {
                outputImage = clipImage.scaledToWidth(frameViewWidth);
            }
            else {
                outputImage = clipImage.scaledToHeight(frameViewHeight);
            }

            break;

        case AnimationProject::ZoomImage:
            // Zoom the image

            int zoomValue = frontend->getProject()->getZoomValue();

            // 1. Step: Clip the image to the output size

            newImageWidth = imageWidth - ((imageWidth - destWidth) * zoomValue / 100);
            newImageHeight = imageHeight - ((imageHeight - destHeight) * zoomValue / 100);

            x = (int)((imageWidth-newImageWidth)/2);
            y = (int)((imageHeight-newImageHeight)/2);

            clipImage = image.copy(x, y, newImageWidth, newImageHeight);

            // 2. Step: Scale up the image

            widthScale = newImageWidth / frameViewWidth;
            heightScale = newImageHeight / frameViewHeight;

            if (widthScale > heightScale) {
                outputImage = clipImage.scaledToWidth(frameViewWidth);
            }
            else {
                outputImage = clipImage.scaledToHeight(frameViewHeight);
            }

            break;
        }
    }
    else {
        // All other modes - nothing to do

        outputImage = image;
    }

    return outputImage;
}


QImage FrameViewInterface::scaleImage(QImage image)
{
    QImage outputImage;

    double imageWidth = image.width();
    double imageHeight = image.height();
    double widthScale = 0;
    double heightScale = 0;

    // Scale the image to the frame view size

    widthScale = imageWidth / frameViewWidth;
    heightScale = imageHeight / frameViewHeight;

    if (widthScale > heightScale) {
        outputImage = image.scaledToWidth(frameViewWidth);
    }
    else {
        outputImage = image.scaledToHeight(frameViewHeight);
    }

    return outputImage;
}
