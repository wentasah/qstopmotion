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

#include "scene.h"

#include "domain/domainfacade.h"
#include "technical/preferencestool.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QtDebug>


Scene::Scene(AnimationProject *project)
{
    parent             = project;
    activeTakeIndex    = -1;
    nextTakeIndex      = 0;
    isProjectFile      = false;
    soundsNumber       = 0;
    soundStartPosition = 0;
}


Scene::~Scene()
{
    unsigned int soundSize = sounds.size();
    for (unsigned int soundIndex = 0; soundIndex < soundSize; ++soundIndex) {
        delete sounds[soundIndex];
        sounds[soundIndex] = NULL;
    }
    sounds.clear();

    unsigned int takeSize = takes.size();
    for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
        delete takes[takeIndex];
        takes[takeIndex] = NULL;
    }
    takes.clear();
}


bool Scene::isEmpty()
{
    return takes.isEmpty();
}


const QString Scene::getId() const
{
    return id;
}


void Scene::setId(const QString &newId)
{
    if (!newId.isEmpty()) {
        id.clear();
        id.append(newId);
    }
}


const QString Scene::getDescription() const
{
    return description;
}


void Scene::setDescription(const QString &newDescription)
{
    if (!newDescription.isEmpty()) {
        description.clear();
        description.append(newDescription);
    }
}


const QString Scene::getNewImagePath() const
{
    return parent->getNewImagePath();
}


const QString Scene::getOldImagePath() const
{
    return parent->getOldImagePath();
}


AnimationProject* Scene::getParent()
{
    return this->parent;
}


unsigned int Scene::getIndex()
{
    unsigned int index;

    index = parent->getSceneIndex(this);

    return index;
}


Frontend* Scene::getFrontend()
{
    return this->parent->getFrontend();
}


void Scene::markAsProjectFile()
{
    isProjectFile = true;
}


bool Scene::readDataFromProject(QDomElement &sceneNode)
{
    qDebug("Scene::readDataFromProject --> Start");

    QString activeTakeId;

    id.append(sceneNode.attributeNode(QString("id")).value());
    description.append(sceneNode.attributeNode(QString("descr")).value());

    QDomElement currElement = sceneNode.firstChildElement();

    while (!currElement.isNull()) {
        QString elementName = currElement.nodeName();
        if (elementName.compare("take") == 0) {
            // This is a take data element
            Take *newTake = new Take(this);
            takes.append(newTake);

            if (!newTake->readDataFromProject(currElement)) {
                qWarning("Scene::readDataFromProject --> Read take data failed");
                return false;
            }
        }
        else if (currElement.nodeName().compare("activetake") == 0) {
            activeTakeId.clear();
            activeTakeId.append(currElement.text());
        }
        else if (currElement.nodeName().compare("nexttake") == 0) {
            nextTakeIndex = currElement.text().toUInt();
        }
        else if (elementName.compare("sound") == 0) {
            if (!readSoundFromProject(currElement)) {
                qWarning("Scene::readDataFromProject --> Read sound data failed");
                return false;
            }
        }
        currElement = currElement.nextSiblingElement();
    }

    // Search the active take with the readed id.
    int takeSize = takes.size();
    for ( int takeIndex = 0; takeIndex < takeSize; takeIndex++) {
        if (takes[takeIndex]->getId().compare(activeTakeId) == 0) {
            activeTakeIndex = takeIndex;
            break;
        }
    }

    // Compatibility for old project files.
    if (nextTakeIndex == 0) {
        nextTakeIndex = takeSize;
    }

    this->isProjectFile = true;

    qDebug("Scene::readDataFromProject --> End");
    return true;
}


