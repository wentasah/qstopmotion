/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
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

#include "projecttab.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/dialogs/descriptiondialog.h"
#include "technical/util.h"

#include <QtCore/QList>
#include <QtCore/QProcess>
#include <QtCore/QtDebug>
#include <QtGui/QLabel>
#include <QtGui/QInputDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QVBoxLayout>


ProjectTab::ProjectTab(Frontend *f,
                       const QString &lvd,
                       QWidget *parent) :
    QWidget(parent)
{
    qDebug("ProjectTab::Constructor --> Start");

    frontend            = f;
    if (!lvd.isEmpty()) {
      lastVisitedDir.append(lvd);
    }

    fileDialog          = NULL;
    projectTree         = 0;

    scenesGroupBox      = 0;
    insertSceneButton   = 0;
    addSceneButton      = 0;
    removeSceneButton   = 0;

    takesGroupBox       = 0;
    insertTakeButton    = 0;
    addTakeButton       = 0;
    removeTakeButton    = 0;

    framesGroupBox      = 0;
    insertFramesButton  = 0;
    addFramesButton     = 0;
    removeFramesButton  = 0;

    editGroupBox        = 0;
    editFrameButton     = 0;

    this->setObjectName("ProjectTab");

    makeGUI();
    enableTab();
    frontend->getView()->attatch(this);

    qDebug("ProjectTab::Constructor --> End");
}


void ProjectTab::makeGUI()
{
    qDebug("ProjectTab::makeGUI --> Start");

    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;

    // ========================================================================
    // Project tree
    projectGroupBox = new QGroupBox(tr("Project Tree"));
    // projectGroupBox->setFlat(true);

    projectTree = new QTreeWidget();
    projectTree->setColumnCount(1);
    projectTree->setHeaderHidden(true);
    connect(projectTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));
    // connect(projectTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
    // connect(projectTree, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

    QHBoxLayout *projectLayout = new QHBoxLayout;
    projectLayout->setMargin(4);
    // projectLayout->setSpacing(2);
    // projectLayout->addStretch(1);
    projectLayout->addWidget(projectTree);
    // projectLayout->addWidget(xxxx);
    // projectLayout->addStretch(10);
    projectGroupBox->setLayout(projectLayout);

    // ========================================================================
    // Scene buttons
    scenesGroupBox = new QGroupBox(tr("Scenes"));
    // scenesGroupBox->setFlat(true);

    insertSceneButton = new QPushButton;
    iconFile.append(QLatin1String("insertscene.png"));
    insertSceneButton->setIcon(QPixmap(iconFile));
    // insertSceneButton->setFlat(true);
    // insertSceneButton->setFocusPolicy( Qt::NoFocus );
    connect(insertSceneButton, SIGNAL(clicked()), this, SLOT(insertSceneSlot()));

    addSceneButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("appendscene.png"));
    addSceneButton->setIcon(QPixmap(iconFile));
    // addSceneButton->setFlat(true);
    // addSceneButton->setFocusPolicy( Qt::NoFocus );
    connect(addSceneButton, SIGNAL(clicked()), this, SLOT(addSceneSlot()));

    removeSceneButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removescene.png"));
    removeSceneButton->setIcon(QPixmap(iconFile));
    // removeSceneButton->setFlat(true);
    // removeSceneButton->setFocusPolicy( Qt::NoFocus );
    connect(removeSceneButton, SIGNAL(clicked()), this, SLOT(removeSceneSlot()));

    QHBoxLayout *sceneLayout = new QHBoxLayout;
    sceneLayout->setMargin(4);
    // sceneLayout->setSpacing(2);
    // sceneLayout->addStretch(1);
    sceneLayout->addWidget(insertSceneButton);
    sceneLayout->addWidget(addSceneButton);
    sceneLayout->addWidget(removeSceneButton);
    // sceneLayout->addStretch(10);
    scenesGroupBox->setLayout(sceneLayout);

    // ========================================================================
    // Take buttons
    takesGroupBox = new QGroupBox(tr("Takes"));
    // takesGroupBox->setFlat(true);

    insertTakeButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("inserttake.png"));
    insertTakeButton->setIcon(QPixmap(iconFile));
    // insertTakeButton->setFlat(true);
    // insertTakeButton->setFocusPolicy( Qt::NoFocus );
    connect(insertTakeButton, SIGNAL(clicked()), this, SLOT(insertTakeSlot()));

    addTakeButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("appendtake.png"));
    addTakeButton->setIcon(QPixmap(iconFile));
    // addTakeButton->setFlat(true);
    // addTakeButton->setFocusPolicy( Qt::NoFocus );
    connect(addTakeButton, SIGNAL(clicked()), this, SLOT(addTakeSlot()));

    removeTakeButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removetake.png"));
    removeTakeButton->setIcon(QPixmap(iconFile));
    // removeTakeButton->setFlat(true);
    // removeTakeButton->setFocusPolicy( Qt::NoFocus );
    connect(removeTakeButton, SIGNAL(clicked()), this, SLOT(removeTakeSlot()));

    QHBoxLayout *takeLayout = new QHBoxLayout;
    takeLayout->setMargin(4);
    // takeLayout->setSpacing(2);
    // takeLayout->addStretch(1);
    takeLayout->addWidget(insertTakeButton);
    takeLayout->addWidget(addTakeButton);
    takeLayout->addWidget(removeTakeButton);
    // takeLayout->addStretch(10);
    takesGroupBox->setLayout(takeLayout);

    // ========================================================================
    // Frame buttons
    framesGroupBox = new QGroupBox(tr("Frames"));
    // framesGroupBox->setFlat(true);

    insertFramesButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("insertframes.png"));
    insertFramesButton->setIcon(QPixmap(iconFile));
    // insertFramesButton->setFlat(true);
    // insertFramesButton->setFocusPolicy( Qt::NoFocus );
    connect(insertFramesButton, SIGNAL(clicked()), this, SLOT(insertFramesSlot()));

    addFramesButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("appendframes.png"));
    addFramesButton->setIcon(QPixmap(iconFile));
    // addFramesButton->setFlat(true);
    // addFramesButton->setFocusPolicy( Qt::NoFocus );
    connect(addFramesButton, SIGNAL(clicked()), this, SLOT(addFramesSlot()));

    removeFramesButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("removeframes.png"));
    removeFramesButton->setIcon(QPixmap(iconFile));
    // removeFramesButton->setFlat(true);
    // removeFramesButton->setFocusPolicy( Qt::NoFocus );
    connect(removeFramesButton, SIGNAL(clicked()), this, SLOT(removeFramesSlot()));

    QHBoxLayout *frameLayout = new QHBoxLayout;
    frameLayout->setMargin(4);
    // frameLayout->setSpacing(2);
    // frameLayout->addStretch(1);
    frameLayout->addWidget(insertFramesButton);
    frameLayout->addWidget(addFramesButton);
    frameLayout->addWidget(removeFramesButton);
    // frameLayout->addStretch(10);
    framesGroupBox->setLayout(frameLayout);

    // ========================================================================
    // Edit buttons
    editGroupBox = new QGroupBox(tr("Edit"));
    // editGroupBox->setFlat(true);

    editFrameButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("launchgimp.png"));
    editFrameButton->setIcon(QPixmap(iconFile));
    // editFrameButton->setFlat(true);
    // editFrameButton->setFocusPolicy( Qt::NoFocus );
    connect(editFrameButton, SIGNAL(clicked()), this, SLOT(editFrameSlot()));

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->setMargin(4);
    // editLayout->setSpacing(2);
    // editLayout->addStretch(1);
    editLayout->addWidget(editFrameButton);
    // editLayout->addStretch(10);
    editGroupBox->setLayout(editLayout);

    // ========================================================================
    // Tab layout
    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(projectGroupBox);
    tabLayout->addWidget(scenesGroupBox);
    tabLayout->addWidget(takesGroupBox);
    tabLayout->addWidget(framesGroupBox);
    tabLayout->addWidget(editGroupBox);
    // tabLayout->addStretch(1);

    setLayout(tabLayout);

    qDebug("ProjectTab::makeGUI --> End");
}


