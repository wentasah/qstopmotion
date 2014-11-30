/******************************************************************************
 *  Copyright (C) 2005-2014 by                                                *
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

#ifndef ANIMATIONPROJECT_H
#define ANIMATIONPROJECT_H

#include "domain/animation/exposure.h"
#include "domain/animation/projectserializer.h"
#include "domain/animation/scene.h"
#include "frontends/frontend.h"
#include "technical/audio/audiodriver.h"
#include "technical/videoencoder/ffmpegencoder.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>

class ProjectSerializer;
// class Scene;
// class Take;
// class Exposure;

/**
 * Implementation of the animationmodel containing the data about the animation.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class AnimationProject : public QObject
{
    Q_OBJECT
public:
    enum ExposureFileLocation {
        NoFile,
        InProjectPath,
        InTempPath,
    };

    enum ImageTransformationMode {
        ScaleImage,      // Scale the image to the output size
        ClipImage,       // Clip the image to the output size according to the adjustment setting
        ZoomImage,       // Zoom the image to the output size according to the zoom setting
    };

    /**
     * Initializes the variables of the animation to starting values.
     * @param f The frontend of the application.
     */
    AnimationProject(Frontend* f);

    /**
     * Cleans up the animation.
     */
    ~AnimationProject();

    /**
     * Get the frontend.
     * @return the frontend.
     */
    Frontend* getFrontend();

    /**
     * Get the project serializer.
     * @return the project serializer.
     */
    ProjectSerializer* getProjectSerializer();

    /**
     * Get the name ot the temp directory of the project
     * @return temp directory of the project
     */
    const QString getAppTempDirName() const;

    /**
     * Retrieves the new project file name
     * @return the project file name if it's setted, NULL otherwise.
     */
    const QString getNewProjectFilePath() const;

    /**
     * Get the new project path of the project
     * @return project path of the project
     */
    const QString getNewProjectPath() const;

    /**
     * Get the description of the project
     * @return Description of the project
     */
    const QString getDescription() const;

    /**
     * Get the new image path of the project
     * @return image path of the project
     */
    const QString getNewImagePath() const;

    /**
     * Get the old image path of the project
     * @return image path of the project
     */
    const QString getOldImagePath() const;

    /**
     * Get the new sound path of the project
     * @return sound path of the project
     */
    const QString getNewSoundPath() const;

    /**
     * Get the old sound path of the project
     * @return sound path of the project
     */
    const QString getOldSoundPath() const;

    /**************************************************************************
     * Project preferences
     **************************************************************************/

    /**
     * Get the active recording mode of the project.
     * @return active recording mode.
     */
    int getRecordingMode() const;

    /**
     * Set the active recording mode of the project.
     * @param newRecordingMode The new recording mode.
     */
    void setRecordingMode(int newRecordingMode);

    /**
     * Get the active video source of the project.
     * @return active video source.
     */
    int getVideoSource() const;

    /**
     * Set the active video source of the project.
     * @param newVideoSource the new active source.
     */
    void setVideoSource(int newVideoSource);

    /**
     * Get the active video resolution of the project.
     * @return active video resolution.
     */
    int getResolution() const;

    /**
     * Set the active video resolution of the project.
     * @param newResolution the new active resolution.
     */
    void setResolution(int newResolution);

    /**
     * Get the active mixing mode of the project.
     * @return active mixing mode.
     */
    int getMixMode() const;

    /**
     * Set the active mixing mode of the project.
     * @param newMixingMode the new mixing mode.
     */
    void setMixMode(int newMixingMode);

    /**
     * Get the active mix count of the project.
     * @return active mix count.
     */
    int getMixCount() const;

    /**
     * Set the active mix count of the project.
     * @param newSource the new mix count source.
     */
    void setMixCount(int newMixCount);

    /**
     * Get the active playback count of the project.
     * @return active playback count.
     */
    int getPlaybackCount() const;

    /**
     * Set the active playback count of the project.
     * @param newPlaybackCount the new playback count source.
     */
    void setPlaybackCount(int newPlaybackCount);

    /**
     * Get the active unit of the project.
     * @return active unit mode.
     */
    int getUnitMode() const;

    /**
     * Set the active unit mode of the project.
     * @param newUnitMode the new unit mode.
     */
    void setUnitMode(int newUnitMode);

    /**************************************************************************
     * Image import preferences
     **************************************************************************/

    /**
     * Get the image grabber source index.
     * @return image grabber source index
     */
    int getGrabberSource();

    /**
     * Set the image grabber source index.
     * @param index Index of the new image grabber source
     */
    void setGrabberSource(int index);

    /**
     * Get the image format of the project.
     * @return image format.
     */
    int getImageFormat();

    /**
     * Set the image format of the project.
     * @param newIF The new image format.
     */
    void setImageFormat(int newIF);

    /**
     * Get the image quality value of the project.
     * @return image quality value.
     */
    int getImageQuality();

    /**
     * Set the image quality value of the project.
     * @param newValue The new image quality value.
     */
    void setImageQuality(int newValue);

    /**
     * Get the image size of the project.
     * @return image size.
     */
    int getImageSize();

    /**
     * Set the image size of the project.
     * @param newVS The new image size.
     */
    void setImageSize(int newVS);

    /**
     * Get the transformation value of the project.
     * @return Transformation value.
     */
    int getImageTransformation();

    /**
     * Set the transformation value of the project.
     * @param newTransform The new transformation value.
     */
    void setImageTransformation(int newTransform);

    /**
     * Get the image adjustment of the project.
     * @return Image adjustment.
     */
    int getImageAdjustment();

    /**
     * Set the image adjustment of the project.
     * @param newIA The new image adjustment.
     */
    void setImageAdjustment(int newIA);

    /**
     * Get the zoom value of the project.
     * @return Zoom value.
     */
    int getZoomValue();

    /**
     * Set the zoom value of the project.
     * @param newZV The new zoom value.
     */
    void setZoomValue(int newZV);

    /**
     * Get the live view fps of the project.
     * @return live view fps value.
     */
    int getLiveViewFps();

    /**
     * Set the live view fps of the project.
     * @param newValue The new live view fps value.
     */
    void setLiveViewFps(int newValue);

    /**************************************************************************
     * Video export preferences
     **************************************************************************/

    /**
     * Get the encoder application of the project.
     * @return Encoder application.
     */
    int getEncoderApplication() const;

    /**
     * Set the encoder application of the project.
     * @param newEA The new encoder application.
     */
    void setEncoderApplication(int newEA);

    /**
     * Get the video format of the project.
     * @return Video format.
     */
    int getVideoFormat() const;

    /**
     * Set the video format of the project.
     * @param newVF The new video format.
     */
    void setVideoFormat(int newVF);

    /**
     * Get the video size of the project.
     * @return Video size.
     */
    int getVideoSize() const;

    /**
     * Set the video size of the project.
     * @param newVS The new video size.
     */
    void setVideoSize(int newVS);

    /**
     * Get the video frames per second of the project.
     * @return The video frames per second.
     */
    int getVideoFps() const;

    /**
     * Set the video frames per second of the project.
     * @param newFPS The new video frames per second.
     */
    void setVideoFps(int newFPS);

    /**
     * Get the use default output file flag of the project.
     * @return Use default output file flag.
     */
    bool getUseDefaultOutputFile() const;

    /**
     * Set the use default output file flag of the project.
     * @param newUDOF The new use default output file flag.
     */
    void setUseDefaultOutputFile(bool newUDOF);

    /**
     * Get the default output file name of the project.
     * @return Default output file name.
     */
    const QString getDefaultOutputFileName();

    /**
     * Set the default output file name of the project.
     * @param newDOFN The new default output file name.
     */
    void setDefaultOutputFileName(const QString newDOFN);

    /**************************************************************************
     * Project functions
     **************************************************************************/

    /**
     * Opens a project.
     * @param filePath the project file path of the project to open.
     * @return true on success, false otherwise
     */
    bool openProject(const QString &filePath);

    /**
     * Saves the active project to a XML-file which is written to disk.
     * @param filepath the path to store the project files within.
     * @param saveAs the project sa to saved to a new project
     * @return true on success, false otherwise
     */
    bool saveProject(const QString &filePath, bool saveAs);

    /**
     * Creates a new project.
     * @param projectDescription the description of the project or a empty string.
     * @return true on success, false otherwise
     */
    bool newProject(const QString &projectDescription);

    /**
     * Checks if there are unsaved changes in the settings.
     * @return true if there are unsaved changes, false otherwise.
     */
    bool isSettingsChanges() const;

    /**
     * Increment the number of unsaved changes in the settings.
     */
    void incSettingsChanges();

    /**
     * Decrement the number of unsaved changes in the settings.
     */
    void decSettingsChanges();

    /**
     * Checks if there are unsaved changes in the animation.
     * @return true if there are unsaved changes, false otherwise.
     */
    bool isAnimationChanges() const;

    /**
     * Increment the number of unsaved changes in the animation.
     */
    void incAnimationChanges();

    /**
     * Decrement the number of unsaved changes in the animation.
     */
    void decAnimationChanges();

    /**
     * Initializes the audio device so it is ready to play sounds.
     * @return true on success, false otherwise
     */
    bool initAudioDevice();

    /**
     * Shutdowns the the audio device so that other programs can use it.
     */
    void shutdownAudioDevice();

    /**
     * Function to call when a file has been altered and the gui
     * should be updated.
     * @param alteredFile the file which has been altered.
     */
    // void animationChanged(const QString &alteredFile);

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
    bool exportToCinelerra(const QString &file);

    /**
     * Get the data of the settings from the project file
     */
    bool readSettingsFromProject(QDomElement &settingsNode);

    /**
     * Set the data of the settings to the project file
     */
    bool saveSettingsToProject(QDomDocument &doc, QDomElement &settingsNode);

    /**
     * Convert all images of the project to a new new image format.
     * @param newFormat The new format of the images.
     * @param newQuality The new quality value of the images.
     * @return true if all images are converted to the new image format.
     */
    bool convertImages(int newFormat, int newQuality);

    /**************************************************************************
     * Scene functions
     **************************************************************************/

    /**
     * Returns the index of the currently active scene.
     * @return the index of the active scene.
     */
    int getActiveSceneIndex() const;

    /**
     * Returns the index of the given scene in the animation project.
     * Returns -1 if no item matched.
     * @return the index of the given scene.
     */
    int getSceneIndex(Scene* scene);

    /**
     * Sets the scene with with the pointer newScene as the active scene.
     * @param newScene the pointer of the new active scene.
     */
    void setActiveScene(Scene *newScene);

    /**
     * Sets the scene with at position sceneIndex as the active scene.
     * @param sceneIndex the index of the new active scene.
     */
    void setActiveSceneIndex(int sceneIndex);

    /**
     * Overloaded function for convenience. Assumes the scene to retrieve there take.
     * @return the active scene.
     */
    Scene* getActiveScene();

    /**
     * Retrieves the number of scenes in the animation.
     * @return the number of scenes in the animation.
     */
    int getSceneSize() const;

    /**
     * Returns the take size of the scene.
     * @arg sceneIndex index of the scene the take size to get.
     * @return the take size of the scene.
     */
    int getSceneTakeSize(int sceneIndex) const;

    /**
     * Returns the exposure size of the scene.
     * @arg sceneIndex index of the scene the exposure size to get.
     * @return the exposure size of the scene.
     */
    int getSceneExposureSize(int sceneIndex) const;

    /**
     * Returns the exposure size of the take of the scene.
     * @arg sceneIndex index of the scene of the take the exposure size to get.
     * @arg takeIndex index of the take the exposure size to get.
     * @return the exposure size of the take.
     */
    int getSceneTakeExposureSize(int sceneIndex, int takeIndex) const;

    /**
     * Retrieves the scene at index sceneIndex.
     * @param sceneIndex the index of the scene.
     * @return the scene.
     */
    Scene *getScene(int sceneIndex);

    /**
     * Create a new scene and adds the scene to the end of the animation.
     * @param sceneDescription the description of the scene or a empty string.
     */
     void addScene(const QString &sceneDescription);

     /**
      * Add a existing scene to the end of the animation.
      * @param scene The existing scene.
      */
      void addScene(Scene *scene);

    /**
     * Create a new scene and insert the scene bevor the active scene in the animation.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     * @param sceneDescription The description of the scene or a empty string.
     */
    void insertScene(int sceneIndex, const QString &sceneDescription);

    /**
     * Insert a existing scene bevor the active scene in the animation.
     * @param sceneIndex The index of the scene where the new scene is inserted bevor.
     * @param scene The existing scene.
     */
    void insertScene(int sceneIndex, Scene *scene);

    /**
     * Moves the scene at position sceneIndex to the position movePosition.
     * @param sceneIndex the index of the scene to move.
     * @param movePosition the position to move the scene to.
     */
    void moveScene(int sceneIndex, int movePosition);

    /**
     * Removes the active scene from the animation.
     */
    // void removeActiveScene();

    /**
     * Removes the active scene from the animation.
     * @param sceneIndex The index of the scene to remove.
     * @return The removed scene.
     */
    Scene *removeScene(int sceneIndex);

    /**
     * Get the data of the scenes from the project file
     */
    bool readScenesFromProject(QDomElement &animationNode);

    /**
     * Set the data of the scenes to the project file
     */
    bool saveScenesToProject(QDomDocument &doc, QDomElement &animationNode);

    /**
     * Set the data of the scenes to the project file
     */
    bool saveAsScenesToProject(QDomDocument &doc, QDomElement &animationNode);

    /**************************************************************************
     * Sound functions
     **************************************************************************/

    /**
     * Adds the sound from the file "sound" to scene at position sceneIndex.
     * @param sceneIndex the index of the scene to add the sound to
     * @param filename the path to the file with the sound
     * @param soundName the new name of the sound.
     * @return zero on success, less than zero on failure;
     * -1 = file is not readable
     * -2 = not a valid audio file
     */
    int addSoundToScene(unsigned int sceneIndex, const QString &filename, const QString &soundName);

    /**
     * Removes the sound with index soundIndex from the scene with index
     * sceneIndex.
     * @param sceneIndex the index of the scene to remove a sound from.
     * @param soundIndex the index of the sound to remove from the frame.
     */
    void removeSoundFromScene(unsigned int sceneIndex, unsigned int soundIndex);

    /**
     * Plays the sound of the scene with index sceneIndex.
     * @param sceneIndex the index of the scene to play.
     */
    void playSound(int sceneIndex);

    /**************************************************************************
     * Take functions
     **************************************************************************/

    /**
     * Retrieving the index of the currently active take.
     * @return the index of the active take.
     */
    int getActiveTakeIndex() const;

    /**
     * Sets the take with at position takeIndex as the active take.
     * @param takeIndex the index of the new active take.
     */
    void setActiveTakeIndex(int takeIndex);

    /**
     * Overloaded function for convenience. Assumes the scene to retrieve there
     * take from is the active scene.
     * @return the active take in the active scene.
     */
    Take *getActiveTake();

    /**
     * Create a new take and add the take as the last to the scene.
     * @param sceneIndex Index of the scene the take is added to.
     * @param takeDescription the description of the take or a empty string.
     */
    void addTake(int sceneIndex,
                 const QString &takeDescription);

    /**
     * Add an existing take as the last to the scene.
     * @param sceneIndex Index of the scene the take is added to.
     * @param take The existing take object.
     */
    void addTake(int sceneIndex,
                 Take *take);

    /**
     * Create a new take and insert the take bevor the active scene.
     * @param sceneIndex Index of the scene the take is added to.
     * @param takeIndex The index of the take to insert the new take before.
     * @param takeDescription the description of the take or a empty string.
     */
    void insertTake(int sceneIndex,
                    int takeIndex,
                    const QString &takeDescription);

    /**
     * Insert an existing take and insert it bevor the active scene.
     * @param sceneIndex Index of the scene the take is added to.
     * @param takeIndex The index of the take to insert the new take before.
     * @param take The existing take object.
     */
    void insertTake(int sceneIndex,
                    int takeIndex,
                    Take *take);

    /**
     * Moves the take at position takeIndex to the position movePosition.
     * @param fromSceneIndex the index of the scene of the take to move.
     * @param fromTakeIndex the index of the take to move.
     * @param toSceneIndex the scene position to move the take to.
     * @param toTakeIndex the position to move the take to.
     */
    void moveTake(int fromSceneIndex, int fromTakeIndex,
                  int toSceneIndex, int toTakeIndex);

    /**
     * Removes the active take from the animation.
     * @return The removed take;
     */
    // Take *removeActiveTake();

    /**
     * Removes the active take from the animation.
     * @param sceneIndex The index of the scene of the take to remove.
     * @param takeIndex The index of the take to remove.
     * @return The removed take;
     */
    Take *removeTake(int sceneIndex, int takeIndex);

    /**
     * Returns the exposure size of the take.
     * @param sceneIndex The index of the scene.
     * @param takeIndex The index of the take.
     * @return The exposure size of the take.
     */
    int getTakeExposureSize(unsigned int sceneIndex, unsigned int takeIndex) const;

    /**
     * Retrieves the take at index takeIndex.
     * @param sceneIndex the index of the scene.
     * @param takeIndex the index of the take.
     * @return the take.
     */
    Take *getTake(int sceneIndex, int takeIndex);

    /**************************************************************************
     * Exposure functions
     **************************************************************************/

    /**
     * Function for changing the currently active exposure.
     * @param exposureIndex the index of the new active exposure.
     */
    void setActiveExposureIndex(int exposureIndex);

    /**
     * Returns the total count of exposures of the model.
     * @return the count of the model.
     */
    unsigned int getTotalExposureSize() const;

    /**
     * Retrieving the index of the currently active exposure.
     * @return the index of the active exposure.
     */
    int getActiveExposureIndex() const;

    /**
     * Return the exposure with the given scene index, take index and exposure index.
     * @param sceneIndex the index of the scene to retrieve the exposure from.
     * @param takeIndex the index of the take to retrieve the exposure from.
     * @param exposureIndex the index of the exposure to retrieve
     * @return the exposure to retrieve.
     */
    Exposure *getExposure(unsigned int sceneIndex,
                          unsigned int takeIndex,
                          unsigned int exposureIndex);

    /**
     * Overloaded function for convenience. Assumes the scene to retrieve there
     * exposure from is the active scene and active take.
     * @return the active exposure in the active scene and active take.
     */
    Exposure *getActiveExposure();

    /**
     * Create a new exposure and add it to end of the vector containing
     * the exposures.
     * @param sceneIndex the index of the scene with the exposure to add
     * @param takeIndex the index of the take with the exposure to add
     * @param fileName the name of the image file.
     * @param location the location of the picture
     */
    void addExposure(int sceneIndex,
                     int takeIndex,
                     const QString &fileName,
                     int location);

    /**
     * Add a existing exposure in the vector containing the exposures.
     * @param sceneIndex the index of the scene with the exposure to add
     * @param takeIndex the index of the take with the exposure to add
     * @param exposure The existing exposure to add.
     */
    void addExposure(int sceneIndex,
                     int takeIndex,
                     Exposure *exposure);

    /**
     * Create a new exposure and insert it in the vector containing the exposures.
     * @param sceneIndex the index of the scene with the exposure to insert
     * @param takeIndex the index of the take with the exposure to insert
     * @param exposureIndex the index of the new exposure where inserted bevor.
     * @param fileName the name to the image file.
     * @param location the location of the picture
     */
     void insertExposure(int sceneIndex,
                         int takeIndex,
                         int exposureIndex,
                         const QString &fileName,
                         int location);

     /**
      * Inser an existing exposure in the vector containing the exposures.
      * @param sceneIndex the index of the scene with the exposure to insert
      * @param takeIndex the index of the take with the exposure to insert
      * @param exposureIndex the index of the new exposure where inserted bevor.
      * @param exposure The existing exposure.
      */
      void insertExposure(int sceneIndex,
                          int takeIndex,
                          int exposureIndex,
                          Exposure *exposure);

    /**
     * Removes the active exposure from the animation.
     * @return The removed exposure or NULL if nothing is done
     */
    Exposure *removeActiveExposure();

    /**
     * Removes the exposure with the given tree item from the animation.
     * @param sceneIndex the index of the scene with the exposure to remove
     * @param takeIndex the index of the take with the exposure to remove
     * @param exposureIndex the index of the exposure to remove
     * @return The removed exposure or NULL if nothing is done
     */
    Exposure *removeExposure(int sceneIndex,
                             int takeIndex,
                             int exposureIndex);

    /**
     * Get the next exposure index.
     * @return Next exposure inde.
     */
    unsigned int getNextTotalExposureIndex();

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

