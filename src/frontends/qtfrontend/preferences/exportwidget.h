/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include "frontends/frontend.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QToolBox>
#include <QWidget>


/**
 * The export tab in the preferences menu
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ExportWidget : public QWidget
{
    Q_OBJECT
public:

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    ExportWidget(Frontend *f, bool type, QWidget *parent = 0);

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

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    // Encoder preferences
    /**
     * Slot for notified the export tab when the encoder application changes,
     * so that encoder preferences can be updated.
     * @param index the new encoder application.
     */
    void changeEncoderApplication(int index);

    /**
     * Slot for notified the export tab when the video format changes,
     * so that encoder preferences can be updated.
     * @param index the new video format.
     */
    void changeVideoFormat(int index);

    /**
     * Slot for notified the export tab when the video size changes,
     * so that encoder preferences can be updated.
     * @param index the new video size.
     */
    void changeVideoSize(int index);

    /**
     * Slot for updating the frames per second value.
     * @param value the new slider value.
     */
    void changeFps(int value);

    /**
     * Slot for updating the splitting mode value.
     * @param value the new mode.
     */
    void changeSplittingMode(int value);

    /**
     * Slot for updating the avalability of splitting mode options.
     * @param enable enable or disable radio buttons.
     */
    void enableSplittingOptions(bool enable);

    // Output file preferences
    /**
     * Update widgets in order to display that user will be asked about
     * output destination everytime.
     */
    void setAskingUserAboutOutputDestination();

    /**
     * Update widgets in order to display that default output destination
     * will be used everytime without user prompting.
     */
    void setUsingDefaultOutputDestination();

    /**
     * Slot for notified the export tab when an new default output file name
     * is selected.
     * @param txt the new default output file name.
     */
    void changeDefaultOutput(const QString &fileName);

    /**
     * Slot for notified the export tab when browse output button is selected,
     * so that encoder preferences can be updated.
     */
    void browseOutputFiles();

private:
    Frontend     *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool          tabType;

    QTextEdit    *infoText;

    QTableWidget *encoderTable;

    // Encoder preferences
    QGroupBox    *encoderPrefs;
    QLabel       *encoderApplicationLabel;
    QComboBox    *encoderApplicationCombo;
    int           activeEncoderApplication;
    QLabel       *videoFormatLabel;
    QComboBox    *videoFormatCombo;
    int           activeVideoFormat;
    QLabel       *videoSizeLabel;
    QComboBox    *videoSizeCombo;
    int           activeVideoSize;
    QLabel       *videoFpsLabel;
    QSpinBox     *videoFpsChooser;
    int           activeVideoFps;

    // Movie's splitting preferences
    QGroupBox    *splittingPrefs;
    QRadioButton *wholeMovieButton;
    QRadioButton *splittedOnScenesButton;
    QRadioButton *splittedOnTakesButton;
    int           splittingMode;

    // Output file preferences
    QGroupBox    *outputPrefs;
    QRadioButton *askUserEveryTimeButton;
    QRadioButton *useDefaultOutputButton;
    bool          activeUseDefaultOutputFile;
    QLabel       *askForOutputLabel;
    QLabel       *defaultOutputLabel;
    QLineEdit    *defaultOutputEdit;
    QString       activeDefaultOutputFileName;
    QPushButton  *browseOutputButton;

    void makeGUI();
};

#endif
