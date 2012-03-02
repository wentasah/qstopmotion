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

#include "frameviewimage.h"

#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QtDebug>
#include <QtGui/QPainter>


const int FrameViewImage::alphaLut[5] = { 128, 64, 43, 32, 26 };

FrameViewImage::FrameViewImage(Frontend *f, QWidget *parent, int pS)
    : FrameViewInterface(f, parent, pS)
{
    qDebug("FrameViewImage::Constructor --> Start");

    frameViewWidth = 0;
    frameViewHeight = 0;

    this->setObjectName("FrameViewImage");

    this->showLogo();

    qDebug("FrameViewImage::Constructor --> End");
}


FrameViewImage::~FrameViewImage()
{
    qDebug("FrameViewImage::Destructor --> Start");

    // Turn off camera if it's on
    if (isPlayingVideo) {
        cameraOff();
    }
/*
    if (actualImage) {
        delete(actualImage);
    }
*/
    clearImageBuffer();

    qDebug("FrameViewImage::Destructor --> End");
}


void FrameViewImage::initCompleted()
{
    qDebug("FrameViewImage::initCompleted --> Start");

    clearImageBuffer();
    loadImageBuffer();

    qDebug("FrameViewImage::initCompleted --> End");
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void FrameViewImage::updateClear()
{
    qDebug("FrameViewImage::updateClear --> Start");

    this->update();

    qDebug("FrameViewImage::updateClear --> End");
}


void FrameViewImage::updatePlaySound(int /*sceneIndex*/ )
{
    qDebug("FrameViewImage::updatePlaySound --> Start");

        /*
    if (sceneIndex > -1) {
        setActiveFrame(frameNumber);
    } else {
        this->update();
    }
        */

    qDebug("FrameViewImage::updatePlaySound --> End");
}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void FrameViewImage::updateRemoveScene(int)
{
    if (frontend->getProject()->getActiveTakeIndex() < 0) {
        showLogo();
        update();
    }
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void FrameViewImage::updateRemoveTake(int,
                                      int)
{
    if (frontend->getProject()->getActiveTakeIndex() < 0) {
        showLogo();
        update();
    }
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void FrameViewImage::updateAddExposure(int sceneIndex,
                                       int takeIndex,
                                       int exposureIndex)
{
    // qDebug("FrameViewImage::updateAddExposure --> Start");

    if (isPlayingVideo) {
        Exposure *exposure = frontend->getProject()->getExposure(sceneIndex, takeIndex, exposureIndex);
        addToImageBuffer(QImage(exposure->getImagePath()));
    }

    // qDebug("FrameViewImage::updateAddExposure --> End");
}


void FrameViewImage::updateInsertExposure(int sceneIndex,
                                          int takeIndex,
                                          int exposureIndex)
{
    qDebug("FrameViewImage::updateInsertExposure --> Start");

    if (isPlayingVideo) {
        Exposure *exposure = frontend->getProject()->getExposure(sceneIndex, takeIndex, exposureIndex);
        addToImageBuffer(QImage(exposure->getImagePath()));
    }

    qDebug("FrameViewImage::updateInsertExposure --> End");
}


void FrameViewImage::updateActivateExposure()
{
    qDebug("FrameViewImage::updateActivateExposure --> Start");

    activateExposure();

    qDebug("FrameViewImage::updateActivateExposure --> End");
}


void FrameViewImage::nextAnimationFrame(int exposureIndex)
{
    // qDebug("FrameViewImage::nextAnimationFrame --> Start");

    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();

    Q_ASSERT(activeSceneIndex >= 0);
    Q_ASSERT(activeTakeIndex >= 0);

    Exposure *exposure;
    exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, exposureIndex);

    if (!exposure->isEmpty()) {
        actualImage.load(exposure->getImagePath());
        QPainter widgetPainter(this);
        widgetPainter.drawImage(0, 0, actualImage);
    }

    // Call the redraw function
    this->update();

    // qDebug("FrameViewImage::nextAnimationFrame --> End");
    return;
}

/**************************************************************************
 * Signals
 **************************************************************************/

/**************************************************************************
 * Slots
 **************************************************************************/

void FrameViewImage::redraw()
{
    // qDebug("FrameViewImage::redraw --> Start");

    actualImage = frontend->getActualImage();

    // qDebug("FrameViewImage::redraw --> Loading image finished");

    this->update();

    // qDebug("FrameViewImage::redraw --> End");
}


void FrameViewImage::nextPlayBack()
{
    qDebug("FrameViewImage::nextPlayBack --> Start");

    static int i = 0;

    // Need to check that there is an active scene before checking
    // what its size is.

    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();

    Q_ASSERT(activeSceneIndex >= 0);
    Q_ASSERT(activeTakeIndex >= 0);
    Q_ASSERT(activeExposureIndex >= 0);

    // TODO: remove activeSceneIndex-Test
    if (activeSceneIndex >= 0) {
        if ((int)i < mixCount && i < (int)activeExposureIndex + 1) {

            Exposure *exposure;
            if (activeExposureIndex <= mixCount) {
                exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, i++);
            } else {
                exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, activeExposureIndex - mixCount + i++);
            }

            if (!exposure->isEmpty()) {
                actualImage.load(exposure->getImagePath());
                QPainter widgetPainter(this);
                widgetPainter.drawImage(0, 0, actualImage);
            }

            this->update();
            //Exit from function/skip redraw(). This is better than having a bool which is
            //set because this is a play function run "often".

            qDebug("FrameViewImage::nextPlayBack --> End");
            return;
        }
    }

    // This code is run if one of the two above tests fail. Can't be an else because
    // then I would have to have two such elses, and I think the return is better.
    i = 0;
    redraw();

    qDebug("FrameViewImage::nextPlayBack --> End");
}