void ProjectTab::initialize()
{
    qDebug("ProjectTab::initialize --> Start");

    /*
    PreferencesTool *pref = PreferencesTool::get();
    int active = -1;

    int numEncoders = pref->getNumberEncoders();

    encoderTable->setRowCount(numEncoders);

    if (numEncoders > 0 ) {
        QString activeId = pref->getActiveEncoder();
        int idx = 0;

        for (int i = 0; i < numEncoders; ++i) {
            QString id(QString("encoder%1").arg(i));
            QString name;
            QString desc;
            QString start;
            QString stop;
            QString filename;

            if ( pref->getEncoder(id, name, desc, start, stop, filename) ) {
                encoderTable->setItem(i, 0, new QTableWidgetItem(name) );
                encoderTable->setItem(i, 1, new QTableWidgetItem(desc) );
                startEncoderStrings.push_back(start);
                stopEncoderStrings.push_back(stop);
                outputFiles.push_back(filename);

                if ( id.compare(activeId) == 0 )
                    active = idx;

                idx++;
            }
        }
    }

    if (active > -1) {
        encoderTable->setCurrentCell(active, 0);
    }
    */

    qDebug("ProjectTab::initialize --> End");
}


void ProjectTab::enableTab()
{
    insertSceneButton->setEnabled(true);
    addSceneButton->setEnabled(true);
    removeSceneButton->setEnabled(true);
    insertTakeButton->setEnabled(true);
    addTakeButton->setEnabled(true);
    removeTakeButton->setEnabled(true);
    insertFramesButton->setEnabled(true);
    addFramesButton->setEnabled(true);
    removeFramesButton->setEnabled(true);
    editFrameButton->setEnabled(false);

    tabEnabled = true;
}


void ProjectTab::disableTab()
{
    insertSceneButton->setEnabled(false);
    addSceneButton->setEnabled(false);
    removeSceneButton->setEnabled(false);
    insertTakeButton->setEnabled(false);
    addTakeButton->setEnabled(false);
    removeTakeButton->setEnabled(false);
    insertFramesButton->setEnabled(false);
    addFramesButton->setEnabled(false);
    removeFramesButton->setEnabled(false);
    editFrameButton->setEnabled(false);

    tabEnabled = false;
}

/*
void ProjectTab::resizeEvent(QResizeEvent *event)
{
    qDebug("ProjectTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ProjectTab::resizeEvent --> End");
}
*/

