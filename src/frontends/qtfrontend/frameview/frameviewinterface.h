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

#ifndef FRAMEVIEWINTERFACE_H
#define FRAMEVIEWINTERFACE_H

#include "domain/animation/exposure.h"
#include "frontends/observer.h"
#include "frontends/frontend.h"

#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtGui/QWidget>

class ImageGrabThread;

/**
 * Interface for the frame view functionality of the widget for viewing the
 * frames in the animation.
 *
 * Note: I'm considering redesigning the entire framework around this class, both
 * to make it more intuiative and to work with dynamic plugins for filters such
 * as onionskinning, diffing, you name it! (plugins are cool) =) However this is
 * not very important and is left for a weekend where i'm bored :p
 *
 * @author Ralf Lange
 */
class FrameViewInterface : public QWidget, public Observer
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
    FrameViewInterface(Frontend *f, QWidget *parent = 0, int pS = 10);

	/**
     * Cleans up after the frameview.
     */
    virtual ~FrameViewInterface();

    /**
     * Get the frontend.
     * @return the frontend.
     */
    Frontend* getFrontend();

    /**
     * Set the frontend.
     * @param f the frontend.
     *
    void setFrontend(Frontend* f);
*/
    /**
     *
     */
    virtual void initCompleted() = 0;

    /**
     * Sets the view to 4:3 format.
     */
    void setWidescreenRatio();

    /**
     * Sets the view to 16:9 format.
     */
    void setNormalRatio();

    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Function to receive notification when the model is erased.
     */
    virtual void updateRemoveProject() = 0;

    /**
     * Function to receive notification when a new Project is created.
     */
    void updateNewProject();

    /**
     * Function for receiving notification when the descriptions of the
     * objects are changed.
     */
    virtual void updateDescriptionsUpdated();

    /**
     * Function to recieve notification when a sound of a scene is to be played.
     * @param sceneIndex the scene to be played
     */
    virtual void updatePlaySound(int sceneIndex) = 0;

    /**
     * Function to recieve notification when new mix mode is set.
     * @param newMixMode the new mix mode
     */
    virtual void updateMixMode(int newMixMode);

    /**
     * Function to recieve notification when new mix count is set.
     * @param newMixCount the new mix count
     */
    virtual void updateMixCount(int newMixCount);

    /**
     * Function to recieve notification when new frames per second is set.
     * @param newFps the new frames per second
     */
    virtual void updateFramesPerSecond(int newFps);

    /**************************************************************************
     * Scene notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new scene is added to the
     * model.
     * @param sceneIndex the index of the new scene.
     */
    void updateAddScene(int sceneIndex);

    /**
     * Function to recieve notification when a new scene is inserted to the
     * model.
     * @param sceneIndex the index of the new scene.
     */
    void updateInsertScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene is selected as the
     * active scene in the animationmodel.
     * @param sceneIndex the new active scene.
     * opening the new active scene.
     */
    // void updateSetNewActiveScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene is selected as the
     * active scene in the animationmodel.
     */
    void updateActivateScene();

    /**
     * Function to recueve notification when a scene is removed from
     * the model.
     * @param sceneIndex the scene which has been removed from the model.
     */
    virtual void updateRemoveScene(int sceneIndex) = 0;

    /**
     * Function which recieve notification when a scene in the animation
     * has been moved.
     * @param sceneNumber the scene which have been moved.
     * @param movePosition the position the scene has been moved to.
     */
    virtual void updateMoveScene(int sceneNumber, int movePosition);

    /**************************************************************************
     * Take notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new take is added to the
     * model.
     * @param sceneIndex the scene where the new take is added.
     * @param takeIndex the index of the new take.
     */
    void updateAddTake(int sceneIndex, int takeIndex);

    /**
     * Function to recieve notification when a new take is inserted to the
     * model.
     * @param sceneIndex the scene where the new take is inserted.
     * @param takeIndex the index of the new take.
     */
    void updateInsertTake(int sceneIndex, int takeIndex);

    /**
     * Function which recieves notification when a take is selected as the
     * active take in the animationmodel.
     * @param takeIndex the new active take.
     * opening the new active take.
     */
    // void updateSetNewActiveTake(int takeIndex);

    /**
     * Function which recieves notification when a take is selected as the
     * active take in the animationmodel.
     */
    void updateActivateTake();

    /**
     * Function to recueve notification when a take is removed from
     * the model.
     * @param sceneIndex The scene of the take which has been removed from the model.
     * @param takeIndex The take which has been removed from the model.
     */
    virtual void updateRemoveTake(int sceneIndex, int takeIndex) = 0;

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
    void updateAddExposure(int sceneIndex, int takeIndex, int exposureIndex) = 0;

    /**
     * Function to recieve notification when a new exposure is inserted to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the index of the new exposure.
     */
    void updateInsertExposure(int sceneIndex, int takeIndex, int exposureIndex) = 0;

    /**
     * Function to recieve notification when a new exposure is selected.
     */
    virtual void updateActivateExposure() = 0;

    /**
     * Function to recieve notification when one exposure are deleted.
     * @param sceneIndex the scene index of the selected exposure.
     * @param takeIndex the take index of the selected exposure.
     * @param exposureIndex the index of the removed exposure.
     */
    void updateRemoveExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Function to recieve notification when one or more frames are deleted.
     */
    void updateRemoveExposures(int, int);

    /**
     * Function to recieve notification when one or more frames are moved.
     *
     */
    void updateMoveExposures(int fromFrame, int toFrame, int movePosition);

    /**************************************************************************
     * Frameview functions
     **************************************************************************/

    /**
     * Turns on the webcamera/video import mode.
     */
    bool cameraOn();

    /**
     * Turns off the webcamera/video import mode.
     */
    void cameraOff();

    /**
     * Returns the mix mode.
     * @return the mix mode.
     */
    int getMixMode() const;

    /**
     * Sets the mix mode/type of effect used when displaying the video.
     * @param mode the type of effect to be showed on the video. The modes are:\n
     *             0: Image mixing/onion skinning\n
     *             1: Image differentiating\n
     *             2: Playback\n
     * @return true if the mode was succesfully changed
     */
    bool setMixMode(int mode);

    /**
     *
     */
    void setMixCount(int mixCount);

    /**
     * Sets the speed for the playback.
     * @param playbackSpeed the speed to be setted
     */
    void setFps(int fps);

    /**
     * Function for performing the redraw with the next frame of the animation.
     * @param exposureIndex Index of the next frame
     */
    virtual void nextAnimationFrame(int exposureIndex) = 0;