/**************************************************************************
 * QWidget functions
 **************************************************************************/

void FrameViewImage::resizeEvent(QResizeEvent*)
{
    qDebug("FrameViewImage::resizeEvent --> Start");

    frameViewWidth = width();
    frameViewHeight = height();
    QString iconFile(frontend->getGraphicsDirName());
    iconFile.append(QLatin1String("qstopmotion_logo_60.png"));
    actualImage.load(iconFile);

    qDebug("FrameViewImage::resizeEvent --> End");
}


void FrameViewImage::paintEvent(QPaintEvent *)
{
    // qDebug("FrameViewImage::paintEvent --> Start");

    QImage   outputImage;

    double imageWidth = actualImage.width();
    double imageHeight = actualImage.height();
    double widthScale = imageWidth / frameViewWidth;
    double heightScale = imageHeight / frameViewHeight;

    if (widthScale > heightScale) {
        outputImage = actualImage.scaledToWidth(frameViewWidth);
    }
    else {
        outputImage = actualImage.scaledToHeight(frameViewHeight);
    }
    int bufferWidth = outputImage.width();
    int bufferHeigth = outputImage.height();

    QPainter imagePainter(&outputImage);
    QPainter widgetPainter(this);
    QRect    widgetRect(this->rect());
    QSize    imageSize(outputImage.size());
    int      x = (widgetRect.width() - imageSize.width()) / 2;
    int      y = (widgetRect.height() - imageSize.height()) / 2;

    if (!actualImage.isNull()) {
        if (isPlayingVideo) {
            // Playing live video
            int offset;
            switch (mixMode) {
            case 0:
                // Image mixing

                // Set the composite mode for the images in the buffer
                imagePainter.setCompositionMode(QPainter::CompositionMode_Overlay);

                offset = imageBuffer.count() - 1;
                for (int i = 0; i < imageBuffer.count() && i < mixCount; ++i) {
                    QImage image(imageBuffer[offset - i]);
                    QImage alphaImage(this->createAlphaImage(image, alphaLut[i]));
                    imagePainter.drawImage(0, 0, alphaImage);
                }
                break;
            case 1:
                // Image differentiating

                if (imageBuffer.count() > 0) {
                    QImage image(imageBuffer.last());
                    QImage diffImage(this->createDifferentiatedImage(actualImage, image));
                    imagePainter.drawImage(0, 0, diffImage);
                }
                break;
            case 2:
                // Image Playback mode

                break;
            }
        }
        else {
            // Playing still image
            widgetPainter.fillRect(widgetRect, QColor(225, 225, 225, 255));
        }
        // Flip the screen???

    }  // End if actualImage

    widgetPainter.drawImage(x, y, outputImage);

    // qDebug("FrameViewImage::paintEvent --> End");
}


/**************************************************************************
 * Private functions
 **************************************************************************/

void FrameViewImage::activateExposure()
{
    qDebug("FrameViewImage::activateExposure --> Start");

    // TODO: set frame from active scene, active take and active exposure
    Exposure *exposure = frontend->getProject()->getActiveExposure();
    if (exposure != NULL) {
        const QString fileName = exposure->getImagePath();
        actualImage.load(fileName);
    } else {
        this->clearImageBuffer();
        this->showLogo();
    }
    this->update();

    qDebug("FrameViewImage::activateExposure --> End");
}

