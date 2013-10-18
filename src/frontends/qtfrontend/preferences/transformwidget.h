/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

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
class TransformWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    TransformWidget(Frontend *f, bool type, QWidget *parent = 0);

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
     * Set the adjustment in the adjustment box
     * @param newAdjustment The new adjustment
     */
    void setAdjustment(int newAdjustment);

private slots:
    // Transformation preferences
    /**
     * Slot for notified the import tab when scale button is selected, so that
     * image transformation preferences can be updated.
     */
    void setScaleButtonOn();

    /**
     * Slot for notified the import tab when clip button is selected, so that
     * image transformation preferences can be updated.
     */
    void setClipButtonOn();

    /**
     * Slot for notified the import tab when zoom button is selected, so that
     * image transformation preferences can be updated.
     */
    void setZoomButtonOn();

    /**
     * Slot for notified the import tab when the zoom changes,
     * so that image transformation preferences can be updated.
     */
    void changeZoom();

private:
    Frontend     *frontend;

    /**
     * Type of the tab: true = general dialog tab, false = project dialog tab
     */
    bool          tabType;

    // Transformation preferences
    QGroupBox    *transformPrefs;
    QTextEdit    *transformText;
    QRadioButton *scaleButton;
    QRadioButton *clipButton;
    QRadioButton *zoomButton;
    int           activeTransform;

    // Adjustment preferences
    QGroupBox    *adjustmentPrefs;
    QRadioButton *leftUpButton;
    QRadioButton *centerUpButton;
    QRadioButton *rightUpButton;
    QRadioButton *leftMiddleButton;
    QRadioButton *centerMiddleButton;
    QRadioButton *rightMiddleButton;
    QRadioButton *leftDownButton;
    QRadioButton *centerDownButton;
    QRadioButton *rightDownButton;
    int           activeImageAdjustment;

    // Zoom preferences
    QGroupBox    *zoomPrefs;
    QLabel       *zoomLabel;
    QSlider      *zoomSlider;
    int           activeZoomValue;
    QLabel       *zoomMinimumLabel;
    QLabel       *zoomMaximumLabel;

    void makeGUI();

    void enableQuality();
};

#endif
