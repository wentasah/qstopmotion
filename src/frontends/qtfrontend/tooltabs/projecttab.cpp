/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QList>
#include <QProcess>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/dialogs/descriptiondialog.h"
#include "technical/util.h"


ProjectTab::ProjectTab(Frontend *f,
                       const QString &lvd,
                       QWidget *parent) :
    QWidget(parent)
{
    qDebug() << "ProjectTab::Constructor --> Start";

    frontend            = f;
    if (!lvd.isEmpty()) {
      lastVisitedDir.append(lvd);
    }

    projectGroupBox     = 0;
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

    activeSceneIndex    = -1;
    activeTakeIndex     = -1;
    activeExposureIndex = -1;

    gimpProcess         = NULL;

    this->setObjectName("ProjectTab");

    makeGUI();
    retranslateStrings();
    enableTab();
    frontend->getView()->attatch(this);

    qDebug() << "ProjectTab::Constructor --> End";
}


ProjectTab::~ProjectTab()
{
    qDebug() << "ProjectTab::Destructor --> Start";


    if (NULL != gimpProcess) {
        gimpProcess->close();
        delete gimpProcess;
        gimpProcess = NULL;
    }

    qDebug() << "ProjectTab::Destructor --> End";
}


void ProjectTab::makeGUI()
{
    qDebug() << "ProjectTab::makeGUI --> Start";

    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;

    // ========================================================================
    // Project tree
    projectGroupBox = new QGroupBox("projectGroupBox");
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
    scenesGroupBox = new QGroupBox("scenesGroupBox");
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
    takesGroupBox = new QGroupBox("takesGroupBox");
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
    framesGroupBox = new QGroupBox("framesGroupBox");
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
    editGroupBox = new QGroupBox("editGroupBox");
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

    qDebug() << "ProjectTab::makeGUI --> End";
}


void ProjectTab::retranslateStrings()
{
    qDebug() << "ProjectTab::retranslateStrings --> Start";

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

    qDebug() << "ProjectTab::retranslateStrings --> End";
}


