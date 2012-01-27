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

#include "undoscenemove.h"


UndoSceneMove::UndoSceneMove(DomainFacade *df,
                             int           fsi,
                             int           tsi)
    :UndoBase(df)
{
    fromSceneIndex = fsi;
    toSceneIndex = tsi;
    setText(QString(QObject::tr("Move scene (%1,%2)")).arg(fromSceneIndex).arg(toSceneIndex));
}


UndoSceneMove::~UndoSceneMove()
{
}


void UndoSceneMove::undo()
{
    // TODO: Change handling for undo
    // facade->undoSceneMove(fromSceneIndex, toSceneIndex);
}


void UndoSceneMove::redo()
{
    facade->redoSceneMove(fromSceneIndex, toSceneIndex);
    facade->writeHistoryEntry(QString("redoSceneMove %1 %2").arg(fromSceneIndex).arg(toSceneIndex));
}
