/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include "timeline.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFrame>
#include <QImage>
#include <QImageReader>
#include <QMimeData>
#include <QPixmap>
#include <QResizeEvent>
#include <QScrollBar>
#include <QtGlobal>
#include <QVBoxLayout>

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/timeline/exposurethumbview.h"


static QImage tryReadImage(const QString &filename)
{
    // qDebug() << "TimeLine::tryReadImage --> Start";

    if (filename.isEmpty()) {
        qWarning() << "Couldn't read image: Invalid file name";

        qDebug() << "TimeLine::tryReadImage --> End";
        return QImage();
    }

    QImageReader imageReader(filename);
    const QImage image = imageReader.read();
    if (!image.isNull()) {
        // qDebug() << "TimeLine::tryReadImage --> End");
        return image;
    }

    if (imageReader.error() != QImageReader::UnsupportedFormatError
            && imageReader.error() != QImageReader::InvalidDataError) {
        qWarning() << "TimeLine::tryReadImage --> Couldn't read image:" << imageReader.errorString();
        return image;
    }

    // At this point we most likely failed to read the image because the suffix
    // of the file was different from the actual image format.
    // Loop through all the supported formats and see if we can find a match.
    const QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
    for (int i = 0; i < supportedImageFormats.size(); ++i) {
        QImageReader anotherImageReader(filename);
        anotherImageReader.setFormat(supportedImageFormats.at(i));
        const QImage anotherImage = anotherImageReader.read();
        if (!anotherImage.isNull()) {
            qDebug() << "TimeLine::tryReadImage --> End";
            return anotherImage;
        }
    }

    qWarning() << "TimeLine::tryReadImage --> Couldn't read image:" << imageReader.errorString();
    return image;
}

TimeLine::TimeLine(Frontend* f, QWidget *parent)
    : QScrollArea(parent)
{
    qDebug() << "TimeLine::Constructor --> Start";

    setObjectName("TimeLine");
    frontend = f;
    activeSceneIndex = -10;
    activeTakeIndex = -10;
    activeExposureIndex = -10;
    movingScene = 0;
    selecting = false;
    selectionFrame = -10;
    scrollDirection = 0;

    lowerScrollAreaX = this->x() + FRAME_WIDTH;
    upperScrollAreaX = this->width() - FRAME_WIDTH;

    lowerAccelScrollAreaX = lowerScrollAreaX - (FRAME_WIDTH >> 1);
    upperAccelScrollAreaX = upperScrollAreaX + (FRAME_WIDTH >> 1);

    minScrollAreaX = lowerScrollAreaX - FRAME_WIDTH + 20;
    maxScrollAreaX = upperScrollAreaX + FRAME_WIDTH - 20;

    minScrollAreaY = this->y() + 20;
    maxScrollAreaY = this->y() + FRAME_HEIGHT - 20;

    scrollTimer = new QTimer(this);
    connect(scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));
    scrollBar = horizontalScrollBar();

    mainWidget = new QWidget;
    mainWidget->setMinimumHeight(FRAME_HEIGHT);
    mainWidget->setMaximumHeight(FRAME_HEIGHT);
    mainWidget->setMinimumWidth(FRAME_WIDTH);
    mainWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    setWidget(mainWidget);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    setMaximumHeight(FRAME_HEIGHT + horizontalScrollBar()->height() - 5);
    setMinimumHeight(FRAME_HEIGHT + horizontalScrollBar()->height() - 5);
    setBackgroundRole(QPalette::Dark);
    setAcceptDrops(true);

    qDebug() << "TimeLine::Constructor --> TimeLine is attatched to the model";
    frontend->getView()->attatch(this);

    qDebug() << "TimeLine::Constructor --> End";
}


TimeLine::~TimeLine()
{
}


Frontend* TimeLine::getFrontend()
{
    Q_ASSERT(frontend != NULL);

    return frontend;
}

/*
void TimeLine::setFrontend(Frontend* f)
{
    frontend = f;
}
*/

/**************************************************************************
 * Animation notification functions
 **************************************************************************/