void ProjectTab::initialize()
{
    qDebug() << "ProjectTab::initialize --> Start";

    /*
    PreferencesTool *pref = PreferencesTool::get();
    int active = -1;

    int numEncoders = pref->getNumberEncoders();

    encoderTable->setRowCount(numEncoders);

    if (numEncoders > 0 ) {
        QString activeId = pref->getActiveEncoder();
        int idx = 0;

        for (int i = 0; i < numEncoders; ++i) {
            QString id(QString("%1%2").arg(QLatin1String("encoder")).arg(i));
            QString name;
            QString desc;
            QString start;
            QString stop;
            QString filename;

            if ( pref->getEncoder(id, name, desc, start, stop, filename) ) {
                encoderTable->setItem(i, 0, new QTableWidgetItem(name) );
                encoderTable->setItem(i, 1, new QTableWidgetItem(desc) );
                startEncoderStrings.append(start);
                stopEncoderStrings.append(stop);
                outputFiles.append(filename);

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

    qDebug() << "ProjectTab::initialize --> End";
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
    editFrameButton->setEnabled(true);

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
    qDebug() << "ProjectTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug() << "ProjectTab::resizeEvent --> End");
}
*/

void ProjectTab::apply()
{
    qDebug() << "ProjectTab::apply --> Start";

    /*
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("%1%2").arg(QLatin1String("encoder")).arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("%1%2").arg(QLatin1String("encoder")).arg(i),
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

    qDebug() << "ProjectTab::apply --> End";
}


/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void ProjectTab::updateRemoveProject()
{
    qDebug() << "ProjectTab::updateRemoveProject --> Start";

    projectTree->clear();
    activeSceneIndex = -1;
    activeTakeIndex = -1;
    activeExposureIndex = -1;

    qDebug() << "ProjectTab::updateRemoveProject --> End";
}


void ProjectTab::updateNewProject()
{
    qDebug() << "ProjectTab::updateNewProject --> Start";

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(projectTree);
    topLevelItem->setText(0, this->frontend->getProject()->getProjectDescription());
    topLevelItem->setFlags(Qt::ItemIsEnabled);
    projectTree->insertTopLevelItem(0, topLevelItem);

    qDebug() << "ProjectTab::updateNewProject --> End";
}


void ProjectTab::updateOpenProject()
{
    qDebug() << "ProjectTab::updateOpenProject --> Start";

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(projectTree);
    int sceneCount = frontend->getProject()->getSceneSize();

    topLevelItem->setText(0, frontend->getProject()->getProjectDescription());
    topLevelItem->setFlags(Qt::ItemIsEnabled);
    projectTree->insertTopLevelItem(0, topLevelItem);

    for (int sceneIndex = 0; sceneIndex < sceneCount ; sceneIndex++) {
        updateAddScene(sceneIndex);
        int takeCount = frontend->getProject()->getSceneTakeSize(sceneIndex);
        for (int takeIndex = 0; takeIndex < takeCount ; takeIndex++) {
            updateAddTake(sceneIndex, takeIndex);
            int exposureCount = frontend->getProject()->getSceneTakeExposureSize(sceneIndex, takeIndex);
            for (int exposureIndex = 0; exposureIndex < exposureCount ; exposureIndex++) {
                updateAddExposure(sceneIndex, takeIndex, exposureIndex);
            }
        }
    }

    qDebug() << "ProjectTab::updateOpenProject --> End";
}


void ProjectTab::updatePlaySound(int) {}


void ProjectTab::updateMixMode(int) {}


void ProjectTab::updateMixCount(int) {}


void ProjectTab::updateLiveViewFps(int) {}


void ProjectTab::updateVideoFps(int) {}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void ProjectTab::updateAddScene(int sceneIndex)
{
    qDebug() << "ProjectTab::updateAddScene --> Start";

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

    qDebug() << "ProjectTab::updateAddScene --> End";
}


void ProjectTab::updateInsertScene(int sceneIndex)
{
    qDebug() << "ProjectTab::updateInsertScene --> Start";

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

    activeSceneIndex++;

    qDebug() << "ProjectTab::updateInsertScene --> End";
}

/*
void ProjectTab::updateSetNewActiveScene(int sceneIndex)
{
    qDebug() << "ProjectTab::updatSeteNewActiveScene --> Start");

    this->setNewActiveScene(sceneIndex);

    qDebug() << "ProjectTab::updatSeteNewActiveScene --> End");
}
*/

void ProjectTab::updateActivateScene()
{
    qDebug() << "ProjectTab::updateActivateScene --> Start";

    int newSceneIndex = frontend->getProject()->getActiveSceneIndex();

    Q_ASSERT(newSceneIndex != activeSceneIndex);

    this->unsetActiveItems();

    activeSceneIndex = newSceneIndex;
    activeTakeIndex = -1;
    activeExposureIndex = -1;

    this->setActiveItems();

    qDebug() << "ProjectTab::updateActivateScene --> End";
}


void ProjectTab::updateRemoveScene(int sceneIndex)
{
    qDebug() << "ProjectTab::updateRemoveScene --> Start";

    this->unsetActiveItems();

    QTreeWidgetItem *topLevelItem = this->projectTree->topLevelItem(0);
    Q_ASSERT(sceneIndex < topLevelItem->childCount());
    this->removeScene(sceneIndex);

    if (sceneIndex < activeSceneIndex) {
        activeSceneIndex--;
        Q_ASSERT(-1 < activeSceneIndex);
    }
    if (-1 == activeSceneIndex) {
        activeTakeIndex = -1;
        activeExposureIndex = -1;
    }
    else {
        activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
        if (-1 == activeTakeIndex) {
            activeExposureIndex = -1;
        }
        else {
            activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
        }
    }

    this->setActiveItems();

    qDebug() << "ProjectTab::updateRemoveScene --> End";
}


void ProjectTab::updateMoveScene(int sceneNumber,
                                 int movePosition)
{
    qDebug() << "ProjectTab::updateMoveScene --> Start";

    // this->moveScene(sceneNumber, movePosition);

    qDebug() << "ProjectTab::updateMoveScene --> End";
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void ProjectTab::updateAddTake(int sceneIndex,
                               int takeIndex)
{
    qDebug() << "ProjectTab::updateAddTake --> Start";

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

    qDebug() << "ProjectTab::updateAddTake --> End";
}

void ProjectTab::updateInsertTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug() << "ProjectTab::updateInsertTake --> Start";

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

    activeTakeIndex++;

    qDebug() << "ProjectTab::updateInsertTake --> End";
}

/*
void ProjectTab::updateSetNewActiveTake(int takeIndex)
{
    qDebug() << "ProjectTab::updateSetNewActiveTake --> Start";

    this->setSetNewActiveTake(takeIndex);

    qDebug() << "ProjectTab::updateSetNewActiveTake --> End";
}
*/

void ProjectTab::updateActivateTake()
{
    qDebug() << "ProjectTab::updateActivateTake --> Start";

    int newTakeIndex = frontend->getProject()->getActiveTakeIndex();

    Q_ASSERT(newTakeIndex != activeTakeIndex);

    this->unsetActiveItems();

    activeTakeIndex = newTakeIndex;
    activeExposureIndex = -1;

    this->setActiveItems();

    qDebug() << "ProjectTab::updateActivateTake --> End";
}


void ProjectTab::updateRemoveTake(int sceneIndex,
                                  int takeIndex)
{
    qDebug() << "ProjectTab::updateRemoveTake --> Start";

    this->unsetActiveItems();

    QTreeWidgetItem *topLevelItem = this->projectTree->topLevelItem(0);
    Q_ASSERT(sceneIndex < topLevelItem->childCount());
    QTreeWidgetItem *activeSceneItem = topLevelItem->child(sceneIndex);
    this->removeTake(activeSceneItem, takeIndex);

    if (sceneIndex == activeSceneIndex) {
        // One of the takes of the active scene are removed
        if (takeIndex < activeTakeIndex) {
            activeTakeIndex--;
            Q_ASSERT(-1 < activeTakeIndex);
        }

        if (-1 == activeTakeIndex) {
            activeExposureIndex = -1;
        }
        else {
            activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
        }
    }

    this->setActiveItems();

    qDebug() << "ProjectTab::updateRemoveTake --> End";
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void ProjectTab::updateAddExposure(int sceneIndex,
                                   int takeIndex,
                                   int exposureIndex)
{
    // qDebug() << "ProjectTab::updateAddExposure --> Start";

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

    // qDebug() << "ProjectTab::updateAddExposure --> End";
}


void ProjectTab::updateInsertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug() << "ProjectTab::updateInsertExposure --> Start";

    // Q_ASSERT(-1 < activeExposureIndex);

    Exposure *exposure = frontend->getProject()->getScene(sceneIndex)->getTake(takeIndex)->getExposure(exposureIndex);

    QTreeWidgetItem *takeItem = this->projectTree->topLevelItem(0)->child(sceneIndex)->child(takeIndex);
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, exposure->getId());
    newItem->setFlags(Qt::ItemIsEnabled |
                      Qt::ItemIsSelectable);
    takeItem->insertChild(exposureIndex, newItem);

    if (exposureIndex <= activeExposureIndex) {
        activeExposureIndex++;
    }

    qDebug() << "ProjectTab::updateInsertExposure --> End";
}


void ProjectTab::updateRemoveExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex)
{
    qDebug() << "ProjectTab::updateRemoveExposure --> Start";

    unsetActiveItems();

    QTreeWidgetItem *activeSceneItem = this->projectTree->topLevelItem(0)->child(sceneIndex);
    QTreeWidgetItem *activeTakeItem = activeSceneItem->child(takeIndex);
    QTreeWidgetItem *activeExposureItem = activeTakeItem->child(exposureIndex);
    activeTakeItem->removeChild(activeExposureItem);
    delete activeExposureItem;

    if (sceneIndex == activeSceneIndex) {
        if (takeIndex == activeTakeIndex) {
            if (exposureIndex < activeExposureIndex) {
                activeExposureIndex--;
            }
        }
    }

    setActiveItems();

    qDebug() << "ProjectTab::updateRemoveExposure --> End";
}


