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

#include "undoprojectopen.h"


UndoProjectOpen::UndoProjectOpen(DomainFacade *df,
                                 const QString &path)
    :UndoBase(df)
{
    projectPath.append(path);
    setText(QString(QObject::tr("Open project '%1'")).arg(projectPath));
}


UndoProjectOpen::~UndoProjectOpen()
{

}


void UndoProjectOpen::undo()
{
    // TODO: Change handling for undo
    // facade->openProjectUndo(projectPath);
}


void UndoProjectOpen::redo()
{
    facade->openProjectRedo(projectPath);
    facade->writeHistoryEntry(QString("redoProjectOpen %1").arg(projectPath));
}
