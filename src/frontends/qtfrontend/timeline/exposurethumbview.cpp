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

#include "exposurethumbview.h"

#include "domain/domainfacade.h"

#include <QtCore/QtDebug>
#include <QtCore/QList>
#include <QtCore/QMimeData>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtGui/QDrag>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QPixmap>


ExposureThumbView::ExposureThumbView(TimeLine *line, QWidget *parent, int number, const char *name)
    : ThumbView(line, parent, number, name)
{
    // qDebug("ExposureThumbView::Constructor --> Start");

    setObjectName("ExposureThumbView");
    stringNumber = QString("%1").arg(number + 1);
    textWidth = 5 + stringNumber.length() * 8;
    selected = false;

    // qDebug("ExposureThumbView::Constructor --> End");
}


ExposureThumbView::~ExposureThumbView()
{
}


void ExposureThumbView::mousePressEvent(QMouseEvent * e)
{
    qDebug("ExposureThumbView::mousePressEvent --> Empty");
/*
    if (e->button() == Qt::LeftButton) {
        if (!timeLine->isSelecting()) {
            int selectionFrame = timeLine->getSelectionFrame();
            int activeFrame = timeLine->getFrontend()->getProject()->getActiveExposureIndex();
            int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
            int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;

            // If the user presses inside the selection area this shouldn't trigger
            // setActiveFrame before the mouse button is released. The reason for this is
            // to give the user a chance to drag the items. See mouseReleaseEvent(...)
            if (thumbIndex > highend || thumbIndex < lowend) {
                timeLine->getFrontend()->getProject()->setActiveExposureIndex(thumbIndex);
            }
            dragPos = e->pos();
        } else {
            timeLine->setSelection(thumbIndex);
        }
    }

    qDebug("ExposureThumbView::mousePressEvent --> End");
    */
}


void ExposureThumbView::mouseReleaseEvent(QMouseEvent * e)
{
    qDebug("ExposureThumbView::mouseReleaseEvent --> Start");

    if (e->button() == Qt::LeftButton) {
        if (!timeLine->isSelecting()) {
            int selectionFrame = timeLine->getSelectionFrame();
            int activeFrame = timeLine->getFrontend()->getProject()->getActiveExposureIndex();
            int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
            int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;
            if (thumbIndex <= highend || thumbIndex >= lowend) {
                timeLine->getFrontend()->getProject()->selectExposureToUndo(timeLine->getFrontend()->getProject()->getActiveSceneIndex(),
                                                                            timeLine->getFrontend()->getProject()->getActiveTakeIndex(),
                                                                            thumbIndex);
            }
        }
    }

    qDebug("ExposureThumbView::mouseReleaseEvent --> End");
}


void ExposureThumbView::mouseMoveEvent(QMouseEvent *me)
{
    qDebug("ExposureThumbView::mouseMoveEvent --> Start");

    if (me->buttons() & Qt::LeftButton) {
        int distance = (me->pos() - dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            startDrag();
        }
    }
    QLabel::mouseMoveEvent(me);

    qDebug("ExposureThumbView::mouseMoveEvent --> End");
}


void ExposureThumbView::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug("ExposureThumbView::mouseDoubleClickEvent --> Start");

    // timeLine->showPreferencesMenu();

    qDebug("ExposureThumbView::mouseDoubleClickEvent --> End");
}


void ExposureThumbView::paintEvent(QPaintEvent * paintEvent)
{
    // qDebug() << "ExposureThumbView::paintEvent --> Start (Exposure: " << this->stringNumber << ", Type: " << paintEvent->type() << ")";

    this->QLabel::paintEvent(paintEvent);
    QPainter painter(this);

    if (selected) {
        painter.fillRect(4, 5, textWidth, 14, QBrush(Qt::white));
        painter.setPen(Qt::black);
    } else {
        painter.fillRect(4, 5, textWidth, 14, QBrush(Qt::black));
        painter.setPen(Qt::white);
    }

    painter.drawText(5, 17, stringNumber);

    // qDebug() << "ExposureThumbView::paintEvent --> End (Exposure: " << this->stringNumber << ", Type: " << paintEvent->type() << ")";
}


