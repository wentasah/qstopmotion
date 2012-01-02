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

#include "mainwindowgui.h"

#include "domain/domainfacade.h"
#include "domain/animation/projectserializer.h"
#include "frontends/qtfrontend/toolbar.h"
#include "frontends/qtfrontend/dialogs/aboutdialog.h"
#include "frontends/qtfrontend/dialogs/startdialog.h"
#include "frontends/qtfrontend/frameview/frameviewimage.h"
#include "technical/preferencestool.h"
#include "technical/util.h"
#include "technical/grabber/gstreamergrabber.h"
#include "technical/videoencoder/ffmpegencoder.h"

#include <QtCore/QtDebug>

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
    quitAct              = 0;
    undoAct              = 0;
    redoAct              = 0;
    cutAct               = 0;
    copyAct              = 0;
    pasteAct             = 0;
    configureAct         = 0;
    whatsthisAct         = 0;
    undoViewAct          = 0;
    aboutAct             = 0;
    helpAct              = 0;

    fileMenu             = 0;
    exportMenu           = 0;
    mostRecentMenu       = 0;
    editMenu             = 0;
    settingsMenu         = 0;
    viewMenu             = 0;
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
    progressBar          = 0;
    infoText             = 0;
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
    undoView             = 0;
    helpBrowser          = 0;

    cameraHandler        = 0;
    editMenuHandler      = 0;
    soundHandler         = 0;
    changeMonitor        = 0;

    grabber              = 0;

    this->setObjectName("MainWindowGUI");
    stApp->installTranslator(&appTranslator);
    stApp->installTranslator(&qtTranslator);

    qDebug("MainWindowGUI::Constructor --> End");
}

