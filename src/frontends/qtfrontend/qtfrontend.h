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

#ifndef QTFRONTEND_H
#define QTFRONTEND_H

#include "frontends/frontend.h"
#include "frontends/qtfrontend/mainwindowgui.h"

#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtGui/QApplication>
#include <QtGui/QLabel>


struct AudioFile {
    unsigned int belongsTo;
    QString filename;
};


/**
 * Frontend for using the program through a GUI developed with the QT library.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
*/
class QtFrontend : public QObject, public Frontend
{
    Q_OBJECT
public:

    /**
     * Initializing the frontend.
     * @param argc the argc argument from the environment through main
     * @param argv argv the argv arguments from the environment through main
     */
    QtFrontend(int &argc, char **argv);

    /**
     * Deallocates allocated memory and sets the pointers to NULL.
     */
    virtual ~QtFrontend();

    /**
     * Check the existance an writability of the application directory.
     * @param binDirName name of the binary directory
     */
    bool checkApplicationDirectory(char *binDirName);

    /**
     * Initialize the frontend.
     */
    void init();

    /**
     * Check for recovery after a crash.
     * Open a project given in the arguments.
     * @return true if a project file is opened.
     */
    bool handleArguments(int argc, char **argv);

    /**
     * The run function for starting the application.
     * @param argc the argc argument from the environment through main.
     * @param argv the argv arguments from the environment through main.
     * @return the return status on exit
     */
    int run(int argc, char **argv);

    /**
     * Finalize the frontend.
     */
    void finalize();

    /**
     * Get the domain facade.
     * @return the domain facade.
     */
    DomainFacade* getProject();

    /**
     * Get the view facade.
     * @return the view facade.
     */
    ViewFacade* getView();

    /**
     * Function for geting the preferences tool.
     * @return the preferences tool.
     */
    PreferencesTool* getPreferences();

    /**
     * Getting the application directory
     * @return the application directory name string
     */
    const char* getUserDirName();

    /**
     * Getting the temporary directory
     * @return the temporary directory name string
     */
    const char* getTempDirName();

    /**
     * Function for getting the application directory
     * @return the application directory name string
     */
    const char* getApplicationDirName();

    /**
     * Function for getting the binary directory
     * @return the binary directory name string
     */
    const char* getBinDirName();

    /**
     * Function for getting the manual directory
     * @return the manual directory name string
     */
    const char* getManualDirName();

    /**
     * Function for getting the translations directory
     * @return the translations directory name string
     */
    const char* getTranslationsDirName();

    /**
     * Function for getting the graphics directory
     * @return the graphics directory name string
     */
    const char* getGraphicsDirName();

    /**
     * Function for getting the icons directory
     * @return the icons directory name string
     */
    const char* getIconsDirName();

    /**
     * Function for getting the pictures directory
     * @return the pictures directory name string
     */
    const char* getPicturesDirName();

    /**
     * Getting all possible GUI languages
     * @return A vector with all languages
     */
    const QVector<QString> getLanguages();

    /**
     * Getting all possible GUI locales
     * @return A vector with all locales
     */
    const QVector<QString> getLocales();

    /**
     * A new language is selected in the preferences menu.
     * @param newIndex New selected index.
     */
    void changeLanguage(int newIndex);

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    void changeCaptureButtonFunction(int newFunction);

    /**
     * Get the status of the vertical lines of the grid.
     * @return The new state of the grid.
     */
    bool getVerticalGrid();

    /**
     * Set the status of the vertical lines of the grid.
     * @param newState The new state of the grid.
     */
    void setVerticalGrid(bool newState);

    /**
     * Get the number of vertical lines of the grid.
     * @return The new number of lines.
     */
    int getVerticalSpin();

    /**
     * Set the number of vertical lines of the grid.
     * @param newSpin The new number of lines.
     */
    void setVerticalSpin(int newSpin);

    /**
     * Get the status of the horizontal lines of the grid.
     * @return The new state of the grid.
     */
    bool getHorizontalGrid();

    /**
     * Set the status of the horizontal lines of the grid.
     * @param newState The new state of the grid.
     */
    void setHorizontalGrid(bool newState);

    /**
     * Get the number of horizontal lines of the grid.
     * @return The new number of lines.
     */
    int getHorizontalSpin();

    /**
     * Set the number of horizontal lines of the grid.
     * @param newSpin The new number of lines.
     */
    void setHorizontalSpin(int newSpin);