void ProjectTab::apply()
{
    qDebug("ProjectTab::apply --> Start");

    /*
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("encoder%1").arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("encoder%1").arg(i),
                             encoderTable->item(i, 0)->text(),
                             encoderTable->item(i, 1)->text(),
                             startEncoderStrings[i],
                             stopEncoderStrings[i],
                             outputFiles[i]);
        }
    }
    else {
        pref->setNumberEncoders(0);
        pref->setActiveEncoder(QString());
    }
    */

    qDebug("ProjectTab::apply --> End");
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void ProjectTab::updateClear()
{
    qDebug("ProjectTab::updateClear --> Start");

//    clear();

    qDebug("ProjectTab::updateClear --> End");
}


void ProjectTab::updateNewProject()
{
    qDebug("ProjectTab::updateNewProject --> Start");

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(projectTree);
    topLevelItem->setText(0, this->frontend->getProject()->getProjectID());
    topLevelItem->setFlags(Qt::ItemIsEnabled);
    projectTree->insertTopLevelItem(0, topLevelItem);

    qDebug("ProjectTab::updateNewProject --> End");
}


void ProjectTab::updateDescriptionsUpdated()
{
    qDebug("ProjectTab::updateDescriptionsUpdated --> Start");

    QTreeWidgetItem *topLevelItem = projectTree->topLevelItem(0);
    QTreeWidgetItem *sceneItem;
    QTreeWidgetItem *takeItem;
    QTreeWidgetItem *exposureItem;
    int sceneCount = topLevelItem->childCount();
    int sceneIndex;
    Scene *scene;
    int takeCount;
    int takeIndex;
    Take *take;
    int exposureCount;
    int exposureIndex;
    Exposure *exposure;

    topLevelItem->setText(0, this->frontend->getProject()->getProjectID());
    for (sceneIndex = 0; sceneIndex < sceneCount ; sceneIndex++) {
        sceneItem = topLevelItem->child(sceneIndex);
        scene = this->frontend->getProject()->getScene(sceneIndex);
        sceneItem->setText(0, scene->getDescription());
        takeCount = sceneItem->childCount();
        for (takeIndex = 0; takeIndex < takeCount ; takeIndex++) {
            takeItem = sceneItem->child(takeIndex);
            take = scene->getTake(takeIndex);
            takeItem->setText(0, take->getDescription());
            exposureCount = takeItem->childCount();
            for (exposureIndex = 0; exposureIndex < exposureCount ; exposureIndex++) {
                exposureItem = takeItem->child(exposureIndex);
                exposure = take->getExposure(exposureIndex);
                exposureItem->setText(0, exposure->getId());
            }
        }
    }

    qDebug("ProjectTab::updateDescriptionsUpdated --> End");
}


void ProjectTab::updatePlaySound(int) {}


void ProjectTab::updateMixingMode(int) {}


void ProjectTab::updateMixCount(int) {}


void ProjectTab::updateFramesPerSecond(int) {}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void ProjectTab::updateAddScene(int sceneIndex)
{
    qDebug("ProjectTab::updateAddScene --> Start");

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    Scene *scene = frontend->getProject()->getScene(sceneIndex);
    if (scene->getDescription().isEmpty()) {
        newItem->setText(0, scene->getId());
    }
    else {
        newItem->setText(0, scene->getDescription());
    }
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->projectTree->topLevelItem(0)->addChild(newItem);

    qDebug("ProjectTab::updateAddScene --> End");
}


void ProjectTab::updateInsertScene(int sceneIndex)
{
    qDebug("ProjectTab::updateInsertScene --> Start");

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    Scene *scene = frontend->getProject()->getScene(sceneIndex);
    if (scene->getDescription().isEmpty()) {
        newItem->setText(0, scene->getId());
    }
    else {
        newItem->setText(0, scene->getDescription());
    }
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    this->projectTree->topLevelItem(0)->insertChild(sceneIndex, newItem);

    qDebug("ProjectTab::updateInsertScene --> End");
}

/*
void ProjectTab::updateSetNewActiveScene(int sceneIndex)
{
    qDebug("ProjectTab::updatSeteNewActiveScene --> Start");

    this->setNewActiveScene(sceneIndex);

    qDebug("ProjectTab::updatSeteNewActiveScene --> End");
}
*/

void ProjectTab::updateActivateScene()
{
    qDebug("ProjectTab::updateActivateScene --> Start");

    // this->activateScene();

    qDebug("ProjectTab::updateActivateScene --> End");
}


void ProjectTab::updateRemoveScene(int sceneIndex)
{
    qDebug("ProjectTab::updateRemoveScene --> Start");

    this->unsetActiveItems();
    this->removeScene(sceneIndex);
    this->setActiveItems();

    qDebug("ProjectTab::updateRemoveScene --> End");
}


void ProjectTab::updateMoveScene(int sceneNumber,
                                 int movePosition)
{
    qDebug("ProjectTab::updateMoveScene --> Start");

    // this->moveScene(sceneNumber, movePosition);

    qDebug("ProjectTab::updateMoveScene --> End");
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void ProjectTab::updateAddTake(int sceneIndex,
                               int takeIndex)
{
    qDebug("ProjectTab::updateAddTake --> Start");

    QTreeWidgetItem *sceneItem = this->projectTree->topLevelItem(0)->child(sceneIndex);
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    Take *take = frontend->getProject()->getScene(sceneIndex)->getTake(takeIndex);
    if (take->getDescription().isEmpty()) {
        newItem->setText(0, take->getId());
    }
    else {
        newItem->setText(0, take->getDescription());
    }
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    sceneItem->addChild(newItem);

    qDebug("ProjectTab::updateAddTake --> End");
}

void ProjectTab::updateInsertTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug("ProjectTab::updateInsertTake --> Start");

    QTreeWidgetItem *sceneItem = this->projectTree->topLevelItem(0)->child(sceneIndex);
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    Take *take = frontend->getProject()->getScene(sceneIndex)->getTake(takeIndex);
    if (take->getDescription().isEmpty()) {
        newItem->setText(0, take->getId());
    }
    else {
        newItem->setText(0, take->getDescription());
    }
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    sceneItem->insertChild(takeIndex, newItem);

    qDebug("ProjectTab::updateInsertTake --> End");
}

/*
void ProjectTab::updateSetNewActiveTake(int takeIndex)
{
    qDebug("ProjectTab::updateSetNewActiveTake --> Start");

    this->setSetNewActiveTake(takeIndex);

    qDebug("ProjectTab::updateSetNewActiveTake --> End");
}
*/

void ProjectTab::updateActivateTake()
{
    qDebug("ProjectTab::updateActivateTake --> Start");

    // this->unsetActiveExposure();

    qDebug("ProjectTab::updateActivateTake --> End");
}


void ProjectTab::updateRemoveTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug("ProjectTab::updateRemoveTake --> Start");

    this->unsetActiveItems();
    QTreeWidgetItem *topLevelItem = this->projectTree->topLevelItem(0);
    Q_ASSERT(sceneIndex < topLevelItem->childCount());
    QTreeWidgetItem *sceneItem = topLevelItem->child(sceneIndex);
    this->removeTake(sceneItem, takeIndex);
    this->setActiveItems();

    qDebug("ProjectTab::updateRemoveTake --> End");
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void ProjectTab::updateAddExposure(int sceneIndex,
                                   int takeIndex,
                                   int exposureIndex)
{
    qDebug("ProjectTab::updateAddExposure --> Start");

    Exposure *exposure = frontend->getProject()->getScene(sceneIndex)->getTake(takeIndex)->getExposure(exposureIndex);

    QTreeWidgetItem *topLevelItem = this->projectTree->topLevelItem(0);
    Q_ASSERT(sceneIndex < topLevelItem->childCount());
    QTreeWidgetItem *sceneItem = topLevelItem->child(sceneIndex);
    Q_ASSERT(takeIndex < sceneItem->childCount());
    QTreeWidgetItem *takeItem = sceneItem->child(takeIndex);
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, exposure->getId());
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    takeItem->addChild(newItem);

    qDebug("ProjectTab::updateAddExposure --> End");
}


