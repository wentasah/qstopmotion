/******************************************************************************
 *  Copyright (C) 2010-2012 by                                                *
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

#ifndef GENERALWIDGET_H
#define GENERALWIDGET_H

#include "frontends/frontend.h"

#include <QtCore/QTranslator>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>


/**
 * The export tab in the preferences menu
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class GeneralWidget : public QWidget
{
    Q_OBJECT
public:

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    GeneralWidget(Frontend *f, QWidget *parent = 0);

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Reset the settings in the import tab.
     */
    void reset();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    /**
     * Slot for notified the application when the language changes.
     * @param index the new language.
     */
    void changeLanguage(int index);

    /**
     * Slot for notified the general tab when bevor button is selected, so that
     * general preferences can be updaten.
     */
    void setBevorButtonOn();

    /**
     * Slot for notified the general tab when after button is selected, so that
     * general preferences can be updaten.
     */
    void setAfterButtonOn();

    /**
     * Slot for notified the general tab when append button is selected, so that
     * general preferences can be updaten.
     */
    void setAppendButtonOn();

private:
    Frontend     *frontend;

    QGroupBox    *languageGroupBox;
    QComboBox    *languageCombo;
    QGroupBox    *captureGroupBox;
    QRadioButton *bevorButton;
    QRadioButton *afterButton;
    QRadioButton *appendButton;

    int           actualLanguage;
    PreferencesTool::captureButtonFunction actualButtonFunction;

};

#endif
