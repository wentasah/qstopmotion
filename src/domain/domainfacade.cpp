/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#include "domainfacade.h"

#include "domain/animation/animationproject.h"
#include "domain/undo/undoexposureadd.h"
#include "domain/undo/undoexposureinsert.h"
#include "domain/undo/undoexposuremove.h"
#include "domain/undo/undoexposureremove.h"
#include "domain/undo/undoprojectclose.h"
#include "domain/undo/undoprojectnew.h"
#include "domain/undo/undoprojectopen.h"
#include "domain/undo/undoprojectsave.h"
#include "domain/undo/undosceneadd.h"
#include "domain/undo/undosceneinsert.h"
#include "domain/undo/undoscenemove.h"
#include "domain/undo/undosceneremove.h"
#include "domain/undo/undotakeadd.h"
#include "domain/undo/undotakeinsert.h"
#include "domain/undo/undotakeremove.h"
#include "domain/undo/undotakemove.h"

#include <QtCore/QtDebug>


DomainFacade::DomainFacade(Frontend *f)
{
    frontend = f;
    animationProject = new AnimationProject(frontend);
}


DomainFacade::~DomainFacade()
{
    if (animationProject != NULL) {
        delete animationProject;
        animationProject = NULL;
    }
    frontend = NULL;
}


Frontend* DomainFacade::getFrontend()
{
    return this->frontend;
}


AnimationProject* DomainFacade::getAnimationProject()
{
    return this->animationProject;
}


QUndoStack* DomainFacade::getUndoStack()
{
    return animationProject->getUndoStack();
}


ViewFacade* DomainFacade::getView()
{
    return frontend->getView();
}


void DomainFacade::clearUndoStack()
{
    animationProject->clearUndoStack();
}


bool DomainFacade::initAudioDevice()
{
    return animationProject->initAudioDevice();
}


void DomainFacade::shutdownAudioDevice()
{
    return animationProject->shutdownAudioDevice();
}


/**************************************************************************
 * Animation functions
 **************************************************************************/

bool DomainFacade::isUnsavedChanges()
{
    return animationProject->isUnsavedChanges();
}


void DomainFacade::setProjectFileName(const QString &fileName)
{
    animationProject->setProjectFileName(fileName);
}


const QString DomainFacade::getProjectFileName()
{
    return animationProject->getProjectFileName();
}


const QString DomainFacade::getProjectPath()
{
    return animationProject->getProjectPath();
}


const QString DomainFacade::getProjectID()
{
    return animationProject->getProjectID();
}


int DomainFacade::getVideoSource()
{
    return animationProject->getVideoSource();
}


void DomainFacade::setVideoSource(int newVideoSource)
{
    animationProject->setVideoSource(newVideoSource);
}


int DomainFacade::getMixingMode()
{
    return animationProject->getMixingMode();
}


void DomainFacade::setMixingMode(int newMixingMode)
{
    animationProject->setMixingMode(newMixingMode);
}


int DomainFacade::getUnitMode()
{
    return animationProject->getUnitMode();
}


void DomainFacade::setUnitMode(int newUnitMode)
{
    animationProject->setUnitMode(newUnitMode);
}


int DomainFacade::getMixCount()
{
    return animationProject->getMixCount();
}


void DomainFacade::setMixCount(int newMixCount)
{
    animationProject->setMixCount(newMixCount);
}


int DomainFacade::getPlaybackCount()
{
    return animationProject->getPlaybackCount();
}


void DomainFacade::setPlaybackCount(int newPlaybackCount)
{
    animationProject->setPlaybackCount(newPlaybackCount);
}


int DomainFacade::getFramesPerSecond()
{
    return animationProject->getFramesPerSecond();
}


void DomainFacade::setFramesPerSecond(int newFps)
{
    animationProject->setFramesPerSecond(newFps);
    getView()->notifyNewFramesPerSecond(newFps);
}


void DomainFacade::newProjectToUndo(const QString &projectDescription)
{
    UndoProjectNew *u = new UndoProjectNew(this, projectDescription);
    getUndoStack()->push(u);
}