void ProjectTab::updateInsertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug("ProjectTab::updateInsertExposure --> Start");

    Exposure *exposure = frontend->getProject()->getScene(sceneIndex)->getTake(takeIndex)->getExposure(exposureIndex);

    QTreeWidgetItem *takeItem = this->projectTree->topLevelItem(0)->child(sceneIndex)->child(takeIndex);
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, exposure->getId());
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    takeItem->insertChild(exposureIndex, newItem);

    qDebug("ProjectTab::updateInsertExposure --> End");
}


void ProjectTab::updateRemoveExposures(int fromFrame,
                                       int toFrame)
{
    qDebug("ProjectTab::updateRemoveExposures --> Start");

    // removeExposures(fromFrame, toFrame);
    // emit modelSizeChanged(frontend->getProject()->getTotalExposureSize());

    qDebug("ProjectTab::updateRemoveExposures --> End");
}


void ProjectTab::updateRemoveExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug("ProjectTab::updateRemoveExposure --> Start");

    int newExposureIndex;

    this->unsetActiveItems();
    QTreeWidgetItem *activeSceneItem = this->projectTree->topLevelItem(0)->child(sceneIndex);
    QTreeWidgetItem *activeTakeItem = activeSceneItem->child(takeIndex);
    QTreeWidgetItem *activeExposureItem = activeTakeItem->child(exposureIndex);
    activeTakeItem->removeChild(activeExposureItem);
    delete activeExposureItem;
    if (exposureIndex == activeTakeItem->childCount()) {
        newExposureIndex = exposureIndex - 1;
    }
    else {
        newExposureIndex = exposureIndex;
    }
    activeExposureItem = activeTakeItem->child(newExposureIndex);
    this->setActiveItems();

    qDebug("ProjectTab::updateRemoveExposure --> End");
}


void ProjectTab::updateMoveExposures(int fromFrame,
                                     int toFrame,
                                     int movePosition)
{
    qDebug("ProjectTab::updateMoveExposures --> Start");

    // moveExposures(fromFrame, toFrame, movePosition);

    qDebug("ProjectTab::updateMoveExposures --> End");
}