void MainWindowGUI::init()
{
    lastVisitedDir.clear();

    QDir homeDir = QDir::home();
    lastVisitedDir.append(homeDir.absolutePath());

    this->initTranslations();
    PreferencesTool *pref = frontend->getPreferences();
    QString activeLocale = pref->getBasicPreference("language", QString());
    createTranslator(activeLocale);

    grabber = new GstreamerGrabber(frontend);

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

    createHandlers();

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

    //Initializes and sets up the menue system.
    createAccelerators();
    createActions();
    createMenus();

    //Mainwindow preferences.
    QString iconFile(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    setWindowIcon(QPixmap(iconFile));

    setContentsMargins(5, 0, 5, 0);
    setAcceptDrops(true);

    statusBar()->showMessage(tr("Ready to rumble ;-)"), 2000);
    statusBar()->setSizeGripEnabled(false);

    // Sets all the text in the program.
    retranslateStrings();

    /* Add another logo here
    QLabel *l = new QLabel(this);
    l->setMaximumSize(150, menuBar()->height() - 5);
    l->setMinimumSize(150, menuBar()->height() - 5);
    l->setScaledContents(true);
    l->setPixmap(qstopmotion_logo);
    l->move(this->width()-150, 0);
    l->show();
    */

    connect(this, SIGNAL(startNewProject()), this, SLOT(newProject()));
    connect(this, SIGNAL(startLastProject()), this, SLOT(openMostRecent()));
    connect(this, SIGNAL(startOpenProject()), this, SLOT(openProject()));
    connect(this, SIGNAL(startExit()),  this, SLOT(closeApplication()));

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


/**
 * @TODO change so that CTRL+A instead of just A selects all the frames.
 */
void MainWindowGUI::keyPressEvent(QKeyEvent *k)
{
    DomainFacade *facade = frontend->getProject();
    int activeScene = facade->getActiveSceneIndex();
    // int activeTake = facade->getActiveTakeIndex();

    switch (k->key()) {
    case Key_Shift:
        timeLine->setSelecting(true);
        break;
    case Key_A:
        if (activeScene >= 0) {
            if (facade->getSceneExposureSize(activeScene) > 1) {
                facade->setActiveExposureIndex(0);
                timeLine->setSelection(facade->getSceneExposureSize(activeScene) - 1);
            }
        }
        break;
    default:
        k->ignore();
        break;
    }
}


void MainWindowGUI::setupDirectoryMonitoring()
{
    qDebug("MainWindowGUI::setupDirectoryMonitoring --> Start");

    changeMonitor = new ExternalChangeMonitor(frontend, this);

    qDebug("MainWindowGUI::setupDirectoryMonitoring --> End");
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
    cameraHandler->changeCaptureButtonFunction(newFunction);
}


void MainWindowGUI::retranslateStrings()
{
    qDebug("MainWindowGUI::retranslateStrings --> Start");

    //The actions caption texts
    newAct->setText(tr("&New"));
    openAct->setText(tr("&Open"));
    saveAct->setText(tr("&Save"));
    saveAsAct->setText(tr("Save &As"));
    videoAct->setText(tr("Video"));
    cinelerraAct->setText(tr("Cinelerra"));
    quitAct->setText(tr("&Quit"));
    undoAct->setText(tr("&Undo"));
    redoAct->setText(tr("Re&do"));
    // cutAct->setText(tr("Cu&t"));
    // copyAct->setText(tr("&Copy"));
    // pasteAct->setText(tr("&Paste"));
    configureAct->setText(tr("&Configure qStopMotion"));
    whatsthisAct->setText(tr("What's &This"));
    undoViewAct->setText(tr("&Undo stack"));
    helpAct->setText(tr("&Help"));
    aboutAct->setText(tr("&About"));

    // Status bar
    projectLabel->setText(tr("Project ID: "));
    sceneLabel->setText(tr("Scene ID: "));
    takeLabel->setText(tr("Take ID: "));
    exposureLabel->setText(tr("Exposure ID: "));

    //Tooltip and whatsthis texts
    retranslateHelpText();

    //The menus
    menuBar()->clear();
    menuBar()->addMenu(fileMenu);

    fileMenu->clear();
    fileMenu->setTitle(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(mostRecentMenu);

    updateMostRecentMenu();

    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addMenu(exportMenu);

    exportMenu->clear();
    exportMenu->setTitle(tr("&Export"));
    exportMenu->addAction(videoAct);
    exportMenu->addAction(cinelerraAct);

    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    // Edit menu
    editMenu->setTitle(tr("&Edit"));
    menuBar()->addMenu(editMenu);

    // Settings menu
    settingsMenu->clear();
    settingsMenu->setTitle(tr("&Settings"));
    settingsMenu->addAction(configureAct);
    menuBar()->addMenu(settingsMenu);

    // View menu
    viewMenu->setTitle(tr("&View"));
    menuBar()->addMenu(viewMenu);

    // Help menu
    helpMenu->setTitle(tr("&Help"));
    menuBar()->addMenu(helpMenu);

    // The submenus
    toolBar->retranslateStrings();
    recordingTab->retranslateStrings();
    projectTab->retranslateStrings();
    // viewTab->retranslateStrings();
    // compositingTab->retranslateStrings();

    qDebug("MainWindowGUI::retranslateStrings --> End");
}


void MainWindowGUI::showProgress(const QString &operation, unsigned int numOperations)
{
    if (numOperations > 0) {
        progressDialog = new QProgressDialog(operation, tr("Cancel"), 0, numOperations, this);
        progressDialog->show();
    } else {
        infoText = new QLabel(operation);
        statusBar()->addWidget(infoText);

        // progressBar = new QProgressBar;
        // progressBar->setFixedWidth(150);
        // progressBar->setRange(0, 100);
        // progressBar->setValue(50);
        // statusBar()->addWidget(progressBar);

        // timer = new QTimer();
        // connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
        // timer->start(10);
    }
}


void MainWindowGUI::hideProgress()
{
    if (progressDialog) {
        progressDialog->hide();
        delete progressDialog;
        progressDialog = NULL;
    } else if (progressBar) {
        // timer->stop();
        // delete timer;
        // timer = NULL;

        // progressBar->hide();
        // statusBar()->removeWidget(progressBar);
        // delete progressBar;
        // progressBar = NULL;

        statusBar()->removeWidget(infoText);
        delete infoText;
        infoText = NULL;
    }
}


void MainWindowGUI::updateProgress(int numOperationsDone)
{
    if (progressDialog) {
        progressDialog->setValue(numOperationsDone);
    }
}


void MainWindowGUI::setProgressInfo(const char *infoText)
{
    if (progressDialog) {
        progressDialog->setLabelText(infoText);
    }
}


void MainWindowGUI::showMessage(const QString &message, int timeout)
{
    statusBar()->showMessage(message, timeout);
}


void MainWindowGUI::clearMessage()
{
    statusBar()->clearMessage();
}


bool MainWindowGUI::isOperationAborted()
{
    if (progressDialog) {
        return progressDialog->wasCanceled();
    }
    return false;
}


void MainWindowGUI::updateProgressBar()
{
    if (progressBar != 0) {
        int p = progressBar->value();
        progressBar->setValue(++p);
    }
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
    qDebug("MainWindowGUI::on --> Start");

    frontend->showProgress(tr("Connecting camera..."));
    grabber->init();
    if (!grabber->isGrabberInited()) {
        frontend->hideProgress();
        frontend->showWarning(tr("Select image grabber"),
                              tr("You have to define an image grabber to use.\n"
                                 "This can be set in the preferences menu."));
        return false;
    }
    recordingTab->initialize();
    frameView->on();
    frontend->hideProgress();

    emit cameraStateChanged(true);

    qDebug("MainWindowGUI::on --> End");
    return true;
}


void MainWindowGUI::stopGrabber()
{
    qDebug("MainWindowGUI::off --> Start");

    emit cameraStateChanged(false);

    frameView->off();
    grabber->finalize();

    qDebug("MainWindowGUI::off --> End");
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


const QImage MainWindowGUI::getActualImage()
{
    return grabber->getActualImage();
}


/**************************************************************************
 * Public slots
 **************************************************************************/

void MainWindowGUI::modelSizeChanged(int modelSize)
{
    qDebug("MainWindowGUI::modelSizeChanged --> Start (Empty)");

    /*
    if (modelSize == 0) {
        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        // gotoFrameAct->setEnabled(false);
    } else {
        cutAct->setEnabled(true);
        copyAct->setEnabled(true);
        // gotoFrameAct->setEnabled(true);
        saveAsAct->setEnabled(true);
    }

    qDebug("MainWindowGUI::modelSizeChanged --> End");
    */
}


void MainWindowGUI::activateMenuOptions()
{
    qDebug("MainWindowGUI::activateMenuOptions --> Start (Empty)");

    /*
    undoAct->setEnabled(true);
    redoAct->setEnabled(true);

    qDebug("MainWindowGUI::activateMenuOptions --> End");
    */
}


/**************************************************************************
 * Private slots
 **************************************************************************/

void MainWindowGUI::newProject()
{
    qDebug("MainWindowGUI::newProject --> Start");

    DomainFacade* project = frontend->getProject();

    bool b = project->isUnsavedChanges();
    if (b) {
        int save = frontend->askQuestion(tr("Unsaved changes"),
                                         tr("There are unsaved changes. Do you want to save?"));
        if (save == 0) { // user pressed button 0, which is 'yes'
            saveProject();
        }
    }

    DescriptionDialog *dialog = new DescriptionDialog(DescriptionDialog::ProjectDescription);
    dialog->setProjectDescription(tr("The Project"));
    dialog->setSceneDescription("001");
    dialog->setTakeDescription("01");
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("ModelHandler::newScene --> End (cancel)");

        delete(dialog);

        return;
    }

    QString projectDescription = dialog->getProjectDescription();
    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    // Create the new project
    project->newProjectRedo(projectDescription);
    this->setProjectSettingsToDefault();

    // Create and activate the new scene
    project->addSceneRedo(sceneDescription);
    project->setActiveSceneIndex(0);

    // Create and activate the new take
    project->addTakeRedo(0, takeDescription);
    project->setActiveTakeIndex(0);

    //fileMenu->setItemEnabled(SAVE, false);

    modelSizeChanged(0);
    toolBar->modelSizeChanged(0);

    delete(dialog);

    qDebug("MainWindowGUI::newProject --> End");
}


void MainWindowGUI::openProject()
{
    qDebug("MainWindowGUI::openProject --> Start");

    QString openFile = QFileDialog::
                   getOpenFileName(this,
                                   tr("Choose project file"),
                                   lastVisitedDir,
                                   QString(tr("Project (*.%1);;Archive (*.%2)")).arg(PreferencesTool::projectSuffix).arg(PreferencesTool::archiveSuffix));
    if (!openFile.isNull()) {
        openProject(openFile);
    }
    setMostRecentProject();

    qDebug("MainWindowGUI::openProject --> End");
}


void MainWindowGUI::openMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(mostRecentAct->text());
    openProject(fileName);
}


void MainWindowGUI::openSecondMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(secondMostRecentAct->text());
    openProject(fileName);
    setMostRecentProject();
}


void MainWindowGUI::openThirdMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(thirdMostRecentAct->text());
    openProject(fileName);
    setMostRecentProject();
}


