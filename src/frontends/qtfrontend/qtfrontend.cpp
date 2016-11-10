/******************************************************************************
 *  Copyright (C) 2005-2016 by                                                *
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

#include <QDate>
#include <QDesktopWidget>
#include <QDir>
#include <QPoint>
#include <QMessageBox>
#include <QSize>
#include <QSplashScreen>

#include "qtfrontend.h"

#include "domain/animation/projectserializer.h"
#include "frontends/viewfacade.h"
#include "frontends/qtfrontend/dialogs/externalcommanddialog.h"
#include "technical/util.h"
#include "technical/preferencestool.h"

#include <cstring>
// #include <unistd.h>

Q_LOGGING_CATEGORY(qstopmotion, "qtfrontend.qstopmotion")

QtFrontend::QtFrontend(int &argc, char **argv)
{
    qCDebug(qstopmotion) << "QtFrontend::Constructor --> Start";
    qCDebug(qstopmotion) << "QtFrontend::Constructor Date:" << QDate::currentDate();

    domainFacade         = NULL;
    viewFacade           = NULL;
    mw                   = NULL;
    preferencesTool      = NULL;

    stApp = new QApplication(argc, argv);

    // #if QT_VERSION == 0x040400
    //     stApp->setAttribute(Qt::AA_NativeWindows);
    // #endif

/*
#if QT_VERSION<0x050000
    // der Codec der Quelltexte ist UTF-8,
    // Die Standardkonvertierung von char* in Unicode geht jedoch von Latin-1 aus (bis QT4.8):
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));       // f"ur tr()
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // const char* in QString
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));   // f"ur locale
#endif
*/

    qCDebug(qstopmotion) << "QtFrontend::Constructor --> End";
}


QtFrontend::~QtFrontend()
{
    qDebug() << "QtFrontend::Destructor --> Start";

    // Save the size and position of the application
    QSize appSize = mw->size();
    QPoint appPos = mw->pos();
    preferencesTool->setIntegerPreference("preferences", "applicationsizeheight", appSize.height());
    preferencesTool->setIntegerPreference("preferences", "applicationsizewidth", appSize.width());
    preferencesTool->setIntegerPreference("preferences", "applicationposx", appPos.x());
    preferencesTool->setIntegerPreference("preferences", "applicationposy", appPos.y());

    // Cleanup the memory
    delete mw;
    mw = NULL;

    if (domainFacade != NULL) {
        delete domainFacade;
        domainFacade = NULL;
    }

    if (viewFacade != NULL) {
        delete viewFacade;
        viewFacade = NULL;
    }

    if(preferencesTool != NULL) {
        delete preferencesTool;
        preferencesTool = NULL;
    }

    delete stApp;
    stApp = NULL;

    qDebug() << "QtFrontend::Destructor --> End";
}


