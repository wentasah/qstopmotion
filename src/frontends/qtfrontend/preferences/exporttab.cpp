/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include "exporttab.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/videoencoder/videoencoder.h"

#include <QtCore/QtDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ExportTab::ExportTab(Frontend *f, QWidget *parent) : QWidget(parent)
{
    qDebug("ExportTab::Constructor --> Start");

    frontend                 = f;

    infoText                 = 0;
    encoderTable             = 0;

    // Encoder preferences
    encoderPrefs             = 0;
    encoderApplicationLabel  = 0;
    encoderApplicationCombo  = 0;
    activeEncoderApplication = VideoEncoder::noneApplication;
    videoFormatLabel         = 0;
    videoFormatCombo         = 0;
    activeVideoFormat        = VideoEncoder::noneFormat;
    videoSizeLabel           = 0;
    videoSizeCombo           = 0;
    activeVideoSize          = VideoEncoder::defaultSize;
    videoFpsLabel            = 0;
    videoFpsChooser          = 0;
    activeProjectFps         = 1;

    // Output file preferences
    outputPrefs              = 0;
    askForOutputLabel        = 0;
    yesButton                = 0;
    noButton                 = 0;
    activeEncoderApplication = 0;
    defaultOutputLabel       = 0;
    defaultOutputEdit        = 0;
    defaultOutputfileName.clear();
    browseOutputButton       = 0;

    this->setObjectName("ExportTab");

    makeGUI();

    qDebug("ExportTab::Constructor --> End");
}


