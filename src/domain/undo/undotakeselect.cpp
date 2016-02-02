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

#include "undotakeselect.h"

#include <QDebug>
#include <QLatin1String>


UndoTakeSelect::UndoTakeSelect(DomainFacade *df,
                               int           nsi,
                               int           nti)
    :UndoBase(df)
{
    oldSceneIndex = facade->getActiveSceneIndex();
    oldTakeIndex = facade->getActiveTakeIndex();
    newSceneIndex = nsi;
    newTakeIndex = nti;
    undoFlag = false;
    setText(QString(tr("Select take (%1,%2)-->(%3,%4)"))
            .arg(oldSceneIndex).arg(oldTakeIndex)
            .arg(newSceneIndex).arg(newTakeIndex));
}


UndoTakeSelect::~UndoTakeSelect()
{
}


void UndoTakeSelect::undo()
{
    qDebug() << "UndoTakeSelect::undo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveTakeIndex(oldTakeIndex);
    facade->getView()->notifyActivateTake();
    if (0 <= oldTakeIndex) {
        Take *activeTake = animationProject->getActiveTake();
        frontend->setTakeID(activeTake->getDescription());
        int activeExposureIndex = activeTake->getActiveExposureIndex();
        if (0 <= activeExposureIndex) {
            facade->getView()->notifyActivateExposure();
            frontend->setExposureID(activeTake->getExposure(activeExposureIndex)->getId());
        }
        else {
            frontend->setExposureID("---");
        }
    }
    else {
        frontend->setTakeID("---");
    }

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = true;

    qDebug() << "UndoTakeSelect::undo --> End";
}


void UndoTakeSelect::redo()
{
    qDebug() << "UndoTakeSelect::redo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveTakeIndex(newTakeIndex);
    facade->getView()->notifyActivateTake();
    if (0 <= newTakeIndex) {
        Take *activeTake = animationProject->getActiveTake();
        frontend->setTakeID(activeTake->getDescription());
        int activeExposureIndex = activeTake->getActiveExposureIndex();
        if (0 <= activeExposureIndex) {
            facade->getView()->notifyActivateExposure();
            frontend->setExposureID(activeTake->getExposure(activeExposureIndex)->getId());
        }
        else {
            frontend->setExposureID("---");
        }
    }
    else {
        frontend->setTakeID("---");
    }

    animationProject->incAnimationChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = false;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3|%4|%5")
                                  .arg(QLatin1String("redoTakeSelect"))
                                  .arg(oldSceneIndex)
                                  .arg(oldTakeIndex)
                                  .arg(newSceneIndex)
                                  .arg(newTakeIndex));
    }

    qDebug() << "UndoTakeSelect::redo --> End";
}
