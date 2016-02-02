/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include "transformwidget.h"

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
#include "technical/grabber/imagegrabber.h"


TransformWidget::TransformWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug() << "TransformWidget::Constructor --> Start";

    frontend                 = f;
    tabType                  = type;

    infoText                 = 0;

    // Transformation preferences
    transformPrefs           = 0;
    scaleButton              = 0;
    clipButton               = 0;
    zoomButton               = 0;
    activeTransform          = 0;

    // Adjustment preferences
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

    // Zoomw preferences
    zoomPrefs                = 0;
    zoomLabel                = 0;
    zoomSlider               = 0;
    activeZoomValue          = 20;
    zoomMinimumLabel         = 0;
    zoomMaximumLabel         = 0;

    this->setObjectName("TransformWidget");

    makeGUI();

    qDebug() << "TransformWidget::Constructor --> End";
}


void TransformWidget::makeGUI()
{
    qDebug() << "TransformWidget::makeGUI --> Start";

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    if (tabType) {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to a new project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }
    else {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to the currently active project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }

    infoText->setMinimumWidth(440);
    infoText->setMinimumHeight(100);
    infoText->setMaximumHeight(100);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Transformation preferences
    transformPrefs = new QGroupBox;
    transformPrefs->setMinimumSize(440, 300);
    transformPrefs->setTitle(tr("Transformation settings"));

    scaleButton = new QRadioButton(tr("Scale the whole image"));
    scaleButton->setChecked(true);
    scaleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(scaleButton, SIGNAL(clicked()), this, SLOT(setScaleButtonOn()));

    clipButton = new QRadioButton(tr("Clip a part of the image"));
    clipButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clipButton->setChecked(false);
    connect(clipButton, SIGNAL(clicked()), this, SLOT(setClipButtonOn()));

    zoomButton = new QRadioButton(tr("Zoom the image"));
    zoomButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoomButton->setChecked(false);
    connect(zoomButton, SIGNAL(clicked()), this, SLOT(setZoomButtonOn()));

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

    centerUpButton = new QRadioButton();
    centerUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerUpButton->setChecked(false);

    rightUpButton = new QRadioButton();
    rightUpButton->setChecked(false);
    rightUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftMiddleButton = new QRadioButton();
    leftMiddleButton->setChecked(false);
    leftMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centerMiddleButton = new QRadioButton();
    centerMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerMiddleButton->setChecked(false);

    rightMiddleButton = new QRadioButton();
    rightMiddleButton->setChecked(false);
    rightMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftDownButton = new QRadioButton();
    leftDownButton->setChecked(false);
    leftDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centerDownButton = new QRadioButton();
    centerDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerDownButton->setChecked(false);

    rightDownButton = new QRadioButton();
    rightDownButton->setChecked(false);
    rightDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

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

    // Zoom preferences
    zoomPrefs = new QGroupBox;
    zoomPrefs->setTitle(tr("Zoom settings"));

    zoomLabel = new QLabel(tr("Zoom value:"));
    zoomSlider = new QSlider();
    zoomSlider->setMaximum(1);
    zoomSlider->setMaximum(100);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMinimumWidth(300);
    zoomSlider->setMaximumWidth(300);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(5);
    zoomSlider->setSingleStep(5);
    zoomSlider->setPageStep(20);
    connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(changeZoom()));
    zoomMinimumLabel = new QLabel(tr("0%"));
    zoomMaximumLabel = new QLabel(tr("100%"));

    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(zoomMinimumLabel, 0, Qt::AlignLeft);
    zoomLayout->addStretch();
    zoomLayout->addWidget(zoomMaximumLabel, 0, Qt::AlignRight);

    QGridLayout *zoomPrefsLayout = new QGridLayout;
    zoomPrefsLayout->setColumnStretch(0, 1);
    zoomPrefsLayout->addWidget(zoomLabel, 1, 0, Qt::AlignLeft);
    zoomPrefsLayout->addWidget(zoomSlider, 1, 1, Qt::AlignRight);
    zoomPrefsLayout->addLayout(zoomLayout, 2, 1, Qt::AlignRight);
    zoomPrefs->setLayout(zoomPrefsLayout);

    // Transform preferences
    QVBoxLayout *transformPrefsLayout = new QVBoxLayout;
    hbLayout = new QHBoxLayout;
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);

    QVBoxLayout *vbLayout;
    vbLayout = new QVBoxLayout;
    vbLayout->addStretch(1);
    vbLayout->addWidget(scaleButton);
    vbLayout->addStretch(1);
    vbLayout->addWidget(clipButton);
    vbLayout->addStretch(1);
    vbLayout->addWidget(zoomButton);
    vbLayout->addStretch(1);
    transformPrefsLayout->addLayout(vbLayout);

    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(adjustmentPrefs);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);

    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(zoomPrefs);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);

    transformPrefsLayout->addStretch(1);
    transformPrefs->setLayout(transformPrefsLayout);

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(transformPrefs);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    qDebug() << "TransformWidget::makeGUI --> End";
}


