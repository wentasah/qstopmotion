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

#ifndef EDITMENUHANDLER_H
#define EDITMENUHANDLER_H

#include "frontends/frontend.h"
#include "frontends/qtfrontend/timeline/timeline.h"

#include <QtCore/QObject>


/**
 * This class handles request related to the editmenu (undo, redo, copy, cut, etc)
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class EditMenuHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates and initializes the Soundhandler.
     * @param f the GUI frontend
     * @param parent the parent of the soundhandler.
     * @param timeLine the timeLine containing thumbnails
     * @param name the name of the soundhandler
     */
    EditMenuHandler(Frontend *f,
                    QObject *parent = 0,
                    TimeLine *timeLine = 0,
                    const char *name = 0);

    /**
     * Sets the gotoMenu property so that it can be closed.
     * @param gotoMenu the gotoMenu.
     */
    void setGotoMenu(QWidget *gotoMenu);

public slots:

    /**
     * Goes to the exposure with the index exposureIndex on the timeline.
     * @param exposureIndex the index of the exposure to go to.
     */
    void gotoFrame(int exposureIndex);

    /**
     * Closes the goto menu and moves the focus to the mainwindowgui.
     */
    void closeGotoMenu();

    /**
     * Copies a selection to the global clipboard.
     */
    void copy();

    /**
     * Pastes a selection from the global clipboard to the program.
     */
    void paste();

    /**
     * Start a undo task.
     */
    void undo();

    /**
     * Start a redo task.
     */
    void redo();

private:
    Frontend   *frontend;
    TimeLine   *timeLine;
    QWidget    *gotoMenu;

signals:
    /**
     * Adds the frames pointed to by a list.
     * @param l the list containing pointers to the frames
     */
    // void addFrames(const QStringList &l);
};

#endif
