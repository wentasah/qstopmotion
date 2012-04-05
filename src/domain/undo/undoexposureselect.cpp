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
    undoFlag = FALSE;
    setText(QString(tr("Select exposure (%1,%2,%3)-->(%4,%5,%6)"))
            .arg(oldSceneIndex).arg(oldTakeIndex).arg(oldExposureIndex)
            .arg(newSceneIndex).arg(newTakeIndex).arg(newExposureIndex));
}


UndoExposureSelect::~UndoExposureSelect()
{
}


void UndoExposureSelect::undo()
{
    qDebug("UndoExposureSelect::undo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveExposureIndex(oldExposureIndex);
    if (0 <= oldExposureIndex) {
        Exposure *activeExposure = animationProject->getActiveExposure();
        frontend->setExposureID(activeExposure->getId().toAscii());
    }
    else {
        frontend->setExposureID("---");
    }
    facade->getView()->notifyActivateExposure();

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QString("undo"));
    undoFlag = TRUE;

    qDebug("UndoExposureSelect::undo --> End");
}


void UndoExposureSelect::redo()
{
    qDebug("UndoExposureSelect::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveExposureIndex(newExposureIndex);
    if (0 <= newExposureIndex) {
        Exposure *activeExposure = animationProject->getActiveExposure();
        frontend->setExposureID(activeExposure->getId().toAscii());
    }
    else {
        frontend->setExposureID("---");
    }
    facade->getView()->notifyActivateExposure();

    animationProject->incAnimationChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QString("redo"));
        undoFlag = FALSE;
    }
    else {
        facade->writeHistoryEntry(QString("redoExposureSelect|%1|%2|%3|%4|%5|%6")
                                  .arg(oldSceneIndex).arg(oldTakeIndex).arg(oldExposureIndex)
                                  .arg(newSceneIndex).arg(newTakeIndex).arg(newExposureIndex));
    }

    qDebug("UndoExposureSelect::redo --> End");
}
