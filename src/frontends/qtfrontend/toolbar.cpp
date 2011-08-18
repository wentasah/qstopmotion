/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
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

#include "frontends/qtfrontend/toolbar.h"

#include <QtCore/QtDebug>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ToolBar::ToolBar(Frontend *f,
                 RunAnimationHandler *runAnimationHandler,
                 CameraHandler *cameraHandler,
                 QWidget *parent) :
    QWidget(parent),
    runAnimationHandler(runAnimationHandler),
    cameraHandler(cameraHandler)
{
    frontend            = f;
    framesIcon          = 0;
    overlaySlider       = 0;
    cameraIcon          = 0;
    toBeginButton       = 0;
    previousFrameButton = 0;
    captureButton       = 0;
    playButton          = 0;
    nextFrameButton     = 0;
    toEndButton         = 0;

    makeGUI();
}


void ToolBar::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    framesIcon = new QLabel();
    iconFile.append(QLatin1String("frames.png"));
    framesIcon->setPixmap(QPixmap(iconFile));

    overlaySlider = new QSlider(Qt::Horizontal);

    cameraIcon = new QLabel();
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("camera.png"));
    cameraIcon->setPixmap(QPixmap(iconFile));

    toBeginButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("tobegin.png"));
    iconFile.append(QLatin1String("firstexposure.png"));
    toBeginButton->setIcon(QPixmap(iconFile));
    // toBeginButton->setFlat(true);
    // toBeginButton->setFocusPolicy( Qt::NoFocus );
    connect(toBeginButton, SIGNAL(clicked()), runAnimationHandler, SLOT(selectToBeginFrame()));
    toBeginButton->setEnabled(false);

    previousFrameButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("previousframe.png"));
    iconFile.append(QLatin1String("previousexposure.png"));
    previousFrameButton->setIcon(QPixmap(iconFile));
    // previousFrameButton->setFlat(true);
    connect(previousFrameButton, SIGNAL(clicked()), runAnimationHandler, SLOT(selectPreviousFrame()));
    previousFrameButton->setEnabled(false);

    captureButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("capture.png"));
    captureButton->setIcon(QPixmap(iconFile));
    // captureButton->setFlat(true);
    // captureButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(captureButton, SIGNAL(clicked()), cameraHandler, SLOT(captureFrame()));

    playButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("play.png"));
    playButton->setIcon(QPixmap(iconFile));
    // playButton->setFlat(true);
    runAnimationHandler->setPlayButton(playButton);
    connect(playButton, SIGNAL(clicked()), runAnimationHandler, SLOT(runAnimation()));

    nextFrameButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("nextframe.png"));
    iconFile.append(QLatin1String("nextexposure.png"));
    nextFrameButton->setIcon(QPixmap(iconFile));
    // nextFrameButton->setFlat(true);
    // nextFrameButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(nextFrameButton, SIGNAL(clicked()), runAnimationHandler, SLOT(selectNextFrame()));
    nextFrameButton->setEnabled(false);

    toEndButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    // iconFile.append(QLatin1String("toend.png"));
    iconFile.append(QLatin1String("lastexposure.png"));
    toEndButton->setIcon(QPixmap(iconFile));
    // toEndButton->setFlat(true);
    // toEndButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(toEndButton, SIGNAL(clicked()), runAnimationHandler, SLOT(selectToEndFrame()));
    toEndButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(2);
    // buttonLayout->addStretch(1);
    buttonLayout->addWidget(framesIcon);
    buttonLayout->addWidget(overlaySlider);
    buttonLayout->addWidget(cameraIcon);
    buttonLayout->addStretch(10);
    buttonLayout->addWidget(toBeginButton);
    buttonLayout->addWidget(previousFrameButton);
    buttonLayout->addWidget(captureButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(nextFrameButton);
    buttonLayout->addWidget(toEndButton);
    buttonLayout->addStretch(10);
    setLayout(buttonLayout);

}