void TransformWidget::initialize()
{
    qDebug() << "TransformWidget::initialize --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        if (pref->getIntegerPreference("preferences", "defaulttransformation", value) == false) {
            value = 0;
        }
        activeTransform = value;

        if (pref->getIntegerPreference("preferences", "defaultimageadjustment", value) == false) {
            value = ImageGrabber::centerDown;
        }
        activeImageAdjustment = value;

        if (pref->getIntegerPreference("preferences", "defaultzoomvalue", value) == false) {
            value = 25;
        }
        activeZoomValue = value;
    }
    else {
        // This is a project dialog tab
        activeTransform = frontend->getProject()->getImageTransformation();
        activeImageAdjustment = frontend->getProject()->getImageAdjustment();
        activeZoomValue = frontend->getProject()->getZoomValue();
    }

    // Transformation preferences
    switch (activeTransform) {
    case 0:
        setScaleButtonOn();
        break;
    case 1:
        setClipButtonOn();
        break;
    case 2:
        setZoomButtonOn();
        break;
    }

    setAdjustment(activeImageAdjustment);
    zoomSlider->setValue(activeZoomValue);


    qDebug() << "TransformWidget::initialize --> End";
}

/*
void TransformWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "TransformWidget::resizeEvent --> Start";

    QWidget::resizeEvent(event);

    qDebug() << "TransformWidget::resizeEvent --> End";
}
*/

void TransformWidget::apply()
{
    qDebug() << "TransformWidget::apply --> Start";

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    int value;
    bool changings = false;

    if (scaleButton->isChecked()) {
        if (activeTransform != 0) {
            activeTransform = 0;
            changings = true;
        }
    }
    else {
        if (clipButton->isChecked()) {
            if (activeTransform != 1) {
                activeTransform = 1;
                changings = true;
            }
        }
        else {
            if (activeTransform != 2) {
                activeTransform = 2;
                changings = true;
            }
        }
    }

    if (leftUpButton->isChecked()) {
        index = ImageGrabber::leftUp;
    }
    if (centerUpButton->isChecked()) {
        index = ImageGrabber::centerUp;
    }
    if (rightUpButton->isChecked()) {
        index = ImageGrabber::rightUp;
    }
    if (leftMiddleButton->isChecked()) {
        index = ImageGrabber::leftMiddle;
    }
    if (centerMiddleButton->isChecked()) {
        index = ImageGrabber::centerMiddle;
    }
    if (rightMiddleButton->isChecked()) {
        index = ImageGrabber::rightMiddle;
    }
    if (leftDownButton->isChecked()) {
        index = ImageGrabber::leftDown;
    }
    if (centerDownButton->isChecked()) {
        index = ImageGrabber::centerDown;
    }
    if (rightDownButton->isChecked()) {
        index = ImageGrabber::rightDown;
    }
    if (activeImageAdjustment != index)
    {
        activeImageAdjustment = index;
        changings = true;
    }
    value = zoomSlider->value();
    if (activeZoomValue != value) {
        activeZoomValue = value;
        changings = true;
    }


    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setIntegerPreference("preferences", "defaulttransformation", activeTransform);
            pref->setIntegerPreference("preferences", "defaultimageadjustment", activeImageAdjustment);
            pref->setIntegerPreference("preferences", "defaultzoomvalue", activeZoomValue);
        }
        else {
            // This is a project dialog tab
            frontend->getProject()->setImageTransformation(activeTransform);
            frontend->getProject()->setImageAdjustment(activeImageAdjustment);
            frontend->getProject()->setZoomValue(activeZoomValue);
        }
    }

    qDebug() << "TransformWidget::apply --> End";
}


void TransformWidget::reset()
{
    qDebug() << "TransformWidget::reset --> Start";

    switch (activeTransform) {
    case 0:
        setScaleButtonOn();
        break;
    case 1:
        setClipButtonOn();
        break;
    case 2:
        setZoomButtonOn();
        break;
    }
    setAdjustment(activeImageAdjustment);
    zoomSlider->setValue(activeZoomValue);

    qDebug() << "TransformWidget::reset --> End";
}


void TransformWidget::setAdjustment(int newAdjustment)
{
    qDebug() << "TransformWidget::setAdjustment --> Start";

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

    qDebug() << "TransformWidget::setAdjustment --> End";
}


void TransformWidget::setScaleButtonOn()
{
    scaleButton->setChecked(true);
    clipButton->setChecked(false);
    zoomButton->setChecked(false);

    adjustmentPrefs->setVisible(false);
    zoomPrefs->setVisible(false);
}


void TransformWidget::setClipButtonOn()
{
    clipButton->setChecked(true);
    scaleButton->setChecked(false);
    zoomButton->setChecked(false);

    adjustmentPrefs->setVisible(true);
    zoomPrefs->setVisible(false);
}


void TransformWidget::setZoomButtonOn()
{
    zoomButton->setChecked(true);
    scaleButton->setChecked(false);
    clipButton->setChecked(false);

    adjustmentPrefs->setVisible(false);
    zoomPrefs->setVisible(true);
}


void TransformWidget::changeZoom()
{
    qDebug() << "TransformWidget::changeZoom --> Start";

    int value = zoomSlider->value();

    if (activeZoomValue == value) {
        return;
    }

    /*
    if (!tabType) {
        // Project settings are changed

    }
    */

    qDebug() << "TransformWidget::changeZoom --> End";
}
