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

#ifndef TIMELINE_H
#define TIMELINE_H

#include "frontends/observer.h"
// #include "domain/animation/animationproject.h"
#include "domain/animation/exposure.h"
#include "frontends/frontend.h"
#include "frontends/qtfrontend/timeline/thumbview.h"

#include <QtGui/QDragEnterEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QScrollArea>
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>
#include <QtCore/QTimer>

class ThumbView;

/**
 * The timeline widget for viewing the animation model.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class TimeLine : public QScrollArea, public Observer
{
    Q_OBJECT
public:
    /**
     * Creates and sets up the timeline.
     * @param parent the parent widget
     */
    TimeLine(Frontend* f, QWidget *parent = 0);

    /**
     * Cleans up after the timeline.
     */
    ~TimeLine();

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
    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Updates the timeline when the active scene, take or exposure are changed.
     */
    // void updateAnimationChanged();

    /**
     * Function to receive notification when the model is erased.
     */
    void updateClear();

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
    void updatePlaySound(int sceneIndex);

    /**
     * Function to recieve notification when new mixing mode is set.
     * @param newMixingMode the new mixing mode
     */
    void updateMixingMode(int newMixingMode);

    /**
     * Function to recieve notification when new mix count is set.
     * @param newMixCount the new mix count
     */
    void updateMixCount(int newMixCount);

    /**
     * Function to recieve notification when new frames per second is set.
     * @param newFps the new frames per second
     */
    void updateFramesPerSecond(int newFps);

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
     * @param sceneIndex the new scene to be set as the active scene in the
     * timeline.
     * opening the new active scene.
     */
    // void updateSetNewActiveScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene is selected as the
     * active scene in the animationmodel.
     */
    void updateActivateScene();

    /**
     * Function to recieve notification when a scene is removed from
     * the model.
     * @param sceneIndex the scene which has been removed from the model.
     */
    void updateRemoveScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene in the animation
     * has been moved and moves the icons in the timeline accordingly.
     * @param sceneNumber the scene which have been moved.
     * @param movePosition the position the scene has been moved to.
     */
    void updateMoveScene(int sceneNumber, int movePosition);

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
     * @param sceneIndex the scene where the new take is added.
     * @param takeIndex the index of the new take.
     */
    void updateInsertTake(int sceneIndex, int takeIndex);

    /**
     * Function which recieves notification when a take is selected as the
     * active take in the animationmodel.
     * @param takeIndex the new scene to be set as the active take in the
     * timeline.
     * opening the new active take.
     */
    // void updateSetNewActiveTake(int takeIndex);

    /**
     * Function which recieves notification when a take is selected as the
     * active take in the animationmodel.
     */
    void updateActivateTake();

    /**
     * Function to recieve notification when a take is removed from
     * the model.
     * @param sceneIndex the scene of the take which has been removed from the model.
     * @param takeIndex the take which has been removed from the model.
     */
    void updateRemoveTake(int sceneIndex, int takeIndex);

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
    void updateAddExposure(int sceneIndex, int takeIndex, int exposureIndex);

    /**
     * Function to recieve notification when a new exposure is inserted to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the index of the new exposure.
     */
    void updateInsertExposure(int sceneIndex, int takeIndex, int exposureIndex);

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
     * @param fromFrame the first frame in the selection
     * @param toFrame the last frame in the selection
     */
    void updateRemoveExposures(int fromFrame, int toFrame);

    /**
     * Function to recieve notification when one or more frames are moved.
     */
    void updateMoveExposures(int fromFrame, int toFrame, int movePosition);

    /**
     * Function to recieve notification when a new frame is selected.
     */
    // void updateSetNewActiveExposure(int exposureIndex);

    /**
     * Function to recieve notification when a new frame is selected.
     */
    void updateActivateExposure();

    /**
     * Function to recieve notification when a new frame is selected.
     */
    // void updateFrame(int sceneIndex, int takeIndex, int exposureIndex);

    /**************************************************************************
     * Other functions
     **************************************************************************/

    /**
     * Set whether the user is engaged selecting several frames or not (pressing
     * shift).
     * @param selecting true if the user is currently selecting multiple pictures
     */
    void setSelecting(bool selecting);

    /**
     * Returns true if the user is currently selecting several thumbviews.
     * @return true if the user is currently selecting several thumbviews.
     */
    bool isSelecting() const;

    /**
     * Sets a selection of frames between (including) this frame and the activeFrame.
     * @todo change name to setSelectionFrame
     * @param selectionFrame the other border frame of the selection in addition to activeFrame
     */
    void setSelection(int selectionFrame);

    /**
     * Returns the current selectionFrame
     * @return the current selectionFrame
     */
    int getSelectionFrame() const;

    /**
     * Retrieves the value of the movingScene property specifying which scene
     * is currently being moved in the timeline.
     * @return the value of the movingScene property.
     */
    int getMovingScene() const;

    /**
     * Sets the value of the movingScene property specifying which scene
     * is currently being moved in the timeline.
     * @param movingScene the new value of the movingScene property.
     */
    void setMovingScene(int movingScene);

    /**
     * Sets whether the scene is currently opening so that close requests
     * can be ignored while it is processing.
     *
     * @param openingScene true if the scene is currently being opened. False
     * if not.
     */
    void setOpeningScene(bool openingScene);

    /**
     * Returns true if a scene is currently being opened.
     * @return true if a scene is currently being opened.
     */
    bool isOpeningScene() const;

    int getFrameWidth() const;
    int getFrameHeight() const;
    int getSpace() const;