void FrameViewImage::addToImageBuffer(QImage const image)
{
    qDebug("FrameViewImage::addToImageBuffer --> Start");

    double imageWidth = image.width();
    double imageHeight = image.height();
    double widthScale = imageWidth / frameViewWidth;
    double heightScale = imageHeight / frameViewHeight;

    while (imageBuffer.count() >= this->mixCount) {
        imageBuffer.removeFirst();
    }

    if (widthScale > heightScale) {
        imageBuffer.append(image.scaledToWidth(frameViewWidth));
    }
    else {
        imageBuffer.append(image.scaledToHeight(frameViewHeight));
    }
    int bufferWidth = imageBuffer.last().width();
    int bufferHeigth = imageBuffer.last().height();

    int widgetWidth = width();
    int widgetHeigth = height();

    qDebug("FrameViewImage::addToImageBuffer --> End");
}


void FrameViewImage::loadImageBuffer()
{
    qDebug("FrameViewImage::loadImageBuffer --> Start");

    int activeExposure = frontend->getProject()->getActiveExposureIndex();
    int exposureIndex = activeExposure - this->mixCount + 1;

    if (exposureIndex < 0) {
        exposureIndex = 0;
    }

    for ( ; exposureIndex <= activeExposure ; exposureIndex++ ) {
        Exposure *exposure = frontend->getProject()->getExposure(frontend->getProject()->getActiveSceneIndex(),
                                                                 frontend->getProject()->getActiveTakeIndex(),
                                                                 exposureIndex);
        addToImageBuffer(QImage(exposure->getImagePath()));
    }

    qDebug("FrameViewImage::loadImageBuffer --> End");
}


void FrameViewImage::clearImageBuffer()
{
    qDebug("FrameViewImage::clearImageBuffer --> Start");

    imageBuffer.clear();

    qDebug("FrameViewImage::clearImageBuffer --> End");
}


const QImage FrameViewImage::createDifferentiatedImage(QImage &image1, QImage &image2)
{
    qDebug("FrameViewImage::createDifferentiatedImage --> Start");

    int imageRows = image2.height();
    int imageCols = image2.width();
    QImage diffImage(imageCols, imageRows, QImage::Format_ARGB32);
    QColor diffColor;
    QColor image1Color;
    QColor image2Color;

    // Goes through the surfaces as one-dimensional arrays.
    for (int r = 0; r < imageRows; ++r) {
        for (int c = 0; c < imageCols; ++c) {
            image1Color = image1.pixel(c, r);
            image2Color = image2.pixel(c, r);
            diffColor.setRed(abs(image1Color.red() - image2Color.red()));
            diffColor.setGreen(abs(image1Color.green() - image2Color.green()));
            diffColor.setBlue(abs(image1Color.blue() - image2Color.blue()));
            diffColor.setAlpha(image1Color.alpha());
            diffImage.setPixel(c, r, diffColor.rgba());
        }
    }

    qDebug("FrameViewImage::createDifferentiatedImage --> End");

    return diffImage;
}


const QImage FrameViewImage::createAlphaImage(QImage &image, int alpha)
{
    // qDebug("FrameViewImage::createAlphaImage --> Start");

    int imageRows = image.height();
    int imageCols = image.width();
    // QImage alphaImage(imageCols, imageRows, QImage::Format_ARGB32);
    QImage alphaImage(image);
    QColor alphaColor;

    // Goes through the surfaces as one-dimensional arrays.
    for (int r = 0; r < imageRows; ++r) {
        for (int c = 0; c < imageCols; ++c) {
            alphaColor = alphaImage.pixel(c, r);
            alphaColor.setAlpha(alpha);
            alphaImage.setPixel(c, r, alphaColor.rgba());
        }
    }

    // qDebug("FrameViewImage::createAlphaImage --> End");

    return alphaImage;
}


void FrameViewImage::showLogo()
{
    qDebug("FrameViewImage::showLogo --> Start");

    // actualImage = new QImage(width(), height(), QImage::Format_ARGB32);
    QString iconFile(frontend->getGraphicsDirName());
    iconFile.append(QLatin1String("qstopmotion_logo_60.png"));

    actualImage.load(iconFile);

    qDebug("FrameViewImage::showLogo --> End");
}
