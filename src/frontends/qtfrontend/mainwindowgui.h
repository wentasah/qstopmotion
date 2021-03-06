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

#ifndef QTGUI_H
#define QTGUI_H

#include <QAction>
#include <QMenu>
#include <QMainWindow>
#include <QProgressBar>
#include <QProgressDialog>
#include <QUndoView>

#include "frontends/frontend.h"
#include "frontends/qtfrontend/menuframe.h"
#include "frontends/qtfrontend/toolbar.h"
#include "frontends/qtfrontend/dialogs/descriptiondialog.h"
#include "frontends/qtfrontend/dialogs/helpbrowser.h"
#include "frontends/qtfrontend/dialogs/cameracontrollerdialog.h"
#include "frontends/qtfrontend/elements/flexiblespinbox.h"
#include "frontends/qtfrontend/frameview/frameviewinterface.h"
#include "frontends/qtfrontend/preferences/generaldialog.h"
#include "frontends/qtfrontend/preferences/projectdialog.h"
#include "frontends/qtfrontend/timeline/timeline.h"
// #include "frontends/qtfrontend/tooltabs/compositingtab.h"
#include "frontends/qtfrontend/tooltabs/projecttab.h"
#include "frontends/qtfrontend/tooltabs/recordingtab.h"
// #include "frontends/qtfrontend/tooltabs/viewtab.h"
#include "technical/externalchangemonitor.h"
#include "technical/grabber/imagegrabberfacade.h"


/**
 *The main window class for the qstopmotion application.
 *This class sets up the main GUI and connects the buttons.
 *
 *@author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
*/
class MainWindowGUI : public QMainWindow
{
    Q_OBJECT
public:

    enum {SAVE, SAVE_AS, UNDO, REDO, CUT, COPY, PASTE, GOTO};

    /**************************************************************************
     * Public functions
     **************************************************************************/

    /**
     * Sets up the program main GUI and connect the widgets and handlers.
     * @param stApp the application class for changing translator
     * on runtime.
     */
    MainWindowGUI(QApplication *stApp, Frontend *f);

    /**
     * Cleans up after the mainwindowgui.
     */
    ~MainWindowGUI();

    /**
     * Retranslates the strings.
     *
     * This function is called after a new translator has been installed so that
     * the program strings are retranslated to the new language.
     */
    void retranslateStrings();

    /**
     * Initializes the main window and fills it with starting values.
     */
    void initialize();

    /**
     * Overloaded mouse listener. Closes the embedded menues when the user
     * clicks inside the main window.
     * @param e information about the event.
     */
    void mousePressEvent(QMouseEvent * e);

    /**
     * Overloaded event listener which recieves information when a keyboard key is
     * pressed.
     * @param k information about the key event.
     */
    void keyPressEvent(QKeyEvent *k);

    /**
     * Getting the active locale
     * @return A string with the active locale
     */
    const QString getActiveLocale();

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
     * Getting all possible GUI styles
     * @return A vector with all styles
     */
    static const QVector<QString> getStyles(Frontend* f);

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    void changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction);

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
     * Get the status of the signal feature.
     * @return The new state of the signal feature.
     */
    bool getSignal();

    /**
     * Set the status of the signal feature.
     * @param newState The new state of the signal feature.
     */
    void setSignal(bool newState);

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
    void setProgressInfo(const QString infoText);

    /**
     * Function for checking if the user has aborted the operation
     * (eg pressed cancel)
     * @return true if the the operation is aborted, false otherwise
     */
    bool isOperationAborted();

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
     * Set the project ID in the status bar
     * @param id The new project id:
     */
    void setProjectID(const QString &id);

    /**
     * Set the scene ID in the status bar
     * @param id The new scene id:
     */
    void setSceneID(const QString &id);

    /**
     * Set the take ID in the status bar
     * @param id The new take id:
     */
    void setTakeID(const QString &id);

    /**
     * Set the exposure ID in the status bar
     * @param id The new exposure id:
     */
    void setExposureID(const QString &id);

    /**
     * Open the start dialog
     */
    void startDialog();

    /**
     * Turns on the webcamera/video import mode.
     */
    bool startGrabber();

    /**
     * Checks if the grabber is inited.
     * @return true if it is inited, false otherwise
     */
    bool isGrabberInited();

    /**
     * Turns off the webcamera/video import mode.
     */
    void stopGrabber();

    /**
     * Get grabber controller.
     * @return The controller of the current grabber
     */
    GrabberController *getGrabberController();

    /**
     * Get the possible video devices.
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
     * Opens a saved project.
     * @param projectFile the project to open
     */
    void openProject(const QString &projectFile);

    /**
     * Set a new state to the tool bar.
     * @param newState the new tool bar state.
     */
    void setToolBarState(int newState);

    /**
     * Get the recording mode.
     * @return The recording mode.
     */
    enum DomainFacade::recordingMode getRecordingMode();

    /**
     * Set the recording mode.
     * @param mode The new recording mode.
     */
    void setRecordingMode(int mode);

    /**
     * Get the unit mode.
     * @return The unit mode.
     */
    virtual int getUnitMode();

    /**
     * Set the unit mode.
     * @param mode The new unit mode.
     */
    virtual void setUnitMode(int mode);

    /**
     * Get the unit count.
     * @return The unit count.
     */
    virtual int getUnitCount();

    /**
     * Set the unit count.
     * @param count The new unit count.
     */
    virtual void setUnitCount(int count);

    /**
     * Get the beep checkbox state.
     * @brief getBeepState
     * @return The actual beep checkbox state
     */
    virtual bool getBeepState();

    /**
     * Set the beep checkbox state.
     * @brief setBeepState
     * @param state The new beep checkbox state.
     */
    virtual void setBeepState(bool state);

    /**
     * Get the beep unit count.
     * @return The beep unit count.
     */
    virtual int getBeepCount();

    /**
     * Set the beep unit count.
     * @param count The new beep unit count.
     */
    virtual void setBeepCount(int count);

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
     * Get the overlay intensity value.
     * @return The overlay intensity value.
     */
    int getOverlayIntensity();

    /**
     * Set the overlay intensity value of the project.
     * @param overlay The new overlay intensity value.
     */
    void setOverlayIntensity(int overlay);

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
     * Open the online help in the default internet browser
     * @param anchor The anchor in the manual to go to after the manual is opend
     */
    void openOnlineHelp(const QString &anchor);

    /**************************************************************************
     * Public slots
     **************************************************************************/

