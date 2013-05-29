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

#include "projectdialog.h"

#include "technical/preferencestool.h"

#include <QtCore/QtDebug>

#include <QtGui/QVBoxLayout>


ProjectDialog::ProjectDialog(Frontend *f, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    qDebug("ProjectDialog::Constructor --> Start");

    frontend           = f;
    // generalSettingsTab = 0;
    // projectValueTab    = 0;
    imageImportTab     = 0;
    videoExportTab     = 0;

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
    setWindowTitle(tr("Project Preferences"));
    setModal(false);

    // makeGeneralSettingsTab();
    // makeProjectValueTab();
    makeImageImportTab();
    makeVideoExportTab();

    qDebug("ProjectDialog::Constructor --> End");
}

/*
void ProjectDialog::makeGeneralSettingsTab()
{
    qDebug("ProjectDialog::makeGeneralSettingsTab --> Start");

    generalSettingsTab = new GeneralWidget(frontend);
    generalSettingsTab->initialize();
    generalSettingsTab->setMinimumHeight(700);
    tabWidget->addTab(generalSettingsTab, tr("&General Settings"));

    qDebug("ProjectDialog::makeGeneralSettingsTab --> End");
}


void ProjectDialog::makeProjectValueTab()
{
    qDebug("ProjectDialog::makeDefaultValueTab --> Start");

    projectValueTab = new ProjectWidget(frontend, false);
    projectValueTab->initialize();
    projectValueTab->setMinimumHeight(700);
    tabWidget->addTab(projectValueTab, tr("&Project Values"));

    qDebug("ProjectDialog::makeDefaultValueTab --> End");
}
*/

void ProjectDialog::makeImageImportTab()
{
    qDebug("ProjectDialog::makeImageImportTab --> Start");

    imageImportTab = new ImportWidget(frontend, false);
    imageImportTab->initialize();
    imageImportTab->setMinimumHeight(700);
    tabWidget->addTab(imageImportTab, tr("&Image Import"));

    qDebug("ProjectDialog::makeImageImportTab --> End");
}


void ProjectDialog::makeVideoExportTab()
{
    qDebug("ProjectDialog::makeVideoExportTab --> Start");

    videoExportTab = new ExportWidget(frontend, false);
    videoExportTab->initialize();
    videoExportTab->setMinimumHeight(700);
    tabWidget->addTab(videoExportTab, tr("Video &Export"));

    qDebug("ProjectDialog::makeVideoExportTab --> End");
}


void ProjectDialog::apply()
{
    qDebug("ProjectDialog::apply --> Start");

    setFocus();
    // this->generalSettingsTab->apply();
    // this->projectValueTab->apply();
    this->imageImportTab->apply();
    this->videoExportTab->apply();
    frontend->getPreferences()->flushPreferences();
    this->setResult(QDialog::Accepted);
    this->hide();

    qDebug("ProjectDialog::apply --> End");
}


void ProjectDialog::close()
{
    qDebug("ProjectDialog::close --> Start");

    setFocus();
    // this->generalSettingsTab->reset();
    // this->projectValueTab->reset();
    this->imageImportTab->reset();
    this->videoExportTab->reset();
    this->hide();

    qDebug("ProjectDialog::close --> End");
}


void ProjectDialog::finish(int result)
{
    qDebug("ProjectDialog::finish --> Start");

    setFocus();
    if (result == 0)
    {
        // generalSettingsTab->reset();
        // projectValueTab->reset();
        imageImportTab->reset();
        videoExportTab->reset();
    }
    this->hide();

    qDebug("ProjectDialog::finish --> End");
}
