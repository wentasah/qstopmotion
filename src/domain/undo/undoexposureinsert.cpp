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
    fileName.append(fn);
    setText(QString(QObject::tr("Insert exposure (%1,%2,%3) '%4'")).arg(sceneIndex).arg(takeIndex).arg(exposureIndex).arg(fileName));
}


UndoExposureInsert::~UndoExposureInsert()
{
}


void UndoExposureInsert::undo()
{
    /* TODO: Change handling for undo
    facade->undoExposureInsert(fileName, sceneIndex, takeIndex, exposureIndex);
    */
}


void UndoExposureInsert::redo()
{
    facade->redoExposureInsert(fileName, sceneIndex, takeIndex, exposureIndex);
    facade->writeHistoryEntry(QString("redoExposureInsert|%1|%2|%3|%4").arg(sceneIndex).arg(takeIndex).arg(exposureIndex).arg(fileName));
}
