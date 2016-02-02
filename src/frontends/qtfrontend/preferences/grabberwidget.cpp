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

#include "grabberwidget.h"

#include <QColorDialog>
#include <QDebug>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"


GrabberWidget::GrabberWidget(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "GrabberWidget::Constructor --> Start";

    frontend                               = f;

    infoText                               = 0;

    // Image grabber preferences
    grabberGroupBox                        = 0;
    gstreamerVideoTestGrabberCheck         = 0;
    actualGstreamerVideoTestGrabber        = true;
    gstreamerV4L2GrabberCheck              = 0;
    actualGstreamerV4L2Grabber             = false;
    gstreamerDv1394GrabberCheck            = 0;
    actualGstreamerDv1394Grabber           = false;
    v4l2GrabberCheck                       = 0;
    actualV4L2Grabber                      = false;
    v4l2ControllerCheck                    = 0;
    actualV4L2Controller                   = false;
    mediaFoundationGrabberCheck            = 0;
    actualMediaFoundationGrabber           = false;
    mediaFoundationControllerCheck         = 0;
    actualMediaFoundationController        = false;
    gphoto2GrabberCheck                    = 0;
    actualGphoto2Grabber                   = false;
    gphoto2ControllerCheck                 = 0;
    actualGphoto2Controller                = false;

    this->setObjectName("GrabberWidget");

    makeGUI();

    qDebug() << "GrabberWidget::Constructor --> End";
}


void GrabberWidget::makeGUI()
{
    qDebug() << "GrabberWidget::makeGUI --> Start";

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p>" +
        tr("Below you can select which image grabber should be used "
           "for grabbing images from the camera. If available the controller "
           "can be used to control focus, zoom and other functionality.") +
        "</p>");

    infoText->setMinimumWidth(440);
    infoText->setMaximumHeight(100);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Image grabber preferences
    grabberGroupBox = new QGroupBox;
    grabberGroupBox->setTitle(tr("Grabber Functionality"));

    gstreamerVideoTestGrabberCheck = new QCheckBox(tr("GStreamer Video Test Source"));
    gstreamerVideoTestGrabberCheck->setChecked(false);

    gstreamerV4L2GrabberCheck = new QCheckBox(tr("GStreamer Video 4 Linux 2 Source"));
    gstreamerV4L2GrabberCheck->setChecked(false);

    gstreamerDv1394GrabberCheck = new QCheckBox(tr("GStreamer DV1394 Source"));
    gstreamerDv1394GrabberCheck->setChecked(false);

    v4l2GrabberCheck = new QCheckBox(tr("Video 4 Linux 2 Source"));
    v4l2GrabberCheck->setChecked(false);
    connect(v4l2GrabberCheck, SIGNAL(stateChanged(int)), this, SLOT(changeV4L2GrabberCheckState(int)));

    v4l2ControllerCheck = new QCheckBox(tr("Camera Controller (Experimental)"));
    v4l2ControllerCheck->setChecked(false);

    mediaFoundationGrabberCheck = new QCheckBox(tr("Microsoft Media Foundation Source"));
    mediaFoundationGrabberCheck->setChecked(false);
    connect(mediaFoundationGrabberCheck, SIGNAL(stateChanged(int)), this, SLOT(changeMediaFoundationGrabberCheckState(int)));

    mediaFoundationControllerCheck = new QCheckBox(tr("Camera Controller (Experimental)"));
    mediaFoundationControllerCheck->setChecked(false);

    gphoto2GrabberCheck = new QCheckBox(tr("Gphoto2 Source (Experimental)"));
    gphoto2GrabberCheck->setChecked(false);
    connect(gphoto2GrabberCheck, SIGNAL(stateChanged(int)), this, SLOT(changeGphoto2GrabberCheckState(int)));

    gphoto2ControllerCheck = new QCheckBox(tr("Camera Controller (Experimental)"));
    gphoto2ControllerCheck->setChecked(false);

    QGridLayout *grabberLayout = new QGridLayout;
    grabberLayout->setColumnMinimumWidth(0, 20);
    grabberLayout->setColumnStretch(1, 1);
    grabberLayout->addWidget(gstreamerVideoTestGrabberCheck, 0, 0, 1, 2);
    grabberLayout->addWidget(gstreamerV4L2GrabberCheck, 1, 0, 1, 2);
    grabberLayout->addWidget(gstreamerDv1394GrabberCheck, 2, 0, 1, 2);
    grabberLayout->addWidget(v4l2GrabberCheck, 3, 0, 1, 2);
    grabberLayout->addWidget(v4l2ControllerCheck, 4, 1, 1, 1);
    grabberLayout->addWidget(mediaFoundationGrabberCheck, 5, 0, 1, 2);
    grabberLayout->addWidget(mediaFoundationControllerCheck, 6, 1, 1, 1);
    grabberLayout->addWidget(gphoto2GrabberCheck, 7, 0, 1, 2);
    grabberLayout->addWidget(gphoto2ControllerCheck, 8, 1, 1, 1);
    grabberGroupBox->setLayout(grabberLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(grabberGroupBox);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "GrabberWidget::makeGUI --> End";
}


