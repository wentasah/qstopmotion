/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include <qwt/qwt_slider.h>

#include "technical/grabber/imagegrabber.h"


/**
 * The image import tab in the preferences menu
 * @author Ralf Lange
 */
class ImportWidget : public QWidget
{
    Q_OBJECT
public:
    const     static int    IMAGEFORMATDEFAULT = ImageGrabber::jpegFormat;
    const     static int    IMAGEQUALITYDEFAULT = 100;
    const     static int    IMAGESIZEDEFAULT = ImageGrabber::defaultSize;
    constexpr static double LIVEVIEWFPSDEFAULT = 2.0f;

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

private slots:
    // image import preferences
    /**
     * Slot for notified the image import tab when the image format changes,
     * so that image import preferences can be updated.
     * @param index the new image format.
     */
    void changeImageFormat(int index);

    /**
     * Slot for notified the image import tab when the image quality changes,
     * so that image import preferences can be updated.
     * @param value The new image quality value
     */
    void changeImageQuality(double value);

    /**
     * Slot for notified the image import tab when the image size changes,
     * so that image import preferences can be updated.
     * @param index the new image size.
     */
    void changeImageSize(int index);

    /**
     * Slot for notified the image import tab when the live view fps changes,
     * so that image import preferences can be updated.
     * @param value The new fps value
     */
    void changeLiveViewFps(double value);

private:
    Frontend     *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool          tabType;

    QTextEdit    *infoText;

    QTableWidget *encoderTable;

    // Image preferences
    QGroupBox    *imagePrefs;
    QLabel       *imageFormatLabel;
    QComboBox    *imageFormatCombo;
    int           activeImageFormat;
    QLabel       *imageQualityLabel;
    QwtSlider    *imageQualitySlider;
    int           activeImageQuality;
    QLabel       *qualityMinimumLabel;
    QLabel       *qualityMaximumLabel;
    QLabel       *imageSizeLabel;
    QComboBox    *imageSizeCombo;
    int           activeImageSize;

    // Live view preferences
    QGroupBox    *liveViewPrefs;
    QLabel       *liveViewFpsLabel;
    QwtSlider    *liveViewFpsSlider;
    double        activeLiveViewFps;

    void makeGUI();

    void enableQuality();
};

#endif