void ProjectTab::updateMoveExposures(int fromFrame,
                                     int toFrame,
                                     int movePosition)
{
    qDebug() << "ProjectTab::updateMoveExposures --> Start";

    // moveExposures(fromFrame, toFrame, movePosition);

    qDebug() << "ProjectTab::updateMoveExposures --> End";
}

/*
void ProjectTab::updateSetNewActiveExposure(int exposureIndex)
{
    qDebug() << "ProjectTab::updateSetNewActiveExposure --> Start";

    setActiveFrame(exposureIndex);

    if (preferencesMenu->isVisible()) {
        if (exposureIndex >= 0) {
            showPreferencesMenu();
        } else {
            preferencesMenu->close();
        }
    }

    // For writing the frame number in the frame number display
    emit newActiveFrame(tr("Exposure number: ") + QString("%1").arg(exposureIndex + 1));

    // For setting the value in the spinbox in the gotomenu
    emit newActiveFrame(exposureIndex + 1);

    qDebug() << "ProjectTab::updateSetNewActiveExposure --> End";
}
*/

void ProjectTab::updateActivateExposure()
{
    qDebug() << "ProjectTab::updateActivateExposure --> Start";

    int newExposureIndex = frontend->getProject()->getActiveExposureIndex();

    Q_ASSERT(newExposureIndex != activeExposureIndex);

    this->unsetActiveItems();

    activeExposureIndex = newExposureIndex;

    this->setActiveItems();

    qDebug() << "ProjectTab::updateActivateExposure --> End";
}


void ProjectTab::updateModifyExposure(int modSceneIndex,
                                      int modTakeIndex,
                                      int modExposureIndex)
{
    qDebug() << "ProjectTab::updateModifyExposure --> Start";

    // modifyExposure();

    qDebug() << "ProjectTab::updateModifyExposure --> End";
}


