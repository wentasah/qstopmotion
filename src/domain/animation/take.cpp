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

#include "take.h"

#include "domain/domainfacade.h"

#include <QtCore/QFileInfo>
#include <QtCore/QtDebug>


Take::Take(Scene *scene)
{
    parent = scene;
    activeExposureIndex = -1;
    nextExposureIndex = 0;
    isProjectFile = false;
    soundStartPosition = 0;
}


Take::~Take()
{
    unsigned int exposureSize = exposures.size();
    for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        delete exposures[exposureIndex];
        exposures[exposureIndex] = NULL;
    }
    exposures.clear();
}


bool Take::isEmpty()
{
    return exposures.isEmpty();
}


const QString Take::getId() const
{
    return id;
}


void Take::setId(const QString &newId)
{
    if (!newId.isEmpty()) {
        id.clear();
        id.append(newId);
    }
}


const QString Take::getDescription() const
{
    return description;
}


void Take::setDescription(const QString &newDescription)
{
    if (!newDescription.isEmpty()) {
        description.clear();
        description.append(newDescription);
    }
}


Scene *Take::getScene()
{
    return parent;
}


unsigned int Take::getIndex()
{
    unsigned int index;

    index = parent->getTakeIndex(this);

    return index;
}


unsigned int Take::getSceneIndex()
{
    unsigned int index;

    index = parent->getIndex();

    return index;
}


const QString Take::getImagePath() const
{
    return parent->getImagePath();
}


Frontend* Take::getFrontend()
{
    return parent->getFrontend();
}


AnimationProject* Take::getAnimationProject()
{
    return parent->getParent();
}


bool Take::readDataFromProject(QDomElement &takeNode)
{
    qDebug("Take::readDataFromProject --> Start");

    id.append(takeNode.attributeNode(QString("id")).value());
    description.append(takeNode.attributeNode(QString("descr")).value());

    QDomElement currElement = takeNode.firstChildElement();

    while (!currElement.isNull()) {
        QString elementName = currElement.nodeName();
        if (elementName.compare("exposure") == 0) {
            // This is a exposure data element
            Exposure *newExposure = new Exposure(this);
            exposures.append(newExposure);
            this->getFrontend()->getProject()->addExposure(newExposure);

            if (!newExposure->readDataFromProject(currElement)) {
                qWarning("Take::readDataFromProject --> Read exposure data failed");
                return false;
            }
        }
        else if (currElement.nodeName().compare("activeexposure") == 0) {
            activeExposureId.clear();
            activeExposureId.append(currElement.text());
        }
        else if (currElement.nodeName().compare("nextexposure") == 0) {
            nextExposureIndex = currElement.text().toUInt();
        }
        else if (elementName.compare("sound") == 0) {
            if (!readSoundFromProject(currElement)) {
                qWarning("Take::readDataFromProject --> Read sound data failed");
                return false;
            }
        }
        currElement = currElement.nextSiblingElement();
    }

    // Search the active exposure with the readed id.
    int exposureSize = exposures.size();
    for ( int exposureIndex = 0; exposureIndex < exposureSize; exposureIndex++) {
        if (exposures[exposureIndex]->getId().compare(activeExposureId) == 0) {
            activeExposureIndex = exposureIndex;
            break;
        }
    }

    // Compatibility for old project files.
    if (nextExposureIndex == 0) {
        nextExposureIndex = exposureSize;
    }

    this->isProjectFile = true;

    qDebug("Take::readDataFromProject --> End");
    return true;
}


bool Take::saveDataToProject(QDomDocument &doc, QDomElement &takeNode,
                             const QString &sceneId, int takeIndex)
{
    qDebug("Take::saveDataFromProject --> Start");

    unsigned int exposureSize = exposures.size();
    unsigned int exposureIndex;

    // Create a new id for the take
    id.clear();
    id.append(QString("%1").arg((takeIndex), 2, 10, QChar('0')));

    // Save take attributes
    takeNode.setAttribute("id", id);
    takeNode.setAttribute("descr", description);

    // Save exposureses
    for (exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        getFrontend()->increaseProgress();
        QDomElement exposureElement = doc.createElement("exposure");
        takeNode.appendChild(exposureElement);
        if (!exposures[exposureIndex]->saveDataToProject(doc, exposureElement, sceneId, id, exposureIndex)) {
            qWarning("Take::saveDataFromProject --> Save exposure data failed");
            return false;
        }
    }

    if (activeExposureIndex >= 0) {
        // Save activeTakeIndex parameter after the exposures, because the names of
        // the exposures are modifyed during the writing of the project file.
        QDomElement aeiElement = doc.createElement("activeexposure");
        QDomText aeiText = doc.createTextNode(exposures[activeExposureIndex]->getId());
        aeiElement.appendChild(aeiText);
        takeNode.appendChild(aeiElement);

        // Save nextExposureIndex parameter.
        QDomElement leiElement = doc.createElement("nextexposure");
        QDomText leiText = doc.createTextNode(QString("%1").arg(exposureSize));
        leiElement.appendChild(leiText);
        takeNode.appendChild(leiElement);
    }

    // Sounds
    if (!saveSoundToProject(doc, takeNode)) {
        qWarning("Take::saveDataFromProject --> Save sound data failed");
        return false;
    }

    this->isProjectFile = true;

    qDebug("Take::saveDataFromProject --> End");
    return true;
}


