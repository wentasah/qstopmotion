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

#include "animationproject.h"

#include "domain/domainfacade.h"
// #include "technical/audio/ossdriver.h"
#include "technical/videoencoder/videoencoderfactory.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtDebug>


AnimationProject::AnimationProject(Frontend* f)
{
    qDebug("AnimationProject::Constructor --> Start");

    frontend          = f;
    serializer        = new ProjectSerializer(f);

    audioDriver       = NULL; //new OSSDriver("/dev/dsp");

    activeSceneIndex  = -1;
    nextSceneIndex    = 0;
    numSounds         = -1;
    nextTotalExposureIndex = 0;

    videoSource       = 0;
    mixMode           = 0;
    mixCount          = 0;
    playbackCount     = 0;
    framesPerSecond   = 0;

    unitMode          = 0;

    unsavedChanges    = false;
    activeProject     = false;

    isAudioDriverInitialized = false;

    undoStack = new QUndoStack();

    qDebug("AnimationProject::Constructor --> End");
}


AnimationProject::~AnimationProject()
{
    qDebug("AnimationProject::Destructor --> Start");

    unsigned int sceneSize = scenes.size();
    for (unsigned int sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        delete scenes[sceneIndex];
        scenes[sceneIndex] = NULL;
    }
    scenes.clear();

    if (serializer != NULL) {
        delete serializer;
    }

    if (audioDriver != NULL) {
        delete audioDriver;
        audioDriver = NULL;
    }

    if (undoStack != NULL) {
        delete undoStack;
        undoStack = NULL;
    }

    frontend = NULL;

    qDebug("AnimationProject::Destructor --> End");
}


Frontend* AnimationProject::getFrontend()
{
    Q_ASSERT(frontend != NULL);

    return frontend;
}


QUndoStack* AnimationProject::getUndoStack()
{
    return undoStack;
}


ProjectSerializer* AnimationProject::getProjectSerializer()
{
    return serializer;
}

// TODO: Implement clearing of undo stack
void AnimationProject::clearUndoStack()
{
    undoStack->clear();
    return;
}


const QString AnimationProject::getAppTempDirName() const
{
    return QString(this->frontend->getTempDirName());
}


const QString AnimationProject::getAppTrashDirName() const
{
    return QString(this->frontend->getTrashDirName());
}


const QString AnimationProject::getNewProjectFilePath() const
{
    return serializer->getNewProjectFilePath();
}


const QString AnimationProject::getNewProjectPath() const
{
    return serializer->getNewProjectPath();
}


const QString AnimationProject::getDescription() const
{
    return this->description;
}


const QString AnimationProject::getNewImagePath() const
{
    return serializer->getNewImagePath();
}


const QString AnimationProject::getOldImagePath() const
{
    return serializer->getOldImagePath();
}


const QString AnimationProject::getNewSoundPath() const
{
    return serializer->getNewSoundPath();
}


const QString AnimationProject::getOldSoundPath() const
{
    return serializer->getOldSoundPath();
}


int AnimationProject::getVideoSource() const
{
    return videoSource;
}


void AnimationProject::setVideoSource(int newVideoSource)
{
    videoSource = newVideoSource;
    setUnsavedChanges();
}


int AnimationProject::getMixMode() const
{
    return mixMode;
}


void AnimationProject::setMixMode(int newMixMode)
{
    mixMode = newMixMode;
    setUnsavedChanges();
}


int AnimationProject::getUnitMode() const
{
    return unitMode;
}


void AnimationProject::setUnitMode(int newUnitMode)
{
    unitMode = newUnitMode;
    setUnsavedChanges();
}


int AnimationProject::getMixCount() const
{
    return mixCount;
}


void AnimationProject::setMixCount(int newMixCount)
{
    mixCount = newMixCount;
    setUnsavedChanges();
}


int AnimationProject::getPlaybackCount() const
{
    return playbackCount;
}


void AnimationProject::setPlaybackCount(int newPlaybackCount)
{
    playbackCount = newPlaybackCount;
    setUnsavedChanges();
}


int AnimationProject::getFramesPerSecond() const
{
    return framesPerSecond;
}