void MainWindowGUI::openFourthMostRecent()
{
    const QString fileName = Util::convertPathFromOsSpecific(fourthMostRecentAct->text());
    openProject(fileName);
    setMostRecentProject();
}


void MainWindowGUI::saveProject()
{
    const QString fileName = frontend->getProject()->getProjectFileName();

    frontend->showProgress(tr("Saving scenes to disk ..."), frontend->getProject()->getSceneSize());

    if (!fileName.isEmpty()) {
        frontend->getProject()->saveProject();
    } else {
        saveProjectAs();
    }
    frontend->hideProgress();
}


void MainWindowGUI::saveProjectAs()
{
    QString file = QFileDialog::
                   getSaveFileName(this,
                                   tr("Save As"),
                                   lastVisitedDir,
                                   QString(tr("Project (*.%1);;Archive (*.%2)")).arg(PreferencesTool::projectSuffix).arg(PreferencesTool::archiveSuffix));

    if (!file.isNull()) {
        frontend->getProject()->setProjectFileName(file.toLocal8Bit());
        frontend->getProject()->saveProject();
        QString path = frontend->getProject()->getProjectPath();
        path.append("/");
        path.append(PreferencesTool::imageDirectory);
        changeMonitor->addDirectory(path);
        //fileMenu->setItemEnabled(SAVE, true);
        saveAct->setEnabled(true);
        setMostRecentProject();
    }
}


