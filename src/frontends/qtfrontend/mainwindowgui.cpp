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

#include "mainwindowgui.h"

#include "domain/domainfacade.h"
#include "domain/animation/projectserializer.h"
#include "frontends/qtfrontend/toolbar.h"
#include "frontends/qtfrontend/dialogs/aboutdialog.h"
#include "frontends/qtfrontend/dialogs/startdialog.h"
#include "frontends/qtfrontend/frameview/frameviewimage.h"
#include "technical/preferencestool.h"
#include "technical/util.h"
#include "technical/videoencoder/ffmpegencoder.h"

#include <QtCore/QtDebug>
#include <QtGui/QFileDialog>

using namespace Qt;


/**************************************************************************
 * Public functions
 **************************************************************************/

MainWindowGUI::MainWindowGUI(QApplication *stApp, Frontend *f)
    : stApp(stApp), frontend(f)
{
    qDebug("MainWindowGUI::Constructor --> Start");

    centerWidget         = 0;
    centerWidgetLayout   = 0;
    workArea             = 0;
    workAreaLayout       = 0;
    viewArea             = 0;
    viewAreaLayout       = 0;
    timeLine             = 0;
    frameView            = 0;
    toolBar              = 0;

    newAct               = 0;
    openAct              = 0;
    mostRecentAct        = 0;
    secondMostRecentAct  = 0;
    thirdMostRecentAct   = 0;
    fourthMostRecentAct  = 0;
    saveAct              = 0;
    saveAsAct            = 0;
    videoAct             = 0;
    cinelerraAct         = 0;
    propertiesAct        = 0;
    quitAct              = 0;
    undoAct              = 0;
    redoAct              = 0;
    insertSceneAct       = 0;
    addSceneAct          = 0;
    removeSceneAct       = 0;
    insertTakeAct        = 0;
    addTakeAct           = 0;
    removeTakeAct        = 0;
    insertFramesAct      = 0;
    addFramesAct         = 0;
    removeFramesAct      = 0;
    cutAct               = 0;
    copyAct              = 0;
    pasteAct             = 0;
    preferencesAct       = 0;
    whatsthisAct         = 0;
    undoViewAct          = 0;
    cameraControllerAct  = 0;
    aboutAct             = 0;
    helpAct              = 0;

    fileMenu             = 0;
    exportMenu           = 0;
    mostRecentMenu       = 0;
    editMenu             = 0;
    windowsMenu          = 0;
    helpMenu             = 0;

    sideBar              = 0;
    recordingTab         = 0;
    projectTab           = 0;
    // viewTab              = 0;
    // compositingTab       = 0;

    // gotoFrameAct         = 0;
    // gotoMenuCloseButton  = 0;
    // gotoMenuWidget       = 0;
    // gotoMenuWidgetLayout = 0;
    // gotoSpinner          = 0;

    // Status bar
    progressDialog       = 0;
    timer                = 0;
    projectLabel         = 0;
    projectID            = 0;
    sceneLabel           = 0;
    sceneID              = 0;
    takeLabel            = 0;
    takeID               = 0;
    exposureLabel        = 0;
    exposureID           = 0;

    // Other widgets
    undoView               = 0;
    cameraControllerDialog = 0;
    helpBrowser            = 0;

    changeMonitor        = 0;

    grabber              = 0;

    verticalGrid         = false;
    verticalSpin         = 5;
    horizontalGrid       = false;
    horizontalSpin       = 5;
    gridColor.setNamedColor("#000000");     // RGB Black

    this->setObjectName("MainWindowGUI");
    stApp->installTranslator(&appTranslator);
    stApp->installTranslator(&qtTranslator);

    qDebug("MainWindowGUI::Constructor --> End");
}


MainWindowGUI::~MainWindowGUI()
{
    qDebug("MainWindowGUI::Destructor --> Start");

    if (helpBrowser) {
        helpBrowser->close();
        delete helpBrowser;
        helpBrowser = 0;
    }

    if (grabber != 0) {
        delete grabber;
        grabber = 0;
    }

    frontend = NULL;

    qDebug("MainWindowGUI::Destructor --> End");
}


void MainWindowGUI::retranslateStrings()
{
    qDebug("MainWindowGUI::retranslateStrings --> Start");

    // The actions caption texts
    newAct->setText(tr("&New"));
    openAct->setText(tr("&Open"));
    saveAct->setText(tr("&Save"));
    saveAsAct->setText(tr("Save &As"));
    videoAct->setText(tr("Video"));
    cinelerraAct->setText(tr("Cinelerra"));
    propertiesAct->setText(tr("Properties"));
    quitAct->setText(tr("&Quit"));
    undoAct->setText(tr("&Undo"));
    redoAct->setText(tr("Re&do"));
    insertSceneAct->setText(tr("Insert Scene"));
    addSceneAct->setText(tr("Add Scene"));
    removeSceneAct->setText(tr("Remove Scene"));
    insertTakeAct->setText(tr("Insert Take"));
    addTakeAct->setText(tr("Add Take"));
    removeTakeAct->setText(tr("Remove Take"));
    insertFramesAct->setText(tr("Insert Frames"));
    addFramesAct->setText(tr("Add Frames"));
    removeFramesAct->setText(tr("Remove Frames"));
    // cutAct->setText(tr("Cu&t"));
    // copyAct->setText(tr("&Copy"));
    // pasteAct->setText(tr("&Paste"));
    preferencesAct->setText(tr("Preferences"));
    whatsthisAct->setText(tr("What's &This"));
    undoViewAct->setText(tr("&Undo stack"));
    cameraControllerAct->setText(tr("&Camera Controller"));
    helpAct->setText(tr("&Help"));
    aboutQtAct->setText(tr("About &Qt"));
    aboutAct->setText(tr("&About"));

    // The menus
    fileMenu->setTitle(tr("&File"));
    mostRecentMenu->setTitle(tr("Open &Recent"));
    exportMenu->setTitle(tr("&Export"));
    editMenu->setTitle(tr("&Edit"));
    windowsMenu->setTitle(tr("&Windows"));
    helpMenu->setTitle(tr("&Help"));

    updateMostRecentMenu();

    // Status bar
    projectLabel->setText(tr("Project ID: "));
    sceneLabel->setText(tr("Scene ID: "));
    takeLabel->setText(tr("Take ID: "));
    exposureLabel->setText(tr("Exposure ID: "));

    // The sub widgets
    toolBar->retranslateStrings();
    recordingTab->retranslateStrings();
    sideBar->setTabText(0, tr("Recording"));
    projectTab->retranslateStrings();
    sideBar->setTabText(1, tr("Project"));
    // viewTab->retranslateStrings();
    // sideBar->setTabText(2, tr("View"));
    // compositingTab->retranslateStrings();
    // sideBar->setTabText(3, tr("Compositing"));

    // Tooltip and whatsthis texts
    retranslateHelpText();

    if (cameraControllerDialog != NULL) {
        cameraControllerDialog->retranslateStrings();
    }

    if (helpBrowser != NULL) {
        delete helpBrowser;
        helpBrowser = NULL;
    }

    qDebug("MainWindowGUI::retranslateStrings --> End");
}


