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
 *
 * @todo replace this class with QFileSystemWatcher!!!!
 */
ExternalChangeMonitor::ExternalChangeMonitor(Frontend *f,
                                             QObject *parent)
    : QObject(parent), socketNotifier(0), fileSystemWatcher(0)
{
    qDebug() << "ExternalChangeMonitor::Constructor --> Start";

    frontend = f;
    // Create the full path of the temp directory
    QDir homeDir = QDir::home();
    QString tmpDirectory;
    tmpDirectory.append(homeDir.absolutePath());
    tmpDirectory.append(QLatin1String("/"));
    tmpDirectory.append(PreferencesTool::applicationDirectory);
    tmpDirectory.append(QLatin1String("/"));
    tmpDirectory.append(PreferencesTool::tempDirectory);
    tmpDirectory.append(QLatin1String("/"));

    // Add the temp directory to the monitored pathes
    addDirectory(tmpDirectory);

    qDebug() << "ExternalChangeMonitor::Constructor --> End";
}


ExternalChangeMonitor::~ExternalChangeMonitor()
{
    qDebug() << "ExternalChangeMonitor::Destructor --> Start";

    // If the monitoring is running, stop it
    stopMonitoring();
    frontend = NULL;

    qDebug() << "ExternalChangeMonitor::Destructor --> End";
}


void ExternalChangeMonitor::addDirectory(const QString &directory)
{
    qDebug() << "ExternalChangeMonitor::addDirectory --> Start";

    // Is the directory always in the list?
    if (directory.isEmpty() || directories.contains(directory)) {
        qDebug() << "ExternalChangeMonitor::addDirectory --> End (without action)";

        return;
    }

    qDebug() << "ExternalChangeMonitor::addDirectory --> Registering directory for changelistening:" << directory;
    // qDebug() << directory.toLocal8Bit();

    directories.append(directory);

    // Restart monitoring with new directory
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::addDirectory --> End";
}


void ExternalChangeMonitor::startMonitoring()
{
    qDebug() << "ExternalChangeMonitor::startMonitoring --> Start";

    if (fileSystemWatcher != 0) {
        qDebug() << "ExternalChangeMonitor::startMonitoring --> Stop Monitoring";
        stopMonitoring();
    }

    qDebug() << "ExternalChangeMonitor::startMonitoring --> Create file system watcher";
    fileSystemWatcher = new QFileSystemWatcher;
    qDebug() << "ExternalChangeMonitor::startMonitoring --> File system watcher created";

    foreach(QString directory, directories) {
        qDebug() << "ExternalChangeMonitor::startMonitoring --> Add directory";
        QString osDirectory(Util::convertPathToOsSpecific(directory));
        fileSystemWatcher->addPath(osDirectory);
        // fileSystemWatcher->addPath(osDirectory.toLocal8Bit());
        qDebug() << "ExternalChangeMonitor::startMonitoring --> Directory added";
        /*
        if (!inotifytools_watch_recursively(directory.toLocal8Bit(), IN_CLOSE_WRITE | IN_DELETE_SELF)) {
            qWarning() << "Failed to start monitoring:";
            qWarning() << strerror(inotifytools_error());
            inotifytools_cleanup();

            qDebug() << "ExternalChangeMonitor::startMonitoring --> End (Error2)";
            return;
        }
        */
    }

    // socketNotifier = new QSocketNotifier(inotifyFd, QSocketNotifier::Read, this);

    qDebug() << "ExternalChangeMonitor::startMonitoring --> Connect to signals";

    connect(fileSystemWatcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChangedEvents(const QString &)));
    connect(fileSystemWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChangedEvents(const QString &)));

    qDebug() << "ExternalChangeMonitor::startMonitoring --> End";
}


void ExternalChangeMonitor::stopMonitoring()
{
    qDebug() << "ExternalChangeMonitor::stopMonitoring --> Start";

    if (socketNotifier != 0) {
        delete socketNotifier;
        socketNotifier = 0;
    }

    // inotifytools_cleanup();
    if (fileSystemWatcher != 0) {
        delete fileSystemWatcher;
        fileSystemWatcher = 0;
    }

    qDebug() << "ExternalChangeMonitor::stopMonitoring --> End";
}


void ExternalChangeMonitor::directoryChangedEvents(const QString &dir)
{
    qDebug() << "ExternalChangeMonitor::directoryChangedEvents --> Start (Nothing) [" << dir << "]";
/*
    Q_UNUSED(socket);

    QTime time;
    time.start();

    struct inotify_event *event = inotifytools_next_event(1);
    if (!event) {
        return;
    }

    do {
        QString filename = QString::fromLocal8Bit(inotifytools_filename_from_wd(event->wd));
        if (event->mask & IN_DELETE_SELF) {
            directories.removeAll(filename);
        } else if (event->len > 0) { // IN_CLOSE_WRITE
            filename += QString::fromLocal8Bit(event->name);
            frontend->getProject()->animationChanged(filename.toLocal8Bit().constData());
        }
    } while (time.msec() < 250 && (event = inotifytools_next_event(1)));

    qDebug() << "ExternalChangeMonitor::directoryChangedEvents --> End";
*/
}


void ExternalChangeMonitor::fileChangedEvents(const QString &file)
{
    qDebug() << "ExternalChangeMonitor::fileChangedEvents --> Start [" << file << "]";

    frontend->showInformation("Not implemented", "ExternalChangeMonitor::fileChangedEvents.");

    qDebug() << "ExternalChangeMonitor::fileChangedEvents --> End";
}


void ExternalChangeMonitor::suspendMonitor()
{
    qDebug() << "ExternalChangeMonitor::suspendMonitor --> Start";
/*
    if (socketNotifier)
        socketNotifier->setEnabled(false);
*/
    qDebug() << "ExternalChangeMonitor::suspendMonitor --> End";
}


void ExternalChangeMonitor::resumeMonitor()
{
    qDebug() << "ExternalChangeMonitor::resumeMonitor --> Start";
/*
    if (socketNotifier)
        socketNotifier->setEnabled(true);
    if (!isMonitoring)
*/
    startMonitoring();

    qDebug() << "ExternalChangeMonitor::resumeMonitor --> End";
}