bool DomainFacade::newProjectRedo(const QString &projectDescription)
{
    qDebug("DomainFacade::newProjectRedo --> Start");

    getView()->notifyClear();
    animationProject->newProject(projectDescription);
    getView()->notifyNewProject();
    frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::newProjectRedo --> End");
    return true;
}

/*
bool DomainFacade::newProjectUndo(const QString &projectDescription)
{
    qDebug("DomainFacade::newProjectUndo --> Start");

    qDebug("DomainFacade::newProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::newProject(const QString &projectDescription)
{
    qDebug("DomainFacade::newProject --> Start");

    getView()->notifyClear();
    getView()->notifyNewProject();
    frontend->setProjectID(projectDescription.toAscii());
    // frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");

    qDebug("DomainFacade::newProject --> End");
    return true;
}


void DomainFacade::openProjectToUndo(const QString &projectPath)
{
    UndoProjectOpen *u = new UndoProjectOpen(this, projectPath);
    getUndoStack()->push(u);
}


bool DomainFacade::openProjectRedo(const QString &projectPath)
{
    qDebug("DomainFacade::openProjectRedo --> Start");

    /* TODO: Implementation of openProjectRedo
    getView()->notifyClear();
    animationProject->newProject(projectDescription);
    getView()->notifyNewProject();
    frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");

    animationProject->setUnsavedChanges();
    */

    qDebug("DomainFacade::openProjectRedo --> End");
    return true;
}

/*
bool DomainFacade::openProjectUndo(const QString &projectDescription)
{
    qDebug("DomainFacade::openProjectUndo --> Start");

    qDebug("DomainFacade::openProjectUndo --> End");
    return true;
}
*/

void DomainFacade::openProject()
{
    qDebug("DomainFacade::openProject --> Start");

    if (animationProject->openProject()) {
        getView()->notifyDescriptionsUpdated();
        getView()->notifyActivateScene();
        getView()->notifyActivateTake();
        getView()->notifyActivateExposure();
        frontend->setProjectID(getProjectID().toAscii());

        Scene *scene = getActiveScene();
        if (NULL == scene) {
            return;
        }
        frontend->setSceneID(scene->getId().toAscii());

        Take *take = getActiveTake();
        if (NULL == take) {
            return;
        }
        frontend->setTakeID(take->getId().toAscii());

        Exposure *exposure = getActiveExposure();
        if (NULL == exposure) {
            return;
        }
        frontend->setExposureID(exposure->getId().toAscii());
    }

    qDebug("DomainFacade::openProject --> End");
}


void DomainFacade::saveProjectToUndo(const QString &projectPath)
{
    UndoProjectSave *u = new UndoProjectSave(this, projectPath);
    getUndoStack()->push(u);
}


bool DomainFacade::saveProjectRedo(const QString &projectPath)
{
    qDebug("DomainFacade::saveProjectRedo --> Start");

    /* TODO: Implementation of saveProjectRedo
    getView()->notifyClear();
    animationProject->newProject(projectDescription);
    getView()->notifyNewProject();
    frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");

    animationProject->setUnsavedChanges();
    */

    qDebug("DomainFacade::saveProjectRedo --> End");
    return true;
}

/*
bool DomainFacade::saveProjectUndo(const QString &projectDescription)
{
    qDebug("DomainFacade::saveProjectUndo --> Start");

    qDebug("DomainFacade::saveProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::saveProject()
{
    return animationProject->saveProject();
}


void DomainFacade::closeProjectToUndo()
{
    UndoProjectClose *u = new UndoProjectClose(this);
    getUndoStack()->push(u);
}


bool DomainFacade::closeProjectRedo()
{
    qDebug("DomainFacade::closeProjectRedo --> Start");

    /* TODO: Implementation of closeProjectRedo
    getView()->notifyClear();
    animationProject->newProject(projectDescription);
    getView()->notifyNewProject();
    frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");

    animationProject->setUnsavedChanges();
    */

    qDebug("DomainFacade::closeProjectRedo --> End");
    return true;
}

