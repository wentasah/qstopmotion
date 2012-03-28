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

#include "undoprojectnew.h"
#include "frontends/qtfrontend/toolbar.h"


UndoProjectNew::UndoProjectNew(DomainFacade *df,
                               const QString &description)
    :UndoBase(df)
{
    projectDescription.append(description);
    setText(QString(tr("New project '%1'")).arg(projectDescription));
}


UndoProjectNew::~UndoProjectNew()
{

}


void UndoProjectNew::undo()
{
    qDebug("UndoProjectNew::undo --> Start");

    facade->writeHistoryEntry(QString("undoProjectNew|%1").arg(projectDescription));

    qDebug("UndoProjectNew::undo --> End");
}


void UndoProjectNew::redo()
{
    qDebug("UndoProjectNew::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->newProject(projectDescription);
    facade->setProjectSettingsToDefault();
    facade->getView()->notifyNewProject();
    frontend->setProjectID(facade->getProjectDescription().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");
    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    animationProject->setUnsavedChanges();

    facade->writeHistoryEntry(QString("redoProjectNew|%1").arg(projectDescription));

    qDebug("UndoProjectNew::redo --> End");
}