public slots:

    /**************************************************************************
     * Private slots
     **************************************************************************/

private slots:

    /**
     * Creates a new project.
     */
    void newProject();

    /**
     * Opens a project.
     */
    void openProject();

    /**
     * Opens the last used projects.
     */
    void openMostRecent();
    void openSecondMostRecent();
    void openThirdMostRecent();
    void openFourthMostRecent();

    /**
     * Saves the project to the last saved file.
     */
    void saveProject();

    /**
     * Saves the project to a given filename from the user.
     */
    void saveProjectAs();

    /**
     * Exports the current project to a video file choosen by the user. It uses the
     * registered video encoder -- if registered -- to export.
     */
    void exportToVideo();

    /**
     * Exports the current project to a valid cinelerra-cv project.
     */
    void exportToCinelerra();

    /**
     * Close the application.
     */
    void closeApplication();

    /**
     * Copies a selection to the global clipboard.
     */
    void copy();

    /**
     * Pastes a selection from the global clipboard to the program.
     */
    void paste();

    /**
     * Attatches a sound to a frame in the animation.
     */
    void addSound();

    /**
     * Removes a sound from a frame in the animation.
     */
    void removeSound();

    /**
     * Brings up a preferences dialog where the user can set
     * preferences for the application.
     */
    void showPreferencesDialog();

    /**
     * Brings up a properties dialog where the user can set
     * properties for the project.
     */
    void showPropertiesDialog();

    /**
     * Show the undo view window.
     */
    void showUndoStack();

    /**
     * Brings up an help dialog with the qstopmotion user manua.
     */
    void showCameraControllerDialog();

    /**
     * Show the tool tip entries for user interface elements
     */
    void whatsThis();

    /**
     * Brings up an help dialog with the qstopmotion user manual.
     */
    void showHelpDialog();

    /**
     * Brings up the browser with the qstopmotion user manual in different languages.
     */
    void showOnlineHelp();

    /**
     * Brings up the browser with the qstopmotion support us page.
     */
    void showSupportUs();

    /**
     * Brings up an about dialog with information about the application.
     */
    void showAboutDialog();

protected:
    void closeEvent(QCloseEvent* event);

signals:
    /**
     * This signal is emitted when in the start dialog a new project is selected.
     */
    void startNewProject();

    /**
     * This signal is emitted when in the start dialog the last project is selected.
     */
    void startLastProject();

    /**
     * This signal is emitted when in the start dialog open project is selected.
     */
    void startOpenProject();

    /**
     * This signal is emitted when in the start dialog exit is selected.
     */
    void startExit();

public:

