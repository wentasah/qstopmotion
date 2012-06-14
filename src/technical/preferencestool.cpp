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

#include "preferencestool.h"

#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QFile>
#include <QtCore/QtGlobal>
#include <QtCore/QTextStream>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomText>


const QString PreferencesTool::applicationName = QLatin1String("qstopmotion");
const QString PreferencesTool::applicationVersion = QLatin1String("1.0.1");
const QString PreferencesTool::preferencesSuffix = QLatin1String("qsmp");
const QString PreferencesTool::preferencesName = QLatin1String("preferences");
const QString PreferencesTool::projectSuffix = QLatin1String("qsmd");
const QString PreferencesTool::archiveSuffix = QLatin1String("qsma");
const QString PreferencesTool::backupSuffix = QLatin1String("old");
const QString PreferencesTool::imageDirectory = QLatin1String("images");
const QString PreferencesTool::soundDirectory = QLatin1String("sounds");
const QString PreferencesTool::tempDirectory = QLatin1String("tmp");
const QString PreferencesTool::applicationDirectory = QLatin1String(".qstopmotion");
const QString PreferencesTool::manualDirectory = QLatin1String("manual");
const QString PreferencesTool::translationsDirectory = QLatin1String("translations");
const QString PreferencesTool::graphicsDirectory = QLatin1String("graphics");
const QString PreferencesTool::iconsDirectory = QLatin1String("icons");
const QString PreferencesTool::picturesDirectory = QLatin1String("pictures");
const QString PreferencesTool::historyFileName = QLatin1String(".history");


PreferencesTool::PreferencesTool(Frontend *f)
{
    frontend = f;

    //Create the xml tree
    QDomImplementation  domImpl;
    QDomDocumentType    dtd     = domImpl.createDocumentType("smil",
                                  "-//W3C//DTD SMIL 2.0//EN",
                                  "http://www.w3.org/2001/SMIL20/SMIL20.dtd");

    doc = new QDomDocument(dtd);
}


PreferencesTool::~PreferencesTool()
{
    cleanTree();
    if (doc != NULL) {
        delete(doc);
        doc = NULL;
    }
}

bool PreferencesTool::setPreferencesFile(const QString &filePath, const QString &version)
{
    qDebug("PreferencesTool::setPreferencesFile --> Start");

    QDomElement  rootElement;
    QDomElement  element;
    QString      currentVersion;
    QString      errorStr;
    int          errorLine;
    int          errorColumn;

    //Deepcopies the path
    preferencesFile.clear();
    preferencesFile.append(filePath);
    QFile file(filePath);

    if (!file.exists()) {
        goto FileCorrupt;
    }

    cleanTree();

    //Parse the xml file as an xml tree
    if (!file.open(QIODevice::ReadOnly))
        frontend->showWarning(tr("DOM Parser"),
                              QString(tr("Couldn't open XML file:\n%1")).arg(filePath));
    if (!doc->setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        file.close();
        frontend->showWarning(tr("DOM Parser"),
                              QString(tr("Parse error at line %1, column %2:\n%3\n%4"))
                              .arg(errorLine)
                              .arg(errorColumn)
                              .arg(errorStr)
                              .arg(filePath));
    }
    file.close();

    rootElement = doc->documentElement();

    element = rootElement.firstChildElement();
    while (!element.isNull()) {
        if (element.nodeName().compare("version") == 0) {
            versionElement = element;
        }
        if (element.nodeName().compare("preferences") == 0) {
            preferencesElement = element;
        }
        if (element.nodeName().compare("encoders") == 0) {
            encodersElement = element;
        }
        if (element.nodeName().compare("projects") == 0) {
            projectsElement = element;
        }
        element = element.nextSiblingElement();
    }
    if (versionElement.isNull()) {
        qWarning("PreferencesTool::setPreferencesFile --> Error while parsing preferences file");
        goto FileCorrupt;
    }
    else {
        currentVersion = versionElement.text();
    }

    //There are no version in the file
    if (currentVersion.isNull()) {
        goto FileCorrupt;
    }

    if (currentVersion.compare(version) != 0) {
        //The version in the file is wrong
        oldVersion.clear();;
        oldVersion.append(currentVersion);
        goto FileCorrupt;
    }

    if (preferencesElement.isNull()) {
        qWarning("PreferencesTool::setPreferencesFile --> Error while parsing preferences file");
        goto FileCorrupt;
    }
    return true;

FileCorrupt:
    cleanTree();

    // If there are no file, no version or if the version is wrong a new preferences
    // file are made.
    QDomNode xmlNode = doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
    doc->insertBefore(xmlNode, doc->firstChild());

    rootElement = doc->createElement("root");
    // rootElement.setAttribute("xmlns", "http://www.w3.org/2001/SMIL20/Language");
    rootElement.setAttribute("xml:lang", "en");
    rootElement.setAttribute("title", "preferences");
    doc->appendChild(rootElement);

    versionElement = doc->createElement("version");
    rootElement.appendChild(versionElement);
    // versionElement.setAttribute("version", version);
    QDomText versionText = doc->createTextNode(version);
    versionElement.appendChild(versionText);
    preferencesElement = doc->createElement("preferences");
    rootElement.appendChild(preferencesElement);
    encodersElement = doc->createElement("encoders");
    rootElement.appendChild(encodersElement);
    projectsElement = doc->createElement("projects");
    rootElement.appendChild(projectsElement);

    flushPreferences();

    qDebug("PreferencesTool::setPreferencesFile --> End");

    return false;
}


