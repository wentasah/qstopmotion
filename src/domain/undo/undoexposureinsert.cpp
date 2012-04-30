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

#include "undoexposureinsert.h"

#include "domain/animation/exposure.h"

#include <QtCore/QVector>


UndoExposureInsert::UndoExposureInsert(DomainFacade  *df,
                                       const QString &fn,
                                       int            si,
                                       int            ti,
                                       int            ei)
    :UndoBase(df)
{
    sceneIndex = si;
    takeIndex = ti;
    exposureIndex = ei;
    exposure = NULL;
    fileName.append(fn);
    setText(QString(tr("Insert exposure (%1,%2,%3) '%4'"))
            .arg(sceneIndex).arg(takeIndex).arg(exposureIndex)
            .arg(fileName.mid(fileName.lastIndexOf('/')+1)));
}


UndoExposureInsert::~UndoExposureInsert()
{
    if (NULL != exposure) {
        delete exposure;
        exposure = NULL;
    }
}


void UndoExposureInsert::undo()
{
    qDebug("UndoExposureInsert::undo --> Start");

    Q_ASSERT(NULL == exposure);

    AnimationProject *animationProject = facade->getAnimationProject();

    exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    exposure->moveToTrash(facade->isRecovering());

    facade->getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QString("undo"));

    qDebug("UndoExposureInsert::undo --> End");
}


void UndoExposureInsert::redo()
{
    qDebug("UndoExposureInsert::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (NULL == exposure) {
        // First call of the redo function after creation of the undo object
        animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex,
                                         fileName, AnimationProject::InTempPath);

        facade->writeHistoryEntry(QString("redoExposureInsert|%1|%2|%3|%4")
                                  .arg(sceneIndex).arg(takeIndex).arg(exposureIndex)
                                  .arg(fileName));
    }
    else {
        // Call of the redo function after a undo call
        exposure->moveToTemp(facade->isRecovering());
        animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex,
                                         exposure);
        exposure = NULL;

        facade->writeHistoryEntry(QString("redo"));
    }

    facade->getView()->notifyInsertExposure(sceneIndex, takeIndex, exposureIndex);

    animationProject->incAnimationChanges();

    qDebug("UndoExposureInsert::redo --> End");
}