void MainWindowGUI::exportToVideo()
{
    qDebug("MainWindowGUI::exportToVideo --> Start");

    VideoEncoder *enc = NULL;
    PreferencesTool *pref = frontend->getPreferences();
    int activeEncoderApplication = pref->getBasicPreference("encoderapplication", VideoEncoder::noneApplication);

    if (activeEncoderApplication == VideoEncoder::noneApplication)
    {
        frontend->showWarning(tr("Warning"),
                              tr("Cannot find any registered encoder to be used for video export.\n"
                                 "This can be setted in the preferences menu.\n"
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

    if (pref->getBasicPreference("usedefaultoutputfile", 0) == 0) {
        QString filter;
        switch(pref->getBasicPreference("videoformat", VideoEncoder::noneFormat)) {
        case VideoEncoder::aviFormat:
            filter.append(tr("AVI Videos (*.avi)"));
            break;
        case VideoEncoder::mp4Format:
            filter.append(tr("MP4 Videos (*.mp4)"));
            break;
        }

        QString outputFile = QFileDialog::
                       getSaveFileName(this, tr("Export to video file"), lastVisitedDir, filter);
        if (outputFile.isEmpty()) {
            delete enc;
            enc = NULL;
            return;
        } else {
            enc->setOutputFile(outputFile);
        }
    } else {
        enc->setOutputFile(pref->getBasicPreference("defaultoutputfilename", ""));
    }

    // Remove an existing file
    QFile::remove(enc->getOutputFile());

    if (!enc->isValid()) {
        frontend->showWarning(tr("Warning"),
                              tr("The registered encoder is not valid.\n"
                                 "Check your settings in the preferences menu!"));
        delete enc;
        enc = NULL;
        return;
    }
    checkSaved();

    frontend->showProgress(tr("Exporting ..."), 0);
    frontend->getProject()->exportToVideo(enc);
    frontend->hideProgress();

    delete enc;
    enc = NULL;

    qDebug("MainWindowGUI::exportToVideo --> End");
}


void MainWindowGUI::exportToCinelerra()
{
    QString outputFile = QFileDialog::
                   getSaveFileName(this, tr("Export to file"), lastVisitedDir, "Cinelerra (*.XML)");

    if (!outputFile.isNull()) {
        frontend->getProject()->exportToCinelerra(outputFile);
    }
}


void MainWindowGUI::closeApplication()
{
    // Last changes saved?
    checkSaved();

    // Exit the application with status success
    exit(0);
}


void MainWindowGUI::showPreferencesDialog()
{
    PreferencesDialog *preferencesDialog;

    preferencesDialog = new PreferencesDialog(frontend);
    int ret = preferencesDialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("MainWindowsGUI::showPreferencesDialog --> End (cancel)");
    }
    delete(preferencesDialog);
}


void MainWindowGUI::showUndoStack()
{
    // Open up undo stack window
    if (undoView == 0) {
        undoView = new QUndoView(frontend->getProject()->getUndoStack());
        undoView->setWindowTitle(tr("qStopMotion - Undo stack"));
        undoView->setAttribute(Qt::WA_QuitOnClose, false);
        undoView->setGeometry(this->x() + this->width() + 15, this->y(),
                              200, 100);
    }
    undoView->show();
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

    QString locale(newLocale);
    QString translationFile;
    QString qmPath(frontend->getTranslationsDirName());
    const QString prefix = QLatin1String("qstopmotion_");
    const QString languagePref = frontend->getPreferences()->getBasicPreference("language", locale);

    if (languagePref.isEmpty()) {
        if (locale.isEmpty()) {
            // Get system locale.
            locale = QLocale::system().name().toLower();
            if (locale == QLatin1String("nb_no"))
                locale = QLatin1String("no_nb");
            else if (locale == QLatin1String("nn_no"))
                locale = QLatin1String("no_nn");
            else if (locale == QLatin1String("se_no"))
                locale = QLatin1String("no_se");
            else
                locale.truncate(2);
        }

        translationFile.append(prefix);
        translationFile.append(locale);
    }
    else {
        translationFile.append(prefix);
        translationFile.append(languagePref);
    }

    if (!translationFile.isEmpty()) {
        qDebug() << "Loading translator: " << translationFile;
        if (!appTranslator.load(translationFile, qmPath)) {
            // Translation file not opend
            qDebug("Translator not loaded!");
        }
        if (!qtTranslator.load("qt_" + locale, qmPath)) {
            // Translation file not opend
            qDebug("Translator not loaded!");
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


void MainWindowGUI::createHandlers()
{
    cameraHandler = new CameraHandler(frontend, this);
    editMenuHandler = new EditMenuHandler(frontend, this, timeLine);
    soundHandler = new SoundHandler(frontend, this, this->lastVisitedDir);
    connect(soundHandler, SIGNAL(soundsChanged()), timeLine, SLOT(frameSoundsChanged()));
}


void MainWindowGUI::createAccelerators()
{
    QShortcut *nextFrameAccel = new QShortcut(QKeySequence(Qt::Key_L), this);
    connect(nextFrameAccel, SIGNAL(activated()), toolBar, SLOT(selectNextFrame()));
    QShortcut *nextFrameAccel2 = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(nextFrameAccel2, SIGNAL(activated()), toolBar, SLOT(selectNextFrame()));

    QShortcut *previousFrameAccel = new QShortcut(QKeySequence(Qt::Key_J), this);
    connect(previousFrameAccel, SIGNAL(activated()), toolBar, SLOT(selectPreviousFrame()));
    QShortcut *previousFrameAccel2 = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(previousFrameAccel2, SIGNAL(activated()), toolBar, SLOT(selectPreviousFrame()));

    QShortcut *nextSceneAccel = new QShortcut(QKeySequence(Qt::Key_O), this);
    connect(nextSceneAccel, SIGNAL(activated()), toolBar, SLOT(selectNextScene()));

    QShortcut *prevSceneAccel = new QShortcut(QKeySequence(Qt::Key_I), this);
    connect(prevSceneAccel, SIGNAL(activated()), toolBar, SLOT(selectPreviousScene()));

    QShortcut *toggleCameraAccel = new QShortcut(QKeySequence(Qt::Key_C), this);
    connect(toggleCameraAccel, SIGNAL(activated()), recordingTab, SLOT(cameraButtonClicked()));

    QShortcut *captureAccel = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(captureAccel, SIGNAL(activated()), cameraHandler, SLOT(captureFrame()));

    QShortcut *newSceneAccel = new QShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_E), this);
    connect(newSceneAccel, SIGNAL(activated()), projectTab, SLOT(addSceneSlot()));

    QShortcut *removeSceneAccel = new QShortcut(QKeySequence(Qt::ShiftModifier + Qt::Key_Delete), this);
    connect(removeSceneAccel, SIGNAL(activated()), projectTab, SLOT(removeSceneSlot()));

    QShortcut *addFrameAccel = new QShortcut(QKeySequence(Qt::Key_F), this);
    connect(addFrameAccel, SIGNAL(activated()), projectTab, SLOT(addFramesSlot()));

    QShortcut *removeFramesAccel = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(removeFramesAccel, SIGNAL(activated()), projectTab, SLOT(removeFramesSlot()));
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
    undoAct->setEnabled(false);
    connect(undoAct, SIGNAL(triggered()), editMenuHandler, SLOT(undo()));

    redoAct = frontend->getProject()->getUndoStack()->createRedoAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("redo.png"));
    redoAct->setIcon(QIcon(iconFile));
    redoAct->setShortcut(ControlModifier + ShiftModifier + Key_Z);
    redoAct->setIconVisibleInMenu(true);
    redoAct->setEnabled(false);
    connect(redoAct, SIGNAL(triggered()), editMenuHandler, SLOT(redo()));
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
    connect(copyAct, SIGNAL(triggered()), editMenuHandler, SLOT(copy()));

    pasteAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("paste.png"));
    pasteAct->setIcon(QIcon(iconFile));
    pasteAct->setShortcut(ControlModifier + Key_V);
    pasteAct->setIconVisibleInMenu(true);
    pasteAct->setEnabled(false);
    connect(pasteAct, SIGNAL(triggered()), editMenuHandler, SLOT(paste()));

    gotoFrameAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("window.png"));
    gotoFrameAct->setIcon(QIcon(iconFile));
    gotoFrameAct->setShortcut(ControlModifier + Key_G);
    gotoFrameAct->setIconVisibleInMenu(true);
    connect(gotoFrameAct, SIGNAL(triggered()), gotoMenuWidget, SLOT(show()));
*/
    // Configure menu
    configureAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("configure.png"));
    configureAct->setIcon(QIcon(iconFile));
    configureAct->setShortcut(ControlModifier + Key_P);
    configureAct->setIconVisibleInMenu(true);
    connect(configureAct, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

    // View menu
    undoViewAct = new QAction(this);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("undoview.png"));
    undoViewAct->setIcon(QIcon(iconFile));
    undoViewAct->setShortcut(ControlModifier + Key_U);
    undoViewAct->setIconVisibleInMenu(true);
    connect(undoViewAct, SIGNAL(triggered()), this, SLOT(showUndoStack()));

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
    fileMenu = menuBar()->addMenu(tr("&File"));
    exportMenu = fileMenu->addMenu(tr("&Export"));
    mostRecentMenu = fileMenu->addMenu(tr("Open &Recent"));

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    // editMenu->addAction(gotoFrameAct);

    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    // cutAct->setEnabled(false);
    // copyAct->setEnabled(false);
    // gotoFrameAct->setEnabled(false);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(undoViewAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(whatsthisAct);
    helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
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
                                    cameraHandler,
                                    this);
    recordingTab->initialize();
    iconFile.append(QLatin1String("clapper.png"));
    sideBar->addTab(recordingTab, QIcon(iconFile), QString(tr("Recording")));

    projectTab = new ProjectTab(frontend,
                                lastVisitedDir);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("view.png"));
    sideBar->addTab(projectTab, QIcon(iconFile), QString(tr("Project")));
