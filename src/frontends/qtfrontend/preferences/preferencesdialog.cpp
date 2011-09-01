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

#include "preferencesdialog.h"

#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

#include <QtGui/QVBoxLayout>


PreferencesDialog::PreferencesDialog(Frontend *f, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    qDebug("PreferencesDialog::Constructor --> Start");

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

    qDebug("PreferencesDialog::Constructor --> End");
}


void PreferencesDialog::makeGeneralSettingsTab()
{
    qDebug("PreferencesDialog::makeGeneralSettingsTab --> Start");

    generalSettingsTab = new GeneralTab(frontend);
    generalSettingsTab->initialize();
    // generalSettingsTab->setMinimumHeight(300);
    tabWidget->addTab(generalSettingsTab, tr("&General Settings"));

    qDebug("PreferencesDialog::makeGeneralSettingsTab --> End");
}


void PreferencesDialog::makeVideoExportTab()
{
    qDebug("PreferencesDialog::makeVideoExportTab --> Start");

    exportVideoTab = new ExportTab(frontend);
    exportVideoTab->initialize();
    // exportVideoTab->setMinimumHeight(300);
    tabWidget->addTab(exportVideoTab, tr("Video &Export"));

    qDebug("PreferencesDialog::makeVideoExportTab --> End");
}


void PreferencesDialog::makeDefaultValueTab()
{
    qDebug("PreferencesDialog::makeDefaultValueTab --> Start");

    defaultValueTab = new DefaultTab(frontend);
    defaultValueTab->initialize();
    // exportVideoTab->setMinimumHeight(300);
    tabWidget->addTab(defaultValueTab, tr("&Default Values"));

    qDebug("PreferencesDialog::makeDefaultValueTab --> End");
}


void PreferencesDialog::apply()
{
    qDebug("PreferencesDialog::apply --> Start");

    setFocus();
    this->generalSettingsTab->apply();
    this->exportVideoTab->apply();
    this->defaultValueTab->apply();
    frontend->getPreferences()->flushPreferences();
    this->hide();

    qDebug("PreferencesDialog::apply --> End");
}


void PreferencesDialog::close()
{
    qDebug("PreferencesDialog::close --> Start");

    setFocus();
    this->generalSettingsTab->reset();
    this->exportVideoTab->reset();
    this->defaultValueTab->reset();
    this->hide();

    qDebug("PreferencesDialog::close --> End");
}


void PreferencesDialog::finish(int result)
{
    qDebug("PreferencesDialog::finish --> Start");

    setFocus();
    if (result == 0)
    {
        generalSettingsTab->reset();
        exportVideoTab->reset();
        defaultValueTab->reset();
    }
    this->hide();

    qDebug("PreferencesDialog::finish --> End");
}
