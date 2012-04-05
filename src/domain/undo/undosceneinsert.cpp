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

#include "undosceneinsert.h"


UndoSceneInsert::UndoSceneInsert(DomainFacade  *df,
                                 const QString &description,
                                 int            scIndex)
    :UndoBase(df)
{
    sceneIndex = scIndex;
    sceneDescription.append(description);
    scene = NULL;
    setText(QString(tr("Insert scene (%1) '%2'")).arg(sceneIndex).arg(sceneDescription));
}


UndoSceneInsert::~UndoSceneInsert()
{
    if (NULL != scene) {
        delete scene;
        scene = NULL;
    }
}


void UndoSceneInsert::undo()
{
    qDebug("UndoSceneInsert::undo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    scene = animationProject->removeScene(sceneIndex);

    facade->getView()->notifyRemoveScene(sceneIndex);

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QString("undoSceneInsert|%1|%2").arg(sceneIndex).arg(sceneDescription));

    qDebug("UndoSceneInsert::undo --> End");
}


void UndoSceneInsert::redo()
{
    qDebug("UndoSceneInsert::redo --> Start");

    AnimationProject *animationProject = facade->getAnimationProject();

    if (NULL == scene) {
        // First call of the redo function after creation of the undo object
        animationProject->insertScene(sceneIndex, sceneDescription);
    }
    else {
        // Call of the redo function after a undo call
        animationProject->insertScene(sceneIndex, scene);
        scene = NULL;
    }

    facade->getView()->notifyInsertScene(sceneIndex);

    animationProject->incAnimationChanges();

    facade->writeHistoryEntry(QString("redoSceneInsert|%1|%2").arg(sceneIndex).arg(sceneDescription));

    qDebug("UndoSceneInsert::redo --> End");
}
