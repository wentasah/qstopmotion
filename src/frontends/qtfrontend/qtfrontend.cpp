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

#include "qtfrontend.h"

#include "domain/animation/projectserializer.h"
#include "frontends/viewfacade.h"
#include "frontends/qtfrontend/dialogs/externalcommanddialog.h"
#include "technical/util.h"
#include "technical/preferencestool.h"

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QtGui>

#include <cstring>
// #include <unistd.h>


QtFrontend::QtFrontend(int &argc, char **argv)
{
    qDebug("QtFrontend::Constructor --> Start");

    domainFacade         = NULL;
    viewFacade           = NULL;
    mw                   = NULL;
    preferencesTool      = NULL;

    stApp = new QApplication(argc, argv);
    /*
    #if QT_VERSION == 0x040400
        stApp->setAttribute(Qt::AA_NativeWindows);
    #endif
    */

    qDebug("QtFrontend::Constructor --> End");
}


QtFrontend::~QtFrontend()
{
    qDebug("QtFrontend::Destructor --> Start");

    // Save the size and position of the application
    QSize appSize = mw->size();
    QPoint appPos = mw->pos();
    preferencesTool->setBasicPreference("applicationsizeheight", appSize.height());
    preferencesTool->setBasicPreference("applicationsizewidth", appSize.width());
    preferencesTool->setBasicPreference("applicationposx", appPos.x());
    preferencesTool->setBasicPreference("applicationposy", appPos.y());

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

    qDebug("QtFrontend::Destructor --> End");
}


bool QtFrontend::checkApplicationDirectory(char *binDirName)
{
    qDebug("QtFrontend::checkApplicationDirectory --> Start");

    qDebug() << "QtFrontend::checkApplicationDirectory --> Call command: " << binDirName;

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

            qDebug("QtFrontend::checkApplicationDirectory --> End (Error)");
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

            qDebug("QtFrontend::checkApplicationDirectory --> End (Error)");
            return 1;
        }
    }

    userDirName.append(homeDir.absolutePath());

    appUserDirName.append(userDirName.toLatin1());
    userDirName.append(QLatin1String("/"));

    otherDirName.append(userDirName);
    otherDirName.append(PreferencesTool::tempDirectory);
    appTempDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(userDirName);
    otherDirName.append(PreferencesTool::trashDirectory);
    appTrashDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(userDirName);
    otherDirName.append(PreferencesTool::packerDirectory);
    appPackerDirName.append(otherDirName.toLatin1());

    QString absoluteAppName = Util::convertPathFromOsSpecific(QString(binDirName));
    int pathLength = absoluteAppName.lastIndexOf("/bin/");

    if (pathLength == -1)
    {
        QString command = Util::checkCommand(binDirName);
        qDebug() << "QtFrontend::checkApplicationDirectory --> Check command: " << command;
        absoluteAppName = Util::convertPathFromOsSpecific(QString(command));
        pathLength = absoluteAppName.lastIndexOf("/bin/");
    }

    if (pathLength == -1)
    {
        // The binDirName is only the application name --> use default values
#ifdef Q_WS_WIN
        // Windows version

        appApplicationDirName.append("C:/Program Files/qstopmotion/");
#else
        // Linux and Apple OS X version

        appApplicationDirName.append("/usr/");
#endif
        otherDirName.clear();
        otherDirName.append(appApplicationDirName);
        otherDirName.append(QLatin1String("bin/"));
        appBinDirName.append(otherDirName.toLatin1());
    }
    else
    {
        // The binDirName is a full qualified application name
        appApplicationDirName.append(absoluteAppName.left(pathLength + 1));
        pathLength = absoluteAppName.lastIndexOf("/");
        appBinDirName.append(absoluteAppName.left(pathLength + 1));
    }
    qDebug("QtFrontend::checkApplicationDirectory --> Application Direcory: " + appApplicationDirName);
    qDebug("QtFrontend::checkApplicationDirectory --> Application Binary Direcory: " + appBinDirName);

#ifdef Q_WS_WIN
    // Windows version

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::manualDirectory);
    otherDirName.append(QLatin1String("/"));
    appManualDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::translationsDirectory);
    otherDirName.append(QLatin1String("/"));
    appTranslationsDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(PreferencesTool::graphicsDirectory);
    otherDirName.append(QLatin1String("/"));
    appGraphicsDirName.append(otherDirName.toLatin1());

#else
    // Linux and Apple OS X version

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/doc/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    // otherDirName.append(PreferencesTool::manualDirectory);
    // otherDirName.append(QLatin1String("/"));
    appManualDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    otherDirName.append(PreferencesTool::translationsDirectory);
    otherDirName.append(QLatin1String("/"));
    appTranslationsDirName.append(otherDirName.toLatin1());

    otherDirName.clear();
    otherDirName.append(appApplicationDirName);
    otherDirName.append(QLatin1String("share/"));
    otherDirName.append(PreferencesTool::applicationName);
    otherDirName.append(QLatin1String("/"));
    otherDirName.append(PreferencesTool::graphicsDirectory);
    otherDirName.append(QLatin1String("/"));
    appGraphicsDirName.append(otherDirName.toLatin1());

