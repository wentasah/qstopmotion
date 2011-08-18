/***************************************************************************
 *   Copyright (C) 2010-2011 by Ralf Lange                                 *
 *   ralf.lange@longsoft.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
// #include <QGroupBox>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>

#include "application/runanimationhandler.h"
#include "application/camerahandler.h"
#include "frontends/frontend.h"

/**
 * The tool bar on the button of the preview window
 * @author Ralf Lange
 */
class ToolBar : public QWidget
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param parent the parent of the this widget
     */
    ToolBar(Frontend *f,
            RunAnimationHandler *runAnimationHandler,
            CameraHandler *cameraHandler,
            QWidget *parent = 0);

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    void retranslateStrings();

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:

    /**
     * This slot is notified when the size of the model changes so that menuframe
     * menu options can be adjusted (activated/deactivated, etc).
     * @param modelSize the new size of the model.
     */
    void modelSizeChanged(int modelSize);

private slots:
    /*  void toBegin();
        void previousFrame();
        void play();
        void capture();
        void nextFrame();
        void toEnd();
    */
private:
    Frontend            *frontend;
    RunAnimationHandler *runAnimationHandler;
    CameraHandler       *cameraHandler;

    QLabel      *framesIcon;
    QSlider     *overlaySlider;
    QLabel      *cameraIcon;
    QPushButton *toBeginButton;
    QPushButton *previousFrameButton;
    QPushButton *playButton;
    QPushButton *captureButton;
    QPushButton *nextFrameButton;
    QPushButton *toEndButton;

    void makeGUI();
};

#endif
