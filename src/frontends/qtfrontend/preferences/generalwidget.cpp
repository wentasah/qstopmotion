/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/mainwindowgui.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/util.h"


const QColor GeneralWidget::GRIDCOLORDEFAULT = Qt::black;

GeneralWidget::GeneralWidget(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "GeneralWidget::Constructor --> Start";

    frontend         = f;

    languageGroupBox = 0;
    languageCombo    = 0;
    actualLanguage   = LANGUAGEDEFAULT;

    styleGroupBox = 0;
    styleCombo    = 0;
    actualStyle   = STYLEDEFAULT;

    captureGroupBox  = 0;
    bevorButton      = 0;
    afterButton      = 0;
    appendButton     = 0;
    actualButtonFunction = BUTTONFUNCTIONDEFAULT;

    gridGroupBox          = 0;
    verticalGridCheck     = 0;
    actualVerticalGrid    = VERTICALGRIDDEFAULT;
    verticalGridSpin      = 0;
    actualVerticalSpin    = VERTICALSPINDEFAULT;
    horizontalGridCheck   = 0;
    actualHorizontalGrid  = HORIZONTALGRIDDEFAULT;
    horizontalGridSpin    = 0;
    actualHorizontalSpin  = HORIZONTALSPINDEFAULT;
    gridColorLabel        = 0;
    gridColorButton       = 0;
    actualGridColor       = GRIDCOLORDEFAULT;
    newGridColor          = GRIDCOLORDEFAULT;

    signalGroupBox        = 0;
    signalCheck           = 0;
    actualSignal          = SIGNALDEFAULT;

    photoEditorBox        = 0;
    photoEditorLabel      = 0;
    photoEditorEdit       = 0;
    photoEditorButton     = 0;
    actualPhotoEditorPath = Util::findDefaultPhotoEditor();

    this->setObjectName("GeneralWidget");

    makeGUI();

    qDebug() << "GeneralWidget::Constructor --> End";
}


