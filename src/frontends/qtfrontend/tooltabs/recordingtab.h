/******************************************************************************
 *  Copyright (C) 2010-2018 by                                                *
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

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QWidget>
#include <QSoundEffect>

#include <qwt_slider.h>

#include "frontends/frontend.h"
#include "frontends/qtfrontend/toolbar.h"


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
     * Destructor
     */
    ~RecordingTab();

    /**
     * Retranslate all strings of the tab after changing the language setting
     */
    void retranslateStrings();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

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
     * Get the video source index.
     * @return video source index
     */
    int getVideoSource();

    /**
     * Set the video source index.
     * @param index Index of the new video source
     * @return true if the new index is set, false else.
     */
    bool setVideoSource(int index);

    /**
     * Get the video resolution index.
     * @return video resolution index
     */
    int getResolution();

    /**
     * Set the video resolution index.
     * @param index Index of the new video resolution
     * @return true if the new index is set, false else.
     */
    bool setResolution(int index);

    /**
     * Get the recording mode.
     * @return The recording mode.
     */
    int getRecordingMode();

    /**
     * Set the recording mode.
     * @param mode The new recording mode.
     */
    void setRecordingMode(int mode);

    /**
     * Get the mix mode.
     * @return The mix mode.
     */
    int getMixMode();

    /**
     * Set the mix mode.
     * @param mode The new mix mode
     */
    void setMixMode(int mode);

    /**
     * Get the mix count.
     * @return The mix count.
     */
    int getMixCount();

    /**
     * Set the mix count.
     * @param count The new mix count.
     */
    void setMixCount(int count);

    /**
     * Get the unit mode.
     * @return The unit mode.
     */
    int getUnitMode();

    /**
     * Set the unit mode.
     * @param mode The new unit mode
     */
    void setUnitMode(int mode);

    /**
     * Get the unit count.
     * @return The unit count.
     */
    int getUnitCount();

    /**
     * Set the unit count.
     * @param count The new unit count.
     */
    void setUnitCount(int count);

    /**
     * Get the beep check box state.
     * @return The beep check box state.
     */
    bool getBeepState();

    /**
     * Set the beep check box state.
     * @param count The new beep check box state.
     */
    void setBeepState(bool checked);

    /**
     * Get the beep unit count.
     * @return The beep unit count.
     */
    int getBeepCount();

    /**
     * Set the beep unit count.
     * @param count The new beep unit count.
     */
    void setBeepCount(int count);

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

    /**
     * Clip and scale the raw image to the project image size
     * @param image The image to clip and scale
     * @return The cliped and scaled image
     */
    QImage clipAndScale(QImage image);

    /**
     * Change the enabled state of the elements of the capture group box.
     * @brief enableCaptureGroupBox
     * @param newState The new state of the capture group box.
     */
    void enableCaptureGroupBox(bool newState);

    /**
     * Change the enabled state of the elements of the timelapse group box.
     * @brief enableTimelapseGroupBox
     * @param newState The new state of the timelapse group box.
     */
    void enableTimelapseGroupBox(bool newState);

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:
    /**
     *
     */
    void captureFrame();

    /**
     * Slot for notified the recording tab when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeVideoSource(int index);

    /**
     * Slot for notified the camera controller when the resolution changes,
     * so that resolution can be updated.
     * @param index the new resolution value.
     */
    void changeResolution(int index);

    /**
     * Slot for notified the recording tab when the recording mode changes, so that widgets
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeRecordingMode(int index);

    /**
     * Slot for notified the recording tab when the mix mode changes, so that widgets
     * can be updated.
     * @param index the new mix mode.
     */
    void changeMixMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     */
    void changeMixCount();

    /**
     * Slot for setting the mix mode to Mixing.
     * Used by the shortcut key.
     */
    void setMixModeMixing();

    /**
     * Slot for setting the mix mode to Diffing.
     * Used by the shortcut key.
     */
    void setMixModeDiffing();

    /**
     * Slot for notified the recording tab when the unit mode changes.
     * @param index the new unit mode.
     */
    void changeUnitMode(int index);

    /**
     * Slot for updating the unit slider value when running in automatic mode.
     */
    void changeUnitCount();

    /**
     * Slot for changing the beep status.
     * @param newState the new check box state.
     */
    void changeBeep(int newState);

    /**
     * Slot for updating the beep slider value when running in automatic mode.
     */
    void changeBeepCount();

    /**
     * Slot for notified the recording tab when the camera button is pressed.
     */
    void cameraButtonClicked();

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param newFpuCount the new slider value.
     */
    // void changeFpuCount(int newFpuCount);

private:
    Frontend    *frontend;
    ToolBar     *toolBar;
    bool         cameraOn;
    int          captureFunction;

    QShortcut   *mixAccel;
    QShortcut   *diffAccel;
    QShortcut   *playbackAccel;

    QGroupBox   *cameraGroupBox;
    QLabel      *videoSourceLabel;
    QComboBox   *videoSourceCombo;
    QLabel      *resolutionLabel;
    QComboBox   *resolutionCombo;

    QGroupBox   *recordingGroupBox;
    QComboBox   *recordingModeCombo;

    QGroupBox   *captureGroupBox;
    QComboBox   *mixModeCombo;
    QLabel      *mixCountSliderCaption;
    QwtSlider   *mixCountSlider;

    QGroupBox   *timelapseGroupBox;
    QLabel      *unitModeComboCaption;
    QComboBox   *unitModeCombo;
    QLabel      *unitCountSliderValue;
    QLabel      *unitCountSliderCaption;
    QwtSlider   *unitCountSlider;
    QCheckBox   *beepCheckBox;
    QLabel      *beepCountSliderValue;
    QLabel      *beepCountSliderCaption;
    QwtSlider   *beepCountSlider;
    QTimer      *timelapseTimer;
    QTimer      *cameraTimer;
    QSoundEffect *beepEffect;
    QSoundEffect *clickEffect;

    QPushButton *cameraButton;

    // QLabel      *fpuSliderCaption;    // Frames per unit
    // QwtSlider   *fpuSlider;

private slots:

    /**
     * Slot called by the timelapseTimer which send a beep signal to the speaker.
     */
    void sendBeep();

    /**
     * Slot called by the cameraTimer which capture the frame  some seconds after the beep signal.
     */
    void storeFrame();

};

#endif
