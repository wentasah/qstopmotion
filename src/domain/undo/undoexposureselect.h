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

#ifndef UNDOEXPOSURESELECT_H
#define UNDOEXPOSURESELECT_H

#include <domain/undo/undobase.h>

/**
 * The UndoExposureSelect class for undoing exposureSelect(...) calls to the project.
 */
class UndoExposureSelect : public UndoBase
{
public:

    /**
     * Sets up the UndoExposureSelect command object with the information needed
     * to undo and redo the add commands.
     * @param df Domain facade for commands.
     * @param nsi the new index of the scene of the take of the exposure.
     * @param nti the new index of the take of the exposure.
     * @param nei the new index of the exposure.
     */
    UndoExposureSelect(DomainFacade *df,
                       int           nsi,
                       int           nti,
                       int           nei);

    /**
     * Destructor
     */
    ~UndoExposureSelect();

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
    int  oldSceneIndex;
    int  oldTakeIndex;
    int  oldExposureIndex;
    int  newSceneIndex;
    int  newTakeIndex;
    int  newExposureIndex;
};

#endif
