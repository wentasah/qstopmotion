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

#include "externalchangemonitor.h"

#include "domain/domainfacade.h"
#include "technical/preferencestool.h"
#include "technical/util.h"

#include <QtCore/QtDebug>
#include <QtCore/QDir>
#include <QtCore/QTime>
#include <QtCore/QSocketNotifier>


/**
 * Class to monitor the changes to the file system and notify the application.
 */
ExternalChangeMonitor::ExternalChangeMonitor(Frontend *f,
                                             QObject *parent)
    : QObject(parent)
{
    qDebug() << "ExternalChangeMonitor::Constructor --> Start";

    fileSystemWatcher = NULL;

    frontend = f;

    qDebug() << "ExternalChangeMonitor::Constructor --> End";
}


ExternalChangeMonitor::~ExternalChangeMonitor()
{
    qDebug() << "ExternalChangeMonitor::Destructor --> Start";

    // If the monitoring is running, stop it
    stopMonitoring();

    directoryList.clear();
    fileList.clear();

    qDebug() << "ExternalChangeMonitor::Destructor --> End";
}


void ExternalChangeMonitor::addDirectory(const QString &directory)
{
    qDebug() << "ExternalChangeMonitor::addDirectory --> Start";

    Q_ASSERT(!directory.isEmpty());

    // Is the directory always in the list?
    Q_ASSERT(!directoryList.contains(directory));

    // Stop monitoring
    stopMonitoring();

    qDebug() << "ExternalChangeMonitor::addDirectory --> Registering directory for changelistening:" << directory;

    directoryList.append(directory);

    // Start monitoring with new directory
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::addDirectory --> End";
}


void ExternalChangeMonitor::removeDirectory(const QString &directory)
{
    qDebug() << "ExternalChangeMonitor::removeDirectory --> Start";

    Q_ASSERT(!directory.isEmpty());
    Q_ASSERT(!directoryList.isEmpty());

    // Is the directory in the list?
    Q_ASSERT(directoryList.contains(directory));

    // Stop monitoring
    stopMonitoring();

    qDebug() << "ExternalChangeMonitor::removeDirectory --> Removing directory:" << directory;

    Q_ASSERT(directoryList.removeOne(directory));

    // Start monitoring without the directory
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::removeDirectory --> End";
}


void ExternalChangeMonitor::removeAllDirectories()
{
    qDebug() << "ExternalChangeMonitor::removeAllDirectories --> Start";

    Q_ASSERT(!directoryList.isEmpty());

    // Stop monitoring
    stopMonitoring();

    // Remove all directories from the list
    directoryList.clear();

    // Start monitoring without the directories
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::removeAllDirectories --> End";
}


void ExternalChangeMonitor::addFile(const QString &file)
{
    qDebug() << "ExternalChangeMonitor::addFile --> Start";

    Q_ASSERT(!file.isEmpty());

    if (fileList.contains(file)) {
        // The file is always in the list!
        qDebug() << "ExternalChangeMonitor::addFile --> End (File exist)";
        return;
    }

    // Stop monitoring
    stopMonitoring();

    qDebug() << "ExternalChangeMonitor::addFile --> Registering file for changelistening:" << file;

    fileList.append(file);

    // Start monitoring with new file
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::addFile --> End";
}


void ExternalChangeMonitor::removeFile(const QString &file)
{
    qDebug() << "ExternalChangeMonitor::removeFile --> Start";

    Q_ASSERT(!file.isEmpty());
    Q_ASSERT(!fileList.isEmpty());

    // Is the file in the list?
    Q_ASSERT(fileList.contains(file));

    // Stop monitoring
    stopMonitoring();

    qDebug() << "ExternalChangeMonitor::removeFile --> Removing file:" << file;

    Q_ASSERT(fileList.removeOne(file));

    // Start monitoring without the file
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::removeFile --> End";
}


void ExternalChangeMonitor::removeAllFiles()
{
    qDebug() << "ExternalChangeMonitor::removeAllFiles --> Start";

    Q_ASSERT(!fileList.isEmpty());

    // Stop monitoring
    stopMonitoring();

    // Remove all files from the file list
    fileList.clear();

    // Start monitoring without the file
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::removeAllFiles --> End";
}


void ExternalChangeMonitor::startMonitoring()
{
    qDebug() << "ExternalChangeMonitor::startMonitoring --> Start";

    Q_ASSERT(NULL == fileSystemWatcher);

    if ((0 == directoryList.length()) && (0 == fileList.length())) {
        // Nothing to monitor
        qDebug() << "ExternalChangeMonitor::startMonitoring --> End (Nothing)";
        return;
    }

    fileSystemWatcher = new QFileSystemWatcher;

    connect(fileSystemWatcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChangedEvents(const QString &)));
    connect(fileSystemWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChangedEvents(const QString &)));

    foreach(QString directory, directoryList) {
        QString osDirectory(Util::convertPathToOsSpecific(directory));
        fileSystemWatcher->addPath(osDirectory);
    }

    foreach(QString file, fileList) {
        QString osFile(Util::convertPathToOsSpecific(file));
        fileSystemWatcher->addPath(osFile);
    }

    qDebug() << "ExternalChangeMonitor::startMonitoring --> End";
}


void ExternalChangeMonitor::stopMonitoring()
{
    qDebug() << "ExternalChangeMonitor::stopMonitoring --> Start";

    if (NULL == fileSystemWatcher) {
        qDebug() << "ExternalChangeMonitor::stopMonitoring --> End (Nothing)";
        // Nothing to do
        return;
    }

    delete fileSystemWatcher;
    fileSystemWatcher = NULL;

    qDebug() << "ExternalChangeMonitor::stopMonitoring --> End";
}


void ExternalChangeMonitor::directoryChangedEvents(const QString &dir)
{
    qDebug() << "ExternalChangeMonitor::directoryChangedEvents --> Start [" << dir << "]";

    frontend->showInformation("Not implemented", "ExternalChangeMonitor::directoryChangedEvents.");

    qDebug() << "ExternalChangeMonitor::directoryChangedEvents --> End";
}


void ExternalChangeMonitor::fileChangedEvents(const QString &file)
{
    qDebug() << "ExternalChangeMonitor::fileChangedEvents --> Start [" << file << "]";

    int modSceneIndex;
    int modTakeIndex;
    int modExposureIndex;

    if (!frontend->getProject()->getModifyedExposure(Util::convertPathFromOsSpecific(file),
                                                     modSceneIndex, modTakeIndex, modExposureIndex)) {
        // Exposure not found!

        qDebug() << "ExternalChangeMonitor::fileChangedEvents --> End (Error)";
        return;
    }
    frontend->getView()->notifyModifyExposure(modSceneIndex, modTakeIndex, modExposureIndex);

    qDebug() << "ExternalChangeMonitor::fileChangedEvents --> End";
}