bool QtFrontend::checkApplicationDirectory(char *binDirName)
{
    qDebug() << "QtFrontend::checkApplicationDirectory --> Start";

    qDebug() << "QtFrontend::checkApplicationDirectory --> Call command:" << binDirName;

    bool hasCorrectPermissions = true;
    QDir homeDir = QDir::home();
    QString userDirName;
    QString otherDirName;

    // Check if ~./qstopmotion directory exists, create it if not
    bool prefsFileExists = homeDir.exists(PreferencesTool::applicationDirectory);
    if (prefsFileExists == false) {
        if (!homeDir.mkdir(PreferencesTool::applicationDirectory)) {
            // Can't create application directory
            showCritical(tr("Create directory"),
                tr("You do not have the necessary permissions to run qStopMotion.\n"
                "You need permission to create the .qstopmotion directory in your home directory."));

            qDebug() << "QtFrontend::checkApplicationDirectory --> End (Error)";
            return 1;
        }
    }
    homeDir.cd(PreferencesTool::applicationDirectory);
    if (homeDir.isReadable() == false) {
        // TODO: Writable and executable test necessary
        hasCorrectPermissions = false;
    }
    if (!hasCorrectPermissions) {
        // Set the rigth permissions
        QFile appDir(homeDir.absolutePath());
        hasCorrectPermissions = appDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        if (!hasCorrectPermissions) {
            // Set the permissions was not successful!
            showCritical(tr("Check Permissions"),
                tr("You do not have the necessary permissions to run qStopMotion.\n"
                "You need permission to read, write and execute on the .qstopmotion directory."));

            qDebug() << "QtFrontend::checkApplicationDirectory --> End (Error)";
            return 1;
        }
    }

    userDirName.append(homeDir.absolutePath());

    appUserDirName.append(userDirName);
    userDirName.append(QLatin1String("/"));

    otherDirName.append(userDirName);
    otherDirName.append(PreferencesTool::tempDirectory);
    appTempDirName.append(otherDirName);

    QString absoluteAppName = Util::convertPathFromOsSpecific(QString(binDirName));
    int pathLength = absoluteAppName.lastIndexOf("/bin/");

    if (pathLength == -1)
    {
        QString command = Util::checkCommand(binDirName);
        qDebug() << "QtFrontend::checkApplicationDirectory --> Check command:" << command;
        absoluteAppName = Util::convertPathFromOsSpecific(QString(command));
        pathLength = absoluteAppName.lastIndexOf("/bin/");
    }

    if (pathLength == -1)
    {
        // The binDirName is only the application name --> use default values
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
        // Windows version

        appApplicationDirName.append("C:/Program Files/qstopmotion/");
#else
        // Linux and Apple OS X version

        appApplicationDirName.append("/usr/");
#endif
        otherDirName.clear();
        otherDirName.append(appApplicationDirName);
        otherDirName.append(QLatin1String("bin/"));
        appBinDirName.append(otherDirName);
    }
    else
    {
        // The binDirName is a full qualified application name
        appApplicationDirName.append(absoluteAppName.left(pathLength + 1));
        pathLength = absoluteAppName.lastIndexOf("/");
        appBinDirName.append(absoluteAppName.left(pathLength + 1));
    }
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Directory:" << appApplicationDirName;
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Binary Directory:" << appBinDirName;

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Windows version

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::manualDirectory);
    otherDirName.append(QLatin1String("/"));
    appManualDirName.append(otherDirName);

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::translationsDirectory);
    otherDirName.append(QLatin1String("/"));
    appTranslationsDirName.append(otherDirName);

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::graphicsDirectory);
    otherDirName.append(QLatin1String("/"));
    appGraphicsDirName.append(otherDirName);

#else
    // Linux and Apple OS X version

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/doc/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    // otherDirName.append(PreferencesTool::manualDirectory);
    // otherDirName.append(QLatin1String("/"));
    appManualDirName.append(otherDirName);

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    otherDirName.append(PreferencesTool::translationsDirectory);
    otherDirName.append(QLatin1String("/"));
    appTranslationsDirName.append(otherDirName);

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    otherDirName.append(PreferencesTool::graphicsDirectory);
    otherDirName.append(QLatin1String("/"));
    appGraphicsDirName.append(otherDirName);

#endif

    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Manual Directory:" << appManualDirName;
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Translations Directory:" << appTranslationsDirName;
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Graphics Directory:" << appGraphicsDirName;


    otherDirName.clear();
    otherDirName.append(appGraphicsDirName);
    otherDirName.append(PreferencesTool::iconsDirectory);
    otherDirName.append(QLatin1String("/"));
    appIconsDirName.append(otherDirName);
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Icon Directory:" << appIconsDirName;

    otherDirName.clear();
    otherDirName.append(appGraphicsDirName);
    otherDirName.append(PreferencesTool::picturesDirectory);
    otherDirName.append(QLatin1String("/"));
    appPicturesDirName.append(otherDirName);
    qDebug() << "QtFrontend::checkApplicationDirectory --> Application Pictures Directory:" << appPicturesDirName;

    qDebug() << "QtFrontend::checkApplicationDirectory --> End";
    return 0;
}


