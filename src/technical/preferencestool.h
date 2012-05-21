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

#ifndef PREFERENCESTOOL_H
#define PREFERENCESTOOL_H

#include <frontends/frontend.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>


/**
 * A xml based tool for adding, changing and removing of
 * various preferences. Everything is saved to a xml organized file
 * and can be readed by the tool for later usage.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class PreferencesTool : public QObject
{
    Q_OBJECT
public:

    enum captureButtonFunction {
        captureButtonBevor,
        captureButtonAfter,
        captureButtonAppend
    };

    /**
     * Name of the application
     */
    static const QString applicationName;

    /**
     * Version of the application
     */
    static const QString applicationVersion;

    /**
     * Relative to project or application directory
     */
    static const QString imageDirectory;

    /**
     * Relative to project or application directory
     */
    static const QString soundDirectory;

    /**
     * Relative to application directory
     */
    static const QString tempDirectory;

    /**
     * Relative to manual directory
     */
    static const QString manualDirectory;

    /**
     * Relative to translations directory
     */
    static const QString translationsDirectory;

    /**
     * Relative to graphics directory
     */
    static const QString graphicsDirectory;

    /**
     * Relative to icons directory
     */
    static const QString iconsDirectory;

    /**
     * Relative to pictures directory
     */
    static const QString picturesDirectory;

    /**
     * Relative to home dirctory
     */
    static const QString applicationDirectory;

    /**
     * Suffix of the property file
     */
    static const QString preferencesSuffix;

    /**
     * Name of the property file
     */
    static const QString preferencesName;

    /**
     * Suffix of the project file
     */
    static const QString projectSuffix;

    /**
     *  Suffix of the archive file
     */
    static const QString archiveSuffix;

    /**
     * Suffix of the backup file
     */
    static const QString backupSuffix;

    /**
     * Name of the history file
     */
    static const QString historyFileName;

    /**
     * Protected constructor to deny external instanciation of the singleton.
     */
    PreferencesTool(Frontend *f);

    /**
     * Cleans up after the preferencestool.
     */
    ~PreferencesTool();

    /**
     * Sets the file to store preferences in.
     * @param filePath the path to the file where the preferences are
     * stored.
     * @param version the version of the program
     * @return true if file exists. false if the file doesn't exists
     * and the preferencesTool makes it.
     */
    bool setPreferencesFile(const QString &filePath, const QString &version);

    /**
     * Sets the version of the preference file.
     * @param version the version number of the preference file
     */
    void setVersion(const QString &version);

    /**
     * Returns the version number of the preference file or 0 if
     * there are no version tag.
     * @return the version number of the preference file or 0 if
     * there are no version tag.
     */
    const QString getOldVersion();

    /**
     * Flushes the preferences to the file specified with setPreferencesFile(..).
     * @return true if the preferences were succesfully saved.
     */
    void flushPreferences();

    /**
     * Adds a string preference.
     * @param key the key for retrieving the preference.
     * @param attribute the attribute for the preference.
     * @param flushLater true if you don't want the preferencesTool to flush
     * the preferences to disk at once. This is given for to allow
     * optimalization when several preferences ar saved at the same time.
     * flushPreferences() should be called at once after the preferences are
     * set to store them to disk.
     * @return true if the preference was succesfully saved. If flushLater is
     * set to true this function will return true automaticaly.
     */
    bool setBasicPreference(const QString &key, const QString &attribute, bool flushLater = false);

    /**
     * Adds an int preference.
     * @param key the key for retrieving the preference.
     * @param attribute the attribute for the preference.
     * @param flushLater true if you don't want the preferencesTool to flush
     * the preferences to disk at once. This is given for to allow
     * optimalization when several preferences ar saved at the same time.
     * flushPreferences() should be called at once after the preferences are
     * set to store them to disk.
     * @return true if the preference was succesfully saved. If flushLater is
     * set to true this function will return true automaticaly.
     */
    bool setBasicPreference(const QString &key, const int attribute, bool flushLater = false);

    /**
     * Retrieves a string preference.
     * @param key the key of the preference to retrieve.
     * @param defaultValue a default value for preferences which aren't set
     * by the user yet.
     * @return the attribute for the given key or "defaultValue" if the key
     * wasn't found.
     */
    const QString getBasicPreference(const QString &key, const QString &defaultValue);

    /**
     * Retrieves an int preference.
     * @param key the key of the preference to retrieve.
     * @param defaultValue a default value for preferences which aren't set
     * by the user yet.
     * @return the attribute for the given key or "defaultValue" if the key
     * wasn't found.
     */
    int getBasicPreference(const QString &key, const int defaultValue);

    /**
     * Removes the preference with the key "key". (Which, in practice, means
     * setting it to default).
     * @param key the key of the preference to remove.
     */
    void removeBasicPreference(const QString &key);

    /**
     * Set the default preferences
     */
    void setBasicPreferenceDefaults();

    /**
     * Add a new path on the first position of the path list
     * @param filename Absoluth file name of the project file
     */
    void addProject(const QString &filename);

    /**
     * Get the file name of the project with the position index in the project list
     * @param index Position of the project in the project list (0....n)
     * @return Absolute file name of the project file or an empty string if a project with the index didn't exist
     */
    const QString getProject(int index);

    /**
     * Delete a project from the project list
     * @param filename Absolut file name of the project file
     * @return true if successful, false otherwise
     */
    bool removeProject(const QString &filename);

protected:

private:

    /**
     * The frontend of the application.
     */
    Frontend     *frontend;

    QDomDocument *doc;
    QDomElement   preferencesElement;
    QDomElement   versionElement;
    QDomElement   encodersElement;
    QDomElement   projectsElement;

    QString       preferencesFile;
    QString       oldVersion;

    /**
     * Retrieves the node with key "key".
     * @param key the key of the node to retrieve.
     * @return the node with the given key.
     */
    QDomElement findPreferencesNode(const QString &key);

    /**
     * Retrieves the encoder node with key "key".
     * @param key the key of the node to retrieve.
     * @return the node with the given key.
     */
    QDomElement findEncodersNode(const QString &key);

    /**
     * Retrieves the project node with key "key".
     * @param key the key of the node to retrieve.
     * @return the node with the given key.
     */
    QDomElement findProjectsNode(const QString &key);

    /**
     * Checks if the file at filePath exists.
     * @return true if the file exist.
     */
    bool fileExists(const QString &filePath);

    /**
     * Checks if the preferencestool has been initialized and exits with an error
     * if it hasn't.
     */
    void checkInitialized();

    /**
     * Cleans the xml tree.
     */
    void cleanTree();
};

#endif