void AnimationProject::setFramesPerSecond(int newFPS)
{
    framesPerSecond = newFPS;
    setUnsavedChanges();
}


bool AnimationProject::openProject(const QString &filePath)
{
    qDebug("AnimationProject::openProject --> Start");

    serializer->setNewProjectFilePath(filePath);
    serializer->read();

    if (!readSettingsFromProject(serializer->getSettingsElement())) {
        qWarning("AnimationProject::openProject --> Read settings failed");
        return false;
    }
    if (!readScenesFromProject(serializer->getAnimationElement())) {
        qWarning("AnimationProject::openProject --> Read animation data failed");
        return false;
    }
    if (scenes.size() == 0) {
        qDebug("AnimationProject::openProject --> End (without scene)");
        return false;
    }

    activeProject = true;

    qDebug("AnimationProject::openProject --> End");
    return true;
}


bool AnimationProject::saveProject(const QString &filePath, bool saveAs)
{
    qDebug("AnimationProject::saveProject --> Start");

    if (!filePath.isEmpty()) {
        serializer->setNewProjectFilePath(filePath);
    }

    frontend->showProgress(tr("Saving scenes to disk ..."), frontend->getProject()->getTotalExposureSize());

    if (!serializer->save(this, saveAs)) {
        qWarning("AnimationProject::saveProject --> save animation data failed");
        frontend->hideProgress();
        return false;
    }
    this->unsavedChanges = false;

    frontend->hideProgress();

    qDebug("AnimationProject::saveProject --> End");
    return true;
}


bool AnimationProject::newProject(const QString &projectDescription)
{
    qDebug("AnimationProject::newProject --> Start");

    description.append(projectDescription);

    activeProject = true;

    qDebug("AnimationProject::newProject --> End");
    return true;
}


void AnimationProject::clearProject()
{
    qDebug("AnimationProject::clearProject --> Start");

    unsigned int sceneSize = scenes.size();
    for (unsigned int sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        delete scenes[sceneIndex];
        scenes[sceneIndex] = NULL;
    }

    scenes.clear();
    serializer->cleanup();
    activeSceneIndex = -1;
    unsavedChanges = false;
    activeProject = false;
    Exposure::tempNum = 0;
    Exposure::trashNum = 0;
    description.clear();

    qDebug("AnimationProject::clearProject --> End");
}


bool AnimationProject::isUnsavedChanges() const
{
    return unsavedChanges;
}


void AnimationProject::setUnsavedChanges()
{
    unsavedChanges = true;
}


bool AnimationProject::isActiveProject() const
{
    return activeProject;
}


bool AnimationProject::initAudioDevice()
{
    qDebug("AnimationProject::initAudioDevice --> Empty");
    /*
    isAudioDriverInitialized = audioDriver->initialize();
    if (!isAudioDriverInitialized && numSounds > -1) {
        frontend->showWarning(tr("Initialization Audio"),
            tr("Cannot play sound. Check that you have the right\n"
            "permissions and other programs do not block\n"
            "the audio device. Audio will be disabled until you\n"
            "have fixed the problem."));
    }

    qDebug("AnimationProject::initAudioDevice --> Start");
    return isAudioDriverInitialized;
    */
    return false;
}


void AnimationProject::shutdownAudioDevice()
{
    qDebug("AnimationProject::shutdownAudioDevice --> Start");

    audioDriver->shutdown();
    isAudioDriverInitialized = false;

    qDebug("AnimationProject::shutdownAudioDevice --> End");
}

/*
void AnimationProject::animationChanged(const QString &alteredFile)
{
    qDebug("AnimationProject::animationChanged --> Start");

    Q_ASSERT(alteredFile != NULL);
    if (activeSceneIndex == -1) {
        return;
    }

    int exposureSize = scenes[activeSceneIndex]->getExposureSize();
    int changedExposure = -1;
    for (int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        Exposure *exposure = getExposure(activeSceneIndex, getActiveTakeIndex(), exposureIndex);
        if (alteredFile.compare(exposure->getImagePath(), Qt::CaseInsensitive) == 0) {
            changedExposure = exposureIndex;
            break;
        }
    }

    if (changedExposure >= 0) {
        notifyRemoveExposures(changedExposure, changedExposure);
        notifyNewExposure(changedExposure);
    }

    qDebug("AnimationProject::animationChanged --> End");
}
*/

