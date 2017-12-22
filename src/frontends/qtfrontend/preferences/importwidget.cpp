/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QRect>
#include <QSpacerItem>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"


ImportWidget::ImportWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug() << "ImportWidget::Constructor --> Start";

    frontend                 = f;
    tabType                  = type;

    infoText                 = 0;
    encoderTable             = 0;

    // Image preferences
    imagePrefs               = 0;
    imageFormatLabel         = 0;
    imageFormatCombo         = 0;
    activeImageFormat        = DomainFacade::IMAGEFORMATDEFAULT;
    imageQualityLabel        = 0;
    imageQualitySlider       = 0;
    activeImageQuality       = DomainFacade::IMAGEQUALITYDEFAULT;
    qualityMinimumLabel      = 0;
    qualityMaximumLabel      = 0;
    imageSizeLabel           = 0;
    imageSizeCombo           = 0;
    activeImageSize          = DomainFacade::IMAGESIZEDEFAULT;

    // Live view preferences
    liveViewPrefs            = 0;
    liveViewFpsLabel         = 0;
    liveViewFpsSlider        = 0;
    activeLiveViewFps        = DomainFacade::LIVEVIEWFPSDEFAULT;

    this->setObjectName("ImportWidget");

    makeGUI();

    qDebug() << "ImportWidget::Constructor --> End";
}