/*
    viewTab = new ViewTab(frontend);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("view.png"));
    sideBar->addTab(viewTab, QIcon(iconFile), QString(tr("View")));

    compositingTab = new CompositingTab(frontend);
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("compositing.png"));
    sideBar->addTab(compositingTab, QIcon(iconFile), QString(tr("Compositing")));
*/
    connect(this, SIGNAL(cameraStateChanged(bool)), recordingTab, SLOT(cameraStateChanged(bool)));
    connect(this, SIGNAL(cameraStateChanged(bool)), cameraHandler, SLOT(cameraStateChanged(bool)));
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

    connect(gotoSpinner, SIGNAL(spinBoxTriggered(int)), editMenuHandler, SLOT(gotoFrame(int)));
    connect(gotoSpinner, SIGNAL(spinBoxCanceled()), editMenuHandler, SLOT(closeGotoMenu()));

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
    editMenuHandler->setGotoMenu(gotoMenuWidget);
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

    toolBar = new ToolBar(frontend, cameraHandler);
    toolBar->setObjectName("ToolBar");
    toolBar->setMinimumSize(400, 30);
    toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    viewAreaLayout->addWidget(toolBar);
    // viewAreaLayout->addWidget(toolBar, 1, Qt::AlignJustify);

    viewArea->setLayout(viewAreaLayout);
    layout->addWidget(viewArea);

    connect(cameraHandler, SIGNAL(capturedFrame()), this, SLOT(activateMenuOptions()));
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