/*
bool DomainFacade::closeProjectUndo()
{
    qDebug("DomainFacade::closeProjectUndo --> Start");

    qDebug("DomainFacade::closeProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::exportToVideo(VideoEncoder *encoder)
{
    return animationProject->exportToVideo(encoder);
}


bool DomainFacade::exportToCinelerra(const QString file)
{
    return animationProject->exportToCinelerra(file);
}

void DomainFacade::activeItemChanged(int newActiveScene,
                                     int newActiveTake,
                                     int newActiveExposure)
{
    // Set the new scene index
    setActiveSceneIndex(newActiveScene);

    // Set the new take index
    setActiveTakeIndex(newActiveTake);

    // Set the new exposure index
    setActiveExposureIndex(newActiveExposure);
}

/**************************************************************************
 * Scene functions
 **************************************************************************/

int DomainFacade::getActiveSceneIndex()
{
    return animationProject->getActiveSceneIndex();
}


void DomainFacade::setActiveSceneIndex(int sceneIndex)
{
    animationProject->setActiveSceneIndex(sceneIndex);
    if (0 <= sceneIndex) {
        Scene *activeScene = animationProject->getActiveScene();
        frontend->setSceneID(activeScene->getDescription().toAscii());
    }
    else {
        frontend->setSceneID("---");
    }
    getView()->notifyActivateScene();

    animationProject->setUnsavedChanges();
}


Scene *DomainFacade::getActiveScene()
{
    return animationProject->getActiveScene();
}


int DomainFacade::getSceneSize()
{
    return animationProject->getSceneSize();
}


Scene *DomainFacade::getScene(int sceneIndex)
{
    return animationProject->getScene(sceneIndex);
}


int DomainFacade::getSceneExposureSize(int sceneIndex)
{
    return animationProject->getSceneExposureSize(sceneIndex);
}


