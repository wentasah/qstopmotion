/******************************************************************************
 *  Copyright (C) 2010-2017 by                                                *
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

#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>

#include <qwt/qwt_slider.h>

#include "frontends/frontend.h"


/**
 * The project widget in the preferences menu
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad & Ralf Lange
 */
class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    ProjectWidget(Frontend *f, bool type, QWidget *parent = 0);

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Reset the settings in the import tab.
     */
    void reset();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

    /**
     * Set the image grabber source in the combo box.
     * @param newSource The new image grabber source
     */
    void setImageGrabberSource(int newSource);

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    /**
     * Slot for notified the project widget when the recording mode changes, so that widgets
     * can be updated.
     * @param index the new recording mode.
     */
    void changeRecordingMode(int index);

    /**
     * Slot for notified the project widget when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeGrabberSource(int index);

    /**
     * Slot for notified the project widget when the mix mode changes, so that widgets
     * can be updated.
     * @param index the new mix mode.
     */
    void changeMixMode(int index);

    /**
     * Slot for updating the mix count slider value.
     */
    void changeMixCount();

    /**
     * Slot for notified the project widget when the unit mode changes.
     * @param index the new unit mode.
     */
    void changeUnitMode(int index);

    /**
     * Slot for notified the project widget when the unit slider changes.
     */
    void changeUnitCount();

    /**
     * Slot for notified the project widget when the beep status changes.
     * @param newState the new check box state.
     */
    void changeBeep(int newState);

    /**
     * Slot for notified the project widget when the beep slider changes.
     */
    void changeBeepCount();

private:
    Frontend    *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool         tabType;

    QGroupBox   *grabberGroupBox;
    QComboBox   *grabberSourceCombo;

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

    int          actualRecordingMode;
    int          actualGrabberSource;
    int          actualMixMode;
    int          actualMixCount;
    int          actualUnitMode;
    int          actualUnitCount;
    bool         actualBeepCheck;
    int          actualBeepCount;
};

#endif