#endif

    qDebug("QtFrontend::checkApplicationDirectory --> Application Manual Direcory: " + appManualDirName);
    qDebug("QtFrontend::checkApplicationDirectory --> Application Translations Direcory: " + appTranslationsDirName);
    qDebug("QtFrontend::checkApplicationDirectory --> Application Graphics Direcory: " + appGraphicsDirName);


    otherDirName.clear();
    otherDirName.append(appGraphicsDirName);
    otherDirName.append(PreferencesTool::iconsDirectory);
    otherDirName.append(QLatin1String("/"));
    appIconsDirName.append(otherDirName.toLatin1());
    qDebug("QtFrontend::checkApplicationDirectory --> Application Icon Direcory: " + appIconsDirName);

    otherDirName.clear();
    otherDirName.append(appGraphicsDirName);
    otherDirName.append(PreferencesTool::picturesDirectory);
    otherDirName.append(QLatin1String("/"));
    appPicturesDirName.append(otherDirName.toLatin1());
    qDebug("QtFrontend::checkApplicationDirectory --> Application Pictures Direcory: " + appPicturesDirName);

    qDebug("QtFrontend::checkApplicationDirectory --> End");
    return 0;
}


void QtFrontend::init()
{
    qDebug("QtFrontend::init --> Start");

    // Need to call this here to get the locale for the language
    // which is used by the translator created in mainWindowGUI
    preferencesTool = new PreferencesTool(this);
    initializePreferences();

    // returns a pointer to the domain facade (allocated with new)
    domainFacade = new DomainFacade(this);

    // returns a pointer to the facade (allocated with new)
    viewFacade = new ViewFacade(this);

    mw = new MainWindowGUI(stApp, this);
    mw->init();

    // Restore the size and position of the application
    QSize appSize;
    QPoint appPos;
    appSize.setHeight(preferencesTool->getBasicPreference("applicationsizeheight", 593));
    appSize.setWidth(preferencesTool->getBasicPreference("applicationsizewidth", 751));
    appPos.setX(preferencesTool->getBasicPreference("applicationposx", 80));
    appPos.setY(preferencesTool->getBasicPreference("applicationposy", 20));

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

    // this->testMainWidget();

    qDebug("QtFrontend::init --> End");
}


