/******************************************************************************
 *  Copyright (C) 2014-2017 by                                                *
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
    v4l2GrabberCheck                       = 0;
    actualV4L2Grabber                      = V4L2GRABBERDEFAULT;
    v4l2ControllerCheck                    = 0;
    actualV4L2Controller                   = V4L2CONTROLERDEFAULT;
    mediaFoundationGrabberCheck            = 0;
    actualMediaFoundationGrabber           = MEDIAFOUNDATIONGRABBERDEFAULT;
    mediaFoundationControllerCheck         = 0;
    actualMediaFoundationController        = MEDIAFOUNDATIONCONTROLERDEFAULT;
    gphoto2GrabberCheck                    = 0;
    actualGphoto2Grabber                   = GPHOTO2GRABBERDEFAULT;
    gphoto2ControllerCheck                 = 0;
    actualGphoto2Controller                = GPHOTO2CONTROLERDEFAULT;

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
    grabberLayout->addWidget(v4l2GrabberCheck, 0, 0, 1, 2);
    grabberLayout->addWidget(v4l2ControllerCheck, 1, 1, 1, 1);
    grabberLayout->addWidget(mediaFoundationGrabberCheck, 2, 0, 1, 2);
    grabberLayout->addWidget(mediaFoundationControllerCheck, 3, 1, 1, 1);
    grabberLayout->addWidget(gphoto2GrabberCheck, 4, 0, 1, 2);
    grabberLayout->addWidget(gphoto2ControllerCheck, 5, 1, 1, 1);
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

#ifdef Q_OS_LINUX
    // Video4Linux2 device
    pref->getBooleanPreference("preferences", "v4l2grabber", actualV4L2Grabber);
    pref->getBooleanPreference("preferences", "v4l2controller", actualV4L2Controller);
#else
    v4l2GrabberCheck->hide();
    v4l2ControllerCheck->hide();
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Media Foundation device
    pref->getBooleanPreference("preferences", "mediafoundationgrabber", actualMediaFoundationGrabber);
    pref->getBooleanPreference("preferences", "mediafoundationcontroller", actualMediaFoundationController);
#else
    mediaFoundationGrabberCheck->hide();
    mediaFoundationControllerCheck->hide();
#endif

#ifdef Q_OS_LINUX
    // gphoto2 device
    pref->getBooleanPreference("preferences", "gphoto2grabber", actualGphoto2Grabber);
    pref->getBooleanPreference("preferences", "gphoto2controller", actualGphoto2Controller);
#else
    gphoto2GrabberCheck->hide();
    gphoto2ControllerCheck->hide();
#endif

    resetDialog();

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

    bool newV4L2Grabber = v4l2GrabberCheck->isChecked();
    if (newV4L2Grabber != actualV4L2Grabber) {
        // Video 4 Linux 2 grabber changed
        pref->setBooleanPreference("preferences", "v4l2grabber", newV4L2Grabber);
        actualV4L2Grabber = newV4L2Grabber;
        changes = true;
    }

    bool newV4L2Controller = v4l2ControllerCheck->isChecked();
    if (newV4L2Controller != actualV4L2Controller) {
        // Video 4 Linux 2 controller changed
        pref->setBooleanPreference("preferences", "v4l2controller", newV4L2Controller);
        actualV4L2Controller = newV4L2Controller;
        changes = true;
    }

    bool newMediaFoundationGrabber = mediaFoundationGrabberCheck->isChecked();
    if (newMediaFoundationGrabber != actualMediaFoundationGrabber) {
        // Media Foundation grabber changed
        pref->setBooleanPreference("preferences", "mediafoundationgrabber", newMediaFoundationGrabber);
        actualMediaFoundationGrabber = newMediaFoundationGrabber;
        changes = true;
    }

    bool newMediaFoundationController = mediaFoundationControllerCheck->isChecked();
    if (newMediaFoundationController != actualMediaFoundationController) {
        // Media Foundation controller changed
        pref->setBooleanPreference("preferences", "mediafoundationcontroller", newMediaFoundationController);
        actualMediaFoundationController = newMediaFoundationController;
        changes = true;
    }

    bool newGphoto2Grabber = gphoto2GrabberCheck->isChecked();
    if (newGphoto2Grabber != actualGphoto2Grabber) {
        // Gphoto 2 grabber changed
        pref->setBooleanPreference("preferences", "gphoto2grabber", newGphoto2Grabber);
        actualGphoto2Grabber = newGphoto2Grabber;
        changes = true;
    }

    bool newGphoto2Controller = gphoto2ControllerCheck->isChecked();
    if (newGphoto2Controller != actualGphoto2Controller) {
        // Gphoto 2 controller changed
        pref->setBooleanPreference("preferences", "gphoto2controller", newGphoto2Controller);
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

    resetDialog();

    qDebug() << "GrabberWidget::reset --> End";
}


void GrabberWidget::resetDialog()
{
    qDebug() << "GrabberWidget::resetDialog --> Start";

    v4l2GrabberCheck->setChecked(actualV4L2Grabber);
    changeV4L2GrabberCheckState(actualV4L2Grabber);
    v4l2ControllerCheck->setChecked(actualV4L2Controller);

    mediaFoundationGrabberCheck->setChecked(actualMediaFoundationGrabber);
    changeMediaFoundationGrabberCheckState(actualMediaFoundationGrabber);
    mediaFoundationControllerCheck->setChecked(actualMediaFoundationController);

    gphoto2GrabberCheck->setChecked(actualGphoto2Grabber);
    changeGphoto2GrabberCheckState(actualGphoto2Grabber);
    gphoto2ControllerCheck->setChecked(actualGphoto2Controller);

    qDebug() << "GrabberWidget::resetDialog --> End";
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
