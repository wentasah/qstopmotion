/******************************************************************************
 *  Copyright (C) 2005-2018 by                                                *
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

#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"


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
    activeTransform          = DomainFacade::IMAGETRANSFORMDEFAULT;

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
    activeImageAdjustment    = DomainFacade::IMAGEADJUSTMENTDEFAULT;

    // Zoomw preferences
    zoomPrefs                = 0;
    zoomLabel                = 0;
    zoomSlider               = 0;
    activeZoomValue          = DomainFacade::ZOOMVALUEDEFAULT;
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

    zoomLabel = new QLabel(tr("Zoom value (%):"));
    zoomSlider = new QwtSlider();
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setScalePosition(QwtSlider::LeadingScale);
    zoomSlider->setGroove(true);
    zoomSlider->setScale(0.0, 100.0);
    zoomSlider->setMinimumWidth(300);
    zoomSlider->setMaximumWidth(300);
    /*
    zoomSlider->setTickInterval(5);
    zoomSlider->setSingleStep(5);
    zoomSlider->setPageStep(20);
    */
    connect(zoomSlider, SIGNAL(valueChanged(double)), this, SLOT(changeZoom(double)));
    zoomMinimumLabel = new QLabel(tr("Min"));
    zoomMaximumLabel = new QLabel(tr("Max"));

    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(zoomMinimumLabel, 0, Qt::AlignLeft);
    zoomLayout->addStretch();
    zoomLayout->addWidget(zoomMaximumLabel, 0, Qt::AlignRight);

    QGridLayout *zoomPrefsLayout = new QGridLayout;
    zoomPrefsLayout->setColumnStretch(0, 1);
    zoomPrefsLayout->addWidget(zoomLabel, 1, 0, Qt::AlignLeft);
    zoomPrefsLayout->addLayout(zoomLayout, 1, 1, Qt::AlignRight);
    zoomPrefsLayout->addWidget(zoomSlider, 2, 1, Qt::AlignRight);
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

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        pref->getIntegerPreference("preferences", "defaulttransformation", activeTransform);
        pref->getIntegerPreference("preferences", "defaultimageadjustment", activeImageAdjustment);
        pref->getDoublePreference("preferences", "defaultzoomvalue", activeZoomValue);
    }
    else {
        // This is a project dialog tab
        activeTransform = frontend->getProject()->getImageTransformation();
        activeImageAdjustment = frontend->getProject()->getImageAdjustment();
        activeZoomValue = frontend->getProject()->getZoomValue();
    }

    reset();

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
    double doubleValue;
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
        index = DomainFacade::leftUp;
    }
    if (centerUpButton->isChecked()) {
        index = DomainFacade::centerUp;
    }
    if (rightUpButton->isChecked()) {
        index = DomainFacade::rightUp;
    }
    if (leftMiddleButton->isChecked()) {
        index = DomainFacade::leftMiddle;
    }
    if (centerMiddleButton->isChecked()) {
        index = DomainFacade::centerMiddle;
    }
    if (rightMiddleButton->isChecked()) {
        index = DomainFacade::rightMiddle;
    }
    if (leftDownButton->isChecked()) {
        index = DomainFacade::leftDown;
    }
    if (centerDownButton->isChecked()) {
        index = DomainFacade::centerDown;
    }
    if (rightDownButton->isChecked()) {
        index = DomainFacade::rightDown;
    }
    if (activeImageAdjustment != index)
    {
        activeImageAdjustment = index;
        changings = true;
    }
    doubleValue = zoomSlider->value();
    if (activeZoomValue != doubleValue) {
        activeZoomValue = doubleValue;
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
    case DomainFacade::ScaleImage:
        setScaleButtonOn();
        break;
    case DomainFacade::ClipImage:
        setClipButtonOn();
        break;
    case DomainFacade::ZoomImage:
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
    case DomainFacade::leftUp:
        leftUpButton->setChecked(true);
        break;
    case DomainFacade::centerUp:
        centerUpButton->setChecked(true);
        break;
    case DomainFacade::rightUp:
        rightUpButton->setChecked(true);
        break;
    case DomainFacade::leftMiddle:
        leftMiddleButton->setChecked(true);
        break;
    case DomainFacade::centerMiddle:
        centerMiddleButton->setChecked(true);
        break;
    case DomainFacade::rightMiddle:
        rightMiddleButton->setChecked(true);
        break;
    case DomainFacade::leftDown:
        leftDownButton->setChecked(true);
        break;
    case DomainFacade::centerDown:
        centerDownButton->setChecked(true);
        break;
    case DomainFacade::rightDown:
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


void TransformWidget::changeZoom(double doubleValue)
{
    qDebug() << "TransformWidget::changeZoom --> Start";

    if (activeZoomValue == doubleValue) {
        return;
    }

    qDebug() << "TransformWidget::changeZoom --> End";
}