void QtFrontend::init()
{
    qDebug() << "QtFrontend::init --> Start";

    QString iconFile(getGraphicsDirName());
    iconFile.append(QLatin1String("qstopmotion_splash_screen.png"));
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(iconFile));
    splash->show();
    Qt::Alignment bottomRight = Qt::AlignBottom | Qt::AlignRight;
    splash->showMessage(PreferencesTool::applicationShowName + " - " + PreferencesTool::applicationVersion, bottomRight, Qt::black);

    // Need to call this here to get the locale for the language
    // which is used by the translator created in mainWindowGUI
    preferencesTool = new PreferencesTool(this);
    initializePreferences();

    // returns a pointer to the domain facade (allocated with new)
    domainFacade = new DomainFacade(this);

    // returns a pointer to the facade (allocated with new)
    viewFacade = new ViewFacade(this);

    mw = new MainWindowGUI(stApp, this);
    mw->initialize();

    // Restore the size and position of the application
    QSize   appSize;
    QPoint  appPos;
    int     value;

    if (preferencesTool->getIntegerPreference("preferences", "applicationsizeheight", value) == false) {
        value = 593;
    }
    appSize.setHeight(value);

    if (preferencesTool->getIntegerPreference("preferences", "applicationsizewidth", value) == false) {
        value = 751;
    }
    appSize.setWidth(value);

    if (preferencesTool->getIntegerPreference("preferences", "applicationposx", value) == false) {
        value = 80;
    }
    appPos.setX(value);

    if (preferencesTool->getIntegerPreference("preferences", "applicationposy", value) == false) {
        value = 20;
    }
    appPos.setY(value);

    // Was the last position on a not existing second screen?
    if (appPos.x() > QApplication::desktop()->width()) {
        appPos.setX(80);
    }
    if (appPos.y() > QApplication::desktop()->height()) {
        appPos.setY(20);
    }

    mw->resize(appSize);
    mw->move(appPos);

    // this->testMainWidget();

    mw->show();

    splash->finish(mw);
    delete splash;

    // this->testMainWidget();

    qDebug() << "QtFrontend::init --> End";
}


bool QtFrontend::handleArguments(int argc, char **argv)
{
    qDebug() << "QtFrontend::handleArguments --> Start";

    if (argc < 2) {
        // No arguments
        qDebug() << "QtFrontend::handleArguments --> End (false)";
        return false;
    }

    if (QFileInfo(argv[1]).isReadable()) {
        mw->openProject(argv[1]);
        qDebug() << "QtFrontend::handleArguments --> End (true)";
        return true;
    }

    qDebug() << "QtFrontend::handleArguments --> End (false)";
    return false;
}


int QtFrontend::run(int, char **)
{
    stApp->connect(stApp, SIGNAL(lastWindowClosed()), stApp, SLOT(quit()));
    return stApp->exec();
}


void QtFrontend::finalize()
{
    this->removeApplicationFiles();
    this->removeApplicationDirectories();
}


DomainFacade* QtFrontend::getProject()
{
    return domainFacade;
}


ViewFacade* QtFrontend::getView()
{
    return viewFacade;
}


PreferencesTool* QtFrontend::getPreferences()
{
    return preferencesTool;
}


const QString QtFrontend::getUserDirName()
{
    return this->appUserDirName;
}


const QString QtFrontend::getTempDirName()
{
    return this->appTempDirName;
}


const QString QtFrontend::getApplicationDirName()
{
    return this->appApplicationDirName;
}


const QString QtFrontend::getBinDirName()
{
    return this->appBinDirName;
}


const QString QtFrontend::getManualDirName()
{
    return this->appManualDirName;
}


const QString QtFrontend::getTranslationsDirName()
{
    return this->appTranslationsDirName;
}


const QString QtFrontend::getGraphicsDirName()
{
    return this->appGraphicsDirName;
}


const QString QtFrontend::getIconsDirName()
{
    return this->appIconsDirName;
}


const QString QtFrontend::getPicturesDirName()
{
    return this->appPicturesDirName;
}


const QVector<QString> QtFrontend::getLanguages()
{
    return mw->getLanguages();
}


const QVector<QString> QtFrontend::getLocales()
{
    return mw->getLocales();
}


void QtFrontend::changeLanguage(int newIndex)
{
    mw->changeLanguage(newIndex);
}


void QtFrontend::changeCaptureButtonFunction(int newFunction)
{
    mw->changeCaptureButtonFunction((PreferencesTool::captureButtonFunction)newFunction);
}


bool QtFrontend::getVerticalGrid()
{
    return mw->getVerticalGrid();
}


void QtFrontend::setVerticalGrid(bool newState)
{
    mw->setVerticalGrid(newState);
}


int QtFrontend::getVerticalSpin()
{
    return mw->getVerticalSpin();
}


void QtFrontend::setVerticalSpin(int newSpin)
{
    mw->setVerticalSpin(newSpin);
}


bool QtFrontend::getHorizontalGrid()
{
    return mw->getHorizontalGrid();
}


void QtFrontend::setHorizontalGrid(bool newState)
{
    mw->setHorizontalGrid(newState);
}


int QtFrontend::getHorizontalSpin()
{
    return mw->getHorizontalSpin();
}


void QtFrontend::setHorizontalSpin(int newSpin)
{
    mw->setHorizontalSpin(newSpin);
}