bool AnimationProject::exportToVideo(VideoEncoder * encoder)
{
    qDebug("AnimationProject::exportToVideo --> Start");

    VideoEncoderFactory factory(this->getFrontend());
    if (factory.createVideoFile(encoder) != NULL) {
        return true;
    }

    qDebug("AnimationProject::exportToVideo --> End");
    return false;
}


bool AnimationProject::exportToCinelerra(const QString&)
{
    return false;
}


bool AnimationProject::readSettingsFromProject(QDomElement &settingsNode)
{
    qDebug("AnimationProject::readSettingsFromProject --> Start");

    QDomElement currElement = settingsNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        // The node is a source node
        if (nodeName.compare("videosource") == 0) {
            QString tmp = currElement.text();
            if (frontend->setVideoSource(tmp.toInt())) {
                videoSource = tmp.toInt();
            }
            else {
                setVideoSource(frontend->getVideoSource());
            }
        }
        else if (nodeName.compare("mixmode") == 0) {
            QString tmp = currElement.text();
            mixMode = tmp.toInt();
            frontend->setMixMode(tmp.toInt());
        }
        else if (nodeName.compare("mixcount") == 0) {
            QString tmp = currElement.text();
            mixCount = tmp.toInt();
            frontend->setMixCount(tmp.toInt());
        }
        else if (nodeName.compare("playbackcount") == 0) {
            QString tmp = currElement.text();
            playbackCount = tmp.toInt();
        }
        else if (nodeName.compare("framespersecond") == 0) {
            QString tmp = currElement.text();
            framesPerSecond = tmp.toInt();
        }
        else if (nodeName.compare("unitmode") == 0) {
            QString tmp = currElement.text();
            unitMode = tmp.toInt();
        }

        currElement = currElement.nextSiblingElement();
    }

    qDebug("AnimationProject::readSettingsFromProject --> End");
    return true;
}


bool AnimationProject::saveSettingsToProject(QDomDocument &doc, QDomElement &settingsNode)
{
    qDebug("AnimationProject::saveSettingsToProject --> Start");

    // Save project parameter

    // Save video source parameter
    QDomElement videoSourceElement = doc.createElement("videosource");
    QDomText videoSourceText = doc.createTextNode(QString("%1").arg(videoSource));
    videoSourceElement.appendChild(videoSourceText);
    settingsNode.appendChild(videoSourceElement);

    // Save mixing mode parameter
    QDomElement mixingModeElement = doc.createElement("mixmode");
    QDomText mixingModeText = doc.createTextNode(QString("%1").arg(mixMode));
    mixingModeElement.appendChild(mixingModeText);
    settingsNode.appendChild(mixingModeElement);

    // Save mix count parameter
    QDomElement mixCountElement = doc.createElement("mixcount");
    QDomText mixCountText = doc.createTextNode(QString("%1").arg(mixCount));
    mixCountElement.appendChild(mixCountText);
    settingsNode.appendChild(mixCountElement);

    // Save playback count parameter
    QDomElement playbackCountElement = doc.createElement("playbackcount");
    QDomText playbackCountText = doc.createTextNode(QString("%1").arg(playbackCount));
    playbackCountElement.appendChild(playbackCountText);
    settingsNode.appendChild(playbackCountElement);

    // Save framesPerSecond parameter
    QDomElement fpsElement = doc.createElement("framespersecond");
    QDomText fpsText = doc.createTextNode(QString("%1").arg(framesPerSecond));
    fpsElement.appendChild(fpsText);
    settingsNode.appendChild(fpsElement);

    // Save unit mode parameter
    QDomElement unitModeElement = doc.createElement("unitmode");
    QDomText unitModeText = doc.createTextNode(QString("%1").arg(unitMode));
    unitModeElement.appendChild(unitModeText);
    settingsNode.appendChild(unitModeElement);

    qDebug("AnimationProject::saveSettingsToProject --> End");
    return true;
}


/**************************************************************************
 * Scene functions
 **************************************************************************/

int AnimationProject::getActiveSceneIndex() const
{
    return activeSceneIndex;
}


