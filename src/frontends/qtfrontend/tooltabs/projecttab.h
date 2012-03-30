/***************************************************************************
 *   Copyright (C) 2010-2012 by Ralf Lange                                 *
 *   ralf.lange@longsoft.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PROJECTTAB_H
#define PROJECTTAB_H

#include "frontends/frontend.h"
#include "frontends/observer.h"

#include <QtCore/QProcess>
#include <QtGui/QFileDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>


/**
 * The project tab on the right hand side of the preview window
 * @author Ralf Lange
 */
class ProjectTab : public QWidget, public Observer
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param f the frontend of the application
     * @param lvd the last visited directory
     * @param parent the parent of the this widget
     */
    ProjectTab(Frontend *f,
               const QString &lvd,
               QWidget *parent = 0);

    /**
     * Descructor
     */
    ~ProjectTab();

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    /**
     * Enable all functionality of the tab
     */
    void enableTab();

    /**
     * Disable all functionality of the tab
     */
    void disableTab();

    /**
     * Retranslate all GUI strings
     */
    void retranslateStrings();

    /**************************************************************************
     * Animation notification functions
     **************************************************************************/

    /**
     * Function to receive notification when the model is erased.
     */
    void updateClear();

    /**
     * Function to receive notification when a new project is created.
     */
    void updateNewProject();

    /**
     * Function for receiving notification when the descriptions of the
     * objects are changed.
     */
    virtual void updateDescriptionsUpdated();

    /**
     * Function to recieve notification when a sound of a scene is to be played.
     * @param sceneIndex the scene to be played
     */
    void updatePlaySound(int sceneIndex);

    /**
     * Function to recieve notification when new mix mode is set.
     * @param newMixMode the new mix mode
     */
    void updateMixMode(int newMixMode);

    /**
     * Function to recieve notification when new mix count is set.
     * @param newMixCount the new mix count
     */
    void updateMixCount(int newMixCount);

    /**
     * Function to recieve notification when new frames per second is set.
     * @param newFps the new frames per second
     */
    void updateFramesPerSecond(int newFps);

    /**************************************************************************
     * Scene notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new scene is added to the
     * model.
     * @param sceneIndex the index of the new scene.
     */
    void updateAddScene(int sceneIndex);

    /**
     * Function to recieve notification when a new scene is added to the
     * model.
     * @param sceneIndex the index of the new scene.
     */
    void updateInsertScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene is selected as the
     * active scene in the animationmodel.
     */
    void updateActivateScene();

    /**
     * Function to recieve notification when a scene is removed from
     * the model.
     * @param sceneIndex the scene which has been removed from the model.
     */
    void updateRemoveScene(int sceneIndex);

    /**
     * Function which recieves notification when a scene in the animation
     * has been moved and moves the icons in the timeline accordingly.
     * @param sceneNumber the scene which have been moved.
     * @param movePosition the position the scene has been moved to.
     */
    void updateMoveScene(int sceneNumber, int movePosition);

    /**************************************************************************
     * Take notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new take is added to the
     * model.
     * @param sceneIndex the scene where the new take is added.
     * @param takeIndex the index of the new take.
     */
    void updateAddTake(int sceneIndex,
                       int takeIndex);

    /**
     * Function to recieve notification when a new take is inserted to the
     * model.
     * @param sceneIndex the scene where the new take is inserted.
     * @param takeIndex the index of the new take.
     */
    void updateInsertTake(int sceneIndex,
                          int takeIndex);

    /**
     * Function which recieves notification when a take is selected as the
     * active take in the animationmodel.
     */
    void updateActivateTake();

    /**
     * Function to recieve notification when a take is removed from
     * the model.
     * @param sceneIndex The scene of the take which has been removed from the model.
     * @param takeIndex The take which has been removed from the model.
     */
    void updateRemoveTake(int sceneIndex,
                          int takeIndex);

    /**************************************************************************
     * Exposure notification functions
     **************************************************************************/

    /**
     * Function to recieve notification when a new exposure is added to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is added.
     * @param takeIndex the take where the new exposure is added.
     * @param exposureIndex the index of the new exposure.
     */
    void updateAddExposure(int sceneIndex,
                           int takeIndex,
                           int exposureIndex);

    /**
     * Function to recieve notification when a new exposure is inserted to the
     * model.
     * @param sceneIndex the scene of the take where the new exposure is inserted.
     * @param takeIndex the take where the new exposure is inserted.
     * @param exposureIndex the index of the new exposure.
     */
    void updateInsertExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Function to recieve notification when one exposure are deleted.
     * @param sceneIndex the scene index of the selected exposure.
     * @param takeIndex the take index of the selected exposure.
     * @param exposureIndex the index of the removed exposure.
     */
    void updateRemoveExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex);

    /**
     * Function to recieve notification when one or more frames are deleted.
     * @param fromFrame the first frame in the selection
     * @param toFrame the last frame in the selection
     */
    void updateRemoveExposures(int fromFrame,
                               int toFrame);

    /**
     * Function to recieve notification when one or more frames are moved.
     */
    void updateMoveExposures(int fromFrame,
                             int toFrame,
                             int movePosition);

    /**
     * Function to recieve notification when a new frame is selected.
     */
    void updateActivateExposure();

    /**
     * Function to recieve notification when the frame of an exposure has modifyed.
     * @param modSceneIndex The index of the scene of the modifyed exposure.
     * @param modTakeIndex The index of the take of the modifyed exposure.
     * @param modExposureIndex The index of the modifyed exposure.
     */
    void updateModifyExposure(int modSceneIndex,
                              int modTakeIndex,
                              int modExposureIndex);

