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

#include "undotakeadd.h"


UndoTakeAdd::UndoTakeAdd(DomainFacade  *df,
                         const QString &description,
                         int            scIndex)
    :UndoBase(df)
{
    sceneIndex = scIndex;
    takeIndex = df->getScene(sceneIndex)->getTakeSize();
    takeDescription.append(description);
    setText(QString(tr("Add take (%1,%2) '%3'")).arg(sceneIndex).arg(takeIndex).arg(takeDescription));
}


UndoTakeAdd::~UndoTakeAdd()
{

}


void UndoTakeAdd::undo()
{
    qDebug("UndoTakeAdd::undo --> Start");


    facade->writeHistoryEntry(QString("undoTakeAdd|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(takeDescription));

    qDebug("UndoTakeAdd::undo --> End");
}


void UndoTakeAdd::redo()
{
    qDebug("UndoTakeAdd::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    Take *take = animationProject->addTake(sceneIndex, takeDescription);
    facade->getView()->notifyAddTake(sceneIndex, take->getIndex());

    animationProject->setUnsavedChanges();

    facade->writeHistoryEntry(QString("redoTakeAdd|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(takeDescription));

    qDebug("UndoTakeAdd::redo --> End");
}