void MainWindowGUI::retranslateHelpText()
{
    QString infoText;

    // File menu
    infoText =
        tr("<h4>New</h4> "
           "<p>Creates a <em>new</em> project.</p>");
    newAct->setWhatsThis(infoText);
    //The prepend part is a trick to keep the accelerator in the tooltip
    infoText =
        newAct->toolTip().prepend(tr("New project"));
    newAct->setToolTip(infoText);

    infoText =
        tr("<h4>Open</h4> "
           "<p><em>Opens</em> a qStopMotion project file.</p>");
    openAct->setWhatsThis(infoText);
    infoText =
        openAct->toolTip().prepend(tr("Open project"));
    openAct->setToolTip(infoText);

    infoText =
        tr("<h4>Save</h4> "
           "<p><em>Saves</em> the current animation as a qStopMotion "
           "project file. <BR>If this project has been saved before it will "
           "automaticly be saved to the previously selected file.</p>");
    saveAct->setWhatsThis(infoText);
    infoText =
        saveAct->toolTip().prepend(tr("Save project"));
    saveAct->setToolTip(infoText);

    infoText =
        tr("<h4>Save As</h4> "
           "<p><em>Saves</em> the current animation as a qStopMotion "
           "project file.</p>");
    saveAsAct->setWhatsThis(infoText);
    infoText =
        saveAsAct->toolTip().prepend(tr("Save project As"));
    saveAsAct->setToolTip(infoText);

    infoText =
        tr("<h4>Video</h4> "
           "<p>Exports the current project as <em>video</em>.</p>"
           "You will be given a wizard to guide you.");
    videoAct->setWhatsThis(infoText);
    videoAct->setToolTip(infoText);

    infoText =
        tr("<h4>Cinelerra</h4> "
           "<p>Exports the current animation as a <em>cinelerra-cv</em> project.</p>"
           "You will be given a wizard to guide you.");
    cinelerraAct->setWhatsThis(infoText);
    cinelerraAct->setToolTip(infoText);

    infoText =
        tr("<h4>Properties of the project</h4> "
           "<p>This will opens a window where you can <em>change</em> "
           "properties of the animation project.</p>");
    propertiesAct->setWhatsThis(infoText);
    infoText =
        propertiesAct->toolTip().prepend(tr("Properties of the animation project"));
    propertiesAct->setToolTip(infoText);

    infoText =
        tr("<h4>Quit</h4> "
           "<p><em>Quits</em> the program.</p>");
    quitAct->setWhatsThis(infoText);
    infoText =
        quitAct->toolTip().prepend(tr("Quit"));
    quitAct->setToolTip(infoText);


    // Edit menu
    infoText =
        tr("<h4>Undo</h4> "
           "<p><em>Undoes</em> your last operation. You can press undo "
           "several time to undo earlier operations.</p>");
    undoAct->setWhatsThis(infoText);
    infoText =
        undoAct->toolTip().prepend(tr("Undo"));
    undoAct->setToolTip(infoText);

    infoText =
        tr("<h4>Redo</h4> "
           "<p><em>Redoes</em> your last operation. You can press redo "
           "several times to redo several operations.</p>");
    redoAct->setWhatsThis(infoText);
    infoText =
        redoAct->toolTip().prepend(tr("Redo"));
    redoAct->setToolTip(infoText);
/*
    infoText =
        tr("<h4>Cut</h4> "
           "<p><em>Cuts</em> the selected frames out of the animation and adds them "
           "to the clipboard so that you can paste them in somewhere else.</p>");
    cutAct->setWhatsThis(infoText);
    infoText =
        cutAct->toolTip().prepend(tr("Cut"));
    cutAct->setToolTip(infoText);

    infoText =
        tr("<h4>Copy</h4> "
           "<p><em>Copies</em> the selected frames to the clipboard. You can "
           "then paste them in another place.</p>");
    copyAct->setWhatsThis(infoText);
    infoText =
        copyAct->toolTip().prepend(tr("Copy"));
    copyAct->setToolTip(infoText);

    infoText =
        tr("<h4>Paste</h4> "
           "<p><em>Pastes</em> the frames which are currently in the clipboard "
           "into the selected location.</p> <p>You can copy/cut images from another "
           "programs and then use this option to paste them into this animation.</p>");
    pasteAct->setWhatsThis(infoText);
    infoText =
        pasteAct->toolTip().prepend(tr("Paste"));
    pasteAct->setToolTip(infoText);

    infoText =
        tr("<h4>Go to frame</h4> "
           "<p>This will bring up a popup-menu at the bottom where you can choose "
           "a frame you want to <em>go to</em>.</p>");
    gotoFrameAct->setWhatsThis(infoText);
    infoText =
        gotoFrameAct->toolTip().prepend(tr("Go to frame"));
    gotoFrameAct->setToolTip(infoText);
*/
    infoText =
        tr("<h4>Preferences of the application</h4> "
           "<p>This will opens a window where you can <em>change</em> "
           "the preferences of the application.</p>");
    preferencesAct->setWhatsThis(infoText);
    infoText =
        preferencesAct->toolTip().prepend(tr("Preferences of qStopMotion"));
    preferencesAct->setToolTip(infoText);


    // Windows menu

    // Help menu
    infoText =
        tr("<h4>What's This</h4> "
           "<p>This will give you a WhatsThis mouse cursor which can be used to "
           "bring up helpful information like this.</p>");
    whatsthisAct->setWhatsThis(infoText);
    infoText =
        whatsthisAct->toolTip().prepend(tr("What's This"));
    whatsthisAct->setToolTip(infoText);

    infoText =
        tr("<h4>Help</h4> "
           "<p>This button will bring up a dialog with the qStopMotion manual</p>");
    helpAct->setWhatsThis(infoText);
    infoText =
        helpAct->toolTip().prepend(tr("Help"));
    helpAct->setToolTip(infoText);

    infoText =
        tr("<h4>About Qt</h4> "
           "<p>This will display a small information box where you can read "
           "general information about the Qt library.</p>");
    aboutQtAct->setWhatsThis(infoText);
    infoText =
        aboutAct->toolTip().prepend(tr("About Qt"));
    aboutQtAct->setToolTip(infoText);

    infoText =
        tr("<h4>About</h4> "
           "<p>This will display a small information box where you can read "
           "general information as well as the names of the developers "
           "behind this excellent piece of software.</p>");
    aboutAct->setWhatsThis(infoText);
    infoText =
        aboutAct->toolTip().prepend(tr("About"));
    aboutAct->setToolTip(infoText);


    // Status bar
    infoText =
        tr("<h4>Project ID</h4><p>This area displays the id "
           "of the currently active project</p>");
    projectLabel->setToolTip(infoText);
    projectLabel->setWhatsThis(infoText);
    projectID->setToolTip(infoText);
    projectID->setWhatsThis(infoText);

    infoText =
        tr("<h4>Scene ID</h4><p>This area displays the id "
           "of the currently selected scene</p>");
    sceneLabel->setToolTip(infoText);
    sceneLabel->setWhatsThis(infoText);
    sceneID->setToolTip(infoText);
    sceneID->setWhatsThis(infoText);

    infoText =
        tr("<h4>Take ID</h4><p>This area displays the id "
           "of the currently selected take</p>");
    takeLabel->setToolTip(infoText);
    takeLabel->setWhatsThis(infoText);
    takeID->setToolTip(infoText);
    takeID->setWhatsThis(infoText);

    infoText =
        tr("<h4>Exposure ID</h4><p>This area displays the id "
           "of the currently selected exposure</p>");
    exposureLabel->setToolTip(infoText);
    exposureLabel->setWhatsThis(infoText);
    exposureID->setToolTip(infoText);
    exposureID->setWhatsThis(infoText);

    // Various menues
    infoText =
        tr("<h4>FrameView</h4><p> In this area you can see the "
           "selected frame. You can also play "
           "animations in this window by pressing the "
           "<b>Play</b> button.</p>");
    frameView->setWhatsThis(infoText);
/*
    infoText =
        tr("<h4>Go to frame menu</h4> "
           "<p>Here you can specify a framenumber and the program will jump "
           "to the specified frame</p> ");
    gotoMenuWidget->setWhatsThis(infoText);
*/
    infoText =
        tr("<h4>TimeLine</h4> "
           "<p>In this area you can see the frames and scenes "
           "in the animations and build the animation "
           "by moving the them around.</p><p>You can "
           "switch to the next and the previous frame "
           "using the <b>arrow buttons</b> or <b>x</b> "
           "and <b>z</b></p> ");
    timeLine->setWhatsThis(infoText);
}


void MainWindowGUI::initialize()
{
    int     value;
    QString gridColorName;

    lastVisitedDir.clear();

    this->initTranslations();
    PreferencesTool *pref = frontend->getPreferences();
    QString activeLocale;

    pref->getStringPreference("preferences", "language", activeLocale);
    createTranslator(activeLocale);

    grabber = new ImageGrabberFacade(frontend);

    grabber->initialization();
    if (!grabber->isGrabberInitialized()) {
        frontend->hideProgress();
        frontend->showWarning(tr("Select image grabber"),
                              tr("You have to define an image grabber to use.\n"
                                 "This can be set in the preferences menu."));
    }

    centerWidget = new QWidget;
    centerWidget->setObjectName("CenterWidget");
    centerWidgetLayout = new QVBoxLayout;
    centerWidgetLayout->setObjectName("CenterWidgetLayout");
    centerWidgetLayout->setSpacing(5);
    centerWidgetLayout->setMargin(0);

    timeLine = new TimeLine(this->frontend);

    // Initializes and sets up the workarea consisting of the toolsmenu and the frameview.
    workArea = new QWidget;
    workArea->setObjectName("WorkArea");
    workAreaLayout = new QHBoxLayout;
    workAreaLayout->setObjectName("WorkAreaLayout");
    workAreaLayout->setSpacing(5);
    workAreaLayout->setMargin(0);
    makeViews(workAreaLayout);
    makeToolsMenu(workAreaLayout);
    workArea->setLayout(workAreaLayout);

    centerWidgetLayout->addWidget(workArea);
    centerWidgetLayout->addWidget(timeLine);

    centerWidget->setLayout(centerWidgetLayout);
    setCentralWidget(centerWidget);

    makeStatusBar();

    // Initializes and sets up the menue system.
    createAccelerators();
    createActions();
    createMenus();

    // Mainwindow preferences.
    setWindowTitle("qStopMotion");
    QString iconFile(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    setWindowIcon(QPixmap(iconFile));

    setContentsMargins(5, 0, 5, 0);
    setAcceptDrops(true);

    statusBar()->showMessage(tr("Ready to rumble ;-)"), 2000);
    statusBar()->setSizeGripEnabled(false);

    // Sets all the text in the program.
    retranslateStrings();

    connect(this, SIGNAL(startNewProject()), this, SLOT(newProject()));
    connect(this, SIGNAL(startLastProject()), this, SLOT(openMostRecent()));
    connect(this, SIGNAL(startOpenProject()), this, SLOT(openProject()));
    connect(this, SIGNAL(startExit()),  this, SLOT(closeApplication()));

    if (pref->getIntegerPreference("preferences", "verticalgrid", value) == false) {
        value = false;
    }
    verticalGrid = value;

    if (pref->getIntegerPreference("preferences", "verticalspin", verticalSpin) == false) {
        verticalSpin = 5;
    }

    if (pref->getIntegerPreference("preferences", "horizontalgrid", value) == false) {
        value = false;
    }
    horizontalGrid = value;

    if (pref->getIntegerPreference("preferences", "horizontalspin", horizontalSpin) == false) {
        horizontalSpin = 5;
    }

    if (pref->getStringPreference("preferences", "gridcolor", gridColorName) == true) {
        gridColor.setNamedColor(gridColorName);
    }

    qDebug("MainWindowGUI::Constructor --> End");
}


void MainWindowGUI::closeEvent(QCloseEvent *event)
{
    qDebug("MainWindowGUI::closeEvent --> Start");

    // Last changes saved?
    checkSaved();

    event->accept();

    qDebug("MainWindowGUI::closeEvent --> End");
}


void MainWindowGUI::mousePressEvent(QMouseEvent *)
{
    // gotoMenuWidget->hide();
}


void MainWindowGUI::keyPressEvent(QKeyEvent *k)
{
    DomainFacade *facade = frontend->getProject();
    int activeSceneIndex = -1;
    int activeTakeIndex = -1;

    if (facade->isActiveProject()) {
        activeSceneIndex = facade->getActiveSceneIndex();
    }
    if (-1 < activeSceneIndex) {
        // If there is a active scene, there can be a active take
        facade->getActiveTakeIndex();
    }

    switch (k->key()) {
    case Key_Shift:
        timeLine->setSelecting(true);
        break;
    case Key_A:
        // TODO: change so that CTRL+A instead of just A selects all the frames.
        if (activeSceneIndex >= 0) {
            if (facade->getSceneExposureSize(activeSceneIndex) > 1) {
                facade->selectExposureToUndo(activeSceneIndex, activeTakeIndex, 0);
                // timeLine->setSelection(facade->getSceneExposureSize(activeScene) - 1);
            }
        }
        break;
    default:
        k->ignore();
        break;
    }
}


const QVector<QString> MainWindowGUI::getLanguages()
{
    return this->translationsLanguages;
}


const QVector<QString> MainWindowGUI::getLocales()
{
    return this->translationsLocales;
}


void MainWindowGUI::changeLanguage(int newIndex)
{
    this->createTranslator(this->translationsLocales[newIndex]);
    retranslateStrings();
}


void MainWindowGUI::changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction)
{
    recordingTab->changeCaptureButtonFunction(newFunction);
}


