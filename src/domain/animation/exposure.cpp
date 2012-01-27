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

#include "exposure.h"

#include "domain/animation/projectserializer.h"
#include "technical/util.h"
#include "technical/preferencestool.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QtDebug>


unsigned int Exposure::tempNum  = 0;
unsigned int Exposure::trashNum = 0;
// QString Exposure::tempPath   = {0};
// QString Exposure::trashPath[256]  = {0};


Exposure::Exposure(Take *take)
{
    parent = take;

    theFrame.clear();

    exposureLocation = AnimationProject::NoFile;

    id.clear();
}


Exposure::Exposure(Exposure *exposure)
{
    parent = exposure->parent;

    theFrame.clear();
    theFrame.append(exposure->theFrame);
    Q_ASSERT(!theFrame.isEmpty());

    exposureLocation = exposure->exposureLocation;

    id.clear();
    id.append(exposure->getId());
}


Exposure::Exposure(Take *take, const QString &fileName, int location)
{
    parent = take;

    Q_ASSERT(!fileName.isEmpty());
    theFrame.clear();
    theFrame.append(fileName);

    exposureLocation = location;

    id.clear();
}


Exposure::~Exposure()
{
}


bool Exposure::isEmpty()
{
    return theFrame.isEmpty();
}


const QString Exposure::getId() const
{
    return id;
}


void Exposure::setId(const QString &newId)
{
    if (!newId.isEmpty()) {
        id.clear();
        id.append(newId);
    }
}


Take *Exposure::getTake() const
{
    return parent;
}


unsigned int Exposure::getIndex()
{
    unsigned int index;

    index = parent->getExposureIndex(this);

    return index;
}


unsigned int Exposure::getTakeIndex() const
{
    unsigned int index;

    index = parent->getIndex();

    return index;
}


unsigned int Exposure::getSceneIndex() const
{
    unsigned int index;

    index = parent->getSceneIndex();

    return index;
}


const QString Exposure::getImagePath() const
{
    // qDebug("Exposure::getImagePath --> Start");

    Q_ASSERT(!theFrame.isEmpty());

    QString absImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        absImagePath.append(parent->getImagePath());
        break;
    case AnimationProject::InTempPath:
        absImagePath.append(parent->getAppTempDirName());
        break;
    case AnimationProject::InTrashPath:
        absImagePath.append(parent->getAppTrashDirName());
        break;
    default:
        return QString();
    }
    absImagePath.append(QLatin1String("/"));
    absImagePath.append(theFrame);

    // qDebug("Exposure::getImagePath --> End");
    return absImagePath;
}

/*
void Exposure::moveToImageDir(const QString &directory, unsigned int imgNum)
{
    Q_ASSERT(!directory.isEmpty());

    QString newPath;
    QString filename;
    QString tmp(QString("%1").arg(imgNum));

    int fileLength = tmp.length();

    // creates a filename with six characters as total length,
    // empty characters are filled with zeros
    for (int i = 0; i < 6 - fileLength; ++i) {
        filename[i] = '0';
    }
    filename.append(tmp);

    // gets a pointer to the last occurence of a '.'
    // and appends the extension
    filename.append(theFrame.mid(theFrame.lastIndexOf('.')));

    // creates new image path
    newPath.append(directory);
    newPath.append(QLatin1String("/"));
    newPath.append(filename);

    if (!QFile::rename(theFrame, newPath)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't move image to image directory!"));
    }

    theFrame.clear();
    theFrame.append(newPath);
}
*/

void Exposure::moveToTemp()
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        fromImagePath.append(parent->getImagePath());
        break;
    case AnimationProject::InTrashPath:
        fromImagePath.append(parent->getAppTrashDirName());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    // creates a new image name
    QString toImageName(QString("tmp_%1%2").arg(Exposure::tempNum)
                        .arg(fromImagePath.mid(fromImagePath.lastIndexOf('.'))));
    Exposure::tempNum++;

    toImagePath.append(parent->getAppTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    if (!QFile::rename(fromImagePath, toImagePath)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't move image to temporary directory!"));
    }

    // if (exposureLocation == InTrashPath) {
    //     Exposure::trashNum--;
    // }

    theFrame.clear();
    theFrame.append(toImageName);
    exposureLocation = AnimationProject::InTempPath;
}


