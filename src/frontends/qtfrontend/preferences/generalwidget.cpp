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

#include "generalwidget.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


GeneralWidget::GeneralWidget(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    qDebug("GeneralWidget::Constructor --> Start");

    frontend         = f;

    languageGroupBox = 0;
    languageCombo    = 0;
    actualLanguage   = 0;

    captureGroupBox  = 0;
    bevorButton      = 0;
    afterButton      = 0;
    appendButton     = 0;
    actualButtonFunction = PreferencesTool::captureButtonAfter;

    gridGroupBox          = 0;
    verticalGridCheck     = 0;
    actualVerticalGrid    = false;
    verticalGridSpin      = 0;
    actualVerticalSpin    = 10;
    horizontalGridCheck   = 0;
    actualHorizontalGrid  = false;
    horizontalGridSpin    = 0;
    actualHorizontalSpin  = 10;

    this->setObjectName("GeneralWidget");

    makeGUI();

    qDebug("GeneralWidget::Constructor --> End");
}


void GeneralWidget::makeGUI()
{
    // QString iconPath(qstopmotion::graphicsDirectory);
    // iconPath.append(QLatin1String("/icons/"));
    // QString iconFile(iconPath);

    QVBoxLayout *tabLayout = new QVBoxLayout;

    languageGroupBox = new QGroupBox(QString(tr("Language")));
    // languageGroupBox->setFlat(true);

    languageCombo = new QComboBox();
    languageCombo->setFocusPolicy(Qt::NoFocus);
    connect(languageCombo, SIGNAL(activated(int)), this, SLOT(changeLanguage(int)));

    // Add all possible languages
    QVector<QString> languages = frontend->getLanguages();
    for (int index = 0 ; index < languages.count() ; index++) {
        languageCombo->addItem(languages[index]);
    }

    QVBoxLayout *languageLayout = new QVBoxLayout;
    // languageLayout->setMargin(0);
    // languageLayout->setSpacing(2);
    // languageLayout->addStretch(1);
    languageLayout->addWidget(languageCombo);
    languageLayout->addStretch(10);
    languageGroupBox->setLayout(languageLayout);

    captureGroupBox = new QGroupBox;
    captureGroupBox->setTitle(tr("Capture Button Functionality"));

    bevorButton = new QRadioButton(tr("Insert new frame bevor selected frame"));
    bevorButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bevorButton->setChecked(false);
    connect(bevorButton, SIGNAL(clicked()), this, SLOT(setBevorButtonOn()));

    afterButton = new QRadioButton(tr("Insert new frame after selected frame"));
    afterButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    afterButton->setChecked(true);
    connect(afterButton, SIGNAL(clicked()), this, SLOT(setAfterButtonOn()));

    appendButton = new QRadioButton(tr("Append new frame at the end of the take"));
    appendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    appendButton->setChecked(false);
    connect(appendButton, SIGNAL(clicked()), this, SLOT(setAppendButtonOn()));

    QVBoxLayout *captureLayout = new QVBoxLayout;
    // captureLayout->setMargin(0);
    // captureLayout->setSpacing(2);
    // captureLayout->addStretch(1);
    captureLayout->addWidget(bevorButton);
    captureLayout->addWidget(afterButton);
    captureLayout->addWidget(appendButton);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);

    gridGroupBox = new QGroupBox;
    gridGroupBox->setTitle(tr("Grid Functionality"));

    verticalGridCheck = new QCheckBox(tr("Vertical Lines"));
    verticalGridCheck->setChecked(false);
    connect(verticalGridCheck, SIGNAL(stateChanged(int)), this, SLOT(setVerticalGridOn(int)));

    verticalGridSpin = new QSpinBox();
    verticalGridSpin->setMinimum(1);
    verticalGridSpin->setMaximum(30);
    verticalGridSpin->setValue(5);
    verticalGridSpin->setFocusPolicy(Qt::NoFocus);
    verticalGridSpin->setEnabled(false);

    horizontalGridCheck = new QCheckBox(tr("Horizontal Lines"));
    horizontalGridCheck->setChecked(false);
    connect(horizontalGridCheck, SIGNAL(stateChanged(int)), this, SLOT(setHorizontalGridOn(int)));

    horizontalGridSpin = new QSpinBox();
    horizontalGridSpin->setMinimum(1);
    horizontalGridSpin->setMaximum(30);
    horizontalGridSpin->setValue(5);
    horizontalGridSpin->setFocusPolicy(Qt::NoFocus);
    horizontalGridSpin->setEnabled(false);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(verticalGridCheck, 0, 0);
    gridLayout->addWidget(verticalGridSpin, 0, 1);
    gridLayout->addWidget(horizontalGridCheck, 1, 0);
    gridLayout->addWidget(horizontalGridSpin, 1, 1);
    gridGroupBox->setLayout(gridLayout);

    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(languageGroupBox);
    tabLayout->addWidget(captureGroupBox);
    tabLayout->addWidget(gridGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    qDebug("GeneralWidget::makeGUI --> End");
}


