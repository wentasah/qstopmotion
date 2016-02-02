/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include <QDebug>
#include <QLatin1String>

#include "frontends/qtfrontend/toolbar.h"


UndoProjectNew::UndoProjectNew(DomainFacade *df,
                               const QString &description)
    :UndoBase(df)
{
    projectDescription.append(description);
    project = NULL;
    setText(QString(tr("New project '%1'")).arg(projectDescription));
}


UndoProjectNew::~UndoProjectNew()
{
    if (NULL != project) {
        delete project;
        project = NULL;
    }
}


void UndoProjectNew::undo()
{
    qDebug() << "UndoProjectNew::undo --> Start";

    Q_ASSERT(NULL == project);

    Frontend *frontend = facade->getFrontend();

    project = facade->removeProject();

    facade->getView()->notifyRemoveProject();
    frontend->setProjectID("---");
    frontend->setSceneID("---");
    frontend->setTakeID("---");
    frontend->setExposureID("---");
    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    facade->writeHistoryEntry(QLatin1String("undo"));

    qDebug() << "UndoProjectNew::undo --> End";
}


void UndoProjectNew::redo()
{
    qDebug() << "UndoProjectNew::redo --> Start";

    Frontend *frontend = facade->getFrontend();

    if (NULL == project) {
        // First call of the redo function after creation of the undo object
        facade->newProject(projectDescription);
        facade->setProjectSettingsToDefault();

        facade->writeHistoryEntry(QString("%1|%2")
                                  .arg(QLatin1String("redoProjectNew"))
                                  .arg(projectDescription));
    }
    else {
        // Call of the redo function after a undo call
        facade->newProject(project);
        project = NULL;

        facade->writeHistoryEntry(QLatin1String("redo"));
    }

    facade->getView()->notifyNewProject();
    frontend->setProjectID(facade->getProjectDescription());
    frontend->setSceneID("---");
    frontend->setTakeID("---");
    frontend->setExposureID("---");
    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    qDebug() << "UndoProjectNew::redo --> End";
}
