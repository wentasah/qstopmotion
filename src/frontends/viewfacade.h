/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#ifndef VIEWFACADE_H
#define VIEWFACADE_H

#include <QDomDocument>
#include <QDomElement>
#include <QUndoStack>
#include <QVector>

#include "frontends/observer.h"
#include "frontends/frontend.h"
#include "technical/videoencoder/ffmpegencoder.h"

class Frontend;
class Observer;
class DomainFacade;


/**
 * The facade to all views class.
 *
 * ViewFacade serves as the Subject in our implementation of the observer
 * pattern, and takes care of notifying the observers when a change occur.
 */
class ViewFacade
{
public:

    /**
     * Constructor.
     * @param f the GUI frontend
     */
    ViewFacade(Frontend *f);

    /**
     * Destructor.
     */
    virtual ~ViewFacade();

    /**
     * Get the frontend.
     * @return the frontend.
     */
    Frontend* getFrontend();

    /**
     * Get the domain facade
     */
    DomainFacade* getProject();

    /**
     * Attatches a new observer to the model. The observer will be notified
     * when something is changed in the model.
     * @param o the observer to be attached to the model.
     */
    void attach(Observer *o);

    /**
     * Detatches an observer from the model. The observer will no longer be
     * notified when something is changed in the model.
     * @param o the observer to be detatched from the model.
     */
    void detatch(Observer *o);

    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Notify the observers that the model has been cleared.
     */
    void notifyRemoveProject();

    /**
     * Notify the observers that a new project is created.
     */
    void notifyNewProject();

    /**
     * Notify the observers that a project is opened.
     */
    void notifyOpenProject();

    /**
     * Notify the observers that a scene is to be played.
     * @param sceneIndex the scene to be played.
     */
    void notifyPlaySound(int sceneIndex);

    /**
     * Notify the observers that a new mix mode is selected.
     * @param newMixMode the new mix mode.
     */
    void notifyNewMixMode(int newMixMode);

    /**
     * Notify the observers that a new mix count is selected.
     * @param newMixCount the new mix count.
     */
    void notifyNewMixCount(int newMixCount);

    /**
     * Notify the observers that a new live view frames per second value is selected.
     * @param newFps the new frames per second.
     */
    void notifyNewLiveViewFps(double newFps);

    /**
     * Notify the observers that a new video frames per second value is selected.
     * @param newFps the new frames per second.
     */
    void notifyNewVideoFps(int newFps);

    /**************************************************************************
     * Scene notification functions
     **************************************************************************/

    /**
     * Notify the observers that a new scene has been added to the animation.
     * @param sceneIndex the place the new scene should be added to.
     */
    void notifyAddScene(int sceneIndex);

    /**
     * Notify the observers that a new scene has been inserted in the animation.
     * @param sceneIndex the place the new scene should be inserted to.
     */
    void notifyInsertScene(int sceneIndex);

    /**
     * Notify the observers that a new scene is activated.
     */
    void notifyActivateScene();

    /**
     * Notify the observers that a scene has been removed from the animation.
     * @param sceneIndex the scene which has been removed from the animation.
     */
    void notifyRemoveScene(int sceneIndex);

    /**
     * Notify the observers that a scene in the animation has been moved.
     * @param sceneIndex the scene which has been moved.
     * @param movePosition the location the scene has been moved to.
     */
    void notifyMoveScene(int sceneIndex,
                         int movePosition);

    /**************************************************************************
     * Take notification functions
     **************************************************************************/

    /**
     * Notify the observers that a new take has been added to the animation.
     * @param sceneIndex the scene the new take should be added to.
     * @param takeIndex the place the new take should be added to.
     */
    void notifyAddTake(int sceneIndex,
                       int takeIndex);

    /**
     * Notify the observers that a new take has been inserted to the animation.
     * @param sceneIndex the scene the new take should be inserted to.
     * @param takeIndex the place the new take should be inserted to.
     */
    void notifyInsertTake(int sceneIndex,
                          int takeIndex);

    /**
     * Notify the observers that a new take is activated.
     */
    void notifyActivateTake();

    /**
     * Notify the observers that a take has been removed from the animation.
     * @param sceneIndex The scene of the take which has been removed from the animation.
     * @param takeIndex The take which has been removed from the animation.
     */
    void notifyRemoveTake(int sceneIndex,
                          int takeIndex);

    /**************************************************************************
     * Exposure notification functions
     **************************************************************************/

    /**
     * Notify the observers that a new exposure has been added to the animation.
     * @param sceneIndex the scene of the take the new take should be added to.
     * @param takeIndex the take the new exposure should be added to.
     * @param exposureIndex the place the new exposure should be added to.
     */
    void notifyAddExposure(int sceneIndex,
                           int takeIndex,
                           int exposureIndex);

    /**
     * Notify the observers that a new exposure has been inserted to the animation.
     * @param sceneIndex the scene of the take the new take should be inserted to.
     * @param takeIndex the take the new exposure should be inserted to.
     * @param exposureIndex the place the new exposure inserted be added to.
     */
    void notifyInsertExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Notify the observers that a new scene is activated.
     */
    void notifyActivateExposure();

    /**
     * Notify the observers that a selected exposure are removed from the model.
     * @param sceneIndex the scene index of the selected exposure.
     * @param takeIndex the take index of the selected exposure.
     * @param exposureIndex the index of the removed exposure.
     */
    void notifyRemoveExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Notify the observers that a frame in the model has changed location (moved).
     * @param fromFrame the first frame in the selection
     * @param toFrame the last frame in the selection
     * @param movePosition the index to move the selected frames
     */
    void notifyMoveExposures(int fromFrame,
                             int toFrame,
                             int movePosition);

    /**
     * Notify the observers the frame of an exposure has changed.
     * @param modSceneIndex The index of the scene of the modified exposure.
     * @param modTakeIndex The index of the take of the modified exposure.
     * @param modExposureIndex The index of the modified exposure.
     */
    void notifyModifyExposure(int modSceneIndex,
                              int modTakeIndex,
                              int modExposureIndex);

protected:

private:

    /**
     * The frontend in the program
     */
    Frontend *frontend;

    /**
     * The datastructure containing the observers to be notified when something is
     * changed in the model.
     */
    QVector<Observer*> observers;
};

#endif