void Exposure::moveToTrash()
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        fromImagePath.append(parent->getImagePath());
        break;
    case AnimationProject::InTempPath:
        fromImagePath.append(parent->getAppTempDirName());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    // creates a new image name
    QString toImageName(QString("tmp_%1%2").arg(Exposure::trashNum)
                        .arg(fromImagePath.mid(fromImagePath.lastIndexOf('.'))));
    Exposure::trashNum++;

    toImagePath.append(parent->getAppTrashDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    if (!QFile::rename(fromImagePath, toImagePath)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't move image to trash directory!"));
    }

    // if (exposureLocation == InTempPath) {
    //     Exposure::tempNum--;
    // }

    theFrame.clear();
    theFrame.append(toImageName);
    exposureLocation = AnimationProject::InTrashPath;
}


void Exposure::moveToProject(const QString newName)
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InTrashPath:
        fromImagePath.append(parent->getAppTrashDirName());
        break;
    case AnimationProject::InTempPath:
        fromImagePath.append(parent->getAppTempDirName());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    toImagePath.append(parent->getImagePath());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(newName);

    if (!QFile::rename(fromImagePath, toImagePath)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't move image to project directory!"));
    }

    switch (exposureLocation) {
    case AnimationProject::InTrashPath:
        Exposure::trashNum--;
        break;
    case AnimationProject::InTempPath:
        Exposure::tempNum--;
        break;
    default:
        break;
    }

    theFrame.clear();
    theFrame.append(newName);
    exposureLocation = AnimationProject::InProjectPath;
}

/*
void Exposure::markAsProjectFile()
{
    isProjectFile = true;
}
*/
/*
const QString Exposure::getImageId()
{
    QString tmp(theFrame);
    QString bname(tmp.mid(tmp.lastIndexOf(QLatin1String("/"))));
    QString imgID(bname.left(bname.indexOf('.')));

    return imgID;
}
*/
/*
Exposure::fileLocation Exposure::getExposureLocation()
{
    return exposureLocation;
}
*/

bool Exposure::readDataFromProject(QDomElement &exposureNode)
{
    // qDebug("Exposure::readDataFromProject --> Start");

    id.append(exposureNode.attributeNode(QString("id")).value());

    QDomElement currNode = exposureNode.firstChildElement();

    while (!currNode.isNull()) {
        QString nodeName = currNode.nodeName();
        if (nodeName.compare("theframe") == 0) {
            // The node is a 'the Frame' node
            QString filename = currNode.text();
            if (!filename.isNull()) {
                theFrame.clear();
                theFrame.append(filename);
                exposureLocation = AnimationProject::InProjectPath;
            }
        }
        else {
            // The node is a 'capture Frame' node
        }

        currNode = currNode.nextSiblingElement();
    }

    // qDebug("Exposure::readDataFromProject --> End");
    return true;
}


bool Exposure::saveDataToProject(QDomDocument &doc, QDomElement &exposureNode,
                                 const QString &/*sceneId*/, const QString &/*takeId*/,
                                 int exposureIndex)
{
    qDebug("Exposure::saveDataFromProject --> Start");

    QDomElement   theFrameElement;

    // Create a new id for the Exposure
    id.clear();
    id.append(QString("%1").arg((exposureIndex), 4, 10, QChar('0')));

    // Create a new file name for the frame

    /* This version didn't work with ffmpeg encoder
    QString newFrameName(sceneId);
    newFrameName.append('_');
    newFrameName.append(takeId);
    newFrameName.append('_');
    newFrameName.append(id);
    newFrameName.append(theFrame.mid(theFrame.lastIndexOf('.')));
    */
    int newTotalExposureIndex = parent->getAnimationProject()->getNextTotalExposureIndex();
    QString newFrameName(QString("%1").arg(newTotalExposureIndex, 6, 10, QLatin1Char('0')));
    newFrameName.append(theFrame.mid(theFrame.lastIndexOf('.')));

    // Move frame file to project directory
    this->moveToProject(newFrameName);

    // Save exposure parameter
    exposureNode.setAttribute("id", id);

    // Save the frame
    theFrameElement = doc.createElement("theframe");
    QDomText theFrameText = doc.createTextNode(theFrame);
    theFrameElement.appendChild(theFrameText);
    exposureNode.appendChild(theFrameElement);

    // Save all captured frames
    /*
    unsigned int numFrames = actScene.getSize();
    for (unsigned int j = 0; j < numFrames; ++j) {
        Frame& frame = actScene.getFrame(j);
        frame.moveToProjectDir(imagePath, soundPath, ++index);
        frame.markAsProjectFile();
        absPath.clear();
        absPath.append(frame.getImagePath());
        QString relPath(absPath.mid(absPath.lastIndexOf(QLatin1String("/")))+1);
        img = doc.createElement("img");
        imagesNode.appendChild(img);
        img.setAttribute("src", relPath);
    }
    */

    qDebug("Exposure::saveDataFromProject --> End");
    return true;
}