    /**
     * Get the RGB value of the color of the grid lines.
     * @param r The red component of the color's RGB value.
     * @param g The green component of the color's RGB value.
     * @param b The blue component of the color's RGB value.
     * @param a The alpha-channel component of the color's RGB value.
     */
    void getGridColorRGB(int *r, int *g, int *b, int *a);

    /**
     * Set the RGB value of the color of the grid lines.
     * @param r The red component of the color's RGB value.
     * @param g The green component of the color's RGB value.
     * @param b The blue component of the color's RGB value.
     * @param a The alpha-channel component of the color's RGB value.
     */
    void setGridColorRGB(int r, int g, int b, int a);

    /**
     * Function for displaying progress on timeconsuming operations.
     * @param infoText the text to display to the user
     * @param numOperations the number of calculated operations to do
     */
    void showProgress(const QString &infoText, unsigned int numOperations = 0);

    /**
     * Function for hiding the progress info.
     */
    void hideProgress();

    /**
     * Function for updating the progress.
     * @param numOperationsDone the number of operations done
     */
    void updateProgress(int numOperationsDone);

    /**
     * Function for increase the progress.
     */
    void increaseProgress();

    /**
     * Function for changing the information to display to the user
     * @param infoText the text to display to the user
     */
    void setProgressInfo(const char *infoText);

    /**
     * Display a message in the status bar.
     * @param message the message to display.
     * @param timeout the timeout for the message in mili-seconds.
     */
    void showMessage(const QString &message, int timeout = 0);

    /**
     * Removes any temporary message being shown in the status bar.
     */
    void clearMessage();

    /**
     * Function for checking if the user has aborted the operation
     * (eg pressed cancel)
     * @return true if the the operation is aborted, false otherwise
     */
    bool isOperationAborted();

    /**
     * Function for changing the project id in the status bar
     * @param id the id to display in the status bar
     */
    void setProjectID(const char *id);

    /**
     * Function for changing the scene id in the status bar
     * @param id the id to display in the status bar
     */
    void setSceneID(const char *id);

    /**
     * Function for changing the take id in the status bar
     * @param id the id to display in the status bar
     */
    void setTakeID(const char *id);

    /**
     * Function for changing the exposure id in the status bar
     * @param id the id to display in the status bar
     */
    void setExposureID(const char *id);

    /**
     * Function for processing Qt events. This is useful on timeconsuming
     * operations which aren't running in separate processes or threads.
     */
    void processEvents();

    /**
     * Function for asking the user a yes/no question.
     * @param title title of the message dialog
     * @param question the question to ask
     * @return 0 if the user answer yes, 1 if no
     */
    int askQuestion(const QString title, const QString &question);

    /**
     * Function for informating the user.
     * @param title title of the message dialog
     * @param info the information for the user
     */
    void showInformation(const QString title, const QString &info);

    /**
     * Function for warning the user.
     * @param title title of the message dialog
     * @param warning the warning for the user
     */
    void showWarning(const QString title, const QString &warning);

    /**
     * Function for signaling a critical state to the user.
     * @param title title of the message dialog
     * @param message the message for the user
     */
    void showCritical(const QString title, const QString &message);

    /**
     * Function to run a external command
     * @param command command to run
     * @return return value of the command
     */
    int runExternalCommand(const QString &command);

    /**
     * Turns on the webcamera/video import mode.
     */
    bool startGrabber();

    /**
     * Turns off the webcamera/video import mode.
     */
    void stopGrabber();

    /**
     * Get the possible video devices
     * @return Vector with the devices.
     */
    const QVector<ImageGrabberDevice*> getDevices();

    /**
     * Get one video devices.
     * @param deviceIndex The index of the device.
     * @return The video devices.
     */
    ImageGrabberDevice *getDevice(int deviceIndex);

    /**
     * Get the possible video devices.
     * @return Vector with the device names.
     */
    const QVector<QString> getDeviceNames();

    /**
     * Get the possible resolution names
     * of the video device.
     * @param deviceIndex The index of the device.
     * @return Vector with the resolution names.
     */
    const QVector<QString> getResolutionNames(int deviceIndex);

    /**
     * Get the live image from the image grabber.
     * @return live image.
     */
    const QImage getLiveImage();

    /**
     * Get the raw image from the image grabber.
     * @return raw image.
     */
    const QImage getRawImage();

