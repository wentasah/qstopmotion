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
    setText(QString(tr("Remove exposure (%1,%2,%3)")).arg(sceneIndex).arg(takeIndex).arg(exposureIndex));
}


UndoExposureRemove::~UndoExposureRemove()
{
}


void UndoExposureRemove::undo()
{
    qDebug("UndoExposureRemove::undo --> Start");

    facade->writeHistoryEntry(QString("undoExposureRemove|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(exposureIndex));

    qDebug("UndoExposureRemove::undo --> End");
}


void UndoExposureRemove::redo()
{
    qDebug("UndoExposureRemove::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    facade->getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    if (exposureIndex == animationProject->getActiveScene()->getActiveTake()->getExposureSize()) {
        // The last Exposure are removed

        // this->setActiveExposureIndex(exposureIndex - 1);
        animationProject->setActiveExposureIndex(exposureIndex - 1);
        if (0 <= (exposureIndex - 1)) {
            Exposure *activeExposure = animationProject->getActiveExposure();
            frontend->setExposureID(activeExposure->getId().toAscii());
        }
        else {
            frontend->setExposureID("---");
        }
    }
    else {
        // Not the last Exposure are removed

        animationProject->setActiveExposureIndex(exposureIndex);
        if (0 <= exposureIndex) {
            Exposure *activeExposure = animationProject->getActiveExposure();
            frontend->setExposureID(activeExposure->getId().toAscii());
        }
        else {
            frontend->setExposureID("---");
        }
    }

    // getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    facade->writeHistoryEntry(QString("redoExposureRemove|%1|%2|%3").arg(sceneIndex).arg(takeIndex).arg(exposureIndex));

    qDebug("UndoExposureRemove::redo --> End");
}
