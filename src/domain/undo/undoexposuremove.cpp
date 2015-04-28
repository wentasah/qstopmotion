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

#include "undoexposuremove.h"


UndoExposureMove::UndoExposureMove(DomainFacade *df,
                                   int           fsi,
                                   int           fti,
                                   int           fei,
                                   int           tsi,
                                   int           tti,
                                   int           tei)
    :UndoBase(df)
{
    fromSceneIndex = fsi;
    fromTakeIndex = fti;
    fromExposureIndex = fei;
    toSceneIndex = tsi;
    toTakeIndex = tti;
    toExposureIndex = tei;
    undoFlag = FALSE;
    setText(QString(tr("Move exposure (%1,%2,%3)")).arg(toSceneIndex).arg(toTakeIndex).arg(toExposureIndex));
}


UndoExposureMove::~UndoExposureMove()
{
}


void UndoExposureMove::undo()
{
    /* TODO: Implementation of the redoExposureMove function

    qDebug("UndoExposureMove::undo --> Start");


    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = TRUE;

    qDebug("UndoExposureMove::undo --> End");
    */
}


void UndoExposureMove::redo()
{
    /* TODO: Implementation of the redoExposureMove function

    qDebug("UndoExposureMove::redo --> Start");

    Exposure* exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    if (exposureIndex == animationProject->getActiveScene()->getActiveTake()->getExposureSize()) {
        this->setActiveExposureIndex(exposureIndex - 1);
    }
    else {
        this->setActiveExposureIndex(exposureIndex);
    }

    animationProject->setUnsavedChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = FALSE;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3|%4|%5|%6|%7")
                                  .arg(QLatin1String("redoExposureMove")
                                  .arg(fromSceneIndex).arg(fromTakeIndex)
                                  .arg(fromExposureIndex)
                                  .arg(toSceneIndex)
                                  .arg(toTakeIndex)
                                  .arg(toExposureIndex));
    }

    qDebug("UndoExposureMove::redo --> End");
    */
}
