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

#ifndef UNDOSCENEREMOVE_H
#define UNDOSCENEREMOVE_H

#include <domain/undo/undobase.h>

/**
 * The UndoSceneRemove class for undoing removeScene(...) calls to the project.
 */
class UndoSceneRemove : public UndoBase
{
public:

    /**
     * Sets up the UndoRemoveScene command object with the information needed to undo and
     * redo the add command.
     * @param df Domain facade for commands.
     * @param si the index the removed scene had.
     * command
     */
    UndoSceneRemove(DomainFacade *df,
                    int           si);

    /**
     * Cleans up after the undo object.
     */
    ~UndoSceneRemove();

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
    int    sceneIndex;
    Scene *removedScene;
};

#endif