void MainWindowGUI::dragEnterEvent(QDragEnterEvent * event)
{
    qDebug("MainWindowGUI::dragEnterEvent --> Start");

    if (event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }

    qDebug("MainWindowGUI::dragEnterEvent --> End");
}


// TODO: New Implementation for Drop event
void MainWindowGUI::dropEvent(QDropEvent *event)
{
    qDebug("MainWindowGUI::dropEvent --> Start (Nothing)");
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


void MainWindowGUI::retranslateHelpText()
{
    QString infoText;

    //File menu
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
        tr("<h4>Quit</h4> "
           "<p><em>Quits</em> the program.</p>");
    quitAct->setWhatsThis(infoText);
    infoText =
        quitAct->toolTip().prepend(tr("Quit"));
    quitAct->setToolTip(infoText);


    //Edit menu
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
        tr("<h4>Configure qStopMotion</h4> "
           "<p>This will opens a window where you can <em>configure</em> "
           "qStopMotion with various input and output devices.</p>");
    configureAct->setWhatsThis(infoText);
    infoText =
        configureAct->toolTip().prepend(tr("Configure qStopMotion"));
    configureAct->setToolTip(infoText);


    //Help menu
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

    //Various menues
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


void MainWindowGUI::setMostRecentProject()
{
    const QString newFirst = frontend->getProject()->getProjectFileName();

    Q_ASSERT(!newFirst.isEmpty());

    PreferencesTool *pref = frontend->getPreferences();
    pref->removeProject(newFirst);
    pref->addProject(newFirst);
    pref->flushPreferences();
}


void MainWindowGUI::updateMostRecentMenu()
{
    mostRecentMenu->clear();
    mostRecentMenu->setTitle(tr("Open &Recent"));
    PreferencesTool *pref = frontend->getPreferences();
    int ProjectFileIndex = 0;
    int MenuIndex = 0;

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


void MainWindowGUI::openProject(const QString &fileName)
{
    Q_ASSERT(fileName != NULL);

    AnimationProject *project = frontend->getProject()->getAnimationProject();

    frontend->getProject()->setProjectFileName(fileName);
    Q_ASSERT(!(project->getProjectFileName().isEmpty()));
    frontend->getProject()->openProject();
    Q_ASSERT(!(project->getProjectFileName().isEmpty()));

    saveAsAct->setEnabled(true);
    Q_ASSERT(!(project->getProjectFileName().isEmpty()));
    saveAct->setEnabled(true);
    Q_ASSERT(!(project->getProjectFileName().isEmpty()));
    int size = frontend->getProject()->getTotalExposureSize();
    if (size > 0) {
        activateMenuOptions();
        modelSizeChanged(size);
        toolBar->modelSizeChanged(size);
    }
}


void MainWindowGUI::checkSaved()
{
    // Last changes saved?
    bool b = frontend->getProject()->isUnsavedChanges();
    if (b) {
        int save = frontend->askQuestion(tr("Unsaved changes"),
                                         tr("There are unsaved changes. Do you want to save?"));
        if (save == 0) { // user pressed button 0, which is 'yes'
            saveProject();
        }
    }
}


void MainWindowGUI::setProjectSettingsToDefault()
{
    PreferencesTool *pref = frontend->getPreferences();

    frontend->getProject()->setVideoSource(pref->getBasicPreference("defaultsource", 0));
    frontend->getProject()->setMixingMode(pref->getBasicPreference("defaultmixingmode", 0));
    frontend->getProject()->setUnitMode(pref->getBasicPreference("defaultunitmode", 0));
    frontend->getProject()->setMixCount(pref->getBasicPreference("defaultmixcount", 0));
    frontend->getProject()->setPlaybackCount(pref->getBasicPreference("defaultplaybackcount", 0));
    frontend->getProject()->setFramesPerSecond(pref->getBasicPreference("defaultframespersecond", 0));
}