void ImportWidget::makeGUI()
{
    qDebug() << "ImportWidget::makeGUI --> Start";

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
    infoText->setMinimumHeight(55);
    infoText->setMaximumHeight(60);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Image import preferences
    imagePrefs = new QGroupBox;
    imagePrefs->setTitle(tr("Image import settings"));

    imageFormatLabel = new QLabel(tr("Image Format:"));
    imageFormatCombo = new QComboBox();
    imageFormatLabel->setBuddy(imageFormatCombo);
    imageFormatCombo->setFocusPolicy(Qt::NoFocus);
    imageFormatCombo->setMinimumWidth(300);
    imageFormatCombo->setMaximumWidth(300);
    connect(imageFormatCombo, SIGNAL(activated(int)), this, SLOT(changeImageFormat(int)));
    imageFormatCombo->addItem(tr("JPEG"));
    imageFormatCombo->addItem(tr("TIFF"));
    imageFormatCombo->addItem(tr("BMP"));

    imageQualityLabel = new QLabel(tr("Image Quality:"));
    imageQualitySlider = new QwtSlider();
    imageQualitySlider->setOrientation(Qt::Horizontal);
    imageQualitySlider->setScalePosition(QwtSlider::LeadingScale);
    imageQualitySlider->setGroove(true);
    imageQualitySlider->setScale(0.0, 100.0);
    imageQualitySlider->setMinimumWidth(300);
    imageQualitySlider->setMaximumWidth(300);
    /*
    imageQualitySlider->setTickInterval(20);
    imageQualitySlider->setSingleStep(10);
    imageQualitySlider->setPageStep(20);
    */
    connect(imageQualitySlider, SIGNAL(valueChanged(double)), this, SLOT(changeImageQuality(double)));
    qualityMinimumLabel = new QLabel(tr("Min"));
    qualityMaximumLabel = new QLabel(tr("Max"));

    imageSizeLabel = new QLabel(tr("Image Size:"));
    imageSizeCombo = new QComboBox();
    imageSizeLabel->setBuddy(imageSizeCombo);
    imageSizeCombo->setFocusPolicy(Qt::NoFocus);
    imageSizeCombo->setMinimumWidth(300);
    imageSizeCombo->setMaximumWidth(300);
    connect(imageSizeCombo, SIGNAL(activated(int)), this, SLOT(changeImageSize(int)));
    imageSizeCombo->addItem(tr("Default Grabber Size"));
    imageSizeCombo->addItem(tr("QVGA (320x240)"));
    imageSizeCombo->addItem(tr("VGA (640x480)"));
    imageSizeCombo->addItem(tr("SVGA (800x600)"));
    imageSizeCombo->addItem(tr("PAL D (704x576)"));
    imageSizeCombo->addItem(tr("HD Ready (1280x720)"));
    imageSizeCombo->addItem(tr("Full HD (1900x1080)"));

    QHBoxLayout *icLayout = new QHBoxLayout;
    icLayout->addWidget(qualityMinimumLabel, 0, Qt::AlignLeft);
    icLayout->addStretch();
    icLayout->addWidget(qualityMaximumLabel, 0, Qt::AlignRight);

    QGridLayout *imagePrefsLayout = new QGridLayout;
    imagePrefsLayout->setColumnStretch(0, 1);
    imagePrefsLayout->addWidget(imageFormatLabel, 0, 0, Qt::AlignLeft);
    imagePrefsLayout->addWidget(imageFormatCombo, 0, 1, Qt::AlignRight);
    imagePrefsLayout->addWidget(imageQualityLabel, 1, 0, 1, 2, Qt::AlignLeft);
    imagePrefsLayout->addLayout(icLayout, 1, 1, Qt::AlignRight);
    imagePrefsLayout->addWidget(imageQualitySlider, 2, 1, Qt::AlignRight);
    imagePrefsLayout->addWidget(imageSizeLabel, 3, 0, Qt::AlignLeft);
    imagePrefsLayout->addWidget(imageSizeCombo, 3, 1, Qt::AlignRight);
    imagePrefs->setLayout(imagePrefsLayout);

    // Live view preferences
    liveViewPrefs = new QGroupBox;
    liveViewPrefs->setTitle(tr("Live view settings"));

    liveViewFpsLabel = new QLabel(tr("Frames per second:"));
    liveViewFpsSlider = new QwtSlider();
    liveViewFpsSlider->setOrientation(Qt::Horizontal);
    liveViewFpsSlider->setScalePosition(QwtSlider::LeadingScale);
    liveViewFpsSlider->setGroove(true);
    liveViewFpsSlider->setScale(0.0, 5.0);
    liveViewFpsSlider->setTotalSteps(5);
    liveViewFpsSlider->setSingleSteps(1);
    liveViewFpsSlider->setStepAlignment(true);
    liveViewFpsSlider->setMinimumWidth(300);
    liveViewFpsSlider->setMaximumWidth(300);
    /*
    liveViewFpsSlider->setTickInterval(2);
    liveViewFpsSlider->setSingleStep(5);
    liveViewFpsSlider->setPageStep(10);
    */
    connect(liveViewFpsSlider, SIGNAL(valueChanged(double)), this, SLOT(changeLiveViewFps(double)));

    QGridLayout *liveViewPrefsLayout = new QGridLayout;
    liveViewPrefsLayout->setColumnStretch(0, 1);
    liveViewPrefsLayout->addWidget(liveViewFpsLabel, 1, 0, Qt::AlignLeft);
    liveViewPrefsLayout->addWidget(liveViewFpsSlider, 1, 1, Qt::AlignRight);
    liveViewPrefs->setLayout(liveViewPrefsLayout);

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(imagePrefs);
    mainLayout->addWidget(liveViewPrefs);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    qDebug() << "ImportWidget::makeGUI --> End";
}


void ImportWidget::initialize()
{
    qDebug() << "ImportWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        pref->getIntegerPreference("preferences", "defaultimageformat", activeImageFormat);
        pref->getIntegerPreference("preferences", "defaultimagequality", activeImageQuality);
        pref->getIntegerPreference("preferences", "defaultimagesize", activeImageSize);
        pref->getDoublePreference("preferences", "defaultliveviewfps", activeLiveViewFps);
    }
    else {
        // This is a project dialog tab
        activeImageFormat = frontend->getProject()->getImageFormat();
        activeImageQuality = frontend->getProject()->getImageQuality();
        activeImageSize = frontend->getProject()->getImageSize();
        activeLiveViewFps = frontend->getProject()->getLiveViewFps();
    }

    reset();

    qDebug() << "ImportWidget::initialize --> End";
}

/*
void ImportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "ImportWidget::resizeEvent --> Start";

    QWidget::resizeEvent(event);

    qDebug() << "ImportWidget::resizeEvent --> End";
}
*/