void PreferencesTool::setVersion(const QString &version)
{
    checkInitialized();

    QDomText versionText = doc->createTextNode(version);
    versionElement.appendChild(versionText);

    flushPreferences();
}


const QString PreferencesTool::getOldVersion()
{
    return oldVersion;
}


bool PreferencesTool::setBasicPreference(const QString &key, const QString &attribute, bool flushLater)
{
    checkInitialized();
    QDomElement element = findPreferencesNode(key);

    if (element.isNull()) {
        element = doc->createElement(key);
        preferencesElement.appendChild(element);
    } else {
        // Remove all existing text nodes
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            if (node.isText())
                element.removeChild(node);
            node = node.nextSibling();
        }
    }

    QDomText newText = doc->createTextNode(attribute);
    element.appendChild(newText);

    if (!flushLater)
        flushPreferences();
    return true;
}


bool PreferencesTool::setBasicPreference(const QString &key, const int attribute, bool flushLater)
{
    checkInitialized();
    QDomElement element = findPreferencesNode(key);

    if (element.isNull()) {
        element = doc->createElement(key);
        preferencesElement.appendChild(element);
    } else {
        // Remove all existing text nodes
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            if (node.isText())
                element.removeChild(node);
            node = node.nextSibling();
        }
    }

    QDomText newText = doc->createTextNode(QString("%1").arg(attribute));
    element.appendChild(newText);

    if (!flushLater)
        flushPreferences();
    return true;
}


const QString PreferencesTool::getBasicPreference(const QString &key, const QString &defaultValue)
{
    checkInitialized();
    QDomElement node = findPreferencesNode(key);
    if (!node.isNull()) {
        return node.text();
    }
    return defaultValue;
}


int PreferencesTool::getBasicPreference(const QString &key, const int defaultValue)
{
    checkInitialized();
    QDomElement node = findPreferencesNode(key);
    if (!node.isNull()) {
        QString tmp = node.text();
        return tmp.toInt();
    }
    return defaultValue;
}


void PreferencesTool::removeBasicPreference(const QString &key)
{
    checkInitialized();
    QDomElement keyNode = findPreferencesNode(key);
    if (!keyNode.isNull()) {
        QDomElement parent = keyNode.parentNode().toElement();
        parent.removeChild(keyNode);
        flushPreferences();
    }
}


QDomElement PreferencesTool::findPreferencesNode(const QString &key)
{
    //Search through the preferences for the element with a key which
    //equals the key parameter.
    QDomElement element = preferencesElement.firstChildElement();
    while (!element.isNull()) {
        QString elementName = element.nodeName();
        if (elementName.compare(key) == 0)
            break;
        element = element.nextSiblingElement();
    }
    return element;
}