const QString Take::getAppTempDirName() const
{
    return parent->getAppTempDirName();
}


const QString Take::getAppTrashDirName() const
{
    return parent->getAppTrashDirName();
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int Take::getActiveExposureIndex() const
{
    return activeExposureIndex;
}


int Take::getExposureIndex(Exposure *exposure)
{
    int index;

    index = exposures.indexOf(exposure);

    return index;
}


void Take::setActiveExposure(Exposure *newExposure)
{
    qDebug("Take::setActiveExposure --> Start");

    // Check the active take index and active scene index
    parent->setActiveTake(this);

    // Set the new active exposure
    int exposureSize = exposures.size();
    int exposureIndex;

    for (exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        if (exposures[exposureIndex] == newExposure) {
            break;
        }
    }

    Q_ASSERT(exposureIndex != exposureSize);

    if (exposureIndex != activeExposureIndex) {
        this->activeExposureIndex = exposureIndex;
        this->activeExposureId.clear();
        this->activeExposureId.append(this->getExposure(this->activeExposureIndex)->getId());
    }

    qDebug("Take::setActiveExposure --> End");
}


void Take::setActiveExposureIndex(int exposureIndex)
{
    qDebug("Take::setActiveExposureIndex --> Start");

    // The new exposure index can be the same as the active exposure index
    // if the active exposure is removed from the exposure vector
    // Q_ASSERT(exposureIndex != activeExposureIndex);

    this->activeExposureIndex = exposureIndex;
    this->activeExposureId.clear();
    if (0 <= exposureIndex) {
        this->activeExposureId.append(this->getExposure(this->activeExposureIndex)->getId());
    }

    qDebug("Take::setActiveExposureIndex --> End");
}


void Take::getExposures(QVector<Exposure*>& allExposures)
{
    unsigned int exposureSize = exposures.size();
    for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        allExposures.append(exposures[exposureIndex]);
    }
}


unsigned int Take::getExposureSize() const
{
    return exposures.size();
}


Exposure* Take::getExposure(unsigned int exposureNumber)
{
    return exposures[exposureNumber];
}


Exposure* Take::addExposure(const QString &exposureName)
{
    qDebug("Take::addExposure --> Start");

    Exposure *newExposure = new Exposure(this);

    newExposure->setId(QString("%1").arg(nextExposureIndex, 4, 10, QChar('0')));
    nextExposureIndex++;

    newExposure->copyToTemp(exposureName);
    exposures.append(newExposure);

    qDebug("Take::addExposure --> End");

    return newExposure;
}


Exposure* Take::insertExposure(const QString &exposureName, int index)
{
    qDebug("Take::insertExposure --> Start");

    Exposure *newExposure = new Exposure(this);

    newExposure->setId(QString("%1").arg(nextExposureIndex, 4, 10, QChar('0')));
    nextExposureIndex++;

    newExposure->copyToTemp(exposureName);
    if (index < 0) {
        exposures.append(newExposure);
    }
    else {
        if (index >= exposures.size()) {
            exposures.append(newExposure);
        }
        else {
            exposures.insert(index, newExposure);
        }
    }

    qDebug("Take::insertExposure --> End");

    return newExposure;
}


const QVector<Exposure*> Take::addExposures(const QVector<QString> &exposureNames, int index,
                                            Frontend *frontend, unsigned int &numberOfCanceledExposures)
{
    qDebug("Take::addExposures --> Start");

    QVector<Exposure*> newExposures;
    bool isImportingAborted = false;

    unsigned i = 0;
    unsigned numElem = exposureNames.size();

    if (exposureNames.size() == 1) {
        QString file = exposureNames[i];
        QFileInfo fileInfo(file);
        if (fileInfo.isReadable()) {
            newExposures.append(insertExposure(file, index));
        } else {
            QString msg(tr("You do not have permission to read that file"));
            frontend->showWarning(tr("Add Exposure"), msg);

            this->getAnimationProject()->setUnsavedChanges();

            return newExposures;
        }
    } else {
        QString msg(tr("Importing exposures from disk ..."));
        frontend->showProgress(msg.toLatin1().constData(), numElem * 2);

        unsigned numNotReadable = 0;
        for (; i < numElem; ++i) {
            frontend->updateProgress(i);
            QString file = exposureNames[i];
            QFileInfo fileInfo(file);
            if (fileInfo.isReadable()) {
                newExposures.append(insertExposure(exposureNames[i], index));
            } else {
                qWarning() << "Wrong permission: " << file;
                ++numNotReadable;
            }

            // Doesn't want to process events for each new exposure added.
            if ((i % 10) == 0) {
                frontend->processEvents();
            }
            if (frontend->isOperationAborted()) {
                isImportingAborted = true;
                ++i; // does this to make cleaning of exposures general
                break;
            }
        }
        frontend->updateProgress(numElem);

        if (isImportingAborted) {
            numberOfCanceledExposures = i;
            newExposures.clear();
        } else {
            numberOfCanceledExposures = numElem;
        }

        if (numNotReadable > 0) {
            QString ss = QString("%1%2%3")
                         .arg(tr("You do not have permission to read "))
                         .arg(numNotReadable)
                         .arg(tr(" file(s)"));
            frontend->showWarning(tr("Add Exposure"), ss);
        }
    }

    this->getAnimationProject()->setUnsavedChanges();

    qDebug("Take::addExposures --> End");
    return newExposures;
}