void TimeLine::updateRemoveProject()
{
    qDebug() << "TimeLine::updateRemoveProject --> Start";

    clear();

    qDebug() << "TimeLine::updateRemoveProject --> End";
}


void TimeLine::updateNewProject()
{
    qDebug() << "TimeLine::updateNewProject --> Start (Nothing)";

    // newProject();

    // qDebug() << "TimeLine::updateNewProject --> End");
}


void TimeLine::updateOpenProject()
{
    qDebug() << "TimeLine::updateOpenProject --> Start (Nothing)";

    // openProject();

    // qDebug() << "TimeLine::updateOpenProject --> End");
}


void TimeLine::updatePlaySound(int) {}


void TimeLine::updateMixMode(int) {}


void TimeLine::updateMixCount(int) {}


void TimeLine::updateLiveViewFps(int) {}


void TimeLine::updateVideoFps(int) {}


/**************************************************************************
 * Scene notification functions
 **************************************************************************/

void TimeLine::updateAddScene(int sceneIndex)
{
    qDebug() << "TimeLine::updateAddScene --> Start";

    this->newScene(sceneIndex);

    qDebug() << "TimeLine::updateAddScene --> End";
}

void TimeLine::updateInsertScene(int sceneIndex)
{
    qDebug() << "TimeLine::updateInsertScene --> Start";

    this->newScene(sceneIndex);

    qDebug() << "TimeLine::updateInsertScene --> End";
}


void TimeLine::updateActivateScene()
{
    qDebug() << "TimeLine::updateActivateScene --> Start";

    this->activateScene();

    qDebug() << "TimeLine::updateActivateScene --> End";
}


void TimeLine::updateRemoveScene(int sceneIndex)
{
    qDebug() << "TimeLine::updateRemoveScene --> Start";

    if (sceneIndex > activeSceneIndex) {
        // Nothing to do
        qDebug() << "TimeLine::updateRemoveScene --> End (Nothing)";
        return;
    }
    if (sceneIndex < activeSceneIndex) {
        activeSceneIndex--;

        Q_ASSERT(-1 < activeSceneIndex);

        qDebug() << "TimeLine::updateRemoveScene --> End (Increment)";
        return;
    }

    // Active scene removed
    this->clear();
    this->activateScene();
    this->activateTake();
    this->activateExposure();

    qDebug() << "TimeLine::updateRemoveScene --> End (Delete)";
}


void TimeLine::updateMoveScene(int sceneNumber, int movePosition)
{
    qDebug() << "TimeLine::updateMoveScene --> Start";

    this->moveScene(sceneNumber, movePosition);

    qDebug() << "TimeLine::updateMoveScene --> End";
}


/**************************************************************************
 * Take notification functions
 **************************************************************************/

void TimeLine::updateAddTake(int sceneIndex,
                             int takeIndex)
{
    qDebug() << "TimeLine::updateNewTake --> Start";

    this->newTake(sceneIndex, takeIndex);

    qDebug() << "TimeLine::updateNewTake --> End";
}


void TimeLine::updateInsertTake(int sceneIndex,
                                int takeIndex)
{
    qDebug() << "TimeLine::updateNewTake --> Start";

    this->newTake(sceneIndex, takeIndex);

    qDebug() << "TimeLine::updateNewTake --> End";
}


void TimeLine::updateActivateTake()
{
    qDebug() << "TimeLine::updateActivateTake --> Start";

    this->activateTake();

    qDebug() << "TimeLine::updateActivateTake --> End";
}


void TimeLine::updateRemoveTake(int sceneIndex,
                                int takeIndex)
{
    qDebug() << "TimeLine::updateRemoveTake --> Start";

    if (sceneIndex != activeSceneIndex) {
        // Nothing to do
        qDebug() << "TimeLine::updateRemoveTake --> End (Nothing)";
        return;
    }
    if (takeIndex > activeTakeIndex) {
        // Nothin to do
        qDebug() << "TimeLine::updateRemoveTake --> End (Nothing)";
        return;
    }
    if (takeIndex < activeTakeIndex) {
        activeTakeIndex--;

        Q_ASSERT(-1 < activeTakeIndex);

        qDebug() << "TimeLine::updateRemoveTake --> End (Increment)";
        return;
    }

    // Remove actual take
    this->clear();
    this->activateScene();
    this->activateTake();
    this->activateExposure();

    qDebug() << "TimeLine::updateRemoveTake --> End (Delete)";
}


