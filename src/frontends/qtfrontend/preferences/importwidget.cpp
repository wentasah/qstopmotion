/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include "importwidget.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"

#include <QtCore/QtDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ImportWidget::ImportWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug("ImportWidget::Constructor --> Start");

    frontend                 = f;
    tabType                  = type;

    infoText                 = 0;
    encoderTable             = 0;

    // Grabber preferences
    grabberPrefs             = 0;
    grabberSourceLabel       = 0;
    grabberSourceCombo       = 0;
    activeGrabberSource      = 0; // ImageEncoder::noneApplication;

    // Image preferences
    imagePrefs               = 0;
    imageFormatLabel         = 0;
    imageFormatCombo         = 0;
    activeImageFormat        = ImageGrabber::noneFormat;
    imageSizeLabel           = 0;
    imageSizeCombo           = 0;
    activeImageSize          = ImageGrabber::defaultSize;

    // Transformation preferences
    transformPrefs           = 0;
    transformText            = 0;
    scaleButton              = 0;
    clipButton               = 0;

    adjustmentPrefs          = 0;
    leftUpButton             = 0;
    centerUpButton           = 0;
    rightUpButton            = 0;
    leftMiddleButton         = 0;
    centerMiddleButton       = 0;
    rightMiddleButton        = 0;
    leftDownButton           = 0;
    centerDownButton         = 0;
    rightDownButton          = 0;
    activeImageAdjustment    = ImageGrabber::centerDown;

    this->setObjectName("ImportWidget");

    makeGUI();

    qDebug("ImportWidget::Constructor --> End");
}


