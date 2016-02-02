/******************************************************************************
 *  Copyright (C) 2014-2015 by                                                *
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

#ifndef GRABBERWIDGET_H
#define GRABBERWIDGET_H

#include "frontends/frontend.h"

#include <QTranslator>
#include <QCheckBox>
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
class GrabberWidget : public QWidget
{
    Q_OBJECT
public:

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    GrabberWidget(Frontend *f, QWidget *parent = 0);

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

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    /**
     * Slot for notified the grabber tab when the state of the
     * v4l2Grabber checkbox are changed,
     * so that general preferences can be updated.
     * @param newState The new state of the check box
     */
    void changeV4L2GrabberCheckState(int newState);

    /**
     * Slot for notified the grabber tab when the state of the
     * mediaFoundationGrabber checkbox are changed,
     * so that general preferences can be updated.
     * @param newState The new state of the check box
     */
    void changeMediaFoundationGrabberCheckState(int newState);

    /**
     * Slot for notified the grabber tab when the state of the
     * gphoto2Grabber checkbox are changed,
     * so that general preferences can be updated.
     * @param newState The new state of the check box
     */
    void changeGphoto2GrabberCheckState(int newState);

private:
    Frontend     *frontend;

    QTextEdit    *infoText;

    QGroupBox    *grabberGroupBox;
    QCheckBox    *gstreamerVideoTestGrabberCheck;
    QCheckBox    *gstreamerV4L2GrabberCheck;
    QCheckBox    *gstreamerDv1394GrabberCheck;
    QCheckBox    *v4l2GrabberCheck;
    QCheckBox    *v4l2ControllerCheck;
    QCheckBox    *mediaFoundationGrabberCheck;
    QCheckBox    *mediaFoundationControllerCheck;
    QCheckBox    *gphoto2GrabberCheck;
    QCheckBox    *gphoto2ControllerCheck;

    bool          actualGstreamerVideoTestGrabber;
    bool          actualGstreamerV4L2Grabber;
    bool          actualGstreamerDv1394Grabber;
    bool          actualV4L2Grabber;
    bool          actualV4L2Controller;
    bool          actualMediaFoundationGrabber;
    bool          actualMediaFoundationController;
    bool          actualGphoto2Grabber;
    bool          actualGphoto2Controller;
};

#endif