int AnimationProject::getSceneIndex(Scene* scene)
{
    int index;

    index = scenes.indexOf(scene);
    Q_ASSERT(index != -1);

    return index;
}


void AnimationProject::setActiveScene(Scene *newScene)
{
    qDebug("AnimationProject::setActiveScene --> Start");

    int sceneSize = scenes.size();
    int sceneIndex;

    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        if (scenes[sceneIndex] == newScene) {
            break;
        }
    }

    Q_ASSERT(sceneIndex != sceneSize);

    if (sceneIndex != activeSceneIndex) {
        this->activeSceneIndex = sceneIndex;
    }

    qDebug("AnimationProject::setActiveScene --> End");
}


void AnimationProject::setActiveSceneIndex(int sceneIndex)
{
    qDebug("AnimationProject::setActiveSceneIndex --> Start");

    // The new scene index can be the same as the active scene index
    // if the active scene is removed from the scene vector
    // Q_ASSERT(sceneIndex != activeSceneIndex)

    this->activeSceneIndex = sceneIndex;
    this->activeSceneId.clear();
    if (0 <= sceneIndex) {
        this->activeSceneId.append(this->getScene(this->activeSceneIndex)->getId());
    }

    qDebug("AnimationProject::setActiveSceneIndex --> End");
}


Scene *AnimationProject::getActiveScene()
{
    Q_ASSERT(activeSceneIndex >= 0);

    return scenes[activeSceneIndex];
}


int AnimationProject::getSceneSize() const
{
    return scenes.size();
}


int AnimationProject::getSceneExposureSize(int sceneIndex) const
{
    return scenes[sceneIndex]->getExposureSize();
}


Scene *AnimationProject::getScene(int sceneIndex)
{
    if (sceneIndex > -1 && sceneIndex < scenes.size()) {
        return scenes[sceneIndex];
    }
    return NULL;
}


Scene *AnimationProject::addScene(const QString &sceneDescription)
{
    qDebug("AnimationProject::addScene --> Start");

    Scene *scene = new Scene(this);

    scene->setId(QString("%1").arg(nextSceneIndex, 3, 10, QChar('0')));
    this->nextSceneIndex++;

    if (!sceneDescription.isEmpty()) {
        scene->setDescription(sceneDescription);
    }

    scenes.append(scene);
    if (activeSceneIndex == -1) {
        // Empty project
        activeSceneIndex = 0;
    }

    qDebug("AnimationProject::addScene --> End");

    return scene;
}


Scene *AnimationProject::insertScene(int sceneIndex, const QString &sceneDescription)
{
    qDebug("AnimationProject::insertScene --> Start");

    Scene *scene = new Scene(this);

    scene->setId(QString("%1").arg(nextSceneIndex, 3, 10, QChar('0')));
    this->nextSceneIndex++;

    if (!sceneDescription.isEmpty()) {
        scene->setDescription(sceneDescription);
    }

    if (activeSceneIndex == -1) {
        // Empty project
        activeSceneIndex = 0;
        scenes.append(scene);
    }
    else {
        // Project has one or more scenes
        scenes.insert(sceneIndex, scene);
        if (sceneIndex <= activeSceneIndex) {
            activeSceneIndex++;
        }
    }

    qDebug("AnimationProject::insertScene --> End");

    return scene;
}


void AnimationProject::moveScene(int sceneIndex, int movePosition)
{
    if (sceneIndex != movePosition) {
        this->setActiveSceneIndex(-1);

        Scene *scene = scenes[sceneIndex];
        scenes.remove(sceneIndex);
        scenes.insert(movePosition, scene);

        // this->notifyMoveScene(sceneIndex, movePosition);
    }
}

/*
void AnimationProject::removeActiveScene()
{
    if (activeSceneIndex < 0) {
        return;
    }

    int sceneIndex = activeSceneIndex;

    if (sceneIndex < getSceneSize() - 1) {
        activeSceneIndex = sceneIndex + 1;
    } else {
        activeSceneIndex = sceneIndex - 1;
    }

    scenes.remove(sceneIndex);
}
*/