void GeneralWidget::makeGUI()
{
    // QString iconPath(qstopmotion::graphicsDirectory);
    // iconPath.append(QLatin1String("/icons/"));
    // QString iconFile(iconPath);

    languageGroupBox = new QGroupBox(tr("Language"));
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

    styleGroupBox = new QGroupBox(tr("Style"));
    // styleGroupBox->setFlat(true);

    styleCombo = new QComboBox();
    styleCombo->setFocusPolicy(Qt::NoFocus);
    connect(styleCombo, SIGNAL(activated(int)), this, SLOT(changeStyle(int)));

    // Add all possible styles
    QVector<QString> styles = MainWindowGUI::getStyles(frontend);
    for (int index = 0 ; index < styles.count() ; index++) {
        styleCombo->addItem(styles[index]);
    }

    QVBoxLayout *styleLayout = new QVBoxLayout;
    // styleLayout->setMargin(0);
    // styleLayout->setSpacing(2);
    // styleLayout->addStretch(1);
    styleLayout->addWidget(styleCombo);
    styleLayout->addStretch(10);
    styleGroupBox->setLayout(styleLayout);

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
    connect(verticalGridCheck, SIGNAL(stateChanged(int)), this, SLOT(changeVerticalGridState(int)));

    verticalGridSpin = new QSpinBox();
    verticalGridSpin->setMinimum(1);
    verticalGridSpin->setMaximum(30);
    verticalGridSpin->setValue(5);
    verticalGridSpin->setFocusPolicy(Qt::NoFocus);
    verticalGridSpin->setEnabled(false);

    horizontalGridCheck = new QCheckBox(tr("Horizontal Lines"));
    horizontalGridCheck->setChecked(false);
    connect(horizontalGridCheck, SIGNAL(stateChanged(int)), this, SLOT(changeHorizontalGridState(int)));

    horizontalGridSpin = new QSpinBox();
    horizontalGridSpin->setMinimum(1);
    horizontalGridSpin->setMaximum(30);
    horizontalGridSpin->setValue(5);
    horizontalGridSpin->setFocusPolicy(Qt::NoFocus);
    horizontalGridSpin->setEnabled(false);

    gridColorLabel = new QLabel(tr("Grid Color:"));
    gridColorButton = new QPushButton(tr("Color"));
    connect(gridColorButton,SIGNAL(clicked()), this, SLOT(clickedGridColorButton()));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(verticalGridCheck, 0, 0);
    gridLayout->addWidget(verticalGridSpin, 0, 1);
    gridLayout->addWidget(horizontalGridCheck, 1, 0);
    gridLayout->addWidget(horizontalGridSpin, 1, 1);
    gridLayout->addWidget(gridColorLabel, 2, 0);
    gridLayout->addWidget(gridColorButton, 2, 1);
    gridGroupBox->setLayout(gridLayout);

    signalGroupBox = new QGroupBox;
    signalGroupBox->setTitle(tr("Signal Functionality"));

    signalCheck = new QCheckBox(tr("Take Picture Signal"));
    signalCheck->setChecked(false);
    // connect(signalCheck, SIGNAL(stateChanged(int)), this, SLOT(changeSignalState(int)));

    QGridLayout *signalLayout = new QGridLayout;
    signalLayout->addWidget(signalCheck, 0, 0);
    signalGroupBox->setLayout(signalLayout);

    // Photo editor's preferences
    photoEditorBox = new QGroupBox;
    photoEditorBox->setTitle(tr("Image editor"));

    photoEditorLabel = new QLabel(tr("Select application for edit exposures:"));
    photoEditorEdit = new FlexibleLineEdit;
    photoEditorButton = new QPushButton(tr("Choose"));
    photoEditorButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(photoEditorButton, SIGNAL(clicked()),
            this, SLOT(choosePhotoEditor()));

    QVBoxLayout *photoEditorLayout = new QVBoxLayout;
    photoEditorLayout->addWidget(photoEditorLabel);
    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->addWidget(photoEditorEdit);
    hbLayout->addWidget(photoEditorButton);
    photoEditorLayout->addLayout(hbLayout);

    photoEditorBox->setLayout(photoEditorLayout);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(languageGroupBox);
    mainLayout->addWidget(styleGroupBox);
    mainLayout->addWidget(captureGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(signalGroupBox);
    mainLayout->addWidget(photoEditorBox);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "GeneralWidget::makeGUI --> End";
}


void GeneralWidget::initialize()
{
    qDebug() << "GeneralWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    QString          actualLocale;
    QString          colorName;
    QString          photoEditorPath;
    QVector<QString> locales = frontend->getLocales();

    pref->getStringPreference("preferences", "language", actualLocale);
    for (int index = 0; index < locales.count(); index++) {
        if (actualLocale.compare(locales[index]) == 0) {
            actualLanguage = index;
        }
    }
    languageCombo->setCurrentIndex(actualLanguage);

    pref->getIntegerPreference("preferences", "style", actualStyle);
    styleCombo->setCurrentIndex(actualStyle);

    pref->getIntegerPreference("preferences", "capturebutton", actualButtonFunction);
    pref->getBooleanPreference("preferences", "verticalgrid", actualVerticalGrid);
    pref->getIntegerPreference("preferences", "verticalspin", actualVerticalSpin);
    pref->getBooleanPreference("preferences", "horizontalgrid", actualHorizontalGrid);
    pref->getIntegerPreference("preferences", "horizontalspin", actualHorizontalSpin);
    if (pref->getStringPreference("preferences", "gridcolor", colorName) == true) {
        actualGridColor.setNamedColor(colorName);
    }
    pref->getBooleanPreference("preferences", "signal", actualSignal);
    if (pref->getStringPreference("preferences", "photoeditor", photoEditorPath) == true) {
        actualPhotoEditorPath = photoEditorPath;
    }

    reset();

    qDebug() << "GeneralWidget::initialize --> End";
}

/*
void GeneralWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "GeneralWidget::resizeEvent --> Start";

    QWidget::resizeEvent(event);

    qDebug() << "GeneralWidget::resizeEvent --> End";
}
*/

void GeneralWidget::apply()
{
    qDebug() << "GeneralWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();

    int newLanguage = languageCombo->currentIndex();
    if (actualLanguage != newLanguage)
    {
        // Language changed
        QVector<QString> locales = frontend->getLocales();
        pref->setStringPreference("preferences", "language", locales[newLanguage]);
        actualLanguage = newLanguage;
    }

    int newStyle = styleCombo->currentIndex();
    if (actualStyle != newStyle)
    {
        // Style changed
        pref->setIntegerPreference("preferences", "style", newStyle);
        actualLanguage = newLanguage;

        frontend->showInformation(tr("Information"), tr("Pease restart qStopMotion to activate the new style!"));
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
        pref->setBooleanPreference("preferences", "verticalgrid", newVerticalGrid);
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
        pref->setBooleanPreference("preferences", "horizontalgrid", newHorizontalGrid);
        actualHorizontalGrid = newHorizontalGrid;
    }

    int newHorizontalSpin = horizontalGridSpin->value();
    if (newHorizontalSpin != actualHorizontalSpin) {
        // Horizontal spin changed
        frontend->setHorizontalSpin(newHorizontalSpin);
        pref->setIntegerPreference("preferences", "horizontalspin", newHorizontalSpin);
        actualHorizontalSpin = newHorizontalSpin;
    }

    if (newGridColor != actualGridColor) {
        // Grid color changed
        int r, g, b, a;
        newGridColor.getRgb(&r, &g, &b, &a);
        frontend->setGridColorRGB(r, g, b, a);
        pref->setStringPreference("preferences", "gridcolor", newGridColor.name());
        actualGridColor = newGridColor;
    }

    bool newSignal = signalCheck->isChecked();
    if (newSignal != actualSignal) {
        // Signal changed
        frontend->setSignal(newSignal);
        pref->setBooleanPreference("preferences", "signal", newSignal);
        actualSignal = newSignal;
    }

    const QString newPhotoEditorPath = photoEditorEdit->text();
    if (newPhotoEditorPath != actualPhotoEditorPath) {
        // Photo editor changed
        pref->setStringPreference("preferences", "photoeditor", newPhotoEditorPath);
        actualPhotoEditorPath = newPhotoEditorPath;
    }

    qDebug() << "GeneralWidget::apply --> End";
}


void GeneralWidget::reset()
{
    qDebug() << "GeneralWidget::reset --> Start";

    changeLanguage(actualLanguage);
    styleCombo->setCurrentIndex(actualStyle);
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
    signalCheck->setChecked(actualSignal);
    photoEditorEdit->setText(actualPhotoEditorPath);

    qDebug() << "GeneralWidget::reset --> End";
}


void GeneralWidget::changeLanguage(int index)
{
    frontend->changeLanguage(index);
}


void GeneralWidget::changeStyle(int index)
{
    // frontend->changeStyle(index);
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


void GeneralWidget::changeVerticalGridState(int newState)
{
    if (newState) {
        verticalGridSpin->setEnabled(true);
    }
    else {
        verticalGridSpin->setEnabled(false);
    }
}


void GeneralWidget::changeHorizontalGridState(int newState)
{
    if (newState) {
        horizontalGridSpin->setEnabled(true);
    }
    else {
        horizontalGridSpin->setEnabled(false);
    }
}


void GeneralWidget::clickedGridColorButton()
{
    newGridColor = QColorDialog::getColor(actualGridColor);
    gridColorButton->setText(newGridColor.name());
}

void GeneralWidget::choosePhotoEditor()
{
    QString currentApp = photoEditorEdit->text();
    if (!QFileInfo::exists(currentApp)) {
        currentApp = QDir::home().absolutePath();
    }

    QString appPath = QFileDialog::getOpenFileName(this, tr("Choose image editor"),
                                                   currentApp);
    if (!appPath.isEmpty()) {
        photoEditorEdit->setText(appPath);
    }
}
