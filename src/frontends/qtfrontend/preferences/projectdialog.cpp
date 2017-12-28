/******************************************************************************
 *  Copyright (C) 2005-2016 by                                                *
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

#include <QDebug>
#include <QVBoxLayout>

#include "technical/preferencestool.h"

#define MIN_TREE_WIDTH   200
#define MAX_TREE_WIDTH   MIN_TREE_WIDTH
#define MIN_PAGE_WIDTH   600
#define MAX_PAGE_WIDTH   MIN_PAGE_WIDTH
#define MIN_PAGE_HEIGHT  500
#define MAX_PAGE_HEIGHT  MIN_PAGE_HEIGHT


ProjectDialog::ProjectDialog(Frontend *f, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    qDebug() << "ProjectDialog::Constructor --> Start";

    frontend               = f;
    pageTree                = 0;
    pageLayout              = 0;
    // generalSettingsPage     = 0;
    // projectValuePage        = 0;
    imageImportPage         = 0;
    imageTransformationPage = 0;
    videoExportPage         = 0;
    // grabberSelectPage       = 0;
    activePage              = 0;

    pageTree = new QTreeWidget();
    pageTree->setColumnCount(1);
    pageTree->setHeaderHidden(true);
    pageTree->setMinimumWidth(MIN_TREE_WIDTH);
    pageTree->setMaximumWidth(MAX_TREE_WIDTH);
    connect(pageTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(pageTree);
    topLevelItem->setText(0, tr("Properties"));
    topLevelItem->setFlags(Qt::ItemIsEnabled);
    topLevelItem->setExpanded(true);
    pageTree->insertTopLevelItem(0, topLevelItem);

    pageLayout = new QHBoxLayout;
    pageLayout->addWidget(pageTree);

    helpButton = new QPushButton(tr("Help"), this);
    helpButton->setDefault(false);
    connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
    helpButton->setShortcut(QKeySequence::HelpContents);
    connect(helpButton, SIGNAL(triggered()), this, SLOT(help()));

    applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setDefault(false);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));

    closeButton = new QPushButton(tr("Close"), this);
    closeButton->setDefault(true);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(finished(int)), this, SLOT(finish(int)));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(helpButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(pageLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Animation Project Properties"));
    setModal(false);

    setMaximumSize(20+MAX_TREE_WIDTH+MAX_PAGE_WIDTH, 40+MAX_PAGE_HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // makeGeneralSettingsPage();
    // makeProjectValuePage();
    makeImageImportPage();
    makeImageTransformPage();
    makeVideoExportPage();
    // makeGrabberSelectPage();

    qDebug() << "ProjectDialog::Constructor --> End";
}

/*
void ProjectDialog::makeGeneralSettingsPage()
{
    qDebug() << "ProjectDialog::makeGeneralSettingsPage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("General Settings"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    generalSettingsPage = new GeneralWidget(frontend);
    generalSettingsPage->initialize();
    generalSettingsPage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    generalSettingsPage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    pageLayout->addWidget(generalSettingsPage);
    generalSettingsPage->setVisible(true);

    qDebug() << "ProjectDialog::makeGeneralSettingsPage --> End";
}


void ProjectDialog::makeProjectValuePage()
{
    qDebug() << "ProjectDialog::makeDefaultValuePage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("New Project Values"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    projectValuePage = new ProjectWidget(frontend, false);
    projectValuePage->initialize();
    projectValuePage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    projectValuePage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    pageLayout->addWidget(projectValuePage);
    projectValuePage->setVisible(false);

    qDebug() << "ProjectDialog::makeDefaultValuePage --> End";
}
*/

void ProjectDialog::makeImageImportPage()
{
    qDebug() << "ProjectDialog::makeImageImportPage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("Image Import"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    imageImportPage = new ImportWidget(frontend, false);
    imageImportPage->initialize();
    imageImportPage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    imageImportPage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    pageLayout->addWidget(imageImportPage);
    imageImportPage->setVisible(true);

    qDebug() << "ProjectDialog::makeImageImportPage --> End";
}


void ProjectDialog::makeImageTransformPage()
{
    qDebug() << "ProjectDialog::makeImageTransformPage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("Image Transformation"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    imageTransformationPage = new TransformWidget(frontend, false);
    imageTransformationPage->initialize();
    imageTransformationPage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    imageTransformationPage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    pageLayout->addWidget(imageTransformationPage);
    imageTransformationPage->setVisible(false);

    qDebug() << "ProjectDialog::makeImageTransformPage --> End";
}


