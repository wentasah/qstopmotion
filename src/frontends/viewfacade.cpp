/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#include "viewfacade.h"

#include <QtCore/QtDebug>


ViewFacade::ViewFacade(Frontend *f)
{
    qDebug("ViewFacade::Constructor --> Start");

    frontend = f;

    qDebug("ViewFacade::Constructor --> End");
}


ViewFacade::~ViewFacade()
{
    frontend = NULL;
}


Frontend* ViewFacade::getFrontend()
{
    return this->frontend;
}


DomainFacade* ViewFacade::getProject()
{
    return this->frontend->getProject();
}


void ViewFacade::attatch(Observer *o)
{
    qDebug("ViewFacade::attatch --> Start");

    if (o != NULL) {
        observers.append(o);
    } else {
        qDebug("ViewFacade::attatch --> Trying to attatch a NULL pointer to the ViewFacade");
    }

    qDebug("ViewFacade::attatch --> End");
}


void ViewFacade::detatch(Observer *o)
{
    qDebug("ViewFacade::detatch --> Start");

    //Sequential search for the observer. Complexity: O(N/2) on the average.
    for (int i = 0; i < observers.size(); ++i) {
        if (observers[i] == o) {
            observers.erase(observers.begin() + i);
            break;
        }
    }

    qDebug("ViewFacade::detatch --> End");
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void ViewFacade::notifyRemoveProject()
{
    qDebug("ViewFacade::notifyClear --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateRemoveProject();
    }

    qDebug("ViewFacade::notifyClear --> End");
}


void ViewFacade::notifyNewProject()
{
    qDebug("ViewFacade::notifyNewProject --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateNewProject();
    }

    qDebug("ViewFacade::notifyNewProject --> End");
}


void ViewFacade::notifyOpenProject()
{
    qDebug("ViewFacade::notifyOpenProject --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateOpenProject();
    }

    qDebug("ViewFacade::notifyOpenProject --> End");
}


void ViewFacade::notifyPlaySound(int sceneIndex)
{
    qDebug("ViewFacade::notifyPlaySound --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updatePlaySound(sceneIndex);
    }

    qDebug("ViewFacade::notifyPlaySound --> End");
}


void ViewFacade::notifyNewMixMode(int newMixMode)
{
    qDebug("ViewFacade::notifyNewMixMode --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateMixMode(newMixMode);
    }

    qDebug("ViewFacade::notifyNewMixMode --> End");
}


void ViewFacade::notifyNewMixCount(int newMixCount)
{
    qDebug("ViewFacade::notifyNewMixCount --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateMixCount(newMixCount);
    }

    qDebug("ViewFacade::notifyNewMixCount --> End");
}


void ViewFacade::notifyNewLiveViewFps(int newFps)
{
    qDebug("ViewFacade::notifyNewLiveViewFps --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateLiveViewFps(newFps);
    }

    qDebug("ViewFacade::notifyNewLiveViewFps --> End");
}


void ViewFacade::notifyNewVideoFps(int newFps)
{
    qDebug("ViewFacade::notifyNewVideoFps --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateVideoFps(newFps);
    }

    qDebug("ViewFacade::notifyNewVideoFps --> End");
}

/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void ViewFacade::notifyAddScene(int sceneIndex)
{
    qDebug("ViewFacade::notifyNewScene --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateAddScene(sceneIndex);
    }

    qDebug("ViewFacade::notifyNewScene --> End");
}

void ViewFacade::notifyInsertScene(int sceneIndex)
{
    qDebug("ViewFacade::notifyNewScene --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateInsertScene(sceneIndex);
    }

    qDebug("ViewFacade::notifyNewScene --> End");
}

void ViewFacade::notifyActivateScene()
{
    qDebug("ViewFacade::notifyActivateScene --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateActivateScene();
    }

    qDebug("ViewFacade::notifyActivateScene --> End");
}


void ViewFacade::notifyRemoveScene(int sceneIndex)
{
    qDebug("ViewFacade::notifyRemoveScene --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateRemoveScene(sceneIndex);
    }

    qDebug("ViewFacade::notifyRemoveScene --> End");
}


void ViewFacade::notifyMoveScene(int sceneNumber,
                                 int movePosition)
{
    qDebug("ViewFacade::notifyMoveScene --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateMoveScene(sceneNumber, movePosition);
    }

    qDebug("ViewFacade::notifyMoveScene --> End");
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void ViewFacade::notifyAddTake(int sceneIndex,
                               int takeIndex)
{
    qDebug("ViewFacade::notifyAddTake --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateAddTake(sceneIndex, takeIndex);
    }

    qDebug("ViewFacade::notifyAddTake --> End");
}

void ViewFacade::notifyInsertTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug("ViewFacade::notifyInsertTake --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateInsertTake(sceneIndex, takeIndex);
    }

    qDebug("ViewFacade::notifyInsertTake --> End");
}

void ViewFacade::notifyActivateTake()
{
    qDebug("ViewFacade::notifyActivateTake --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateActivateTake();
    }

    qDebug("ViewFacade::notifyActivateTake --> End");
}


void ViewFacade::notifyRemoveTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug("ViewFacade::notifyRemoveTake --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateRemoveTake(sceneIndex, takeIndex);
    }

    qDebug("ViewFacade::notifyRemoveTake --> End");
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void ViewFacade::notifyAddExposure(int sceneIndex,
                                   int takeIndex,
                                   int exposureIndex)
{
    // qDebug("ViewFacade::notifyAddExposure --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateAddExposure(sceneIndex, takeIndex, exposureIndex);
    }

    // qDebug("ViewFacade::notifyAddExposure --> End");
}


void ViewFacade::notifyInsertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug("ViewFacade::notifyInsertExposure --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateInsertExposure(sceneIndex, takeIndex, exposureIndex);
    }

    qDebug("ViewFacade::notifyInsertExposure --> End");
}


void ViewFacade::notifyActivateExposure()
{
    qDebug("ViewFacade::notifyActivateExposure --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateActivateExposure();
    }

    qDebug("ViewFacade::notifyActivateExposure --> End");
}


void ViewFacade::notifyRemoveExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug("ViewFacade::notifyRemoveExposure --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateRemoveExposure(sceneIndex, takeIndex, exposureIndex);
    }

    qDebug("ViewFacade::notifyRemoveExposure --> End");
}


void ViewFacade::notifyMoveExposures(int fromFrame,
                                     int toFrame,
                                     int movePosition)
{
    qDebug("ViewFacade::notifyMoveExposures --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateMoveExposures(fromFrame, toFrame, movePosition);
    }

    qDebug("ViewFacade::notifyMoveExposures --> End");
}


void ViewFacade::notifyModifyExposure(int modSceneIndex,
                                      int modTakeIndex,
                                      int modExposureIndex)
{
    qDebug("ViewFacade::notifyModifyExposure --> Start");

    int numElem = observers.size();
    for (int i = 0; i < numElem; ++i) {
        observers[i]->updateModifyExposure(modSceneIndex, modTakeIndex, modExposureIndex);
    }

    qDebug("ViewFacade::notifyModifyExposure --> End");
}


