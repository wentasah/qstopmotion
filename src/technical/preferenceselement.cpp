/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include "preferenceselement.h"

#include "technical/grabber/imagegrabber.h"
#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QFile>
#include <QtCore/QtGlobal>
#include <QtCore/QTextStream>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomText>


PreferencesElement::PreferencesElement(const QString &n, QDomElement &d)
{
    elementName.clear();
    elementName.append(n);
    elementDom = d;
}


PreferencesElement::~PreferencesElement()
{
}


const QString PreferencesElement::getName() {
    return elementName;
}


bool PreferencesElement::setStringPreference(const QString &key, const QString &attribute)
{
    QDomElement element = findPreferencesNode(key);

    if (element.isNull()) {
        element = createElement(key);
        elementDom.appendChild(element);
    } else {
        // Remove all existing text nodes
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            if (node.isText())
                element.removeChild(node);
            node = node.nextSibling();
        }
    }

    QDomText newText = createTextNode(attribute);
    element.appendChild(newText);

    return true;
}


bool PreferencesElement::getStringPreference(const QString &key, QString &value)
{
    QDomElement node = findPreferencesNode(key);

    if (!node.isNull()) {
        value.clear();
        value.append(node.text());
        return true;
    }

    return false;
}


bool PreferencesElement::setIntegerPreference(const QString &key, const int attribute)
{
    QDomElement element = findPreferencesNode(key);

    if (element.isNull()) {
        element = createElement(key);
        elementDom.appendChild(element);
    } else {
        // Remove all existing text nodes
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            if (node.isText())
                element.removeChild(node);
            node = node.nextSibling();
        }
    }

    QDomText newText = createTextNode(QString("%1").arg(attribute));
    element.appendChild(newText);

    return true;
}


bool PreferencesElement::getIntegerPreference(const QString &key, int &value)
{
    QDomElement node = findPreferencesNode(key);

    if (!node.isNull()) {
        QString tmp = node.text();
        value = tmp.toInt();
        return true;
    }

    return false;
}


void PreferencesElement::removePreference(const QString &key)
{
    QDomElement keyNode = findPreferencesNode(key);
    if (!keyNode.isNull()) {
        QDomElement parent = keyNode.parentNode().toElement();
        parent.removeChild(keyNode);
    }
}


QDomElement PreferencesElement::findPreferencesNode(const QString &key)
{
    //Search through the preferences for the element with a key which
    //equals the key parameter.
    QDomElement element = elementDom.firstChildElement();

    while (!element.isNull()) {
        QString elementName = element.nodeName();
        if (elementName.compare(key) == 0)
            break;
        element = element.nextSiblingElement();
    }

    return element;
}


QDomElement PreferencesElement::createElement(const QString &key)
{
    QDomNode parent = elementDom.parentNode();

    while (!parent.isDocument()) {
        parent = parent.parentNode();
    }
    return parent.toDocument().createElement(key);
}


QDomText PreferencesElement::createTextNode(const QString &attribute)
{
    QDomNode parent = elementDom.parentNode();

    while (!parent.isDocument()) {
        parent = parent.parentNode();
    }
    return parent.toDocument().createTextNode(attribute);
}