void GrabberWidget::initialize()
{
    qDebug() << "GrabberWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

#ifdef Q_OS_LINUX
    // GStreamer video test device
    if (frontend->isGstreamerInstalled()) {
        if (pref->getIntegerPreference("preferences", "gstreamervideotestgrabber", value) == false) {
            value = false;
        }
        actualGstreamerVideoTestGrabber = value;
        gstreamerVideoTestGrabberCheck->setChecked(actualGstreamerVideoTestGrabber);
    }
    else {
        gstreamerVideoTestGrabberCheck->hide();
    }
#else
    gstreamerVideoTestGrabberCheck->hide();
#endif

#ifdef Q_OS_LINUX
    // GStreamer video4linux2 device
    if (frontend->isGstreamerInstalled()) {
        if (pref->getIntegerPreference("preferences", "gstreamerv4l2grabber", value) == false) {
            value = true;
        }
        actualGstreamerV4L2Grabber = value;
        gstreamerV4L2GrabberCheck->setChecked(actualGstreamerV4L2Grabber);
    }
    else {
        gstreamerV4L2GrabberCheck->hide();
    }
#else
    gstreamerV4L2GrabberCheck->hide();
#endif

#ifdef Q_OS_LINUX
    // GStreamer dv1394 device
    if (frontend->isGstreamerInstalled()) {
        if (pref->getIntegerPreference("preferences", "gstreamerdv1394grabber", value) == false) {
            value = false;
        }
        actualGstreamerDv1394Grabber = value;
        gstreamerDv1394GrabberCheck->setChecked(actualGstreamerDv1394Grabber);
    }
    else {
        gstreamerDv1394GrabberCheck->hide();
    }
#else
    gstreamerDv1394GrabberCheck->hide();
#endif

#ifdef Q_OS_LINUX
    // Video4Linux2 device
    if (pref->getIntegerPreference("preferences", "v4l2grabber", value) == false) {
        value = true;
    }
    actualV4L2Grabber = value;
    v4l2GrabberCheck->setChecked(actualV4L2Grabber);
    changeV4L2GrabberCheckState(actualV4L2Grabber);

    if (pref->getIntegerPreference("preferences", "v4l2controller", value) == false) {
        value = false;
    }
    actualV4L2Controller = value;
    v4l2ControllerCheck->setChecked(actualV4L2Controller);

    // Temporary not enabled
    // v4l2GrabberCheck->setEnabled(false);
    // v4l2ControllerCheck->setEnabled(false);
#else
    v4l2GrabberCheck->hide();
    v4l2ControllerCheck->hide();
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Media Foundation device
    if (pref->getIntegerPreference("preferences", "mediafoundationgrabber", value) == false) {
        value = true;
    }
    actualMediaFoundationGrabber = value;
    mediaFoundationGrabberCheck->setChecked(actualMediaFoundationGrabber);
    changeMediaFoundationGrabberCheckState(actualMediaFoundationGrabber);

    if (pref->getIntegerPreference("preferences", "mediafoundationcontroller", value) == false) {
        value = false;
    }
    actualMediaFoundationController = value;
    mediaFoundationControllerCheck->setChecked(actualMediaFoundationController);
#else
    mediaFoundationGrabberCheck->hide();
    mediaFoundationControllerCheck->hide();
#endif

#ifdef Q_OS_LINUX
    // gphoto2 device
    if (pref->getIntegerPreference("preferences", "gphoto2grabber", value) == false) {
        value = false;
    }
    actualGphoto2Grabber = value;
    gphoto2GrabberCheck->setChecked(actualGphoto2Grabber);
    changeGphoto2GrabberCheckState(actualGphoto2Grabber);

    if (pref->getIntegerPreference("preferences", "gphoto2controller", value) == false) {
        value = false;
    }
    actualGphoto2Controller = value;
    gphoto2ControllerCheck->setChecked(actualGphoto2Controller);
#else
    gphoto2GrabberCheck->hide();
    gphoto2ControllerCheck->hide();
#endif

    qDebug() << "GrabberWidget::initialize --> End";
}

/*
void GrabberWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "GrabberWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug() << "GrabberWidget::resizeEvent --> End");
}
*/