/*
void ProjectTab::updateSetNewActiveExposure(int exposureIndex)
{
    qDebug("ProjectTab::updateSetNewActiveExposure --> Start");

    setActiveFrame(exposureIndex);

    if (preferencesMenu->isVisible()) {
        if (exposureIndex >= 0) {
            showPreferencesMenu();
        } else {
            preferencesMenu->close();
        }
    }

    // For writing the frame number in the frame number display
    emit newActiveFrame(QString(tr("Exposure number: ")) + QString("%1").arg(exposureIndex + 1));

    // For setting the value in the spinbox in the gotomenu
    emit newActiveFrame(exposureIndex + 1);

    qDebug("ProjectTab::updateSetNewActiveExposure --> End");
}
*/

void ProjectTab::updateActivateExposure()
{
    qDebug("ProjectTab::updateActivateExposure --> Start");

    this->unsetActiveItems();
    this->setActiveItems();

    qDebug("ProjectTab::updateActivateExposure --> End");
}

/*
void ProjectTab::updateFrame(int sceneIndex,
                             int takeIndex,
                             int exposureIndex)
{
    qDebug("ProjectTab::updateFrame --> Start");

    updateNewActiveFrame(exposureIndex);

    qDebug("ProjectTab::updateFrame --> End");
}
*/

/**************************************************************************
 * Other functions
 **************************************************************************/

void ProjectTab::unsetActiveItems()
{
    qDebug("ProjectTab::unsetActiveExposure --> Start");

    QList<QTreeWidgetItem*> selectedItems = this->projectTree->selectedItems();
    int selectedItemsCount = selectedItems.count();
    int selectedItemsIndex;

    for (selectedItemsIndex = 0 ; selectedItemsIndex < selectedItemsCount ; selectedItemsIndex++) {
        selectedItems[selectedItemsIndex]->setSelected(FALSE);
        // selectedItems[selectedItemsIndex]->setExpanded(FALSE);
        // projectTree->collapseItem(selectedItems[selectedItemsIndex]);
    }
    projectTree->collapseAll();

    qDebug("ProjectTab::unsetActiveExposure --> End");
}


void ProjectTab::setActiveItems()
{
    qDebug("ProjectTab::setActiveExposure --> Start");

    int activeSceneIndex;
    int activeTakeIndex;
    int activeExposureIndex;

    QTreeWidgetItem *activeExposureItem = NULL;
    QTreeWidgetItem *activeTakeItem = NULL;
    QTreeWidgetItem *activeSceneItem = NULL;
    QTreeWidgetItem *activeProjectItem = projectTree->topLevelItem(0);
    activeProjectItem->setExpanded(TRUE);
    activeProjectItem->setSelected(TRUE);

    activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    if (0 > activeSceneIndex) {
        qDebug("ProjectTab::setActiveExposure --> End (Only project)");
        return;
    }
    activeSceneItem = activeProjectItem->child(activeSceneIndex);
    activeSceneItem->setExpanded(TRUE);
    activeSceneItem->setSelected(TRUE);
    projectTree->scrollToItem(activeSceneItem);

    activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    if (0 > activeTakeIndex) {
        qDebug("ProjectTab::setActiveExposure --> End (Up to scene)");
        return;
    }
    activeTakeItem = activeSceneItem->child(activeTakeIndex);
    activeTakeItem->setExpanded(TRUE);
    activeTakeItem->setSelected(TRUE);
    projectTree->scrollToItem(activeTakeItem);

    activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    if (0 > activeExposureIndex) {
        qDebug("ProjectTab::setActiveExposure --> End (Up to take)");
        return;
    }
    activeExposureItem = activeTakeItem->child(activeExposureIndex);
    activeExposureItem->setSelected(TRUE);
    projectTree->scrollToItem(activeExposureItem);

    qDebug("ProjectTab::setActiveExposure --> End (All)");
}


void ProjectTab::itemClicked(QTreeWidgetItem * /*exposureItem*/,
                             int /*column*/)
{
    qDebug("ProjectTab::itemClicked --> Start");

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    if (!getSelectedItems(sceneIndex, takeIndex, exposureIndex)) {
        // Project item selected
        setActiveItems();
        return;
    }
    if (-1 == takeIndex) {
        // Scene item selected
        Scene *scene = frontend->getProject()->getScene(sceneIndex);
        takeIndex = scene->getActiveTakeIndex();
        if (-1 == takeIndex) {
            // This is an empty scene
        }
        else {
            // One or more takes exists
            Take *take = scene->getTake(takeIndex);
            exposureIndex = take->getActiveExposureIndex();
        }
        frontend->getProject()->activeItemChanged(sceneIndex, takeIndex, exposureIndex);

        qDebug("ProjectTab::itemClicked --> End (Scene selected)");
        return;
    }
    if (-1 == exposureIndex) {
        // Take item selected
        Scene *scene = frontend->getProject()->getScene(sceneIndex);
        Take *take = scene->getTake(takeIndex);
        exposureIndex = take->getActiveExposureIndex();
    }

    frontend->getProject()->activeItemChanged(sceneIndex, takeIndex, exposureIndex);

    qDebug("ProjectTab::itemClicked --> End (Take or exposure selected)");
}


void ProjectTab::itemDoubleClicked(QTreeWidgetItem * exposureItem,
                                   int /*column*/)
{
    qDebug("ProjectTab::itemDoubleClicked --> Start");

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    if (-1 == exposureIndex) {
        // Clicked item is not a exposure item
        return;
    }

    frontend->getProject()->activeItemChanged(sceneIndex,
                                              takeIndex,
                                              exposureIndex);

    qDebug("ProjectTab::itemDoubleClicked --> End");
}


