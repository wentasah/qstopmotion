/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#ifndef UNDOTAKESELECT_H
#define UNDOTAKESELECT_H

#include <domain/undo/undobase.h>

/**
 * The UndoTakeSelect class for undoing takeSelect(...) calls to the project.
 */
class UndoTakeSelect : public UndoBase
{
public:

    /**
     * Sets up the UndoTakeSelect command object with the information needed
     * to undo and redo the add commands.
     * @param df Domain facade for commands.
     * @param lastIndex the index of the last selected take.
     * @param newIndex the index of the new selected take.
     */
    UndoTakeSelect(DomainFacade *df,
                   int  lastIndex,
                   int  newIndex);

    /**
     * Destructor
     */
    ~UndoTakeSelect();

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
    int  lastTakeIndex;
    int  newTakeIndex;
};

#endif
