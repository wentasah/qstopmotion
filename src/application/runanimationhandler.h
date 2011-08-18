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

#ifndef RUNANIMATIONHANDLER_H
#define RUNANIMATIONHANDLER_H

#include "frontends/frontend.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/QPushButton>


/**
 * Handles the running of the animation as a sequence of pictures. This is implemented
 * as a timer trigger, to to make it easyer to time the fps and, more importantly, to
 * avoid threads.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class RunAnimationHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates the RunAnimationHandler and initializes the member fields.
     * @param f the GUI frontend
     * @param parent the parent of this QOject.
     * @param name the name of this QObject
     */
    RunAnimationHandler(Frontend *f,
                        QObject *parent = 0,
                        const char *name = 0);


    /**
     * Destructor.
     */
    ~RunAnimationHandler();

    /**
     * Stores the playButton so that it can be toggled
     * @param playButton the button which starts and stops the
     * animation running. This is needed for toggling the button states.
     */
    void setPlayButton(QPushButton *playButton);

    /**
     * Sets the loop button.
     * @param loopButton the button to be used as loop button
     */
    void setLoopButton(QPushButton *loopButton);

    /**
     * Sets the pause button.
     * @param pauseButton the button to be used as pause button
     */
    void setPauseButton(QPushButton *pauseButton);

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

private:
    Frontend         *frontend;
    QPushButton      *playButton;
    QPushButton      *loopButton;
    QPushButton      *pauseButton;
    QTimer           *timer;
    int               frameNr;
    int               fps;
    int               exposureCount;
    bool              isLooping;

private slots:
    /**
     * Slot for playing the next frame. This slot is triggered by the timer.
     */
    void playNextFrame();
};

#endif