void PreferencesTool::setBasicPreferenceDefaults()
{
    setBasicPreference("language", "en");
    setBasicPreference("capturebutton", captureButtonAfter);
    setBasicPreference("defaultrecordingmode", 0);
    setBasicPreference("defaultvideosource", 0);
    setBasicPreference("defaultmixmode", 0);
    setBasicPreference("defaultmixcount", 2);
    setBasicPreference("defaultplaybackcount", 5);
    setBasicPreference("defaultencoderapplication", VideoEncoder::noneApplication);
    setBasicPreference("defaultvideoformat", VideoEncoder::noneFormat);
    setBasicPreference("defaultvideosize", VideoEncoder::defaultSize);
    setBasicPreference("defaultframespersecond", 12);
    setBasicPreference("defaultusedefaultoutputfile", false);
}


void PreferencesTool::addProject(const QString &filename)
{
    QDomElement newProject;
    QDomElement newElement;
    QDomText    newText;

    checkInitialized();

    newProject = doc->createElement("project");
    // newProject.setAttribute("id", id);

    newElement = doc->createElement("file");
    newText = doc->createTextNode(filename);
    newElement.appendChild(newText);
    newProject.appendChild(newElement);

    // Insert the new project as first project in the list
    projectsElement.insertBefore(newProject, projectsElement.firstChild());

}


const QString PreferencesTool::getProject(int index)
{
    int runIndex = -1;

    checkInitialized();

    QDomElement project = projectsElement.firstChildElement();
    while (!project.isNull()) {
        QString nodeName = project.nodeName();
        if (nodeName.compare(QString("project")) == 0) {
            runIndex++;
            if (runIndex != index) {
                project = project.nextSiblingElement();
                continue;
            }
            QDomElement file = project.firstChildElement();
            while (!file.isNull()) {
                QString fileName = file.nodeName();
                if (fileName.compare(QString("file")) == 0)
                    return(file.text());
                file = file.nextSiblingElement();
            }
        }
        project = project.nextSiblingElement();
    }
    return(QString());
}


bool PreferencesTool::removeProject(const QString &filename)
{
    checkInitialized();

    QDomElement project = projectsElement.firstChildElement();
    while (!project.isNull()) {
        QString nodeName = project.nodeName();
        if (nodeName.compare(QString("project")) == 0) {
            QDomElement file = project.firstChildElement();
            while (!file.isNull()) {
                QString fileName = file.nodeName();
                if (fileName.compare(QString("file")) == 0) {
                    if (filename.compare(file.text()) == 0) {
                        projectsElement.removeChild(project);
                        return(true);
                    }
                }
                file = file.nextSiblingElement();
            }
        }
        project = project.nextSiblingElement();
    }
    return(false);
}


void PreferencesTool::flushPreferences()
{
    bool prefsFileExists = QFile::exists(preferencesFile);
    if (prefsFileExists == true) {
        QString oldPrefsFile(preferencesFile);
        oldPrefsFile.append(QLatin1String("."));
        oldPrefsFile.append(PreferencesTool::backupSuffix);
        if (QFile::exists(oldPrefsFile)) {
            if (!QFile::remove(oldPrefsFile)) {
                frontend->showCritical(tr("Critical"),
                                       tr("Can't remove preferences backup file!"));
            }
        }

        if (!QFile::copy(preferencesFile, oldPrefsFile)) {
            // Not successful
            frontend->showCritical(tr("Critical"),
                                   tr("Can't copy preferences file to backup file!"));
        }
    }

    QFile       file(preferencesFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    const int   Ident = 4;

    doc->save(out, Ident);

    out.flush();
    file.close();
}


bool PreferencesTool::fileExists(const QString &filePath)
{
    if (QFile::exists(filePath)) {
        return true;
    }
    return false;
}


void PreferencesTool::checkInitialized()
{
    if (doc == NULL) {
        qWarning("PreferencesTool::setPreferencesFile --> A preferencesfile has to be specified before using the PreferencesTool.");
        exit(1);
    }
}


void PreferencesTool::cleanTree()
{
    if (doc != NULL)
        doc->clear();
}