void QtFrontend::getGridColorRGB(int *r, int *g, int *b, int *a)
{
    mw->getGridColorRGB(r, g, b, a);
}


void QtFrontend::setGridColorRGB(int r, int g, int b, int a)
{
    mw->setGridColorRGB(r, g, b, a);
}


void QtFrontend::showProgress(const QString &operation, unsigned int numOperations)
{
    mw->showProgress(operation, numOperations);
}


void QtFrontend::hideProgress()
{
    mw->hideProgress();
}


void QtFrontend::updateProgress(int numOperationsDone)
{
    mw->updateProgress(numOperationsDone);
}


void QtFrontend::increaseProgress()
{
    mw->increaseProgress();
}


void QtFrontend::setProgressInfo(const QString infoText)
{
    mw->setProgressInfo(infoText);
}


void QtFrontend::showMessage(const QString &message, int timeout)
{
    mw->showMessage(message, timeout);
}


void QtFrontend::clearMessage()
{
    mw->clearMessage();
}


void QtFrontend::setProjectID(const QString id)
{
    mw->setProjectID(id);
}


void QtFrontend::setSceneID(const QString id)
{
    mw->setSceneID(id);
}


void QtFrontend::setTakeID(const QString id)
{
    mw->setTakeID(id);
}


void QtFrontend::setExposureID(const QString id)
{
    mw->setExposureID(id);
}


bool QtFrontend::isOperationAborted()
{
    return mw->isOperationAborted();
}


void QtFrontend::processEvents()
{
    stApp->processEvents();
}


void QtFrontend::initializePreferences()
{
    qDebug() << "QtFrontend::initializePreferences --> Start";

    QDir homeDir = QDir::home();
    QString preferencesFile = homeDir.absolutePath();
    preferencesFile.append(QLatin1String("/"));
    preferencesFile.append(PreferencesTool::applicationDirectory);
    preferencesFile.append(QLatin1String("/"));
    preferencesFile.append(PreferencesTool::preferencesName);
    preferencesFile.append(QLatin1String("."));
    preferencesFile.append(PreferencesTool::preferencesSuffix);

    if (!preferencesTool->setPreferencesFile(preferencesFile, PreferencesTool::preferencesVersion)) {
        // File doesn't exist or is corrupt
        setDefaultPreferences();
    }

    qDebug() << "QtFrontend::initializePreferences --> End";
}


void QtFrontend::setDefaultPreferences()
{
    qDebug() << "QtFrontend::setDefaultPreferences --> Start";

    preferencesTool->setVersion(PreferencesTool::preferencesVersion);
    preferencesTool->setBasicPreferenceDefaults();
    // preferencesTool->setEncoderDefaults();

    qDebug() << "QtFrontend::setDefaultPreferences --> End";
}


int QtFrontend::askQuestion(const QString title, const QString &question)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(question);
    // msgBox.setInformativeText(information);
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setDefaultButton(QMessageBox::NoButton);
    QPushButton *yesButton = msgBox.addButton(tr("&Yes"), QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton(tr("&No"), QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);

    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        return 0;
    }

    return 1;
}

void QtFrontend::showInformation(const QString title, const QString &info)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(info);
    // msgBox.setInformativeText(information);
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setDefaultButton(QMessageBox::NoButton);
    msgBox.addButton(tr("&OK"), QMessageBox::AcceptRole);
    msgBox.setIcon(QMessageBox::Information);

    msgBox.exec();
}


void QtFrontend::showWarning(const QString title, const QString &warning)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(warning);
    // msgBox.setInformativeText(information);
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setDefaultButton(QMessageBox::NoButton);
    msgBox.addButton(tr("&OK"), QMessageBox::AcceptRole);
    msgBox.setIcon(QMessageBox::Warning);

    msgBox.exec();
}


void QtFrontend::showCritical(const QString title, const QString &message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    // msgBox.setInformativeText(information);
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setDefaultButton(QMessageBox::NoButton);
    msgBox.addButton(tr("&OK"), QMessageBox::AcceptRole);
    msgBox.setIcon(QMessageBox::Critical);

    msgBox.exec();

    Q_ASSERT(0);
}


int QtFrontend::runExternalCommand(const QString &command, const QStringList &arguments)
{
    ExternalCommandDialog *ec = new ExternalCommandDialog;
    ec->show();
    ec->run(command, arguments);
    return 0;
}