void ProjectTab::itemSelectionChanged()
{
    qDebug("ProjectTab::itemSelectionChanged --> Start");
/*
    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    if (-1 == exposureIndex) {
        // Clicked item is not a exposure item
        return;
    }
*/
    qDebug("ProjectTab::itemSelectionChanged --> End");
}


void ProjectTab::insertSceneSlot()
{
    qDebug("ProjectTab::insertSceneSlot --> Start");

    DescriptionDialog *dialog = new DescriptionDialog(DescriptionDialog::SceneDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectID());
    dialog->setSceneDescription("Scene ???");
    dialog->setTakeDescription("Take ???");
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("ProjectTab::addTakeSlot --> End (cancel)");
        return;
    }

    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    frontend->getProject()->insertSceneToUndo(sceneIndex, sceneDescription);
    frontend->getProject()->addTakeToUndo(sceneIndex, takeDescription);

    qDebug("ProjectTab::insertSceneSlot --> End");
}


void ProjectTab::addSceneSlot()
{
    qDebug("ProjectTab::addSceneSlot --> Start");

    DescriptionDialog *dialog = new DescriptionDialog(DescriptionDialog::SceneDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectID());
    dialog->setSceneDescription("Scene ???");
    dialog->setTakeDescription("Take ???");
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("ProjectTab::addTakeSlot --> End (cancel)");
        return;
    }

    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    frontend->getProject()->addSceneToUndo(sceneDescription);

    int activeSceneIndex = frontend->getProject()->getSceneSize() - 1;
    frontend->getProject()->addTakeToUndo(activeSceneIndex, takeDescription);

    qDebug("ProjectTab::addSceneSlot --> End");
}


void ProjectTab::removeSceneSlot()
{
    qDebug("ProjectTab::removeSceneSlot --> Start");

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    frontend->getProject()->removeSceneToUndo(sceneIndex);

    qDebug("ProjectTab::removeSceneSlot --> End");
}


void ProjectTab::insertTakeSlot()
{
    qDebug("ProjectTab::insertTakeSlot --> Start");

    Scene *activeScene = frontend->getProject()->getActiveScene();

    DescriptionDialog *dialog = new DescriptionDialog(DescriptionDialog::TakeDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectID());
    dialog->setSceneDescription(activeScene->getId());
    dialog->setTakeDescription("Take ???");
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("ProjectTab::addTakeSlot --> End (cancel)");
        return;
    }

    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    frontend->getProject()->insertTakeToUndo(sceneIndex, takeIndex, takeDescription);

    qDebug("ProjectTab::insertTakeSlot --> End");
}


void ProjectTab::addTakeSlot()
{
    qDebug("ProjectTab::addTakeSlot --> Start");

    Scene *activeScene = frontend->getProject()->getActiveScene();
    int activeSceneIndex = activeScene->getIndex();

    DescriptionDialog *dialog = new DescriptionDialog(DescriptionDialog::TakeDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectID());
    dialog->setSceneDescription(activeScene->getId());
    dialog->setTakeDescription("Take ???");
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug("ProjectTab::addTakeSlot --> End (cancel)");
        return;
    }

    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    frontend->getProject()->addTakeToUndo(activeSceneIndex, takeDescription);

    qDebug("ProjectTab::addTakeSlot --> End");
}


void ProjectTab::removeTakeSlot()
{
    qDebug("ProjectTab::removeTakeSlot --> Start");

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    frontend->getProject()->removeTakeToUndo(sceneIndex, takeIndex);

    qDebug("ProjectTab::removeTakeSlot --> End");
}


void ProjectTab::insertFramesSlot()
{
    qDebug("ProjectTab::insertFramesSlot --> Start");

    QStringList selectedFiles = selectFiles();
    int selectedFilesCount = selectedFiles.count();
    if (0 == selectedFilesCount) {
        // The user didn't select one ore more files
        qDebug("ProjectTab::addFrameSlot --> End (no file selected)");
        return;
    }
    int selectedFilesIndex;
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
    for (selectedFilesIndex = 0 ; selectedFilesIndex < selectedFilesCount ; selectedFilesIndex++) {
        // insert the selected files
        frontend->getProject()->insertExposureToUndo(activeSceneIndex, activeTakeIndex, activeExposureIndex, false, selectedFiles[selectedFilesIndex]);
        activeExposureIndex++;
    }

    qDebug("ProjectTab::insertFramesSlot --> End");
}


void ProjectTab::addFramesSlot()
{
    qDebug("ProjectTab::addFramesSlot --> Start");

    QStringList selectedFiles = selectFiles();
    int selectedFilesCount = selectedFiles.count();
    if (0 == selectedFilesCount) {
        // The user didn't select one ore more files
        qDebug("ProjectTab::addFrameSlot --> End (no file selected)");
        return;
    }
    int selectedFilesIndex;
    int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
    int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
    for (selectedFilesIndex = 0 ; selectedFilesIndex < selectedFilesCount ; selectedFilesIndex++) {
        // Add the selected files
        frontend->getProject()->addExposureToUndo(activeSceneIndex, activeTakeIndex, selectedFiles[selectedFilesIndex]);
    }

    qDebug("ProjectTab::addFramesSlot --> End");
}


