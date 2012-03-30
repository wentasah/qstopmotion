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

#ifndef EXTERNALCHANGEMONITOR_H
#define EXTERNALCHANGEMONITOR_H

#include "frontends/frontend.h"

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QSocketNotifier;

/**
 * Class for listening after changes to the disc representation of the
 * animationmodel.
 *
 * The class uses Inotify-tools to listen for changes
 * in the project directories.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ExternalChangeMonitor : public QObject
{
    Q_OBJECT
public:
    /**
     * Initializes the ExternalChangeMonitor and registers the animationModel
     * to notify of changes when the disk representation of the animation is
     * changed
     * @param frontend the GUI frontend
     * @param parent the parent of this QOject.
     */
    ExternalChangeMonitor(Frontend *f,
                          QObject *parent = 0);

    /**
     * Calls stopMonitoring and cleans up.
     */
    ~ExternalChangeMonitor();

    /**
     * Register the directory for monitoring.
     * @param directory the directory to listen for changes in.
     */
    void addDirectory(const QString &directory);

    /**
     * Remove the directory from monitoring.
     * @param directory the directory to remove.
     */
    void removeDirectory(const QString &directory);

    /**
     * Remove the directory from monitoring.
     * @param directory the directory to remove.
     */
    void removeAllDirectories();

    /**
     * Register the file for monitoring.
     * @param file the file to listen for changes in.
     */
    void addFile(const QString &file);

    /**
     * Remove the file from monitoring.
     * @param file the file to remove.
     */
    void removeFile(const QString &file);

    /**
     * Remove all files from monitoring.
     */
    void removeAllFiles();

    /**
     * Creates a inotify connection and listens for changes in the project directories.
     */
    void startMonitoring();

    /**
     * Stop the listening for changes in the project directories.
     */
    void stopMonitoring();

private slots:

    /**
     * Callback function for when the QFileSystemWatcher send a directory changed event.
     * @param dir The changed directory.
     */
    void directoryChangedEvents(const QString &dir);

    /**
     * Callback function for when the QFileSystemWatcher send a file changed event.
     * @param file The changed file.
     */
    void fileChangedEvents(const QString &file);

private:

    Frontend           *frontend;
    QStringList         directoryList;
    QStringList         fileList;
    QFileSystemWatcher *fileSystemWatcher;

};

#endif