bool QtFrontend::startGrabber()
{
    bool ret;

    qDebug() << "QtFrontend::startGrabber --> Start";

    ret = mw->startGrabber();

    qDebug() << "QtFrontend::startGrabber --> End";
    return ret;
}


bool QtFrontend::isGrabberInited()
{
    bool ret;

    qDebug() << "QtFrontend::on --> Start";

    ret = mw->isGrabberInited();

    qDebug() << "QtFrontend::on --> End";
    return ret;
}


void QtFrontend::stopGrabber()
{
    qDebug() << "QtFrontend::stopGrabber --> Start";

    mw->stopGrabber();

    qDebug() << "QtFrontend::stopGrabber --> End";
}


const QVector<ImageGrabberDevice*> QtFrontend::getDevices()
{
    return mw->getDevices();
}


ImageGrabberDevice* QtFrontend::getDevice(int deviceIndex)
{
    return mw->getDevice(deviceIndex);
}


const QVector<QString> QtFrontend::getDeviceNames()
{
    return mw->getDeviceNames();
}


const QVector<QString> QtFrontend::getResolutionNames(int deviceIndex)
{
    return mw->getResolutionNames(deviceIndex);
}


const QImage QtFrontend::getLiveImage()
{
    return mw->getLiveImage();
}


const QImage QtFrontend::getRawImage()
{
    return mw->getRawImage();
}


void QtFrontend::nextAnimationFrame(int exposureIndex)
{
    mw->nextAnimationFrame(exposureIndex);
}


bool QtFrontend::removeContentInDirectory(const QString &dirPath)
{
    bool ret = true;
    QDir aDir(dirPath);

    if (!(aDir.exists())) {
        return false;
    }

    QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot |
                            QDir::Dirs | QDir::Files);
    int count = entries.size();
    for (int idx = 0; ((idx < count) && (ret)); idx++) {
        QFileInfo entryInfo = entries[idx];
        QString path = entryInfo.absoluteFilePath();
        if (entryInfo.isDir()) {
            // This entry is a directory
            ret = removeContentInDirectory(path);
            if (!ret) {
                continue;
            }
            if (!aDir.rmdir(path)) {
                ret = false;
            }
        }
        else {
            // This entry is a file
            if (!aDir.remove(path)) {
                ret = false;
            }
        }
    }

    return(ret);
}


void QtFrontend::makeApplicationDirectories()
{
    QDir homeDir = QDir::home();
    bool ret;

    ret = homeDir.mkpath(getTempDirName());
    Q_ASSERT(ret == true);
    // QFile appDir(homeDir.absolutePath() and temp subdirectory);
    // hasCorrectPermissions = appDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
}


void QtFrontend::removeApplicationDirectories()
{
    QDir homeDir(getUserDirName());
    bool ret;

    ret = homeDir.rmdir(getTempDirName());
    Q_ASSERT(ret == true);
}


void QtFrontend::removeApplicationFiles()
{
    bool ret;

    ret = removeContentInDirectory(getTempDirName());
    Q_ASSERT(ret == true);
    getProject()->removeHistoryFile();
}


bool QtFrontend::isRecoveryMode()
{
    qDebug() << "QtFrontend::isRecoveryMode --> Start";

    if (QFile::exists(getTempDirName()) == false) {
        qDebug() << "QtFrontend::isRecoveryMode --> End (False)";
        return false;
    }

    // Everything is intact and we have to run in recovery mode
    qDebug() << "QtFrontend::isRecoveryMode --> End (True)";
    return true;
}


bool QtFrontend::recoverProject()
{
    qDebug() << "QtFrontend::recoverProject --> Start";

    int ret = askQuestion(tr("Recovery"),
                  tr("Something caused qStopmotion to exit abnormally\n"
                  "last time it was run. Do you want to recover?"));
    // The user wants to recover
    if (ret != 0) {
        qDebug() << "QtFrontend::recoverProject --> End (False)";
        return false;
    }

    mw->setWindowTitle(tr("qStopMotion - Recovered Project"));
    bool recovered = getProject()->recoverProject();

    qDebug() << "QtFrontend::recoverProject --> End";
    return recovered;
}


void QtFrontend::startDialog()
{
    qDebug() << "QtFrontend::startDialog --> Start";

    mw->startDialog();

    qDebug() << "QtFrontend::startDialog --> End";
}


void QtFrontend::setToolBarState(int newState)
{
    qDebug() << "MainWindowGUI::setToolBarState --> Start";

    mw->setToolBarState(newState);

    qDebug() << "MainWindowGUI::setToolBarState --> End";
}


