/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include <QFile>
#include <QProcess>


const QString Util::checkCommand(const QString &command)
{
    Q_ASSERT(!command.isEmpty());

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Windows version
    QString tmp(QString("%1%2")
                .arg(QLatin1String("where "))
                .arg(command));
#else
    // Linux and Apple OS X version
    QString tmp(QString("%1%2")
                .arg(QLatin1String("which "))
                .arg(command));
#endif

    QProcess which;
    which.start(tmp);
    if (!which.waitForStarted())
        return QString();

    if (!which.waitForFinished())
        return QString();

    QString line(which.readAll());

    if (!line.isEmpty()) {
        if (line.endsWith("\n")) {
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
            // Windows version (remove CR and LF)
            return Util::convertPathFromOsSpecific(line.left(line.length()-2));
#else
            // Linux and Apple OS X version (remove LF)
            return Util::convertPathFromOsSpecific(line.left(line.length()-1));
#endif
        }
        return Util::convertPathFromOsSpecific(line);
    }

    return QString();
}

const QString Util::convertPathToOsSpecific(const QString &path)
{
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
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

    QString newPath2("");
    for (int index = 0 ; index < newPath.length() ; index++) {
        QChar unicodeChar = newPath[index];
        int   intChar = unicodeChar.unicode();
        if (intChar >= 128) {
            newPath2.append(QString("&#%1;").arg(intChar));
        }
        else {
            newPath2.append(unicodeChar);
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
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
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

const QString Util::findDefaultPhotoEditor()
{
    QString editorPath("");

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Windows version
    const QString gimpPath(Util::checkCommand("gimp-?.?"));
    if (!gimpPath.isEmpty()) {
        editorPath = gimpPath;
    } else {
        const QString mspaintPath(Util::checkCommand("mspaint"));
        if (!mspaintPath.isEmpty()) {
            editorPath = mspaintPath;
        } else {
            editorPath = QString("");
        }
    }
#else
    // Linux and Apple OS X version
    const QString gimpCommand = Util::checkCommand("gimp");
    if (!gimpPath.isEmpty()) {
        editorPath = gimpPath;
    } else {
        editorPath = QString("");
    }
#endif

    return editorPath;
}