void ImportWidget::apply()
{
    qDebug() << "ImportWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              index;
    int              value;
    double           doubleValue;
    bool             changings = false;

    index = imageFormatCombo->currentIndex();
    if (activeImageFormat != index) {
        activeImageFormat = index;
        changings = true;
    }

    value = imageQualitySlider->value();
    if (activeImageQuality != value) {
        activeImageQuality = value;
        changings = true;
    }

    index = imageSizeCombo->currentIndex();
    if (activeImageSize != index) {
        activeImageSize = index;
        changings = true;
    }

    doubleValue = liveViewFpsSlider->value();
    if (activeLiveViewFps != doubleValue) {
        activeLiveViewFps = doubleValue;
        changings = true;
    }

    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setIntegerPreference("preferences", "defaultimageformat", activeImageFormat);
            pref->setIntegerPreference("preferences", "defaultimagequality", activeImageQuality);
            pref->setIntegerPreference("preferences", "defaultimagesize", activeImageSize);
            pref->setDoublePreference("preferences", "defaultliveviewfps", activeLiveViewFps);
        }
        else {
            // This is a project dialog tab
            frontend->getProject()->setImageFormat(activeImageFormat);
            frontend->getProject()->setImageQuality(activeImageQuality);
            frontend->getProject()->setImageSize(activeImageSize);
            frontend->getProject()->setLiveViewFps(activeLiveViewFps);
        }
    }

    qDebug() << "ImportWidget::apply --> End";
}


void ImportWidget::reset()
{
    qDebug() << "ImportWidget::reset --> Start";

    imageFormatCombo->setCurrentIndex(activeImageFormat);
    enableQuality();
    imageQualitySlider->setValue(activeImageQuality);
    imageSizeCombo->setCurrentIndex(activeImageSize);
    liveViewFpsSlider->setValue(activeLiveViewFps);

    qDebug() << "ImportWidget::reset --> End";
}


void ImportWidget::changeImageFormat(int index)
{
    qDebug() << "ImportWidget::changeImageFormat --> Start";

    if (!tabType) {
        // Project settings are changed

        // Convert the existing images

        if (frontend->convertImages(index, activeImageQuality)) {
            // Images converted
            activeImageFormat = index;
        }
        else {
            // Images not converted, user break
            imageFormatCombo->setCurrentIndex(activeImageFormat);
        }
    }

    enableQuality();

    qDebug() << "ImportWidget::changeImageFormat --> End";
}


void ImportWidget::changeImageQuality(double newQuality)
{
    qDebug() << "ImportWidget::changeImageQuality --> Start";

    int value = (int)newQuality;

    if (activeImageQuality == value) {
        return;
    }

    if (!tabType) {
        // Project settings are changed

        // Convert the existing images

        if (frontend->convertImages(activeImageFormat, value)) {
            // Images converted
            activeImageQuality = value;
        }
        else {
            // Images not converted, user break
            imageQualitySlider->setValue(activeImageQuality);
        }
    }

    qDebug() << "ImportWidget::changeImageQuality --> End";
}


void ImportWidget::changeImageSize(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageSize --> Start";

    // qDebug() << "ImportWidget::changeImageSize --> End";
}


void ImportWidget::enableQuality()
{
    if (imageFormatCombo->currentIndex() == DomainFacade::jpegFormat) {
        imageQualityLabel->setEnabled(true);
        imageQualitySlider->setEnabled(true);
        qualityMinimumLabel->setEnabled(true);
        qualityMaximumLabel->setEnabled(true);
    }
    else {
        imageQualityLabel->setEnabled(false);
        imageQualitySlider->setEnabled(false);
        qualityMinimumLabel->setEnabled(false);
        qualityMaximumLabel->setEnabled(false);
    }
}


void ImportWidget::changeLiveViewFps(double value)
{
    qDebug() << "ImportWidget::changeLiveViewFps --> Start";

    if (activeLiveViewFps == value) {
        return;
    }

    /*
    if (!tabType) {
        // Project settings are changed

    }
    */

    qDebug() << "ImportWidget::changeLiveViewFps --> End";
}