/**************************************************************************
 * Other functions
 **************************************************************************/

void ProjectTab::unsetActiveItems()
{
    qDebug() << "ProjectTab::unsetActiveExposure --> Start";

    QList<QTreeWidgetItem*> selectedItems = this->projectTree->selectedItems();
    int selectedItemsCount = selectedItems.count();
    int selectedItemsIndex;

    for (selectedItemsIndex = 0 ; selectedItemsIndex < selectedItemsCount ; selectedItemsIndex++) {
        selectedItems[selectedItemsIndex]->setSelected(false);
        // selectedItems[selectedItemsIndex]->setExpanded(false);
        // projectTree->collapseItem(selectedItems[selectedItemsIndex]);
    }
    projectTree->collapseAll();

    qDebug() << "ProjectTab::unsetActiveExposure --> End";
}


void ProjectTab::setActiveItems()
{
    qDebug() << "ProjectTab::setActiveExposure --> Start";

    QTreeWidgetItem *newExposureItem = NULL;
    QTreeWidgetItem *newTakeItem = NULL;
    QTreeWidgetItem *newSceneItem = NULL;
    QTreeWidgetItem *newProjectItem = projectTree->topLevelItem(0);
    newProjectItem->setExpanded(true);
    newProjectItem->setSelected(true);

    if (0 > activeSceneIndex) {
        qDebug() << "ProjectTab::setActiveExposure --> End (Only project)";
        return;
    }
    newSceneItem = newProjectItem->child(activeSceneIndex);
    newSceneItem->setExpanded(true);
    newSceneItem->setSelected(true);
    projectTree->scrollToItem(newSceneItem);

    if (0 > activeTakeIndex) {
        qDebug() << "ProjectTab::setActiveExposure --> End (Up to scene)";
        return;
    }
    newTakeItem = newSceneItem->child(activeTakeIndex);
    newTakeItem->setExpanded(true);
    newTakeItem->setSelected(true);
    projectTree->scrollToItem(newTakeItem);

    if (0 > activeExposureIndex) {
        qDebug() << "ProjectTab::setActiveExposure --> End (Up to take)";
        return;
    }
    newExposureItem = newTakeItem->child(activeExposureIndex);
    newExposureItem->setSelected(true);
    projectTree->scrollToItem(newExposureItem);

    qDebug() << "ProjectTab::setActiveExposure --> End (All)";
}


void ProjectTab::itemClicked(QTreeWidgetItem * /*exposureItem*/,
                             int               /*column*/)
{
    qDebug() << "ProjectTab::itemClicked --> Start";

    int    newSceneIndex;
    int    newTakeIndex;
    int    newExposureIndex;
    Scene *scene = NULL;
    Take  *take = NULL;

    if (!getSelectedItems(newSceneIndex, newTakeIndex, newExposureIndex)) {
        // Project item selected
        setActiveItems();
        return;
    }

    if (activeSceneIndex != newSceneIndex) {
        frontend->getProject()->selectSceneToUndo(newSceneIndex);
        activeTakeIndex = -1;
        activeExposureIndex = -1;
    }

    scene = frontend->getProject()->getScene(activeSceneIndex);
    if (-1 == newTakeIndex) {
        // Scene item selected
        newTakeIndex = scene->getActiveTakeIndex();
    }

    if (activeTakeIndex != newTakeIndex) {
        if (newTakeIndex != frontend->getProject()->getActiveTakeIndex()) {
            frontend->getProject()->selectTakeToUndo(newSceneIndex, newTakeIndex);
            qDebug() << "ProjectTab::itemClicked --> End (Scene or Take selected)";
            return;
        }
        activeTakeIndex = newTakeIndex;
        activeExposureIndex = -1;
    }

    if (-1 == newExposureIndex) {
        if (0 <= newTakeIndex) {
            // Take item selected
            take = scene->getTake(newTakeIndex);
            newExposureIndex = take->getActiveExposureIndex();
        }
    }

    if (activeExposureIndex != newExposureIndex) {
        if (newExposureIndex != frontend->getProject()->getActiveExposureIndex()) {
            frontend->getProject()->selectExposureToUndo(newSceneIndex, newTakeIndex, newExposureIndex);
            qDebug() << "ProjectTab::itemClicked --> End (Scene, Take or exposure selected)";
            return;
        }
        activeExposureIndex = newExposureIndex;
    }

    this->setActiveItems();

    qDebug() << "ProjectTab::itemClicked --> End (Nothing new selected)";
}


