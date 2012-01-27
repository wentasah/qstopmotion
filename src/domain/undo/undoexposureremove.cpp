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
    setText(QString(QObject::tr("Remove exposure (%1,%2,%3)")).arg(sceneIndex).arg(takeIndex).arg(exposureIndex));
}


UndoExposureRemove::~UndoExposureRemove()
{
}


void UndoExposureRemove::undo()
{
    /* TODO: Change handling for undo
    facade->undoExposureRemove(sceneIndex, takeIndex, exposureIndex);
    */
}


void UndoExposureRemove::redo()
{
    exposure = facade->redoExposureRemove(sceneIndex, takeIndex, exposureIndex);
    facade->writeHistoryEntry(QString("redoExposureRemove %1 %2 %3").arg(sceneIndex).arg(takeIndex).arg(exposureIndex));
}