/**************************************************************************
 * Exposure notification functions
 **************************************************************************/

void TimeLine::updateAddExposure(int sceneIndex,
                                 int takeIndex,
                                 int exposureIndex)
{
    // qDebug() << "TimeLine::updateAddExposure --> Start";

    this->newExposure(sceneIndex, takeIndex, exposureIndex);

    // qDebug() << "TimeLine::updateAddExposure --> End";
}


void TimeLine::updateInsertExposure(int sceneIndex,
                                    int takeIndex,
                                    int exposureIndex)
{
    qDebug() << "TimeLine::updateInsertExposure --> Start";

    this->newExposure(sceneIndex, takeIndex, exposureIndex);

    qDebug() << "TimeLine::updateInsertExposure --> End";
}


void TimeLine::updateRemoveExposure(int sceneIndex,
                                    int takeIndex,
                                    int exposureIndex)
{
    qDebug() << "TimeLine::updateRemoveExposure --> Start";

    removeExposure(sceneIndex, takeIndex, exposureIndex);

    qDebug() << "TimeLine::updateRemoveExposure --> End";
}


void TimeLine::updateMoveExposures(int fromFrame, int toFrame, int movePosition)
{
    qDebug() << "TimeLine::updateMoveExposures --> Start";

    moveExposures(fromFrame, toFrame, movePosition);

    qDebug() << "TimeLine::updateMoveExposures --> End";
}


void TimeLine::updateActivateExposure()
{
    qDebug() << "TimeLine::updateActivateExposure --> Start";

    activateExposure();

    qDebug() << "TimeLine::updateActivateExposure --> End";
}


void TimeLine::updateModifyExposure(int modSceneIndex,
                                    int modTakeIndex,
                                    int modExposureIndex)
{
    qDebug() << "TimeLine::updateModifyExposure --> Start";

    modifyExposure(modSceneIndex, modTakeIndex, modExposureIndex);

    qDebug() << "TimeLine::updateModifyExposure --> End";
}


/**************************************************************************
 * Other functions
 **************************************************************************/

void TimeLine::moveThumbView(int fromPosition, int toPosition)
{
    qDebug() << "TimeLine::moveThumbView --> Start";

    ThumbView *f = thumbViews[fromPosition];
    f->setThumbIndex(toPosition - 1);
    f->setSelected(false);
    thumbViews.erase(thumbViews.begin() + fromPosition);
    thumbViews.insert(thumbViews.begin() + toPosition, f);

    qDebug() << "TimeLine::moveThumbView --> End";
}


void TimeLine::setSelecting(bool selecting)
{
    qDebug() << "TimeLine::setSelecting --> Start";

    this->selecting = selecting;

    qDebug() << "TimeLine::setSelecting --> End";
}


bool TimeLine::isSelecting() const
{
    return selecting;
}


int TimeLine::getSelectionFrame() const
{
    return selectionFrame;
}


void TimeLine::frameSoundsChanged()
{
    qDebug() << "TimeLine::frameSoundsChanged --> Start";

    int activeFrame = frontend->getProject()->getActiveExposureIndex();
    int activeThumb = activeFrame + frontend->getProject()->getActiveSceneIndex() + 1;

    Exposure *exposure = frontend->getProject()->getActiveExposure();
    if (!exposure->isEmpty()) {
//        if (exposure->getNumberOfSounds() > 0) {
            thumbViews[activeThumb]->setHasSounds(true);
//        } else {
//            thumbViews[activeThumb]->setHasSounds(false);
//        }
    }

    qDebug() << "TimeLine::frameSoundsChanged --> End";
}


