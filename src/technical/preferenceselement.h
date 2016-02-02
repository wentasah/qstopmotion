/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#ifndef PREFERENCESELEMENT_H
#define PREFERENCESELEMENT_H

#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <QString>


/**
 * Preferences element.
 *
 * @author Ralf Lange
 */
class PreferencesElement : public QObject
{
    Q_OBJECT
public:

    /**
     * Constructor
     * @param n Name of the element.
     * @param d DOM Element of the element.
     */
    PreferencesElement(const QString &n, QDomElement &d);

    /**
     * Cleans up after the preferencestool.
     */
    ~PreferencesElement();

    /**
     * Get the name of the preferences element.
     * @return The name of the preferences element.
     */
    const QString getName();

    /**
     * Adds a string preference.
     * @param key The key for retrieving the preference.
     * @param attribute The attribute for the preference.
     * @return True if the preference was succesfully saved. If flushLater is
     * set to true this function will return true automaticaly.
     */
    bool setStringPreference(const QString &key, const QString &attribute);

    /**
     * Retrieves a string preference.
     * @param key The key of the preference to retrieve.
     * @param value The value of the preference.
     * @return True if the preference was succesfully readed.
     */
    bool getStringPreference(const QString &key, QString &value);

    /**
     * Adds an int preference.
     * @param key The key for retrieving the preference.
     * @param attribute The attribute for the preference.
     * @return True if the preference was succesfully saved. If flushLater is
     * set to true this function will return true automaticaly.
     */
    bool setIntegerPreference(const QString &key, const int attribute);

    /**
     * Retrieves an int preference.
     * @param key The key of the preference to retrieve.
     * @param value The value of the preference.
     * @return True if the preference was succesfully readed.
     */
    bool getIntegerPreference(const QString &key, int &value);

    /**
     * Removes the preference with the key "key". (Which, in practice, means
     * setting it to default).
     * @param key the key of the preference to remove.
     */
    void removePreference(const QString &key);

protected:

private:

    /**
     * The name of the element.
     */
    QString elementName;

    /**
     * The DOM element of the element
     */
    QDomElement elementDom;

    /**
     * Retrieves the node with key "key".
     * @param key the key of the node to retrieve.
     * @return the node with the given key.
     */
    QDomElement findPreferencesNode(const QString &key);

    /**
     * Create a element with key "key".
     * @param key the key of the element to create.
     * @return the new created element.
     */
    QDomElement createElement(const QString &key);

    /**
     * Create a text node with attribute "attribute".
     * @param attribute the attribute of the text node to create.
     * @return the new created text node.
     */
    QDomText createTextNode(const QString &attribute);
};

#endif
