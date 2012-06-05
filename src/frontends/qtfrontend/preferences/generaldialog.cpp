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

#include "generaldialog.h"

#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

#include <QtGui/QVBoxLayout>


GeneralDialog::GeneralDialog(Frontend *f, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    qDebug("GeneralDialog::Constructor --> Start");

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
    setWindowTitle(tr("General Preferences"));
    // setMaximumWidth(600);
    setMinimumWidth(500);
    setMinimumHeight(200);
    setModal(false);

    makeGeneralSettingsTab();
    makeProjectValueTab();
    makeVideoExportTab();

    qDebug("GeneralDialog::Constructor --> End");
}


void GeneralDialog::makeGeneralSettingsTab()
{
    qDebug("GeneralDialog::makeGeneralSettingsTab --> Start");

    generalSettingsTab = new GeneralWidget(frontend);
    generalSettingsTab->initialize();
    // generalSettingsTab->setMinimumHeight(300);
    tabWidget->addTab(generalSettingsTab, tr("&General Settings"));

    qDebug("GeneralDialog::makeGeneralSettingsTab --> End");
}


void GeneralDialog::makeVideoExportTab()
{
    qDebug("GeneralDialog::makeVideoExportTab --> Start");

    exportVideoTab = new ExportWidget(frontend, true);
    exportVideoTab->initialize();
    // exportVideoTab->setMinimumHeight(300);
    tabWidget->addTab(exportVideoTab, tr("Video &Export"));

    qDebug("GeneralDialog::makeVideoExportTab --> End");
}


void GeneralDialog::makeProjectValueTab()
{
    qDebug("GeneralDialog::makeProjectValueTab --> Start");

    projectValueTab = new ProjectWidget(frontend);
    projectValueTab->initialize();
    // projectValueTab->setMinimumHeight(300);
    tabWidget->addTab(projectValueTab, tr("&New Project Values"));

    qDebug("GeneralDialog::makeProjectValueTab --> End");
}


void GeneralDialog::apply()
{
    qDebug("GeneralDialog::apply --> Start");

    setFocus();
    this->generalSettingsTab->apply();
    this->exportVideoTab->apply();
    this->projectValueTab->apply();
    frontend->getPreferences()->flushPreferences();
    this->setResult(QDialog::Accepted);
    this->hide();

    qDebug("GeneralDialog::apply --> End");
}


void GeneralDialog::close()
{
    qDebug("GeneralDialog::close --> Start");

    setFocus();
    this->generalSettingsTab->reset();
    this->exportVideoTab->reset();
    this->projectValueTab->reset();
    this->hide();

    qDebug("GeneralDialog::close --> End");
}


void GeneralDialog::finish(int result)
{
    qDebug("GeneralDialog::finish --> Start");

    setFocus();
    if (result == 0)
    {
        generalSettingsTab->reset();
        exportVideoTab->reset();
        projectValueTab->reset();
    }
    this->hide();

    qDebug("GeneralDialog::finish --> End");
}