void ToolBar::initialize()
{
    /*
    PreferencesTool *pref = PreferencesTool::get();
    int active = -1;

    int numEncoders = pref->getNumberEncoders();

    encoderTable->setRowCount(numEncoders);

    if (numEncoders > 0 ) {
        QString activeId = pref->getActiveEncoder();
        int idx = 0;

        for (int i = 0; i < numEncoders; ++i) {
            QString id(QString("encoder%1").arg(i));
            QString name;
            QString desc;
            QString start;
            QString stop;
            QString filename;

            if ( pref->getEncoder(id, name, desc, start, stop, filename) ) {
                encoderTable->setItem(i, 0, new QTableWidgetItem(name) );
                encoderTable->setItem(i, 1, new QTableWidgetItem(desc) );
                startEncoderStrings.push_back(start);
                stopEncoderStrings.push_back(stop);
                outputFiles.push_back(filename);

                if ( id.compare(activeId) == 0 )
                    active = idx;

                idx++;
            }
        }
    }

    if (active > -1) {
        encoderTable->setCurrentCell(active, 0);
    }
    */
}

/*
void ToolBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
*/

void ToolBar::apply()
{
    /*
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("encoder%1").arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("encoder%1").arg(i),
                             encoderTable->item(i, 0)->text(),
                             encoderTable->item(i, 1)->text(),
                             startEncoderStrings[i],
                             stopEncoderStrings[i],
                             outputFiles[i]);
        }
    }
    else {
        pref->setNumberEncoders(0);
        pref->setActiveEncoder(QString());
    }
    */
}


void ToolBar::modelSizeChanged(int modelSize)
{
    qDebug("ToolBar::modelSizeChanged --> Start");

    //Not <=1 because it is signed with a meaning for -1.
    if (modelSize == 0 || modelSize == 1) {
        if (previousFrameButton->isEnabled()) {
            previousFrameButton->setEnabled(false);
            nextFrameButton->setEnabled(false);
            playButton->setEnabled(false);
            // pauseButton->setEnabled(false);
            // stopButton->setEnabled(false);
            toEndButton->setEnabled(false);
            toBeginButton->setEnabled(false);
        }
    }
    else {
        if (modelSize >= 2) {
            if ( !previousFrameButton->isEnabled() && !cameraHandler->isCameraRunning() ) {
                previousFrameButton->setEnabled(true);
                nextFrameButton->setEnabled(true);
                playButton->setEnabled(true);
                // pauseButton->setEnabled(true);
                // stopButton->setEnabled(true);
                toEndButton->setEnabled(true);
                toBeginButton->setEnabled(true);
            }
        }
    }

    qDebug("ToolBar::modelSizeChanged --> End");
}

/*
void ToolBar::toBegin()
{
}


void ToolBar::previousFrame()
{
}


void ToolBar::play()
{
}


void ToolBar::capture()
{
}


void ToolBar::nextFrame()
{
}


void ToolBar::toEnd()
{
}
*/

void ToolBar::retranslateStrings()
{
    QString infoText = tr("<h4>Play animation (K, P)</h4>");
    playButton->setToolTip(infoText);
    /*
    infoText = tr("<h4>Previous scene (I)</h4>");
    previousSceneButton->setToolTip(infoText);

    infoText = tr("<h4>Previous scene (I)</h4>");
    previousTakeButton->setToolTip(infoText);
    */
    infoText = tr("<h4>First frame of the take (J, Left)</h4>");
    toBeginButton->setToolTip(infoText);

    infoText = tr("<h4>Previous frame (J, Left)</h4>");
    previousFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Previous frame (J, Left)</h4>");
    previousFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Next frame (L, Right)</h4>");
    nextFrameButton->setToolTip(infoText);

    infoText = tr("<h4>Last frame of the take (L, Right)</h4>");
    toEndButton->setToolTip(infoText);
    /*
    infoText = tr("<h4>Next take (O)</h4>");
    nextTakeButton->setToolTip(infoText);

    infoText = tr("<h4>Next scene (O)</h4>");
    nextSceneButton->setToolTip(infoText);
    */
    infoText =
        tr("<h4>Capture Frame (Space)</h4> "
           "<p>Click on this button to <em>capture</em> a frame from the "
           "camera an put it in the animation</p> <p> This can also be "
           "done by pressing the <b>Space key</b></p>");
    captureButton->setWhatsThis(infoText);
    captureButton->setToolTip(infoText);

}
