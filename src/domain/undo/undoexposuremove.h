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

#ifndef UNDOEXPOSUREMOVE_H
#define UNDOEXPOSUREMOVE_H

#include <domain/undo/undobase.h>

/**
 * The UndoExposureMove class for undoing moveExposure(...) calls to the project.
 */
class UndoExposureMove : public UndoBase
{
    Q_OBJECT
public:

    /**
     * Sets up the UndoExposureMove command object with the information needed to undo and
     * redo the add commands.
     * @param df Domain facade for commands.
     * @param fsi the index the scene of the take of the exposure had before being moved.
     * @param fti the index the take of the exposure had before being moved.
     * @param fei the index of the exposure had before being moved.
     * @param tsi the index the scene of the take of the exposure after moving.
     * @param tti the index the take of the exposure after moving.
     * @param tei the index of the exposure after moving.
     */
    UndoExposureMove(DomainFacade *df,
                     int           fsi,
                     int           fti,
                     int           fei,
                     int           tsi,
                     int           tti,
                     int           tei);

    /**
     * Destructor
     */
    ~UndoExposureMove();

    /**
     * Abstract function for undoing the command represented by this undo object.
     */
    virtual void undo();

    /**
     * Abstract function for redoing (performing) the command represented by this
     * undo object.
     */
    virtual void redo();

private:
    /**
     * The model to perform the redo command on.
     */
    int   fromSceneIndex;
    int   fromTakeIndex;
    int   fromExposureIndex;
    int   toSceneIndex;
    int   toTakeIndex;
    int   toExposureIndex;
    bool  undoFlag;
};

#endif
