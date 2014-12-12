/******************************************************************************
 *  Copyright (C) 2005-2014 by                                                *
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

#include "technical/grabber/imagegrabber.h"
#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QFile>
#include <QtCore/QtGlobal>
#include <QtCore/QTextStream>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomText>


const QString PreferencesTool::applicationName = QLatin1String("qstopmotion");
const QString PreferencesTool::applicationVersion = QLatin1String("2.0.0 (Beta)");
const QString PreferencesTool::preferencesVersion = QLatin1String("0.9");
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

const QString PreferencesTool::jpegSuffix = QLatin1String("jpg");
const QString PreferencesTool::tiffSuffix = QLatin1String("tif");
const QString PreferencesTool::bmpSuffix = QLatin1String("bmp");


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
    PreferencesElement *element;

    cleanTree();
    if (doc != NULL) {
        delete(doc);
        doc = NULL;
    }
    for (int elementIndex = 0 ; elementIndex < elements.count() ; elementIndex++) {
        element = elements[elementIndex];
        delete(element);
        elements[elementIndex] = NULL;
    }
}

bool PreferencesTool::setPreferencesFile(const QString &filePath, const QString &newVersion)
{
    qDebug("PreferencesTool::setPreferencesFile --> Start");

    QDomElement  element;
    PreferencesElement *prefElement;
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
            element = element.nextSiblingElement();
            continue;
        }
        if (element.nodeName().compare("projects") == 0) {
            projectsElement = element;
            element = element.nextSiblingElement();
            continue;
        }
        // No special element, add to preferences element vector
        prefElement = new PreferencesElement(element.nodeName(), element);
        elements.append(prefElement);

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

    if (currentVersion.compare(newVersion) != 0) {
        //The version in the file is wrong
        version.clear();
        version.append(currentVersion);
        goto FileCorrupt;
    }

    if (findPreferencesElement("preferences") == NULL) {
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
    // preferencesElement = doc->createElement("preferences");
    // rootElement.appendChild(preferencesElement);
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


const QString PreferencesTool::getVersion()
{
    return version;
}


bool PreferencesTool::setStringPreference(const QString &name, const QString &key, const QString &attribute, bool flushLater)
{
    checkInitialized();
    PreferencesElement *element = findPreferencesElement(name);

    if (element == NULL) {
        QDomElement elementDom = doc->createElement(name);
        rootElement.appendChild(elementDom);
        element = new PreferencesElement(name, elementDom);
        elements.append(element);
    }
    else {
        element->setStringPreference(key, attribute);
    }

    if (!flushLater) {
        flushPreferences();
    }

    return true;
}


bool PreferencesTool::getStringPreference(const QString &name, const QString &key, QString &value)
{
    checkInitialized();
    PreferencesElement *element = findPreferencesElement(name);

    if (element != NULL) {
        return element->getStringPreference(key, value);
    }

    return false;
}


bool PreferencesTool::setIntegerPreference(const QString &name, const QString &key, const int attribute, bool flushLater)
{
    checkInitialized();
    PreferencesElement *element = findPreferencesElement(name);

    if (element == NULL) {
        QDomElement elementDom = doc->createElement(name);
        rootElement.appendChild(elementDom);
        element = new PreferencesElement(name, elementDom);
        elements.append(element);
    }
    else {
        element->setIntegerPreference(key, attribute);
    }

    if (!flushLater) {
        flushPreferences();
    }

    return true;
}


bool PreferencesTool::getIntegerPreference(const QString &name, const QString &key, int &value)
{
    checkInitialized();
    PreferencesElement *element = findPreferencesElement(name);

    if (element != NULL) {
        return element->getIntegerPreference(key, value);
    }

    return false;
}


void PreferencesTool::removePreference(const QString &name, const QString &key)
{
    checkInitialized();
    PreferencesElement *element = findPreferencesElement(name);

    if (element != NULL) {
        element->removePreference(key);
        flushPreferences();
    }
}


PreferencesElement* PreferencesTool::findPreferencesElement(const QString &name)
{
    // Search through the preference elements for the element with name which
    // equals the name parameter.
    PreferencesElement* element;

    for (int elementIndex = 0 ; elementIndex < elements.count() ; elementIndex++) {
        element = elements[elementIndex];
        if (element->getName().compare(name) == 0) {
            return element;
        }
    }
    return NULL;
}


void PreferencesTool::setBasicPreferenceDefaults()
{
    // General defaults
    setStringPreference("preferences", "language", "en");
    setIntegerPreference("preferences", "capturebutton", captureButtonAfter);
    // Project defaults
    setIntegerPreference("preferences", "defaultrecordingmode", 0);
    setIntegerPreference("preferences", "defaultgrabbersource", 0);
    setIntegerPreference("preferences", "defaultmixmode", 0);
    setIntegerPreference("preferences", "defaultmixcount", 2);
    setIntegerPreference("preferences", "defaultplaybackcount", 5);
    // Image import defaults
    setIntegerPreference("preferences", "defaultimageformat", ImageGrabber::jpegFormat);
    setIntegerPreference("preferences", "defaultimagesize", ImageGrabber::defaultSize);
    setIntegerPreference("preferences", "defaulttransformation", true);
    setIntegerPreference("preferences", "defaultimageadjustment", ImageGrabber::centerDown);
    setIntegerPreference("preferences", "defaulliveviewfps", 20);
    // Video export defaults
    setIntegerPreference("preferences", "defaultencoderapplication", VideoEncoder::noneApplication);
    setIntegerPreference("preferences", "defaultvideoformat", VideoEncoder::mp4Format);
    setIntegerPreference("preferences", "defaultvideosize", VideoEncoder::defaultSize);
    setIntegerPreference("preferences", "defaultvideofps", 12);
    setIntegerPreference("preferences", "defaultusedefaultoutputfile", false);
    // Grabber defaults
    setIntegerPreference("preferences", "gstreamervideotestgrabber", false);
    setIntegerPreference("preferences", "gstreamerv4l2grabber", true);
    setIntegerPreference("preferences", "gstreamerv4l2controller", false);
    setIntegerPreference("preferences", "gstreamerdv1394grabber", false);
    setIntegerPreference("preferences", "gstreamerdirectshowusbgrabber", true);
    setIntegerPreference("preferences", "mediafoundationgrabber", true);
    setIntegerPreference("preferences", "mediafoundationcontroller", false);
    setIntegerPreference("preferences", "gphoto2grabber", false);
    setIntegerPreference("preferences", "gphoto2controller", false);
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
    PreferencesElement *element;

    if (doc != NULL) {
        // rootElement = NULL;
        // versionElement = NULL;
        // projectsElement = NULL;
        for (int elementIndex = 0 ; elementIndex < elements.count() ; elementIndex++) {
            element = elements[elementIndex];
            delete(element);
            elements[elementIndex] = NULL;
        }
        elements.clear();
        doc->clear();
    }
}