Scene *AnimationProject::removeScene(unsigned int sceneIndex)
{
    qDebug("AnimationProject::removeScene --> Start");

    if (activeSceneIndex == (getSceneSize() - 1)) {
        // Last scene is active and will be removed
        activeSceneIndex--;
    }
    Scene *removedScene = scenes[sceneIndex];
    scenes.remove(sceneIndex);

    qDebug("AnimationProject::removeScene --> End");
    return removedScene;
}


bool AnimationProject::readScenesFromProject(QDomElement &animationNode)
{
    qDebug("AnimationProject::readScenesFromProject --> Start");

    description.append(animationNode.attributeNode(QString("descr")).value());

    QDomElement currElement = animationNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        // The node is a scene node
        if (nodeName.compare("scene") == 0) {
            Scene *newScene = new Scene(this);
            scenes.append(newScene);
            frontend->getView()->notifyAddScene(newScene->getIndex());

            if (!newScene->readDataFromProject(currElement)) {
                qDebug("AnimationProject::readScenesFromProject --> End (false)");
                return false;
            }
        }
        else if (nodeName.compare("activescene") == 0) {
            activeSceneId.clear();
            activeSceneId.append(currElement.text());
        }
        else if (currElement.nodeName().compare("nextscene") == 0) {
            nextSceneIndex = currElement.text().toUInt();
        }

        currElement = currElement.nextSiblingElement();
    }

    // Search the active scene with the readed id.
    int sceneSize = scenes.size();
    for ( int sceneIndex = 0; sceneIndex < sceneSize; sceneIndex++) {
        if (scenes[sceneIndex]->getId().compare(activeSceneId) == 0) {
            activeSceneIndex = sceneIndex;
            break;
        }
    }

    // Compatibility for old project files.
    if (nextSceneIndex == 0) {
        nextSceneIndex = sceneSize;
    }

    qDebug("AnimationProject::readScenesFromProject --> End");
    return true;
}


bool AnimationProject::saveScenesToProject(QDomDocument &doc, QDomElement &animationNode)
{
    qDebug("AnimationProject::saveScenesToProject --> Start");

    unsigned int  sceneSize = scenes.size();
    unsigned int  sceneIndex;
    QDomElement   sceneElement;

    // Save project parameter
    animationNode.setAttribute("descr", description);

    // Removes frames which already are saved. Had to do this to prevent
    // frames to overwrite each other.
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        Scene *scene = scenes[sceneIndex];
        unsigned int takeSize = scene->getTakeSize();
        for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
            Take *take = scene->getTake(takeIndex);
            unsigned int exposureSize = take->getExposureSize();
            for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                Exposure *exposure = take->getExposure(exposureIndex);
                exposure->moveToTemp();
            }
        }
    }

    // Delete all image file that stay in the image directory
    // (All files that deleted from the project.)
    QString     imagePath = this->getOldImagePath();
    QDir        imageDir(imagePath);
    QStringList fileNameList = imageDir.entryList(QDir::Files);
    for (int fileIndex = 0 ; fileIndex < fileNameList.count() ; fileIndex++) {
        imageDir.remove(fileNameList[fileIndex]);
    }

    nextTotalExposureIndex = 0;
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        // Scenes
        sceneElement = doc.createElement("scene");
        animationNode.appendChild(sceneElement);

        if (!scenes[sceneIndex]->saveDataToProject(doc, sceneElement, sceneIndex)) {
            qDebug("AnimationProject::saveScenesToProject --> End (false)");
            return false;
        }
    }

    if (activeSceneIndex >= 0) {
        // Save activeSceneIndex parameter after the scenes, because the names of
        // the scenes are modifyed during the writing of the project file.
        QDomElement asiElement = doc.createElement("activescene");
        QDomText asiText = doc.createTextNode(scenes[activeSceneIndex]->getId());
        asiElement.appendChild(asiText);
        animationNode.appendChild(asiElement);

        // Save nextSceneIndex parameter.
        QDomElement leiElement = doc.createElement("nextscene");
        QDomText leiText = doc.createTextNode(QString("%1").arg(sceneSize));
        leiElement.appendChild(leiText);
        animationNode.appendChild(leiElement);
    }

    qDebug("AnimationProject::saveScenesToProject --> End");
    return true;
}


