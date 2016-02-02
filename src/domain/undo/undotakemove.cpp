/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include "undotakemove.h"

#include <QDebug>
#include <QLatin1String>


UndoTakeMove::UndoTakeMove(DomainFacade *df,
                           int           fsi,
                           int           fti,
                           int           tsi,
                           int           tti)
    :UndoBase(df)
{
    fromSceneIndex = fsi;
    fromTakeIndex = fti;
    toSceneIndex = tsi;
    toTakeIndex = tti;
    undoFlag = false;
    setText(QString(tr("Move take (%1,%2)"))
            .arg(toSceneIndex).arg(toTakeIndex));
}


UndoTakeMove::~UndoTakeMove()
{
}


void UndoTakeMove::undo()
{
    qDebug() << "UndoTakeMove::undo --> Start";


    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = true;

    qDebug() << "UndoTakeMove::undo --> Start";
}


void UndoTakeMove::redo()
{
    qDebug() << "UndoTakeMove::redo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();

    animationProject->moveTake(fromSceneIndex, fromTakeIndex, toSceneIndex, toTakeIndex);
    animationProject->incAnimationChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = false;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3|%4|%5")
                                  .arg(QLatin1String("redoTakeMove"))
                                  .arg(fromSceneIndex)
                                  .arg(fromTakeIndex)
                                  .arg(toSceneIndex)
                                  .arg(toTakeIndex));
    }

    qDebug() << "UndoTakeMove::redo --> Start";
}
