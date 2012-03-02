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
#include "domain/undo/undoexposureselect.h"
#include "domain/undo/undoprojectclose.h"
#include "domain/undo/undoprojectnew.h"
#include "domain/undo/undoprojectopen.h"
#include "domain/undo/undoprojectsave.h"
#include "domain/undo/undosceneadd.h"
#include "domain/undo/undosceneinsert.h"
#include "domain/undo/undoscenemove.h"
#include "domain/undo/undosceneremove.h"
#include "domain/undo/undosceneselect.h"
#include "domain/undo/undotakeadd.h"
#include "domain/undo/undotakeinsert.h"
#include "domain/undo/undotakemove.h"
#include "domain/undo/undotakeremove.h"
#include "domain/undo/undotakeselect.h"
#include "frontends/qtfrontend/toolbar.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtDebug>


DomainFacade::DomainFacade(Frontend *f)
{
    frontend = f;
    animationProject = new AnimationProject(frontend);

    historyFilePath.clear();
    historyFilePath.append(frontend->getUserDirName());
    historyFilePath.append("/");
    historyFilePath.append(PreferencesTool::historyFileName);

    historyFile = new QFile(historyFilePath);

    writeHistory = true;
}


DomainFacade::~DomainFacade()
{
    if (animationProject != NULL) {
        clearUndoStack();
        delete animationProject;
        animationProject = NULL;
    }

    writeHistory = false;

    if (historyFile != NULL) {
        delete historyFile;
        historyFile = NULL;
    }

    frontend = NULL;
}


Frontend* DomainFacade::getFrontend()
{
    return this->frontend;
}


ViewFacade* DomainFacade::getView()
{
    return frontend->getView();
}

/**************************************************************************
 * Audio functions
 **************************************************************************/

bool DomainFacade::initAudioDevice()
{
    return animationProject->initAudioDevice();
}


void DomainFacade::shutdownAudioDevice()
{
    return animationProject->shutdownAudioDevice();
}

/**************************************************************************
 * Undo functions
 **************************************************************************/

QUndoStack* DomainFacade::getUndoStack()
{
    return animationProject->getUndoStack();
}


void DomainFacade::clearUndoStack()
{
    animationProject->clearUndoStack();
    if (!historyFile->remove()) {
        // Error
        frontend->showCritical(tr("Critical"),
                               tr("Can't remove history file!"));
    }
}


void DomainFacade::writeHistoryEntry(const QString &entry)
{
    if (!writeHistory) {
        // Write history functionality is off
        return;
    }

    if (!historyFile->open(QIODevice::Append | QIODevice::Text)) {
        // Error
        frontend->showCritical(tr("Critical"),
                               tr("Can't open history file to write entry!"));
        return;
    }

    QTextStream out(historyFile);
    out << entry << "\n";

    // Flush and close the file
    historyFile->close();

    return;
}

/**************************************************************************
 * Animation functions
 **************************************************************************/

AnimationProject* DomainFacade::getAnimationProject()
{
    return this->animationProject;
}


bool DomainFacade::isUnsavedChanges()
{
    return animationProject->isUnsavedChanges();
}


bool DomainFacade::isActiveProject()
{
    return animationProject->isActiveProject();
}


const QString DomainFacade::getNewProjectFilePath()
{
    return animationProject->getNewProjectFilePath();
}


