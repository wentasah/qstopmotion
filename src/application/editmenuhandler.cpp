/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#include "editmenuhandler.h"

// #include "domain/domainfacade.h"
#include "domain/animation/exposure.h"
#include "frontends/frontend.h"

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QDrag>


EditMenuHandler::EditMenuHandler(Frontend *f,
                                 QObject *parent,
                                 TimeLine *line,
                                 const char *name)
    : QObject(parent), timeLine(line)
{
    frontend = f;
    gotoMenu = 0;
    setObjectName(name);
}


void EditMenuHandler::setGotoMenu(QWidget * gotoMenu)
{
    this->gotoMenu = gotoMenu;
}


// TODO: New implementation of goto frame
void EditMenuHandler::gotoFrame(int frameNumber)
{
    /*
    frontend->getProject()->setActiveExposureIndex(frameNumber);
    this->closeGotoMenu();
    */
}


void EditMenuHandler::closeGotoMenu()
{
    gotoMenu->hide();
}


// TODO: New implementation of copy necessary
void EditMenuHandler::copy()
{
    /*
    QList<QUrl> urls;

    // TODO: return the scene, take and frame from the frame bar!!!!
    int selectionFrame = timeLine->getSelectionFrame();
    int activeFrame = 1; // frontend->getProject()->getActiveExposureIndex();
    int highend = (selectionFrame > activeFrame) ? selectionFrame : activeFrame;
    int lowend = (selectionFrame < activeFrame) ? selectionFrame : activeFrame;

    for (int i = lowend; i <= highend; ++i) {
        // TODO: Get the filename from the frame bar
        Exposure* exposure = frontend->getProject()->getExposure(1, 1, i);
        urls.append(QUrl::fromLocalFile(exposure->getImagePath()));
    }

    //QDrag *drag = new QDrag((MainWindowGUI*)this->parent());
    QMimeData *mimeData = new QMimeData;

    mimeData->setUrls(urls);
    //drag->setMimeData(mimeData);

    //drag->start(Qt::MoveAction);
    QApplication::clipboard()->setMimeData(mimeData);
    */
}

// TODO: New implementation of past necessary
void EditMenuHandler::paste()
{
    /*
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    if (mimeData->hasUrls()) {
        QStringList fileNames;
        QList<QUrl> urls = mimeData->urls();
        int numFrames = urls.size();
        for (int i = 0; i < numFrames; ++i) {
            fileNames.append(urls[i].toLocalFile());
        }
        emit addFrames(fileNames);
    }
    */
}


void EditMenuHandler::undo()
{
    // TODO: Implement undo task!
}


void EditMenuHandler::redo()
{
    // TODO: Implement redo task!
}
