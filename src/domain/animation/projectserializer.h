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

#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

// #include "domain/animation/animationproject.h"
#include "frontends/frontend.h"

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class ProjectSerializer;
class AnimationProject;

/**
 * Class for serializing the project. It creates differents directories containing
 * the files belonging to the current project. Finally everything is packed in
 * a tarball.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad.
 */
class ProjectSerializer
{
public:

    /**
     * Constructor.
     */
    ProjectSerializer(Frontend* f);

    /**
     * Destructor.
     */
    ~ProjectSerializer();

    /**
     * Opens the project file and read the data.
     * @return true if reading was sucessful, false otherwise
     */
    bool read();

    /**
     * Saves the files in differents directories.
     * @param animation the project to be saved
     * @param frontend the frontend to display progress to
     * @return true if saving was successfull, false otherwise
     */
    bool save(AnimationProject *animation);

    /**
     * Set a new the project file name.
     * @param fileName the new project file name.
     * @return true if this is a new file name.
     */
    bool setProjectFileName(const QString &fileName);

    /**
     * Retrieves the project file name.
     * @return the project path if it exist, NULL otherwise
     */
    const QString getProjectFileName() const;

    /**
     * Retrieves the absolute project path.
     * This is the path where the files within the tarball are located or
     * the path to the project directory
     * @return the project path if it exist, NULL otherwise
     */
    const QString getProjectPath() const;

    /**
     * Retrieves the absolute image path, a subdirectory of the project path.
     * This is the path where the images can be found.
     * @return the image path if it exist, NULL otherwise
     */
    const QString getImagePath() const;

    /**
     * Retrieves the absolute sound path, a subdirectory of the project path.
     * This is the path where the sound can be found.
     * @return the sound path if it exist, NULL otherwise
     */
    const QString getSoundPath() const;

    /**
     * Get the animation element of the projet tree
     * @return the animation element
     */
    QDomElement &getAnimationElement();

    /**
     * Get the settings element of the projet tree
     * @return the settings element
     */
    QDomElement &getSettingsElement();

    /**
     * Get the version element of the projet tree
     * @return the version element
     */
    const QDomElement getVersionElement() const;

    /**
     * Deletes unessecary files and pointers.
     */
    void cleanup();

private:

    /**
     * Frontend of the application.
     */
    Frontend *frontend;

    QDomDocument doc;
    // QDomElement  rootNode;
    QDomElement  versionElement;
    QDomElement  settingsElement;
    QDomElement  animationElement;

    /**
     * Absolute path to the project
     */
    QString projectPath;

    /**
     * Absolute path to the image files
     */
    QString imagePath;

    /**
     * Absolute path to the sound files
     */
    QString soundPath;

    /**
     * Absolute file name of the project file
     */
    QString projectFileName;

    /**
     * Absolute file name of the archiv file
     */
    QString archiveFileName;

    QString prevProjectPath;                 // absolute
    QString prevImgPath;                 // absolute
    QString prevArchiveFileName;             // absolute

    // void saveDOMToFile(const QDomDocument &doc);
    void cleanupPrev();
    void storeOldPaths();

    void setProjectPath(const QString &unpacked, bool isSave);

};

#endif
