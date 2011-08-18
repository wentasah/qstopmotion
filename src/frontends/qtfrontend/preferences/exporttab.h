/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#ifndef EXPORTTAB_H
#define EXPORTTAB_H

#include "frontends/frontend.h"

#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>


/**
 * The export tab in the preferences menu
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ExportTab : public QWidget
{
    Q_OBJECT
public:
    enum encoderApplication {
        ffmpegApplication,
        mencoderApplication,
        noneApplication
    };

    enum videoFormat {
        mpeg1Format,
        mpeg4Format,
        noneFormat
    };

    enum videoSize {
        qvgaSize,          // 320x240
        vgaSize,           // 640x480
        nonSize
    };

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    ExportTab(Frontend *f, QWidget *parent = 0);

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

    void retranslateStrings();

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

    // Output file preferences
    /**
     * Slot for notified the export tab when yes button is selected, so that
     * encoder preferences can be updaten.
     */
    void setYesButtonOn();

    /**
     * Slot for notified the export tab when no button is selected, so that
     * encoder preferences can be updaten.
     */
    void setNoButtonOn();

    /**
     * Slot for notified the export tab when an new default output file name
     * is selected.
     * @param txt the new default output file name.
     */
    void changeDefaultOutput(const QString &fileName);

    /**
     * Slot for notified the export tab when browse output button is selected,
     * so that encoder preferences can be updaten.
     */
    void browseOutputFiles();

private:
    Frontend     *frontend;

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
    QLineEdit    *videoFpsEdit;

    // Output file preferences
    QGroupBox    *outputPrefs;
    QRadioButton *yesButton;
    QRadioButton *noButton;
    int           useDefaultOutputFile;
    QLabel       *askForOutputLabel;
    QLabel       *defaultOutputLabel;
    QLineEdit    *defaultOutputEdit;
    QString       defaultOutputfileName;
    QPushButton  *browseOutputButton;

    void makeGUI();
};

#endif
