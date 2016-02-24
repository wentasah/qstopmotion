/******************************************************************************
 *  Copyright (C) 2010-2016 by                                                *
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

#include <QDebug>
#include <QPainter>
#include <QRect>
#include <QSize>


const int FrameViewImage::alphaLut[5] = { 128, 64, 43, 32, 26 };


FrameViewImage::FrameViewImage(Frontend *f, QWidget *parent, int pS)
    : FrameViewInterface(f, parent, pS)
{
    qDebug() << "FrameViewImage::Constructor --> Start";

    this->setObjectName("FrameViewImage");

    // setAttribute(Qt::WA_PaintOnScreen);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->showLogo();

    qDebug() << "FrameViewImage::Constructor --> End";
}


FrameViewImage::~FrameViewImage()
{
    qDebug() << "FrameViewImage::Destructor --> Start";

    // Turn off camera if it's on
    if (displayMode == liveImageMode) {
        cameraOff();
    }

    clearImageBuffer();

    qDebug() << "FrameViewImage::Destructor --> End";
}


void FrameViewImage::initCompleted()
{
    qDebug() << "FrameViewImage::initCompleted --> Start";

    // Fill the image buffer
    clearImageBuffer();
    loadImageBuffer();

    // Clear the display
    displayMode = emptyMode;
    repaint();

    qDebug() << "FrameViewImage::initCompleted --> End";
}


QSize FrameViewImage::sizeHint() const
{
    return QSize(400, 200);
}


QSize FrameViewImage::minimumSizeHint() const
{
    return QSize(100, 100);
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void FrameViewImage::updateRemoveProject()
{
    qDebug() << "FrameViewImage::updateRemoveProject --> Start";

    showLogo();

    qDebug() << "FrameViewImage::updateRemoveProject --> End";
}


void FrameViewImage::updatePlaySound(int /*sceneIndex*/ )
{
    qDebug() << "FrameViewImage::updatePlaySound --> Start";

        /*
    if (sceneIndex > -1) {
        setActiveFrame(frameNumber);
    } else {
        this->update();
    }
        */

    qDebug() << "FrameViewImage::updatePlaySound --> End";
}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void FrameViewImage::updateRemoveScene(int)
{
    if (frontend->getProject()->getActiveSceneIndex() < 0) {
        showLogo();
        return;
    }
    if (frontend->getProject()->getActiveTakeIndex() < 0) {
        showLogo();
    }
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void FrameViewImage::updateRemoveTake(int,
                                      int)
{
    if (frontend->getProject()->getActiveSceneIndex() < 0) {
        showLogo();
        return;
    }
    if (frontend->getProject()->getActiveTakeIndex() < 0) {
        showLogo();
    }
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void FrameViewImage::updateAddExposure(int sceneIndex,
                                       int takeIndex,
                                       int exposureIndex)
{
    // qDebug() << "FrameViewImage::updateAddExposure --> Start";

    if (displayMode == liveImageMode) {
        Exposure *exposure = frontend->getProject()->getExposure(sceneIndex, takeIndex, exposureIndex);
        addToImageBuffer(QImage(exposure->getImagePath()));
    }

    // qDebug() << "FrameViewImage::updateAddExposure --> End";
}


void FrameViewImage::updateInsertExposure(int sceneIndex,
                                          int takeIndex,
                                          int exposureIndex)
{
    qDebug() << "FrameViewImage::updateInsertExposure --> Start";

    if (displayMode == liveImageMode) {
        Exposure *exposure = frontend->getProject()->getExposure(sceneIndex, takeIndex, exposureIndex);
        addToImageBuffer(QImage(exposure->getImagePath()));
    }

    qDebug() << "FrameViewImage::updateInsertExposure --> End";
}


void FrameViewImage::updateActivateExposure()
{
    qDebug() << "FrameViewImage::updateActivateExposure --> Start";

    activateExposure();

    qDebug() << "FrameViewImage::updateActivateExposure --> End";
}


void FrameViewImage::updateModifyExposure(int modSceneIndex,
                                          int modTakeIndex,
                                          int modExposureIndex)
{
    qDebug() << "FrameViewImage::updateModifyExposure --> Start";

    modifyExposure(modSceneIndex, modTakeIndex, modExposureIndex);

    qDebug() << "FrameViewImage::updateModifyExposure --> End";
}


/**************************************************************************
 * Other functions
 **************************************************************************/

void FrameViewImage::nextAnimationFrame(int exposureIndex)
{
    // qDebug() << "FrameViewImage::nextAnimationFrame --> Start");

    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();

    Q_ASSERT(activeSceneIndex >= 0);
    Q_ASSERT(activeTakeIndex >= 0);

    Exposure *exposure;
    exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, exposureIndex);

    if (!exposure->isEmpty()) {
        QImage nextImage;
        nextImage.load(exposure->getImagePath());
        activeImage = scaleImage(nextImage);
        QPainter widgetPainter(this);
        widgetPainter.drawImage(0, 0, scaleImage(activeImage));
    }

    // Call the redraw function
    this->update();

    // qDebug() << "FrameViewImage::nextAnimationFrame --> End");
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
    // qDebug() << "FrameViewImage::redraw --> Start";

    const QImage liveImage = frontend->getLiveImage();
    if (liveImage.isNull()) {
        qDebug() << "FrameViewImage::redraw --> End (empty live image)";
        return;
    }

    activeImage = clipImage(liveImage);

    update();

    // qDebug() << "FrameViewImage::redraw --> End";
}


void FrameViewImage::nextPlayBack()
{
    qDebug() << "FrameViewImage::nextPlayBack --> Start";

    static int i = 0;

    // Need to check that there is an active scene before checking
    // what its size is.

    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();

    Q_ASSERT(activeSceneIndex >= 0);
    Q_ASSERT(activeTakeIndex >= 0);
    Q_ASSERT(activeExposureIndex >= 0);

    if ((int)i < mixCount && i < (int)activeExposureIndex + 1) {

        Exposure *exposure;
        if (activeExposureIndex <= mixCount) {
            exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, i++);
        } else {
            exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, activeExposureIndex - mixCount + i++);
        }

        if (!exposure->isEmpty()) {
            activeImage.load(exposure->getImagePath());
            QPainter widgetPainter(this);
            widgetPainter.drawImage(0, 0, scaleImage(activeImage));
        }

        this->update();
        //Exit from function/skip redraw(). This is better than having a bool which is
        //set because this is a play function run "often".

        qDebug() << "FrameViewImage::nextPlayBack --> End";
        return;
    }

    // This code is run if one of the two above tests fail. Can't be an else because
    // then I would have to have two such elses, and I think the return is better.
    i = 0;
    redraw();

    qDebug() << "FrameViewImage::nextPlayBack --> End";
}


