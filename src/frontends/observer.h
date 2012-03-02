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

#ifndef OBSERVER_H
#define OBSERVER_H

#include "domain/animation/exposure.h"

#include <QtCore/QVector>

class Exposure;


/**
 * The observer interface. All classes who wants to be notified when something
 * changes in the animationmodel has to implement from this class.
 *
 * The observers implemented with this class also has to be attatched to the
 * animationmodel.
 *
 * The observer is implemented with strong use of the push model. Although this
 * decreases the flexibility we gain alot in efficiency which is more
 * important to us.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class Observer
{
public:
    virtual ~Observer() {}

    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Abstract function for receiving notification when the model is erased.
     */
    virtual void updateClear() = 0;

    /**
     * Abstract function for receiving notification when a new project are created.
     */
    virtual void updateNewProject() = 0;

    /**
     * Abstract function for receiving notification when the descriptions of the
     * objects are changed.
     */
    virtual void updateDescriptionsUpdated() = 0;

    /**
     * Abstract function for recieving notification when a scene is to be played.
     * @param sceneIndex the scene to be played
     */
    virtual void updatePlaySound(int sceneIndex) = 0;

    /**
     * Abstract function for recieving notification when a new mix mode is set.
     * @param newMixMode the new mix mode
     */
    virtual void updateMixMode(int newMixMode) = 0;

    /**
     * Abstract function for recieving notification when a new mix count is set.
     * @param newMixCount the new mix count
     */
    virtual void updateMixCount(int newMixCount) = 0;

    /**
     * Abstract function for recieving notification when a new frames per second is set.
     * @param newFps the new frames per second
     */
    virtual void updateFramesPerSecond(int newFps) = 0;

    /**************************************************************************
     * Scene notification functions
     **************************************************************************/

    /**
     * Abstract function for recieving notification when a new scene is added
     * at location index.
     * @param index the location where the new scene is added.
     */
    virtual void updateAddScene(int sceneIndex) = 0;

    /**
     * Abstract function for recieving notification when a new scene is inserted
     * at location index.
     * @param index the location where the new scene is inserted.
     */
    virtual void updateInsertScene(int sceneIndex) = 0;

    /**
     * Abstract function for recieving notification when a new scene is set as the
     * current "active" scene.
     * @param sceneIndex the new active scene.
     */
    // virtual void updateNewActiveScene(int sceneIndex) = 0;

    /**
     * Abstract function for recieving notification when a new scene is set as the
     * current "active" scene.
     */
    virtual void updateActivateScene() = 0;

    /**
     * Abstract function for recieving notification when a scene is removed from
     * the model.
     * @param sceneIndex the scene which has been removed from the model.
     */
    virtual void updateRemoveScene(int sceneIndex) = 0;

    /**
     * Abstract function for recieving notification when a scene in the animation
     * has been moved.
     * @param sceneIndex the scene which have been moved.
     * @param movePosition the position the scene has been moved to.
     */
    virtual void updateMoveScene(int sceneIndex,
                                 int movePosition) = 0;

    /**************************************************************************
     * Take notification functions
     **************************************************************************/

    /**
     * Abstract function for recieving notification when a new take is added
     * at location index.
     * @param sceneIndex the scene where the new take is added.
     * @param takeIndex the location where the new take is added.
     */
    virtual void updateAddTake(int sceneIndex,
                               int takeIndex) = 0;

    /**
     * Abstract function for recieving notification when a new take is inserted
     * at location index.
     * @param sceneIndex the scene where the new take is added.
     * @param takeIndex the location where the new take is inserted.
     */
    virtual void updateInsertTake(int sceneIndex,
                                  int takeIndex) = 0;

    /**
     * Abstract function for recieving notification when a new take is set as the
     * current "active" take.
     * @param takeIndex the new active take.
     */
    // virtual void updateSetNewActiveTake(int takeIndex) = 0;

    /**
     * Abstract function for recieving notification when a new take is is the
     * current "active" take.
     */
    virtual void updateActivateTake() = 0;

    /**
     * Abstract function for recieving notification when a take is removed from
     * the model.
     * @param sceneIndex The scene of the take which has been removed from the model.
     * @param takeIndex The take which has been removed from the model.
     */
    virtual void updateRemoveTake(int sceneIndex,
                                  int takeIndex) = 0;

    /**************************************************************************
     * Exposure notification functions
     **************************************************************************/

    /**
     * Abstract function for recieving notification when a new exposure is added
     * at location index.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the location where the new exposure is added.
     */
    virtual void updateAddExposure(int sceneIndex,
                                   int takeIndex,
                                   int exposureIndex) = 0;

    /**
     * Abstract function for recieving notification when a new exposure is inserted
     * at location index.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the location where the new exposure is inserted.
     */
    virtual void updateInsertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex) = 0;

    /**
     * Function to recieve notification when one exposure are deleted.
     * @param sceneIndex the scene index of the selected exposure.
     * @param takeIndex the take index of the selected exposure.
     * @param exposureIndex the index of the removed exposure.
     */
    virtual void updateRemoveExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex) = 0;

    /**
     * Abstract function for recieving notification about frames removed from the model.
     * @param fromFrame the first frame of those removed
     * @param toFrame the last frame of those removed
     */
    virtual void updateRemoveExposures(int fromFrame,
                                       int toFrame) = 0;

    /**
     * Abstract function for recieving notification when frames are moved in the model.
     * @param fromFrame index of the first selected frame
     * @param toFrame index of the last selected frame
     * @param movePosition index to where the selection should be move to
     */
    virtual void updateMoveExposures(int fromFrame,
                                     int toFrame,
                                     int movePosition) = 0;

    /**
     * Abstract function for recieving notification when a new frame are selected.
     * @param ExposureIndex Index of the new active exposure.
     */
    // virtual void updateSetNewActiveExposure(int ExposureIndex) = 0;

    /**
     * Abstract function for recieving notification when a new frame are selected.
     * @param frameNumber the new active frame.
     */
    virtual void updateActivateExposure() = 0;

    /**
     * Abstract function for recieving notification when a new frame are selected.
     * @param frameNumber the new active frame.
     */
    // virtual void updateNewActiveExposure(int frameNumber) = 0;

    /**
     * Abstract function for recieving notification when a new frame are selected.
     * @param frameNumber the new active frame.
     */
    // virtual void updateExposure(int sceneIndex,
    //                             int takeIndex,
    //                             int exposureIndex) = 0;
};

#endif
