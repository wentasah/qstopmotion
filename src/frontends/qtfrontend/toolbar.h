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
#include <QTimer>

#include "frontends/frontend.h"

/**
 * The tool bar on the button of the preview window
 * @author Ralf Lange
 */
class ToolBar : public QWidget
{
    Q_OBJECT
public:
    enum toolBarFunction {
        toolBarNothing,
        toolBarCameraOff,
        toolBarCameraOn
    };

    /**
     * Sets up the tab.
     * @param parent the parent of the this widget
     */
    ToolBar(Frontend     *f,
            QWidget      *parent = 0);

    /**
     * Get the capture button of the tool bar
     * @return The capture button
     */
    QPushButton* getCaptureButton();

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    void retranslateStrings();

    /**
     * Set an new state to the tool bar.
     * @arg newState The new tool bar state.
     */
    void setActualState(toolBarFunction newState);

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:
    /**
     * Starts the animation if it isn't playing and stops it if it is.
     *
     * This function is provided for ease of use with keyaccelerators.
     */
    void toggleRunning();

    /**
     * Runs the animation as a sequence of pictures triggered by a timer
     */
    void runAnimation();

    /**
     * Stops the running of the animation.
     */
    void stopAnimation();

    /**
     * Freezes the running of the animation.
     */
    void pauseAnimation();

    /**
     * Selects the previous scene.
     */
    void selectPreviousScene();

    /**
     * Selects the previous take.
     */
    void selectPreviousTake();

    /**
     * Selects the first frame in the take.
     */
    void selectToBeginFrame();

    /**
     * Selects the previous frame.
     */
    void selectPreviousFrame();

    /**
     * Selects the next frame.
     */
    void selectNextFrame();

    /**
     * Selects the last frame in the take.
     */
    void selectToEndFrame();

    /**
     * Selects the next scene.
     */
    void selectNextTake();

    /**
     * Selects the next scene.
     */
    void selectNextScene();

    /**
     * Toggles between looping the animation when it is running and closing it
     * when it reaches the end.
     */
    void toggleLooping();

    /**
     * This slot is notified when the size of the model changes so that menuframe
     * menu options can be adjusted (activated/deactivated, etc).
     */
    void modelSizeChanged();

private:
    Frontend        *frontend;
    QTimer          *runAnimationTimer;
    toolBarFunction  actualState;

    QLabel          *framesIcon;
    QSlider         *overlaySlider;
    QLabel          *cameraIcon;
    QPushButton     *toBeginButton;
    QPushButton     *previousFrameButton;
    QPushButton     *captureButton;
    QPushButton     *playButton;
    QPushButton     *nextFrameButton;
    QPushButton     *toEndButton;

    int              fps;
    int              exposureCount;
    bool             isLooping;
    int              exposureIndex;

    void makeGUI();

private slots:
    /*  void toBegin();
        void previousFrame();
        void play();
        void capture();
        void nextFrame();
        void toEnd();
    */

    /**
     * Slot for playing the next frame. This slot is triggered by the timer.
     */
    void playNextFrame();
};

#endif
