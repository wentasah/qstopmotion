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

#include "undosceneinsert.h"


UndoSceneInsert::UndoSceneInsert(DomainFacade  *df,
                                 const QString &description,
                                 int            scIndex)
    :UndoBase(df)
{
    sceneIndex = scIndex;
    sceneDescription.append(description);
    setText(QString(tr("Insert scene (%1) '%2'")).arg(sceneIndex).arg(sceneDescription));
}


UndoSceneInsert::~UndoSceneInsert()
{

}


void UndoSceneInsert::undo()
{
    // TODO: Change handling for undo
    // facade->undoSceneInsert(sceneDescription, sceneIndex);
}


void UndoSceneInsert::redo()
{
    facade->redoSceneInsert(sceneDescription, sceneIndex);
    facade->writeHistoryEntry(QString("redoSceneInsert|%1|%2").arg(sceneIndex).arg(sceneDescription));
}