/**************************************************************************
 * Private animation functions
 **************************************************************************/

/**************************************************************************
 * Private scene functions
 **************************************************************************/

void TimeLine::newScene(int sceneIndex)
{
    qDebug() << "TimeLine::newScene --> Start";

    qDebug() << "TimeLine::newScene --> Adding new scene thumb to timeline";

    if (sceneIndex > activeSceneIndex) {
        // New Scene is inserted after the active scene --> Nothing to do
        qDebug() << "TimeLine::newScene --> End (Nothing)";
        return;
    }

    activeSceneIndex++;

    qDebug() << "TimeLine::newScene --> End";
}


void TimeLine::moveScene(int sceneNumber, int movePosition)
{
    qDebug() << "TimeLine::moveScene --> Start";

    if (thumbViews.size() <= 0) {
        qDebug() << "TimeLine::moveScene --> End (nothing to do)";
        return;
    }
    if (movePosition < sceneNumber) {
        for (int i = movePosition; i < sceneNumber; ++i) {
            thumbViews[i]->move(thumbViews[i]->x() + (FRAME_WIDTH + SPACE), 0);
            thumbViews[i]->setThumbIndex(thumbViews[i]->getThumbIndex() + 1);
        }
    } else {
        for (int i = sceneNumber + 1; i <= movePosition; ++i) {
            thumbViews[i]->move(thumbViews[i]->x() - (FRAME_WIDTH + SPACE), 0);
            thumbViews[i]->setThumbIndex(thumbViews[i]->getThumbIndex() - 1);
        }
    }

    ThumbView *const tv = thumbViews[sceneNumber];
    tv->move(tv->x() - (FRAME_WIDTH + SPACE) *(sceneNumber - movePosition), 0);
    tv->setThumbIndex(movePosition);

    thumbViews.erase(thumbViews.begin() + sceneNumber);
    thumbViews.insert(thumbViews.begin() + movePosition, tv);

    qDebug() << "TimeLine::moveScene --> End";
}

void TimeLine::activateScene()
{
    qDebug() << "TimeLine::activateScene --> Start";

    int newActiveScene = frontend->getProject()->getActiveSceneIndex();

    if (newActiveScene == activeSceneIndex) {
        // Nothing to do
        qDebug() << "TimeLine::activateScene --> End (Nothing)";
        return;
    }

    if (0 <= activeTakeIndex) {
        this->removeAllExposures();
        activeTakeIndex = -1;
    }
    activeSceneIndex = newActiveScene;

    qDebug() << "TimeLine::activateScene --> End";
}


/**************************************************************************
 * Private take functions
 **************************************************************************/

void TimeLine::newTake(int sceneIndex,
                       int takeIndex)
{
    qDebug() << "TimeLine::newTake --> Start";

    if (sceneIndex != activeSceneIndex) {
        // Not the active scene --> Nothing to do
        qDebug() << "TimeLine::newTake --> End (Nothing)";
        return;
    }
    if (takeIndex > activeTakeIndex) {
        // New take is inserted after the activ take --> Nothing to do
        qDebug() << "TimeLine::newTake --> End (Nothing)";
        return;
    }

    activeTakeIndex++;

    qDebug() << "TimeLine::newTake --> End";
}


void TimeLine::activateTake()
{
    qDebug() << "TimeLine::activateTake --> Start";

    if (activeSceneIndex < 0) {
        // Nothing to do
        return;
    }

    int newActiveTake = frontend->getProject()->getActiveTakeIndex();

    if (newActiveTake == activeTakeIndex) {
        // Nothing to do
        return;
    }

    if (activeTakeIndex >= 0) {
        this->removeAllExposures();
        // thumbViews[activeTakeIndex]->setOpened(false);
    }

    activeTakeIndex = newActiveTake;

    if (activeTakeIndex >= 0) {
        // thumbViews[activeTakeIndex]->setOpened(true);
        Take *take = frontend->getProject()->getTake(activeSceneIndex, activeTakeIndex);
        if (take->getExposureSize() > 0) {
            QVector<Exposure*> allExposures;
            take->getExposures(allExposures);
            this->addExposures(allExposures, 0);
            // activateExposure();
        }
    }

    ensureVisible((FRAME_WIDTH + SPACE) * thumbViews.size() + FRAME_WIDTH, FRAME_HEIGHT);

    qDebug() << "TimeLine::activateTake --> End";
}