void DomainFacade::addSceneToUndo(const QString &sceneDescription)
{
    UndoSceneAdd *u = new UndoSceneAdd(this, sceneDescription);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoSceneAdd(const QString &sceneDescription)
{
    animationProject->addScene(sceneDescription);
}
*/

Scene *DomainFacade::redoSceneAdd(const QString &sceneDescription)
{
    qDebug("DomainFacade::redoSceneAdd --> Start");

    Scene *scene = animationProject->addScene(sceneDescription);
    getView()->notifyAddScene(scene->getIndex());

    animationProject->setActiveSceneIndex(scene->getIndex());
    getView()->notifyActivateScene();
    getView()->notifyActivateTake();
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoSceneAdd --> End");
    return scene;
}


void DomainFacade::addScene(Scene *newScene)
{
    getView()->notifyAddScene(newScene->getIndex());
}


void DomainFacade::insertSceneToUndo(const QString &sceneDescription,
                                     int sceneIndex)
{
    UndoSceneInsert *u = new UndoSceneInsert(this, sceneDescription, sceneIndex);
    getUndoStack()->push(u);

}

/*
void DomainFacade::undoSceneInsert(const QString &sceneDescription,
                                   int            sceneIndex)
{
}
*/

Scene *DomainFacade::redoSceneInsert(const QString &sceneDescription,
                                     int sceneIndex)
{
    qDebug("DomainFacade::redoSceneInsert --> Start");

    Scene *scene = animationProject->insertScene(sceneIndex, sceneDescription);
    getView()->notifyInsertScene(scene->getIndex());

    animationProject->setActiveSceneIndex(scene->getIndex());
    getView()->notifyActivateScene();
    getView()->notifyActivateTake();
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoSceneInsert --> End");
    return scene;
}


void DomainFacade::removeSceneToUndo(int sceneIndex)
{
    UndoSceneRemove *u = new UndoSceneRemove(this, sceneIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::removeSceneUndo(int sceneIndex)
{
}
*/

Scene *DomainFacade::redoSceneRemove(int sceneIndex)
{
    qDebug("DomainFacade::redoSceneRemove --> Start");

    Scene *removedScene = animationProject->removeScene(sceneIndex);
    animationProject->setUnsavedChanges();

    getView()->notifyRemoveScene(sceneIndex);

    qDebug("DomainFacade::redoSceneRemove --> End");
    return removedScene;
}


void DomainFacade::moveSceneToUndo(int fromSceneIndex,
                                   int toSceneIndex)
{
    UndoSceneMove *u = new UndoSceneMove(this, fromSceneIndex, toSceneIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoSceneMove(int fromSceneIndex,
                                 int toSceneIndex)
{
}
*/

void DomainFacade::redoSceneMove(int fromSceneIndex,
                                 int toSceneIndex)
{
    qDebug("DomainFacade::redoSceneMove --> Start");

    animationProject->moveScene(fromSceneIndex, toSceneIndex);
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoSceneMove --> Start");
}


int DomainFacade::addSoundToScene(int           sceneIndex,
                                  const QString filename,
                                  const QString soundName)
{
    qDebug("DomainFacade::addSoundToScene --> Start");

    return animationProject->addSoundToScene(sceneIndex, filename, soundName);
}


void DomainFacade::removeSoundFromScene(int sceneIndex,
                                        int soundNumber)
{
    animationProject->removeSoundFromScene(sceneIndex, soundNumber);
}


void DomainFacade::playSound(int sceneIndex)
{
    animationProject->playSound(sceneIndex);
}


/**************************************************************************
 * Take functions
 **************************************************************************/

int DomainFacade::getActiveTakeIndex()
{
    return animationProject->getActiveTakeIndex();
}


void DomainFacade::setActiveTakeIndex(int takeIndex)
{
    animationProject->setActiveTakeIndex(takeIndex);
    if (0 <= takeIndex) {
        Take *activeTake = animationProject->getActiveTake();
        frontend->setTakeID(activeTake->getDescription().toAscii());
    }
    else {
        frontend->setTakeID("---");
    }
    getView()->notifyActivateTake();

    animationProject->setUnsavedChanges();
}


Take *DomainFacade::getActiveTake()
{
    return animationProject->getActiveTake();
}


Take *DomainFacade::getTake(int sceneIndex,
                            int takeIndex)
{
    return animationProject->getTake(sceneIndex, takeIndex);
}


int DomainFacade::getTakeExposureSize(int sceneIndex,
                                      int takeIndex)
{
    return animationProject->getTakeExposureSize(sceneIndex, takeIndex);
}


void DomainFacade::addTakeToUndo(const QString &takeDescription,
                                 int            sceneIndex)
{
    UndoTakeAdd *u = new UndoTakeAdd(this, takeDescription, sceneIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoTakeAdd(const QString &takeDescription,
                               int            sceneIndex)
{
}
*/

Take *DomainFacade::redoTakeAdd(const QString &takeDescription,
                                int            sceneIndex)
{
    qDebug("DomainFacade::redoTakeAdd --> Start");

    Take *take = animationProject->addTake(sceneIndex, takeDescription);
    getView()->notifyAddTake(sceneIndex, take->getIndex());

    animationProject->setActiveTakeIndex(take->getIndex());
    getView()->notifyActivateTake();
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoTakeAdd --> End");
    return take;
}


void DomainFacade::addTake(Take *newTake)
{
    getView()->notifyAddTake(newTake->getSceneIndex(), newTake->getIndex());
}


void DomainFacade::insertTakeToUndo(const QString &takeDescription,
                                    int            sceneIndex,
                                    int            takeIndex)
{
    UndoTakeInsert *u = new UndoTakeInsert(this, takeDescription, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoTakeInsert(const QString &takeDescription,
                                  int            sceneIndex,
                                  int            takeIndex)
{
}
*/

Take *DomainFacade::redoTakeInsert(const QString &takeDescription,
                                   int sceneIndex,
                                   int takeIndex)
{
    qDebug("DomainFacade::redoTakeInsert --> Start");

    Take *take = animationProject->insertTake(sceneIndex, takeDescription);
    getView()->notifyInsertTake(sceneIndex, takeIndex);

    animationProject->setActiveTakeIndex(take->getIndex());
    getView()->notifyActivateTake();
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoTakeInsert --> End");
    return take;
}


void DomainFacade::removeTakeToUndo(int sceneIndex,
                                    int takeIndex)
{
    UndoTakeRemove *u = new UndoTakeRemove(this, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoTakeRemove(int sceneIndex,
                                  int takeIndex)
{
}
*/

Take *DomainFacade::redoTakeRemove(int sceneIndex,
                                   int takeIndex)
{
    qDebug("DomainFacade::redoTakeRemove --> Start");

    Take *removedTake = animationProject->removeTake(sceneIndex, takeIndex);
    animationProject->setUnsavedChanges();

    getView()->notifyRemoveTake(sceneIndex, takeIndex);

    qDebug("DomainFacade::redoTakeRemove --> End");
    return removedTake;
}


void DomainFacade::moveTakeToUndo(int fromSceneIndex,
                                  int fromTakeIndex,
                                  int toSceneIndex,
                                  int toTakeIndex)
{
    UndoTakeMove *u = new UndoTakeMove(this, fromSceneIndex, fromTakeIndex, toSceneIndex, toTakeIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoTakeMove(int fromSceneIndex,
                                int fromTakeIndex,
                                int toSceneIndex,
                                int toTakeIndex)
{
}
*/

void DomainFacade::redoTakeMove(int fromSceneIndex,
                                int fromTakeIndex,
                                int toSceneIndex,
                                int toTakeIndex)
{
    qDebug("DomainFacade::redoTakeMove --> Start");

    animationProject->moveTake(fromSceneIndex, fromTakeIndex, toSceneIndex, toTakeIndex);
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoTakeMove --> Start");
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int DomainFacade::getActiveExposureIndex()
{
    int activeExposureIndex = animationProject->getActiveExposureIndex();

    return activeExposureIndex;
}


void DomainFacade::setActiveExposureIndex(int exposureIndex)
{
    animationProject->setActiveExposureIndex(exposureIndex);
    if (0 <= exposureIndex) {
        Exposure *activeExposure = animationProject->getActiveExposure();
        frontend->setExposureID(activeExposure->getId().toAscii());
    }
    else {
        frontend->setExposureID("---");
    }
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();
}


Exposure *DomainFacade::getActiveExposure()
{
    return animationProject->getActiveExposure();
}


Exposure *DomainFacade::getExposure(int sceneIndex,
                                    int takeIndex,
                                    int exposureIndex)
{
    Q_ASSERT(sceneIndex > -1);
    Q_ASSERT(takeIndex > -1);
    Q_ASSERT(exposureIndex > -1);

    return animationProject->getExposure(sceneIndex, takeIndex, exposureIndex);
}


int DomainFacade::getTotalExposureSize()
{
    return animationProject->getTotalExposureSize();
}


void DomainFacade::addExposureToUndo(int sceneIndex,
                                     int takeIndex,
                                     const QString &fileName)
{
    UndoExposureAdd *u = new UndoExposureAdd(this, sceneIndex, takeIndex, 0, fileName, tr("<empty>"));
    getUndoStack()->push(u);
}

/*
void DomainFacade::addExposureUndo(int sceneIndex,
                                   int takeIndex,
                                   const QString &fileName)
{
}
*/

void DomainFacade::addExposureRedo(int sceneIndex,
                                   int takeIndex,
                                   const QString &fileName)
{
    qDebug("DomainFacade::addExposureRedo --> Start");

    Exposure *exposure = animationProject->addExposure(fileName);
    getView()->notifyAddExposure(sceneIndex, takeIndex, exposure->getIndex());
    animationProject->setActiveExposureIndex(exposure->getIndex());
    getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::addExposureRedo --> End");
}


void DomainFacade::addExposure(Exposure *newExposure)
{
    getView()->notifyAddExposure(newExposure->getSceneIndex(), newExposure->getTakeIndex(), newExposure->getIndex());
}


void DomainFacade::insertExposureToUndo(int sceneIndex,
                                        int takeIndex,
                                        int exposureIndex,
                                        bool after,
                                        const QString &fileName)
{
    UndoExposureInsert *u = new UndoExposureInsert(this, sceneIndex, takeIndex, exposureIndex, after, fileName, tr("<empty>"));
    getUndoStack()->push(u);
}

/*
void DomainFacade::insertExposureUndo(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex,
                                      bool after,
                                      const QString &fileName)
{
}
*/

void DomainFacade::insertExposureRedo(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex,
                                      bool after,
                                      const QString &fileName)
{
    qDebug("DomainFacade::insertExposureRedo --> Start");

    Exposure *exposure = NULL;

    if (after) {
        exposure = animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex + 1, fileName);
    }
    else {
        exposure = animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex, fileName);
    }
    getView()->notifyInsertExposure(sceneIndex,
                                    takeIndex,
                                    exposure->getIndex());

    animationProject->setActiveExposureIndex(exposure->getIndex());
    getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::insertExposureRedo --> End");
}

void DomainFacade::removeExposureToUndo(int sceneIndex,
                                        int takeIndex,
                                        int exposureIndex)
{
    UndoExposureRemove *u = new UndoExposureRemove(this, sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::removeExposureUndo(QTreeWidgetItem *scItem,
                                      QTreeWidgetItem *taItem,
                                      QTreeWidgetItem *exItem)
{
}
*/

Exposure *DomainFacade::removeExposureRedo(int sceneIndex,
                                           int takeIndex,
                                           int exposureIndex)
{
    qDebug("DomainFacade::removeExposureRedo --> Start");

    Exposure* exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    if (exposureIndex == animationProject->getActiveScene()->getActiveTake()->getExposureSize()) {
        this->setActiveExposureIndex(exposureIndex - 1);
    }
    else {
        this->setActiveExposureIndex(exposureIndex);
    }

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::removeExposureRedo --> End");
    return exposure;
}


/**************************************************************************
 * Old Frames functions
 **************************************************************************/

void DomainFacade::addFrames(const QVector<QString>& frameNames)
{
    if (!(animationProject->getActiveSceneIndex() < 0 &&
            animationProject->getSceneSize() > 0)) {

        qDebug("Adding frames in the domainfacade");

        QVector<Exposure*> tmp = animationProject->addFrames(frameNames,
                                 animationProject->getActiveExposureIndex() + 1);

        int numElem = tmp.size();
        // there are no elements in the vector if the user has aborted the operation
        if (numElem > 0) {
        }
    }

    // TODO: Implementation of addFrames undo
    // UndoExposureNew *u = new UndoExposureNew(this, "<empty>", fromIndex, frameNames, getActiveSceneIndex());
    // undoStack->push(u);
}


void DomainFacade::removeFrames(int fromFrame,
                                int toFrame)
{
    qDebug("Removing frames in the domainfacade");
    QVector<Exposure*> trash = animationProject->removeFrames(fromFrame, toFrame);

    int numElem = trash.size();
    // there are no elements in the vector if the user has aborted the operation
    if (numElem > 0) {
        QVector<QString> frameNames;
        for (int i = 0 ; i < numElem ; i++) {
            frameNames.append(trash[i]->getImagePath());
        }
    }

    // TODO: Implementation of removeFrames undo
    // UndoExposureRemove *u = new UndoExposureRemove(frameNames, fromFrame,
    //                               animationProject->getActiveSceneIndex());
    // undoStack->push(u);
}


void DomainFacade::moveFrames(int fromFrame,
                              int toFrame,
                              int movePosition)
{
    animationProject->moveFrames(fromFrame, toFrame, movePosition);

    // TODO: Implementation of moveFrames undo
    // UndoExposureMove *u = new UndoExposureMove(fromFrame, toFrame, movePosition,
    //                            animationProject->getActiveSceneIndex());
    // undoStack->push(u);
}
