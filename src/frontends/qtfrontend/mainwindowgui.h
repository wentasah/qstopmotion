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

#ifndef QTGUI_H
#define QTGUI_H

#include "frontends/frontend.h"
#include "frontends/qtfrontend/menuframe.h"
#include "frontends/qtfrontend/toolbar.h"
#include "frontends/qtfrontend/dialogs/descriptiondialog.h"
#include "frontends/qtfrontend/dialogs/helpbrowser.h"
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
#include "technical/grabber/imagegrabber.h"

#include <QtGui/QProgressBar>
#include <QtGui/QProgressDialog>
#include <QtGui/QUndoView>
#include <QtGui/QtGui>


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

    void init();

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
     * Retranslates the strings.
     *
     * This function is called after a new translator has been installed so that
     * the program strings are retranslated to the new language.
     */
    void retranslateStrings();

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
     * Turns off the webcamera/video import mode.
     */
    void stopGrabber();

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
     * Brings up a general preferences dialog where the user can set
     * preferences for the application.
     */
    void showGeneralDialog();

    /**
     * Brings up a project preferences dialog where the user can set
     * preferences for the project.
     */
    void showProjectDialog();

    /**
     * Show the undo view window.
     */
    void showUndoStack();

    /**
     *
     */
    void whatsThis();

    /**
     * Brings up an help dialog with the qstopmotion user manua.
     */
    void showHelpDialog();

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
    QAction *quitAct;
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
    QAction *generalAct;
    QAction *projectAct;
    QAction *undoViewAct;
    QAction *whatsthisAct;
    QAction *aboutQtAct;
    QAction *aboutAct;
    QAction *helpAct;

    // Menues
    QMenu           *fileMenu;
    QMenu           *exportMenu;
    QMenu           *mostRecentMenu;
    QMenu           *editMenu;
    QMenu           *settingsMenu;
    QMenu           *viewMenu;
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
    QUndoView       *undoView;
    HelpBrowser     *helpBrowser;

    // Handlers
    ExternalChangeMonitor *changeMonitor;

    // Translation handling
    QTranslator       appTranslator;
    QTranslator       qtTranslator;
    QString           translationsPath;
    QVector<QString>  translationsLanguages;
    QVector<QString>  translationsLocales;

    // Others
    QString           lastVisitedDir;
    ImageGrabber     *grabber;
    bool              verticalGrid;
    int               verticalSpin;
    bool              horizontalGrid;
    int               horizontalSpin;

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
