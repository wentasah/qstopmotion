/******************************************************************************
 *  Copyright (C) 2005-2014 by                                                *
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

#ifndef IMPORTWIDGET_H
#define IMPORTWIDGET_H

#include "frontends/frontend.h"

#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>


/**
 * The image import tab in the preferences menu
 * @author Ralf Lange
 */
class ImportWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    ImportWidget(Frontend *f, bool type, QWidget *parent = 0);

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

private:
    /**
     * Set the image grabber source in the combo box.
     * @param newSource The new image grabber source
     */
    void setImageGrabberSource(int newSource);

private slots:
    // image import preferences
    /**
     * Slot for notified the image import tab when the image grabber sources changes,
     * so that image import preferences can be updated.
     * @param index the new image grabber sources.
     */
    void changeImageGrabberSources(int index);

    /**
     * Slot for notified the image import tab when the image format changes,
     * so that image import preferences can be updated.
     * @param index the new image format.
     */
    void changeImageFormat(int index);

    /**
     * Slot for notified the image import tab when the image quality changes,
     * so that image import preferences can be updated.
     */
    void changeImageQuality();

    /**
     * Slot for notified the image import tab when the image size changes,
     * so that image import preferences can be updated.
     * @param index the new image size.
     */
    void changeImageSize(int index);

    /**
     * Slot for notified the image import tab when the live view fps changes,
     * so that image import preferences can be updated.
     */
    void changeLiveViewFps();

private:
    Frontend     *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool          tabType;

    QTextEdit    *infoText;

    QTableWidget *encoderTable;

    // Grabber preferences
    QGroupBox    *grabberPrefs;
    QLabel       *grabberSourceLabel;
    QComboBox    *grabberSourceCombo;
    int           activeGrabberSource;

    // Image preferences
    QGroupBox    *imagePrefs;
    QLabel       *imageFormatLabel;
    QComboBox    *imageFormatCombo;
    int           activeImageFormat;
    QLabel       *imageQualityLabel;
    QSlider      *imageQualitySlider;
    int           activeImageQuality;
    QLabel       *qualityMinimumLabel;
    QLabel       *qualityMaximumLabel;
    QLabel       *imageSizeLabel;
    QComboBox    *imageSizeCombo;
    int           activeImageSize;

    // Live view preferences
    QGroupBox    *liveViewPrefs;
    QLabel       *liveViewFpsLabel;
    QSlider      *liveViewFpsSlider;
    int           activeLiveViewFps;
    QLabel       *fpsMinimumLabel;
    QLabel       *fpsMaximumLabel;

    void makeGUI();

    void enableQuality();
};

#endif