bool AnimationProject::saveAsScenesToProject(QDomDocument &doc, QDomElement &animationNode)
{
    qDebug("AnimationProject::saveScenesToProject --> Start");

    unsigned int  sceneSize = scenes.size();
    unsigned int  sceneIndex;
    QDomElement   sceneElement;

    // Save project parameter
    animationNode.setAttribute("descr", description);

    // Removes frames which already are saved. Had to do this to prevent
    // frames to overwrite each other.
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        Scene *scene = scenes[sceneIndex];
        unsigned int takeSize = scene->getTakeSize();
        for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
            Take *take = scene->getTake(takeIndex);
            unsigned int exposureSize = take->getExposureSize();
            for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                Exposure *exposure = take->getExposure(exposureIndex);
                exposure->copyToTemp();
            }
        }
    }

    nextTotalExposureIndex = 0;
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        // Scenes
        sceneElement = doc.createElement("scene");
        animationNode.appendChild(sceneElement);

        if (!scenes[sceneIndex]->saveDataToProject(doc, sceneElement, sceneIndex)) {
            qDebug("AnimationProject::saveScenesToProject --> End (false)");
            return false;
        }
    }

    if (activeSceneIndex >= 0) {
        // Save activeSceneIndex parameter after the scenes, because the names of
        // the scenes are modifyed during the writing of the project file.
        QDomElement asiElement = doc.createElement("activescene");
        QDomText asiText = doc.createTextNode(scenes[activeSceneIndex]->getId());
        asiElement.appendChild(asiText);
        animationNode.appendChild(asiElement);

        // Save nextSceneIndex parameter.
        QDomElement leiElement = doc.createElement("nextscene");
        QDomText leiText = doc.createTextNode(QString("%1").arg(sceneSize));
        leiElement.appendChild(leiText);
        animationNode.appendChild(leiElement);
    }

    qDebug("AnimationProject::saveScenesToProject --> End");
    return true;
}


/**************************************************************************
 * Sound functions
 **************************************************************************/

int AnimationProject::addSoundToScene(unsigned int sceneIndex, const QString &sound, const QString &soundName)
{
    qDebug("AnimationProject::addSoundToScene --> Start");

    ++numSounds;
    int ret = scenes[sceneIndex]->addSound(sound, soundName);
    if (ret == -1) {
        frontend->showWarning(tr("Add Sound"),
            tr("Cannot open the selected audio file for reading.\n"
            "Check that you have the right permissions set.\n"
            "The animation will be runned without sound if you\n"
            "choose to play."));
        --numSounds;
    } else if (ret == -2) {
        frontend->showWarning(tr("Add Sound"),
            tr("The selected audio file is not valid within the\n"
            "given audio format. The animation will be runned\n"
            "without sound if you choose to play."));
        --numSounds;
    }
    this->unsavedChanges = true;

    qDebug("AnimationProject::addSoundToScene --> End");
    return ret;
}


void AnimationProject::removeSoundFromScene(unsigned int sceneIndex, unsigned int soundNumber)
{
    scenes[sceneIndex]->removeSound(soundNumber);
    --numSounds;
    this->unsavedChanges = true;
}


void AnimationProject::playSound(int sceneIndex)
{
    if (isAudioDriverInitialized) {
        scenes[sceneIndex]->playSounds(audioDriver);
    }
    // notifyPlaySound(sceneIndex);
}


/**************************************************************************
 * Take functions
 **************************************************************************/

int AnimationProject::getActiveTakeIndex() const
{
    Q_ASSERT(activeSceneIndex > -1);

    return scenes[activeSceneIndex]->getActiveTakeIndex();
}


void AnimationProject::setActiveTakeIndex(int takeIndex)
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *activeScene = scenes[activeSceneIndex];

    activeScene->setActiveTakeIndex(takeIndex);
}


Take *AnimationProject::getActiveTake()
{
    Q_ASSERT(activeSceneIndex >= 0);

    return getTake(activeSceneIndex, getActiveTakeIndex());
}


Take *AnimationProject::addTake(unsigned int sceneIndex, const QString &takeDescription)
{
    Scene *activeScene = scenes[sceneIndex];
    Take *take = activeScene->addTake(takeDescription);

    return take;
}