/**************************************************************************
 * QWidget functions
 **************************************************************************/

void FrameViewImage::resizeEvent(QResizeEvent*)
{
    qDebug() << "FrameViewImage::resizeEvent --> Start";

    frameViewWidth = width();
    frameViewHeight = height();
    if (activeImage.isNull()) {
        showLogo();
    }

    qDebug() << "FrameViewImage::resizeEvent --> End";
}


void FrameViewImage::paintEvent(QPaintEvent *)
{
    qDebug() << "FrameViewImage::paintEvent --> Start";

    // QImage   outputImage = activeImage;

    QPainter widgetPainter(this);
    QColor   gridColor;
    QRect    widgetRect(this->rect());

    widgetPainter.setRenderHint(QPainter::Antialiasing, true);

    if (displayMode == liveImageMode) {
        // Playing live video

        QPainter imagePainter(&activeImage);
        int offset;

        switch (mixMode) {
        case 0:
            // Image mixing

            // Set the composite mode for the images in the buffer
            imagePainter.setCompositionMode(QPainter::CompositionMode_Overlay);

            offset = imageBuffer.count() - 1;
            for (int i = 0; i < imageBuffer.count() && i < mixCount; ++i) {
                QImage image(imageBuffer[offset - i]);
                // QImage alphaImage(this->createAlphaImage(image, alphaLut[i]));
                QImage alphaImage(this->createGrayAlphaImage(image, alphaLut[i]));
                imagePainter.drawImage(0, 0, scaleImage(alphaImage));
            }
            break;
        case 1:
            // Image differentiating

            if (imageBuffer.count() > 0) {
                QImage image(imageBuffer.last());
                QImage diffImage(this->createDifferentiatedImage(activeImage, image));
                imagePainter.eraseRect(activeImage.rect());
                imagePainter.drawImage(0, 0, scaleImage(diffImage));
            }
            break;
        }
    }

    switch (displayMode) {
    case emptyMode:
        // Display nothing
        widgetPainter.fillRect(widgetRect, QColor(225, 225, 225, 255));
        break;
    case logoMode:
        // Fill the start screen
        widgetPainter.fillRect(widgetRect, QColor(225, 225, 225, 255));
    default:
        // Display the actual image
        QImage   outputImage = scaleImage(activeImage);
        QSize    outputImageSize(outputImage.size());
        int x = (widgetRect.width() - outputImageSize.width()) / 2;
        int y = (widgetRect.height() - outputImageSize.height()) / 2;
        widgetPainter.drawImage(x, y, outputImage);
    }

    switch (displayMode) {
    case liveImageMode:
    case playbackMode:
        int r, g, b, a;
        frontend->getGridColorRGB(&r, &g, &b, &a);
        gridColor.setRgb(r, g, b, a);
        if (frontend->getVerticalGrid()) {
            // Draw a vertical grid on top of the image

            int verticalLines = frontend->getVerticalSpin();
            int vericalDistance = widgetRect.width() / (verticalLines + 1);
            int x1, y1, x2, y2;

            x1 = y1 = 0;
            y2 = widgetRect.height();

            widgetPainter.setPen(gridColor);

            for (int v = 0 ; v < verticalLines ; v++) {
                x1 += vericalDistance;
                x2 = x1;
                widgetPainter.drawLine(x1, y1, x2, y2);
            }
        }

        if (frontend->getHorizontalGrid()) {
            // Draw a horizontal grid on top of the image

            int horizontalLines = frontend->getHorizontalSpin();
            int horizontalDistance = widgetRect.height() / (horizontalLines + 1);
            int x1, y1, x2, y2;

            x1 = y1 = 0;
            x2 = widgetRect.width();

            widgetPainter.setPen(gridColor);

            for (int h = 0 ; h < horizontalLines ; h++) {
                y1 += horizontalDistance;
                y2 = y1;
                widgetPainter.drawLine(x1, y1, x2, y2);
            }
        }

        break;
    }

    widgetPainter.setRenderHint(QPainter::Antialiasing, false);

    // qDebug() << "FrameViewImage::paintEvent --> End";
}


