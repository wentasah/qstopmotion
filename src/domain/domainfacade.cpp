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

#include "domainfacade.h"

#include "domain/animation/animationproject.h"
#include "domain/undo/undoexposureadd.h"
#include "domain/undo/undoexposureinsert.h"
#include "domain/undo/undoexposuremove.h"
#include "domain/undo/undoexposureremove.h"
#include "domain/undo/undoexposureselect.h"
#include "domain/undo/undoprojectnew.h"
#include "domain/undo/undoprojectopen.h"
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
#include "technical/grabber/imagegrabber.h"
#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtDebug>


DomainFacade::DomainFacade(Frontend *f)
{
    frontend = f;
    animationProject = NULL;

    undoStack = new QUndoStack();

    historyFilePath.clear();
    historyFilePath.append(frontend->getUserDirName());
    historyFilePath.append("/");
    historyFilePath.append(PreferencesTool::historyFileName);

    historyFile = new QFile(historyFilePath);

    recovering = false;
}


DomainFacade::~DomainFacade()
{
    if (animationProject != NULL) {
        clearUndoStack();
        delete animationProject;
        animationProject = NULL;
    }

    recovering = false;

    if (undoStack != NULL) {
        delete undoStack;
        undoStack = NULL;
    }

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


bool DomainFacade::isRecovering()
{
    return recovering;
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
    return undoStack;
}


void DomainFacade::clearUndoStack()
{
    undoStack->clear();
}


void DomainFacade::writeHistoryEntry(const QString &entry)
{
    if (recovering) {
        // The project is in revovering mode --> no writing of history information
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


void DomainFacade::removeHistoryFile()
{
    if (!historyFile->remove()) {
        // Error
        // frontend->showCritical(tr("Critical"),
        //                        tr("Can't remove history file!"));
    }
}

/**************************************************************************
 * Animation functions
 **************************************************************************/

AnimationProject* DomainFacade::getAnimationProject()
{
    return this->animationProject;
}


bool DomainFacade::isProjectSettingsChanges()
{
    if (NULL == animationProject) {
        return FALSE;
    }
    return animationProject->isSettingsChanges();
}


bool DomainFacade::isProjectAnimationChanges()
{
    if (NULL == animationProject) {
        return FALSE;
    }
    return animationProject->isAnimationChanges();
}


bool DomainFacade::isActiveProject()
{
    if (NULL == animationProject) {
        return FALSE;
    }
    return TRUE;
}


const QString DomainFacade::getNewProjectFilePath()
{
    return animationProject->getNewProjectFilePath();
}


const QString DomainFacade::getNewProjectPath()
{
    return animationProject->getNewProjectPath();
}


const QString DomainFacade::getProjectDescription()
{
    return animationProject->getDescription();
}


/**************************************************************************
 * Project preferences
 **************************************************************************/

int DomainFacade::getRecordingMode()
{
    return animationProject->getRecordingMode();
}


void DomainFacade::setRecordingMode(int newRecordingMode)
{
    animationProject->setRecordingMode(newRecordingMode);
    frontend->setRecordingMode(newRecordingMode);
}

int DomainFacade::getVideoSource()
{
    return animationProject->getVideoSource();
}


void DomainFacade::setVideoSource(int newVideoSource)
{
    animationProject->setVideoSource(newVideoSource);
    frontend->setVideoSource(newVideoSource);
}


int DomainFacade::getResolution()
{
    return animationProject->getResolution();
}


void DomainFacade::setResolution(int newResolution)
{
    animationProject->setResolution(newResolution);
    frontend->setResolution(newResolution);
}


bool DomainFacade::convertImages(int newFormat, int newQuality)
{
    return animationProject->convertImages(newFormat, newQuality);
}


int DomainFacade::getMixMode()
{
    return animationProject->getMixMode();
}


void DomainFacade::setMixMode(int newMixMode)
{
    animationProject->setMixMode(newMixMode);
    frontend->setMixMode(newMixMode);
}


int DomainFacade::getMixCount()
{
    return animationProject->getMixCount();
}


void DomainFacade::setMixCount(int newMixCount)
{
    animationProject->setMixCount(newMixCount);
    frontend->setMixCount(newMixCount);
}


int DomainFacade::getPlaybackCount()
{
    return animationProject->getPlaybackCount();
}


void DomainFacade::setPlaybackCount(int newPlaybackCount)
{
    animationProject->setPlaybackCount(newPlaybackCount);
}


int DomainFacade::getUnitMode()
{
    return animationProject->getUnitMode();
}


void DomainFacade::setUnitMode(int newUnitMode)
{
    animationProject->setUnitMode(newUnitMode);
}

/**************************************************************************
 * Image import preferences
 **************************************************************************/

int DomainFacade::getImageFormat()
{
    return animationProject->getImageFormat();
}


void DomainFacade::setImageFormat(int newIF)
{
    animationProject->setImageFormat(newIF);
}


int DomainFacade::getImageQuality()
{
    return animationProject->getImageQuality();
}


void DomainFacade::setImageQuality(int newValue)
{
    animationProject->setImageQuality(newValue);
}


int DomainFacade::getImageSize()
{
    return animationProject->getImageSize();
}


void DomainFacade::setImageSize(int newIS)
{
    animationProject->setImageSize(newIS);
}


int DomainFacade::getImageTransformation()
{
    if (animationProject == NULL) {
        // If no project is active use scale
        return 0;
    }
    return animationProject->getImageTransformation();
}


void DomainFacade::setImageTransformation(int newTransform)
{
    animationProject->setImageTransformation(newTransform);
}


int DomainFacade::getImageAdjustment()
{
    return animationProject->getImageAdjustment();
}


void DomainFacade::setImageAdjustment(int newIA)
{
    animationProject->setImageAdjustment(newIA);
}


int DomainFacade::getZoomValue()
{
    return animationProject->getZoomValue();
}


void DomainFacade::setZoomValue(int newZV)
{
    animationProject->setZoomValue(newZV);
}


int DomainFacade::getLiveViewFps()
{
    return animationProject->getLiveViewFps();
}


void DomainFacade::setLiveViewFps(int newValue)
{
    animationProject->setLiveViewFps(newValue);
    getView()->notifyNewLiveViewFps(newValue);
}

/**************************************************************************
 * Video export preferences
 **************************************************************************/

int DomainFacade::getEncoderApplication()
{
    return animationProject->getEncoderApplication();
}


void DomainFacade::setEncoderApplication(int newEA)
{
    animationProject->setEncoderApplication(newEA);
}


int DomainFacade::getVideoFormat()
{
    return animationProject->getVideoFormat();
}


void DomainFacade::setVideoFormat(int newVF)
{
    animationProject->setVideoFormat(newVF);
}


int DomainFacade::getVideoSize()
{
    return animationProject->getVideoSize();
}


void DomainFacade::setVideoSize(int newVS)
{
    animationProject->setVideoSize(newVS);
}


int DomainFacade::getVideoFps()
{
    return animationProject->getVideoFps();
}


void DomainFacade::setVideoFps(int newFps)
{
    animationProject->setVideoFps(newFps);
    getView()->notifyNewVideoFps(newFps);
}


bool DomainFacade::getUseDefaultOutputFile()
{
    return animationProject->getUseDefaultOutputFile();
}


void DomainFacade::setUseDefaultOutputFile(bool newUDOF)
{
    animationProject->setUseDefaultOutputFile(newUDOF);
}


const QString DomainFacade::getDefaultOutputFileName()
{
    return animationProject->getDefaultOutputFileName();
}


void DomainFacade::setDefaultOutputFileName(const QString newDOFN)
{
    animationProject->setDefaultOutputFileName(newDOFN);
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
    recovering = true;

    QTextStream in(historyFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList partStringList = line.split('|');
        //
        // Exposure entries
        //
        if (0 == partStringList.at(0).compare("redoExposureAdd")) {
            // Add a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();

            addExposureToUndo(partStringList.at(4), sceneIndex, takeIndex);

            continue;
        }
        if (0 == partStringList.at(0).compare("redoExposureInsert")) {
            // Insert a exposure
            int sceneIndex = partStringList.at(1).toInt();
            int takeIndex = partStringList.at(2).toInt();
            int exposureIndex = partStringList.at(3).toInt();

            insertExposureToUndo(partStringList.at(4), sceneIndex, takeIndex, exposureIndex);

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
            // int oldSceneIndex = partStringList.at(1).toInt();
            // int oldTakeIndex = partStringList.at(2).toInt();
            // int oldExposureIndex = partStringList.at(3).toInt();
            int newSceneIndex = partStringList.at(4).toInt();
            int newTakeIndex = partStringList.at(5).toInt();
            int newExposureIndex = partStringList.at(6).toInt();

            selectExposureToUndo(newSceneIndex, newTakeIndex, newExposureIndex);

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
            // int oldSceneIndex = partStringList.at(1).toInt();
            // int oldTakeIndex = partStringList.at(2).toInt();
            int newSceneIndex = partStringList.at(3).toInt();
            int newTakeIndex = partStringList.at(4).toInt();

            selectTakeToUndo(newSceneIndex, newTakeIndex);

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
            // int oldSceneIndex = partStringList.at(1).toInt();
            int newSceneIndex = partStringList.at(2).toInt();

            selectSceneToUndo(newSceneIndex);

            continue;
        }
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
        //
        // Undo/redo entries
        //
        if (0 == partStringList.at(0).compare("undo")) {
            // undo the last redo command
            undoStack->undo();

            recovered = true;

            continue;
        }
        if (0 == partStringList.at(0).compare("redo")) {
            // redo the last undo command
            undoStack->redo();

            recovered = true;

            continue;
        }
    }

    historyFile->close();

    // Switch the write history functionality on
    recovering = false;

    frontend->setToolBarState(ToolBar::toolBarCameraOff);

    qDebug("DomainFacade::recoverProject --> End");
    return recovered;
}


void DomainFacade::setProjectSettingsToDefault()
{
    PreferencesTool *pref = frontend->getPreferences();
    int              value;

    // Project preferences
    if (pref->getIntegerPreference("preferences", "defaultrecordingmode", value) == false) {
        value = 0;
    }
    setRecordingMode(value);

    if (pref->getIntegerPreference("preferences", "defaultvideosource", value) == false) {
        value = 0;
    }
    setVideoSource(value);

    if (pref->getIntegerPreference("preferences", "defaultmixingmode", value) == false) {
        value = 0;
    }
    setMixMode(value);

    if (pref->getIntegerPreference("preferences", "defaultmixcount", value) == false) {
        value = 0;
    }
    setMixCount(value);

    if (pref->getIntegerPreference("preferences", "defaultplaybackcount", value) == false) {
        value = 0;
    }
    setPlaybackCount(value);

    if (pref->getIntegerPreference("preferences", "defaultunitmode", value) == false) {
        value = 0;
    }
    setUnitMode(value);

    // Image import preferences
    if (pref->getIntegerPreference("preferences", "defaultimageformat", value) == false) {
        value = ImageGrabber::jpegFormat;
    }
    setImageFormat(value);

    if (pref->getIntegerPreference("preferences", "defaultimagequality", value) == false) {
        value = 100;
    }
    setImageQuality(value);

    if (pref->getIntegerPreference("preferences", "defaultimagesize", value) == false) {
        value = ImageGrabber::defaultSize;
    }
    setImageSize(value);

    if (pref->getIntegerPreference("preferences", "defaulttransformation", value) == false) {
        value = 0;
    }
    setImageTransformation(value);

    if (pref->getIntegerPreference("preferences", "defaultimageadjustment", value) == false) {
        value = ImageGrabber::centerDown;
    }
    setImageAdjustment(value);

    if (pref->getIntegerPreference("preferences", "defaultliveviewfps", value) == false) {
        value = 20;
    }
    setLiveViewFps(value);

    // Video export preferences
    if (pref->getIntegerPreference("preferences", "defaultencoderapplication", value) == false) {
        value = VideoEncoder::noneApplication;
    }
    setEncoderApplication(value);

    if (pref->getIntegerPreference("preferences", "defaultvideoformat", value) == false) {
        value = VideoEncoder::noneFormat;
    }
    setVideoFormat(value);

    if (pref->getIntegerPreference("preferences", "defaultvideosize", value) == false) {
        value = VideoEncoder::defaultSize;
    }
    setVideoSize(value);

    if (pref->getIntegerPreference("preferences", "defaultvideofps", value) == false) {
        value = 12;
    }
    setVideoFps(value);

    if (pref->getIntegerPreference("preferences", "defaultusedefaultoutputfile", value) == false) {
        value = false;
    }
    setUseDefaultOutputFile(value);
}


void DomainFacade::newProjectToUndo(const QString &projectDescription)
{
    UndoProjectNew *u = new UndoProjectNew(this, projectDescription);
    getUndoStack()->push(u);
}


void DomainFacade::newProject(const QString &projectDescription)
{
    Q_ASSERT(NULL == animationProject);

    animationProject = new AnimationProject(frontend);

    animationProject->newProject(projectDescription);
}


void DomainFacade::newProject(AnimationProject *project)
{
    Q_ASSERT(NULL == animationProject);

    animationProject = project;
}


void DomainFacade::openProjectToUndo(const QString &projectPath)
{
    UndoProjectOpen *u = new UndoProjectOpen(this, projectPath);
    getUndoStack()->push(u);
}


bool DomainFacade::openProject(const QString &projectPath)
{
    Q_ASSERT(NULL == animationProject);

    animationProject = new AnimationProject(frontend);

    return animationProject->openProject(projectPath);
}


void DomainFacade::openProject(AnimationProject *project)
{
    Q_ASSERT(NULL == animationProject);

    animationProject = project;
}


void DomainFacade::saveProject(const QString &projectPath, bool saveAs)
{
    qDebug("DomainFacade::saveProject --> Start");

    Q_ASSERT(NULL != animationProject);

    bool ret;

    ret = animationProject->saveProject(projectPath, saveAs);
    if (ret) {
        clearUndoStack();
        removeHistoryFile();
        writeHistoryEntry(QString("%1%2")
                          .arg(QLatin1String("redoProjectOpen|"))
                          .arg(projectPath));
    }

    qDebug("DomainFacade::saveProject --> End");
}


void DomainFacade::closeProject()
{
    qDebug("DomainFacade::closeProject --> Start");

    Q_ASSERT(NULL != animationProject);

    delete animationProject;
    animationProject = NULL;

    clearUndoStack();
    removeHistoryFile();

    frontend->removeApplicationFiles();

    getView()->notifyRemoveProject();

    qDebug("DomainFacade::closeProject --> End");
}


AnimationProject *DomainFacade::removeProject()
{
    Q_ASSERT(NULL != animationProject);

    AnimationProject *oldProject = animationProject;

    animationProject = NULL;

    return oldProject;
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


int DomainFacade::getSceneTakeSize(int sceneIndex)
{
    return animationProject->getSceneTakeSize(sceneIndex);
}


int DomainFacade::getSceneExposureSize(int sceneIndex)
{
    return animationProject->getSceneExposureSize(sceneIndex);
}


int DomainFacade::getSceneTakeExposureSize(int sceneIndex, int takeIndex)
{
    return animationProject->getSceneTakeExposureSize(sceneIndex, takeIndex);
}


void DomainFacade::addSceneToUndo(const QString &sceneDescription)
{
    UndoSceneAdd *u = new UndoSceneAdd(this, sceneDescription);
    getUndoStack()->push(u);
}


void DomainFacade::insertSceneToUndo(const QString &sceneDescription,
                                     int sceneIndex)
{
    UndoSceneInsert *u = new UndoSceneInsert(this, sceneDescription, sceneIndex);
    getUndoStack()->push(u);

}


void DomainFacade::removeSceneToUndo(int sceneIndex)
{
    UndoSceneRemove *u = new UndoSceneRemove(this, sceneIndex);
    getUndoStack()->push(u);
}


void DomainFacade::moveSceneToUndo(int fromSceneIndex,
                                   int toSceneIndex)
{
    UndoSceneMove *u = new UndoSceneMove(this, fromSceneIndex, toSceneIndex);
    getUndoStack()->push(u);
}


void DomainFacade::selectSceneToUndo(int newSceneIndex)
{
    UndoSceneSelect *u = new UndoSceneSelect(this, newSceneIndex);
    getUndoStack()->push(u);
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


void DomainFacade::insertTakeToUndo(const QString &takeDescription,
                                    int            sceneIndex,
                                    int            takeIndex)
{
    UndoTakeInsert *u = new UndoTakeInsert(this, takeDescription, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}


void DomainFacade::removeTakeToUndo(int sceneIndex,
                                    int takeIndex)
{
    UndoTakeRemove *u = new UndoTakeRemove(this, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}


void DomainFacade::moveTakeToUndo(int fromSceneIndex,
                                  int fromTakeIndex,
                                  int toSceneIndex,
                                  int toTakeIndex)
{
    UndoTakeMove *u = new UndoTakeMove(this, fromSceneIndex, fromTakeIndex, toSceneIndex, toTakeIndex);
    getUndoStack()->push(u);
}


void DomainFacade::selectTakeToUndo(int newSceneIndex,
                                    int newTakeIndex)
{
    UndoTakeSelect *u = new UndoTakeSelect(this, newSceneIndex, newTakeIndex);
    getUndoStack()->push(u);
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
                                     int            takeIndex)
{
    Exposure::tempNumber++;

    UndoExposureAdd *u = new UndoExposureAdd(this, filePath, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}


void DomainFacade::addExposureToUndo(const QImage &rawImage,
                                     int           sceneIndex,
                                     int           takeIndex)
{
    QString newFileName;
    newFileName = copyToTemp(rawImage);

    UndoExposureAdd *u = new UndoExposureAdd(this, newFileName, sceneIndex, takeIndex);
    getUndoStack()->push(u);
}


void DomainFacade::insertExposureToUndo(const QString &filePath,
                                        int            sceneIndex,
                                        int            takeIndex,
                                        int            exposureIndex)
{
    Exposure::tempNumber++;

    UndoExposureInsert *u = new UndoExposureInsert(this, filePath,
                                                   sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
}


void DomainFacade::insertExposureToUndo(const QImage &rawImage,
                                        int           sceneIndex,
                                        int           takeIndex,
                                        int           exposureIndex)
{
    QString newFileName;

    newFileName = copyToTemp(rawImage);

    UndoExposureInsert *u = new UndoExposureInsert(this, newFileName,
                                                   sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
}


void DomainFacade::removeExposureToUndo(int  sceneIndex,
                                        int  takeIndex,
                                        int  exposureIndex)
{
    UndoExposureRemove *u = new UndoExposureRemove(this,
                                                   sceneIndex, takeIndex, exposureIndex);
    getUndoStack()->push(u);
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


void DomainFacade::selectExposureToUndo(int newSceneIndex,
                                        int newTakeIndex,
                                        int newExposureIndex)
{
    UndoExposureSelect *u = new UndoExposureSelect(this, newSceneIndex, newTakeIndex, newExposureIndex);
    getUndoStack()->push(u);
}


bool DomainFacade::getModifyedExposure(const QString &filePath,
                                       int &modSceneIndex,
                                       int &modTakeIndex,
                                       int &modExposureIndex)
{
    return animationProject->getModifyedExposure(filePath, modSceneIndex, modTakeIndex, modExposureIndex);
}

/**************************************************************************
 * Private functions
 **************************************************************************/

const QString DomainFacade::copyToTemp(const QImage &rawImage)
{
    bool ret;

    qDebug("DomainFacade::copyToTemp --> Start");

    // creates a new image name
    QString toImageName(QString("%1%2%3")
                        .arg(QLatin1String("tmp_"))
                        .arg(Exposure::tempNumber)
                        .arg(QLatin1String(".")));
    switch (animationProject->getImageFormat()) {
    case ImageGrabber::jpegFormat:
        toImageName.append(PreferencesTool::jpegSuffix);
        break;
    case ImageGrabber::tiffFormat:
        toImageName.append(PreferencesTool::tiffSuffix);
        break;
    case ImageGrabber::bmpFormat:
        toImageName.append(PreferencesTool::bmpSuffix);
        break;
    }

    // creates a new image path
    QString toImagePath;
    toImagePath.append(frontend->getTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    // Copy file to temp directory
    switch (animationProject->getImageFormat()) {
    case ImageGrabber::jpegFormat:
        ret = rawImage.save(toImagePath, "JPEG", 100);
        break;
    case ImageGrabber::tiffFormat:
        ret = rawImage.save(toImagePath, "TIFF", 100);
        break;
    case ImageGrabber::bmpFormat:
        ret = rawImage.save(toImagePath, "BMP", 100);
        break;
    }
    if (!ret) {
        // Not successful
        frontend->showCritical(tr("Critical"),
                               tr("Can't copy image to temp directory!"));
    }

    Exposure::tempNumber++;

    qDebug("DomainFacade::copyToTemp --> End");

    return toImageName;
}