public slots:
    /**
     * Draws the next frame from the camera.
     */
    virtual void redraw() = 0;

    /**
     * Function for performing playbacks. Will call redraw with regular intervals.
     */
    virtual void nextPlayBack() = 0;

protected:
    /**
     * Activate a new scene.
     */
    virtual void activateScene();

    /**
     * Activate a new take.
     */
    virtual void activateTake();

    /**
     * Loads the new frames picture into the frameview.
     * @param sceneIndex
     * @param takeIndex
     * @param exposureIndex
     */
    // virtual void setFrame(int sceneIndex, int takeIndex, int exposureIndex) = 0;

    /**
     * Loads the new active frames picture into the frameview.
     */
    virtual void activateExposure() = 0;

    /**
     * Delete all entries in the image buffer
     */
    virtual void clearImageBuffer() = 0;

    /**
     * Frontend of the application
     */
    Frontend *frontend;

    /**
     *
     */
    QTimer grabTimer;

    /**
     *
     */
    QTimer playbackTimer;

    /**
     *
     */
    int framesPerSecond;

    /**
     *
     */
    bool isPlayingVideo;

    /**
     *
     */
    int widthConst;

    /**
     *
     */
    int heightConst;

    /**
     * Number of images to display in mix mode.
     */
    int mixCount;

    /**
     * Mode to viewing the images.
     */
    int mixMode;

};

#endif