const QString DomainFacade::getNewProjectPath()
{
    return animationProject->getNewProjectPath();
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


int DomainFacade::getMixMode()
{
    return animationProject->getMixMode();
}


void DomainFacade::setMixMode(int newMixMode)
{
    animationProject->setMixMode(newMixMode);
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


bool DomainFacade::exportToVideo(VideoEncoder *encoder)
{
    return animationProject->exportToVideo(encoder);
}


bool DomainFacade::exportToCinelerra(const QString file)
{
    return animationProject->exportToCinelerra(file);
}

/**************************************************************************
 * Project functions
 **************************************************************************/

bool DomainFacade::recoverProject()
{
    qDebug("DomainFacade::recoverProject --> Start");

    bool recovered = false;

    // Open the history file
    if (!historyFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Error
        frontend->showCritical(tr("Critical"),
                               tr("Can't open history file to recover project!"));
        return recovered;
    }

    // Switch the write history functionality off
    writeHistory = false;

    QTextStream in(historyFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList partStringList = line.split('|');
        //
        // Project entries
        //
        if (0 == partStringList.at(0).compare("redoProjectNew")) {
            // Create the new project
            newProjectToUndo(partStringList.at(1));

            recovered = true;

            continue;
        }
        if (0 == partStringList.at(0).compare("redoProjectOpen")) {
            // Open project
            openProjectToUndo(partStringList.at(1));

            recovered = true;

            continue;
        }
        if (0 == partStringList.at(0).compare("redoProjectSave")) {
            // Save project
            openProjectToUndo(partStringList.at(1));

            recovered = true;

            continue;
        }
        if (0 == partStringList.at(0).compare("redoProjectClose")) {
            // Close project --> Nothing to do
            Q_ASSERT(1);

            continue;
        }
        //
        // Exposure entries
        //
        if (0 == partStringList.at(0).compare("redoExposureAdd")) {
            // Add a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();

            addExposureToUndo(partStringList.at(4), sceneIndex, takeIndex, false);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoExposureInsert")) {
            // Insert a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();
            int exposureIndex = partStringList.at(3).toInt();

            insertExposureToUndo(partStringList.at(4), sceneIndex, takeIndex, exposureIndex, false);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoExposureMove")) {
            // Move a exposure --> Not implemented
            Q_ASSERT(1);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoExposureRemove")) {
            // Remove a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();
            int exposureIndex = partStringList.at(3).toInt();

            removeExposureToUndo(sceneIndex, takeIndex, exposureIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoExposureSelect")) {
            // Select a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();
            int exposureIndex = partStringList.at(3).toInt();

            selectExposureToUndo(sceneIndex, takeIndex, exposureIndex);

            continue;
        }
        //
        // Take entries
        //
        if (0 == partStringList.at(0).compare("redoTakeAdd")) {
            // Add a take
            int sceneIndex = partStringList.at(1).toInt();

            addTakeToUndo(partStringList.at(3), sceneIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoTakeInsert")) {
            // Insert a take
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();

            insertTakeToUndo(partStringList.at(3), sceneIndex, takeIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoTakeMove")) {
            // Move a take --> Not implemented
            Q_ASSERT(1);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoTakeRemove")) {
            // Remove a take
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();

            removeTakeToUndo(sceneIndex, takeIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoTakeSelect")) {
            // Select a take
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();

            selectTakeToUndo(sceneIndex, takeIndex);

            continue;
        }
        //
        // Scene entries
        //
        if (0 == partStringList.at(0).compare("redoSceneAdd")) {
            // Add a scene
            addSceneToUndo(partStringList.at(2));

            continue;
        }
        if (0 == partStringList.at(0).compare("redoSceneInsert")) {
            // Insert a scene
            int sceneIndex = partStringList.at(1).toInt();

            insertSceneToUndo(partStringList.at(2), sceneIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoSceneMove")) {
            // Move a scene --> Not implemented
            Q_ASSERT(1);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoSceneRemove")) {
            // Remove a scene
            int sceneIndex = partStringList.at(1).toInt();

            removeSceneToUndo(sceneIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoSceneSelect")) {
            // Select a scene
            int sceneIndex = partStringList.at(1).toInt();

            selectSceneToUndo(sceneIndex);

            continue;
        }
    }

    historyFile->close();

    // Switch the write history functionality on
    writeHistory = true;

    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    qDebug("DomainFacade::recoverProject --> End");
    return recovered;
}


void DomainFacade::newProjectToUndo(const QString &projectDescription)
{
    UndoProjectNew *u = new UndoProjectNew(this, projectDescription);
    getUndoStack()->push(u);
}

/*
bool DomainFacade::newProjectUndo(const QString &projectDescription)
{
    qDebug("DomainFacade::newProjectUndo --> Start");

    qDebug("DomainFacade::newProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::newProjectRedo(const QString &projectDescription)
{
    qDebug("DomainFacade::newProjectRedo --> Start");

    getView()->notifyClear();
    animationProject->newProject(projectDescription);
    setProjectSettingsToDefault();
    getView()->notifyNewProject();
    frontend->setProjectID(getProjectID().toAscii());
    frontend->setSceneID("");
    frontend->setTakeID("");
    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::newProjectRedo --> End");
    return true;
}

void DomainFacade::openProjectToUndo(const QString &projectPath)
{
    UndoProjectOpen *u = new UndoProjectOpen(this, projectPath);
    getUndoStack()->push(u);
}

/*
bool DomainFacade::openProjectUndo(const QString &projectDescription)
{
    qDebug("DomainFacade::openProjectUndo --> Start");

    qDebug("DomainFacade::openProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::openProjectRedo(const QString &projectPath)
{
    qDebug("DomainFacade::openProjectRedo --> Start");

    getView()->notifyClear();
    getView()->notifyNewProject();

    if (animationProject->openProject(projectPath)) {
        getView()->notifyDescriptionsUpdated();
        getView()->notifyActivateScene();
        getView()->notifyActivateTake();
        getView()->notifyActivateExposure();
        frontend->setProjectID(getProjectID().toAscii());

        Scene *scene = getActiveScene();
        Q_ASSERT(NULL != scene);
        frontend->setSceneID(scene->getId().toAscii());

        Take *take = getActiveTake();
        Q_ASSERT(NULL != take);
        frontend->setTakeID(take->getId().toAscii());

        Exposure *exposure = getActiveExposure();
        if (NULL != exposure) {
            frontend->setExposureID(exposure->getId().toAscii());
        }
    }

    qDebug("DomainFacade::openProjectRedo --> End");
    return true;
}


void DomainFacade::saveProjectToUndo(const QString &projectPath, bool saveAs)
{
    UndoProjectSave *u = new UndoProjectSave(this, projectPath, saveAs);
    getUndoStack()->push(u);
}

/*
bool DomainFacade::saveProjectUndo(const QString &projectDescription, bool saveAs)
{
    qDebug("DomainFacade::saveProjectUndo --> Start");

    qDebug("DomainFacade::saveProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::saveProjectRedo(const QString &projectPath, bool saveAs)
{
    qDebug("DomainFacade::saveProjectRedo --> Start");

    bool ret;

    ret = animationProject->saveProject(projectPath, saveAs);
    if (ret) {
        clearUndoStack();
    }

    qDebug("DomainFacade::saveProjectRedo --> End");
    return ret;
}


void DomainFacade::closeProjectToUndo()
{
    UndoProjectClose *u = new UndoProjectClose(this);
    getUndoStack()->push(u);
}

/*
bool DomainFacade::closeProjectUndo()
{
    qDebug("DomainFacade::closeProjectUndo --> Start");

    qDebug("DomainFacade::closeProjectUndo --> End");
    return true;
}
*/

bool DomainFacade::closeProjectRedo()
{
    qDebug("DomainFacade::closeProjectRedo --> Start");

    getView()->notifyClear();
    animationProject->clearProject();
    clearUndoStack();

    // neccessary??
    frontend->removeApplicationFiles();

    qDebug("DomainFacade::closeProjectRedo --> End");
    return true;
}

/**************************************************************************
 * Scene functions
 **************************************************************************/

int DomainFacade::getActiveSceneIndex()
{
    return animationProject->getActiveSceneIndex();
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


void DomainFacade::selectSceneToUndo(int newSceneIndex)
{
    UndoSceneSelect *u = new UndoSceneSelect(this, newSceneIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoSceneSelect(int oldSceneIndex,
                                   int newSceneIndex)
{
}
*/

void DomainFacade::redoSceneSelect(int oldSceneIndex,
                                   int newSceneIndex)
{
    qDebug("DomainFacade::redoSceneSelect --> Start");

    animationProject->setActiveSceneIndex(newSceneIndex);
    if (0 <= newSceneIndex) {
        Scene *activeScene = animationProject->getActiveScene();
        frontend->setSceneID(activeScene->getDescription().toAscii());
    }
    else {
        frontend->setSceneID("---");
    }
    getView()->notifyActivateScene();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoSceneSelect --> Start");
}


/**************************************************************************
 * Scene sound functions
 **************************************************************************/

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


void DomainFacade::selectTakeToUndo(int newSceneIndex,
                                    int newTakeIndex)
{
    UndoTakeSelect *u = new UndoTakeSelect(this, newSceneIndex, newTakeIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoTakeSelect(int oldSceneIndex,
                                  int oldTakeIndex,
                                  int newSceneIndex,
                                  int newTakeIndex)
{
}
*/

void DomainFacade::redoTakeSelect(int oldSceneIndex,
                                  int oldTakeIndex,
                                  int newSceneIndex,
                                  int newTakeIndex)
{
    qDebug("DomainFacade::redoTakeSelect --> Start");

    animationProject->setActiveTakeIndex(newTakeIndex);
    if (0 <= newTakeIndex) {
        Take *activeTake = animationProject->getActiveTake();
        frontend->setTakeID(activeTake->getDescription().toAscii());
    }
    else {
        frontend->setTakeID("---");
    }
    getView()->notifyActivateTake();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoTakeSelect --> End");
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int DomainFacade::getActiveExposureIndex()
{
    int activeExposureIndex = animationProject->getActiveExposureIndex();

    return activeExposureIndex;
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


void DomainFacade::addExposureToUndo(const QString &filePath,
                                     int            sceneIndex,
                                     int            takeIndex,
                                     bool           copy)
{
    QString newFileName;
    if (copy) {
        newFileName = copyToTemp(filePath);
    }
    else {
        newFileName = filePath;
        Exposure::tempNum++;
    }
    UndoExposureAdd *u = new UndoExposureAdd(this, newFileName, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoExposureAdd(const QString &fileName,
                                   int            sceneIndex,
                                   int            takeIndex)
{
}
*/

void DomainFacade::redoExposureAdd(const QString &fileName,
                                   int            sceneIndex,
                                   int            takeIndex)
{
    qDebug("DomainFacade::redoExposureAdd --> Start");

    Exposure *exposure = animationProject->addExposure(fileName, AnimationProject::InTempPath);
    getView()->notifyAddExposure(sceneIndex, takeIndex, exposure->getIndex());
    animationProject->setActiveExposureIndex(exposure->getIndex());
    getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoExposureAdd --> End");
}


void DomainFacade::insertExposureToUndo(const QString &filePath,
                                        int            sceneIndex,
                                        int            takeIndex,
                                        int            exposureIndex,
                                        bool           copy)
{
    QString newFileName;

    if (copy) {
        newFileName = copyToTemp(filePath);
    }
    else {
        newFileName = filePath;
        Exposure::tempNum++;
    }
    UndoExposureInsert *u = new UndoExposureInsert(this, newFileName,
                                                   sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoExposureInsert(const QString &fileName,
                                      int            sceneIndex,
                                      int            takeIndex,
                                      int            exposureIndex)
{
}
*/

void DomainFacade::redoExposureInsert(const QString &fileName,
                                      int            sceneIndex,
                                      int            takeIndex,
                                      int            exposureIndex)
{
    qDebug("DomainFacade::redoExposureInsert --> Start");

    Exposure *exposure = NULL;

    exposure = animationProject->insertExposure(sceneIndex, takeIndex, exposureIndex,
                                                fileName, AnimationProject::InTempPath);
    getView()->notifyInsertExposure(sceneIndex, takeIndex, exposure->getIndex());

    animationProject->setActiveExposureIndex(exposure->getIndex());
    getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoExposureInsert --> End");
}


void DomainFacade::removeExposureToUndo(int sceneIndex,
                                        int takeIndex,
                                        int exposureIndex)
{
    UndoExposureRemove *u = new UndoExposureRemove(this, sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoExposureRemove(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
}
*/

Exposure *DomainFacade::redoExposureRemove(int sceneIndex,
                                           int takeIndex,
                                           int exposureIndex)
{
    qDebug("DomainFacade::redoExposureRemove --> Start");

    Exposure* exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    if (exposureIndex == animationProject->getActiveScene()->getActiveTake()->getExposureSize()) {
        // this->setActiveExposureIndex(exposureIndex - 1);
        animationProject->setActiveExposureIndex(exposureIndex - 1);
        if (0 <= (exposureIndex - 1)) {
            Exposure *activeExposure = animationProject->getActiveExposure();
            frontend->setExposureID(activeExposure->getId().toAscii());
        }
        else {
            frontend->setExposureID("---");
        }
    }
    else {
        animationProject->setActiveExposureIndex(exposureIndex);
        if (0 <= exposureIndex) {
            Exposure *activeExposure = animationProject->getActiveExposure();
            frontend->setExposureID(activeExposure->getId().toAscii());
        }
        else {
            frontend->setExposureID("---");
        }
    }

    getView()->notifyActivateExposure();
    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoExposureRemove --> End");
    return exposure;
}


void DomainFacade::moveExposureToUndo(int fromSceneIndex,
                                      int fromTakeIndex,
                                      int fromExposureIndex,
                                      int toSceneIndex,
                                      int toTakeIndex,
                                      int toExposureIndex)
{
    UndoExposureMove *u = new UndoExposureMove(this, fromSceneIndex, fromTakeIndex, fromExposureIndex,
                                               toSceneIndex, toTakeIndex, toExposureIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoExposureMove(int fromSceneIndex,
                                    int fromTakeIndex,
                                    int fromExposureIndex,
                                    int toSceneIndex,
                                    int toTakeIndex,
                                    int toExposureIndex)
{
}
*/

Exposure *DomainFacade::redoExposureMove(int fromSceneIndex,
                                         int fromTakeIndex,
                                         int fromExposureIndex,
                                         int toSceneIndex,
                                         int toTakeIndex,
                                         int toExposureIndex)
{
    /* TODO: Implementation of the redoExposureMove function

    qDebug("DomainFacade::redoExposureMove --> Start");

    Exposure* exposure = animationProject->removeExposure(sceneIndex, takeIndex, exposureIndex);
    getView()->notifyRemoveExposure(sceneIndex, takeIndex, exposureIndex);

    if (exposureIndex == animationProject->getActiveScene()->getActiveTake()->getExposureSize()) {
        this->setActiveExposureIndex(exposureIndex - 1);
    }
    else {
        this->setActiveExposureIndex(exposureIndex);
    }

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoExposureMove --> End");
    return exposure;
    */
    return NULL;
}


void DomainFacade::selectExposureToUndo(int newSceneIndex,
                                        int newTakeIndex,
                                        int newExposureIndex)
{
    UndoExposureSelect *u = new UndoExposureSelect(this, newSceneIndex, newTakeIndex, newExposureIndex);
    getUndoStack()->push(u);
}

/*
void DomainFacade::undoExposureSelect(int oldSceneIndex,
                                      int oldTakeIndex,
                                      int oldExposureIndex,
                                      int newSceneIndex,
                                      int newTakeIndex,
                                      int newExposureIndex)
{
}
*/

void DomainFacade::redoExposureSelect(int oldSceneIndex,
                                      int oldTakeIndex,
                                      int oldExposureIndex,
                                      int newSceneIndex,
                                      int newTakeIndex,
                                      int newExposureIndex)
{
    qDebug("DomainFacade::redoExposureSelect --> Start");

    animationProject->setActiveExposureIndex(newExposureIndex);
    if (0 <= newExposureIndex) {
        Exposure *activeExposure = animationProject->getActiveExposure();
        frontend->setExposureID(activeExposure->getId().toAscii());
    }
    else {
        frontend->setExposureID("---");
    }
    getView()->notifyActivateExposure();

    animationProject->setUnsavedChanges();

    qDebug("DomainFacade::redoExposureSelect --> End");
}


/**************************************************************************
 * Old Frames functions
 **************************************************************************/

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

/**************************************************************************
 * Private functions
 **************************************************************************/

const QString DomainFacade::copyToTemp(const QString &fromImagePath)
{
    // creates a new image name
    QString toImageName(QString("tmp_%1%2").arg(Exposure::tempNum)
                        .arg(fromImagePath.mid(fromImagePath.lastIndexOf('.'))));

    // creates a new image path
    QString toImagePath;
    toImagePath.append(frontend->getTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    // Copy file to temp directory
    if (!QFile::copy(fromImagePath, toImagePath)) {
        // Not successful
        frontend->showCritical(tr("Critical"),
                               tr("Can't copy image to temp directory!"));
    }

    Exposure::tempNum++;

    return toImageName;
}


void DomainFacade::setProjectSettingsToDefault()
{
    PreferencesTool *pref = frontend->getPreferences();

    setVideoSource(pref->getBasicPreference("defaultsource", 0));
    setMixMode(pref->getBasicPreference("defaultmixingmode", 0));
    setUnitMode(pref->getBasicPreference("defaultunitmode", 0));
    setMixCount(pref->getBasicPreference("defaultmixcount", 0));
    setPlaybackCount(pref->getBasicPreference("defaultplaybackcount", 0));
    setFramesPerSecond(pref->getBasicPreference("defaultframespersecond", 0));
}
