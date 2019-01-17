/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include "exportwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"


ExportWidget::ExportWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug() << "ExportWidget::Constructor --> Start";

    frontend                   = f;
    tabType                    = type;

    infoText                   = 0;
    encoderTable               = 0;

    // Encoder preferences
    encoderPrefs               = 0;
    encoderApplicationLabel    = 0;
    encoderApplicationCombo    = 0;
    activeEncoderApplication   = DomainFacade::ENCODERAPPLICATIONDEFAULT;
    videoFormatLabel           = 0;
    videoFormatCombo           = 0;
    activeVideoFormat          = DomainFacade::VIDEOFORMATDEFAULT;
    videoSizeLabel             = 0;
    videoSizeCombo             = 0;
    activeVideoSize            = DomainFacade::VIDEOSIZEDEFAULT;
    videoFpsLabel              = 0;
    videoFpsChooser            = 0;
    activeVideoFps             = DomainFacade::VIDEOFPSDEFAULT;

    // Movie's splitting preferences
    splittingPrefs             = 0;
    wholeMovieButton           = 0;
    splittedOnScenesButton     = 0;
    splittedOnTakesButton      = 0;
    splittingMode              = DomainFacade::MOVIESPLITTINGMODEDEFAULT;

    // Output file preferences
    outputPrefs                = 0;
    askForOutputLabel          = 0;
    yesButton                  = 0;
    noButton                   = 0;
    activeEncoderApplication   = 0;
    defaultOutputLabel         = 0;
    defaultOutputEdit          = 0;
    activeDefaultOutputFileName.clear();
    browseOutputButton         = 0;
    activeUseDefaultOutputFile = DomainFacade::USEDEFAULTOUTPUTFILENAMEDEFAULT;

    this->setObjectName("ExportWidget");

    makeGUI();

    qDebug() << "ExportWidget::Constructor --> End";
}


void ExportWidget::makeGUI()
{
    qDebug() << "ExportWidget::makeGUI --> Start";

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    if (tabType) {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which program should be used "
               "for encoding a new project to a video file.") +
            "</p>");
    }
    else {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which program should be used "
               "for encoding the currently active project to a video file.") +
            "</p>");
    }

    infoText->setMinimumWidth(440);
    infoText->setMaximumHeight(52);
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
    encoderApplicationCombo->addItem(tr("libav"));

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
    videoSizeCombo->addItem(tr("SVGA (800x600)"));
    videoSizeCombo->addItem(tr("PAL D (704x576)"));
    videoSizeCombo->addItem(tr("HD Ready (1280x720)"));
    videoSizeCombo->addItem(tr("Full HD (1900x1080)"));

    videoFpsLabel = new QLabel(tr("Frames per Second:"));
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

    // Movie's splitting preferences
    splittingPrefs = new QGroupBox;
    splittingPrefs->setTitle(tr("Splitting up the movie on several files"));

    wholeMovieButton = new QRadioButton(tr("All scenes and takes will be united in one movie"));
    wholeMovieButton->setChecked(true);
    wholeMovieButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    splittedOnScenesButton = new QRadioButton(tr("Movie will be splitted up by scenes"));
    splittedOnScenesButton->setChecked(false);
    splittedOnScenesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    splittedOnTakesButton = new QRadioButton(tr("Movie will be splitted up by takes"));
    splittedOnTakesButton->setChecked(false);
    splittedOnTakesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Output file preferences
    outputPrefs = new QGroupBox;
    outputPrefs->setTitle(tr("Output file settings"));

    askForOutputLabel = new QLabel(
        tr("Do you want to be asked for an output file everytime you choose to export?"));
    askForOutputLabel->setWordWrap(true);

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

    // Movie's splitting preferences
    QVBoxLayout *splittingPrefsLayout = new QVBoxLayout;
    splittingPrefsLayout->addStretch(1);
    splittingPrefsLayout->addWidget(wholeMovieButton);
    splittingPrefsLayout->addStretch(1);
    splittingPrefsLayout->addWidget(splittedOnScenesButton);
    splittingPrefsLayout->addStretch(1);
    splittingPrefsLayout->addWidget(splittedOnTakesButton);
    splittingPrefsLayout->addStretch(1);
    splittingPrefs->setLayout(splittingPrefsLayout);

    // Output file preferences
    QVBoxLayout *outputPrefsLayout = new QVBoxLayout;
    outputPrefsLayout->addWidget(askForOutputLabel);

    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->addWidget(yesButton);
    hbLayout->addWidget(noButton);
    hbLayout->addStretch(1);
    outputPrefsLayout->addLayout(hbLayout);
    if (!tabType) {
        outputPrefsLayout->addWidget(defaultOutputLabel);
        hbLayout = new QHBoxLayout;
        hbLayout->addWidget(defaultOutputEdit);
        hbLayout->addWidget(browseOutputButton);
        outputPrefsLayout->addLayout(hbLayout);
    }
    outputPrefs->setLayout(outputPrefsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(encoderPrefs);
    mainLayout->addWidget(splittingPrefs);
    mainLayout->addWidget(outputPrefs);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "ExportWidget::makeGUI --> End";
}