void ProjectTab::itemDoubleClicked(QTreeWidgetItem* /*exposureItem*/,
                                   int              /*column*/)
{
    qDebug() << "ProjectTab::itemDoubleClicked --> Start";

    int sceneIndex;
    int takeIndex;
    int exposureIndex;

    getSelectedItems(sceneIndex, takeIndex, exposureIndex);

    if (-1 == exposureIndex) {
        // Clicked item is not a exposure item
        return;
    }

    frontend->getProject()->selectExposureToUndo(sceneIndex, takeIndex, exposureIndex);

    qDebug() << "ProjectTab::itemDoubleClicked --> End";
}


void ProjectTab::itemSelectionChanged()
{
    qDebug() << "ProjectTab::itemSelectionChanged --> Start";
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
    qDebug() << "ProjectTab::itemSelectionChanged --> End";
}


void ProjectTab::insertSceneSlot()
{
    qDebug() << "ProjectTab::insertSceneSlot --> Start";

    DescriptionDialog *dialog = new DescriptionDialog(frontend, DescriptionDialog::SceneDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectDescription());
    dialog->setSceneDescription(tr("Scene 000"));
    dialog->setTakeDescription(tr("Take 00"));
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug() << "ProjectTab::addTakeSlot --> End (cancel)";
        return;
    }

    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    int newSceneIndex = activeSceneIndex;

    if (-1 == newSceneIndex) {
        frontend->getProject()->addSceneToUndo(sceneDescription);
        newSceneIndex = 0;
    }
    else {
        frontend->getProject()->insertSceneToUndo(sceneDescription, newSceneIndex);
    }
    frontend->getProject()->addTakeToUndo(takeDescription, newSceneIndex);

    qDebug() << "ProjectTab::insertSceneSlot --> End";
}


void ProjectTab::addSceneSlot()
{
    qDebug() << "ProjectTab::addSceneSlot --> Start";

    DescriptionDialog *dialog = new DescriptionDialog(frontend, DescriptionDialog::SceneDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectDescription());
    dialog->setSceneDescription(tr("Scene 000"));
    dialog->setTakeDescription(tr("Take 00"));
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug() << "ProjectTab::addTakeSlot --> End (cancel)";
        return;
    }

    QString sceneDescription = dialog->getSceneDescription();
    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    int newSceneIndex = frontend->getProject()->getSceneSize();

    frontend->getProject()->addSceneToUndo(sceneDescription);

    frontend->getProject()->addTakeToUndo(takeDescription, newSceneIndex);

    qDebug() << "ProjectTab::addSceneSlot --> End";
}


void ProjectTab::removeSceneSlot()
{
    qDebug() << "ProjectTab::removeSceneSlot --> Start";

    if (-1 == activeSceneIndex) {
        // No scene selected
        qDebug() << "ProjectTab::removeSceneSlot --> End (Nothing)";
        return;
    }

    DomainFacade *animationProject = frontend->getProject();

    if (-1 == animationProject->getActiveScene()->getActiveTakeIndex()) {
        // There is no active take in this scene

        removeSceneTakes(activeSceneIndex);
    }
    else {
        // There is a active take in this scene

        // Remove of all exposures using the remove frame slot
        while (-1 < activeTakeIndex) {
            removeTakeSlot();
        }
    }

    int removeSceneIndex = activeSceneIndex;  // The activeSceneIndex will be changed by the selectSceneToUndo call

    if (removeSceneIndex == (animationProject->getSceneSize()-1)) {
        // Last scene of the project selected
        animationProject->selectSceneToUndo(removeSceneIndex-1);
    }
    else {
        // Not the last scene of the project selected
        animationProject->selectSceneToUndo(removeSceneIndex+1);
    }

    frontend->getProject()->removeSceneToUndo(removeSceneIndex);

    qDebug() << "ProjectTab::removeSceneSlot --> End";
}


void ProjectTab::removeSceneTakes(int removeSceneIndex)
{
    qDebug() << "ProjectTab::removeSceneTakes --> Start";

    DomainFacade *animationProject = frontend->getProject();

    // Remove of all takes
    for (int removeTakeIndex = animationProject->getSceneTakeSize(removeSceneIndex)-1 ; 0 <= removeTakeIndex ; removeTakeIndex--) {
        removeTakeExposures(removeSceneIndex, removeTakeIndex);
        animationProject->removeTakeToUndo(removeSceneIndex, removeTakeIndex);
    }

    qDebug() << "ProjectTab::removeSceneTakes --> End";
}