bool MainWindowGUI::getVerticalGrid()
{
    return verticalGrid;
}


void MainWindowGUI::setVerticalGrid(bool newState)
{
    verticalGrid = newState;
}


int MainWindowGUI::getVerticalSpin()
{
    return verticalSpin;
}


void MainWindowGUI::setVerticalSpin(int newSpin)
{
    verticalSpin = newSpin;
}


bool MainWindowGUI::getHorizontalGrid()
{
    return horizontalGrid;
}


void MainWindowGUI::setHorizontalGrid(bool newState)
{
    horizontalGrid = newState;
}


int MainWindowGUI::getHorizontalSpin()
{
    return horizontalSpin;
}


void MainWindowGUI::setHorizontalSpin(int newSpin)
{
    horizontalSpin = newSpin;
}


void MainWindowGUI::getGridColorRGB(int *r, int *g, int *b, int *a)
{
    gridColor.getRgb(r, g, b, a);
}


void MainWindowGUI::setGridColorRGB(int r, int g, int b, int a)
{
    gridColor.setRgb(r, g, b, a);
}


void MainWindowGUI::showProgress(const QString &operation, unsigned int numOperations)
{
    if (numOperations > 0) {
        progressDialog = new QProgressDialog(operation, tr("Cancel"), 0, numOperations, this);
        progressDialog->show();
    }
}


void MainWindowGUI::hideProgress()
{
    if (progressDialog) {
        progressDialog->hide();
        delete progressDialog;
        progressDialog = NULL;
    }
}


void MainWindowGUI::updateProgress(int numOperationsDone)
{
    if (progressDialog) {
        progressDialog->setValue(numOperationsDone);
    }
}


void MainWindowGUI::increaseProgress()
{
    if (progressDialog) {
        progressDialog->setValue(progressDialog->value() + 1);
    }
}


void MainWindowGUI::setProgressInfo(const char *infoText)
{
    if (progressDialog) {
        progressDialog->setLabelText(infoText);
    }
}


bool MainWindowGUI::isOperationAborted()
{
    if (progressDialog) {
        return progressDialog->wasCanceled();
    }
    return false;
}


void MainWindowGUI::showMessage(const QString &message, int timeout)
{
    statusBar()->showMessage(message, timeout);
}


void MainWindowGUI::clearMessage()
{
    statusBar()->clearMessage();
}


void MainWindowGUI::setProjectID(const QString &id)
{
    projectID->setText(id);
}


void MainWindowGUI::setSceneID(const QString &id)
{
    sceneID->setText(id);
}


void MainWindowGUI::setTakeID(const QString &id)
{
    takeID->setText(id);
}


void MainWindowGUI::setExposureID(const QString &id)
{
    exposureID->setText(id);
}


void MainWindowGUI::startDialog()
{
    qDebug("MainWindowGUI::startDialog --> Start");

    QString pID(mostRecentAct->text());
    StartDialog *dialog = new StartDialog(frontend, pID, this);
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        emit startExit();
        qDebug("MainWindowGUI::startDialog --> End (cancel)");
        return;
    }
    int selection = dialog->getSelectedPossibility();
    switch (selection)
    {
    case 1:
        // New project
        emit startNewProject();
        break;
    case 2:
        // Last project
        emit startLastProject();
        break;
    case 3:
        // Open project
        emit startOpenProject();
        break;
    }

    qDebug("MainWindowGUI::startDialog --> End");
}

bool MainWindowGUI::startGrabber()
{
    qDebug("MainWindowGUI::startGrabber --> Start");

    frontend->showMessage(tr("Connecting camera..."));
    grabber->init();
    if (!grabber->isGrabberInited()) {
        frontend->hideProgress();
        frontend->showWarning(tr("Select image grabber"),
                              tr("You have to define an image grabber to use.\n"
                                 "This can be set on the recording tool tab."));
        return false;
    }

    frameView->cameraOn();

    frontend->clearMessage();

    if (0 != cameraControllerDialog) {
        if (!cameraControllerDialog->isHidden()) {
            cameraControllerDialog->setUp();
        }
    }

    qDebug("MainWindowGUI::startGrabber --> End");
    return true;
}


bool MainWindowGUI::isGrabberInited()
{
    qDebug("MainWindowGUI::isGrabberInited --> Start");

    bool ret = grabber->isGrabberInited();

    qDebug("MainWindowGUI::isGrabberInited --> End");

    return ret;
}


void MainWindowGUI::stopGrabber()
{
    qDebug("MainWindowGUI::stopGrabber --> Start");

    if (0 != cameraControllerDialog) {
        cameraControllerDialog->tearDown();
    }

    frameView->cameraOff();

    grabber->finalize();

    qDebug("MainWindowGUI::stopGrabber --> End");
}


GrabberController * MainWindowGUI::getGrabberController()
{
    return grabber->getDevice(getVideoSource())->getController();
}


const QVector<ImageGrabberDevice*> MainWindowGUI::getDevices()
{
    return grabber->getDevices();
}


ImageGrabberDevice* MainWindowGUI::getDevice(int deviceIndex)
{
    return grabber->getDevice(deviceIndex);
}


const QVector<QString> MainWindowGUI::getDeviceNames()
{
    return grabber->getDeviceNames();
}


const QVector<QString> MainWindowGUI::getResolutionNames(int deviceIndex)
{
    return grabber->getResolutionNames(deviceIndex);
}


const QImage MainWindowGUI::getLiveImage()
{
    return grabber->getLiveImage();
}


const QImage MainWindowGUI::getRawImage()
{
    return grabber->getRawImage();
}


void MainWindowGUI::nextAnimationFrame(int exposureIndex)
{
    frameView->nextAnimationFrame(exposureIndex);
}


void MainWindowGUI::openProject(const QString &filePath)
{
    Q_ASSERT(filePath != NULL);

    checkSaved();

    if (frontend->getProject()->isActiveProject()) {
        // Close the old project
        // frontend->getProject()->closeProjectToUndo();
        frontend->getProject()->closeProject();
    }

    frontend->getProject()->openProjectToUndo(filePath);
    setMostRecentProject();

    saveAsAct->setEnabled(true);
    saveAct->setEnabled(true);
    toolBar->setActualState(ToolBar::toolBarCameraOff);

    int lastSlash = filePath.lastIndexOf("/");
    QString fileName = filePath.mid(lastSlash+1);
    QString fileDirectory = filePath.left(lastSlash+1);

    lastVisitedDir.clear();
    lastVisitedDir.append(fileDirectory);

    setWindowTitle("qStopMotion - " + fileName);
}


void MainWindowGUI::setToolBarState(int newState)
{
    qDebug("MainWindowGUI::setToolBarState --> Start");

    toolBar->setActualState(newState);

    qDebug("MainWindowGUI::setToolBarState --> End");
}


int MainWindowGUI::getRecordingMode()
{
    return recordingTab->getRecordingMode();
}


void MainWindowGUI::setRecordingMode(int mode)
{
    recordingTab->setRecordingMode(mode);
}


int MainWindowGUI::getVideoSource()
{
    return recordingTab->getVideoSource();
}


bool MainWindowGUI::setVideoSource(int index)
{
    if (recordingTab->setVideoSource(index) == false) {
        return false;
    }

    if (cameraControllerDialog != 0) {
        if (cameraControllerDialog->isVisible()) {
            cameraControllerDialog->reject();
        }
        delete cameraControllerDialog;
        cameraControllerDialog = 0;
    }

    if (grabber->getDevice(index)->isController()) {
        cameraControllerAct->setEnabled(true);
    }
    else {
        cameraControllerAct->setEnabled(false);
    }

    return true;
}


int MainWindowGUI::getResolution()
{
    return recordingTab->getResolution();
}


bool MainWindowGUI::setResolution(int index)
{
    if (recordingTab->setResolution(index) == false) {
        return false;
    }
    /*
    if (cameraControllerDialog != 0) {
        if (cameraControllerDialog->isVisible()) {
            cameraControllerDialog->reject();
        }
        delete cameraControllerDialog;
        cameraControllerDialog = 0;
    }

    if (grabber->getDevice(index)->isController()) {
        cameraControllerAct->setEnabled(true);
    }
    else {
        cameraControllerAct->setEnabled(false);
    }
    */
    return true;
}


const GrabberResolution MainWindowGUI::getResolution(int index)
{
    return grabber->getResolution(frontend->getProject()->getVideoSource(), index);
}


bool MainWindowGUI::convertImages(int newFormat, int newQuality)
{
    if (frontend->getProject()->getTotalExposureSize()) {
        int convert = frontend->askQuestion(tr("Existing Images"),
                                            tr("There are some images in the open project. Do you want to convert the images to the new file format or quality?"));
        if (convert == 0) {
            // user pressed button 0, which is 'yes'

            if (frontend->getProject()->convertImages(newFormat, newQuality)) {
                saveProject();
                return true;
            }
            else {
                return false;
            }
        }
        else {
            // user say not 'yes'
            return false;
        }
    }

    // The project is empty
    return true;
}


