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

#ifndef UTIL_H
#define UTIL_H

#include <QString>


class Util
{
public:
    /**
     * Check the existence of the command on the OS.
     * @arg command The command to search for.
     * @return The path to the command with Qt specific separators or an empty string.
     */
    static const QString checkCommand(const QString &command);

    /**
     * Converts Qt specific separators to the OS specific separator.
     * @arg path The Qt specific path string.
     * @return The OS specific path string.
     */
    static const QString convertPathToOsSpecific(const QString &path);

    /**
     * Converts separators from the OS specific separator to Qt specific separator.
     * @arg path The OS specific path string.
     * @return The Qt specific path string.
     */
    static const QString convertPathFromOsSpecific(const QString &path);

};

#endif

