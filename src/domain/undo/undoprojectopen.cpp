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
    project = NULL;
    setText(QString(tr("Open project '%1'")).arg(projectPath.mid(projectPath.lastIndexOf('/')+1)));
}


UndoProjectOpen::~UndoProjectOpen()
{
    if (NULL != project) {
        delete project;
        project = NULL;
    }
}


void UndoProjectOpen::undo()
{
    qDebug("UndoProjectOpen::undo --> Start");

    Q_ASSERT(NULL == project);

    project = facade->removeProject();

    facade->getView()->notifyRemoveProject();

    // animationProject->clearProject();

    // TODO: Implement separate frontende changes
    // frontend->setFrontendChanges();

    facade->writeHistoryEntry(QString("undo"));

    qDebug("UndoProjectOpen::undo --> End");
}


void UndoProjectOpen::redo()
{
    qDebug("UndoProjectOpen::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();
    bool projectOpen = TRUE;

    // facade->getView()->notifyNewProject();

    if (NULL == project) {
        // First call of the redo function after creation of the undo object
        projectOpen = facade->openProject(projectPath);

        facade->writeHistoryEntry(QString("redoProjectOpen|%1").arg(projectPath));
    }
    else {
        // Call of the redo function after a undo call
        facade->openProject(project);

        facade->writeHistoryEntry(QString("redo"));
    }

    if (projectOpen) {
        facade->getView()->notifyDescriptionsUpdated();
        frontend->setProjectID(facade->getProjectDescription().toAscii());

        Scene *scene = facade->getActiveScene();
        Q_ASSERT(NULL != scene);
        facade->getView()->notifyActivateScene();
        frontend->setSceneID(scene->getDescription().toAscii());

        Take *take = facade->getActiveTake();
        Q_ASSERT(NULL != take);
        facade->getView()->notifyActivateTake();
        frontend->setTakeID(take->getDescription().toAscii());

        Exposure *exposure = facade->getActiveExposure();
        if (NULL != exposure) {
            facade->getView()->notifyActivateExposure();
            frontend->setExposureID(exposure->getId().toAscii());
        }
    }

    qDebug("UndoProjectOpen::redo --> End");
}