public slots:
    /**
     * Insert scene butten pressed.
     */
    void insertSceneSlot();

    /**
     * Add scene butten pressed.
     */
    void addSceneSlot();

    /**
     * Remove scene button pressed.
     */
    void removeSceneSlot();

    /**
     * Insert take butten pressed.
     */
    void insertTakeSlot();

    /**
     * Add take butten pressed.
     */
    void addTakeSlot();

    /**
     * Remove take button pressed.
     */
    void removeTakeSlot();

    /**
     * Insert frames button pressed.
     */
    void insertFramesSlot();

    /**
     * Add frames button pressed.
     */
    void addFramesSlot();

    /**
     * Remove frames button pressed.
     */
    void removeFramesSlot();

protected:
    // void resizeEvent(QResizeEvent *event);

private:
    /**
     * Get the selected item indexes for scene, take and exposure.
     * @param sceneIndex the index of the selected scene.
     * @param takeIndex the index of the selected take or -1 if only a scene is selected.
     * @param exposureIndex the index of the selected exposure or -1 if only a scene/take is selected.
     * @return True if some items selected, false if not.
     */
    bool getSelectedItems(int &sceneIndex,
                          int &takeIndex,
                          int &exposureIndex);

    /**
     * Remove take with all exposures from scene item.
     * @param sceneIndex The index of the scene to remove.
     */
    void removeScene(int sceneIndex);

    /**
     * Remove take with all exposures from scene item.
     * @param sceneItem The item of the scene to remove the take
     * @param takeIndex The index of the take to remove.
     */
    void removeTake(QTreeWidgetItem *sceneItem,
                    int takeIndex);

    /**
     * Unset the active exposure in the project tree.
     */
    void unsetActiveItems();

    /**
     * Set a new active exposure in the project tree.
     */
    void setActiveItems();

    /**
     * Select the graphic files to insert or append
     * @return List with selected files
     */
    QStringList selectFiles();

    /**
     * Create all GUI elements.
     */
    void makeGUI();

    /**
     * Start the gimp process.
     * @param exposureImagePath The path to the first image to edit.
     */
    void startGimpProcess(const QString &exposureImagePath);

    /**
     * Stop the gimp process.
     */
    void stopGimpProcess();

private slots:
    /**
     * Click in the project tree
     * @param exposureItem The specified item is the item that was clicked, or 0 if no item was clicked.
     * @param column The column is the item's column that was clicked.
     */
    void itemClicked(QTreeWidgetItem *exposureItem,
                     int column);

    /**
     * Double click in the project tree
     * @param exposureItem The specified item is the item that was clicked, or 0 if no item was clicked.
     * @param column The column is the item's column that was clicked.
     */
    void itemDoubleClicked(QTreeWidgetItem *exposureItem,
                           int column);

    /**
     * Double click in the project tree
     */
    void itemSelectionChanged();

    /**
     * Launch the photo editor button pressed
     */
    void editFrameSlot();

    /**
     * Brings up a dialog so that the user can choose a file to load.
     */
    void chooseFrame();

    /**
     *
     */
    void gimpProcessStarted();

    /**
     *
     */
    void gimpProcessError(QProcess::ProcessError error);

    /**
     *
     */
    void gimpProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Frontend            *frontend;
    QFileDialog         *fileDialog;
    QString              lastVisitedDir;
    bool                 tabEnabled;

    /** The active scene in the projecttree */
    int activeSceneIndex;

    /** The active take in the timeline */
    int activeTakeIndex;

    /** The active exposure in the timeline*/
    int activeExposureIndex;

    QGroupBox   *projectGroupBox;
    QTreeWidget *projectTree;

    QGroupBox   *scenesGroupBox;
    QPushButton *insertSceneButton;
    QPushButton *addSceneButton;
    QPushButton *removeSceneButton;

    QGroupBox   *takesGroupBox;
    QPushButton *insertTakeButton;
    QPushButton *addTakeButton;
    QPushButton *removeTakeButton;

    QGroupBox   *framesGroupBox;
    QPushButton *insertFramesButton;
    QPushButton *addFramesButton;
    QPushButton *removeFramesButton;

    QGroupBox   *editGroupBox;
    QPushButton *editFrameButton;

    QProcess    *gimpProcess;
};

#endif
