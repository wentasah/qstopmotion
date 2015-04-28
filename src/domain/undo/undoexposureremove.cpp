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

#include "undoexposureremove.h"

#include "domain/animation/exposure.h"


UndoExposureRemove::UndoExposureRemove(DomainFacade *df,
                                       int           si,
                                       int           ti,
                                       int           ei)
    :UndoBase(df)
{
    sceneIndex = si;
    takeIndex = ti;
    exposureIndex = ei;
    exposure = NULL;
    undoFlag = FALSE;
    setText(QString(tr("Remove exposure (%1,%2,%3)"))
            .arg(sceneIndex).arg(takeIndex).arg(exposureIndex));
}


UndoExposureRemove::~UndoExposureRemove()
{
    if (NULL != exposure) {
        delete exposure;
        exposure = NULL;
    }
}


void UndoExposureRemove::undo()
{
    qDebug("UndoExposureRemove::undo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (exposureIndex >= animationProject->getTakeExposureSize(sceneIndex, takeIndex)) {
        animationProject->addExposure(sceneIndex, takeIndex,
                                      exposure);

        facade->getView()->notifyAddExposure(sceneIndex, takeIndex, exposureIndex);
    }
    else {
        animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex,
                                         exposure);

        facade->getView()->notifyInsertExposure(sceneIndex, takeIndex, exposureIndex);
    }
    exposure = NULL;
    undoFlag = TRUE;

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));

    qDebug("UndoExposureRemove::undo --> End");
}


void UndoExposureRemove::redo()
{
    qDebug("UndoExposureRemove::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);

    facade->getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    animationProject->incAnimationChanges();

    if (undoFlag) {
        // Call of the redo function after a undo call
        undoFlag = FALSE;

        facade->writeHistoryEntry(QLatin1String("redo"));
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3|%4")
                                  .arg(QLatin1String("redoExposureRemove"))
                                  .arg(sceneIndex)
                                  .arg(takeIndex)
                                  .arg(exposureIndex));
    }

    qDebug("UndoExposureRemove::redo --> End");
}