bool Scene::saveDataToProject(QDomDocument &doc, QDomElement &sceneNode)
{
    qDebug("Scene::saveDataToProject --> Start");

    unsigned int takeSize = takes.size();
    unsigned int takeIndex;

    // Save scene attributes
    sceneNode.setAttribute("id", id);
    sceneNode.setAttribute("descr", description);

    // Save takes
    for (takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
        QDomElement takeElement = doc.createElement("take");
        sceneNode.appendChild(takeElement);
        if (!takes[takeIndex]->saveDataToProject(doc, takeElement)) {
            qWarning("Scene::saveDataToProject --> Save take data failed");
            return false;
        }
    }

    if (activeTakeIndex >= 0) {
        // Save activeTakeIndex parameter after the takes, because the names of
        // the takes are modifyed during the writing of the project file.
        QDomElement atiElement = doc.createElement("activetake");
        QDomText atiText = doc.createTextNode(takes[activeTakeIndex]->getId());
        atiElement.appendChild(atiText);
        sceneNode.appendChild(atiElement);

        // Save nextTakeIndex parameter.
        QDomElement leiElement = doc.createElement("nexttake");
        QDomText leiText = doc.createTextNode(QString("%1").arg(takeSize));
        leiElement.appendChild(leiText);
        sceneNode.appendChild(leiElement);
    }

    // Sounds
    if (!saveSoundToProject(doc, sceneNode)) {
        qWarning("Scene::saveDataToProject --> Save sound data failed");
        return false;
    }

    this->isProjectFile = true;

    qDebug("Scene::saveDataToProject --> End");
    return true;
}


const QString Scene::getAppTempDirName() const
{
    return parent->getAppTempDirName();
}


/**************************************************************************
 * Take functions
 **************************************************************************/

int Scene::getActiveTakeIndex() const
{
    return activeTakeIndex;
}


int Scene::getTakeIndex(Take *take) const
{
    int index;

    index = takes.indexOf(take);

    return index;
}


void Scene::setActiveTakeIndex(int takeIndex)
{
    qDebug("Scene::setActiveTakeIndex --> Start");

    // The new take index can be the same as the active take index
    // if the active take is removed from the take vector
    // Q_ASSERT(takeIndex != activeTakeIndex);

    this->activeTakeIndex = takeIndex;

    qDebug("Scene::setActiveTakeIndex --> End");
}


Take *Scene::getActiveTake()
{
    Q_ASSERT(activeTakeIndex > -1);

    return takes[activeTakeIndex];
}


void Scene::setActiveTake(Take *newTake)
{
    qDebug("Scene::setActiveTake --> Start");

    // Check the actual scene index
    parent->setActiveScene(this);

    int takeSize = takes.size();
    int takeIndex;
    Take *newActiveTake;

    for (takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
        newActiveTake = takes[takeIndex];
        if (newActiveTake == newTake) {
            break;
        }
    }

    Q_ASSERT(takeIndex != takeSize);

    if (takeIndex != activeTakeIndex) {
        this->activeTakeIndex = takeIndex;
    }

    qDebug("Scene::setActiveTake --> End");
}


int Scene::getTakeSize() const
{
    return takes.size();
}


Take* Scene::getTake(unsigned int takeIndex)
{
    return takes[takeIndex];
}


void Scene::addTake(const QString &takeDescription)
{
    qDebug("Scene::addTake(new) --> Start");

    Take *take = new Take(this);

    take->setId(QString("%1").arg(nextTakeIndex, 2, 10, QChar('0')));
    nextTakeIndex++;

    if (!takeDescription.isEmpty()) {
        take->setDescription(takeDescription);
    }
    takes.append(take);

    qDebug("Scene::addTake(new) --> End");
}


void Scene::addTake(Take *take)
{
    qDebug("Scene::addTake(exist) --> Start");

    takes.append(take);

    qDebug("Scene::addTake(exist) --> End");
}


void Scene::insertTake(int takeIndex, const QString &takeDescription)
{
    qDebug("Scene::insertTake(new) --> Start");

    Q_ASSERT(takeIndex > -1);
    Q_ASSERT(takeIndex < getTakeSize());

    Take *take = new Take(this);

    take->setId(QString("%1").arg(nextTakeIndex, 2, 10, QChar('0')));
    nextTakeIndex++;

    if (!takeDescription.isEmpty()) {
        take->setDescription(takeDescription);
    }

    takes.insert(takeIndex, take);
    if (takeIndex <= activeTakeIndex) {
        setActiveTakeIndex(activeTakeIndex+1);
    }

    qDebug("Scene::insertTake(new) --> End");
}