Take *AnimationProject::insertTake(unsigned int sceneIndex, const QString &takeDescription)
{
    Scene *activeScene = scenes[sceneIndex];
    Take *take = activeScene->insertTake(takeDescription);

    return take;
}


void AnimationProject::moveTake(int fromSceneIndex, int fromTakeIndex, int toSceneIndex, int toTakeIndex)
{
    Q_ASSERT(fromSceneIndex > -1);
    Q_ASSERT(toSceneIndex > -1);

    // TODO: Implement the move from one scene to another
    Q_ASSERT(fromSceneIndex == toSceneIndex);

    Scene *fromScene = scenes[fromSceneIndex];

    fromScene->moveTake(fromTakeIndex, toTakeIndex);
}

/*
Take *AnimationProject::removeActiveTake()
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *activeScene = scenes[activeSceneIndex];

    Take *removedTake = activeScene->removeActiveTake();

    return removedTake;
}
*/

Take *AnimationProject::removeTake(unsigned int sceneIndex, unsigned int takeIndex)
{
    Scene *scene = scenes[sceneIndex];

    Take *removedTake = scene->removeTake(takeIndex);

    return removedTake;
}


int AnimationProject::getTakeExposureSize(unsigned int sceneIndex, unsigned int takeIndex) const
{
    return scenes[sceneIndex]->getTake(takeIndex)->getExposureSize();
}


Take *AnimationProject::getTake(int sceneIndex, int takeIndex)
{
    if (takeIndex > -1 && takeIndex < int(scenes[sceneIndex]->getTakeSize())) {
        return scenes[sceneIndex]->getTake(takeIndex);
    }
    return NULL;
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int AnimationProject::getActiveExposureIndex() const
{
    if (activeSceneIndex == -1) {
        return -1;
    }
    Scene *activeScene = scenes[activeSceneIndex];
    int activeExposureIndex = activeScene->getActiveExposureIndex();
    return activeExposureIndex;
}


void AnimationProject::setActiveExposureIndex(int exposureIndex)
{
    qDebug("AnimationProject::setActiveExposureIndex --> Start");

    scenes[this->activeSceneIndex]->setActiveExposureIndex(exposureIndex);

    qDebug("AnimationProject::setActiveExposureIndex --> End");
}


unsigned int AnimationProject::getTotalExposureSize() const
{
    unsigned int modelSize = 0;
    unsigned int size = scenes.size();
    for (unsigned int i = 0; i < size; ++i) {
        modelSize += scenes[i]->getExposureSize();
    }
    return modelSize;
}


Exposure *AnimationProject::getExposure(unsigned int sceneIndex,
                                        unsigned int takeIndex,
                                        unsigned int exposureIndex)
{
    // qDebug("AnimationProject::getExposure --> Start");

    Q_ASSERT(exposureIndex < scenes[sceneIndex]->getExposureSize());

    Exposure* exposure = scenes[sceneIndex]->getExposure(takeIndex, exposureIndex);

    // qDebug("AnimationProject::getExposure --> End");
    return exposure;
}


Exposure *AnimationProject::getActiveExposure()
{
    if (0 > activeSceneIndex) {
        // return a empty exposure
        return NULL;
    }

    int activeTakeIndex = getActiveTakeIndex();
    if (0 > activeTakeIndex) {
        // return a empty exposure
        return NULL;
    }

    int activeExposureIndex = getActiveExposureIndex();
    if (0 > activeExposureIndex) {
        // return a empty exposure
        return NULL;
    }

    return getExposure(activeSceneIndex, getActiveTakeIndex(), getActiveExposureIndex());
}


Exposure *AnimationProject::addExposure(const QString &fileName,
                                        int location)
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *activeScene = scenes[activeSceneIndex];
    Take  *activeTake = activeScene->getTake(activeScene->getActiveTakeIndex());

    Exposure *exposure = activeTake->addExposure(fileName, location);

    return exposure;
}


Exposure *AnimationProject::insertExposure(int sceneIndex,
                                           int takeIndex,
                                           int exposureIndex,
                                           const QString &fileName,
                                           int location)
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *scene = scenes[sceneIndex];
    Take  *take = scene->getTake(takeIndex);

    Exposure *exposure = take->insertExposure(fileName, location, exposureIndex);

    return exposure;
}