void GrabberWidget::apply()
{
    qDebug() << "GrabberWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    bool             changes = false;

    bool newGstreamerVideoTestGrabber = gstreamerVideoTestGrabberCheck->isChecked();
    if (newGstreamerVideoTestGrabber != actualGstreamerVideoTestGrabber) {
        // Video test grabber changed
        pref->setIntegerPreference("preferences", "gstreamervideotestgrabber", newGstreamerVideoTestGrabber);
        actualGstreamerVideoTestGrabber = newGstreamerVideoTestGrabber;
        changes = true;
    }

    bool newGstreamerV4L2Grabber = gstreamerV4L2GrabberCheck->isChecked();
    if (newGstreamerV4L2Grabber != actualGstreamerV4L2Grabber) {
        // Video 4 Linux 2 grabber changed
        pref->setIntegerPreference("preferences", "gstreamerv4l2grabber", newGstreamerV4L2Grabber);
        actualGstreamerV4L2Grabber = newGstreamerV4L2Grabber;
        changes = true;
    }

    bool newGstreamerDv1394Grabber = gstreamerDv1394GrabberCheck->isChecked();
    if (newGstreamerDv1394Grabber != actualGstreamerDv1394Grabber) {
        // DV1394 grabber changed
        pref->setIntegerPreference("preferences", "gstreamerdv1394grabber", newGstreamerDv1394Grabber);
        actualGstreamerDv1394Grabber = newGstreamerDv1394Grabber;
        changes = true;
    }

    bool newV4L2Grabber = v4l2GrabberCheck->isChecked();
    if (newV4L2Grabber != actualV4L2Grabber) {
        // Video 4 Linux 2 grabber changed
        pref->setIntegerPreference("preferences", "v4l2grabber", newV4L2Grabber);
        actualV4L2Grabber = newV4L2Grabber;
        changes = true;
    }

    bool newV4L2Controller = v4l2ControllerCheck->isChecked();
    if (newV4L2Controller != actualV4L2Controller) {
        // Video 4 Linux 2 controller changed
        pref->setIntegerPreference("preferences", "v4l2controller", newV4L2Controller);
        actualV4L2Controller = newV4L2Controller;
        changes = true;
    }

    bool newMediaFoundationGrabber = mediaFoundationGrabberCheck->isChecked();
    if (newMediaFoundationGrabber != actualMediaFoundationGrabber) {
        // Media Foundation grabber changed
        pref->setIntegerPreference("preferences", "mediafoundationgrabber", newMediaFoundationGrabber);
        actualMediaFoundationGrabber = newMediaFoundationGrabber;
        changes = true;
    }

    bool newMediaFoundationController = mediaFoundationControllerCheck->isChecked();
    if (newMediaFoundationController != actualMediaFoundationController) {
        // Media Foundation controller changed
        pref->setIntegerPreference("preferences", "mediafoundationcontroller", newMediaFoundationController);
        actualMediaFoundationController = newMediaFoundationController;
        changes = true;
    }

    bool newGphoto2Grabber = gphoto2GrabberCheck->isChecked();
    if (newGphoto2Grabber != actualGphoto2Grabber) {
        // Gphoto 2 grabber changed
        pref->setIntegerPreference("preferences", "gphoto2grabber", newGphoto2Grabber);
        actualGphoto2Grabber = newGphoto2Grabber;
        changes = true;
    }

    bool newGphoto2Controller = gphoto2ControllerCheck->isChecked();
    if (newGphoto2Controller != actualGphoto2Controller) {
        // Gphoto 2 controller changed
        pref->setIntegerPreference("preferences", "gphoto2controller", newGphoto2Controller);
        actualGphoto2Controller = newGphoto2Controller;
        changes = true;
    }

    if (true == changes) {
        // There are changes in the grabber preferences
        frontend->showInformation(tr("Information"), tr("Pease restart qStopMotion to activate the changings!"));
    }
    qDebug() << "GrabberWidget::apply --> End";
}


void GrabberWidget::reset()
{
    qDebug() << "GrabberWidget::reset --> Start";

    gstreamerVideoTestGrabberCheck->setChecked(actualGstreamerVideoTestGrabber);

    gstreamerV4L2GrabberCheck->setChecked(actualGstreamerV4L2Grabber);

    gstreamerDv1394GrabberCheck->setChecked(actualGstreamerDv1394Grabber);

    v4l2GrabberCheck->setChecked(actualV4L2Grabber);
    v4l2ControllerCheck->setChecked(actualV4L2Controller);

    mediaFoundationGrabberCheck->setChecked(actualMediaFoundationGrabber);
    mediaFoundationControllerCheck->setChecked(actualMediaFoundationController);

    gphoto2GrabberCheck->setChecked(actualGphoto2Grabber);
    gphoto2ControllerCheck->setChecked(actualGphoto2Controller);

    qDebug() << "GrabberWidget::reset --> End";
}


void GrabberWidget::changeV4L2GrabberCheckState(int newState)
{
    if (newState) {
        v4l2ControllerCheck->setEnabled(true);
    }
    else {
        v4l2ControllerCheck->setEnabled(false);
    }
}


void GrabberWidget::changeMediaFoundationGrabberCheckState(int newState)
{
    if (newState) {
        mediaFoundationControllerCheck->setEnabled(true);
    }
    else {
        mediaFoundationControllerCheck->setEnabled(false);
    }
}


void GrabberWidget::changeGphoto2GrabberCheckState(int newState)
{
    if (newState) {
        gphoto2ControllerCheck->setEnabled(true);
    }
    else {
        gphoto2ControllerCheck->setEnabled(false);
    }
}
