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

#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include "frontends/frontend.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QListWidget>


/**
 * This class handles request related to sounds (adding, removing, etc).
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class SoundHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates and initializes the Soundhandler.
     * @param f the GUI frontend
     * @param parent the parent of the soundhandler.
     * @param sb a statusbar for displaying status information to the user.
     * @param homeDir the home directory of the user.
     * @param name the name of the soundhandler
     */
    SoundHandler(Frontend *f,
                 QObject *parent = 0,
                 const QString &homeDir = QString(),
                 const QString &name = QString());

    /**
     * Add the soundsList to the handler so that it can be cleared and filled as
     * needed.
     * @param soundsList the listbox to add to the handler.
     */
    //void setSoundsList(Q3ListBox *soundsList);
    void setSoundsList(QListWidget *soundsList);

public slots:

    /**
     * Attatches a sound to a frame in the animation.
     */
    void addSound();

    /**
     * Removes a sound from a frame in the animation.
     */
    void removeSound();

    /**
     * Sets the name of a sound in a frame in the animation.
     */
    void setSoundName();

private:
    Frontend    *frontend;
    QString      homeDir;
    //Q3ListBox *soundsList;
    QListWidget *soundsList;

signals:
    void soundsChanged();
};

#endif
