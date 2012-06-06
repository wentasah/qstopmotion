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

#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include "frontends/frontend.h"

#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>


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

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    /**
     * Slot for notified the default tab when the recording mode changes, so that widgets
     * can be updated.
     * @param index the new recording mode.
     */
    void changeRecordingMode(int index);

    /**
     * Slot for notified the default tab when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeVideoSource(int index);

    /**
     * Slot for notified the default tab when the mix mode changes, so that widgets
     * can be updated.
     * @param index the new mix mode.
     */
    void changeMixMode(int index);

    /**
     * Slot for updating the mix count slider value.
     * @param value the new slider value.
     */
    void changeMixCount(int value);

    /**
     * Slot for notified the default tab when the unit mode changes.
     * @param index the new unit mode.
     */
    // void changeUnitMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param value the new slider value.
     */
    // void changeFpuCount(int value);

private:
    Frontend    *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool         tabType;

    QGroupBox   *recordingGroupBox;
    QComboBox   *recordingModeCombo;

    QGroupBox   *cameraGroupBox;
    QComboBox   *videoSourceCombo;

    QGroupBox   *captureGroupBox;
    QComboBox   *mixModeCombo;
    QLabel      *mixCountSliderCaption;
    QSlider     *mixCountSlider;
    QLabel      *fpsChooserCaption;
    QSpinBox    *fpsChooser;

    // QGroupBox   *autoGroupBox;
    // QComboBox   *unitModeCombo;

    int          defaultRecordingMode;
    int          defaultVideoSource;
    int          defaultMixMode;
    int          defaultMixCount;
    int          defaultPlaybackCount;
    int          defaultFps;
    // int          defaultUnitMode;

};

#endif