void GeneralWidget::initialize()
{
    qDebug("GeneralWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int              value;
    QString          actualLocale;
    QVector<QString> locales = frontend->getLocales();

    pref->getStringPreference("preferences", "language", actualLocale);
    for (int index = 0; index < locales.count(); index++) {
        if (actualLocale.compare(locales[index]) == 0) {
            actualLanguage = index;
        }
    }
    languageCombo->setCurrentIndex(actualLanguage);

    if (pref->getIntegerPreference("preferences", "capturebutton", actualButtonFunction) == false) {
        actualButtonFunction = PreferencesTool::captureButtonAfter;
    }
    switch (actualButtonFunction) {
    case PreferencesTool::captureButtonBevor:
        setBevorButtonOn();
        break;
    case PreferencesTool::captureButtonAfter:
        setAfterButtonOn();
        break;
    case PreferencesTool::captureButtonAppend:
        setAppendButtonOn();
        break;
    }

    if (pref->getIntegerPreference("preferences", "verticalgrid", value) == false) {
        value = false;
    }
    actualVerticalGrid = value;
    verticalGridCheck->setChecked(actualVerticalGrid);

    if (pref->getIntegerPreference("preferences", "verticalspin", value) == false) {
        value = 5;
    }
    actualVerticalSpin = value;
    verticalGridSpin->setValue(actualVerticalSpin);

    if (pref->getIntegerPreference("preferences", "horizontalgrid", value) == false) {
        value = false;
    }
    actualHorizontalGrid = value;
    horizontalGridCheck->setChecked(actualHorizontalGrid);

    if (pref->getIntegerPreference("preferences", "horizontalspin", value) == false) {
        value = 5;
    }
    actualHorizontalSpin = value;
    horizontalGridSpin->setValue(actualHorizontalSpin);

    qDebug("GeneralWidget::initialize --> End");
}

/*
void GeneralWidget::resizeEvent(QResizeEvent *event)
{
    qDebug("GeneralWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("GeneralWidget::resizeEvent --> End");
}
*/

void GeneralWidget::apply()
{
    qDebug("GeneralWidget::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    int newLanguage = languageCombo->currentIndex();
    if (actualLanguage != newLanguage)
    {
        QVector<QString> locales = frontend->getLocales();
        pref->setStringPreference("preferences", "language", locales[newLanguage]);
        actualLanguage = newLanguage;
    }

    PreferencesTool::captureButtonFunction newButtonFunction;
    if (bevorButton->isChecked()) {
        newButtonFunction = PreferencesTool::captureButtonBevor;
    }
    if (afterButton->isChecked()) {
        newButtonFunction = PreferencesTool::captureButtonAfter;
    }
    if (appendButton->isChecked()) {
        newButtonFunction = PreferencesTool::captureButtonAppend;
    }
    if (newButtonFunction != actualButtonFunction)
    {
        // Selection changed
        frontend->changeCaptureButtonFunction(newButtonFunction);
        pref->setIntegerPreference("preferences", "capturebutton", newButtonFunction);
        actualButtonFunction = newButtonFunction;
    }

    bool newVerticalGrid = verticalGridCheck->isChecked();
    if (newVerticalGrid != actualVerticalGrid) {
        // Vertical grid changed
        frontend->setVerticalGrid(newVerticalGrid);
        pref->setIntegerPreference("preferences", "verticalgrid", newVerticalGrid);
        actualVerticalGrid = newVerticalGrid;
    }

    int newVerticalSpin = verticalGridSpin->value();
    if (newVerticalSpin != actualVerticalSpin) {
        // Vertical spin changed
        frontend->setVerticalSpin(newVerticalSpin);
        pref->setIntegerPreference("preferences", "verticalspin", newVerticalSpin);
        actualVerticalSpin = newVerticalSpin;
    }

    bool newHorizontalGrid = horizontalGridCheck->isChecked();
    if (newHorizontalGrid != actualHorizontalGrid) {
        // Horizontal grid changed
        frontend->setHorizontalGrid(newHorizontalGrid);
        pref->setIntegerPreference("preferences", "horizontalgrid", newHorizontalGrid);
        actualHorizontalGrid = newHorizontalGrid;
    }

    int newHorizontalSpin = horizontalGridSpin->value();
    if (newHorizontalSpin != actualHorizontalSpin) {
        // Horizontal spin changed
        frontend->setHorizontalSpin(newHorizontalSpin);
        pref->setIntegerPreference("preferences", "horizontalspin", newHorizontalSpin);
        actualHorizontalSpin = newHorizontalSpin;
    }

    qDebug("GeneralWidget::apply --> End");
}


void GeneralWidget::reset()
{
    qDebug("GeneralWidget::reset --> Start");

    changeLanguage(actualLanguage);
    frontend->changeCaptureButtonFunction(actualButtonFunction);
    switch (actualButtonFunction) {
    case PreferencesTool::captureButtonBevor:
        setBevorButtonOn();
        break;
    case PreferencesTool::captureButtonAfter:
        setAfterButtonOn();
        break;
    case PreferencesTool::captureButtonAppend:
        setAppendButtonOn();
        break;
    }
    verticalGridCheck->setChecked(actualVerticalGrid);
    verticalGridSpin->setValue(actualVerticalSpin);
    horizontalGridCheck->setChecked(actualHorizontalGrid);
    horizontalGridSpin->setValue(actualHorizontalSpin);

    qDebug("GeneralWidget::reset --> End");
}


void GeneralWidget::changeLanguage(int index)
{
    frontend->changeLanguage(index);
}


void GeneralWidget::setBevorButtonOn()
{
    bevorButton->setChecked(true);
    afterButton->setChecked(false);
    appendButton->setChecked(false);
}


void GeneralWidget::setAfterButtonOn()
{
    bevorButton->setChecked(false);
    afterButton->setChecked(true);
    appendButton->setChecked(false);
}


void GeneralWidget::setAppendButtonOn()
{
    bevorButton->setChecked(false);
    afterButton->setChecked(false);
    appendButton->setChecked(true);
}


void GeneralWidget::setVerticalGridOn(int newState)
{
    if (newState) {
        verticalGridSpin->setEnabled(true);
    }
    else {
        verticalGridSpin->setEnabled(false);
    }
}


void GeneralWidget::setHorizontalGridOn(int newState)
{
    if (newState) {
        horizontalGridSpin->setEnabled(true);
    }
    else {
        horizontalGridSpin->setEnabled(false);
    }
}
