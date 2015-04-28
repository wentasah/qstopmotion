/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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
    take = NULL;

    setText(QString(tr("Insert take (%1,%2) '%3'"))
            .arg(sceneIndex).arg(takeIndex)
            .arg(takeDescription));
}


UndoTakeInsert::~UndoTakeInsert()
{
    if (NULL != take) {
        delete take;
        take = NULL;
    }
}


void UndoTakeInsert::undo()
{
    qDebug("UndoTakeInsert::undo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    take = animationProject->removeTake(sceneIndex, takeIndex);

    facade->getView()->notifyRemoveTake(sceneIndex, takeIndex);

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));

    qDebug("UndoTakeInsert::undo --> End");
}


void UndoTakeInsert::redo()
{
    qDebug("UndoTakeInsert::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (NULL == take) {
        // First call of the redo function after creation of the undo object
        animationProject->insertTake(sceneIndex, takeIndex, takeDescription);

        facade->writeHistoryEntry(QString("%1|%2|%3|%4")
                                  .arg(QLatin1String("redoTakeInsert"))
                                  .arg(sceneIndex)
                                  .arg(takeIndex)
                                  .arg(takeDescription));
    }
    else {
        // Call of the redo function after a undo call
        animationProject->insertTake(sceneIndex, takeIndex, take);
        take = NULL;

        facade->writeHistoryEntry(QLatin1String("redo"));
    }

    facade->getView()->notifyInsertTake(sceneIndex, takeIndex);

    animationProject->incAnimationChanges();

    qDebug("UndoTakeInsert::redo --> End");
}