void Scene::insertTake(int takeIndex, Take *take)
{
    qDebug("Scene::insertTake(exist) --> Start");

    Q_ASSERT(takeIndex > -1);
    Q_ASSERT(takeIndex < getTakeSize());

    takes.insert(takeIndex, take);
    if (takeIndex <= activeTakeIndex) {
        setActiveTakeIndex(activeTakeIndex+1);
    }

    qDebug("Scene::insertTake(exist) --> End");
}


void Scene::moveTake(int takeIndex, int movePosition)
{
    if (takeIndex != movePosition) {
        this->setActiveTakeIndex(-1);

        Take *take = takes[takeIndex];
        takes.remove(takeIndex);
        takes.insert(movePosition, take);

        // this->notifyMoveScene(takeIndex, movePosition);
    }
}


Take *Scene::removeTake(int takeIndex)
{
    qDebug("Scene::removeTake --> Start");

    Q_ASSERT(takeIndex != activeTakeIndex);

    if (takeIndex < activeTakeIndex) {
        // Take bevor the active take will be removed
        setActiveTakeIndex(activeTakeIndex-1);
        Q_ASSERT(-1 < activeTakeIndex);
    }
    Take *removedTake = takes[takeIndex];
    takes.remove(takeIndex);

    qDebug("Scene::removeTake --> End");
    return removedTake;
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int Scene::getActiveExposureIndex() const
{
    Q_ASSERT(activeTakeIndex >= 0);

    Take *activeTake = takes[activeTakeIndex];
    int   activeExposureIndex = activeTake->getActiveExposureIndex();
    return activeExposureIndex;
}


void Scene::setActiveExposureIndex(int exposureIndex)
{
    if (0 > activeTakeIndex) {
        // No active take --> nothing to do
        return;
    }
    takes[activeTakeIndex]->setActiveExposureIndex(exposureIndex);
}


unsigned int Scene::getExposureSize() const
{
    unsigned int exposureNumber = 0;
    unsigned int numElem = takes.size();

    for (unsigned int i = 0; i < numElem; ++i) {
        exposureNumber += takes[i]->getExposureSize();
    }

    return exposureNumber;
}


Exposure* Scene::getExposure(unsigned int takeIndex, unsigned int exposureIndex)
{
    return takes[takeIndex]->getExposure(exposureIndex);
}


void Scene::getExposures(QVector<Exposure*>& allExposures)
{
    unsigned int numTakes = takes.size();

    for (unsigned int countTake = 0; countTake < numTakes; ++countTake) {
        unsigned int numExpos = takes[countTake]->getExposureSize();

        for (unsigned int countExpo = 0; countExpo < numExpos; ++countExpo) {
            allExposures.append(takes[countTake]->getExposure(countExpo));
        }
    }
}


void Scene::addExposure(int takeIndex, const QString &fileName, int location)
{
    getTake(takeIndex)->addExposure(fileName, location);
}


void Scene::addExposure(int takeIndex, Exposure *exposure)
{
    getTake(takeIndex)->addExposure(exposure);
}


void Scene::insertExposure(int takeIndex, int exposureIndex,
                           const QString &fileName, int location)
{
    getTake(takeIndex)->insertExposure(exposureIndex, fileName, location);
}


void Scene::insertExposure(int takeIndex, int exposureIndex, Exposure *exposure)
{
    getTake(takeIndex)->insertExposure(exposureIndex, exposure);
}


bool Scene::getModifyedExposure(const QString &filePath,
                                int &modTakeIndex,
                                int &modExposureIndex)
{
    qDebug("Scene::getModifyedExposure --> Start");

    int takeSize = takes.size();

    for (modTakeIndex = 0; modTakeIndex < takeSize; ++modTakeIndex) {
        if (takes[modTakeIndex]->getModifyedExposure(filePath, modExposureIndex)) {
            // Exposure found
            qDebug("Scene::getModifyedExposure --> End");
            return true;
        }
    }

    modTakeIndex = -1;

    qDebug("Scene::getModifyedExposure --> End (Error)");
    return false;
}


void Scene::cleanExposures(unsigned int fromExposure, unsigned int toExposure)
{
    takes.erase(takes.begin() + fromExposure, takes.begin() + toExposure);
}


void Scene::addSavedExposure(Exposure *e)
{
    takes[activeTakeIndex]->addSavedExposure(e);;
}


const QVector<Exposure*> Scene::removeExposures(unsigned int fromExposure, const unsigned int toExposure)
{
    return takes[activeTakeIndex]->removeExposures(fromExposure, toExposure);
}


void Scene::moveExposures(unsigned int fromExposure, unsigned int toExposure,
                          unsigned int movePosition)
{
    takes[activeTakeIndex]->moveExposures(fromExposure, toExposure, movePosition);
}


Exposure* Scene::removeExposure(unsigned int takeIndex, unsigned int exposureIndex)
{
    Take *take = takes[takeIndex];

    return take->removeExposure(exposureIndex);
}


QVector<QString> Scene::getImagePaths() const
{
    QVector<QString> sceneImagePaths;
    unsigned int takeSize = takes.size();
    unsigned int takeIndex;
    unsigned int takeImageSize;
    unsigned int takeImageIndex;

    for (takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
        QVector<QString> takeImagePaths = takes[takeIndex]->getImagePaths();
        takeImageSize = takeImagePaths.size();
        for (takeImageIndex = 0; takeImageIndex < takeImageSize; ++takeImageIndex) {
            sceneImagePaths.append(takeImagePaths[takeImageIndex]);
        }
    }
    return sceneImagePaths;
}


/**************************************************************************
 * Sound functions
 **************************************************************************/

unsigned int Scene::getSoundStartPosition() const
{
    qDebug("Take::getSountPosition --> Start");

    return this->soundStartPosition;
}


void Scene::setSoundStartPosition(unsigned int newSoundStartPosition)
{
    soundStartPosition = newSoundStartPosition;
}


// TODO: Check audio type (ogg, mp3, wav ...)
int Scene::addSound(const QString & /*filename*/, const QString & /*soundname*/)
{
    qDebug("Scene::addSound --> Start");

    /*
    AudioFormat *f = new OggVorbis();
    int ret = f->setFilename(filename);
    if (ret != 0) {
        delete f;
        return ret;
    }

    // Check if the file exsists, which it probably should do.
    // The setFilename function will fail if the file doesn't exists,
    // but paranoid people have to dobbel check :-).
    if (QFile::exists(filename) == 0) {
        // Create a new path
        QString imgId(getImageId());
        QString newSoundPath(QString("%1%2_snd_%3%4").arg(tempPath)
                             .arg(imgId)
                             .arg(++soundNum)
                             .arg(filename.mid(filename.lastIndexOf('.'))));

        // Check if the sound already is inside the tmp directory.
        // (This can be the fact if we runs in recovery mode.)
        QString tempName;
        tempName.append(QLatin1String("/"));
        tempName.append(ProjectSerializer::applicationDirectory);
        tempName.append(QLatin1String("/"));
        tempName.append(ProjectSerializer::tempDirectory);
        if (filename.indexOf(tempName, 0, Qt::CaseInsensitive) == -1) {
            if (!QFile::copy(filename, newSoundPath)) {
                // Not successful
                parent->getFrontend()->showCritical(tr("Critical"),
                                                    tr("Can't copy sound file!"));
            }
        } else {
            if (!QFile::rename(filename, newSoundPath)) {
                // Not successful
                parent->getFrontend()->showCritical(tr("Critical"),
                                                    tr("Can't rename soundfile!"));
            }
        }

        // Update with the new path
        f->setFilename(newSoundPath);
        // and add it to the vector
        sounds.append(f);

        QString ss("Sound");
        ss.append(QString::number(soundsNumber));
        soundNames.append(ss);

        soundNames[soundNumber] = soundName;

        return 0;
    }
    */
    qDebug("Scene::addSound --> End");

    return 0;
}


void Scene::removeSound(unsigned int soundIndex)
{
    Q_ASSERT(sounds[soundIndex] != NULL);

    AudioFormat *f = sounds[soundIndex];
    if (QFile::exists(f->getSoundPath()) == 0) {
        if (!QFile::remove(f->getSoundPath())) {
            // Not successful
            parent->getFrontend()->showCritical(tr("Critical"),
                                                tr("Can't remove sound file!"));
        }
    }
    delete sounds[soundIndex];
    sounds[soundIndex] = NULL;
    sounds.erase(sounds.begin() + soundIndex);
    --soundsNumber;
}


unsigned int Scene::getNumberOfSounds() const
{
    return sounds.size();
}


QVector<AudioFormat*>& Scene::getSounds()
{
    return sounds;
}


const QString Scene::getSoundName(unsigned int soundIndex) const
{
    return soundNames[soundIndex];
}


void Scene::playSounds(AudioDriver *driver)
{
    unsigned int numElem = sounds.size();
    if (numElem > 0) {
        for (unsigned int i = 0; i < numElem; ++i) {
            driver->addAudioFile(sounds[i]);
        }
        driver->playInThread();
    }
}


void Scene::moveToSoundDir(const QString &directory)
{
    Q_ASSERT(directory.isEmpty());

    // Move all of the sounds belonging to this Scene
    // to the sounds directory
    unsigned int numSounds = sounds.size();
    for (unsigned int i = 0; i < numSounds; ++i) {
        AudioFormat *f = sounds[i];
        QString soundPath = f->getSoundPath();

        // Create a new sound path
        QString newSoundPath(QString("%1/%2_snd_%3%4")
                             .arg(directory)
                             .arg(this->id)
                             .arg(++soundsNumber)
                             .arg(soundPath.mid(soundPath.lastIndexOf('.'))));

        if (QFile::exists(soundPath) == 0) {
            // Move from old path to new path
            if (!QFile::rename(soundPath, newSoundPath)) {
                // Not successful
                parent->getFrontend()->showCritical(tr("Critical"),
                                                    tr("Can't move sound file!"));
            }
        }
        // Update with the new path
        f->setFilename(newSoundPath);
        f = NULL;
    }
}


bool Scene::readSoundFromProject(QDomElement &soundNode)
{
    qDebug("Scene::readSoundFromProject --> Start");

    QDomElement currElement = soundNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        if (nodeName.compare("clip") == 0) {
            // The node is a sound file
            QString soundFile(QString("%1/%2")
                              .arg(soundPath)
                              .arg(currElement.text()));
            QString soundName = currElement.attribute("descr", NULL);
            addSound(soundFile, soundName);
        }
        else if (nodeName.compare("offset") == 0) {
            // The node is a offset value
            QString tmp = currElement.text();
            soundStartPosition = tmp.toInt();
        }

        currElement = currElement.nextSiblingElement();
    }

    qDebug("Scene::readSoundFromProject --> End");
    return true;
}