int MainWindowGUI::getMixMode()
{
    return recordingTab->getMixMode();
}


void MainWindowGUI::setMixMode(int mode)
{
    recordingTab->setMixMode(mode);
}


int MainWindowGUI::getMixCount()
{
    return recordingTab->getMixCount();
}


void MainWindowGUI::setMixCount(int count)
{
    recordingTab->setMixCount(count);
}


int MainWindowGUI::getPlaybackCount()
{
    return recordingTab->getPlaybackCount();
}


void MainWindowGUI::setPlaybackCount(int count)
{
    recordingTab->setPlaybackCount(count);
}


void MainWindowGUI::setupDirectoryMonitoring()
{
    qDebug("MainWindowGUI::setupDirectoryMonitoring --> Start");

    changeMonitor = new ExternalChangeMonitor(frontend, this);

    qDebug("MainWindowGUI::setupDirectoryMonitoring --> End");
}


void MainWindowGUI::addDirectoryToMonitoring(const QString &directory)
{
    changeMonitor->addDirectory(directory);
}


void MainWindowGUI::removeDirectoryFromMonitoring(const QString &directory)
{
    changeMonitor->removeDirectory(directory);
}


void MainWindowGUI::removeAllDirectoriesFromMonitoring()
{
    changeMonitor->removeAllDirectories();
}


void MainWindowGUI::addFileToMonitoring(const QString &file)
{
    changeMonitor->addFile(file);
}


void MainWindowGUI::removeFileFromMonitoring(const QString &file)
{
    changeMonitor->removeFile(file);
}

void MainWindowGUI::removeAllFilesFromMonitoring()
{
    changeMonitor->removeAllFiles();
}

/**************************************************************************
 * Public slots
 **************************************************************************/

/**************************************************************************
 * Private slots
 **************************************************************************/

void MainWindowGUI::newProject()
{
    qDebug("MainWindowGUI::newProject --> Start");

    DomainFacade* project = frontend->getProject();

    recordingTab->checkCameraOff();
    checkSaved();

    DescriptionDialog *dialog = new DescriptionDialog(frontend, DescriptionDialog::ProjectDescription);
    dialog->setProjectDescription(tr("The Project"));
    dialog->setSceneDescription(tr("Scene 001"));
    dialog->setTakeDescription(tr("Take 01"));
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("MainWindowGUI::newProject --> End (cancel)");

        delete(dialog);

        return;
    }

    QString projectDescription = dialog->getProjectDescription();
    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    if (frontend->getProject()->isActiveProject()) {
        // Close the old project
        // frontend->getProject()->closeProjectToUndo();
        frontend->getProject()->closeProject();
    }

    // Create the new project
    project->newProjectToUndo(projectDescription);

    // Create and activate the new scene
    project->addSceneToUndo(sceneDescription);
    project->selectSceneToUndo(0);

    // Create and activate the new take
    project->addTakeToUndo(takeDescription, 0);
    project->selectTakeToUndo(0, 0);

    //fileMenu->setItemEnabled(SAVE, false);

    setToolBarState(ToolBar::toolBarCameraOff);

    delete(dialog);

    QDir homeDir = QDir::home();
    lastVisitedDir.clear();
    lastVisitedDir.append(homeDir.absolutePath());

    setWindowTitle(tr("qStopMotion - New Animation Project"));

    qDebug("MainWindowGUI::newProject --> End");
}


void MainWindowGUI::openProject()
{
    qDebug("MainWindowGUI::openProject --> Start");

    recordingTab->checkCameraOff();

    QFileDialog fileDialog(this, tr("Choose project file"), lastVisitedDir);
    QStringList filters;
    filters << QString(tr("Project (*.%1)")).arg(PreferencesTool::projectSuffix);
    //         << QString(tr("Archive (*.%2)")).arg(PreferencesTool::archiveSuffix);
    fileDialog.setFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    int ret = fileDialog.exec();
    if (ret == QDialog::Accepted) {
        QStringList openFiles = fileDialog.selectedFiles();

        // Open the new project
        openProject(openFiles[0]);
    }

    qDebug("MainWindowGUI::openProject --> End");
}


void MainWindowGUI::openMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(mostRecentAct->text());

    recordingTab->checkCameraOff();

    // Open the new project
    openProject(fileName);
}


void MainWindowGUI::openSecondMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(secondMostRecentAct->text());

    recordingTab->checkCameraOff();

    // Open the new project
    openProject(fileName);
}


void MainWindowGUI::openThirdMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(thirdMostRecentAct->text());

    recordingTab->checkCameraOff();

    // Open the new project
    openProject(fileName);
}


void MainWindowGUI::openFourthMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(fourthMostRecentAct->text());

    recordingTab->checkCameraOff();

    // Open the new project
    openProject(fileName);
}


void MainWindowGUI::saveProject()
{
    const QString filePath = frontend->getProject()->getNewProjectFilePath();

    recordingTab->checkCameraOff();

    if (!filePath.isEmpty()) {
        frontend->getProject()->saveProject(filePath, false);
    } else {
        saveProjectAs();
    }
}


void MainWindowGUI::saveProjectAs()
{
    QString filePath;

    recordingTab->checkCameraOff();

    QFileDialog fileDialog(this, tr("Save As"), lastVisitedDir);
    QStringList filters;
    int         ret;

    filters << QString(tr("Project (*.%1)")).arg(PreferencesTool::projectSuffix);
    //         << QString(tr("Archive (*.%2)")).arg(PreferencesTool::archiveSuffix);
    fileDialog.setFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);

    while (1) {
        ret = fileDialog.exec();
        if (ret == QDialog::Accepted) {
            QStringList openFiles = fileDialog.selectedFiles();
            filePath.append(openFiles[0]);
        }

        if (filePath.isEmpty()) {
            return;
        }
        if (!filePath.endsWith(PreferencesTool::projectSuffix)) {
            // Append the project suffix
            filePath.append(".");
            filePath.append(PreferencesTool::projectSuffix);
        }
        if (filePath.indexOf('|') != -1) {
            // Remove all '|' characters
            frontend->showInformation(tr("Information"), tr("The character '|' is not allowed in the project file name and will be removed."));
            filePath.remove('|');
        }
        // Only one project per directory is possible
        QDir fileDirectory = QFileInfo(filePath).absoluteDir();
        QString nameFilter("*.");
        nameFilter.append(PreferencesTool::projectSuffix);
        QStringList nameFilterList(nameFilter);
        QStringList fileList = fileDirectory.entryList(nameFilterList);
        if (fileList.count() == 0) {
            break;
        }

        // User must select an other directory
        frontend->showWarning(tr("Warning"), tr("The project directory must not contain more than one project file."));
        filePath.clear();
    }

    frontend->getProject()->saveProject(filePath.toLocal8Bit(), true);
    QString fileDir = frontend->getProject()->getNewProjectPath();
    QString fileName = filePath.mid(filePath.lastIndexOf("/")+1);

    lastVisitedDir.clear();
    lastVisitedDir.append(fileDir);

    saveAct->setEnabled(true);
    setMostRecentProject();

    setWindowTitle("qStopMotion - " + fileName);
}


void MainWindowGUI::exportToVideo()
{
    qDebug("MainWindowGUI::exportToVideo --> Start");

    VideoEncoder    *enc = NULL;
    int              activeEncoderApplication;
    bool             useDefaultOutputFile;

    recordingTab->checkCameraOff();

    activeEncoderApplication = this->frontend->getProject()->getEncoderApplication();
    if (activeEncoderApplication == VideoEncoder::noneApplication) {
        frontend->showWarning(tr("Warning"),
                              tr("No encoder selected for the video export.\n"
                                 "This can be setted in the properties dialog of the project.\n"
                                 "Export to video will not be possible until you\n"
                                 "have setted an encoder to use!"));
        return;
    }

    switch (activeEncoderApplication)
    {
    case VideoEncoder::ffmpegApplication:
        enc = new FfmpegEncoder(this->frontend->getProject()->getAnimationProject());

        break;
    case VideoEncoder::mencoderApplication:
        break;
    default:
        return;
    }

    useDefaultOutputFile = this->frontend->getProject()->getUseDefaultOutputFile();
    if (useDefaultOutputFile == false) {
        QStringList  filters;
        QString      exportSuffix;
        QString      outputFile;
        int          videoFormat = VideoEncoder::noneFormat;

        videoFormat = this->frontend->getProject()->getVideoFormat();
        switch(videoFormat) {
        case VideoEncoder::aviFormat:
            filters << tr("AVI Videos (*.avi)");
            exportSuffix.append("avi");
            break;
        case VideoEncoder::mp4Format:
            filters << tr("MP4 Videos (*.mp4)");
            exportSuffix.append("mp4");
            break;
        case VideoEncoder::noneFormat:
            frontend->showWarning(tr("Warning"),
                                  tr("No video format selected for the video export.\n"
                                     "This can be setted in the properties dialog of the project.\n"
                                     "Export to video will not be possible until you\n"
                                     "have setted an video format to use!"));
            return;
        }

        QFileDialog fileDialog(this, tr("Export to video file"), lastVisitedDir);
        fileDialog.setFilters(filters);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        int ret = fileDialog.exec();
        if (ret == QDialog::Accepted) {
            QStringList openFiles = fileDialog.selectedFiles();
            outputFile.append(openFiles[0]);
        }
        if (outputFile.isEmpty()) {
            delete enc;
            enc = NULL;
            return;
        } else {
            if (!outputFile.endsWith(exportSuffix)) {
                outputFile.append(".");
                outputFile.append(exportSuffix);
            }
            enc->setOutputFile(outputFile);
        }
    } else {
        QString  outputFileName;
        int      videoFormat = VideoEncoder::noneFormat;

        outputFileName = this->frontend->getProject()->getDefaultOutputFileName();
        if (outputFileName.isEmpty()) {
            frontend->showWarning(tr("Warning"),
                                  tr("No default output file name defined.\n"
                                     "Check your settings in the properties dialo of the project!"));
            return;
        }

        videoFormat = this->frontend->getProject()->getVideoFormat();
        switch(videoFormat) {
        case VideoEncoder::aviFormat:
            if (outputFileName.indexOf(".avi") == -1) {
                outputFileName.append(".avi");
            }
            break;
        case VideoEncoder::mp4Format:
            if (outputFileName.indexOf(".mp4") == -1 ) {
                outputFileName.append(".mp4");
            }
            break;
        }
        enc->setOutputFile(outputFileName);
    }

    // Remove an existing file
    QFile::remove(enc->getOutputFile());

    if (!enc->isValid()) {
        frontend->showWarning(tr("Warning"),
                              tr("The selected encoder is not valid.\n"
                                 "Check your settings in the properties dialog of the project!"));
        delete enc;
        enc = NULL;
        return;
    }
    checkSaved();

    frontend->showProgress(tr("Exporting ..."), frontend->getProject()->getTotalExposureSize());
    frontend->getProject()->exportToVideo(enc);
    frontend->hideProgress();

    delete enc;
    enc = NULL;

    qDebug("MainWindowGUI::exportToVideo --> End");
}