void ExportTab::makeGUI()
{
    qDebug("ExportTab::makeGUI --> Start");

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p>" +
        tr("Below you can set which program/process qStopMotion should use "
           "for encoding the currently active project to a video file.") +
        "</p>");

    infoText->setMinimumWidth(440);
    infoText->setMaximumHeight(50);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Encoder preferences
    encoderPrefs = new QGroupBox;
    encoderPrefs->setTitle(tr("Encoder settings"));

    encoderApplicationLabel = new QLabel(tr("Encoder Application:"));
    encoderApplicationCombo = new QComboBox();
    encoderApplicationLabel->setBuddy(encoderApplicationCombo);
    encoderApplicationCombo->setFocusPolicy(Qt::NoFocus);
    connect(encoderApplicationCombo, SIGNAL(activated(int)), this, SLOT(changeEncoderApplication(int)));
    encoderApplicationCombo->addItem(tr("ffmpeg"));
    /*
#ifdef Q_WS_X11
    encoderApplicationCombo->addItem(tr("USB Source"));
    encoderApplicationCombo->addItem(tr("FireWire Source"));
#endif
#ifdef Q_WS_WIN
    encoderApplicationCombo->addItem(tr("DirectShow Source"));
#endif
#ifdef Q_WS_MAC
#endif
    */

    videoFormatLabel = new QLabel(tr("Video Format:"));
    videoFormatCombo = new QComboBox();
    videoFormatLabel->setBuddy(videoFormatCombo);
    videoFormatCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoFormatCombo, SIGNAL(activated(int)), this, SLOT(changeVideoFormat(int)));
    videoFormatCombo->addItem(tr("AVI"));
    videoFormatCombo->addItem(tr("MP4"));

    videoSizeLabel = new QLabel(tr("Video Size:"));
    videoSizeCombo = new QComboBox();
    videoSizeLabel->setBuddy(videoSizeCombo);
    videoSizeCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoSizeCombo, SIGNAL(activated(int)), this, SLOT(changeVideoSize(int)));
    videoSizeCombo->addItem(tr("Frame Size"));
    videoSizeCombo->addItem(tr("QVGA (320x240)"));
    videoSizeCombo->addItem(tr("VGA (640x480)"));
    videoSizeCombo->addItem(tr("SVGQ (800x600)"));
    videoSizeCombo->addItem(tr("PAL D (704x576)"));
    videoSizeCombo->addItem(tr("HD Ready (1280x720)"));
    videoSizeCombo->addItem(tr("Full HD (1900x1080"));

    videoFpsLabel = new QLabel(tr("Project Frames per Second:"));
    videoFpsChooser = new QSpinBox();

    QString whatsThis =
        tr("<h4>FPS chooser</h4> "
           "<p>By changing the value in this "
           "chooser you set which speed the "
           "animation in the <b>FrameView</b> "
           "should run at.</p> "
           "<p>To start an animation press the "
           "<b>Run Animation</b> button.</p>");
    videoFpsLabel->setWhatsThis(whatsThis);
    videoFpsChooser->setWhatsThis(whatsThis);

    videoFpsChooser->setMinimum(1);
    videoFpsChooser->setMaximum(30);
    videoFpsChooser->setValue(1);
    videoFpsChooser->setFocusPolicy(Qt::NoFocus);
    connect(videoFpsChooser, SIGNAL(valueChanged(int)), this, SLOT(changeFps(int)));

    videoFpsLabel->setBuddy(videoFpsChooser);

    // Output file preferences
    outputPrefs = new QGroupBox;
    outputPrefs->setTitle(tr("Output file settings"));

    askForOutputLabel = new QLabel(
        tr("Do you want to be asked for an output file everytime you choose to export?"));

    yesButton = new QRadioButton(tr("Yes"));
    yesButton->setChecked(true);
    yesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(yesButton, SIGNAL(clicked()), this, SLOT(setYesButtonOn()));

    noButton = new QRadioButton(tr("No"));
    noButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    noButton->setChecked(false);
    connect(noButton, SIGNAL(clicked()), this, SLOT(setNoButtonOn()));

    defaultOutputLabel = new QLabel(tr("Set default output file:"));
    defaultOutputEdit = new FlexibleLineEdit;
    defaultOutputEdit->setEnabled(false);
    connect(defaultOutputEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(changeDefaultOutput(const QString &)));

    browseOutputButton = new QPushButton(tr("Browse"));
    browseOutputButton->setEnabled(false);
    browseOutputButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(browseOutputButton, SIGNAL(clicked()), this, SLOT(browseOutputFiles()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(encoderPrefs);
    mainLayout->addWidget(outputPrefs);
    setLayout(mainLayout);

    // Encoder preferences
    QGridLayout *encoderPrefsLayout = new QGridLayout;
    encoderPrefsLayout->addWidget(encoderApplicationLabel, 0, 0);
    encoderPrefsLayout->addWidget(encoderApplicationCombo, 0, 1);
    encoderPrefsLayout->addWidget(videoFormatLabel, 1, 0);
    encoderPrefsLayout->addWidget(videoFormatCombo, 1, 1);
    encoderPrefsLayout->addWidget(videoSizeLabel, 2, 0);
    encoderPrefsLayout->addWidget(videoSizeCombo, 2, 1);
    encoderPrefsLayout->addWidget(videoFpsLabel, 3, 0);
    encoderPrefsLayout->addWidget(videoFpsChooser, 3, 1);
    encoderPrefs->setLayout(encoderPrefsLayout);

    // Output file preferences
    QVBoxLayout *outputPrefsLayout = new QVBoxLayout;
    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    hbLayout->addStretch(1);

    outputPrefsLayout->addLayout(hbLayout);
    outputPrefsLayout->addWidget(askForOutputLabel);
    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(yesButton);
    hbLayout->addWidget(noButton);
    hbLayout->addStretch(1);
    outputPrefsLayout->addLayout(hbLayout);
    outputPrefsLayout->addWidget(defaultOutputLabel);
    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(defaultOutputEdit);
    hbLayout->addWidget(browseOutputButton);
    outputPrefsLayout->addLayout(hbLayout);
    outputPrefs->setLayout(outputPrefsLayout);

    qDebug("ExportTab::makeGUI --> End");
}


void ExportTab::initialize()
{
    qDebug("ExportTab::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    // Encoder preferences
    activeEncoderApplication = pref->getBasicPreference("encoderapplication", VideoEncoder::noneApplication);
    if (activeEncoderApplication == VideoEncoder::noneApplication)
    {
        encoderApplicationCombo->setCurrentIndex(0);
    }
    else
    {
        encoderApplicationCombo->setCurrentIndex(activeEncoderApplication);
    }

    activeVideoFormat = pref->getBasicPreference("videoformat", VideoEncoder::noneFormat);
    if (activeVideoFormat == VideoEncoder::noneFormat)
    {
        videoFormatCombo->setCurrentIndex(0);
    }
    else
    {
        videoFormatCombo->setCurrentIndex(activeVideoFormat);
    }

    activeVideoSize = pref->getBasicPreference("videosize", VideoEncoder::defaultSize);
    if (activeVideoSize == VideoEncoder::defaultSize)
    {
        videoSizeCombo->setCurrentIndex(0);
    }
    else
    {
        videoSizeCombo->setCurrentIndex(activeVideoSize);
    }

    activeProjectFps = frontend->getProject()->getFramesPerSecond();
    this->videoFpsChooser->setValue(activeProjectFps);

    // Output file preferences
    useDefaultOutputFile = pref->getBasicPreference("usedefaultoutputfile", 0);
    if (useDefaultOutputFile)
    {
        setYesButtonOn();
    }
    else
    {
        setNoButtonOn();
    }

    defaultOutputfileName.append(pref->getBasicPreference("defaultoutputfilename", ""));
    defaultOutputEdit->setText(defaultOutputfileName);

    qDebug("ExportTab::initialize --> End");
}

/*
void ExportTab::resizeEvent(QResizeEvent *event)
{
    qDebug("ExportTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ExportTab::resizeEvent --> End");
}
*/

void ExportTab::apply()
{
    qDebug("ExportTab::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    int newFps;

    index = encoderApplicationCombo->currentIndex();
    if (activeEncoderApplication != index)
    {
        pref->setBasicPreference("encoderapplication", index);
        activeEncoderApplication = index;
    }

    index = videoFormatCombo->currentIndex();
    if (activeVideoFormat != index)
    {
        pref->setBasicPreference("videoformat", index);
        activeVideoFormat = index;
    }

    index = videoSizeCombo->currentIndex();
    if (activeVideoSize != index)
    {
        pref->setBasicPreference("videosize", index);
        activeVideoSize = index;
    }

    newFps = videoFpsChooser->value();
    if (activeProjectFps != newFps) {
        frontend->getProject()->setFramesPerSecond(newFps);
    }

    qDebug("ExportTab::apply --> End");
}


void ExportTab::reset()
{
    qDebug("ExportTab::reset --> Start");

    encoderApplicationCombo->setCurrentIndex(activeEncoderApplication);
    videoFormatCombo->setCurrentIndex(activeVideoFormat);
    videoSizeCombo->setCurrentIndex(activeVideoSize);

    qDebug("ExportTab::reset --> End");
}


void ExportTab::changeEncoderApplication(int /*index*/)
{
    // qDebug() << "ExportTab::changeEncoderApplication --> Start";

    // qDebug() << "ExportTab::changeEncoderApplication --> End";
}


void ExportTab::changeVideoFormat(int /*index*/)
{
    // qDebug() << "ExportTab::changeVideoFormat --> Start";

    // qDebug() << "ExportTab::changeVideoFormat --> End";
}


void ExportTab::changeVideoSize(int /*index*/)
{
    // qDebug() << "ExportTab::changeVideoSize --> Start";

    // qDebug() << "ExportTab::changeVideoSize --> End";
}


void ExportTab::changeFps(int newFps)
{
}


void ExportTab::setYesButtonOn()
{
    if (useDefaultOutputFile == 0)
    {
        // Selection changed
        PreferencesTool *pref = frontend->getPreferences();

        pref->setBasicPreference("usedefaultoutputfile", 1);

        useDefaultOutputFile = 1;
    }

    yesButton->setChecked(true);
    noButton->setChecked(false);
    defaultOutputEdit->setEnabled(false);
    browseOutputButton->setEnabled(false);
}


void ExportTab::setNoButtonOn()
{
    if (useDefaultOutputFile == 1)
    {
        // Selection changed
        PreferencesTool *pref = frontend->getPreferences();

        pref->setBasicPreference("usedefaultoutputfile", 0);

        useDefaultOutputFile = 0;
    }

    noButton->setChecked(true);
    yesButton->setChecked(false);
    defaultOutputEdit->setEnabled(true);
    browseOutputButton->setEnabled(true);
}


void ExportTab::changeDefaultOutput(const QString &fileName)
{
    qDebug("ExportTab::setDefaultOutput --> Start");

    if (defaultOutputfileName.compare(fileName) != 0)
    {
        PreferencesTool *pref = frontend->getPreferences();

        pref->setBasicPreference("defaultoutputfilename", fileName);

        defaultOutputfileName.clear();
        defaultOutputfileName.append(fileName);
    }

    qDebug("ExportTab::setDefaultOutput --> End");
}


void ExportTab::browseOutputFiles()
{
    QDir homeDir = QDir::home();

    QString outputFile = QFileDialog::getSaveFileName(this, tr("Choose output file"), homeDir.absolutePath());
    if (!outputFile.isEmpty()) {
        defaultOutputEdit->setText(outputFile);
    }
}
