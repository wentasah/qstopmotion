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

#include "undotakeselect.h"


UndoTakeSelect::UndoTakeSelect(DomainFacade *df,
                               int           nsi,
                               int           nti)
    :UndoBase(df)
{
    oldSceneIndex = facade->getActiveSceneIndex();
    oldTakeIndex = facade->getActiveTakeIndex();
    newSceneIndex = nsi;
    newTakeIndex = nti;
    setText(QString(tr("Select take (%1,%2)")).arg(newSceneIndex).arg(newTakeIndex));
}


UndoTakeSelect::~UndoTakeSelect()
{
}


void UndoTakeSelect::undo()
{
    /* TODO: Change handling for undo
    facade->undoTakeSelect(oldSceneIndex, oldTakeIndex, newSceneIndex, newTakeIndex);
    */
}


void UndoTakeSelect::redo()
{
    facade->redoTakeSelect(oldSceneIndex, oldTakeIndex, newSceneIndex, newTakeIndex);
    facade->writeHistoryEntry(QString("redoTakeSelect|%1|%2|%3|%4").arg(oldSceneIndex).arg(oldTakeIndex)
                              .arg(newSceneIndex).arg(newTakeIndex));
}