void Take::cleanExposures(unsigned int fromExposure, unsigned int toExposure)
{
    exposures.erase(exposures.begin() + fromExposure, exposures.begin() + toExposure);
}


void Take::addSavedExposure(Exposure *f)
{
    exposures.append(f);
}


void Take::moveExposures(unsigned int fromExposure,
                         unsigned int toExposure,
                         unsigned int movePosition)
{
    if (movePosition < fromExposure) {
        for (unsigned int i = fromExposure, j = movePosition; i <= toExposure; ++i, ++j) {
            Exposure *exposure = exposures[i];
            exposures.erase(exposures.begin() + i);
            exposures.insert(exposures.begin() + j, exposure);
        }
    } else {
        for (unsigned int i = fromExposure; i <= toExposure; ++i) {
            Exposure *exposure = exposures[fromExposure];
            exposures.erase(exposures.begin() + fromExposure);
            exposures.insert(exposures.begin() + movePosition, exposure);
        }
    }

    return;
}


const QVector<Exposure*> Take::removeExposures(unsigned int fromExposure,
                                               const unsigned int toExposure)
{
    QVector<Exposure*> newExposures;

    if (toExposure < (unsigned int)exposures.size()) {
        for (unsigned int i = fromExposure; i <= toExposure; ++i) {
            Exposure *exposure = exposures[fromExposure];
            exposure->moveToTrash();
            newExposures.append(exposure);
            exposures.remove(fromExposure);
        }
    }

    return newExposures;
}


Exposure* Take::removeActiveExposure()
{
    unsigned int  exposureSize = exposures.size();
    Exposure     *exposure;

    exposure = exposures[activeExposureIndex];
    exposures.remove(activeExposureIndex);
    if (activeExposureIndex == (exposureSize - 1)) {
        // It is the last exposure in the vector
        this->setActiveExposureIndex(activeExposureIndex - 1);
    }
    else {
        // It is a exposure in the middle of the vector
        // Selecting of the new exposure necessary
        this->setActiveExposureIndex(activeExposureIndex);
    }

    return exposure;
}


Exposure* Take::removeExposure(unsigned int exposureIndex)
{
    unsigned int  exposureSize = exposures.size();
    Exposure     *exposure;

    exposure = exposures[exposureIndex];
    exposures.remove(exposureIndex);
    if (exposureIndex == this->activeExposureIndex) {
        if (exposureIndex == (exposureSize - 1)) {
            // It is the last exposure in the vector
            this->setActiveExposureIndex(exposureIndex - 1);
        }
        else {
            // It is a exposure in the middle of the vector
            // Selecting of the new exposure necessary
            this->setActiveExposureIndex(exposureIndex);
        }
    }

    return exposure;
}


QVector<QString> Take::getImagePaths() const
{
    QVector<QString> imagePaths;
    unsigned int exposureSize = exposures.size();

    for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
        imagePaths.push_back(exposures[exposureIndex]->getImagePath());
    }
    return imagePaths;
}


/**************************************************************************
 * Sound functions
 **************************************************************************/

unsigned int Take::getSoundStartPosition() const
{
    qDebug("Take::getSountPosition --> Start");

    return soundStartPosition;
}


void Take::setSoundStartPosition(unsigned int newSoundStartPosition)
{
    soundStartPosition = newSoundStartPosition;
}


bool Take::readSoundFromProject(QDomElement &soundNode)
{
    qDebug("Take::readSoundFromProject --> Start");

    QDomElement currElement = soundNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        if (nodeName.compare("offset") == 0) {
            // The node is a offset value
            QString tmp = currElement.text();
            soundStartPosition = tmp.toInt();
        }

        currElement = currElement.nextSiblingElement();
    }

    qDebug("Take::readSoundFromProject --> End");
    return true;
}


bool Take::saveSoundToProject(QDomDocument &/*doc*/,
                              QDomElement &/*takeNode*/)
{
    qDebug("Take::saveSoundFromProject --> Start");
    // Save the node offset value
    
    qDebug("Take::saveSoundFromProject --> End");
    return true;
}

/**************************************************************************
 * Private functions
 **************************************************************************/
/*
void Take::activateExposure()
{
    qDebug("Take::activateExposure --> Start");

    parent->getParent()->notifyActivateExposure();

    qDebug("Take::activateExposure --> End");
}
*/