/**************************************************************************
 * Private exposure functions
 **************************************************************************/

void TimeLine::newExposure(int sceneIndex,
                           int takeIndex,
                           int newExposureIndex)
{
    // qDebug() << "TimeLine::newExposure --> Start";

    Q_ASSERT(newExposureIndex >= 0);

    if (activeTakeIndex < 0) {
        // No active take in the timeline
        // qDebug() << "TimeLine::newExposure --> End (nothing)";
        return;
    }

    Q_ASSERT(sceneIndex == activeSceneIndex);    // Response to new sceneIndex not implemented
    Q_ASSERT(takeIndex == activeTakeIndex);      // Response to new takeIndex not implemented

    ExposureThumbView *thumb = 0;

    Exposure *exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, newExposureIndex);
    thumb = new ExposureThumbView(this, this, newExposureIndex);
    thumb->setMinimumSize(FRAME_WIDTH, FRAME_HEIGHT);
    thumb->setMaximumSize(FRAME_WIDTH, FRAME_HEIGHT);
    thumb->setScaledContents(true);
    thumb->setPixmap(QPixmap::fromImage(tryReadImage(exposure->getImagePath()).scaled(FRAME_WIDTH, FRAME_HEIGHT)));
    thumb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    thumb->setParent(mainWidget);
    thumb->move((FRAME_WIDTH + SPACE) *newExposureIndex, 0);
    thumb->show();

    // Sets the note icon on the respective frames.
    // if (exposure->getNumberOfSounds() > 0) {
         thumb->setHasSounds(true);
    // }

    thumbViews.insert(newExposureIndex, thumb);

    // Move all tumbs after the new tumb
    for (int thumbIndex = newExposureIndex + 1; thumbIndex < thumbViews.count(); thumbIndex++) {
        thumb = (ExposureThumbView*)thumbViews[thumbIndex];
        thumb->setThumbIndex(thumbIndex);
        thumb->move((FRAME_WIDTH + SPACE) *thumbIndex, 0);
        thumb->show();
    }

    mainWidget->resize((FRAME_WIDTH + SPACE) * thumbViews.size() - SPACE, FRAME_HEIGHT);

    if (newExposureIndex <= activeExposureIndex) {
        activeExposureIndex++;
        selectionFrame++;
    }

    // qDebug() << "TimeLine::newExposure --> End";
}


