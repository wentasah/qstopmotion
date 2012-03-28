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
    setText(QString(tr("Open project '%1'")).arg(projectPath.mid(projectPath.lastIndexOf('/')+1)));
}


UndoProjectOpen::~UndoProjectOpen()
{

}


void UndoProjectOpen::undo()
{
    qDebug("UndoProjectOpen::undo --> Start");

    facade->getView()->notifyClear();

    facade->getAnimationProject()->clearProject();

    facade->writeHistoryEntry(QString("undoProjectOpen|%1").arg(projectPath));

    qDebug("UndoProjectOpen::undo --> End");
}


void UndoProjectOpen::redo()
{
    qDebug("UndoProjectOpen::redo --> Start");

    facade->getView()->notifyNewProject();

    if (facade->getAnimationProject()->openProject(projectPath)) {
        facade->getView()->notifyDescriptionsUpdated();
        facade->getFrontend()->setProjectID(facade->getProjectDescription().toAscii());

        Scene *scene = facade->getActiveScene();
        Q_ASSERT(NULL != scene);
        facade->getView()->notifyActivateScene();
        facade->getFrontend()->setSceneID(scene->getDescription().toAscii());

        Take *take = facade->getActiveTake();
        Q_ASSERT(NULL != take);
        facade->getView()->notifyActivateTake();
        facade->getFrontend()->setTakeID(take->getDescription().toAscii());

        Exposure *exposure = facade->getActiveExposure();
        if (NULL != exposure) {
            facade->getView()->notifyActivateExposure();
            facade->getFrontend()->setExposureID(exposure->getId().toAscii());
        }
    }

    facade->writeHistoryEntry(QString("redoProjectOpen|%1").arg(projectPath));

    qDebug("UndoProjectOpen::redo --> End");
}