/**************************************************************************
 * Private functions
 **************************************************************************/

void FrameViewImage::activateExposure()
{
    qDebug() << "FrameViewImage::activateExposure --> Start";

    Exposure *exposure = NULL;

    switch (displayMode) {
    case logoMode:
        displayMode = stillImageMode;
    case stillImageMode:
        exposure = frontend->getProject()->getActiveExposure();

        // Q_ASSERT(exposure != NULL);

        if (exposure != NULL) {
            const QString fileName = exposure->getImagePath();
            activeImage = QImage(fileName);
        } else {
            showLogo();
        }
        update();
        break;
    case liveImageMode:
        break;
    case playbackMode:
        break;
    }

    qDebug() << "FrameViewImage::activateExposure --> End";
}


void FrameViewImage::modifyExposure(int modSceneIndex,
                                    int modTakeIndex,
                                    int modExposureIndex)
{
    qDebug() << "FrameViewImage::modifyExposure --> Start";

    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeSceneIndex != modSceneIndex) {
        // The scene of the modifyed exposure is not displayed in the frame view
        qDebug() << "FrameViewImage::modifyExposure --> End (Nothing)";
        return;
    }

    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    if (activeTakeIndex != modTakeIndex) {
        // The take of the modifyed exposure is not displayed in the frame view
        qDebug() << "FrameViewImage::modifyExposure --> End (Nothing)";
        return;
    }

    int activeExposureIndex = frontend->getProject()->getActiveSceneIndex();
    if (activeExposureIndex != modExposureIndex) {
        // The modifyed exposure is not displayed in the frame view
        qDebug() << "FrameViewImage::modifyExposure --> End (Nothing)";
        return;
    }

    Exposure *exposure = frontend->getProject()->getActiveExposure();
    if (exposure != NULL) {
        const QString fileName = exposure->getImagePath();
        activeImage.load(fileName);
    } else {
        this->clearImageBuffer();
        this->showLogo();
    }
    this->update();

    qDebug() << "FrameViewImage::modifyExposure --> End";
}