void TimeLine::addExposures(const QVector<Exposure*>& exposures, int index)
{
    qDebug() << "TimeLine::addExposures --> Start";

    int size = thumbViews.size();
    int exposureSize = exposures.size();
    int from = index;
    int to = size;
    int moveDistance = exposureSize - 1;

    if (exposureSize)
    // Move the frames behind the place we are inserting the new ones.
    for (int i = from; i < size; ++i) {
        qDebug() << "TimeLine::addExposures --> move frame";

        thumbViews[i]->move(thumbViews[i]->x() + (FRAME_WIDTH + SPACE) * exposureSize, 0);
        if (i < to) {
            thumbViews[i]->setThumbIndex(i + moveDistance);
        }
    }

    ThumbView *thumb = 0;
    bool operationCanceled = false;
    int exposureIndex = 0;

    frontend->showProgress(tr("Load images to time line"), exposureSize);

    // Adds the new frames to the timeline
    for (; exposureIndex < exposureSize; ++exposureIndex) {
        qDebug() << "TimeLine::addExposures --> add frame";

        Exposure *exposure = exposures[exposureIndex];
        thumb = new ExposureThumbView(this, this, index + exposureIndex);
        thumb->setMinimumSize(FRAME_WIDTH, FRAME_HEIGHT);
        thumb->setMaximumSize(FRAME_WIDTH, FRAME_HEIGHT);
        thumb->setScaledContents(true);
        thumb->setPixmap(QPixmap::fromImage(tryReadImage(exposure->getImagePath()).scaled(FRAME_WIDTH, FRAME_HEIGHT)));
        thumb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        thumb->setParent(mainWidget);
        thumb->move((FRAME_WIDTH + SPACE) *(index + exposureIndex), 0);
        thumb->show();

        // Sets the note icon on the respective frames.
        // if (exposure->getNumberOfSounds() > 0) {
             thumb->setHasSounds(true);
        // }

        thumbViews.insert(index + exposureIndex, thumb);

        frontend->updateProgress(exposureIndex);
        if ((exposureIndex % 10) == 0) {
            frontend->processEvents();
        }

        if (frontend->isOperationAborted()) {
            operationCanceled = true;
            break;
        }
    }

    frontend->hideProgress();

    if (operationCanceled) {
        for (int j = index + exposureIndex, k = index; j < exposureIndex + size; ++j, ++k) {
            qDebug() << "TimeLine::addExposures --> move frame back";

            thumbViews[j]->move(thumbViews[j]->x() - exposureSize * (FRAME_WIDTH + SPACE), 0);
            thumbViews[j]->setThumbIndex(k);
        }

        for (int j = index; j <= index + exposureIndex; ++j) {
            qDebug() << "TimeLine::addExposures --> delete frame";

            delete thumbViews[index];
        }
        thumbViews.erase(thumbViews.begin() + index, thumbViews.begin() + index + exposureIndex);
    } else {
        mainWidget->resize((FRAME_WIDTH + SPACE) * thumbViews.size() - SPACE, FRAME_HEIGHT);
    }

    qDebug() << "TimeLine::addExposures --> End";
}


void TimeLine::removeExposure(int sceneIndex,
                              int takeIndex,
                              int exposureIndex)
{
    qDebug() << "TimeLine::removeExposure --> Start";

    if (this->activeSceneIndex != sceneIndex) {
        qDebug() << "TimeLine::removeExposure --> End (Nothing)";
        return;
    }

    // The timeline is in the right scene
    if (this->activeTakeIndex != takeIndex) {
        qDebug() << "TimeLine::removeExposure --> End (Nothing)";
        return;
    }

    // The timeline is in the right take

    int fromExposure = exposureIndex + 1;

    int numExposures = thumbViews.size();

    // The exposure to be deleted are between other exposures
    if (fromExposure < numExposures) {

        // Move all exposures behind the deleted exposure forward.
        // int stop = numExposures - frontend->getProject()->getSceneSize() + 1;
        for (int k = fromExposure; k < numExposures; ++k) {
            thumbViews[k]->move(thumbViews[k]->x() - (FRAME_WIDTH + SPACE), 0);
            thumbViews[k]->setThumbIndex(k - 1);
        }
    }

    delete thumbViews[exposureIndex];
    thumbViews.remove(exposureIndex);
    mainWidget->resize((FRAME_WIDTH + SPACE) * thumbViews.size(), FRAME_HEIGHT);

    // int newExposureIndex = activeExposureIndex;
    if (exposureIndex < activeExposureIndex) {
        activeExposureIndex--;
    }
    else {
        if (activeExposureIndex == thumbViews.size()) {
            activeExposureIndex--;
        }
    }

    if (-1 < activeExposureIndex) {
        thumbViews[activeExposureIndex]->setSelected(true);
    }

    qDebug() << "TimeLine::removeExposure --> End";
}


void TimeLine::removeAllExposures()
{
    qDebug() << "TimeLine::removeAllExposures --> Start";

    int tumbSize = thumbViews.size();

    for (int tumbIndex = tumbSize-1; tumbIndex >= 0; tumbIndex--) {
        delete thumbViews[tumbIndex];
        thumbViews.remove(tumbIndex);
    }

    mainWidget->resize((FRAME_WIDTH + SPACE) * thumbViews.size(), FRAME_HEIGHT);

    activeExposureIndex = -1;

    qDebug() << "TimeLine::removeAllExposures --> End";
}