int QtFrontend::getRecordingMode()
{
    return mw->getRecordingMode();
}


void QtFrontend::setRecordingMode(int mode)
{
    mw->setRecordingMode(mode);
}


int QtFrontend::getVideoSource()
{
    return mw->getVideoSource();
}


bool QtFrontend::setVideoSource(int index)
{
    return mw->setVideoSource(index);
}


int QtFrontend::getResolution()
{
    return mw->getResolution();
}


bool QtFrontend::setResolution(int index)
{
    return mw->setResolution(index);
}


const GrabberResolution QtFrontend::getResolution(int Index)
{
    return mw->getResolution(Index);
}


bool QtFrontend::convertImages(int newFormat, int newQuality)
{
    return mw->convertImages(newFormat, newQuality);
}


int QtFrontend::getMixMode()
{
    return mw->getMixMode();
}


void QtFrontend::setMixMode(int mode)
{
    mw->setMixMode(mode);
}


int QtFrontend::getMixCount()
{
    return mw->getMixCount();
}


void QtFrontend::setMixCount(int count)
{
    mw->setMixCount(count);
}


int QtFrontend::getPlaybackCount()
{
    return mw->getPlaybackCount();
}


void QtFrontend::setPlaybackCount(int count)
{
    mw->setPlaybackCount(count);
}


int QtFrontend::getOverlayIntensity()
{
    return mw->getOverlayIntensity();
}


void QtFrontend::setOverlayIntensity(int overlay)
{
    mw->setOverlayIntensity(overlay);
}


void QtFrontend::setupDirectoryMonitoring()
{
    mw->setupDirectoryMonitoring();
}


void QtFrontend::addDirectoryToMonitoring(const QString &directory)
{
    mw->addDirectoryToMonitoring(directory);
}


void QtFrontend::removeDirectoryFromMonitoring(const QString &directory)
{
    mw->removeDirectoryFromMonitoring(directory);
}


void QtFrontend::removeAllDirectoriesFromMonitoring()
{
    mw->removeAllDirectoriesFromMonitoring();
}


void QtFrontend::addFileToMonitoring(const QString &file)
{
    mw->addFileToMonitoring(file);
}


void QtFrontend::removeFileFromMonitoring(const QString &file)
{
    mw->removeFileFromMonitoring(file);
}


void QtFrontend::removeAllFilesFromMonitoring()
{
    mw->removeAllFilesFromMonitoring();
}


void QtFrontend::test(QWidget *parentWidget, int level)
{
    QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " --> Start <<<<<<<<<<<<<<<=======================";

    QObjectList testList = parentWidget->children();
    int testSize = testList.size();
    if (testSize == 0) {
        QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " --> End (no children) <<<<<<<<<<<<<<<=======================";
        return;
    }
    QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " --> testSize: " << testSize;
    for (int testIndex = 0 ; testIndex < testSize ; testIndex++ ) {
        if ((level == 1) && (testIndex > 3)) {
            QDebug(QtDebugMsg) << "QtFrontend::test: ++++++++++++++++++++++ Break +++++++++++++++++";
        }

        QObject *testObject = testList.at(testIndex);
        if (testObject == 0) {
            QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " index: " << testIndex << " --> Widget korrupt !!!";
            continue;
        }

        if (testObject->objectName().isEmpty()) {
            QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " index: " << testIndex << " --> testObject.className: " << testObject->metaObject()->className();
        }
        else {
            QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " index: " << testIndex << " --> testObject.objectName: " << testObject->objectName();
        }
        if (testObject->parent() != parentWidget) {
            QDebug(QtDebugMsg) << "QtFrontend::test: ?????????????? Parent different ?????????????";
        }

        QWidget *testWidget = qobject_cast<QWidget *>(testObject);
        if (testWidget == 0) {
            QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " index: " << testIndex << " --> no Widget !!!";
            continue;
        }

        QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " index: " << testIndex << " --> testWidget.winID: " << testWidget->winId();
        this->test(testWidget, (level+1));
    }

    QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " --> End <<<<<<<<<<<<<<<=======================";
}


void QtFrontend::testMainWidget()
{
    QDebug(QtDebugMsg) << "QtFrontend::testMainWidget --> Start";

    test(this->mw, 0);

    QDebug(QtDebugMsg) << "QtFrontend::testMainWidget --> End";
}
