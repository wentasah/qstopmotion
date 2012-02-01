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
    setText(QString(QObject::tr("Add exposure (%1,%2,%3) '%4'")).arg(sceneIndex).arg(takeIndex).arg(exposureIndex).arg(fileName));
}


UndoExposureAdd::~UndoExposureAdd()
{
}


void UndoExposureAdd::undo()
{
    /* TODO: Change handling for undo
    facade->undoExposureAdd(fileName, sceneIndex, takeIndex);
    */
}


void UndoExposureAdd::redo()
{
    facade->redoExposureAdd(fileName, sceneIndex, takeIndex);
    facade->writeHistoryEntry(QString("redoExposureAdd|%1|%2|%3|%4").arg(sceneIndex).arg(takeIndex).arg(exposureIndex).arg(fileName));
}