void ImportWidget::makeGUI()
{
    qDebug("ImportWidget::makeGUI --> Start");

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    if (tabType) {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image sources should be used "
               "for importing images to a new project.") +
            "</p>");
    }
    else {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image sources should be used "
               "for importing images to the currently active project.") +
            "</p>");
    }

    infoText->setMinimumWidth(440);
    infoText->setMaximumHeight(52);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Encoder preferences
    grabberPrefs = new QGroupBox;
    grabberPrefs->setTitle(tr("Image grabber settings"));

    grabberSourceLabel = new QLabel(tr("Image Grabber:"));
    grabberSourceCombo = new QComboBox();
    grabberSourceLabel->setBuddy(grabberSourceCombo);
    grabberSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(grabberSourceCombo, SIGNAL(activated(int)), this, SLOT(changeEncoderApplication(int)));
    grabberSourceCombo->addItem(tr("Test Source"));
    grabberSourceCombo->addItem(tr("Video 4 Linux (USB WebCam)"));
    grabberSourceCombo->addItem(tr("IEEE 1394 (FireWire DigiCam)"));
    grabberSourceCombo->addItem(tr("Direct Show (USB WebCam)"));
    grabberSourceCombo->addItem(tr("Direct Show (FireWire DigiCam)"));
    grabberSourceCombo->addItem(tr("gphoto (USB Compact Camera)"));

    imagePrefs = new QGroupBox;
    imagePrefs->setTitle(tr("Image import settings"));

    imageFormatLabel = new QLabel(tr("Image Format:"));
    imageFormatCombo = new QComboBox();
    imageFormatLabel->setBuddy(imageFormatCombo);
    imageFormatCombo->setFocusPolicy(Qt::NoFocus);
    connect(imageFormatCombo, SIGNAL(activated(int)), this, SLOT(changeImageFormat(int)));
    imageFormatCombo->addItem(tr("JPEG"));
    imageFormatCombo->addItem(tr("TIFF"));
    imageFormatCombo->addItem(tr("BMP"));

    imageSizeLabel = new QLabel(tr("Image Size:"));
    imageSizeCombo = new QComboBox();
    imageSizeLabel->setBuddy(imageSizeCombo);
    imageSizeCombo->setFocusPolicy(Qt::NoFocus);
    connect(imageSizeCombo, SIGNAL(activated(int)), this, SLOT(changeImageSize(int)));
    imageSizeCombo->addItem(tr("Default Grabber Size"));
    imageSizeCombo->addItem(tr("QVGA (320x240)"));
    imageSizeCombo->addItem(tr("VGA (640x480)"));
    imageSizeCombo->addItem(tr("SVGQ (800x600)"));
    imageSizeCombo->addItem(tr("PAL D (704x576)"));
    imageSizeCombo->addItem(tr("HD Ready (1280x720)"));
    imageSizeCombo->addItem(tr("Full HD (1900x1080)"));

    // Transformation preferences
    transformPrefs = new QGroupBox;
    transformPrefs->setTitle(tr("Transformation settings"));

    transformText = new QTextEdit;
    transformText->setReadOnly(true);
    if (tabType) {
        transformText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to a new project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }
    else {
        transformText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to the currently active project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }

    transformText->setMinimumWidth(440);
    transformText->setMaximumHeight(52);
    transformText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    scaleButton = new QRadioButton(tr("Scale the whole image"));
    scaleButton->setChecked(true);
    scaleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(scaleButton, SIGNAL(clicked()), this, SLOT(setScaleButtonOn()));

    clipButton = new QRadioButton(tr("Clip a part of the image"));
    clipButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clipButton->setChecked(false);
    connect(clipButton, SIGNAL(clicked()), this, SLOT(setClipButtonOn()));

    adjustmentPrefs = new QGroupBox;
    adjustmentPrefs->setMinimumSize(200, 100);
    // adjustmentPrefs->setStyleSheet("border-width = 2px");
    // adjustmentPrefs->setStyleSheet("border-color = black");
    // adjustmentPrefs->setStyleSheet("border = 2px solid black");
    // adjustmentPrefs->setStyle("plastique");
    // QPalette p = adjustmentPrefs->palette();
    // p.setColor(adjustmentPrefs->backgroundRole(), QColor(0, 255, 255));
    // adjustmentPrefs->setPalette(p);
    // adjustmentPrefs->setTitle(tr("Adjustment settings"));

    leftUpButton = new QRadioButton();
    leftUpButton->setChecked(false);
    leftUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(leftUpButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::leftUp)));

    centerUpButton = new QRadioButton();
    centerUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerUpButton->setChecked(false);
    connect(centerUpButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::centerUp)));

    rightUpButton = new QRadioButton();
    rightUpButton->setChecked(false);
    rightUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(rightUpButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::rightUp)));

    leftMiddleButton = new QRadioButton();
    leftMiddleButton->setChecked(false);
    leftMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(leftMiddleButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::leftMiddle)));

    centerMiddleButton = new QRadioButton();
    centerMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerMiddleButton->setChecked(false);
    connect(centerMiddleButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::centerMiddle)));

    rightMiddleButton = new QRadioButton();
    rightMiddleButton->setChecked(false);
    rightMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(rightMiddleButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::rightMiddle)));

    leftDownButton = new QRadioButton();
    leftDownButton->setChecked(false);
    leftDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(leftDownButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::leftDown)));

    centerDownButton = new QRadioButton();
    centerDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerDownButton->setChecked(false);
    connect(centerDownButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::centerDown)));

    rightDownButton = new QRadioButton();
    rightDownButton->setChecked(false);
    rightDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(rightDownButton, SIGNAL(clicked()), this, SLOT(setAdjustment(ImageGrabber::rightDown)));

    QVBoxLayout *adjustmentPrefsLayout = new QVBoxLayout;
    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftUpButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerUpButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightUpButton);
    adjustmentPrefsLayout->addLayout(hbLayout);
    adjustmentPrefsLayout->addStretch(1);

    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftMiddleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerMiddleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightMiddleButton);
    adjustmentPrefsLayout->addLayout(hbLayout);
    adjustmentPrefsLayout->addStretch(1);

    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftDownButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerDownButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightDownButton);
    adjustmentPrefsLayout->addLayout(hbLayout);

    adjustmentPrefs->setLayout(adjustmentPrefsLayout);

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(grabberPrefs);
    mainLayout->addWidget(imagePrefs);
    mainLayout->addWidget(transformPrefs);
    setLayout(mainLayout);

    // Grabber preferences
    QGridLayout *grabberPrefsLayout = new QGridLayout;
    grabberPrefsLayout->addWidget(grabberSourceLabel, 0, 0);
    grabberPrefsLayout->addWidget(grabberSourceCombo, 0, 1);
    grabberPrefs->setLayout(grabberPrefsLayout);

    // Image import preferences
    QGridLayout *imagePrefsLayout = new QGridLayout;
    imagePrefsLayout->addWidget(imageFormatLabel, 1, 0);
    imagePrefsLayout->addWidget(imageFormatCombo, 1, 1);
    imagePrefsLayout->addWidget(imageSizeLabel, 2, 0);
    imagePrefsLayout->addWidget(imageSizeCombo, 2, 1);
    imagePrefs->setLayout(imagePrefsLayout);

    // Transform preferences
    QVBoxLayout *transformPrefsLayout = new QVBoxLayout;
    hbLayout = new QHBoxLayout;
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    hbLayout->addStretch(1);

    transformPrefsLayout->addLayout(hbLayout);
    transformPrefsLayout->addWidget(transformText);
    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(scaleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(clipButton);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);
    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(adjustmentPrefs);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);
    transformPrefs->setLayout(transformPrefsLayout);

    qDebug("ImportWidget::makeGUI --> End");
}