void MainWindowGUI::exportToCinelerra()
{
    QString outputFile;

    recordingTab->checkCameraOff();

    QFileDialog fileDialog(this, tr("Export to file"), lastVisitedDir);
    QStringList filters;
    filters << "Cinelerra (*.XML)";
    fileDialog.setFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    int ret = fileDialog.exec();
    if (ret == QDialog::Accepted) {
        QStringList openFiles = fileDialog.selectedFiles();
        outputFile.append(openFiles[0]);
    }

    if (!outputFile.isNull()) {
        frontend->getProject()->exportToCinelerra(outputFile);
    }
}


void MainWindowGUI::closeApplication()
{
    // Switch off the camera
    recordingTab->checkCameraOff();

    // Last changes saved?
    checkSaved();

    // remove all temporary files and directories
    frontend->removeApplicationFiles();
    frontend->removeApplicationDirectories();

    // Exit the application with status success
    // TODO: This exit cancel the application without calling finalize of the QtFrontend class!!!
    exit(0);
}

// TODO: New implementation of copy necessary
void MainWindowGUI::copy()
{
    /*
    QList<QUrl> urls;

    // DO: return the scene, take and frame from the frame bar!!!!
    int selectionFrame = timeLine->getSelectionFrame();
    int activeFrame = 1; // frontend->getProject()->getActiveExposureIndex();
    int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
    int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;

    for (int i = lowend; i <= highend; ++i) {
        // DO: Get the filename from the frame bar
        Exposure* exposure = frontend->getProject()->getExposure(1, 1, i);
        urls.append(QUrl::fromLocalFile(exposure->getImagePath()));
    }

    //QDrag *drag = new QDrag((MainWindowGUI*)this->parent());
    QMimeData *mimeData = new QMimeData;

    mimeData->setUrls(urls);
    //drag->setMimeData(mimeData);

    //drag->start(Qt::MoveAction);
    QApplication::clipboard()->setMimeData(mimeData);
    */
}

// TODO: New implementation of past necessary
void MainWindowGUI::paste()
{
    /*
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    if (mimeData->hasUrls()) {
        QStringList fileNames;
        QList<QUrl> urls = mimeData->urls();
        int numFrames = urls.size();
        for (int i = 0; i < numFrames; ++i) {
            fileNames.append(urls[i].toLocalFile());
        }
        emit addFrames(fileNames);
    }
    */
}

// TODO: New Implementation of add sound
void MainWindowGUI::addSound()
{
    /*
    QString openFile;

    QFileDialog fileDialog(this, tr("Choose sound file"), lastVisitedDir);
    QStringList filters;
    filters << tr("Sounds (*.ogg)");
    fileDialog.setFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    int ret = fileDialog.exec();
    if (ret == QDialog::Accepted) {
        QStringList openFiles = fileDialog.selectedFiles();
        openFile.append(openFiles[0]);
    }

    if (!openFile.isNull()) {
        DomainFacade *facade = frontend->getProject();
        bool ok = false;
        int ret = 0;
        QString text = QInputDialog::getText(0, tr("Sound name"), tr("Enter the name of the sound:"),
                                             QLineEdit::Normal, QString::null, &ok);
        if (ok && !text.isEmpty()) {
            ret = facade->addSoundToScene(facade->getActiveSceneIndex(), openFile, text);
        }
        else {
            ret = facade->addSoundToScene(facade->getActiveSceneIndex(), openFile, QString());
        }

        if (ret == 0) {
            Scene *scene = facade->getScene(facade->getActiveSceneIndex());
            if (!scene->isEmpty()) {
                soundsList->insertItem(soundsList->count(),
                                       new QListWidgetItem(scene->getSoundName(soundsList->count())));
                soundsList->item(soundsList->currentRow())->setText(text);
                emit soundsChanged();
            }
        }
    }
    */
}

// TODO: New Implementation of remove sound
void MainWindowGUI::removeSound()
{
    /*
    int index = soundsList->currentRow();
    if (index >= 0) {
        frontend->getProject()->removeSoundFromScene(frontend->getProject()->getActiveSceneIndex(), index);
        QListWidgetItem *qlwi = soundsList->takeItem(index);
        delete qlwi;
        emit soundsChanged();
    }
    */
}


void MainWindowGUI::showPreferencesDialog()
{
    GeneralDialog *preferencesDialog;

    preferencesDialog = new GeneralDialog(frontend);
    int ret = preferencesDialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("MainWindowsGUI::showPreferencesDialog --> End (cancel)");
    }
    delete(preferencesDialog);
}


void MainWindowGUI::showPropertiesDialog()
{
    ProjectDialog *propertiesDialog;

    propertiesDialog = new ProjectDialog(frontend);
    int ret = propertiesDialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("MainWindowsGUI::showPropertiesDialog --> End (cancel)");
    }
    delete(propertiesDialog);
}


void MainWindowGUI::showUndoStack()
{
    QRect fGeo = this->frameGeometry();

    // Open up undo stack window
    if (undoView == 0) {
        undoView = new QUndoView(frontend->getProject()->getUndoStack());
        undoView->setWindowTitle(tr("qStopMotion - Undo stack"));
        undoView->setAttribute(Qt::WA_QuitOnClose, false);
        undoView->setGeometry(geometry().x() + fGeo.width(), geometry().y(),
                              250, height());
    }
    undoView->show();
    this->activateWindow();
}


void MainWindowGUI::showCameraControllerDialog()
{
    QRect fGeo = this->frameGeometry();

    Q_ASSERT(grabber->getDevice(getVideoSource())->getController() != NULL);

    if (cameraControllerDialog == 0) {
        cameraControllerDialog = new CameraControllerDialog(frontend,
                                                            grabber->getDevice(getVideoSource()),
                                                            this);
        cameraControllerDialog->initialize();
        cameraControllerDialog->setGeometry(geometry().x() + fGeo.width(), geometry().y(),
                                            200, height());
    }
    cameraControllerDialog->show();

    if (frontend->isGrabberInited()) {
        cameraControllerDialog->setUp();
    }

}


void MainWindowGUI::whatsThis()
{
    QWhatsThis::enterWhatsThisMode();
}


void MainWindowGUI::showHelpDialog()
{
    if (helpBrowser == 0) {
        helpBrowser = new HelpBrowser(frontend, this);
        helpBrowser->setGeometry(this->x() + 40, this->y() + 40, this->width(), this->height());
    }
    helpBrowser->show();
}


void MainWindowGUI::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(frontend, this);
    aboutDialog->show();
}


/**************************************************************************
 * Private functions
 **************************************************************************/