void ProjectTab::insertTakeSlot()
{
    qDebug() << "ProjectTab::insertTakeSlot --> Start";

    if (0 > activeSceneIndex) {
        // No scene selected
        frontend->showInformation(tr("Insert Take"), tr("No scene selected. Please select a scene in the project tree."));
        return;
    }

    DescriptionDialog *dialog = new DescriptionDialog(frontend, DescriptionDialog::TakeDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectDescription());
    dialog->setSceneDescription(frontend->getProject()->getActiveScene()->getDescription());
    dialog->setTakeDescription(tr("Take 00"));
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug() << "ProjectTab::addTakeSlot --> End (cancel)";
        return;
    }

    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    if (-1 == activeTakeIndex) {
        frontend->getProject()->addTakeToUndo(takeDescription, activeSceneIndex);
    }
    else {
        frontend->getProject()->insertTakeToUndo(takeDescription, activeSceneIndex, activeTakeIndex);
    }

    qDebug() << "ProjectTab::insertTakeSlot --> End";
}


void ProjectTab::addTakeSlot()
{
    qDebug() << "ProjectTab::addTakeSlot --> Start";

    if (0 > activeSceneIndex) {
        // No scene selected
        frontend->showInformation(tr("Add Take"), tr("No scene selected. Please select a scene in the project tree."));
        return;
    }

    DescriptionDialog *dialog = new DescriptionDialog(frontend, DescriptionDialog::TakeDescription);
    dialog->setProjectDescription(frontend->getProject()->getProjectDescription());
    dialog->setSceneDescription(frontend->getProject()->getActiveScene()->getDescription());
    dialog->setTakeDescription(tr("Take 00"));
    int ret = dialog->exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the input dialog
        qDebug() << "ProjectTab::addTakeSlot --> End (cancel)";
        return;
    }

    QString takeDescription = dialog->getTakeDescription();

    delete(dialog);

    frontend->getProject()->addTakeToUndo(takeDescription, activeSceneIndex);

    qDebug() << "ProjectTab::addTakeSlot --> End";
}


void ProjectTab::removeTakeSlot()
{
    qDebug() << "ProjectTab::removeTakeSlot --> Start";

    if (-1 == activeTakeIndex) {
        // No take selected
        qDebug() << "ProjectTab::removeTakeSlot --> End (Nothing)";
        return;
    }

    DomainFacade *animationProject = frontend->getProject();

    if (-1 == animationProject->getActiveTake()->getActiveExposureIndex()) {
        // There is no active exposure in this take

        removeTakeExposures(activeSceneIndex, activeTakeIndex);
    }
    else {
        // There is a active exposure in this take

        // Remove of all exposures using the remove frame slot
        while (-1 < activeExposureIndex) {
            removeFramesSlot();
        }
    }

    int removeTakeIndex = activeTakeIndex;  // The activeTakeIndex will be changed by the selectTakeToUndo call

    if (removeTakeIndex == (animationProject->getSceneTakeSize(activeSceneIndex)-1)) {
        // Last take of the scene selected
        animationProject->selectTakeToUndo(activeSceneIndex, removeTakeIndex-1);
    }
    else {
        // Not the last take of the scene selected
        animationProject->selectTakeToUndo(activeSceneIndex, removeTakeIndex+1);
    }

    frontend->getProject()->removeTakeToUndo(activeSceneIndex, removeTakeIndex);

    qDebug() << "ProjectTab::removeTakeSlot --> End";
}


void ProjectTab::removeTakeExposures(int removeSceneIndex, int removeTakeIndex)
{
    qDebug() << "ProjectTab::removeTakeExposures --> Start";

    DomainFacade *animationProject = frontend->getProject();

    // Remove of all exposures
    for (int removeExposureIndex = animationProject->getSceneTakeExposureSize(removeSceneIndex, removeTakeIndex)-1 ; 0 <= removeExposureIndex ; removeExposureIndex--) {
        animationProject->removeExposureToUndo(removeSceneIndex, removeTakeIndex, removeExposureIndex);
    }

    qDebug() << "ProjectTab::removeTakeExposures --> End";
}


void ProjectTab::insertFramesSlot()
{
    qDebug() << "ProjectTab::insertFramesSlot --> Start";

    if (0 > activeSceneIndex) {
        // No scene selected
        frontend->showInformation(tr("Insert Frames"), tr("No scene selected. Please select a scene in the project tree."));
        return;
    }
    if (0 > activeTakeIndex) {
        // No scene selected
        frontend->showInformation(tr("Insert Frames"), tr("No take selected. Please select a take in the project tree."));
        return;
    }

    QStringList selectedFiles = selectFiles();
    int selectedFilesCount = selectedFiles.count();
    if (0 == selectedFilesCount) {
        // The user didn't select one ore more files
        qDebug() << "ProjectTab::addFrameSlot --> End (no file selected)";
        return;
    }
    int selectedFilesIndex;
    for (selectedFilesIndex = 0 ; selectedFilesIndex < selectedFilesCount ; selectedFilesIndex++) {
        // insert the selected files
        QImage rawImage(selectedFiles[selectedFilesIndex]);
        if (-1 == activeExposureIndex) {
            frontend->getProject()->addExposureToUndo(rawImage, activeSceneIndex, activeTakeIndex);
        }
        else {
            frontend->getProject()->insertExposureToUndo(rawImage, activeSceneIndex, activeTakeIndex, activeExposureIndex);
        }
    }

    qDebug() << "ProjectTab::insertFramesSlot --> End";
}


