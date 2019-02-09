/******************************************************************************
 *  Copyright (C) 2005-2018 by                                                *
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

#include <QFile>
#include <QObject>
#include <QString>
#include <QVector>
#include <QTreeWidget>
#include <QUndoStack>

#include "domain/animation/animationproject.h"
#include "frontends/frontend.h"

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
     * @brief Enum with all possible recording modes
     */
    enum recordingMode {
        singleFrameMode,    // Singe frame capture
        timeLapseMode,      // Time lapse automatic recording
        lastRecordingMode   // Last mode marker for asserts
    };

    /**
     * @brief Default recording mode for new installations
     */
    const static int  RECORDINGMODEDEFAULT = singleFrameMode;

    /**
     * @brief Default grabber source index for new installations
     */
    const static int  GRABBERSOURCEDEFAULT = 0;

    /**
     * @brief Enum with all possible project mix modes
     */
    enum imageMixMode {
        mixImageMode,       // Mix the actual image with the last mixCount images
        diffImageMode,      // Differentiate the actual image with the last image
        lastImageMixMode    // Last mode marker for asserts
    };

    /**
     * @brief Default mix mode for new installations
     */
    const static int  MIXMODEDEFAULT = mixImageMode;

    /**
     * @brief Default mix count for new installations
     */
    const static int  MIXCOUNTDEFAULT = 2;

    /**
     * @brief Enum with all possible project mix modes
     */
    enum unitMode {
        secondsMode,        // Seconds are the units for time lapse intervall
        minutesMode,        // Minutes are the units for time lapse intervall
        hoursMode,          // Hours are the units for time lapse intervall
        daysMode,           // Minutes are the units for time lapse intervall
        lastUnitMode        // Last mode marker for asserts
    };

    /**
     * @brief Default unit mode for new installations
     */
    const static int  UNITMODEDEFAULT = minutesMode;

    /**
     * @brief Default unit count for new installations
     */
    const static int  UNITCOUNTDEFAULT = 5;

    /**
     * @brief Default beep check box state for new installations
     */
    const static bool BEEPCHECKDEFAULT = true;

    /**
     * @brief Default beep count for new installations
     */
    const static int  BEEPCOUNTDEFAULT = 5;

    /**
     * @brief Enum with all possible image output formats
     */
    enum imageFormat {
        jpegFormat,            // JPEG format
        tiffFormat,            // TIFF format
        bmpFormat,             // BMP format
        lastImageFormat        // Last format marker for asserts
    };

    /**
     * @brief Default image format for new installations
     */
    const static int    IMAGEFORMATDEFAULT = jpegFormat;

    /**
     * @brief Default image quality for new installations
     */
    const static int    IMAGEQUALITYDEFAULT = 100;

    /**
     * @brief Enum with all possible image sizes
     */
    enum imageSize {
        defaultImageSize,     // Source size
        qvgaImageSize,        // 320x240 (QVGA)
        vgaImageSize,         // 640x480 (VGA)
        svgaImageSize,        // 800x600 (SVGA)
        paldImageSize,        // 704x576 (PAL D)
        hdreadyImageSize,     // 1280x720 (HD Ready)
        fullhdImageSize,      // 1900x1080 (Full HD)
        lastImageImageSize    // Last size marker for asserts
    };

    /**
     * @brief Default image size for new installations
     */
    const static int    IMAGESIZEDEFAULT = defaultImageSize;

    /**
     * @brief Enum with all possible image transformation modes
     */
    enum ImageTransformationMode {
        ScaleImage,      // Scale the image to the output size
        ClipImage,       // Clip the image to the output size according to the adjustment setting
        ZoomImage,       // Zoom the image to the output size according to the zoom setting
    };

    /**
     * @brief Default image transformation mode for new installations
     */
    const static int IMAGETRANSFORMDEFAULT = ScaleImage;

    /**
     * @brief Enum with all possible image adjustments
     */
    enum imageAdjustment {
        leftUp,               // Left up corner
        centerUp,             // Center up
        rightUp,              // Right up corner
        leftMiddle,           // Left middle
        centerMiddle,         // Center middle
        rightMiddle,          // Right middle
        leftDown,             // Left down corner
        centerDown,           // Center down
        rightDown,            // Right down corner
        lastImageAdjustment   // Last adjustment marker for asserts
    };

    /**
     * @brief Default image adgustment for new installations
     */
    const static int IMAGEADJUSTMENTDEFAULT = centerDown;

    /**
     * @brief Default image zoom value for new installations
     */
    const static double ZOOMVALUEDEFAULT;

    /**
     * @brief Default live view FPS for new installations
     */
    const static double LIVEVIEWFPSDEFAULT;

    /**
     * @brief Enum with all possible encoder applications.
     */
    enum encoderApplication {
        ffmpegApplication,
        libavApplication,
        noneEncoderApplication
    };

    /**
     * @brief Default video encoder application for new installations
     */
    const static int  ENCODERAPPLICATIONDEFAULT = noneEncoderApplication;

    /**
     * @brief Enum with all possible video output formats
     */
    enum videoFormat {
        aviFormat,            // AVI format
        mp4Format,            // MP4 format
        noneFormat
    };

    /**
     * @brief Default video format for new installations
     */
    const static int  VIDEOFORMATDEFAULT = mp4Format;

    /**
     * @brief Enum with all possible video sizes
     */
    enum videoSize {
        defaultVideoSize,     // Source size
        qvgaVideoSize,        // 320x240 (QVGA)
        vgaVideoSize,         // 640x480 (VGA)
        svgaVideoSize,        // 800x600 (SVGA)
        paldVideoSize,        // 704x576 (PAL D)
        hdreadyVideoSize,     // 1280x720 (HD Ready)
        fullhdVideoSize       // 1900x1080 (Full HD)
    };

    /**
     * @brief Default video size for new installations
     */
    const static int  VIDEOSIZEDEFAULT = defaultVideoSize;

    /**
     * @brief Default video frames per second for new instalations
     */
    const static int  VIDEOFPSDEFAULT = 12;

    /**
     * @brief Enum with all possible video output formats
     */
    enum movieSplittingMode {
        exportAsWholeMovie,        // All scenes and takes are united in one movie
        exportAsSplittedOnScenes, // Movie is splitted up on several files - separate for each per scene
        exportAsSplittedOnTakes   // Movie is splitted up on multiple files - separate for each per take
    };

    /**
     * @brief Default splitting mode for new installations
     */
    const static int  MOVIESPLITTINGMODEDEFAULT = exportAsWholeMovie;

    /**
     * @brief Default usage of the default output file name for new installations
     */
    const static bool USEDEFAULTOUTPUTFILENAMEDEFAULT = false;

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

    /**
     * Is the project in recovering mode
     */
    bool isRecovering();

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
     * @return the project file path if it's set, NULL otherwise.
     */
    const QString getNewProjectFilePath();

    /**
     * Retrieves the new project path.
     * @return the project path if it's set, NULL otherwise.
     */
    const QString getNewProjectPath();

    /**
     * Retrieves the project description.
     * @return the project description if it's set, NULL otherwise.
     */
    const QString getProjectDescription();

    /**************************************************************************
     * Project preferences
     **************************************************************************/

    /**
     * Get the active recording mode of the project.
     * @return active recording mode.
     */
    int getRecordingMode();

    /**
     * Set the active recording mode of the project.
     * @param newRecordingMode The new recording mode.
     */
    void setRecordingMode(int newRecordingMode);

    /**
     * Get the unit mode.
     * @return The unit mode.
     */
    int getUnitMode();

    /**
     * Set the unit mode.
     * @param mode The new unit mode.
     */
    void setUnitMode(int mode);

    /**
     * Get the unit count.
     * @return The unit count.
     */
    int getUnitCount();

    /**
     * Set the unit count.
     * @param count The new unit count.
     */
    void setUnitCount(int count);

    /**
     * Get the beep checkbox state.
     * @brief getBeepState
     * @return The actual beep checkbox state
     */
    bool getBeepState();

    /**
     * Set the beep checkbox state.
     * @brief setBeepState
     * @param state The new beep checkbox state.
     */
    void setBeepState(bool state);

    /**
     * Get the beep unit count.
     * @return The beep unit count.
     */
    int getBeepCount();

    /**
     * Set the beep unit count.
     * @param count The new beep unit count.
     */
    void setBeepCount(int count);

    /**
     * Get the active video source of the project.
     * @return active video source.
     */
    int getVideoSource();

    /**
     * Set the active video source of the project.
     * @param newVideoSource the new active source.
     */
    void setVideoSource(int newVideoSource);

    /**
     * Get the active video resolution of the project.
     * @return active video resolution.
     */
    int getResolution();

    /**
     * Set the active video resolution of the project.
     * @param newVideoSource the new active resolution.
     */
    void setResolution(int newResolution);

    /**
     * Convert all images of the current project to a new image file format.
     * @param newFormat The new format of the images.
     * @param newQuality The new quality value of the images.
     * @return true if all images are converted to the new image format.
     */
    bool convertImages(int newFormat, int newQuality);

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
     * Get the overlay intensity value.
     * @return The overlay intensity value.
     */
    int getOverlayIntensity();

    /**
     * Set the overlay intensity value of the project.
     * @param overlay The new overlay intensity value.
     */
    void setOverlayIntensity(int overlay);

    /**************************************************************************
     * Image import preferences
     **************************************************************************/

    /**
     * Get the image format of the project.
     * @return image format.
     */
    int getImageFormat();

    /**
     * Set the image format of the project.
     * @param newVF The new image format.
     */
    void setImageFormat(int newVF);

    /**
     * Get the image quality of the project.
     * @return image quality value.
     */
    int getImageQuality();

    /**
     * Set the image quality of the project.
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
    double getZoomValue();

    /**
     * Set the zoom value of the project.
     * @param newZV The new zoom value.
     */
    void setZoomValue(double newZV);

    /**
     * Get the live view fps of the project.
     * @return live view fps value.
     */
    double getLiveViewFps();

    /**
     * Set the live view fps of the project.
     * @param newValue The new live view fps value.
     */
    void setLiveViewFps(double newValue);

    /**************************************************************************
     * Video export preferences
     **************************************************************************/

    /**
     * Get the encoder application of the project.
     * @return Encoder application.
     */
    int getEncoderApplication();

    /**
     * Set the encoder application of the project.
     * @param newEA The new encoder application.
     */
    void setEncoderApplication(int newEA);

    /**
     * Get the video format of the project.
     * @return Video format.
     */
    int getVideoFormat();

    /**
     * Set the video format of the project.
     * @param newVF The new video format.
     */
    void setVideoFormat(int newVF);

    /**
     * Get the video size of the project.
     * @return Video size.
     */
    int getVideoSize();

    /**
     * Set the video size of the project.
     * @param newVS The new video size.
     */
    void setVideoSize(int newVS);

    /**
     * Get the video frames per second of the project.
     * @return The video frames per second.
     */
    int getVideoFps();

    /**
     * Set the video frames per second of the project.
     * @param newFPS The new video frames per second.
     */
    void setVideoFps(int newFPS);

    /**
     * Get the splitting mode during movie export.
     * @return The splitting mode.
     */
    int getMovieExportSplittingMode();

    /**
     * Set the splitting mode during movie export.
     * @param newSplittingMode The splitting mode.
     */
    void setMovieExportSplittingMode(int newSplittingMode);

    /**
     * Get the use default output file flag of the project.
     * @return Use default output file flag.
     */
    bool getUseDefaultOutputFile();

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
     * Save the project.
     * @param projectPath the path to the project file.
     * @param saveAs this is a save as operation.
     */
    void saveProject(const QString &projectPath, bool saveAs);

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
     * @arg sceneIndex index of the scene the exposure size to get.
     * @return the exposure size of the scene.
     */
    int getSceneExposureSize(int sceneIndex);

    /**
     * Returns the exposure size of the take of the scene.
     * @arg sceneIndex index of the scene of the take the exposure size to get.
     * @arg takeIndex index of the take the exposure size to get.
     * @return the exposure size of the scene.
     */
    int getSceneTakeExposureSize(int sceneIndex, int takeIndex);

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
     * @param filePath the file path to the image file of the exposure.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     */
    void addExposureToUndo(const QString &filePath,
                           int            sceneIndex,
                           int            takeIndex);

    /**
     * Creates a new addExposure object in the undo history.
     * Save the image to the temp path.
     * @param rawImage the image of the exposure.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     */
    void addExposureToUndo(const QImage &rawImage,
                           int           sceneIndex,
                           int           takeIndex);

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
     * Creates a new insertExposure object in the undo history.
     * Save the image to the temp path.
     * @param rawImage the image of the exposure or a empty string.
     * @param sceneIndex the index of the scene of the take the exposure to add
     * @param takeIndex the index of the take the exposure to add
     * @param exposureIndex The index of the exposure where the new exposures are inserted bevor.
     */
    void insertExposureToUndo(const QImage &rawImage,
                              int           sceneIndex,
                              int           takeIndex,
                              int           exposureIndex);

    /**
     * Add a new remove exposure object to the undo history.
     * @param sceneIndex the index of the scene of the exposure to remove
     * @param takeIndex the index of the take of the exposure to remove
     * @param exposureIndex the index of the exposure to remove
     */
    void removeExposureToUndo(int  sceneIndex,
                              int  takeIndex,
                              int  exposureIndex);

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
     * @param rawImage the image to copy.
     * @return the file name of the new file.
     */
    const QString copyToTemp(const QImage &rawImage);

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
     * Flag to indicate that recovery is running.
     */
    bool recovering;

    /**
     * Container where one can register and retrieve undo objects for undo and
     * remove operations.
     */
    QUndoStack *undoStack;

};

#endif