void MainWindowGUI::createTranslator(const QString &newLocale)
{
    qDebug("MainWindowGUI::createTranslator --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    QString appTranslationFile("qstopmotion_");
    QString qtTranslationFile("qt_");
    QString qmPath(frontend->getTranslationsDirName());
    QString languagePref;

    if (pref->getStringPreference("preferences", "language", languagePref) == false) {
        languagePref.append(newLocale);
    }
    if (newLocale.isEmpty()) {
        if (languagePref.isEmpty()) {
            // Get system locale.
            languagePref = QLocale::system().name().toLower();
            if (languagePref == QLatin1String("nb_no"))
                languagePref = QLatin1String("no_nb");
            else if (languagePref == QLatin1String("nn_no"))
                languagePref = QLatin1String("no_nn");
            else if (languagePref == QLatin1String("se_no"))
                languagePref = QLatin1String("no_se");
            else
                languagePref.truncate(2);

            pref->setStringPreference("preferences", "language", languagePref);
        }

        appTranslationFile.append(languagePref);
        qtTranslationFile.append(languagePref);
    }
    else {
        appTranslationFile.append(newLocale);
        qtTranslationFile.append(newLocale);
    }

    if (!appTranslationFile.isEmpty()) {
        qDebug() << "MainWindowGUI::createTranslator --> Loading translator:" << appTranslationFile;
        if (!appTranslator.load(appTranslationFile, qmPath)) {
            // Translation file not opend
            qDebug("MainWindowGUI::createTranslator --> qStopMotion translator not loaded!");
        }
        qDebug() << "MainWindowGUI::createTranslator --> Loading translator:" << qtTranslationFile;
        if (!qtTranslator.load(qtTranslationFile, qmPath)) {
            // Translation file not opend
            qDebug("MainWindowGUI::createTranslator --> Qt translator not loaded!");
        }
    }

    qDebug("MainWindowGUI::createTranslator --> End");
}


void MainWindowGUI::initTranslations()
{
    qDebug("MainWindowGUI::initTranslations --> Start");

    // Fill the list with possible languages
    QString qmPath(frontend->getTranslationsDirName());
    QDir dir(qmPath);
    QStringList fileNames = dir.entryList(QStringList("qstopmotion_*.qm"));

    //English is a special case (base language)
    translationsLanguages.append("English");
    translationsLocales.append("en");

    for (int i = 0; i < fileNames.size(); ++i) {
        QTranslator translator;
        translator.load(fileNames[i], qmPath);

        QString language = translator.translate("MainWindowGUI", "English",
                                                "This should be translated to the name of the "
                                                "language you are translating to, in that language. "
                                                "Example: English = Deutsch (Deutsch is \"German\" "
                                                "in German)");

        // Checks that the mimimum requirement for accepting a string is covered.
        // The mimimum requirement is that the menu option string (English) is translated.
        if (language != "") {
            QString locale = fileNames[i];
            locale = locale.mid(locale.indexOf('_') + 1);
            locale.truncate(locale.indexOf('.'));
            translationsLanguages.append(language);
            translationsLocales.append(locale);
        }
    }

    qDebug("MainWindowGUI::initTranslations --> End");
}


void MainWindowGUI:: createAccelerators()
{
    // First Exposure
    QShortcut *firstExposureAccel1 = new QShortcut(QKeySequence(Qt::Key_3), this);
    connect(firstExposureAccel1, SIGNAL(activated()), toolBar, SLOT(selectFirstFrame()));
    QShortcut *firstExposureAccel2 = new QShortcut(QKeySequence(Qt::Key_PageDown), this);
    connect(firstExposureAccel2, SIGNAL(activated()), toolBar, SLOT(selectFirstFrame()));
    QShortcut *firstExposureAccel3 = new QShortcut(QKeySequence(Qt::Key_L), this);
    connect(firstExposureAccel3, SIGNAL(activated()), toolBar, SLOT(selectFirstFrame()));

    // Previous Exposure
    QShortcut *previousExposureAccel = new QShortcut(QKeySequence(Qt::Key_6), this);
    connect(previousExposureAccel, SIGNAL(activated()), toolBar, SLOT(selectPreviousFrame()));
    QShortcut *previousExposureAccel2 = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(previousExposureAccel2, SIGNAL(activated()), toolBar, SLOT(selectPreviousFrame()));
    QShortcut *previousExposureAcce3 = new QShortcut(QKeySequence(Qt::Key_O), this);
    connect(previousExposureAcce3, SIGNAL(activated()), toolBar, SLOT(selectPreviousFrame()));

    // Next Exposure
    QShortcut *nextExposureAccel1 = new QShortcut(QKeySequence(Qt::Key_9), this);
    connect(nextExposureAccel1, SIGNAL(activated()), toolBar, SLOT(selectNextFrame()));
    QShortcut *nextExposureAccel2 = new QShortcut(QKeySequence(Qt::Key_PageUp), this);
    connect(nextExposureAccel2, SIGNAL(activated()), toolBar, SLOT(selectNextFrame()));

    // Last Exposure
    QShortcut *lastExposureAccel1 = new QShortcut(QKeySequence(Qt::Key_Asterisk), this);
    connect(lastExposureAccel1, SIGNAL(activated()), toolBar, SLOT(selectLastFrame()));
    QShortcut *lastExposureAccel2 = new QShortcut(QKeySequence(Qt::Key_P), this);
    connect(lastExposureAccel2, SIGNAL(activated()), toolBar, SLOT(selectLastFrame()));

    // Add exposure
    QShortcut *captureAccel1 = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(captureAccel1, SIGNAL(activated()), recordingTab, SLOT(captureFrame()));
    QShortcut *captureAccel2 = new QShortcut(QKeySequence(Qt::Key_Return), this);     // Return key on the keyboard
    connect(captureAccel2, SIGNAL(activated()), recordingTab, SLOT(captureFrame()));
    QShortcut *captureAccel3 = new QShortcut(QKeySequence(Qt::Key_Enter), this);      // Enter key on the keypad
    connect(captureAccel3, SIGNAL(activated()), recordingTab, SLOT(captureFrame()));

    // Delete exposure
    QShortcut *removeFramesAccel = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(removeFramesAccel, SIGNAL(activated()), projectTab, SLOT(removeFramesSlot()));

    // First take
    QShortcut *firstTakeAccel1 = new QShortcut(QKeySequence(Qt::Key_2), this);
    connect(firstTakeAccel1, SIGNAL(activated()), toolBar, SLOT(selectFirstTake()));
    QShortcut *firstTakeAccel2 = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(firstTakeAccel2, SIGNAL(activated()), toolBar, SLOT(selectFirstTake()));
    QShortcut *firstTakeAccel3 = new QShortcut(QKeySequence(Qt::Key_K), this);
    connect(firstTakeAccel3, SIGNAL(activated()), toolBar, SLOT(selectFirstTake()));

    // Previous take
    QShortcut *prevTakeAccel1 = new QShortcut(QKeySequence(Qt::Key_5), this);
    connect(prevTakeAccel1, SIGNAL(activated()), toolBar, SLOT(selectPreviousTake()));
    QShortcut *prevTakeAccel2 = new QShortcut(QKeySequence(Qt::Key_Clear), this);
    connect(prevTakeAccel2, SIGNAL(activated()), toolBar, SLOT(selectPreviousTake()));
    QShortcut *prevTakeAccel3 = new QShortcut(QKeySequence(Qt::Key_I), this);
    connect(prevTakeAccel3, SIGNAL(activated()), toolBar, SLOT(selectPreviousTake()));

    // Next take
    QShortcut *nextTakeAccel1 = new QShortcut(QKeySequence(Qt::Key_8), this);
    connect(nextTakeAccel1, SIGNAL(activated()), toolBar, SLOT(selectNextTake()));
    QShortcut *nextTakeAccel2 = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(nextTakeAccel2, SIGNAL(activated()), toolBar, SLOT(selectNextTake()));

    // Last take
    QShortcut *lastTakeAccel1 = new QShortcut(QKeySequence(Qt::Key_Slash), this);
    connect(lastTakeAccel1, SIGNAL(activated()), toolBar, SLOT(selectLastTake()));

    // Add take
    QShortcut *addTakeAccel1 = new QShortcut(QKeySequence(Qt::Key_Plus), this);
    connect(addTakeAccel1, SIGNAL(activated()), projectTab, SLOT(addTakeSlot()));

    // First scene
    QShortcut *firstSceneAccel1 = new QShortcut(QKeySequence(Qt::Key_0), this);
    connect(firstSceneAccel1, SIGNAL(activated()), toolBar, SLOT(selectFirstScene()));
    QShortcut *firstSceneAccel2 = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(firstSceneAccel2, SIGNAL(activated()), toolBar, SLOT(selectFirstScene()));
    QShortcut *firstSceneAccel3 = new QShortcut(QKeySequence(Qt::Key_M), this);
    connect(firstSceneAccel3, SIGNAL(activated()), toolBar, SLOT(selectFirstScene()));

    // Previous scene
    QShortcut *prevSceneAccel1 = new QShortcut(QKeySequence(Qt::Key_1), this);
    connect(prevSceneAccel1, SIGNAL(activated()), toolBar, SLOT(selectPreviousScene()));
    QShortcut *prevSceneAccel2 = new QShortcut(QKeySequence(Qt::Key_End), this);
    connect(prevSceneAccel2, SIGNAL(activated()), toolBar, SLOT(selectPreviousScene()));
    QShortcut *prevSceneAccel3 = new QShortcut(QKeySequence(Qt::Key_J), this);
    connect(prevSceneAccel3, SIGNAL(activated()), toolBar, SLOT(selectPreviousScene()));

    // Next scene
    QShortcut *nextSceneAccel1 = new QShortcut(QKeySequence(Qt::Key_4), this);
    connect(nextSceneAccel1, SIGNAL(activated()), toolBar, SLOT(selectNextScene()));
    QShortcut *nextSceneAccel2 = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(nextSceneAccel2, SIGNAL(activated()), toolBar, SLOT(selectNextScene()));
    QShortcut *nextSceneAccel3 = new QShortcut(QKeySequence(Qt::Key_U), this);
    connect(nextSceneAccel3, SIGNAL(activated()), toolBar, SLOT(selectNextScene()));

    // Last scene
    QShortcut *lastSceneAccel1 = new QShortcut(QKeySequence(Qt::Key_7), this);
    connect(lastSceneAccel1, SIGNAL(activated()), toolBar, SLOT(selectLastScene()));
    QShortcut *lastSceneAccel2 = new QShortcut(QKeySequence(Qt::Key_Home), this);
    connect(lastSceneAccel2, SIGNAL(activated()), toolBar, SLOT(selectLastScene()));

    // Add scene
    QShortcut *addSceneAccel1 = new QShortcut(QKeySequence(Qt::Key_Minus), this);
    connect(addSceneAccel1, SIGNAL(activated()), projectTab, SLOT(addSceneSlot()));
    QShortcut *addSceneAccel2 = new QShortcut(QKeySequence(Qt::Key_Odiaeresis), this);
    connect(addSceneAccel2, SIGNAL(activated()), projectTab, SLOT(addSceneSlot()));

    // Camera button clicked
    QShortcut *toggleCameraAccel = new QShortcut(QKeySequence(Qt::Key_C), this);
    connect(toggleCameraAccel, SIGNAL(activated()), recordingTab, SLOT(cameraButtonClicked()));

    /* Test test test

    QShortcut *captureAccel4 = new QShortcut(QKeySequence(Qt::Key_5), this);
    connect(captureAccel4, SIGNAL(activated()), recordingTab, SLOT(captureFrame()));
    QShortcut *captureAccel5 = new QShortcut(QKeySequence(Qt::KeypadModifier + Qt::Key_1), this);
    connect(captureAccel5, SIGNAL(activated()), recordingTab, SLOT(captureFrame()));

    QShortcut *removeSceneAccel = new QShortcut(QKeySequence(Qt::ShiftModifier + Qt::Key_Delete), this);
    connect(removeSceneAccel, SIGNAL(activated()), projectTab, SLOT(removeSceneSlot()));

    QShortcut *addExposureAccel = new QShortcut(QKeySequence(Qt::Key_F), this);
    connect(addExposureAccel, SIGNAL(activated()), projectTab, SLOT(addFramesSlot()));
    */
}


void MainWindowGUI::createActions()
{
    QString iconFile(frontend->getIconsDirName());

    // File menu
    newAct = new QAction(this);
    iconFile.append(QLatin1String("filenew.png"));
    newAct->setIcon(QIcon(iconFile));
    newAct->setShortcut(ControlModifier + Key_N);
    newAct->setIconVisibleInMenu(true);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newProject()));

    openAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("fileopen.png"));
    openAct->setIcon(QPixmap(iconFile));
    openAct->setShortcut(ControlModifier + Key_O);
    openAct->setIconVisibleInMenu(true);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openProject()));

    mostRecentAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("project1.png"));
    mostRecentAct->setIcon(QIcon(iconFile));
    mostRecentAct->setIconVisibleInMenu(true);
    connect(mostRecentAct, SIGNAL(triggered()), this, SLOT(openMostRecent()));

    secondMostRecentAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("project2.png"));
    secondMostRecentAct->setIcon(QIcon(iconFile));
    secondMostRecentAct->setIconVisibleInMenu(true);
    connect(secondMostRecentAct, SIGNAL(triggered()), this, SLOT(openSecondMostRecent()));

    thirdMostRecentAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("project3.png"));
    thirdMostRecentAct->setIcon(QIcon(iconFile));
    thirdMostRecentAct->setIconVisibleInMenu(true);
    connect(thirdMostRecentAct, SIGNAL(triggered()), this, SLOT(openThirdMostRecent()));

    fourthMostRecentAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("project4.png"));
    fourthMostRecentAct->setIcon(QIcon(iconFile));
    fourthMostRecentAct->setIconVisibleInMenu(true);
    connect(fourthMostRecentAct, SIGNAL(triggered()), this, SLOT(openFourthMostRecent()));

    saveAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("filesave.png"));
    saveAct->setIcon(QIcon(iconFile));
    saveAct->setShortcut(ControlModifier + Key_S);
    saveAct->setIconVisibleInMenu(true);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    saveAsAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("filesaveas.png"));
    saveAsAct->setIcon(QIcon(iconFile));
    saveAsAct->setShortcut(ControlModifier + ShiftModifier + Key_S);
    saveAsAct->setIconVisibleInMenu(true);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveProjectAs()));

    videoAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("videoexport.png"));
    videoAct->setIcon(QIcon(iconFile));
    videoAct->setShortcut(ControlModifier + ALT + Key_V);
    videoAct->setIconVisibleInMenu(true);
    connect(videoAct, SIGNAL(triggered()), this, SLOT(exportToVideo()));

    cinelerraAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    cinelerraAct->setIcon(QIcon(iconFile));
    cinelerraAct->setShortcut(ControlModifier + ALT + Key_C);
    cinelerraAct->setEnabled(false);
    cinelerraAct->setIconVisibleInMenu(true);
    connect(cinelerraAct, SIGNAL(triggered()), this, SLOT(exportToCinelerra()));

    propertiesAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("configure.png"));
    propertiesAct->setIcon(QIcon(iconFile));
    propertiesAct->setShortcut(ControlModifier + Key_P);
    propertiesAct->setIconVisibleInMenu(true);
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(showPropertiesDialog()));

    quitAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("quit.png"));
    quitAct->setIcon(QIcon(iconFile));
    quitAct->setShortcut(ControlModifier + Key_Q);
    quitAct->setIconVisibleInMenu(true);
    // connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(closeApplication()));

    // Edit menu
    undoAct = frontend->getProject()->getUndoStack()->createUndoAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("undo.png"));
    undoAct->setIcon(QIcon(iconFile));
    undoAct->setShortcut(ControlModifier + Key_Z);
    undoAct->setIconVisibleInMenu(true);

    redoAct = frontend->getProject()->getUndoStack()->createRedoAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("redo.png"));
    redoAct->setIcon(QIcon(iconFile));
    redoAct->setShortcut(ControlModifier + ShiftModifier + Key_Z);
    redoAct->setIconVisibleInMenu(true);

    insertSceneAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("insertscene.png"));
    insertSceneAct->setIcon(QIcon(iconFile));
    // insertSceneAct->setShortcut(ControlModifier + Key_Q);
    insertSceneAct->setIconVisibleInMenu(true);
    connect(insertSceneAct, SIGNAL(triggered()), projectTab, SLOT(insertSceneSlot()));

    addSceneAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("addscene.png"));
    addSceneAct->setIcon(QIcon(iconFile));
    // addSceneAct->setShortcut(ControlModifier + Key_Q);
    addSceneAct->setIconVisibleInMenu(true);
    connect(addSceneAct, SIGNAL(triggered()), projectTab, SLOT(addSceneSlot()));

    removeSceneAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removescene.png"));
    removeSceneAct->setIcon(QIcon(iconFile));
    // removeSceneAct->setShortcut(ControlModifier + Key_Q);
    removeSceneAct->setIconVisibleInMenu(true);
    connect(removeSceneAct, SIGNAL(triggered()), projectTab, SLOT(removeSceneSlot()));

    insertTakeAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("inserttake.png"));
    insertTakeAct->setIcon(QIcon(iconFile));
    // insertTakeAct->setShortcut(ControlModifier + Key_Q);
    insertTakeAct->setIconVisibleInMenu(true);
    connect(insertTakeAct, SIGNAL(triggered()), projectTab, SLOT(insertTakeSlot()));

    addTakeAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("addtake.png"));
    addTakeAct->setIcon(QIcon(iconFile));
    // addTakeAct->setShortcut(ControlModifier + Key_Q);
    addTakeAct->setIconVisibleInMenu(true);
    connect(addSceneAct, SIGNAL(triggered()), projectTab, SLOT(addTakeSlot()));

    removeTakeAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removetake.png"));
    removeTakeAct->setIcon(QIcon(iconFile));
    // removeTakeAct->setShortcut(ControlModifier + Key_Q);
    removeTakeAct->setIconVisibleInMenu(true);
    connect(removeTakeAct, SIGNAL(triggered()), projectTab, SLOT(removeTakeSlot()));

    insertFramesAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("insertframes.png"));
    insertFramesAct->setIcon(QIcon(iconFile));
    // insertFramesAct->setShortcut(ControlModifier + Key_Q);
    insertFramesAct->setIconVisibleInMenu(true);
    connect(insertFramesAct, SIGNAL(triggered()), projectTab, SLOT(insertFramesSlot()));

    addFramesAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("addframes.png"));
    addFramesAct->setIcon(QIcon(iconFile));
    // addFramesAct->setShortcut(ControlModifier + Key_Q);
    addFramesAct->setIconVisibleInMenu(true);
    connect(addFramesAct, SIGNAL(triggered()), projectTab, SLOT(addFramesSlot()));

    removeFramesAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removeframes.png"));
    removeFramesAct->setIcon(QIcon(iconFile));
    // removeFramesAct->setShortcut(ControlModifier + Key_Q);
    removeFramesAct->setIconVisibleInMenu(true);
    connect(removeFramesAct, SIGNAL(triggered()), projectTab, SLOT(removeFramesSlot()));

    /*
    cutAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("cut.png"));
    cutAct->setIcon(QIcon(iconFile));
    cutAct->setIconVisibleInMenu(true);
    cutAct->setEnabled(false);
    cutAct->setShortcut(ControlModifier + Key_X);

    copyAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("copy.png"));
    copyAct->setIcon(QIcon(iconFile));
    copyAct->setShortcut(ControlModifier + Key_C);
    copyAct->setIconVisibleInMenu(true);
    copyAct->setEnabled(false);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("paste.png"));
    pasteAct->setIcon(QIcon(iconFile));
    pasteAct->setShortcut(ControlModifier + Key_V);
    pasteAct->setIconVisibleInMenu(true);
    pasteAct->setEnabled(false);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
*/
    preferencesAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("configure.png"));
    preferencesAct->setIcon(QIcon(iconFile));
    preferencesAct->setShortcut(ControlModifier + Key_P);
    preferencesAct->setIconVisibleInMenu(true);
    connect(preferencesAct, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

    // Windows menu
    undoViewAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("undoview.png"));
    undoViewAct->setIcon(QIcon(iconFile));
    undoViewAct->setShortcut(ControlModifier + Key_U);
    undoViewAct->setIconVisibleInMenu(true);
    connect(undoViewAct, SIGNAL(triggered()), this, SLOT(showUndoStack()));

    cameraControllerAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("undoview.png"));
    cameraControllerAct->setIcon(QIcon(iconFile));
    cameraControllerAct->setShortcut(ControlModifier + Key_C);
    cameraControllerAct->setIconVisibleInMenu(true);
    connect(cameraControllerAct, SIGNAL(triggered()), this, SLOT(showCameraControllerDialog()));

    // Help menu
    whatsthisAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("whatsthis.png"));
    whatsthisAct->setIcon(QIcon(iconFile));
    whatsthisAct->setShortcut(ShiftModifier + Key_F1);
    whatsthisAct->setIconVisibleInMenu(true);
    connect(whatsthisAct, SIGNAL(triggered()), this, SLOT(whatsThis()));

    helpAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    helpAct->setIcon(QIcon(iconFile));
    helpAct->setShortcut(Key_F1);
    helpAct->setIconVisibleInMenu(true);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(showHelpDialog()));

    aboutQtAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("qtlogo.png"));
    aboutQtAct->setIcon(QIcon(iconFile));
    aboutQtAct->setIconVisibleInMenu(true);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    aboutAct->setIcon(QIcon(iconFile));
    aboutAct->setIconVisibleInMenu(true);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}