void ProjectTab::addFramesSlot()
{
    qDebug() << "ProjectTab::addFramesSlot --> Start";

    if (0 > activeSceneIndex) {
        // No scene selected
        frontend->showInformation(tr("Add Frames"), tr("No scene selected. Please select a scene in the project tree."));
        return;
    }
    if (0 > activeTakeIndex) {
        // No scene selected
        frontend->showInformation(tr("Add Frames"), tr("No take selected. Please select a take in the project tree."));
        return;
    }

    QStringList selectedFiles = selectFiles();
    int selectedFilesCount = selectedFiles.count();
    if (0 == selectedFilesCount) {
        // The user didn't select one ore more files
        qDebug() << "ProjectTab::addFrameSlot --> End (no file selected)";
        return;
    }
    int selectedFilesIndex;
    for (selectedFilesIndex = 0 ; selectedFilesIndex < selectedFilesCount ; selectedFilesIndex++) {
        // Add the selected files
        QImage rawImage(selectedFiles[selectedFilesIndex]);
        frontend->getProject()->addExposureToUndo(rawImage, activeSceneIndex, activeTakeIndex);
    }

    qDebug() << "ProjectTab::addFramesSlot --> End";
}


void ProjectTab::removeFramesSlot()
{
    qDebug() << "ProjectTab::removeFramesSlot --> Start";

    if (-1 == activeExposureIndex) {
        // No exposure selected
        qDebug() << "ProjectTab::removeFramesSlot --> End (Nothing)";
        return;
    }

    DomainFacade *animationProject = frontend->getProject();
    int removeExposureIndex = activeExposureIndex;  // The activeExposureIndex will be changed by the selectExposureToUndo call

    if (removeExposureIndex == (animationProject->getTakeExposureSize(activeSceneIndex, activeTakeIndex)-1)) {
        // Last exposure of the take selected
        animationProject->selectExposureToUndo(activeSceneIndex, activeTakeIndex, removeExposureIndex-1);
    }
    else {
        // Not the last exposure of the take selected
        animationProject->selectExposureToUndo(activeSceneIndex, activeTakeIndex, removeExposureIndex+1);
    }

    // TODO: Optinaly save the image to remove in a separate directory
    animationProject->removeExposureToUndo(activeSceneIndex, activeTakeIndex, removeExposureIndex);

    qDebug() << "ProjectTab::removeFramesSlot --> End";
}


void ProjectTab::editFrameSlot()
{
    qDebug() << "ProjectTab::editFrameSlot --> Start";

    // Determine the active scene and active frame.
    if (activeSceneIndex < 0 || activeTakeIndex < 0 || activeExposureIndex < 0) {
        frontend->showWarning(tr("Warning"),
                              tr("There is no active frame to open"));
        return;
    }

    Exposure *exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, activeExposureIndex);
    if (exposure->isEmpty()) {
        frontend->showWarning(tr("Warning"),
                              tr("The active frame is corrupt"));
        return;
    }
    QString exposureImagePath(exposure->getImagePath());

    frontend->addFileToMonitoring(exposureImagePath);

    if (NULL == gimpProcess) {
        startGimpProcess(exposureImagePath);
    }

    qDebug() << "ProjectTab::editFrameSlot --> End";
}


void ProjectTab::gimpProcessStarted()
{
    qDebug() << "ProjectTab::gimpProcessStarted --> Start (Nothing)";

    // qDebug() << "ProjectTab::gimpProcessStarted --> End";
}


void ProjectTab::gimpProcessError(QProcess::ProcessError error)
{
    qDebug() << "ProjectTab::gimpProcessError --> Start (Nothing)";

    // qDebug() << "ProjectTab::gimpProcessError --> End";
}


void ProjectTab::gimpProcessFinished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    qDebug() << "ProjectTab::gimpProcessFinished --> Start";

    stopGimpProcess();

    frontend->removeAllFilesFromMonitoring();

    qDebug() << "ProjectTab::gimpProcessFinished --> End";
}
void ProjectTab::chooseFrame()
{
}


bool ProjectTab::getSelectedItems(int &sceneIndex,
                                  int &takeIndex,
                                  int &exposureIndex)
{
    qDebug() << "ProjectTab::getSelectedItems --> Start";

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

    qDebug() << "ProjectTab::getSelectedItems --> End";
    return true;
}


