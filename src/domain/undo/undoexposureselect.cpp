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

#include "undoexposureselect.h"


UndoExposureSelect::UndoExposureSelect(DomainFacade *df,
                                       int           nsi,
                                       int           nti,
                                       int           nei)
    :UndoBase(df)
{
    oldSceneIndex = facade->getActiveSceneIndex();
    oldTakeIndex = facade->getActiveTakeIndex();
    oldExposureIndex = facade->getActiveExposureIndex();
    newSceneIndex = nsi;
    newTakeIndex = nti;
    newExposureIndex = nei;
    setText(QString(QObject::tr("Select exposure (%1,%2,%3)")).arg(newSceneIndex).arg(newTakeIndex).arg(newExposureIndex));
}


UndoExposureSelect::~UndoExposureSelect()
{
}


void UndoExposureSelect::undo()
{
    /* TODO: Change handling for undo
    facade->undoExposureSelect(oldSceneIndex, oldTakeIndex, oldExposureIndex,
                               newSceneIndex, newTakeIndex, newExposureIndex);
    */
}


void UndoExposureSelect::redo()
{
    facade->redoExposureSelect(oldSceneIndex, oldTakeIndex, oldExposureIndex,
                               newSceneIndex, newTakeIndex, newExposureIndex);
    facade->writeHistoryEntry(QString("redoExposureSelect %1 %2 %3 %4 %5 %6").arg(oldSceneIndex).arg(oldTakeIndex).arg(oldExposureIndex)
                              .arg(newSceneIndex).arg(newTakeIndex).arg(newExposureIndex));
}