void TimeLine::moveExposures(int fromFrame, int toFrame, int movePosition)
{
    qDebug() << "TimeLine::moveExposures --> Start";

    fromFrame += 1;
    toFrame += 1;
    movePosition += 1;

    if (movePosition < fromFrame) {
        for (int i = movePosition; i < fromFrame; ++i) {
            thumbViews[i]->move(thumbViews[i]->x() + (FRAME_WIDTH + SPACE) *(toFrame - fromFrame + 1), 0);
            thumbViews[i]->setThumbIndex(i + (toFrame - fromFrame));
        }

        for (int j = fromFrame; j <= toFrame; ++j) {
            thumbViews[j]->move(thumbViews[j]->x() - (FRAME_WIDTH + SPACE) *(fromFrame - movePosition), 0);
            moveThumbView(j, j - (fromFrame - movePosition));
        }
    } else if (movePosition > fromFrame) {
        for (int i = toFrame + 1; i <= movePosition; ++i) {
            thumbViews[i]->move(thumbViews[i]->x() - (FRAME_WIDTH + SPACE) *(toFrame - fromFrame + 1), 0);
            thumbViews[i]->setThumbIndex(i - (toFrame - fromFrame + 2));
        }

        for (int j = fromFrame, k = toFrame; j <= toFrame; ++j, --k) {
            thumbViews[k]->move(thumbViews[k]->x() + (FRAME_WIDTH + SPACE) *(movePosition - toFrame), 0);
            moveThumbView(k, k + (movePosition - toFrame));
        }
    }

    qDebug() << "TimeLine::moveExposures --> End";
}


void TimeLine::activateExposure()
{
    qDebug() << "TimeLine::activateExposure --> Start";

    if (activeSceneIndex < 0) {
        // Nothing to do
        return;
    }
    if (activeTakeIndex < 0) {
        // Nothing to do
        return;
    }
    int newActiveExposure = frontend->getProject()->getActiveExposureIndex();
    if (newActiveExposure == activeExposureIndex) {
        // Nothing to do
        return;
    }

    if (activeExposureIndex < thumbViews.size()) {
        if (activeExposureIndex >= 0) {
            thumbViews[activeExposureIndex]->setSelected(false);
        }
    }
    activeExposureIndex = newActiveExposure;
    if (activeExposureIndex > -1) {
        thumbViews[activeExposureIndex]->setSelected(true);
        int activeExposureX = activeExposureIndex * (FRAME_WIDTH + SPACE);
        if (this->width() < (activeExposureX + SPACE + FRAME_WIDTH)) {
            activeExposureX += SPACE + FRAME_WIDTH;
        }
        ensureVisible(activeExposureX, FRAME_HEIGHT);
    }

    selectionFrame = activeExposureIndex;
    this->selecting = false;

    qDebug() << "TimeLine::activateExposure --> End";
}


void TimeLine::modifyExposure(int modSceneIndex,
                              int modTakeIndex,
                              int modExposureIndex)
{
    qDebug() << "TimeLine::modifyExposure --> Start";

    if (activeSceneIndex != modSceneIndex) {
        // The scene of the modifyed exposure is not displayed in the frame view
        qDebug() << "TimeLine::modifyExposure --> End (Nothing)";
        return;
    }

    if (activeTakeIndex != modTakeIndex) {
        // The take of the modifyed exposure is not displayed in the frame view
        qDebug() << "TimeLine::modifyExposure --> End (Nothing)";
        return;
    }

    Exposure *exposure = frontend->getProject()->getExposure(activeSceneIndex, activeTakeIndex, modExposureIndex);
    thumbViews[modExposureIndex]->setPixmap(QPixmap::fromImage(tryReadImage(exposure->getImagePath()).scaled(FRAME_WIDTH, FRAME_HEIGHT)));

    qDebug() << "TimeLine::modifyExposure --> End";
}


/**************************************************************************
 * Other private functions
 **************************************************************************/

int TimeLine::getMovingScene() const
{
    return movingScene;
}


