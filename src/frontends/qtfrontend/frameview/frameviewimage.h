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

#ifndef FRAMEVIEWIMAGE_H
#define FRAMEVIEWIMAGE_H

#include <QTimer>
#include <QImage>
#include <QPaintEvent>
#include <QResizeEvent>

#include "domain/animation/exposure.h"
#include "frontends/qtfrontend/frameview/frameviewinterface.h"


/**
 * Implementation of the interface for the widget for viewing the frames in the
 * animation using QImage.
 *
 * Note: I'm considering redesigning the entire framework around this class, both
 * to make it more intuiative and to work with dynamic plugins for filters such
 * as onionskinning, diffing, you name it! (plugins are cool) =) However this is
 * not very important and is left for a weekend where i'm bored :p
 *
 * @author Ralf Lange
 */
class FrameViewImage : public FrameViewInterface
{
	Q_OBJECT

public:
    /**
     * Creates and initializes the frameview.
     * @param f the frontend of the applikaction.
     * @param parent the parent widget.
     * @param playbackSpeed which speed the playback has to be played in if
     * the playback mode is choosen
     */
    FrameViewImage(Frontend *f, QWidget *parent = 0, int pS = 10);

    /**
     * Cleans up after the frameview.
     */
	~FrameViewImage();

    /**
     * Initiation is compleated.
     */
    void initCompleted();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Function to receive notification when the model is erased.
     */
    void updateRemoveProject();

    /**
     * Function to recieve notification when a sound of a scene is to be played.
     * @param sceneIndex the scene to be played
     */
    void updatePlaySound(int sceneIndex);

    /**************************************************************************
     * Scene notification functions
     **************************************************************************/

    /**
     * Function to recueve notification when a scene is removed from
     * the model.
     * @param sceneIndex the scene which has been removed from the model.
     */
    void updateRemoveScene(int sceneIndex);

    /**************************************************************************
     * Take notification functions
     **************************************************************************/

    /**
     * Function to recueve notification when a take is removed from
     * the model.
     * @param sceneIndex The scene of the take which has been removed from the model.
     * @param takeIndex The take which has been removed from the model.
     */
    void updateRemoveTake(int sceneIndex,
                          int takeIndex);

    /**************************************************************************
     * Exposure notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new exposure is added to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the index of the new exposure.
     */
    void updateAddExposure(int sceneIndex,
                           int takeIndex,
                           int exposureIndex);

    /**
     * Function to recieve notification when a new exposure is inserted to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is inserted.
     * @param takeIndex the take where the new exposure is inserted.
     * @param exposureIndex the index of the new exposure.
     */
    void updateInsertExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Function to recieve notification when a new frame is selected.
     */
    void updateActivateExposure();

    /**
     * Function to recieve notification when the frame of an exposure has modifyed.
     * @param modSceneIndex The index of the scene of the modifyed exposure.
     * @param modTakeIndex The index of the take of the modifyed exposure.
     * @param modExposureIndex The index of the modifyed exposure.
     */
    void updateModifyExposure(int modSceneIndex,
                              int modTakeIndex,
                              int modExposureIndex);

    /**************************************************************************
     * Other functions
     **************************************************************************/

    /**
     * Function for performing the redraw with the next frame of the animation.
     * @param exposureIndex Index of the next frame
     */
    void nextAnimationFrame(int exposureIndex);

public slots:
    /**
     * Draws the next frame from the camera.
     */
    void redraw();

    /**
     * Function for performing playbacks. Will call redraw with regular intervals.
     */
    void nextPlayBack();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    // bool event(QEvent *event) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent *event);

private:
    static const int alphaLut[5];

    QImage           activeImage;
    QQueue<QImage>   imageBuffer;

    QTimer           grabTimer;
    QTimer           playbackTimer;
    ImageGrabThread *grabThread;

    /**
     * Loads the new frames picture into the frameview.
     * @param sceneIndex
     * @param takeIndex
     * @param exposureIndex
     */
    // void setFrame(int sceneIndex, int takeIndex, int exposureIndex);

    /**
     * Loads the new active frame picture into the frameview.
     */
    void activateExposure();

    /**
     * Loads the modifyed frame picture into the frameview.
     * @param modSceneIndex The index of the scene of the modifyed exposure.
     * @param modTakeIndex The index of the take of the modifyed exposure.
     * @param modExposureIndex The index of the modifyed exposure.
     */
    void modifyExposure(int modSceneIndex,
                        int modTakeIndex,
                        int modExposureIndex);

    /**
     * Add a image to the image buffer
     */
    void addToImageBuffer(QImage const image);

    /**
     * Load all entries in the image buffer
     */
    void loadImageBuffer();

    /**
     * Delete all entries in the image buffer
     */
    void clearImageBuffer();

    /**
     * Highly tweaked/optimized homemade function for taking the rgb differences
     * between two images.
     *
     * deltaRed = abs(r1 - r2), deltaGreen = abs(g1 - g2), deltaBlue = abs(b1 - b2)
     * for all pixels.
     *
     * @param image1 the first image of the two to differentiate.
     * @param image2 the second image of the two to differentiate.
     * @return a image with the rgb difference of image1 and image2.
     */
    const QImage createDifferentiatedImage(QImage &image1, QImage &image2);

    /**
     * Change the alpha value of the image.
     *
     * @param image the first image of the two to differentiate.
     * @return a image with the new alpha value.
     */
    const QImage createAlphaImage(QImage &image, int alpha);

    /**
     * Show the logo as active image
     */
    void showLogo();

    void paintMethod();
};

#endif
