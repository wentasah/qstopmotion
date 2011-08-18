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

#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include "frontends/qtfrontend/frameview/frameviewinterface.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/QPushButton>


/**
 *
 *@author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class CameraHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor for setting up the camerahandler.
     * @param f the frontend of the application
     * @param parent the parent widget
     * @param handler for adding frames when capturing.
     * @param name the name of the object
     */
    CameraHandler(Frontend *f,
                  QObject *parent = 0,
                  const QString &name = QString());

    ~CameraHandler();

    /**
     *
     */
    void setCameraButton(QPushButton *cameraButton);

    /**
     *
     */
    void setFrameView(FrameViewInterface *frameView);

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    void changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction);

    /**
     * Checks if the camera is on.
     * @return true if the camera is on.
     */
    bool isCameraRunning();

public slots:
    /**
     *
     */
    void toggleCamera();

    /**
     *
     */
    void captureFrame();

    /**
     * Slot to recieve a message when the videoView are finished seting up the
     * camera.
     */
    void switchToVideoView();

private:
    Frontend           *frontend;
    QPushButton        *cameraButton;
    QTimer             *timer;
    QString             temp;
    bool                isCameraOn;
    FrameViewInterface *frameView;
    PreferencesTool::captureButtonFunction captureFunction;

    void cameraOn();
    void cameraOff();

private slots:
    /**
     * Slot called by the timer which checks if the frame has been fully captured.
     */
    void storeFrame();

signals:
    /**
     *
     */
    void capturedFrame();

    /**
     * This signal is emitted when the state of the camera is changed.
     * @param isActivated true if the camera has been activated.
     */
    void cameraStateChanged(bool isActivated);

};

#endif
