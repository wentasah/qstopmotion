/******************************************************************************
 *  Copyright (C) 2010-2012 by                                                *
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

#ifndef RECORDINGTAB_H
#define RECORDINGTAB_H

#include "frontends/frontend.h"
#include "frontends/qtfrontend/toolbar.h"

#include <QtCore/QTimer>
#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QLabel>
#include <QtGui/QShortcut>


/**
 * The recording tab on the right hand side of the preview window
 * @author Ralf Lange
 */
class RecordingTab : public QWidget
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param df the domain facade of the application
     * @param tb the tool bar
     * @param parent the parent of the this widget
     */
    RecordingTab(Frontend *f,
                 ToolBar  *tb,
                 QWidget  *parent = 0);

    /**
     * Is the camera switched on.
     * @return true if the camera is on.
     */
    bool isCameraOn();

    /**
     * Check that the camera is switched off.
     * If not the camera is switched off.
     */
    void checkCameraOff();

    /**
     * Applies the settings in the import tab.
     */
    // void apply();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    void retranslateStrings();

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    void changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction);

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

    /**
     * Creates key accelerators (keyboard shortcuts)
     * More can be found in the function MainWindowGUI::createAccelerators().
     */
    void createAccelerators();

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:
    /**
     *
     */
    void captureFrame();

    /**
     * Slot for notified the recording tab when the recording mode changes, so that widgets
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeRecordingMode(int index);

    /**
     * Slot for notified the recording tab when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeVideoSource(int index);

    /**
     * Slot for notified the recording tab when the camera button is pressed.
     */
    void cameraButtonClicked();

    /**
     * Slot for notified the recording tab when the viewing mode changes, so that widgets
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeMixingMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param value the new slider value.
     */
    void changeMixCount(int value);

    /**
     * Slot for setting the camera mode to mixing.
     * Used by the shortcut key.
     */
    void setMixingMode();

    /**
     * Slot for setting the camera mode to Diffing.
     * Used by the shortcut key.
     */
    void setDiffingMode();

    /**
     * Slot for setting the camera mode to Playback.
     * Used by the shortcut key.
     */
    void setPlaybackMode();

    /**
     * Slot for notified the recording tab when the unit mode changes.
     * @param index the new unit mode.
     */
    // void changeUnitMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param newFpuCount the new slider value.
     */
    // void changeFpuCount(int newFpuCount);

private:
    Frontend            *frontend;
    ToolBar             *toolBar;
    bool                 cameraOn;
    QTimer             *cameraTimer;
    QString             captureFilePath;
    PreferencesTool::captureButtonFunction captureFunction;

    QShortcut           *mixAccel;
    QShortcut           *diffAccel;
    QShortcut           *playbackAccel;

    QGroupBox   *recordingGroupBox;
    QComboBox   *recordingModeCombo;

    QGroupBox   *cameraGroupBox;
    QComboBox   *videoSourceCombo;
    QPushButton *cameraButton;

    QGroupBox   *captureGroupBox;
    QComboBox   *mixingModeCombo;
    QLabel      *mixCountSliderCaption;
    QSlider     *mixCountSlider;

    // QGroupBox   *autoGroupBox;
    // QComboBox   *unitChooseCombo;
    // QLabel      *fpuSliderCaption;    // Frames per unit
    // QSlider     *fpuSlider;
    // QTimer      *autoCaptureTimer;

private slots:
    /**
     * Slot called by the cameraTimer which checks if the frame has been fully captured.
     */
    void storeFrame();

};

#endif
