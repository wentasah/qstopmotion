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

#ifndef UNDOEXPOSUREREMOVE_H
#define UNDOEXPOSUREREMOVE_H

#include <domain/undo/undobase.h>

/**
 * The UndoExposureRemove class for undoing removeExposure(...) calls to the project.
 */
class UndoExposureRemove : public UndoBase
{
    Q_OBJECT
public:

    /**
     * Sets up the UndoAdd command object with the information needed to undo and
     * redo the add commands.
     *
     * Note: The needed information ``toIndex'' is extracted from the length of the
     * frameNames vector.
     * @param df Domain facade for commands.
     * @arg si the index of the scene of the exposure to remove
     * @arg ti the index of the take of the exposure to remove
     * @arg ei the index of the exposure to remove
     */
    UndoExposureRemove(DomainFacade *df,
                       int           si,
                       int           ti,
                       int           ei);

    ~UndoExposureRemove();

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
    int           sceneIndex;
    int           takeIndex;
    int           exposureIndex;
    Exposure     *exposure;
};

#endif