private:

    /**
     * Frontend of the application.
     */
    Frontend *frontend;

    /**
     * Description of the animation.
     */
    QString description;

    /**
     * Serializer to be used on saving and loading of the project.
     */
    ProjectSerializer *serializer;

    /**
     * Active recording mode of the project.
     */
    int recordingMode;

    /**
     * Active video source of the project.
     */
    int videoSource;

    /**
     * Active video resolution of the project.
     */
    int videoResolution;

    /**
     * Active mixing mode of the project.
     */
    int mixMode;

    /**
     * Active mix count of the project.
     */
    int mixCount;

    /**
     * Active playback count of the project.
     */
    int playbackCount;

    /**
     * Active encoder application of the project.
     */
    int encoderApplication;

    /**
     * Active grabber source of the project.
     */
    int grabberSource;

    /**
     * Active image format of the project.
     */
    int imageFormat;

    /**
     * Active image quality of the project.
     */
    int imageQuality;

    /**
     * Active image size of the project.
     */
    int imageSize;

    /**
     * Active image transformation mode
     */
    int imageTransformation;

    /**
     * Active image adjustment of the project.
     */
    int imageAdjustment;

    /**
     * Active zoom value of the project.
     */
    int zoomValue;

    /**
     * Active live view fps of the project.
     */
    int liveViewFps;

    /**
     * Active video format of the project.
     */
    int videoFormat;

    /**
     * Active video size of the project.
     */
    int videoSize;

    /**
     * Active video frames per second of the project.
     */
    int videoFps;

    /**
     * Active use default output file flag of the project.
     */
    bool useDefaultOutputFile;

    /**
     * Active default output file name of the project.
     */
    QString defaultOutputFileName;

    /**
     * Active unit mode of the project.
     */
    int unitMode;

    /**
     * All of the scenes in the animation.
     */
    QVector<Scene*> scenes;

    /**
     * Index of the active scene.
     */
    int activeSceneIndex;

    /**
     * Next scene index to use to create a dummy description string.
     */
    unsigned int nextSceneIndex;

    /**
     * Index of the active scene.
     */
    unsigned int nextTotalExposureIndex;

    /**
     * Number of unsaved changes in the settings
     */
    int settingsChanges;

    /**
     * Number of unsaved changes in the animation
     */
    int animationChanges;

    /**
     * Number of sounds added to the model.
     */
    int soundsNumber;

    /**
     * Audio driver that can play sound.
     */
    AudioDriver *audioDriver;

    /**
     * Variable for checking if the audio device is successfully initialized.
     */
    bool isAudioDriverInitialized;

};

#endif
