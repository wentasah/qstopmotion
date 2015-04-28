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

#include "undosceneremove.h"


UndoSceneRemove::UndoSceneRemove(DomainFacade *df,
                                 int           si)
    :UndoBase(df)
{
    sceneIndex = si;
    removedScene = NULL;
    undoFlag = FALSE;
    setText(QString(tr("Remove scene (%1)")).arg(sceneIndex));
}


UndoSceneRemove::~UndoSceneRemove()
{
    if (NULL != removedScene) {
        delete removedScene;
        removedScene = NULL;
    }
}


void UndoSceneRemove::undo()
{
    qDebug("UndoSceneRemove::undo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (sceneIndex >= animationProject->getSceneSize()) {
        animationProject->addScene(removedScene);

        facade->getView()->notifyAddScene(sceneIndex);
    }
    else {
        animationProject->insertScene(sceneIndex, removedScene);

        facade->getView()->notifyInsertScene(sceneIndex);
    }
    removedScene = NULL;

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = TRUE;

    qDebug("UndoSceneRemove::undo --> End");
}


void UndoSceneRemove::redo()
{
    qDebug("UndoSceneRemove::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    removedScene = animationProject->removeScene(sceneIndex);

    animationProject->incAnimationChanges();

    facade->getView()->notifyRemoveScene(sceneIndex);

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = FALSE;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2")
                                  .arg(QLatin1String("redoSceneRemove"))
                                  .arg(sceneIndex));
    }

    qDebug("UndoSceneRemove::redo --> End");
}