void MainWindowGUI::createMenus()
{
    // Export menu
    exportMenu = new QMenu(this);
    exportMenu->addAction(videoAct);
    exportMenu->addAction(cinelerraAct);

    // Most recent menu
    mostRecentMenu = new QMenu(this);

    // File menu
    fileMenu = new QMenu(this);
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(mostRecentMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addMenu(exportMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(propertiesAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
    menuBar()->addMenu(fileMenu);

    // Edit menu
    editMenu = new QMenu(this);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(insertSceneAct);
    editMenu->addAction(addSceneAct);
    editMenu->addAction(removeSceneAct);
    editMenu->addSeparator();
    editMenu->addAction(insertTakeAct);
    editMenu->addAction(addTakeAct);
    editMenu->addAction(removeTakeAct);
    editMenu->addSeparator();
    editMenu->addAction(insertFramesAct);
    editMenu->addAction(addFramesAct);
    editMenu->addAction(removeFramesAct);
    // editMenu->addSeparator();
    // editMenu->addAction(cutAct);
    // editMenu->addAction(copyAct);
    // editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(preferencesAct);
    menuBar()->addMenu(editMenu);

    // Windows menu
    windowsMenu = new QMenu(this);
    windowsMenu->addAction(undoViewAct);
    windowsMenu->addAction(cameraControllerAct);
    menuBar()->addMenu(windowsMenu);

    // Help menu
    helpMenu = new QMenu(this);
    helpMenu->addAction(whatsthisAct);
    helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(aboutAct);
    menuBar()->addMenu(helpMenu);
}


void MainWindowGUI::makeToolsMenu(QHBoxLayout *layout)
{
    QString iconFile(frontend->getIconsDirName());

    sideBar = new QTabWidget();
    sideBar->setObjectName("sideBar");
    sideBar->setTabPosition(QTabWidget::South);
    sideBar->setMaximumWidth(250);
    sideBar->setMinimumWidth(200);
    layout->addWidget(sideBar);

    Q_ASSERT(frontend != 0);

    recordingTab = new RecordingTab(frontend,
                                    toolBar,
                                    this);
    recordingTab->initialize();
    iconFile.append(QLatin1String("clapper.png"));
    sideBar->addTab(recordingTab, QIcon(iconFile), "Recording");

    projectTab = new ProjectTab(frontend,
                                lastVisitedDir);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("view.png"));
    sideBar->addTab(projectTab, QIcon(iconFile), "Project");
/*
    viewTab = new ViewTab(frontend);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("view.png"));
    sideBar->addTab(viewTab, QIcon(iconFile), "View");

    compositingTab = new CompositingTab(frontend);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("compositing.png"));
    sideBar->addTab(compositingTab, QIcon(iconFile), "Compositing");
*/
}

/*
void MainWindowGUI::makeGotoMenu(QVBoxLayout *layout)
{
    QString iconFile(frontend->getIconsDirName());

    gotoMenuWidget = new QWidget;
    gotoMenuWidget->setObjectName("gotoMenuWidget");
    gotoMenuWidgetLayout = new QHBoxLayout;
    gotoMenuWidgetLayout->setSpacing(5);
    gotoMenuWidgetLayout->setMargin(0);

    gotoFrameLabel = new QLabel;

    gotoSpinner = new FlexibleSpinBox;
    gotoSpinner->setMaximumWidth(60);
    gotoSpinner->setRange(1, 1);

    gotoMenuCloseButton = new QPushButton;
    iconFile.append(QLatin1String("close.png"));
    gotoMenuCloseButton->setIcon(QPixmap(iconFile));
    gotoMenuCloseButton->setFlat(true);
    gotoMenuCloseButton->setMinimumSize(16, 16);
    gotoMenuCloseButton->setMaximumSize(16, 16);
    connect(gotoMenuCloseButton, SIGNAL(clicked()), gotoMenuWidget, SLOT(hide()));

    gotoMenuWidgetLayout->addWidget(gotoFrameLabel);
    gotoMenuWidgetLayout->addWidget(gotoSpinner);
    gotoMenuWidgetLayout->addStretch();
    gotoMenuWidgetLayout->addWidget(gotoMenuCloseButton);
    gotoMenuWidget->setLayout(gotoMenuWidgetLayout);

    layout->addWidget(gotoMenuWidget);
    gotoMenuWidget->hide();
}
*/

void MainWindowGUI::makeViews(QHBoxLayout *layout)
{
    viewArea = new QWidget;
    viewArea->setObjectName("ViewArea");
    viewAreaLayout = new QVBoxLayout;
    viewAreaLayout->setObjectName("ViewAreaLayout");
    // viewAreaLayout->setSpacing(2);
    viewAreaLayout->setMargin(2);

    frameView = new FrameViewImage(frontend, this);

    frameView->setMinimumSize(400, 270);
    frameView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    viewAreaLayout->addWidget(frameView);

    toolBar = new ToolBar(frontend, recordingTab);
    toolBar->setObjectName("ToolBar");
    toolBar->setMinimumSize(400, 30);
    toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    viewAreaLayout->addWidget(toolBar);
    // viewAreaLayout->addWidget(toolBar, 1, Qt::AlignJustify);

    viewArea->setLayout(viewAreaLayout);
    layout->addWidget(viewArea);
}


void MainWindowGUI::makeStatusBar()
{
    projectLabel = new QLabel;
    projectLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    projectLabel->setMargin(0);
    this->statusBar()->addPermanentWidget(projectLabel);

    projectID = new QLabel;
    projectID->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    projectID->setMargin(0);
    this->statusBar()->addPermanentWidget(projectID);

    sceneLabel = new QLabel;
    sceneLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sceneLabel->setMargin(0);
    this->statusBar()->addPermanentWidget(sceneLabel);

    sceneID = new QLabel;
    sceneID->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sceneID->setMargin(0);
    this->statusBar()->addPermanentWidget(sceneID);

    takeLabel = new QLabel;
    takeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    takeLabel->setMargin(0);
    this->statusBar()->addPermanentWidget(takeLabel);

    takeID = new QLabel;
    takeID->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    takeID->setMargin(0);
    this->statusBar()->addPermanentWidget(takeID);

    exposureLabel = new QLabel;
    exposureLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    exposureLabel->setMargin(0);
    this->statusBar()->addPermanentWidget(exposureLabel);

    exposureID = new QLabel;
    exposureID->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    exposureID->setMargin(0);
    this->statusBar()->addPermanentWidget(exposureID);
}


void MainWindowGUI::dragEnterEvent(QDragEnterEvent * /* event */)
{
    qDebug("MainWindowGUI::dragEnterEvent --> Start (Nothing)");
    // TODO: New Implementation for Drag enter event
/*
    if (event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }

    qDebug("MainWindowGUI::dragEnterEvent --> End");
*/
}


void MainWindowGUI::dropEvent(QDropEvent * /* event */)
{
    qDebug("MainWindowGUI::dropEvent --> Start (Nothing)");
    // TODO: New Implementation for Drop event
/*
    if (event->mimeData()->hasUrls()) {
        QStringList fileNames;
        QList<QUrl> urls = event->mimeData()->urls();
        int numFrames = urls.size();
        for (int i = 0; i < numFrames; ++i) {
            fileNames.append(urls[i].toLocalFile());
            projectHandler->addFrames(fileNames);
        }
    }

    qDebug("MainWindowGUI::dropEvent --> End");
*/
}


void MainWindowGUI::keyReleaseEvent(QKeyEvent * k)
{
    switch (k->key()) {
    case Key_Shift: {
        timeLine->setSelecting(false);
        break;
    }
    }
}


void MainWindowGUI::setMostRecentProject()
{
    const QString newFirst = frontend->getProject()->getNewProjectFilePath();

    Q_ASSERT(!newFirst.isEmpty());

    PreferencesTool *pref = frontend->getPreferences();
    pref->removeProject(newFirst);
    pref->addProject(newFirst);
    pref->flushPreferences();
    // Update the most recent menu!
    updateMostRecentMenu();
}


void MainWindowGUI::updateMostRecentMenu()
{
    PreferencesTool *pref = frontend->getPreferences();
    int ProjectFileIndex = 0;
    int MenuIndex = 0;

    mostRecentMenu->clear();

    do {
        QString fileName(pref->getProject(ProjectFileIndex));
        if (fileName.isEmpty()) break;
        if (QFile::exists(fileName)) {
            switch (MenuIndex) {
            case 0:
                mostRecentAct->setVisible(true);
                mostRecentAct->setText(Util::convertPathToOsSpecific(fileName));
                mostRecentMenu->addAction(mostRecentAct);
                break;
            case 1:
                secondMostRecentAct->setVisible(true);
                secondMostRecentAct->setText(Util::convertPathToOsSpecific(fileName));
                mostRecentMenu->addAction(secondMostRecentAct);
                break;
            case 2:
                thirdMostRecentAct->setVisible(true);
                thirdMostRecentAct->setText(Util::convertPathToOsSpecific(fileName));
                mostRecentMenu->addAction(thirdMostRecentAct);
                break;
            case 3:
                fourthMostRecentAct->setVisible(true);
                fourthMostRecentAct->setText(Util::convertPathToOsSpecific(fileName));
                mostRecentMenu->addAction(fourthMostRecentAct);
                break;
            }
            MenuIndex++;
        } else {
            // Project file didn't exist --> remove the entry from the preferences
            pref->removeProject(fileName);
        }
        ProjectFileIndex++;
    } while (1);
}


void MainWindowGUI::checkSaved()
{
    // Last changes saved?
    bool b = frontend->getProject()->isProjectAnimationChanges() || frontend->getProject()->isProjectSettingsChanges();
    if (b) {
        int save = frontend->askQuestion(tr("Unsaved changes"),
                                         tr("There are unsaved changes. Do you want to save?"));
        if (save == 0) { // user pressed button 0, which is 'yes'
            saveProject();
        }
    }
}

