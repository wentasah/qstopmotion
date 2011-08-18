/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

UndoExposureInsert::UndoExposureInsert(DomainFacade *df,
                                       int scIndex,
                                       int taIndex,
                                       int exIndex,
                                       bool after,
                                       const QString &name,
                                       const QString &description)
{
    facade = df;
    sceneIndex = scIndex;
    takeIndex = taIndex;
    exposureIndex = exIndex;
    afterFlag = after,
    fileName.append(name);
    exposureDescription.append(description);
    setText(QString(QObject::tr("Insert exposure '%1'")).arg(description));
}


UndoExposureInsert::~UndoExposureInsert()
{
}


void UndoExposureInsert::undo()
{
    /* TODO: Change handling for undo
    facade->setActiveSceneIndex(activeSceneIndex);

    // Give animation model order to remove frames in the range 'fromIndex -> end of vector'.
    QVector<Exposure*> newExposures = facade->removeFrames(fromIndex, fromIndex +   frameNames.size() - 1);

    // Deallocates the old allocated image paths.
    int exposuresSize = newExposures.size();
    for (int i = 0; i < exposuresSize; ++i) {
        frameNames[i] = newExposures[i]->getImagePath();
    }
    */
}


void UndoExposureInsert::redo()
{
    facade->insertExposureRedo(sceneIndex, takeIndex, exposureIndex, afterFlag, fileName);
}