void TimeLine::setMovingScene(int movingScene)
{
    this->movingScene = movingScene;
}


void TimeLine::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "TimeLine::dragEnterEvent --> Start";

    if (event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }

    qDebug() << "TimeLine::dragEnterEvent --> End";
}


void TimeLine::dropEvent(QDropEvent *event)
{
    qDebug() << "TimeLine::dropEvent --> Start";

    scrollTimer->stop();
    scrollDirection = 0;

    int index = (event->pos().x() + -mainWidget->pos().x()) / (FRAME_WIDTH + SPACE);
    if (index < static_cast<int>(thumbViews.size())) {
        thumbViews[index]->contentsDropped(event);
    }

    qDebug() << "TimeLine::dropEvent --> End";
}


void TimeLine::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "TimeLine::dragMoveEvent --> Start";

    int dragPosX = event->pos().x();
    int dragPosY = event->pos().y();

    if (dragPosX < lowerScrollAreaX || dragPosX > upperScrollAreaX) {
        if (!scrollTimer->isActive()) {
            scrollDirection = dragPosX < lowerScrollAreaX ? -1 : 1;
            scrollTimer->start(50);
        }
        // Increase speed
        else if (dragPosX < lowerAccelScrollAreaX || dragPosX > upperAccelScrollAreaX) {
            scrollTimer->setInterval(25);
        }
        // Decrease speed
        else {
            scrollTimer->setInterval(50);
        }
    } else {
        scrollTimer->stop();
        scrollDirection = 0;
    }

    // Stop scrolling if we're dragging outside timeline
    bool outsideHorizontal = dragPosX < minScrollAreaX || dragPosX > maxScrollAreaX;
    bool outsideVertical = dragPosY < minScrollAreaY || dragPosY > maxScrollAreaY;
    if (outsideHorizontal || outsideVertical) {
        scrollTimer->stop();
        scrollDirection = 0;
    }

    qDebug() << "TimeLine::dragMoveEvent --> End";
}


void TimeLine::resizeEvent(QResizeEvent *event)
{
    qDebug() << "TimeLine::resizeEvent --> Start";

    lowerScrollAreaX = this->x() + FRAME_WIDTH;
    upperScrollAreaX = this->width() - FRAME_WIDTH;

    lowerAccelScrollAreaX = lowerScrollAreaX - (FRAME_WIDTH >> 1);
    upperAccelScrollAreaX = upperScrollAreaX + (FRAME_WIDTH >> 1);

    minScrollAreaX = lowerScrollAreaX - FRAME_WIDTH + 20;
    maxScrollAreaX = upperScrollAreaX + FRAME_WIDTH - 20;

    minScrollAreaY = this->y() + 20;
    maxScrollAreaY = this->y() + FRAME_HEIGHT - 20;

    QScrollArea::resizeEvent(event);

    qDebug() << "TimeLine::resizeEvent --> End";
}


void TimeLine::scroll()
{
    if (scrollDirection == -1) {
        scrollBar->setSliderPosition(scrollBar->sliderPosition() - 15);
    } else if (scrollDirection == 1) {
        scrollBar->setSliderPosition(scrollBar->sliderPosition() + 15);
    }
}


int TimeLine::getFrameWidth() const
{
    return FRAME_WIDTH;
}


int TimeLine::getFrameHeight() const
{
    return FRAME_HEIGHT;
}


int TimeLine::getSpace() const
{
    return SPACE;
}


void TimeLine::clear()
{
    qDebug() << "TimeLine::clear --> Start";

    clearTake();

    qDebug() << "TimeLine::clear --> End";
}


void TimeLine::clearTake()
{
    qDebug() << "TimeLine::clearTake --> Start";

    int size = thumbViews.size();
    for (int i = 0; i < size; ++i) {
        delete thumbViews[i];
    }
    thumbViews.clear();

    activeExposureIndex = -1;
    activeTakeIndex = -1;
    activeSceneIndex = -1;

    qDebug() << "TimeLine::clearTake --> End";
}
