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

#include "exposure.h"

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QString>

#include "domain/animation/projectserializer.h"
#include "technical/util.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"


unsigned int Exposure::tempNumber = 0;


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
    // qDebug() << "Exposure::getNewImagePath --> Start");

    Q_ASSERT(!theFrame.isEmpty());

    QString absImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        absImagePath.append(parent->getNewImagePath());
        break;
    case AnimationProject::InTempPath:
        absImagePath.append(parent->getAppTempDirName());
        break;
    default:
        return QString();
    }
    absImagePath.append(QLatin1String("/"));
    absImagePath.append(theFrame);

    // qDebug() << "Exposure::getNewImagePath --> End");
    return absImagePath;
}


void Exposure::moveToTemp(bool isRecovery)
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        fromImagePath.append(parent->getNewImagePath());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    // creates a new image name
    QString toImageName(QString("%1%2%3")
                        .arg(QLatin1String("tmp_"))
                        .arg(Exposure::tempNumber)
                        .arg(fromImagePath.mid(fromImagePath.lastIndexOf('.'))));
    Exposure::tempNumber++;

    toImagePath.append(parent->getAppTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    if (!isRecovery) {
        if (!QFile::rename(fromImagePath, toImagePath)) {
            // Rename not successful --> copy and remove
            if (!QFile::copy(fromImagePath, toImagePath)) {
                parent->getFrontend()->showCritical(tr("Critical"),
                                                    tr("Can't copy the image to the temporary directory!"));
            }
            if (!QFile::remove(fromImagePath)) {
                parent->getFrontend()->showCritical(tr("Critical"),
                                                    tr("Can't remove the image from the project directory!"));
            }
        }
    }

    theFrame.clear();
    theFrame.append(toImageName);
    exposureLocation = AnimationProject::InTempPath;
}


void Exposure::copyToTemp()
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        fromImagePath.append(parent->getOldImagePath());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    // creates a new image name
    QString toImageName(QString("%1%2%3")
                        .arg(QLatin1String("tmp_"))
                        .arg(Exposure::tempNumber)
                        .arg(fromImagePath.mid(fromImagePath.lastIndexOf('.'))));
    Exposure::tempNumber++;

    toImagePath.append(parent->getAppTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    if (!QFile::copy(fromImagePath, toImagePath)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't copy the image to the temporary directory!"));
    }

    // if (exposureLocation == InTrashPath) {
    //     Exposure::trashNum--;
    // }

    theFrame.clear();
    theFrame.append(toImageName);
    exposureLocation = AnimationProject::InTempPath;
}


void Exposure::convertToTemp(int newFormat, int quality)
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InProjectPath:
        fromImagePath.append(parent->getOldImagePath());
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
    QString toImageName(theFrame.left(theFrame.lastIndexOf('.') + 1));  // Including the '.')

    switch(newFormat) {
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

    toImagePath.append(parent->getAppTempDirName());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(toImageName);

    // Save the image in the new file format to the temp dirctory
    QImage image(fromImagePath);

    if (!image.save(toImagePath, 0, quality)) {
        // Not successful
        parent->getFrontend()->showCritical(tr("Critical"),
                                            tr("Can't save the image in the new file format to the temporary directory!"));
    }

    theFrame.clear();
    theFrame.append(toImageName);
    exposureLocation = AnimationProject::InTempPath;
}


void Exposure::moveToProject(const QString newName)
{
    QString fromImagePath;
    QString toImagePath;

    switch (exposureLocation) {
    case AnimationProject::InTempPath:
        fromImagePath.append(parent->getAppTempDirName());
        break;
    default:
        return;
    }

    fromImagePath.append(QLatin1String("/"));
    fromImagePath.append(theFrame);

    toImagePath.append(parent->getNewImagePath());
    toImagePath.append(QLatin1String("/"));
    toImagePath.append(newName);

    if (!QFile::rename(fromImagePath, toImagePath)) {
        // Rename not successful --> copy and remove
        if (!QFile::copy(fromImagePath, toImagePath)) {
            parent->getFrontend()->showCritical(tr("Critical"),
                                                tr("Can't copy the image to the project directory!"));
        }
        if (!QFile::remove(fromImagePath)) {
            parent->getFrontend()->showCritical(tr("Critical"),
                                                tr("Can't remove the image in the temporary directory!"));
        }
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
    // qDebug() << "Exposure::readDataFromProject --> Start");

    id.append(exposureNode.attributeNode(QLatin1String("id")).value());

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

    // qDebug() << "Exposure::readDataFromProject --> End");
    return true;
}


bool Exposure::saveDataToProject(QDomDocument &doc, QDomElement &exposureNode)
{
    qDebug() << "Exposure::saveDataFromProject --> Start";

    QDomElement   theFrameElement;

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

    qDebug() << "Exposure::saveDataFromProject --> End";
    return true;
}