bool Scene::saveSoundToProject(QDomDocument &doc, QDomElement &sceneNode)
{
    qDebug("Scene::saveSoundFromProject --> Start");


    QDomElement   clipElement;

    // QString       absPath;
    QString       relPath;
    AudioFormat  *sound;

    moveToSoundDir(soundPath);
    markAsProjectFile();

    // Sounds
    unsigned int soundSize = getNumberOfSounds();
    for (unsigned int soundIndex = 0; soundIndex < soundSize; ++soundIndex) {
        sound = sounds[soundIndex];
        relPath.clear();
        QString soundPath = sounds[soundIndex]->getSoundPath();
        relPath.append(soundPath.mid(soundPath.lastIndexOf("/")));
        clipElement = doc.createElement("clip");
        sceneNode.appendChild(clipElement);
        clipElement.setAttribute("src", relPath);
        clipElement.setAttribute("alt", getSoundName(soundIndex));
    }

    qDebug("Scene::saveSoundFromProject --> End");
    return true;
}

/**************************************************************************
 * Operator functions
 **************************************************************************/
/*
bool Scene::operator!= (const Scene &other) const
{
    qDebug("Scene::operator!= --> Start");

    qDebug("Scene::operator!= --> End");
}
*/
/**************************************************************************
 * Private functions
 **************************************************************************/
/*
void Scene::activateTake()
{
    qDebug("Scene::activateTake --> Start");

    parent->notifyActivateTake();

    qDebug("Scene::activateTake --> End");
}
*/
