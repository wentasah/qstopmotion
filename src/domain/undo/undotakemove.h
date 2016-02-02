/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#ifndef UndoTakeMove_H
#define UndoTakeMove_H

#include <domain/undo/undobase.h>


/**
 * The UndoTakeMove class for undoing moveTake(...) calls to the project.
 */
class UndoTakeMove : public UndoBase
{
    Q_OBJECT
public:

    /**
     * Sets up the UndoTakeMove command object with the information needed to undo and
     * redo the add command.
     * @param df Domain facade for commands.
     * @param fsi the index the scene of the take had before being moved.
     * @param fti the index of the take had before being moved.
     * @param tsi the index the scene of the take after moving.
     * @param tti the index of the take after moving.
     */
    UndoTakeMove(DomainFacade *df,
                 int           fsi,
                 int           fti,
                 int           tsi,
                 int           tti);

    /**
     * Cleans up after the undo object.
     */
    ~UndoTakeMove();

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
    int   toSceneIndex;
    int   toTakeIndex;
    bool  undoFlag;
};

#endif