protected:
    /**
     * Overloaded event function for when a drag enter occurs in the timeline
     * @param event information about the dragEnterEvent
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /**
     * Overloaded event function for when a drop event occurs in the timeline.
     * @param event information about the dropEvent
     */
    void dropEvent(QDropEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void resizeEvent(QResizeEvent *event);

public slots:
    /**
     * Recieves notification when the sounds in a frame has been changed.
     */
    void frameSoundsChanged();

private slots:
    void scroll();

private:

    /**
     * Frontend of the application
     */
    Frontend *frontend;

    static const int FRAME_HEIGHT = 88;
    static const int FRAME_WIDTH = 117;
    static const int SPACE = 2;

    /** Vector of thumbviews to keep track of the pictures in the timeline*/
    QVector<ThumbView*>thumbViews;

    /** Index of the active scene in the timeline */
    int activeSceneIndex;

    /** Index of the active take in the timeline */
    int activeTakeIndex;

    /** Index of the active exposure in the timeline*/
    int activeExposureIndex;

    /** The scene which are being moved when draging a scene */
    int movingScene;

    /** The other border frame in a selection together with activeFrame.
    *   If only one is selected selectionFrame == activeFrame            */
    int selectionFrame;

    /** Direction to scroll when dragging.
     * -1 = scroll negative, 0 = no scroll, 1 = scroll positive */
    int scrollDirection;

    int lowerScrollAreaX;
    int upperScrollAreaX;

    int lowerAccelScrollAreaX;
    int upperAccelScrollAreaX;

    int minScrollAreaX;
    int maxScrollAreaX;

    int minScrollAreaY;
    int maxScrollAreaY;

    /**
     * True if the user is currently holding down shift to select multiple frames
     */
    bool selecting;

    /**
     * ????
     */
    bool openingScene;

    QTimer *scrollTimer;
    QScrollBar *scrollBar;

    QWidget *mainWidget;

    /**************************************************************************
     * Private animation functions
     **************************************************************************/

    /**
     * Delete all thumviews from the time line
     */
    void clear();

    /**************************************************************************
     * Private scene functions
     **************************************************************************/

    /**
     * Creates a new scene and adds it to the timeline.
     * @param sceneIndex the location the new scene should be added at.
     */
    void newScene(int sceneIndex);

    /**
     * Removes the scene with index sceneIndex from the timeline.
     * @param sceneIndex The scene to be removed from the timeline.
     */
    void removeScene(int sceneIndex);

    /**
     * Moves a scene in the timeline.
     * @param sceneNumber the scene that are moved.
     * @param movePosition the position the scene are moved to.
     */
    void moveScene(int sceneNumber, int movePosition);

    /**
     * Sets a new scene as the active scene in the timeline and opens it.
     * @param sceneNumber the scene to set as the active scene.
     * frames to the scene.
     */
    // void setNewActiveScene(int sceneNumber);

    /**
     * Sets a new scene as the active scene in the timeline and opens it.
     */
    void activateScene();

    /**************************************************************************
     * Private take functions
     **************************************************************************/

    /**
     * Creates a new take and adds it to the timeline.
     * @param sceneIndex the scene the new take should be added at.
     * @param takeIndex the location the new take should be added at.
     */
    void newTake(int sceneIndex,
                 int takeIndex);

    /**
     * Sets a new take as the active take in the timeline and opens it.
     */
    void activateTake();

    /**
     * Delete all thumviews of the take from the time line
     */
    void clearTake();

    /**
     * Removes the take with index takeIndex from the timeline.
     * @param sceneIndex The scene of the take to be removed from the timeline.
     * @param takeIndex The take to be removed from the timeline.
     */
    void removeTake(int sceneIndex, int takeIndex);

    /**************************************************************************
     * Private exposure functions
     **************************************************************************/

    /**
     * Creates a new exposure and adds it to the timeline.
     * @param sceneIndex the scene of the take the new exposure should be added at.
     * @param takeIndex the take the new exposure should be added at.
     * @param exposureIndex the location the new exposure should be added at.
     */
    void newExposure(int sceneIndex,
                     int takeIndex,
                     int exposureIndex);

    /**
     *Adds the picture to the frame to the timeline at position index.
     *
     *@param frame the path of the frame to be added to the timeline.
     *@param index the place to add the frame.
     */
    void addExposure(Exposure* frame, int index);

    /**
     * Adds the frames in the framesvector to the timeline.
     * @param frames the frames to be added to the timeline.
     * @param index the location to add the frames to.calc
     * progressbar.
     */
    void addExposures(const QVector<Exposure*>& frames, int index);

    /**
     * Removes a exposure from the timeline.
     * @param sceneIndex Index of the scene of the exposure to remove.
     * @param takeIndex Index of the take of the exposure to remove.
     * @param exposureIndex Index of the exposure to remove.
     */
    void removeExposure(int sceneIndex,
                        int takeIndex,
                        int exposureIndex);

    /**
     * Removes a selection of thumbviews from the timeline.
     * @param fromFrame the first thumbview to remove.
     * @param toFrame the last thumbview to remove.
     */
    void removeExposures(int fromFrame, int toFrame);

    /**
     * Removes all thumbviews from the timeline.
     */
    void removeAllExposures();

    /**
     * Move the frames from fromFrame (inclusive) to toFrame to movePosition.
     * @param fromFrame the first frame to move.
     * @param toFrame the last frame to move.
     * @param movePosition the position to move the frames to.
     */
    void moveExposures(int fromFrame, int toFrame, int movePosition);

    /**
     *Sets the thumbview frameNumber in the timeline to be the active frame.
     *@param frameNumber the number of the new active frame
     */
    // void setNewActiveExposure(int exposureIndex);

    /**
     *Sets the thumbview frameNumber in the timeline to be the active frame.
     *@param frameNumber the number of the new active frame
     */
    void activateExposure();

    /**************************************************************************
     * Other private functions
     **************************************************************************/

    /**
     *Moves a thumbview in the thumbviews-vector from fromPosition to toPosition.
     *@param fromPosition the place in the vector the thumbview is moved from.
     *@param toPosition the place in the vector the thumbview is moved to.
     */
    void moveThumbView(int fromPosition, int toPosition);
};

#endif