void ImportWidget::initialize()
{
    qDebug("ImportWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        activeGrabberSource = pref->getBasicPreference("defaultgrabbersource", 0 /*ImageEncoder::noneApplication*/);
        activeImageFormat = pref->getBasicPreference("defaultimageformat", ImageGrabber::noneFormat);
        activeImageSize = pref->getBasicPreference("defaultimagesize", ImageGrabber::defaultSize);
        activeTransform = pref->getBasicPreference("defaulttransformation", false);
        activeImageAdjustment = pref->getBasicPreference("defaultimageadjustment", ImageGrabber::centerDown);
    }
    else {
        // This is a project dialog tab
        /*
        activeGrabberSource = frontend->getProject()->getGrabberSource();
        activeImageFormat = frontend->getProject()->getImageFormat();
        activeImageSize = frontend->getProject()->getImageSize();
        activeTransformation = frontend->getProject()->getTransformation();
        activeImageAdjustment = frontend->getProject()->getImageAdjustment();
        */
    }

    if (activeGrabberSource == 0)
    {
        grabberSourceCombo->setCurrentIndex(0);
    }
    else
    {
        grabberSourceCombo->setCurrentIndex(activeGrabberSource);
    }

    if (activeImageFormat == VideoEncoder::noneFormat)
    {
        imageFormatCombo->setCurrentIndex(0);
    }
    else
    {
        imageFormatCombo->setCurrentIndex(activeImageFormat);
    }

    if (activeImageSize == VideoEncoder::defaultSize)
    {
        imageSizeCombo->setCurrentIndex(0);
    }
    else
    {
        imageSizeCombo->setCurrentIndex(activeImageSize);
    }

    // Transformation preferences
    if (activeTransform)
    {
        setScaleButtonOn();
    }
    else
    {
        setClipButtonOn();
    }

    setAdjustment(activeImageAdjustment);

    qDebug("ImportWidget::initialize --> End");
}

/*
void ImportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug("ImportWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ImportWidget::resizeEvent --> End");
}
*/

