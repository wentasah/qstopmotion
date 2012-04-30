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

#include "undoexposureadd.h"

#include "domain/animation/exposure.h"

#include <QtCore/QVector>


UndoExposureAdd::UndoExposureAdd(DomainFacade  *df,
                                 const QString &fn,
                                 int            si,
                                 int            ti)
    :UndoBase(df)
{
    sceneIndex = si;
    takeIndex = ti;
    exposureIndex = facade->getScene(sceneIndex)->getTake(takeIndex)->getExposureSize();
    fileName.append(fn);
    exposure = NULL;

    setText(QString(tr("Add exposure (%1,%2,%3) '%4'"))
            .arg(sceneIndex).arg(takeIndex).arg(exposureIndex)
            .arg(fileName.mid(fileName.lastIndexOf('/')+1)));
}


UndoExposureAdd::~UndoExposureAdd()
{
    if (NULL != exposure) {
        delete exposure;
        exposure = NULL;
    }
}


void UndoExposureAdd::undo()
{
    qDebug("UndoExposureAdd::undo --> Start");

    Q_ASSERT(NULL == exposure);

    AnimationProject *animationProject = facade->getAnimationProject();

    exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    exposure->moveToTrash(facade->isRecovering());

    facade->getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QString("undo"));

    qDebug("UndoExposureAdd::undo --> End");
}


void UndoExposureAdd::redo()
{
    qDebug("UndoExposureAdd::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (NULL == exposure) {
        // First call of the redo function after creation of the undo object
        animationProject->addExposure(sceneIndex, takeIndex, fileName, AnimationProject::InTempPath);

        facade->writeHistoryEntry(QString("redoExposureAdd|%1|%2|%3|%4")
                                  .arg(sceneIndex).arg(takeIndex).arg(exposureIndex)
                                  .arg(fileName));
    }
    else {
        // Call of the redo function after a undo call
        exposure->moveToTemp(facade->isRecovering());
        animationProject->addExposure(sceneIndex, takeIndex, exposure);
        exposure = NULL;

        facade->writeHistoryEntry(QString("redo"));
    }

    facade->getView()->notifyAddExposure(sceneIndex, takeIndex, exposureIndex);

    animationProject->incAnimationChanges();

    qDebug("UndoExposureAdd::redo --> End");
}
