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

#include "videoencoderfactory.h"

#include <QDebug>
#include <QFileInfo>
#include <QTextCodec>


VideoEncoderFactory::VideoEncoderFactory(Frontend *f)
{
    frontend = f;
}


VideoEncoderFactory::~VideoEncoderFactory()
{
    frontend = NULL;
}


const QString VideoEncoderFactory::createVideoFile(VideoEncoder *encoder)
{
    qDebug() << "VideoEncoderFactory::createVideoFile --> Start";

    const int activeEncoderApplication = frontend->getProject()->getEncoderApplication();
    if (activeEncoderApplication == DomainFacade::ffmpegApplication) {
        QMap<QString, QStringList> movies; // key - generated movie's path, values - exposures list
        const int splittingMode = frontend->getProject()->getMovieExportSplittingMode();
        AnimationProject *project = frontend->getProject()->getAnimationProject();
        const int scenesCount = project->getSceneSize();
        for (int s = 0; s < scenesCount; s++) {
            Scene *scene = project->getScene(s);
            const unsigned int takesCount = static_cast<unsigned int>(scene->getTakeSize());
            for (unsigned int t = 0; t < takesCount; t++) {
                Take *take = scene->getTake(t);
                const unsigned int exposuresCount = static_cast<unsigned int>(take->getExposureSize());
                QStringList exposures;
                for (unsigned int e = 0; e < exposuresCount; e++) {
                    const Exposure *exposure = take->getExposure(e);
                    const QString imagePath = exposure->getImagePath();
                    exposures.append(imagePath);
                    qDebug() << imagePath;
                }
                if (splittingMode == DomainFacade::exportAsWholeMovie) {
                    const QFileInfo fileInfo(encoder->getOutputFile());
                    const QString wholeMovieName = fileInfo.completeBaseName();
                    movies[wholeMovieName].append(exposures);
                } else if (splittingMode == DomainFacade::exportAsSplittedOnScenes) {
                    const QString sceneName = scene->getDescription();
                    movies[sceneName].append(exposures);
                } else if (splittingMode == DomainFacade::exportAsSplittedOnTakes) {
                    const QString sceneName = scene->getDescription();
                    const QString takeName = take->getDescription();
                    const QString moviePath = sceneName + " - " + takeName;
                    movies[moviePath] = exposures;
                } else {
                    qCritical() << "MainWindowGUI::exportToVideo unknown export splitting mode" << splittingMode;
                }
            }
        }

        const QString command = encoder->getEncoderCommand();
        QList<ExternalCommand> commands;
        const QStringList filePaths = encoder->createInputFilelists(movies, frontend->getTempDirName());
        foreach (const QString &filePath, filePaths) {
            const QStringList arguments = encoder->getEncoderArguments(filePath);
            if (arguments.isEmpty()) {
                qCritical() << "MainWindowGUI::exportToVideo couldn't get arguments for encoder."
                            << "file path:" << filePath;
            }
            ExternalCommand cmd(command, arguments);
            commands << cmd;
        }
        frontend->runExternalCommands(commands);
        qDebug() << "VideoEncoderFactory::createVideoFile --> End (Nothing)";
        return QString();
    }

    QString command = encoder->getEncoderCommand();
    QStringList arguments = encoder->getEncoderArguments(QString(""));

    qDebug() << "VideoEncoderFactory::createVideoFile --> Command:";
    qDebug() << command << arguments;

    if (!command.isEmpty()) {
        if (frontend->runExternalCommand(command, arguments) == 0) {
            qDebug() << "VideoEncoderFactory::createVideoFile --> End (Done)";
            return encoder->getOutputFile();
        }
    }

    qDebug() << "VideoEncoderFactory::createVideoFile --> End (Nothing)";
    return QString();
}
