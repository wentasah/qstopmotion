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

#include "projectserializer.h"

#include "domain/domainfacade.h"
#include "technical/preferencestool.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocumentType>
#include <QtXml/QDomImplementation>


ProjectSerializer::ProjectSerializer(Frontend* f)
{
    frontend = f;

    //Create the xml tree
    QDomImplementation  domImpl;
    QDomDocumentType    dtd     = domImpl.createDocumentType("qsmd",
                                  "-//qStopMotion//DTD QSMD 1.0//EN",
                                  "http://www.qstopmotion.org/xmltemplates/qsmd1.dtd");

    doc = QDomDocument(dtd);
    // rootNode = NULL;

    cleanup();
}


ProjectSerializer::~ProjectSerializer()
{
    doc.clear();
    versionElement.clear();
    animationElement.clear();
    cleanup();
}


bool ProjectSerializer::read()
{
    qDebug("ProjectSerializer::read --> Start");

    QString      errorStr;
    int          errorLine;
    int          errorColumn;
    QDomElement  rootElement;
    QDomElement  element;

    QFile file(newProjectFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        frontend->showWarning(tr("DOM Parser"),
                              QString(tr("Couldn't open XML file:\n%1")).arg(newProjectFilePath));
        return false;
    }
    if (!doc.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        file.close();
        frontend->showWarning(tr("DOM Parser"),
                              QString(tr("Parse error at line %1, column %2:\n%3\n%4"))
                              .arg(errorLine)
                              .arg(errorColumn)
                              .arg(errorStr)
                              .arg(newProjectFilePath));
        return false;
    }
    file.close();

    rootElement = doc.documentElement();

    element = rootElement.firstChildElement();
    while (!element.isNull()) {
        if (element.nodeName().compare("version") == 0) {
            versionElement = element;
        }
        if (element.nodeName().compare("settings") == 0) {
            settingsElement = element;
        }
        if (element.nodeName().compare("animation") == 0) {
            animationElement = element;
        }
        element = element.nextSiblingElement();
    }
    if (versionElement.isNull()) {
        qWarning("ProjectSerializer::read --> Error while parsing project file");
        return false;
    }

    qDebug("ProjectSerializer::read --> End");
    return true;
}


// check if the user wants to save an opened project to an another file.
bool ProjectSerializer::save(AnimationProject *animation, bool saveAs)
{
    qDebug("ProjectSerializer::save --> Start");

    bool isSaved = FALSE;
    QDomElement rootElement;
    QDomElement versionElement;
    // QDomImplementation *domImpl = new QDomImplementation();
    // QDomDocumentType    dtd     = domImpl->createDocumentType("smil",
    //                                                        "-//W3C//DTD SMIL 2.0//EN",
    //                                                        "http://www.w3.org/2001/SMIL20/SMIL20.dtd");

    doc.clear();
    // doc = QDomDocument(dtd);

    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    rootElement = doc.createElement("qsmd");
    rootElement.setAttribute("xmlns", "http://www.qstopmotion.org/xmltemplates/Language");
    rootElement.setAttribute("xml:lang", "en");
    rootElement.setAttribute("title", "qStopMotion");
    doc.appendChild(rootElement);
    versionElement = doc.createElement("version");
    QDomText versionText = doc.createTextNode("1.0");
    versionElement.appendChild(versionText);
    rootElement.appendChild(versionElement);

    // Save settings
    settingsElement = doc.createElement("settings");
    rootElement.appendChild(settingsElement);

    animation->saveSettingsToProject(doc, settingsElement);

    // Save animation
    animationElement = doc.createElement("animation");
    rootElement.appendChild(animationElement);

    if (saveAs) {
        animation->saveAsScenesToProject(doc, animationElement);
    }
    else {
        animation->saveScenesToProject(doc, animationElement);
    }

    if (QFile::exists(newProjectFilePath)) {
        QString backup(newProjectFilePath + PreferencesTool::backupSuffix);
        if (QFile::exists(backup)) {
            if (!QFile::remove(backup)) {
                // Not successful
               frontend->showCritical(tr("Critical"),
                                      tr("Can't remove old backup of project file!"));
            }
        }
        if (!QFile::rename(newProjectFilePath, backup)) {
            // Not successful
            frontend->showCritical(tr("Critical"),
                                   tr("Can't rename project file to backup!"));
        }
    }
    QFile file(newProjectFilePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        qWarning("ProjectSerializer::save --> Can't open project file");
        return isSaved;
    }
    QTextStream out(&file);
    doc.save(out, 2);
    out.flush();
    file.close();
    isSaved = TRUE;

    unsigned int numElem = animation->getTotalExposureSize();
    animation->getFrontend()->updateProgress(numElem);
    animation->getFrontend()->hideProgress();

    qDebug("ProjectSerializer::save --> End");
    return isSaved;
}

