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

#include "undotakeinsert.h"


UndoTakeInsert::UndoTakeInsert(DomainFacade  *df,
                               const QString &description,
                               int            scIndex,
                               int            taIndex)
    :UndoBase(df)
{
    sceneIndex = scIndex;
    takeIndex = taIndex;
    takeDescription.append(description);
    setText(QString(tr("Insert take (%1,%2) '%3'")).arg(sceneIndex).arg(takeIndex).arg(takeDescription));
}


UndoTakeInsert::~UndoTakeInsert()
{

}


void UndoTakeInsert::undo()
{
    qDebug("UndoTakeInsert::undo --> Start");


    facade->writeHistoryEntry(QString("undoTakeInsert|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(takeDescription));

    qDebug("UndoTakeInsert::undo --> End");
}


void UndoTakeInsert::redo()
{
    qDebug("UndoTakeInsert::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    Take *take = animationProject->insertTake(sceneIndex, takeDescription);
    facade->getView()->notifyInsertTake(sceneIndex, takeIndex);

    animationProject->setUnsavedChanges();

    facade->writeHistoryEntry(QString("redoTakeInsert|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(takeDescription));

    qDebug("UndoTakeInsert::redo --> End");
}
