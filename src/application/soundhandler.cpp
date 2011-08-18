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

#include "soundhandler.h"

#include "domain/domainfacade.h"
#include "domain/animation/scene.h"

#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>


SoundHandler::SoundHandler(Frontend *f,
                           QObject *parent,
                           const QString &homeDir,
                           const QString &name)
    : QObject(parent), homeDir(homeDir)
{
    frontend = f;
    soundsList = NULL;
    setObjectName(name);
}


void SoundHandler::setSoundsList(QListWidget *soundsList)
{
    this->soundsList = soundsList;
}


void SoundHandler::addSound()
{
    QString openFile = QFileDialog::getOpenFileName(0, tr("Choose sound file"), QString(homeDir), tr("Sounds (*.ogg)"));
    if (!openFile.isNull()) {
        DomainFacade *facade = frontend->getProject();
        bool ok = false;
        int ret = 0;
        QString text = QInputDialog::getText(0, tr("Sound name"), tr("Enter the name of the sound:"),
                                             QLineEdit::Normal, QString::null, &ok);
        if (ok && !text.isEmpty()) {
            ret = facade->addSoundToScene(facade->getActiveSceneIndex(), openFile, text);
        }
        else {
            ret = facade->addSoundToScene(facade->getActiveSceneIndex(), openFile, QString());
        }

        if (ret == 0) {
            Scene *scene = facade->getScene(facade->getActiveSceneIndex());
            if (!scene->isEmpty()) {
                soundsList->insertItem(soundsList->count(),
                                       new QListWidgetItem(scene->getSoundName(soundsList->count())));
                soundsList->item(soundsList->currentRow())->setText(text);
                emit soundsChanged();
            }
        }
    }
}


void SoundHandler::removeSound()
{
    int index = soundsList->currentRow();
    if (index >= 0) {
        frontend->getProject()->removeSoundFromScene(frontend->getProject()->getActiveSceneIndex(), index);
        QListWidgetItem *qlwi = soundsList->takeItem(index);
        delete qlwi;
        emit soundsChanged();
    }
}


void SoundHandler::setSoundName()
{
}