/*
void ProjectSerializer::saveDOMToFile(const QDomDocument &doc)
{
    QFile       file(projectFilePath);
    QTextStream out(&file);
    const int   Ident = 4;

    doc.save(out, Ident);
}
*/

void ProjectSerializer::setNewProjectFilePath(const QString &pfp)
{
    Q_ASSERT(!pfp.isEmpty());

    if(newProjectFilePath.compare(pfp) == 0) {
        // No new file path
        return;
    }

    // This is a new project file name

    oldProjectFilePath.append(newProjectFilePath);
    newProjectFilePath.clear();
    oldArchiveFilePath.append(newArchiveFilePath);
    newArchiveFilePath.clear();
    oldProjectPath.append(newProjectPath);
    newProjectPath.clear();
    oldImagePath.append(newImagePath);
    newImagePath.clear();
    oldSoundPath.append(newSoundPath);
    newSoundPath.clear();

    QString rootDir;
    QFileInfo projectFileInfo(pfp);

    // Get the absolute path of the project file
    rootDir.append(projectFileInfo.absoluteDir().absolutePath());

    newProjectPath.append(rootDir);

    newProjectFilePath.append(pfp);

    newArchiveFilePath.append(pfp);
    newArchiveFilePath.replace(PreferencesTool::projectSuffix, PreferencesTool::archiveSuffix);

    newImagePath.append(QString("%1/%2")
                        .arg(newProjectPath)
                        .arg(PreferencesTool::imageDirectory));

    newSoundPath.append(QString("%1/%2")
                        .arg(newProjectPath)
                        .arg(PreferencesTool::soundDirectory));

    if (!QFile::exists(newProjectPath)) {
        if (!QDir::root().mkpath(newProjectPath)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't create project directory!"));
        }
        if (!QFile::setPermissions(newProjectPath, QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't change permissions of the project directory!"));
        }
    }
    if (!QFile::exists(newImagePath)) {
        if (!QDir::root().mkpath(newImagePath)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't create image directory!"));
        }
        // Set permissions to 0755
        if (!QFile::setPermissions(newImagePath, QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't change permissions of the image directory!"));
        }
    }
    if (!QFile::exists(newSoundPath)) {
        if (!QDir::root().mkpath(newSoundPath)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't create sound directory!"));
        }
        if (!QFile::setPermissions(newSoundPath, QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther)) {
            this->frontend->showCritical(tr("Critical"),
                                         tr("Can't change permissions of the sound directory!"));
        }
    }
}


const QString ProjectSerializer::getNewProjectFilePath() const
{
    return newProjectFilePath;
}


const QString ProjectSerializer::getNewProjectPath() const
{
    return newProjectPath;
}


const QString ProjectSerializer::getNewImagePath() const
{
    return newImagePath;
}


const QString ProjectSerializer::getOldImagePath() const
{
    if (!oldImagePath.isEmpty()) {
        return oldImagePath;
    }
    return newImagePath;
}


const QString ProjectSerializer::getNewSoundPath() const
{
    return newSoundPath;
}


const QString ProjectSerializer::getOldSoundPath() const
{
    if (!oldSoundPath.isEmpty()) {
        return oldSoundPath;
    }
    return newSoundPath;
}


QDomElement &ProjectSerializer::getAnimationElement()
{
    return animationElement;
}


QDomElement &ProjectSerializer::getSettingsElement()
{
    return settingsElement;
}


const QDomElement ProjectSerializer::getVersionElement() const
{
    return versionElement;
}


void ProjectSerializer::cleanup()
{
    newProjectFilePath.clear();
    newArchiveFilePath.clear();

    newProjectPath.clear();
    newImagePath.clear();
    newSoundPath.clear();

    cleanupOldFiles();
}


void ProjectSerializer::cleanupOldFiles()
{
    oldProjectFilePath.clear();
    oldArchiveFilePath.clear();

    oldProjectPath.clear();
    oldImagePath.clear();
    oldSoundPath.clear();
}