Exposure *AnimationProject::removeActiveExposure()
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene    *activeScene = scenes[activeSceneIndex];
    Take     *activeTake = activeScene->getTake(activeScene->getActiveTakeIndex());
    Exposure *exposure = activeTake->removeActiveExposure();

    return exposure;
}


Exposure *AnimationProject::removeExposure(int sceneIndex,
                                           int takeIndex,
                                           int exposureIndex)
{
    Scene    *scene = scenes[sceneIndex];
    Exposure *exposure = scene->removeExposure(takeIndex, exposureIndex);

    return exposure;
}


unsigned int AnimationProject::getNextTotalExposureIndex()
{
    unsigned int ret = nextTotalExposureIndex;
    nextTotalExposureIndex++;

    return ret;
}


bool AnimationProject::getModifyedExposure(const QString &filePath,
                                           int &modSceneIndex,
                                           int &modTakeIndex,
                                           int &modExposureIndex)
{
    qDebug("AnimationProject::getModifyedExposure --> Start");

    int sceneSize = scenes.size();

    for (modSceneIndex = 0; modSceneIndex < sceneSize; ++modSceneIndex) {
        if (scenes[modSceneIndex]->getModifyedExposure(filePath, modTakeIndex, modExposureIndex)) {
            // Exposure found
            qDebug("AnimationProject::getModifyedExposure --> End");
            return true;
        }
    }

    modSceneIndex = -1;

    qDebug("AnimationProject::getModifyedExposure --> End (Error)");
    return false;
}


/**************************************************************************
 * Old frame functions
 **************************************************************************/

const QVector<Exposure*> AnimationProject::removeFrames(unsigned int fromFrame, unsigned int toFrame)
{
    frontend->showInformation("Not Implemented", QString("AnimationProject::removeFrames not implemented.(%1,%2)").arg(fromFrame).arg(toFrame));
/*
    Q_ASSERT(fromFrame <= toFrame);

    QVector<Exposure*> newImagePaths;

    if (activeFrame >= 0) {
        newImagePaths = scenes[activeScene].removeExposures(fromFrame, toFrame);

        this->notifyRemove(fromFrame, toFrame);

        if (toFrame < scenes[activeScene].getExposureNumber()) {
            this->setActiveFrame(toFrame);
        } else {
            this->setActiveFrame(scenes[activeScene].getExposureNumber() - 1);
        }
        unsavedChanges = true;
    }
    this->unsavedChanges = true;

    return newImagePaths;
*/

    return QVector<Exposure*>();
}


void AnimationProject::moveFrames(unsigned int fromFrame, unsigned int toFrame,
                                  unsigned int movePosition)
{
    frontend->showInformation("Not Implemented", QString("AnimationProject::moveFrames not implemented.(%1,%2,%3)").arg(fromFrame).arg(toFrame).arg(movePosition));
/*
    unsigned int framesSize = scenes[activeScene].getExposureNumber();
    if ((fromFrame < framesSize) && (toFrame < framesSize) && (movePosition < framesSize) &&
            ((movePosition < fromFrame) || (movePosition > toFrame))) {

        scenes[activeScene].moveExposures(fromFrame, toFrame, movePosition);

        unsavedChanges = true;
        this->notifyMove(fromFrame, toFrame, movePosition);
        this->setActiveFrame(movePosition);
    }
    this->unsavedChanges = true;
*/
}


/**************************************************************************
 * Private functions
 **************************************************************************/

/*
void AnimationProject::loadSavedScenes()
{
    qDebug("AnimationProject::loadSavedScenes --> Start");

    unsigned int numElem = scenes.size();
    for (unsigned int i = 0; i < numElem; ++i) {
        notifyNewScene(i);
    }
    setActiveSceneIndex(numElem - 1);

    qDebug("AnimationProject::loadSavedScenes --> End");
}


void AnimationProject::activateScene()
{
    qDebug("AnimationProject::activateScene --> Start");

    this->notifyActivateScene();

    qDebug("AnimationProject::activateScene --> End");
}
*/