void ExposureThumbView::startDrag()
{
    qDebug("ExposureThumbView::startDrag --> Start");

    // If the drag ends on a scene this tells the scene that it is frames who are
    // being moved.
    timeLine->setMovingScene(-1);

    QList<QUrl> urls;

    int selectionFrame = timeLine->getSelectionFrame();
    int activeScene = timeLine->getFrontend()->getProject()->getActiveSceneIndex();
    int activeTake = timeLine->getFrontend()->getProject()->getActiveTakeIndex();
    int activeFrame = timeLine->getFrontend()->getProject()->getActiveExposureIndex();
    int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
    int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;
    for (int i = lowend; i <= highend; ++i) {
        Exposure *exposure = timeLine->getFrontend()->getProject()->getExposure(activeScene, activeTake, i);
        urls.append(QUrl::fromLocalFile(exposure->getImagePath()));
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setUrls(urls);
    drag->setMimeData(mimeData);
    drag->setPixmap(*pixmap());

    //Qt::DropAction dropAction = drag->start(Qt::CopyAction | Qt::MoveAction);
    drag->start(Qt::MoveAction);

    qDebug("ExposureThumbView::startDrag --> End");
}


void ExposureThumbView::setThumbIndex(int index)
{
    qDebug("ExposureThumbView::setNumber --> Start");

    this->thumbIndex = index;
    stringNumber = QString("%1").arg(index + 1);
    textWidth = 5 + stringNumber.length() * 8;
    update();

    qDebug("ExposureThumbView::setNumber --> End");
}


void ExposureThumbView::setSelected(bool selected)
{
    qDebug() << "ExposureThumbView::setSelected --> Start (Exposure: " << this->stringNumber << ", Selected: " << selected << ")";

    this->selected = selected;
    if (selected) {
        setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        setLineWidth(5);
        setMidLineWidth(6);
    } else {
        setFrameShape(QFrame::NoFrame);
    }

    qDebug() << "ExposureThumbView::setSelected --> End (Exposure: " << this->stringNumber << ", Selected: " << selected << ")";
}


/**
 *@todo FIXME this doesn't work between applications of this type because of the
 *if-check (event->source() != 0).
 */
void ExposureThumbView::contentsDropped(QDropEvent * event)
{
    qDebug("ExposureThumbView::contentsDropped --> Start");

    if ((event->source() != 0) && (timeLine->getMovingScene() == -1)) {
        qDebug("Moving picture");
        int selectionFrame = timeLine->getSelectionFrame();
        int activeFrame = timeLine->getFrontend()->getProject()->getActiveExposureIndex();
        int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
        int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;
        timeLine->getFrontend()->getProject()->moveFrames(lowend, highend, thumbIndex);
    } else {
        qDebug("Adding picture(s)");
        timeLine->getFrontend()->getProject()->selectExposureToUndo(timeLine->getFrontend()->getProject()->getActiveSceneIndex(),
                                                                    timeLine->getFrontend()->getProject()->getActiveTakeIndex(),
                                                                    thumbIndex);

        if (event->mimeData()->hasUrls()) {
            QStringList fileNames;
            QList<QUrl> urls = event->mimeData()->urls();
            int numFrames = urls.size();
            for (int i = 0; i < numFrames; ++i) {
                fileNames.append(urls[i].toLocalFile());
            }
            QVector<QString> fNames;
            QStringList::Iterator it = fileNames.begin();
            while (it != fileNames.end()) {
                QString fileName = *it;
                fNames.push_back(fileName);
                ++it;
            }
            timeLine->getFrontend()->getProject()->addFrames(fNames);
        }
    }

    qDebug("ExposureThumbView::contentsDropped --> End");
}