void ExportWidget::initialize()
{
    qDebug() << "ExportWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab

        pref->getIntegerPreference("preferences", "defaultencoderapplication", activeEncoderApplication);
        pref->getIntegerPreference("preferences", "defaultvideoformat", activeVideoFormat);
        pref->getIntegerPreference("preferences", "defaultvideosize", activeVideoSize);
        pref->getIntegerPreference("preferences", "defaultvideofps", activeVideoFps);
        pref->getIntegerPreference("preferences", "defaultmovieexportsplittingmode", splittingMode);
        pref->getBooleanPreference("preferences", "defaultusedefaultoutputfile", activeUseDefaultOutputFile);
    }
    else {
        // This is a project dialog tab
        activeEncoderApplication = frontend->getProject()->getEncoderApplication();
        activeVideoFormat = frontend->getProject()->getVideoFormat();
        activeVideoSize = frontend->getProject()->getVideoSize();
        activeVideoFps = frontend->getProject()->getVideoFps();
        splittingMode = frontend->getProject()->getMovieExportSplittingMode();
        activeUseDefaultOutputFile = frontend->getProject()->getUseDefaultOutputFile();
        activeDefaultOutputFileName = frontend->getProject()->getDefaultOutputFileName();
    }

    reset();

    qDebug() << "ExportWidget::initialize --> End";
}

/*
void ExportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "ExportWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug() << "ExportWidget::resizeEvent --> End");qDebug() << "
}
*/

void ExportWidget::apply()
{
    qDebug() << "ExportWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    bool changings = false;

    index = encoderApplicationCombo->currentIndex();
    if (activeEncoderApplication != index) {
        activeEncoderApplication = index;
        changings = true;
    }

    index = videoFormatCombo->currentIndex();
    if (activeVideoFormat != index) {
        activeVideoFormat = index;
        changings = true;
    }

    index = videoSizeCombo->currentIndex();
    if (activeVideoSize != index) {
        activeVideoSize = index;
        changings = true;
    }

    index = videoFpsChooser->value();
    if (activeVideoFps != index) {
        activeVideoFps = index;
        changings = true;
    }

    int currentSplittingMode = DomainFacade::MOVIESPLITTINGMODEDEFAULT;
    if (wholeMovieButton->isChecked()) {
        currentSplittingMode = DomainFacade::exportAsWholeMovie;
    } else if (splittedOnScenesButton->isChecked()) {
        currentSplittingMode = DomainFacade::exportAsSplittedOnScenes;
    } else if (splittedOnTakesButton->isChecked()) {
        currentSplittingMode = DomainFacade::exportAsSplittedOnTakes;
    } else {
        qWarning() << "unexpected buttons state: use default movie export mode";
    }
    if (currentSplittingMode != splittingMode) {
        splittingMode = currentSplittingMode;
        changings = true;
    }

    if (noButton->isChecked()) {
        if (false == activeUseDefaultOutputFile) {
            activeUseDefaultOutputFile = true;
            changings = true;
        }
    }
    else {
        if (true == activeUseDefaultOutputFile) {
            activeUseDefaultOutputFile = false;
            changings = true;
        }
    }

    if (activeDefaultOutputFileName.compare(defaultOutputEdit->text()) != 0) {
        activeDefaultOutputFileName.clear();
        activeDefaultOutputFileName.append(defaultOutputEdit->text());
        changings = true;
    }

    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setIntegerPreference("preferences", "defaultencoderapplication", activeEncoderApplication);
            pref->setIntegerPreference("preferences", "defaultvideoformat", activeVideoFormat);
            pref->setIntegerPreference("preferences", "defaultvideosize", activeVideoSize);
            pref->setIntegerPreference("preferences", "defaultvideofps", activeVideoFps);
            pref->setIntegerPreference("preferences", "defaultmovieexportsplittingmode", splittingMode);
            pref->setBooleanPreference("preferences", "defaultusedefaultoutputfile", activeUseDefaultOutputFile);
        }
        else {
            // This is a project dialog tab
            frontend->getProject()->setEncoderApplication(activeEncoderApplication);
            frontend->getProject()->setVideoFormat(activeVideoFormat);
            frontend->getProject()->setVideoSize(activeVideoSize);
            frontend->getProject()->setVideoFps(activeVideoFps);
            frontend->getProject()->setMovieExportSplittingMode(splittingMode);
            frontend->getProject()->setUseDefaultOutputFile(activeUseDefaultOutputFile);
            frontend->getProject()->setDefaultOutputFileName(activeDefaultOutputFileName);
        }
    }

    qDebug() << "ExportWidget::apply --> End";
}


