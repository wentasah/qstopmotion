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

#ifndef CONTROLERWIDGET_H
#define CONTROLERWIDGET_H

#include "frontends/frontend.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QWidget>


/**
 * The image import tab in the preferences menu
 * @author Ralf Lange
 */
class ControlerWidget : public QWidget
{
    Q_OBJECT
public:
    const static bool BRIGHTNESSDEFAULT = true;
    const static bool CONTRASTDEFAULT   = true;
    const static bool SATURATIONDEFAULT = true;
    const static bool HUEDEFAULT        = true;
    const static bool GAMMADEFAULT      = true;
    const static bool SHARPNESSDEFAULT  = true;
    const static bool BACKLIGHTDEFAULT  = true;
    const static bool WHITEDEFAULT      = true;
    const static bool GAINDEFAULT       = true;
    const static bool COLORDEFAULT      = true;

    const static bool EXPOSUREDEFAULT   = true;
    const static bool ZOOMDEFAULT       = true;
    const static bool FOCUSDEFAULT      = true;
    const static bool PANDEFAULT        = true;
    const static bool TILTDEFAULT       = true;
    const static bool IRISDEFAULT       = true;
    const static bool ROLLDEFAULT       = true;

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    ControlerWidget(Frontend *f, QWidget *parent = 0);

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

private:
    Frontend     *frontend;

    QTextEdit    *infoText;

    // Image quality preferences
    QGroupBox    *qualityPrefs;
    QCheckBox    *brightnessCheckBox;
    QCheckBox    *contrastCheckBox;
    QCheckBox    *saturationCheckBox;
    QCheckBox    *hueCheckBox;
    QCheckBox    *gammaCheckBox;
    QCheckBox    *sharpnessCheckBox;
    QCheckBox    *backlightCheckBox;
    QCheckBox    *whiteCheckBox;
    QCheckBox    *gainCheckBox;
    QCheckBox    *colorCheckBox;

    // Live view preferences
    QGroupBox    *controlPrefs;
    QCheckBox    *exposureCheckBox;
    QCheckBox    *zoomCheckBox;
    QCheckBox    *focusCheckBox;
    QCheckBox    *panCheckBox;
    QCheckBox    *tiltCheckBox;
    QCheckBox    *irisCheckBox;
    QCheckBox    *rollCheckBox;

    // Actual values
    bool actualBrightness;
    bool actualContrast;
    bool actualSaturation;
    bool actualHue;
    bool actualGamma;
    bool actualSharpness;
    bool actualBacklight;
    bool actualWhite;
    bool actualGain;
    bool actualColor;

    bool actualExposure;
    bool actualZoom;
    bool actualFocus;
    bool actualPan;
    bool actualTilt;
    bool actualIris;
    bool actualRoll;

    void makeGUI();

    void resetDialog();
};

#endif
