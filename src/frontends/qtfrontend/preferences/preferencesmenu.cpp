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

#include "preferencesmenu.h"

#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

#include <QtGui/QVBoxLayout>


PreferencesMenu::PreferencesMenu(Frontend *f, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    qDebug("PreferencesMenu::Constructor --> Start");

    frontend       = f;
    exportVideoTab = 0;

    tabWidget = new QTabWidget;

    applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setDefault(true);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));

    closeButton = new QPushButton(tr("Close"), this);
    closeButton->setDefault(true);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(finished(int)), this, SLOT(finish(int)));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Preferences Menu"));
    // setMaximumWidth(600);
    setMinimumWidth(500);
    setMinimumHeight(200);
    setModal(false);

    makeGeneralSettingsTab();
    makeVideoExportTab();
    makeDefaultValueTab();

    qDebug("PreferencesMenu::Constructor --> End");
}


void PreferencesMenu::makeGeneralSettingsTab()
{
    qDebug("PreferencesMenu::makeGeneralSettingsTab --> Start");

    generalSettingsTab = new GeneralTab(frontend);
    generalSettingsTab->initialize();
    // generalSettingsTab->setMinimumHeight(300);
    tabWidget->addTab(generalSettingsTab, tr("&General Settings"));

    qDebug("PreferencesMenu::makeGeneralSettingsTab --> End");
}


void PreferencesMenu::makeVideoExportTab()
{
    qDebug("PreferencesMenu::makeVideoExportTab --> Start");

    exportVideoTab = new ExportTab(frontend);
    exportVideoTab->initialize();
    // exportVideoTab->setMinimumHeight(300);
    tabWidget->addTab(exportVideoTab, tr("Video &Export"));

    qDebug("PreferencesMenu::makeVideoExportTab --> End");
}


void PreferencesMenu::makeDefaultValueTab()
{
    qDebug("PreferencesMenu::makeDefaultValueTab --> Start");

    defaultValueTab = new DefaultTab(frontend);
    defaultValueTab->initialize();
    // exportVideoTab->setMinimumHeight(300);
    tabWidget->addTab(defaultValueTab, tr("&Default Values"));

    qDebug("PreferencesMenu::makeDefaultValueTab --> End");
}


void PreferencesMenu::apply()
{
    qDebug("PreferencesMenu::apply --> Start");

    setFocus();
    this->generalSettingsTab->apply();
    this->exportVideoTab->apply();
    this->defaultValueTab->apply();
    frontend->getPreferences()->flushPreferences();
    this->hide();

    qDebug("PreferencesMenu::apply --> End");
}


void PreferencesMenu::close()
{
    qDebug("PreferencesMenu::close --> Start");

    setFocus();
    this->generalSettingsTab->reset();
    this->exportVideoTab->reset();
    this->defaultValueTab->reset();
    this->hide();

    qDebug("PreferencesMenu::close --> End");
}


void PreferencesMenu::finish(int result)
{
    qDebug("PreferencesMenu::finish --> Start");

    setFocus();
    if (result == 0)
    {
        generalSettingsTab->reset();
        exportVideoTab->reset();
        defaultValueTab->reset();
    }
    this->hide();

    qDebug("PreferencesMenu::finish --> End");
}


void PreferencesMenu::display()
{
    this->show();
}


void PreferencesMenu::retranslateStrings()
{
    applyButton->setText(tr("Apply"));
    closeButton->setText(tr("Close"));
    setWindowTitle(tr("Preferences Menu"));

    tabWidget->setTabText(0, tr("&General Settings"));
    tabWidget->setTabText(1, tr("Video &Export"));
    tabWidget->setTabText(2, tr("&Default Values"));

    this->generalSettingsTab->retranslateStrings();
    this->exportVideoTab->retranslateStrings();
    this->defaultValueTab->retranslateStrings();
}