private:
    QApplication       *stApp;
    Frontend           *frontend;
    QWidget            *centerWidget;
    QVBoxLayout        *centerWidgetLayout;
    QWidget            *workArea;
    QHBoxLayout        *workAreaLayout;
    QWidget            *viewArea;
    QVBoxLayout        *viewAreaLayout;
    TimeLine           *timeLine;
    FrameViewInterface *frameView;
    ToolBar            *toolBar;

    // Actions
    // File menu
    QAction *newAct;
    QAction *openAct;
    QAction *mostRecentAct;
    QAction *secondMostRecentAct;
    QAction *thirdMostRecentAct;
    QAction *fourthMostRecentAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *videoAct;
    QAction *cinelerraAct;
    QAction *propertiesAct;
    QAction *quitAct;
    // Edit menu
    QAction *undoAct;
    QAction *redoAct;
    QAction *insertSceneAct;
    QAction *addSceneAct;
    QAction *removeSceneAct;
    QAction *insertTakeAct;
    QAction *addTakeAct;
    QAction *removeTakeAct;
    QAction *insertFramesAct;
    QAction *addFramesAct;
    QAction *removeFramesAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *preferencesAct;
    // Windows menu
    QAction *undoViewAct;
    QAction *cameraControllerAct;
    // Help menu
    QAction *whatsthisAct;
    QAction *helpAct;
    QAction *onlineHelpAct;
    QAction *supportUsAct;
    QAction *aboutQtAct;
    QAction *aboutAct;

    // Menues
    QMenu           *fileMenu;
    QMenu           *exportMenu;
    QMenu           *mostRecentMenu;
    QMenu           *editMenu;
    QMenu           *windowsMenu;
    QMenu           *helpMenu;
    QTabWidget      *sideBar;

    RecordingTab    *recordingTab;
    ProjectTab      *projectTab;
    // ViewTab         *viewTab;
    // CompositingTab  *compositingTab;

    // QAction *gotoFrameAct;
    // MenuFrame       *gotoMenu;
    // QPushButton     *gotoMenuCloseButton;
    // QWidget         *gotoMenuWidget;
    // QHBoxLayout     *gotoMenuWidgetLayout;
    // FlexibleSpinBox *gotoSpinner;
    // QLabel          *gotoFrameLabel;

    // Status bar
    QProgressDialog *progressDialog;
    // QLabel          *infoText;
    QTimer          *timer;
    QLabel          *projectLabel;
    QLabel          *projectID;
    QLabel          *sceneLabel;
    QLabel          *sceneID;
    QLabel          *takeLabel;
    QLabel          *takeID;
    QLabel          *exposureLabel;
    QLabel          *exposureID;

    // Other widgets
    QUndoView              *undoView;
    CameraControllerDialog *cameraControllerDialog;
    HelpBrowser            *helpBrowser;

    // Handlers
    ExternalChangeMonitor *changeMonitor;

    // Translation handling
    QString                activeLocale;
    QTranslator            appTranslator;
    QTranslator            qtTranslator;
    QString                translationsPath;
    QVector<QString>       translationsLanguages;
    QVector<QString>       translationsLocales;

    // Style handling
    int                    activeStyle;

    // Others
    QString                lastVisitedDir;
    ImageGrabberFacade    *grabber;
    bool                   verticalGrid;
    int                    verticalSpin;
    bool                   horizontalGrid;
    int                    horizontalSpin;
    QColor                 gridColor;
    bool                   signal;

    /**************************************************************************
     * Private functions
     **************************************************************************/

    /**
     * Create the translator of the application.
     * @paramnewLocale The new locale.
     */
    void createTranslator(const QString &newLocale);

    /**
     * Initialize the possible languages
     */
    void initTranslations();

    /**
     * Initialize the possible styles
     */
    void initStyles();

    /**
     * Creates key accelerators (keyboard shortcuts)
     *
     * More can be found in the function ToolsMenu::createAccelerators().
     */
    void createAccelerators();

    /**
     * Creates the actions from which the menus are created.
     */
    void createActions();

    /**
     * Creates and sets up the menu and the toolbar.
     */
    void createMenus();

    /**
     *Creates and sets up the toolsmenu.
     *@param parent the widget the toolsmenu will be inserted into.
     */
    void makeToolsMenu(QHBoxLayout *layout);

    /**
     * Creates and sets up the menu for going to a specified framenumber.
     * @param parent the widget the gotomenu will be inserted into.
     */
    // void makeGotoMenu(QVBoxLayout *layout);

    /**
     *Creates and sets up the frameview.
     *@param parent the widget the frameview will be inserted into.
     */
    void makeViews(QHBoxLayout *layout);

    /**
     * Sets up the statusbar with custom widgets.
     */
    void makeStatusBar();

    /**
     *Overloaded event listener for when when a drag enters the application.
     *@param event information about the dragEnterEvent
     */
    void dragEnterEvent(QDragEnterEvent * event);

    /**
     *Overloaded event listener for when a drop event occur in the application.
     *@param event information about the dropEvent
     */
    void dropEvent(QDropEvent *event);

    /**
     * Overloaded event listener which recieves information when a keyboard key is
     * released
     * @param k information about the key event.
     */
    void keyReleaseEvent(QKeyEvent * k);

    /**
     * Retranslates the tooltip and whatsthis text.
     *
     * This function is called from retranslateStrings.
     */
    void retranslateHelpText();

    /**
     * Changes the project history so that they shows correctly in the
     * most recent menu.
     */
    void setMostRecentProject();

    /**
     * Updates the most recent menu.
     */
    void updateMostRecentMenu();

    /**
     * Check for saving of all changes.
     */
    void checkSaved();
};

#endif
