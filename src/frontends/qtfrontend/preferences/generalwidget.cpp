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

    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(languageGroupBox);
    tabLayout->addWidget(captureGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    qDebug("GeneralWidget::makeGUI --> End");
}


void GeneralWidget::initialize()
{
    qDebug("GeneralWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    QString actualLocale = pref->getBasicPreference("language", QString());
    QVector<QString> locales = frontend->getLocales();
    for (int index = 0; index < locales.count(); index++) {
        if (actualLocale.compare(locales[index]) == 0) {
            actualLanguage = index;
        }
    }
    languageCombo->setCurrentIndex(actualLanguage);

    actualButtonFunction = (PreferencesTool::captureButtonFunction)pref->getBasicPreference("capturebutton", PreferencesTool::captureButtonAfter);
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
        pref->setBasicPreference("language", locales[newLanguage]);
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
        pref->setBasicPreference("capturebutton", newButtonFunction);
        actualButtonFunction = newButtonFunction;
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