void ImportWidget::apply()
{
    qDebug("ImportWidget::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    bool changings = false;

    index = grabberSourceCombo->currentIndex();
    if (activeGrabberSource != index)
    {
        activeGrabberSource = index;
        changings = true;
    }

    index = imageFormatCombo->currentIndex();
    if (activeImageFormat != index)
    {
        activeImageFormat = index;
        changings = true;
    }

    index = imageSizeCombo->currentIndex();
    if (activeImageSize != index)
    {
        activeImageSize = index;
        changings = true;
    }

    if (clipButton->isChecked()) {
        if (activeTransform) {
            activeTransform = false;
            changings = true;
        }
    }

    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setBasicPreference("defaultgrabbersource", activeGrabberSource);
            pref->setBasicPreference("defaultimageformat", activeImageFormat);
            pref->setBasicPreference("defaultimagesize", activeImageSize);
            pref->setBasicPreference("defaulttransformation", activeTransform);
            pref->setBasicPreference("defaultimageadjustment", activeImageAdjustment);
        }
        else {
            // This is a project dialog tab
            /*
            frontend->getProject()->setGrabberSource(activeGrabberSource);
            frontend->getProject()->setImageFormat(activeImageFormat);
            frontend->getProject()->setImageSize(activeImageSize);
            frontend->getProject()->setTransformation(activeTransformation);
            frontend->getProject()->setImageAdjustment(activeImageAdjustment);
            */
        }
    }

    qDebug("ImportWidget::apply --> End");
}


void ImportWidget::reset()
{
    qDebug("ImportWidget::reset --> Start");

    grabberSourceCombo->setCurrentIndex(activeGrabberSource);
    imageFormatCombo->setCurrentIndex(activeImageFormat);
    imageSizeCombo->setCurrentIndex(activeImageSize);
    if (activeTransform)
    {
        setScaleButtonOn();
    }
    else
    {
        setClipButtonOn();
    }
    setAdjustment(activeImageAdjustment);

    qDebug("ImportWidget::reset --> End");
}


void ImportWidget::changeImageGrabberSources(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageGrabberSources --> Start";

    // qDebug() << "ImportWidget::changeImageGrabberSources --> End";
}


void ImportWidget::changeImageFormat(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageFormat --> Start";

    // qDebug() << "ImportWidget::changeImageFormat --> End";
}


void ImportWidget::changeImageSize(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageSize --> Start";

    // qDebug() << "ImportWidget::changeImageSize --> End";
}


void ImportWidget::setScaleButtonOn()
{
    scaleButton->setChecked(true);
    clipButton->setChecked(false);
    // defaultOutputEdit->setEnabled(false);
    // browseOutputButton->setEnabled(false);
}


void ImportWidget::setClipButtonOn()
{
    clipButton->setChecked(true);
    scaleButton->setChecked(false);
    // defaultOutputEdit->setEnabled(true);
    // browseOutputButton->setEnabled(true);
}


void ImportWidget::setAdjustment(int newAdjustment)
{
    activeImageAdjustment = newAdjustment;

    leftUpButton->setChecked(false);
    centerUpButton->setChecked(false);
    rightUpButton->setChecked(false);

    switch (newAdjustment) {
    case ImageGrabber::leftUp:
        leftUpButton->setChecked(true);
        break;
    case ImageGrabber::centerUp:
        centerUpButton->setChecked(true);
        break;
    case ImageGrabber::rightUp:
        rightUpButton->setChecked(true);
        break;
    case ImageGrabber::leftMiddle:
        leftMiddleButton->setChecked(true);
        break;
    case ImageGrabber::centerMiddle:
        centerMiddleButton->setChecked(true);
        break;
    case ImageGrabber::rightMiddle:
        rightMiddleButton->setChecked(true);
        break;
    case ImageGrabber::leftDown:
        leftDownButton->setChecked(true);
        break;
    case ImageGrabber::centerDown:
        centerDownButton->setChecked(true);
        break;
    case ImageGrabber::rightDown:
        rightDownButton->setChecked(true);
        break;
    }
}