void ProjectDialog::makeVideoExportPage()
{
    qDebug() << "ProjectDialog::makeVideoExportPage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("Video Export"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    videoExportPage = new ExportWidget(frontend, false);
    videoExportPage->initialize();
    videoExportPage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    videoExportPage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    videoExportPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pageLayout->addWidget(videoExportPage);
    videoExportPage->setVisible(false);

    qDebug() << "ProjectDialog::makeVideoExportPage --> End";
}

/*
void GeneralDialog::makeGrabberSelectPage()
{
    qDebug() << "GeneralDialog::makeGrabberSelectPage --> Start";

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, tr("Grabber"));
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->pageTree->topLevelItem(0)->addChild(newItem);

    grabberSelectPage = new GrabberWidget(frontend);
    grabberSelectPage->initialize();
    grabberSelectPage->setMinimumSize(MIN_PAGE_WIDTH, MIN_PAGE_HEIGHT);
    grabberSelectPage->setMaximumSize(MAX_PAGE_WIDTH, MAX_PAGE_HEIGHT);
    grabberSelectPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pageLayout->addWidget(grabberSelectPage);
    grabberSelectPage->setVisible(false);

    qDebug() << "GeneralDialog::makeGrabberSelectPage --> End";
}
*/

void ProjectDialog::itemClicked(QTreeWidgetItem *pageItem,
                                int               /*column*/)
{
    qDebug() << "ProjectDialog::itemClicked --> Start";

    activePage = 0;

    if (NULL != pageItem->parent())
    {
        activePage = pageItem->parent()->indexOfChild(pageItem);
    }

    // generalSettingsPage->setVisible(false);
    // projectValuePage->setVisible(false);
    imageImportPage->setVisible(false);
    imageTransformationPage->setVisible(false);
    videoExportPage->setVisible(false);
    // grabberSelectPage->setVisible(false);

    switch (activePage)
    {
    default:
    case 0:
        // Image import settings
        imageImportPage->setVisible(true);
        break;
    case 1:
        // Image transformation settings
        imageTransformationPage->setVisible(true);
        break;
    case 2:
        // Video export settings
        videoExportPage->setVisible(true);
        break;
    }

    qDebug() << "ProjectDialog::itemClicked --> End";
}


void ProjectDialog::help()
{
    qDebug() << "ProjectDialog::help --> Start";

    switch (activePage)
    {
    default:
    case 0:
        // Image import settings
        frontend->openOnlineHelp("#properties-import");
        break;
    case 1:
        // Image transformation settings
        frontend->openOnlineHelp("#properties-trans");
        break;
    case 2:
        // Video export settings
        frontend->openOnlineHelp("#properties-export");
        break;
    }

    qDebug() << "ProjectDialog::help --> End";
}


void ProjectDialog::apply()
{
    qDebug() << "ProjectDialog::apply --> Start";

    setFocus();
    // this->generalSettingsPage->apply();
    // this->projectValuePage->apply();
    this->imageImportPage->apply();
    this->imageTransformationPage->apply();
    this->videoExportPage->apply();
    // this->grabberSelectPage->apply();
    frontend->getPreferences()->flushPreferences();
    this->setResult(QDialog::Accepted);
    this->hide();

    qDebug() << "ProjectDialog::apply --> End";
}


void ProjectDialog::close()
{
    qDebug() << "ProjectDialog::close --> Start";

    setFocus();
    // this->generalSettingsPage->reset();
    // this->projectValuePage->reset();
    this->imageImportPage->reset();
    this->imageTransformationPage->reset();
    this->videoExportPage->reset();
    // this->grabberSelectPage->reset();
    this->hide();

    qDebug() << "ProjectDialog::close --> End";
}


void ProjectDialog::finish(int result)
{
    qDebug() << "ProjectDialog::finish --> Start";

    setFocus();
    if (result == 0)
    {
        // generalSettingsPage->reset();
        // projectValuePage->reset();
        imageImportPage->reset();
        imageTransformationPage->reset();
        videoExportPage->reset();
        // grabberSelectPage->reset();
    }
    this->hide();

    qDebug() << "ProjectDialog::finish --> End";
}