bool QtFrontend::handleArguments(int argc, char **argv)
{
    qDebug("QtFrontend::handleArguments --> Start");

    if (argc < 2) {
        // No arguments
        qDebug("QtFrontend::handleArguments --> End (false)");
        return false;
    }

    if (QFileInfo(argv[1]).isReadable()) {
        mw->openProject(argv[1]);
        qDebug("QtFrontend::handleArguments --> End (true)");
        return true;
    }

    qDebug("QtFrontend::handleArguments --> End (false)");
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
    this->removeCaptureFiles();
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


const char* QtFrontend::getUserDirName()
{
    return this->appUserDirName.constData();
}


const char* QtFrontend::getTempDirName()
{
    return this->appTempDirName.constData();
}


const char* QtFrontend::getTrashDirName()
{
    return this->appTrashDirName.constData();
}


const char* QtFrontend::getPackerDirName()
{
    return this->appPackerDirName.constData();
}


const char* QtFrontend::getApplicationDirName()
{
    return this->appApplicationDirName.constData();
}


const char* QtFrontend::getBinDirName()
{
    return this->appBinDirName.constData();
}


const char* QtFrontend::getManualDirName()
{
    return this->appManualDirName.constData();
}


const char* QtFrontend::getTranslationsDirName()
{
    return this->appTranslationsDirName.constData();
}


const char* QtFrontend::getGraphicsDirName()
{
    return this->appGraphicsDirName.constData();
}


const char* QtFrontend::getIconsDirName()
{
    return this->appIconsDirName.constData();
}


const char* QtFrontend::getPicturesDirName()
{
    return this->appPicturesDirName.constData();
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


void QtFrontend::setProgressInfo(const char *infoText)
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


void QtFrontend::setProjectID(const char *id)
{
    mw->setProjectID(QString(id));
}


void QtFrontend::setSceneID(const char *id)
{
    mw->setSceneID(QString(id));
}


void QtFrontend::setTakeID(const char *id)
{
    mw->setTakeID(QString(id));
}


void QtFrontend::setExposureID(const char *id)
{
    mw->setExposureID(QString(id));
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
    qDebug("QtFrontend::initializePreferences --> Start");

    QDir homeDir = QDir::home();
    QString preferencesFile = homeDir.absolutePath();
    preferencesFile.append(QLatin1String("/"));
    preferencesFile.append(PreferencesTool::applicationDirectory);
    preferencesFile.append(QLatin1String("/"));
    preferencesFile.append(PreferencesTool::preferencesName);
    preferencesFile.append(QLatin1String("."));
    preferencesFile.append(PreferencesTool::preferencesSuffix);

    if (!preferencesTool->setPreferencesFile(preferencesFile, "0.9")) {
        // File doesn't exist or is corrupt
        setDefaultPreferences();
    }

    qDebug("QtFrontend::initializePreferences --> End");
}


void QtFrontend::setDefaultPreferences()
{
    qDebug("QtFrontend::setDefaultPreferences --> Start");

    preferencesTool->setBasicPreferenceDefaults();
    // preferencesTool->setEncoderDefaults();

    qDebug("QtFrontend::setDefaultPreferences --> End");
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


int QtFrontend::runExternalCommand(const QString &command)
{
    ExternalCommandDialog *ec = new ExternalCommandDialog;
    ec->show();
    ec->run(command);
    return 0;
}


bool QtFrontend::startGrabber()
{
    bool ret;

    qDebug("QtFrontend::on --> Start");

    ret = mw->startGrabber();

    qDebug("QtFrontend::on --> End");
    return ret;
}


void QtFrontend::stopGrabber()
{
    qDebug("QtFrontend::off --> Start");

    mw->stopGrabber();

    qDebug("QtFrontend::off --> End");
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


const QImage QtFrontend::getActualImage()
{
    return mw->getActualImage();
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
    ret = homeDir.mkpath(getTrashDirName());
    Q_ASSERT(ret == true);
    // setPermissions(...)
    ret = homeDir.mkpath(getPackerDirName());
    Q_ASSERT(ret == true);
    // setPermissions(...)
}


void QtFrontend::removeApplicationDirectories()
{
    QDir homeDir(getUserDirName());
    bool ret;

    ret = homeDir.rmdir(getTempDirName());
    Q_ASSERT(ret == true);
    ret = homeDir.rmdir(getTrashDirName());
    Q_ASSERT(ret == true);
    ret = homeDir.rmdir(getPackerDirName());
    Q_ASSERT(ret == true);
}


void QtFrontend::removeApplicationFiles()
{
    bool ret;

    ret = removeContentInDirectory(getTempDirName());
    Q_ASSERT(ret == true);
    ret = removeContentInDirectory(getTrashDirName());
    Q_ASSERT(ret == true);
    ret = removeContentInDirectory(getPackerDirName());
    Q_ASSERT(ret == true);
    getProject()->removeHistoryFile();
}


void QtFrontend::removeCaptureFiles()
{
    bool ret;

    QDir homeDir(getUserDirName());
    QStringList nameFilter;
    QString fileName(PreferencesTool::capturedFileName);
    fileName.append(".");
    fileName.append(PreferencesTool::capturedFileSuffix);
    nameFilter.append(fileName);

    homeDir.setNameFilters(nameFilter);
    homeDir.setFilter(QDir::Files);
    QStringList fileNames = homeDir.entryList();
    if (fileNames.count() > 0) {
        ret = homeDir.remove(fileNames[0]);
        Q_ASSERT(ret == true);
    }
}


bool QtFrontend::isRecoveryMode()
{
    qDebug("QtFrontend::isRecoveryMode --> Start");

    if (QFile::exists(getTempDirName()) == false) {
        qDebug("QtFrontend::isRecoveryMode --> End (False)");
        return false;
    }
    if (QFile::exists(getTrashDirName()) == false) {
        qDebug("QtFrontend::isRecoveryMode --> End (False)");
        return false;
    }
    if (QFile::exists(getPackerDirName()) == false) {
        qDebug("QtFrontend::isRecoveryMode --> End (False)");
        return false;
    }

    // Everything is intact and we have to run in recovery mode
    qDebug("QtFrontend::isRecoveryMode --> End (True)");
    return true;
}


bool QtFrontend::recoverProject()
{
    qDebug("QtFrontend::recoverProject --> Start");

    int ret = askQuestion(tr("Recovery"),
                  tr("Something caused qStopmotion to exit abnormally\n"
                  "last time it was runned. Do you want to recover?"));
    // The user wants to recover
    if (ret != 0) {
        qDebug("QtFrontend::recoverProject --> End (False)");
        return false;
    }

    mw->setWindowTitle(tr("qStopMotion - Recovered Project"));
    bool recovered = getProject()->recoverProject();

    qDebug("QtFrontend::recoverProject --> End");
    return recovered;
}


void QtFrontend::startDialog()
{
    qDebug("QtFrontend::startDialog --> Start");

    mw->startDialog();

    qDebug("QtFrontend::startDialog --> End");
}


void QtFrontend::setToolBarState(int newState)
{
    qDebug("MainWindowGUI::setToolBarState --> Start");

    mw->setToolBarState(newState);

    qDebug("MainWindowGUI::setToolBarState --> End");
}


int QtFrontend::getVideoSource()
{
    return mw->getVideoSource();
}


bool QtFrontend::setVideoSource(int index)
{
    return mw->setVideoSource(index);
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
        QDebug(QtDebugMsg) << "QtFrontend::test of level: " << level << " --> End (no childs) <<<<<<<<<<<<<<<=======================";
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