void FrameViewImage::addToImageBuffer(QImage const image)
{
    qDebug() << "FrameViewImage::addToImageBuffer --> Start";

    imageBuffer.append(image);

    qDebug() << "FrameViewImage::addToImageBuffer --> End";
}


void FrameViewImage::loadImageBuffer()
{
    qDebug() << "FrameViewImage::loadImageBuffer --> Start";

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

    qDebug() << "FrameViewImage::loadImageBuffer --> End";
}


void FrameViewImage::clearImageBuffer()
{
    qDebug() << "FrameViewImage::clearImageBuffer --> Start";

    imageBuffer.clear();

    qDebug() << "FrameViewImage::clearImageBuffer --> End";
}


const QImage FrameViewImage::createDifferentiatedImage(QImage &image1, QImage &image2)
{
    qDebug() << "FrameViewImage::createDifferentiatedImage --> Start";

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

    qDebug() << "FrameViewImage::createDifferentiatedImage --> End";

    return diffImage;
}


const QImage FrameViewImage::createAlphaImage(QImage &image, int alpha)
{
    // qDebug() << "FrameViewImage::createAlphaImage --> Start";

    int imageRows = image.height();
    int imageCols = image.width();
    QImage alphaImage(image);
    QColor alphaColor;

    // We assume the format to be RGB32!!!
    Q_ASSERT(image.format() == QImage::Format_RGB32);

    // Goes through the surfaces as one-dimensional arrays.
    for (int r = 0; r < imageRows; ++r) {
        for (int c = 0; c < imageCols; ++c) {
            alphaColor = alphaImage.pixel(c, r);
            alphaColor.setAlpha(alpha);
            alphaImage.setPixel(c, r, alphaColor.rgba());
        }
    }

    // qDebug() << "FrameViewImage::createAlphaImage --> End";

    return alphaImage;
}


const QImage FrameViewImage::createGrayAlphaImage(QImage &image, int alpha)
{
    // qDebug() << "FrameViewImage::createGrayAlphaImage --> Start";

    int imageRows = image.height();
    int imageCols = image.width();

    // We assume the format to be RGB32!!!
    Q_ASSERT(image.format() == QImage::Format_RGB32);

    // Goes through the surfaces as one-dimensional arrays.
    for (int ii = 0; ii < imageRows; ii++) {
        QRgb *pixel = reinterpret_cast<QRgb*>(image.scanLine(ii));
        QRgb *end = pixel + imageCols;
        for (; pixel != end; pixel++) {
            int gray = qGray(*pixel);
            *pixel = QColor(QColor(gray, gray, gray, qAlpha(*pixel)).lighter(200)).rgb();
        }
    }

    // qDebug() << "FrameViewImage::createGrayAlphaImage --> End";

    return image;
}


void FrameViewImage::showLogo()
{
    qDebug() << "FrameViewImage::showLogo --> Start";

    QString iconFile(frontend->getGraphicsDirName());
    iconFile.append(QLatin1String("qstopmotion_logo_60.png"));

    activeImage = scaleImage(QImage(iconFile));

    displayMode = logoMode;
    update();

    qDebug() << "FrameViewImage::showLogo --> End";
}