    /**
     * Function for performing the redraw with the next frame of the animation.
     * @param exposureIndex Index of the next frame
     */
    void nextAnimationFrame(int exposureIndex);

    /**
     * Set a new state to the tool bar.
     * @param newState the new tool bar state.
     */
    void setToolBarState(int newState);

    /**
     * Get the recording mode.
     * @return The recording mode.
     */
    int getRecordingMode();

    /**
     * Set the recording mode.
     * @param mode The new recording mode.
     */
    void setRecordingMode(int mode);

    /**
     * Get the video source index.
     * @return video source index
     */
    int getVideoSource();

    /**
     * Set the video source index.
     * @param index Index of the new video source
     * @return true if the new index is set, false else.
     */
    bool setVideoSource(int index);

    /**
     * Get the video resolution index.
     * @return video resolution index
     */
    int getResolution();

    /**
     * Set the video resolution index.
     * @param index Index of the new video resolution
     * @return true if the new index is set, false else.
     */
    bool setResolution(int index);

    /**
     * Get the video resolution object.
     * @param index Index of the new video resolution
     * @return video resolution object
     */
    const GrabberResolution getResolution(int index);

    /**
     * Convert all images of the current project to a new image file format.
     * @param newFormat The new format of the images.
     * @param newQuality The new quality value of the images.
     * @return true if all images are converted to the new image format.
     */
    bool convertImages(int newFormat, int newQuality);

    /**
     * Get the mix mode.
     * @return The mix mode.
     */
    int getMixMode();

    /**
     * Set the mix mode.
     * @param mode The new mix mode
     */
    void setMixMode(int mode);

    /**
     * Get the mix count.
     * @return The mix count.
     */
    int getMixCount();

    /**
     * Set the mix count.
     * @param count The new mix count.
     */
    void setMixCount(int count);

    /**
     * Get the playback count.
     * @return The playback count.
     */
    int getPlaybackCount();

    /**
     * Set the playback count.
     * @param count The new playback count.
     */
    void setPlaybackCount(int count);

    /**
     * Sets up the ExternalChangeMonitor to monitor the project directories
     * for changes in the project files.
     */
    void setupDirectoryMonitoring();

    /**
     * Register the directory for external change monitoring.
     * @param directory the directory to listen for changes in.
     */
    void addDirectoryToMonitoring(const QString &directory);

    /**
     * Remove the directory from external change monitoring.
     * @param directory the directory to remove.
     */
    void removeDirectoryFromMonitoring(const QString &directory);

    /**
     * Remove all directories from external change monitoring.
     */
    void removeAllDirectoriesFromMonitoring();

    /**
     * Register the file for external change monitoring.
     * @param file the file to listen for changes in.
     */
    void addFileToMonitoring(const QString &file);

    /**
     * Remove the file from external change monitoring.
     * @param file the file to remove.
     */
    void removeFileFromMonitoring(const QString &file);

    /**
     * Remove all files from external change monitoring.
     */
    void removeAllFilesFromMonitoring();

    /**
     * Test function for debugging purposes
     */
    void test(QWidget *parentWidget, int level);

    /**
     * Test function for debugging purposes
     */
    void testMainWidget();

    /**
     * Remove the content of the given directory.
     * @param path the path to te directory.
     * @return true if the removing was successful.
     */
    bool removeContentInDirectory(const QString &path);

    /**
     * Create all temporary application directories (temp)
     */
    void makeApplicationDirectories();

    /**
     * Remove all temporary application directories (temp)
     */
    void removeApplicationDirectories();

    /**
     * Remove all temporary application files
     */
    void removeApplicationFiles();

    /**
     *
     */
    bool isRecoveryMode();

    /**
     * Recover the last changes after a crash.
     * @return true if an crashed project is recovered
     */
    bool recoverProject();

    /**
     * Open the start dialog
     */
    void startDialog();


protected slots:

private:
    QApplication    *stApp;
    MainWindowGUI   *mw;
    DomainFacade    *domainFacade;
    ViewFacade      *viewFacade;
    PreferencesTool *preferencesTool;
    QByteArray       appUserDirName;
    QByteArray       appTempDirName;
    QByteArray       appApplicationDirName;
    QByteArray       appBinDirName;
    QByteArray       appManualDirName;
    QByteArray       appTranslationsDirName;
    QByteArray       appGraphicsDirName;
    QByteArray       appIconsDirName;
    QByteArray       appPicturesDirName;

    void initializePreferences();
    void setDefaultPreferences();
};

#endif