void ProjectTab::removeFramesSlot()
{
    qDebug("ProjectTab::removeFramesSlot --> Start");

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    if (-1 == exposureIndex) {
        // Selected item is not a exposure item
        qDebug("ProjectTab::removeFramesSlot --> End (Nothing)");
        return;
    }

    // TODO: Optinaly save the image to remove in a separate directory
    frontend->getProject()->removeExposureToUndo(sceneIndex, takeIndex, exposureIndex);

    qDebug("ProjectTab::removeFramesSlot --> End");
}


void ProjectTab::editFrameSlot()
{
    qDebug("ProjectTab::editFrameSlot --> Start");

    const QString gimpCommand = Util::checkCommand("gimp");
    if (gimpCommand.isEmpty()) {
        frontend->showWarning(tr("Warning"),
                              tr("You do not have Gimp installed on your system"));
        return;
    }

    // Determine the active scene and active frame.
    int activeScene = frontend->getProject()->getActiveSceneIndex();
    int activeTake = frontend->getProject()->getActiveTakeIndex();
    int activeExposure = frontend->getProject()->getActiveExposureIndex();

    if (activeScene < 0 || activeTake < 0 || activeExposure < 0) {
        frontend->showWarning(tr("Warning"),
                              tr("There is no active frame to open"));
        return;
    }

    Exposure *exposure = frontend->getProject()->getExposure(activeScene, activeTake, activeExposure);
    if (exposure->isEmpty()) {
        frontend->showWarning(tr("Warning"),
                              tr("The active frame is corrupt"));
        return;
    }

    QStringList argList;
    // arg0 are the options, and arg1 is the path of the frame.
    // Start Gimp without splash screen.
    argList.append(QLatin1String("--no-splash"));
    argList.append(exposure->getImagePath());

    QProcess process;
    if (!process.startDetached(gimpCommand, argList)) {
        frontend->showWarning(tr("Warning"),
                              tr("Failed to start Gimp!"));
        return;
    }

    qDebug("ProjectTab::editFrameSlot --> End");
}


void ProjectTab::retranslateStrings()
{
    qDebug("ProjectTab::retranslateStrings --> Start");

    projectGroupBox->setTitle(tr("Project Tree"));

    scenesGroupBox->setTitle(tr("Scenes"));

    QString infoText =
        tr("<h4>Insert new Scene (CTRL+E)</h4> "
           "<p>Click this button to create a new <em>scene</em> "
           "and <em>insert</em> it bevor the selected scene.</p>");
    insertSceneButton->setToolTip(infoText);
    insertSceneButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Append new Scene (CTRL+E)</h4> "
           "<p>Click this button to create a new <em>scene</em> "
           "and <em>append</em> it at the end of the animation.</p>");
    addSceneButton->setToolTip(infoText);
    addSceneButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Remove Scene (SHIFT+Delete)</h4> "
           "<p>Click this button to <em>remove</em> the selected <em>scene</em> "
           "from the animation.</p>");
    removeSceneButton->setToolTip(infoText);
    removeSceneButton->setWhatsThis(infoText);

    takesGroupBox->setTitle(tr("Takes"));

    infoText =
        tr("<h4>Insert new Take (CTRL+E)</h4> "
           "<p>Click this button to create a new <em>take</em> "
           "and <em>insert</em> it bevor the selected take.</p>");
    insertTakeButton->setToolTip(infoText);
    insertTakeButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Append new Take (CTRL+E)</h4> "
           "<p>Click this button to create a new <em>take</em> "
           "and <em>append</em> it to the end of the scene.</p>");
    addTakeButton->setToolTip(infoText);
    addTakeButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Remove Take (SHIFT+Delete)</h4> "
           "<p>Click this button to <em>remove</em> the selected <em>take</em> "
           "from the animation.</p>");
    removeTakeButton->setToolTip(infoText);
    removeTakeButton->setWhatsThis(infoText);

    framesGroupBox->setTitle(tr("Frames"));

    infoText =
        tr("<h4>Insert Frames (CTRL+F)</h4> "
           "<p>Click on this button to <em>insert frames</em> "
           "bevor the selected frame.</p>");
    insertFramesButton->setToolTip(infoText);
    insertFramesButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Append Frames (CTRL+F)</h4> "
           "<p>Click on this button to <em>append frames</em> at the "
           "end of the take.</p>");
    addFramesButton->setToolTip(infoText);
    addFramesButton->setWhatsThis(infoText);

    infoText =
        tr("<h4>Remove Frame (Delete)</h4> "
           "<p>Click this button to <em>remove</em> the selected <em>frame</em> "
           "from the take.</p>");
    removeFramesButton->setToolTip(infoText);
    removeFramesButton->setWhatsThis(infoText);

    editGroupBox->setTitle(tr("Edit"));

    infoText =
        tr("<h4>Launch Photo Editor</h4> "
           "<p>Click this button to open the active frame in the photo editor</p> "
           "<p>Note that you can also drag images from the frame bar and drop them on the photo editor</p>");
    editFrameButton->setToolTip(infoText);
    editFrameButton->setWhatsThis(infoText);

    qDebug("ProjectTab::retranslateStrings --> End");
}


