/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include <QtCore/QFile>
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
     * Get the view facade.
     */
    ViewFacade* getView();

    /**************************************************************************
     * Audio functions
     **************************************************************************/

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
     * Undo functions
     **************************************************************************/

    /**
     * Get the undo stack.
     * @return The undo stack.
     */
    QUndoStack* getUndoStack();

    /**
     * Clears the undo stack and erase the history file.
     */
    void clearUndoStack();

    /**
     * Write a entry to the history file
     */
    void writeHistoryEntry(const QString &entry);

    /**
     * Remove the history file and all of its content.
     * @return True if successful; otherwise fale.
     */
    void removeHistoryFile();

    /**************************************************************************
     * Animation functions
     **************************************************************************/

    /**
     * Retrieves the animation model.
     * @return the animation model.
     */
    AnimationProject* getAnimationProject();

    /**
     * Checks if there are unsaved changes in the settings of the project.
     * @return true if there are unsaved changes, false otherwise.
     */
    bool isProjectSettingsChanges();

    /**
     * Checks if there are unsaved changes in the animation of the project.
     * @return true if there are unsaved changes, false otherwise.
     */
    bool isProjectAnimationChanges();

    /**
     * Checks if there are a active project.
     * @return true if there are a active project, false otherwise.
     */
    bool isActiveProject();

    /**
     * Retrieves the new project file path.
     * @return the project file path if it's setted, NULL otherwise.
     */
    const QString getNewProjectFilePath();

    /**
     * Retrieves the new project path.
     * @return the project path if it's setted, NULL otherwise.
     */
    const QString getNewProjectPath();

    /**
     * Retrieves the project description.
     * @return the project description if it's setted, NULL otherwise.
     */
    const QString getProjectDescription();

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
    int getMixMode();

    /**
     * Set the active mix mode of the project.
     * @param newMixMode the new mix mode.
     */
    void setMixMode(int newMixMode);

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

    /**************************************************************************
     * Project functions
     **************************************************************************/

    /**
     * Recover the last changes after a crash.
     * @return true if an crashed project is recovered
     */
    bool recoverProject();

    /**
     * Set all settings to the actual default settings.
     */
    void setProjectSettingsToDefault();

    /**
     * Add a new newProject object to the undo history.
     * @param projectDescription the description of the scene or a empty string.
     */
    void newProjectToUndo(const QString &projectDescription);

    /**
     * Create a new project.
     * @param projectDescription the description of the scene or a empty string.
     */
    void newProject(const QString &projectDescription);

    /**
     * Create a existing project.
     * @param project The existing project.
     */
    void newProject(AnimationProject *project);

    /**
     * Add a new openProject object to the undo history.
     * @param projectPath the path to the project file.
     */
    void openProjectToUndo(const QString &projectPath);

    /**
     * Open an project.
     * @param projectPath the path to the project file.
     * @return true on success, false otherwise
     */
    bool openProject(const QString &projectPath);

    /**
     * Open an existing project.
     * @param project The existing project.
     */
    void openProject(AnimationProject *project);

    /**
     * Add a new saveProject object to the undo history.
     * @param projectPath the path to the project file.
     * @param saveAs this is a save as operation.
     */
    void saveProjectToUndo(const QString &projectPath, bool saveAs);

    /**
     * Add a new closeProject object to the undo history.
     */
    // void closeProjectToUndo();

    /**
     * Close the project.
     */
    void closeProject();

    /**
     * Remove the animation project from the facade.
     */
    AnimationProject *removeProject();

    /**************************************************************************
     * Scene functions
     **************************************************************************/

    /**
     * Returns the index of the currently active scene.
     * @return the index of the active scene.
     */
    int getActiveSceneIndex();

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
     * Returns the take size of the scene.
     * @param sceneIndex the index of the scene.
     * @return the take size of the scene.
     */
    int getSceneTakeSize(int sceneIndex);

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
     * Add a new insert scene object to the undo history.
     * @param sceneDescription the description of the scene or a empty string.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     */
    void insertSceneToUndo(const QString &sceneDescription,
                           int            sceneIndex);

    /**
     * Add a remove scene object to the undo history.
     * @param sceneIndex the index of the scene to retrieve.
     */
    void removeSceneToUndo(int sceneIndex);

    /**
     * Add a move scene object to the undo history.
     * @param fromSceneIndex the index of the scene to move.
     * @param toSceneIndex the position to move the scene to.
     */
    void moveSceneToUndo(int fromSceneIndex,
                         int toSceneIndex);

    /**
     * Add a select scene object to the undo history.
     * @param newSceneIndex the position to move the scene to.
     */
    void selectSceneToUndo(int newSceneIndex);

    /**************************************************************************
     * Scene sound functions
     **************************************************************************/

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
     * Add a new insert take object to the undo history.
     * @param takeDescription The description of the take or a empty string.
     * @param sceneIndex The index of the scene the take to add
     * @param takeIndex The index of the take where the new take is inserted bevor.
     */
    void insertTakeToUndo(const QString &takeDescription,
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
     * Add a new select take object to the undo history.
     * @param newSceneIndex the new index of the scene of the take.
     * @param newTakeIndex the new index of the take.
     */
    void selectTakeToUndo(int newSceneIndex,
                          int newTakeIndex);

    /**************************************************************************
     * Exposure functions
     **************************************************************************/

    /**
     * Retrieving the index of the currently active exposure.
     * @return the index of the active exposure.
     */
    int getActiveExposureIndex();

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
     * @param copy the file has to copy to the temp dirctory
     */
    void addExposureToUndo(const QString &filePath,
                           int            sceneIndex,
                           int            takeIndex,
                           bool           copy);

    /**
     * Creates a new insertExposure object in the undo history.
     * @param filePath the file path of the image file of the exposure or a empty string.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     * @param exposureIndex The index of the exposure where the new exposures are inserted bevor.
     * @param copy the file has to copy to the temp dirctory
     */
    void insertExposureToUndo(const QString &filePath,
                              int            sceneIndex,
                              int            takeIndex,
                              int            exposureIndex,
                              bool           copy);

    /**
     * Add a new remove exposure object to the undo history.
     * @param sceneIndex the index of the scene of the exposure to remove
     * @param takeIndex the index of the take of the exposure to remove
     * @param exposureIndex the index of the exposure to remove
     */
    void removeExposureToUndo(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Add a new move exposure object to the undo history.
     * @param fromSceneIndex the from index of the scene of the take of the exposure to move
     * @param fromTakeIndex the from index of the take of the exposure to move
     * @param fromExposureIndex the from index of the exposure to move
     * @param toSceneIndex the to index of the scene of the take of the exposure to move
     * @param toTakeIndex the to index of the take of the exposure to move
     * @param toExposureIndex the to index of the exposure to move
     */
    void moveExposureToUndo(int fromSceneIndex,
                            int fromTakeIndex,
                            int fromExposureIndex,
                            int toSceneIndex,
                            int toTakeIndex,
                            int toExposureIndex);

    /**
     * Add a new select exposure object to the undo history.
     * @param newSceneIndex the new index of the scene of the take of the exposure to select
     * @param newTakeIndex the new index of the take of the exposure to select
     * @param newExposureIndex the new index of the exposure to select
     */
    void selectExposureToUndo(int newSceneIndex,
                              int newTakeIndex,
                              int newExposureIndex);

    /**
     * Get the position of the modified exposure in the project.
     * @param filePath The path to the modified exposure.
     * @param modSceneIndex the index of the scene of the take of the modified exposure.
     * @param modTakeIndex the index of the take of the modified exposure.
     * @param modExposureIndex the index of the modified exposure.
     * @return True if the exposure is found and false else.
     */
    bool getModifyedExposure(const QString &filePath,
                             int &modSceneIndex,
                             int &modTakeIndex,
                             int &modExposureIndex);

    /**************************************************************************
     * Private functions
     **************************************************************************/

private:

    /**
     * Copies the files belonging to this Exposure to a temporary directory.
     * @param fromImagePath the path to the image to copy.
     * @return the file name of the new file.
     */
    const QString copyToTemp(const QString &fromImagePath);

    /**************************************************************************
     * Private members
     **************************************************************************/

private:

    /**
     * The frontend in the program
     */
    Frontend *frontend;

    /**
     * The animation project in the program
     */
    AnimationProject *animationProject;

    /**
     * Path of the history file.
     */
    QString historyFilePath;

    /**
     * The history file.
     */
    QFile *historyFile;

    /**
     * Flag to enable/disable history write function.
     */
    bool writeHistory;

    /**
     * Container where one can register and retrieve undo objects for undo and
     * remove operations.
     */
    QUndoStack *undoStack;

};

#endif
