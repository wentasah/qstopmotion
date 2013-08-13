/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"

#include <QtCore/QtDebug>
#include <QtCore/QRect>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>


TransformWidget::TransformWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug("TransformWidget::Constructor --> Start");

    frontend                 = f;
    tabType                  = type;

    // Transformation preferences
    transformPrefs           = 0;
    transformText            = 0;
    scaleButton              = 0;
    clipButton               = 0;

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

    this->setObjectName("TransformWidget");

    makeGUI();

    qDebug("TransformWidget::Constructor --> End");
}


void TransformWidget::makeGUI()
{
    qDebug("TransformWidget::makeGUI --> Start");

    // Transformation preferences
    transformPrefs = new QGroupBox;
    transformPrefs->setMinimumSize(440, 300);
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
    transformText->setMinimumHeight(75);
    transformText->setMaximumHeight(90);
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

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(transformPrefs);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    qDebug("TransformWidget::makeGUI --> End");
}


void TransformWidget::initialize()
{
    qDebug("TransformWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        if (pref->getIntegerPreference("preferences", "defaulttransformation", value) == false) {
            value = false;
        }
        activeTransform = value;

        if (pref->getIntegerPreference("preferences", "defaultimageadjustment", value) == false) {
            value = ImageGrabber::centerDown;
        }
        activeImageAdjustment = value;
    }
    else {
        // This is a project dialog tab
        activeTransform = frontend->getProject()->getImageTransformation();
        activeImageAdjustment = frontend->getProject()->getImageAdjustment();
    }

    // Transformation preferences
    if (activeTransform) {
        setScaleButtonOn();
    }
    else {
        setClipButtonOn();
    }

    setAdjustment(activeImageAdjustment);

    qDebug("TransformWidget::initialize --> End");
}

/*
void TransformWidget::resizeEvent(QResizeEvent *event)
{
    qDebug("TransformWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("TransformWidget::resizeEvent --> End");
}
*/

void TransformWidget::apply()
{
    qDebug("TransformWidget::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    bool changings = false;

    if (clipButton->isChecked()) {
        if (activeTransform) {
            activeTransform = false;
            changings = true;
        }
    }
    else {
        if (!activeTransform) {
            activeTransform = true;
            changings = true;
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

    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setIntegerPreference("preferences", "defaulttransformation", activeTransform);
            pref->setIntegerPreference("preferences", "defaultimageadjustment", activeImageAdjustment);
        }
        else {
            // This is a project dialog tab
            frontend->getProject()->setImageTransformation(activeTransform);
            frontend->getProject()->setImageAdjustment(activeImageAdjustment);
        }
    }

    qDebug("TransformWidget::apply --> End");
}


void TransformWidget::reset()
{
    qDebug("TransformWidget::reset --> Start");

    if (activeTransform)
    {
        setScaleButtonOn();
    }
    else
    {
        setClipButtonOn();
    }
    setAdjustment(activeImageAdjustment);

    qDebug("TransformWidget::reset --> End");
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

    adjustmentPrefs->setEnabled(false);

    leftUpButton->setEnabled(false);
    centerUpButton->setEnabled(false);
    rightUpButton->setEnabled(false);
    leftMiddleButton->setEnabled(false);
    centerMiddleButton->setEnabled(false);
    rightMiddleButton->setEnabled(false);
    leftDownButton->setEnabled(false);
    centerDownButton->setEnabled(false);
    rightDownButton->setEnabled(false);
}


void TransformWidget::setClipButtonOn()
{
    clipButton->setChecked(true);
    scaleButton->setChecked(false);

    adjustmentPrefs->setEnabled(true);

    leftUpButton->setEnabled(true);
    centerUpButton->setEnabled(true);
    rightUpButton->setEnabled(true);
    leftMiddleButton->setEnabled(true);
    centerMiddleButton->setEnabled(true);
    rightMiddleButton->setEnabled(true);
    leftDownButton->setEnabled(true);
    centerDownButton->setEnabled(true);
    rightDownButton->setEnabled(true);
}
