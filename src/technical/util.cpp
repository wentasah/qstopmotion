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

#include "technical/util.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QtGlobal>


const QString Util::checkCommand(const QString &command)
{
    Q_ASSERT(!command.isEmpty());

#ifdef Q_WS_WIN
    // Windows version
    QString tmp(QString("where %1").arg(command));
#else
    // Linux and Apple OS X version
    QString tmp(QString("which %1").arg(command));
#endif

    QProcess which;
    which.start(tmp);
    if (!which.waitForStarted())
        return QString("");

    if (!which.waitForFinished())
        return QString("");

    QString line(which.readAll());

    if (!line.isEmpty()) {
        if (line.endsWith("\n")) {
            return line.left(line.length()-1);
        }
        return line;
    }

    return QString();
}

const QString Util::convertPathToOsSpecific(const QString &path)
{
#ifdef Q_WS_WIN
    // Windows version
    QString     newPath("");
    QStringList sections = path.split('/');
    int         sectionsSize = sections.length();

    for (int index = 0 ; index < sectionsSize ; index++) {
        newPath.append(sections.at(index));
        if (index < (sectionsSize -1)) {
            newPath.append("\\");
        }
    }
    return newPath;
#else
    // Linux and Apple OS X version

    // do nothing
    return path;
#endif
}

const QString Util::convertPathFromOsSpecific(const QString &path)
{
#ifdef Q_WS_WIN
    // Windows version
    QString     newPath("");
    QStringList sections = path.split('\\');
    int         sectionsSize = sections.length();

    for (int index = 0 ; index < sectionsSize ; index++) {
        newPath.append(sections.at(index));
        if (index < (sectionsSize -1)) {
            newPath.append("/");
        }
    }
    return newPath;
#else
    // Linux and Apple OS X version

    // do nothing
    return path;
#endif
}
