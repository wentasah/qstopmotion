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

#ifndef DOMAINFACADE_H
#define DOMAINFACADE_H

#include "domain/animation/animationproject.h"
#include "frontends/frontend.h"
// #include "frontends/viewfacade.h"
#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtGui/QTreeWidget>
#include <QtGui/QUndoStack>

class AnimationProject;
class Frontend;
class Scene;
class Take;
class Exposure;
class ViewFacade;

/**
 * Singleton facade for the domain level. All requests into the domain layer
 * has to pass through this facade. The only exception is observer requests
 * for display data.
 *
 * DomainFacade is implemented with lazy allocation.
 *
 *@author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class DomainFacade : public QObject
{
    Q_OBJECT
public:
    /**
     * The constructor. It is protected so that it will be impossible for other classes,
     * which don't inherit from it to instanciate the singleton.
     * @param f the GUI frontend
     */
    DomainFacade(Frontend *f);

    /**
     * Destructor
     */
    ~DomainFacade();

    /**
     * Retrieves the registered frontend.
     * @return the frontend if it is a valid frontend pointer, NULL otherwise.
     */
    Frontend* getFrontend();

    /**
     * Retrieves the animation model.
     * @return the animation model.
     */
    AnimationProject* getAnimationProject();

    /**
     * Undoes the last undoable operation on the model.
     */
    QUndoStack* getUndoStack();

    /**
     * Get the view facade.
     */
    ViewFacade* getView();

    /**
     * Clears the undo stack.
     */
    void clearUndoStack();

    /**
     * Initializes the audio device so it is ready to play sounds. It will sends
     * an error message to the frontend if somethings goes wrong, and audio will
     * be disabled until the problem is fixed.
     * @return true on success, false otherwise
     */
    bool initAudioDevice();

    /**
     * Shutdowns the audio device so other programs can use it.
     */
    void shutdownAudioDevice();

    /**************************************************************************
     * Animation functions
     **************************************************************************/

    /**
     * Checks if there are unsaved changes in the model.
     * @return true if there are unsaved changes, false otherwise.
     */
    bool isUnsavedChanges();

    /**
     * Retrieves the project file name.
     * @return the project file name if it's setted, NULL otherwise.
     */
    const QString getProjectFileName();

    /**
     * Retrieves the project path.
     * @return the project path if it's setted, NULL otherwise.
     */
    const QString getProjectPath();

    /**
     * Retrieves the project ID.
     * @return the project ID if it's setted, NULL otherwise.
     */
    const QString getProjectID();

    /**
     * Get the active source of the project.
     * @return active source.
     */
    int getVideoSource();

    /**
     * Set the active video source of the project.
     * @param newVideoSource the new active source.
     */
    void setVideoSource(int newVideoSource);

    /**
     * Get the active mixing mode of the project.
     * @return active mixing mode.
     */
    int getMixingMode();

    /**
     * Set the active mixing mode of the project.
     * @param newMixingMode the new mixing mode.
     */
    void setMixingMode(int newMixingMode);

    /**
     * Get the active unit of the project.
     * @return active unit mode.
     */
    int getUnitMode();

    /**
     * Set the active unit mode of the project.
     * @param newUnitMode the new unit mode.
     */
    void setUnitMode(int newUnitMode);

    /**
     * Get the active mix count of the project.
     * @return active mix count.
     */
    int getMixCount();

    /**
     * Set the active mix count of the project.
     * @param newSource the new mix count source.
     */
    void setMixCount(int newMixCount);

    /**
     * Get the active playback count of the project.
     * @return active playback count.
     */
    int getPlaybackCount();

    /**
     * Set the active playback count of the project.
     * @param newPlaybackCount the new playback count source.
     */
    void setPlaybackCount(int newPlaybackCount);

    /**
     * Get the number of frames per second of the project.
     * @return number of frames per second.
     */
    int getFramesPerSecond();

    /**
     * Set the number of frames per second of the project.
     * @param newFPS The new number of frames per second.
     */
    void setFramesPerSecond(int newFPS);

    /**
     * Add a new newProject object to the undo history.
     * @param projectDescription the description of the scene or a empty string.
     */
    void newProjectToUndo(const QString &projectDescription);

    /**
     * Undo create a new project.
     * @param projectDescription the description of the project or a empty string.
     */
    // bool newProjectUndo(const QString &projectDescription);

    /**
     * Creates a new project.
     * @param projectDescription the description of the project or a empty string.
     */
    bool newProjectRedo(const QString &projectDescription);

    /**
     * Add a new openProject object to the undo history.
     * @param projectPath the path to the project file.
     */
    void openProjectToUndo(const QString &projectPath);

    /**
     * Undo open project.
     * @param projectPath the path to the project file.
     */
    // bool openProjectUndo(const QString &projectPath);

    /**
     * Open a project.
     * @param projectPath the path to the project file.
     */
    bool openProjectRedo(const QString &projectPath);

    /**
     * Add a new saveProject object to the undo history.
     * @param projectPath the path to the project file.
     */
    void saveProjectToUndo(const QString &projectPath);

    /**
     * Undo save project.
     * @param projectPath the path to the project file.
     */
    // bool saveProjectUndo(const QString &projectPath);

    /**
     * Save a project.
     * @param projectPath the path to the project file.
     */
    bool saveProjectRedo(const QString &projectPath);

    /**
     * Add a new closeProject object to the undo history.
     */
    void closeProjectToUndo();

    /**
     * Undo close project.
     */
    // bool closeProjectUndo();

    /**
     * Close a project.
     */
    bool closeProjectRedo();

    /**
     * Exports the current project to a video file as specified by the video encoder.
     * @param encoder the video encoder to use for export to video
     * @return true on success, false otherwise
     */
    bool exportToVideo(VideoEncoder *encoder);

    /**
     * Exports the current project to a cinelerra-cv project.
     * @param file the cinelerra-cv project file
     * @return true on success, false otherwise
     */
    bool exportToCinelerra(const QString file);

    /**
     * The active scene, take and exposure are changed
     * @param newActiveScene The new selected scene.
     * @param newActiveTake The new selected take.
     * @param newActiveExposuree The new selected exposure.
     */
    void activeItemChanged(int newActiveScene,
                           int newActiveTake,
                           int newActiveExposure);

    /**************************************************************************
     * Scene functions
     **************************************************************************/

    /**
     * Returns the index of the currently active scene.
     * @return the index of the active scene.
     */
    int getActiveSceneIndex();

    /**
     * Sets a new active scene (the scene to be worked on now).
     * @param sceneIndex the new active scene.
     */
    void setActiveSceneIndex(int sceneIndex);

    /**
     * Return the active scene.
     * @return the active scene.
     */
    Scene *getActiveScene();

    /**
     * Function for retrieving number of scenes.
     * @return number of scenes
     */
    int getSceneSize();

    /**
     * Function for retrieving a given scene.
     * @param sceneIndex the scene number
     * @return scene
     */
    Scene *getScene(int sceneIndex);

    /**
     * Returns the exposure size of the scene.
     * @return the exposure size of the scene.
     */
    int getSceneExposureSize(int sceneIndex);

    /**
     * Add a new addScene object to the undo history.
     * @param sceneDescription the description of the scene or a empty string.
     */
    void addSceneToUndo(const QString &sceneDescription);

    /**
     * Create a new scene and adds the scene to the end of the animation.
     * @param sceneDescription the description of the scene or a empty string.
     */
    // void undoSceneAdd(const QString &sceneDescription);

    /**
     * Create a new scene and adds the scene to the end of the animation.
     * @param sceneDescription the description of the scene or a empty string.
     * @return The new added scene.
     */
    Scene *redoSceneAdd(const QString &sceneDescription);

    /**
     * Add a scene to the animation project.
     * @param newScene The new scene.
     */
    void addScene(Scene *newScene);

    /**
     * Add a new insert scene object to the undo history.
     * @param sceneDescription the description of the scene or a empty string.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     */
    void insertSceneToUndo(const QString &sceneDescription,
                           int            sceneIndex);

    /**
     * Undo create a new scene and insert the scene bevor the active scene in the animation.
     * @param sceneDescription the description of the scene or a empty string.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     */
    // void undoSceneInsert(const QString &sceneDescription,
    //                      int sceneIndex);

    /**
     * Create a new scene and insert the scene bevor the active scene in the animation.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     * @param sceneDescription the description of the scene or a empty string.
     * @return The new inserted scene.
     */
    Scene *redoSceneInsert(const QString &sceneDescription,
                           int sceneIndex);

    /**
     * Add a remove scene object to the undo history.
     * @param sceneIndex the index of the scene to retrieve.
     */
    void removeSceneToUndo(int sceneIndex);

    /**
     * Undo removes the active scene from the animation.
     * @param sceneIndex the index of the scene to retrieve.
     */
    // void undoSceneRemove(int sceneIndex);

    /**
     * Removes the active scene from the animation.
     * @param sceneIndex the index of the scene to retrieve.
     * @return The removed scene.
     */
    Scene *redoSceneRemove(int sceneIndex);

    /**
     * Add a move scene object to the undo history.
     * @param fromSceneIndex the number of the scene to move.
     * @param toSceneIndex the position to move the scene to.
     */
    void moveSceneToUndo(int fromSceneIndex,
                         int toSceneIndex);

    /**
     * Undo moves the scene at position sceneIndex to the position movePosition.
     * @param fromSceneIndex the number of the scene to move.
     * @param toSceneIndex the position to move the scene to.
     */
    // void undoSceneMove(int fromSceneIndex,
    //                    int toSceneIndex);

    /**
     * Moves the scene at position sceneIndex to the position movePosition.
     * @param fromSceneIndex the number of the scene to move.
     * @param toSceneIndex the position to move the scene to.
     */
    void redoSceneMove(int fromSceneIndex,
                       int toSceneIndex);

    /**
     * Adds a sound the given frame number. An error message will be
     * sent to the frontend if somethings goes wrong with the adding.
     * @param frameNumber the number of the frame to add the sound to
     * @param filename the path to the file with the sound
     * @param soundName the new name of the sound.
     * @return zero on success, less than zero on failure
     */
    int addSoundToScene(int sceneIndex,
                        const QString filename,
                        const QString soundName);

    /**
     * Removes the sound with index soundNumber from the frame with index
     * frameNumber.
     * @param frameNumber the index of the frame to remove a sound from.
     * @param soundNumber the index of the sound to remove from the frame.
     */
    void removeSoundFromScene(int sceneIndex,
                              int soundNumber);

    /**
     * Plays the sound of the scene with the index sceneIndex
     * @param sceneIndex the index of the scene to play
     */
    void playSound(int sceneIndex);

    /**************************************************************************
     * Take functions
     **************************************************************************/

    /**
     * Retrieving the index of the currently active take.
     * @return the index of the active take.
     */
    int getActiveTakeIndex();

    /**
     * Sets a new active take (the take to be worked on now).
     * @param takeIndex the new active take.
     */
    void setActiveTakeIndex(int takeIndex);

    /**
     * Return the active take from the active scene.
     * @return the active take.
     */
    Take *getActiveTake();

    /**
     * Function for retrieving a given take of a given scene.
     * @param sceneIndex the index of the scene.
     * @param takeIndex the index of the take.
     * @return take
     */
    Take *getTake(int sceneIndex,
                  int takeIndex);

    /**
     * Returns the exposure size of the take.
     * @param sceneIndex the index of the scene.
     * @param takeIndex the index of the take.
     * @return the exposure size of the take.
     */
    int getTakeExposureSize(int sceneIndex,
                            int takeIndex);

    /**
     * Add a new add take object to the undo history.
     * @param takeDescription the description of the take or a empty string.
     * @param sceneIndex the index of the scene the take to add
     */
    void addTakeToUndo(const QString &takeDescription,
                       int            sceneIndex);

    /**
     * Undo create a new take and add the take as the last to the scene.
     * @param takeDescription the description of the take or a empty string.
     * @param sceneIndex the index of the scene the take to add
     */
    // void undoTakeAdd(const QString &takeDescription,
    //                  int            sceneIndex);

    /**
     * Create a new take and add the take as the last to the scene.
     * @param takeDescription the description of the take or a empty string.
     * @param sceneIndex the index of the scene the take to add
     * @return The new inserted take.
     */
    Take *redoTakeAdd(const QString &takeDescription,
                      int            sceneIndex);

    /**
     * Add a take to the scene.
     * @param newTake The new take.
     */
    void addTake(Take *newTake);

    /**
     * Add a new insert take object to the undo history.
     * @param takeDescription The description of the take or a empty string.
     * @param sceneIndex The index of the scene the take to add
     * @param takeIndex The index of the take where the new take is inserted bevor.
     */
    void insertTakeToUndo(const QString &takeDescription,
                          int            sceneIndex,
                          int            takeIndex);

    /**
     * Undo create a new take and insert the take bevor the active scene.
     * @param takeDescription the description of the take or a empty string.
     * @param sceneIndex the index of the scene the take to add
     * @param takeIndex The index of the take where the new take is inserted bevor.
     */
    // void undoTakeInsert(const QString &takeDescription,
    //                     int            sceneIndex,
    //                     int            sceneIndex);

    /**
     * Createa new take and insert the take bevor the active scene.
     * @param takeDescription the description of the take or a empty string.
     * @param sceneIndex the index of the scene the take to add
     * @param takeIndex The index of the take where the new take is inserted bevor.
     * @return The new inserted take.
     */
    Take *redoTakeInsert(const QString &takeDescription,
                         int            sceneIndex,
                         int            takeIndex);

    /**
     * Removes the active take from the animation.
     * @param sceneIndex the index of the scene to retrieve the take from.
     * @param takeIndex the index of the take to retrieve.
     */
    void removeTakeToUndo(int sceneIndex,
                          int takeIndex);

    /**
     * Undo removes the active take from the animation.
     * @param sceneIndex the index of the scene to retrieve the take from.
     * @param takeIndex the index of the take to retrieve.
     */
    // void undoTakeRemove(int sceneIndex,
    //                     int takeIndex);

    /**
     * Removes the active take from the animation.
     * @param sceneIndex the index of the scene to retrieve the take from.
     * @param takeIndex the index of the take to retrieve.
     * @return The removed take.
     */
    Take *redoTakeRemove(int sceneIndex,
                         int takeIndex);

    /**
     * Add a new move take object to the undo history.
     * @param fromSceneIndex the index of the scene of the take to move.
     * @param fromTakeIndex the index of the take to move.
     * @param toSceneIndex the index of the scene of move the take to.
     * @param toTakeIndex the index to move the take to.
     */
    void moveTakeToUndo(int fromSceneIndex,
                        int fromTakeIndex,
                        int toSceneIndex,
                        int toTakeIndex);

    /**
     * Undo moves the take at position takeIndex to the position movePosition.
     * @param fromSceneIndex the index of the scene of the take to move.
     * @param fromTakeIndex the index of the take to move.
     * @param toSceneIndex the index of the scene of move the take to.
     * @param toTakeIndex the index to move the take to.
     */
    // void undoTakeMove(int fromSceneIndex,
    //                   int fromTakeIndex,
    //                   int toSceneIndex,
    //                   int toTakeIndex);

    /**
     * Moves the take at position takeIndex to the position movePosition.
     * @param takeIndex the number of the take to move.
     * @param movePosition the position to move the scene to.
     */
    void redoTakeMove(int fromSceneIndex,
                      int fromTakeIndex,
                      int toSceneIndex,
                      int toTakeIndex);

    /**************************************************************************
     * Exposure functions
     **************************************************************************/

    /**
     * Retrieving the index of the currently active exposure.
     * @return the index of the active exposure.
     */
    int getActiveExposureIndex();

    /**
     * Function to change the currently active exposure. (Working frame).
     * @param exposureIndex the index of the new active exposure.
     */
    void setActiveExposureIndex(int exposureIndex);

    /**
     * Return the active exposure from the active scene and active take.
     * @return the actual exposure.
     */
    Exposure *getActiveExposure();

    /**
     * Return the exposure with the given scene index, take index and exposure index.
     * @param sceneIndex the index of the scene to retrieve the exposure from.
     * @param takeIndex the index of the take to retrieve the exposure from.
     * @param exposureIndex the index of the exposure to retrieve
     * @return the exposure to retrieve.
     */
    Exposure *getExposure(int sceneIndex,
                          int takeIndex,
                          int exposureIndex);

    /**
     * Retrieves the size of the model
     * @return the size of the model
     */
    int getTotalExposureSize();

    /**
     * Creates a new addExposure object in the undo history.
     * @param filePath the file path ot the image file of the exposure.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     */
    void addExposureToUndo(const QString &filePath,
                           int            sceneIndex,
                           int            takeIndex);

    /**
     * Undo creates a new exposure and add it to the animation project.
     * @param filePath the file path of the image file of the exposure.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     */
    // void undoExposureAdd(const QString &filePath,
    //                      int            sceneIndex,
    //                      int            takeIndex);

    /**
     * Creates a new exposure and add it to the animation project.
     * @param filePath the file path of the image file of the exposure.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     */
    void redoExposureAdd(const QString &filePath,
                         int            sceneIndex,
                         int            takeIndex);

    /**
     * Add a new Exposure to the take.
     * @param newExposure The new Exposure to add.
     */
    void addExposure(Exposure *newExposure);

    /**
     * Creates a new insertExposure object in the undo history.
     * @param filePath the file path of the image file of the exposure or a empty string.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     * @param exposureIndex The index of the exposure where the new exposures are inserted bevor.
     */
    void insertExposureToUndo(const QString &filePath,
                              int            sceneIndex,
                              int            takeIndex,
                              int            exposureIndex);

    /**
     * Undo creates a new exposure and insert it in the animation project.
     * @param filePath the file path of the image file of the exposure or a empty string.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     * @param exposureIndex The index of the exposure where the new exposures are inserted bevor.
     */
    // void undoExposureInsert(const QString &filePath
    //                         int            sceneIndex,
    //                         int            takeIndex,
    //                         int            exposureIndex);

    /**
     * Creates a new exposure and insert it in the animation project.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     * @param exposureIndex The index of the exposure where the new exposures are inserted bevor.
     * @param after The new exposure is inserterd after the exposure index.
     * @param fileName the file name of the image file of the exposure or a empty string.
     */
    void redoExposureInsert(const QString &filePath,
                            int            sceneIndex,
                            int            takeIndex,
                            int            exposureIndex);

    /**
     * Add a new remove exposure object to the undo history.
     * @arg sceneIndex the index of the scene of the exposure to remove
     * @arg takeIndex the index of the take of the exposure to remove
     * @arg exposureIndex the index of the exposure to remove
     */
    void removeExposureToUndo(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Removes the exposure with the given tree item from the animation.
     * @arg sceneIndex the index of the scene of the exposure to remove
     * @arg takeIndex the index of the take of the exposure to remove
     * @arg exposureIndex the index of the exposure to remove
     */
    // void undoExposureRemove(int sceneIndex,
    //                         int takeIndex,
    //                         int exposureIndex);

    /**
     * Removes the exposure with the given tree item from the animation.
     * @arg sceneIndex the index of the scene of the exposure to remove
     * @arg takeIndex the index of the take of the exposure to remove
     * @arg exposureIndex the index of the exposure to remove
     */
    Exposure *redoExposureRemove(int sceneIndex,
                                 int takeIndex,
                                 int exposureIndex);

    /**************************************************************************
     * Old frames functions
     **************************************************************************/

    /**
     * Adds the frames in the vector to the animation model and sets up the undo
     * command object
     * @param frameNames a vector containing the frames to be added to the animation.
     */
    void addFrames(const QVector<QString> &frameNames);

    /**
     * Removes the frame between (inclusive) fromFrame and toFrame from
     * the animation model.
     * @param fromFrame the first frame to remove.
     * @param toFrame the last frame to remove.
     */
    void removeFrames(int fromFrame,
                      int toFrame);

    /**
     * Moves the frames in the frameNumbers container to toPosition.
     * @param fromFrame the first frame in the selection.
     * @param toFrame the last frame in the selection.
     * @param movePosition the position to move the selected frames
     */
    void moveFrames(int fromFrame,
                    int toFrame,
                    int movePosition);

protected:

private:

    /**
     * The frontend in the program
     */
    Frontend *frontend;

    /**
     * The animation project in the program
     */
    AnimationProject *animationProject;
};

#endif