void ProjectTab::chooseFrame()
{
}


bool ProjectTab::getSelectedItems(int &sceneIndex,
                                  int &takeIndex,
                                  int &exposureIndex)
{
    qDebug("ProjectTab::getSelectedItems --> Start");

    QTreeWidgetItem *projectItem = NULL;
    QTreeWidgetItem *exposureItem = NULL;
    QTreeWidgetItem *takeItem = NULL;
    QTreeWidgetItem *sceneItem = NULL;
    QList<QTreeWidgetItem *> selectedItems = projectTree->selectedItems();

    if (0 == selectedItems.count()) {
        return false;
    }

    if (1 == selectedItems.count()) {
        if (NULL == selectedItems[0]->parent()->parent()) {
            sceneItem = selectedItems[0];
        }
        else {
            if (NULL == selectedItems[0]->parent()->parent()->parent()) {
                takeItem = selectedItems[0];
            }
            else {
                exposureItem = selectedItems[0];
                takeItem = exposureItem->parent();
            }
            sceneItem = takeItem->parent();
        }
    }
    else {
        // The first selected item must be a scene item
        Q_ASSERT(NULL == selectedItems[0]->parent()->parent());
        sceneItem = selectedItems[0];
        if (1 < selectedItems.count()) {
            // The second selected item must be a take item
            Q_ASSERT(NULL == selectedItems[1]->parent()->parent()->parent());
            takeItem = selectedItems[1];
            if (2 < selectedItems.count()) {
                // The third selected item is a exposure item
                exposureItem = selectedItems[2];
            }
        }
    }

    projectItem = sceneItem->parent();
    sceneIndex = projectItem->indexOfChild(sceneItem);
    if (NULL == takeItem) {
        takeIndex = -1;
    }
    else {
        takeIndex = sceneItem->indexOfChild(takeItem);
    }
    if (NULL == exposureItem) {
        exposureIndex = -1;
    }
    else {
        exposureIndex = takeItem->indexOfChild(exposureItem);
    }

    qDebug("ProjectTab::getSelectedItems --> End");
    return true;
}


void ProjectTab::removeScene(int sceneIndex)
{
    qDebug("ProjectTab::removeScene --> Start");

    QTreeWidgetItem *projectItem = projectTree->topLevelItem(0);

    Q_ASSERT(sceneIndex < projectItem->childCount());

    QTreeWidgetItem *sceneItem = projectItem->child(sceneIndex);
    int takeCount = sceneItem->childCount();
    int takeIndex;
    for (takeIndex = takeCount - 1 ; takeIndex >= 0  ; takeIndex--) {
        removeTake(sceneItem, takeIndex);
    }
    projectItem->removeChild(sceneItem);
    delete sceneItem;

    qDebug("ProjectTab::removeScene --> End");
}


void ProjectTab::removeTake(QTreeWidgetItem *sceneItem,
                            int takeIndex)
{
    qDebug("ProjectTab::removeTake --> Start");

    Q_ASSERT(takeIndex < sceneItem->childCount());

    QTreeWidgetItem *takeItem = sceneItem->child(takeIndex);
    int exposureCount = takeItem->childCount();
    int exposureIndex;
    QTreeWidgetItem *exposureItem;
    for (exposureIndex = exposureCount - 1 ; exposureIndex >= 0  ; exposureIndex--) {
        exposureItem = takeItem->child(exposureIndex);
        takeItem->removeChild(exposureItem);
        delete exposureItem;
    }
    sceneItem->removeChild(takeItem);
    delete takeItem;

    qDebug("ProjectTab::removeTake --> End");
}


QStringList ProjectTab::selectFiles()
{
    qDebug("ProjectTab::selectFiles --> Start");

    fileDialog = new QFileDialog(this, tr("Choose frames to add"), lastVisitedDir);
    QStringList filters;
    filters << "Images (*.png *.jpg *.jpeg  *.gif *.PNG *.JPG *.JPEG *.GIF)"
            << "Joint Photographic Ex. Gr. (*.jpg *.jpeg *.JPG *.JPEG)"
            << "Portable Network Graphics (*.png *.PNG)"
            << "GIMP native (*.xcf *.XCF)"
            << "Tagged Image File Format (*.tif *.TIF)"
            << "Windows Bitmap (*.bmp *.BMP)"
            << "TrueVision Targa (*.tga *.TGA)"
            << "Portable Anymap (*.pnm *.PNM)"
            << "X11 Pixmap (*.xpm *.XPM)"
            << "ZSoft IBM PC Paintbrush (*.pcx *.PCX)"
            << "CompuServe Graph. Interch. Format (*.gif *.GIF)"
            << "Interleaved Bitmap (*.lbm *.iff *.LBM *.IFF)"
            << "All files (*)";
    fileDialog->setFilters(filters);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

    //PicturePreview* p = new PicturePreview(fileDialog);

    QObject::connect(fileDialog, SIGNAL(filesSelected(const QStringList &)),
                     this, SLOT(addFrames(const QStringList &)));

    int ret = fileDialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the file dialog
        qDebug("ProjectTab::addFrameSlot --> End (dialog cancel)");
        return QStringList();
    }

    lastVisitedDir.append(fileDialog->directory().path());

    qDebug("ProjectTab::selectFiles --> End");
    return fileDialog->selectedFiles();
}