void ProjectTab::removeScene(int sceneIndex)
{
    qDebug() << "ProjectTab::removeScene --> Start";

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

    qDebug() << "ProjectTab::removeScene --> End";
}


void ProjectTab::removeTake(QTreeWidgetItem *sceneItem,
                            int takeIndex)
{
    qDebug() << "ProjectTab::removeTake --> Start";

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

    qDebug() << "ProjectTab::removeTake --> End";
}


QStringList ProjectTab::selectFiles()
{
    qDebug() << "ProjectTab::selectFiles --> Start";

    QFileDialog fileDialog(this, tr("Choose frames to add"), lastVisitedDir);
    QStringList filters;
    filters << "Images (*.bmp *.png *.jpg *.tif *.gif *.BMP *.PNG *.JPG *.TIF *.GIF)"
            << "Windows Bitmap (*.bmp *.BMP)"
            << "Graphic Interchange Format (*.gif *.GIF)"
            << "Joint Photographic Experts Group (*.jpg *.jpeg *.JPG *.JPEG)"
            << "Portable Network Graphics (*.png *.PNG)"
            << "Portable Bitmap (*.pbm *.PBM)"
            << "Portable Pixmap (*.ppm *.PPM)"
            << "Tagged Image File Format (*.tif *.tiff *.TIF *.TIFF)"
            << "X11 Bitmap (*.xbm *.XBM)"
            << "X11 Pixmap (*.xpm *.XPM)"
            << "All files (*)";
    fileDialog.setNameFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    //PicturePreview* p = new PicturePreview(fileDialog);

    // QObject::connect(fileDialog, SIGNAL(filesSelected(const QStringList &)),
    //                  this, SLOT(addFrames(const QStringList &)));

    int ret = fileDialog.exec();
    if (ret == QDialog::Rejected) {
        // The user canceled the file dialog
        qDebug() << "ProjectTab::addFrameSlot --> End (dialog cancel)";
        return QStringList();
    }

    lastVisitedDir.append(fileDialog.directory().path());

    qDebug() << "ProjectTab::selectFiles --> End";
    return fileDialog.selectedFiles();
}


void ProjectTab::startGimpProcess(const QString &exposureImagePath)
{
    qDebug() << "ProjectTab::startGimpProcess --> Start";

    Q_ASSERT(NULL == gimpProcess);

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Windows version
    const QString gimpCommand(Util::checkCommand("gimp-?.?"));
#else
    // Linux and Apple OS X version
    const QString gimpCommand = Util::checkCommand("gimp");
#endif

    if (gimpCommand.isEmpty()) {
        frontend->showWarning(tr("Warning"),
                              tr("You do not have Gimp installed on your system"));
        return;
    }

    QStringList argList;
    // arg0 are the options, and arg1 is the path of the frame.
    // Start Gimp without splash screen.
    argList.append(QLatin1String("--no-splash"));
    argList.append(exposureImagePath);

    /*
    // Start the process with communication.
    gimpProcess = new QProcess(this);

    connect(gimpProcess, SIGNAL(started()), this, SLOT(gimpProcessStarted()));
    connect(gimpProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(gimpProcessError(QProcess::ProcessError)));
    connect(gimpProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(gimpProcessFinished(int, QProcess::ExitStatus)));

    gimpProcess->start(gimpCommand, argList);
    gimpProcess->waitForStarted();
    switch(gimpProcess->state()) {
    case QProcess::NotRunning:
        qDebug() << "ProjectTab::startGimpProcess --> Gimp Process not running");
        break;
    case QProcess::Starting:
        qDebug() << "ProjectTab::startGimpProcess --> Gimp Process starting");
        break;
    case QProcess::Running:
        qDebug() << "ProjectTab::startGimpProcess --> Gimp process running");
        break;
    }
    */

    QProcess process;
    if (!process.startDetached(gimpCommand, argList)) {
        frontend->showWarning(tr("Warning"),
                              tr("Failed to start Gimp!"));
        frontend->removeFileFromMonitoring(exposureImagePath);
        qDebug() << "ProjectTab::startGimpProcess --> End (Error)";
        return;
    }

    qDebug() << "ProjectTab::startGimpProcess --> End";
}


void ProjectTab::stopGimpProcess()
{
    qDebug() << "ProjectTab::stopGimpProcess --> Start";

    /*
    Q_ASSERT(NULL != gimpProcess);

    gimpProcess->close();
    delete gimpProcess;
    gimpProcess = NULL;
    */

    qDebug() << "ProjectTab::stopGimpProcess --> End";
}