void ExportWidget::reset()
{
    qDebug() << "ExportWidget::reset --> Start";

    if (activeEncoderApplication == DomainFacade::noneEncoderApplication) {
        encoderApplicationCombo->setCurrentIndex(0);
    }
    else {
        encoderApplicationCombo->setCurrentIndex(activeEncoderApplication);
    }

    if (activeVideoFormat == DomainFacade::noneFormat) {
        videoFormatCombo->setCurrentIndex(0);
    }
    else {
        videoFormatCombo->setCurrentIndex(activeVideoFormat);
    }

    if (activeVideoSize == DomainFacade::defaultVideoSize) {
        videoSizeCombo->setCurrentIndex(0);
    }
    else {
        videoSizeCombo->setCurrentIndex(activeVideoSize);
    }

    videoFpsChooser->setValue(activeVideoFps);

    changeSplittingMode(splittingMode);

    if (activeUseDefaultOutputFile) {
        setYesButtonOn();
    }
    else {
        setNoButtonOn();
    }

    defaultOutputEdit->setText(activeDefaultOutputFileName);

    qDebug() << "ExportWidget::reset --> End";
}


void ExportWidget::changeEncoderApplication(int /*index*/)
{
    // qDebug() << "ExportWidget::changeEncoderApplication --> Start";

    // qDebug() << "ExportWidget::changeEncoderApplication --> End";
}


void ExportWidget::changeVideoFormat(int /*index*/)
{
    // qDebug() << "ExportWidget::changeVideoFormat --> Start";

    // qDebug() << "ExportWidget::changeVideoFormat --> End";
}


void ExportWidget::changeVideoSize(int /*index*/)
{
    // qDebug() << "ExportWidget::changeVideoSize --> Start";

    // qDebug() << "ExportWidget::changeVideoSize --> End";
}


void ExportWidget::changeFps(int newFps)
{
}

void ExportWidget::changeSplittingMode(int value)
{
    switch (value) {
    case DomainFacade::exportAsWholeMovie:
        wholeMovieButton->setChecked(true);
        splittedOnScenesButton->setChecked(false);
        splittedOnTakesButton->setChecked(false);
        break;
    case DomainFacade::exportAsSplittedOnScenes:
        splittedOnScenesButton->setChecked(true);
        wholeMovieButton->setChecked(false);
        splittedOnTakesButton->setChecked(false);
        break;
    case DomainFacade::exportAsSplittedOnTakes:
        splittedOnTakesButton->setChecked(true);
        wholeMovieButton->setChecked(false);
        splittedOnScenesButton->setChecked(false);
        break;
    default:
        qWarning() << "ExportWidget::changeSplittingMode Invalid splitting mode:" << value;
        break;
    }
}


void ExportWidget::setYesButtonOn()
{
    yesButton->setChecked(true);
    noButton->setChecked(false);
    if (!tabType) {
        defaultOutputEdit->setEnabled(false);
        browseOutputButton->setEnabled(false);
    }
}


void ExportWidget::setNoButtonOn()
{
    noButton->setChecked(true);
    yesButton->setChecked(false);
    if (!tabType) {
        defaultOutputEdit->setEnabled(true);
        browseOutputButton->setEnabled(true);
    }
}


void ExportWidget::changeDefaultOutput(const QString &fileName)
{
    qDebug() << "ExportWidget::setDefaultOutput --> Start";

    qDebug() << "ExportWidget::setDefaultOutput --> End";
}


void ExportWidget::browseOutputFiles()
{
    QDir homeDir = QDir::home();

    QString outputFile = QFileDialog::getSaveFileName(this, tr("Choose output file"), homeDir.absolutePath());
    if (!outputFile.isEmpty()) {
        defaultOutputEdit->setText(outputFile);
    }
}
