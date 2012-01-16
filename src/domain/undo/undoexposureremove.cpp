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
                                       int  scIndex,
                                       int  taIndex,
                                       int  exIndex)
    :UndoBase(df)
{
    sceneIndex = scIndex;
    takeIndex = taIndex;
    exposureIndex = exIndex;
    exposure = NULL;
    setText(QObject::tr("Remove exposure"));
}


UndoExposureRemove::~UndoExposureRemove()
{
}


void UndoExposureRemove::undo()
{
    /* TODO: Change handling for undo
    facade->setActiveSceneIndex(activeSceneIndex);

    QVector<Exposure*> newExposures = animationProject->addFrames(frameNames, fromIndex);

    // Deallocates the old allocated image paths.
    int numElem = newExposures.size();
    for (int i = 0; i < numElem; ++i) {
        frameNames[i] = newExposures[i]->getImagePath();
    }
    */
}


void UndoExposureRemove::redo()
{
    exposure = facade->removeExposureRedo(sceneIndex, takeIndex, exposureIndex);
}
