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


QMap<QString, QStringList> VideoEncoderFactory::splitMovieOnParts(DomainFacade *domain,
                                                                  const QString outputFile) const
{
    QMap<QString, QStringList> movies; // key - generated movie's path, values - exposures list
    const int splittingMode = domain->getMovieExportSplittingMode();
    AnimationProject *project = domain->getAnimationProject();
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
            }
            const QFileInfo fileInfo(outputFile);
            const QString baseMovieName = fileInfo.completeBaseName();
            if (splittingMode == DomainFacade::exportAsWholeMovie) {
                movies[baseMovieName].append(exposures);
            } else if (splittingMode == DomainFacade::exportAsSplittedOnScenes) {
                const QString sceneName = scene->getDescription();
                const QString movieName = baseMovieName + " - " + sceneName;
                movies[movieName].append(exposures);
            } else if (splittingMode == DomainFacade::exportAsSplittedOnTakes) {
                const QString sceneName = scene->getDescription();
                const QString takeName = take->getDescription();
                const QString movieName = baseMovieName + " - " + sceneName + " - " + takeName;
                movies[movieName] = exposures;
            } else {
                qCritical() << "MainWindowGUI::splitMovieOnParts unknown export splitting mode" << splittingMode;
            }
        }
    }
    return movies;
}


const QString VideoEncoderFactory::createVideoFile(VideoEncoder *encoder)
{
    qDebug() << "VideoEncoderFactory::createVideoFile --> Start";

    const QString command = encoder->getEncoderCommand();
    const int activeEncoderApplication = frontend->getProject()->getEncoderApplication();

    switch (activeEncoderApplication) {
    case DomainFacade::ffmpegApplication: {
        QList<ExternalCommand> commands;
        const QMap<QString, QStringList> movies = splitMovieOnParts(frontend->getProject(),
                                                                    encoder->getOutputFile());
        const QStringList filePaths = encoder->createInputFilelists(movies, frontend->getTempDirName());
        foreach (const QString &filePath, filePaths) {
            const QFileInfo fileInfo(encoder->getOutputFile());
            const QString outputDirectory = fileInfo.absolutePath();
            const QStringList arguments = encoder->getEncoderArguments(filePath, outputDirectory);
            if (arguments.isEmpty()) {
                qCritical() << "MainWindowGUI::createVideoFile couldn't get arguments for encoder."
                            << "file path:" << filePath;
                return QString();
            }
            ExternalCommand cmd(command, arguments);
            commands << cmd;
        }
        frontend->runExternalCommands(commands);
        break;
    }
    default: {
        const QStringList arguments = encoder->getEncoderArguments(QString(""), QString(""));

        if (!command.isEmpty()) {
            if (frontend->runExternalCommand(command, arguments) == 0) {
                qDebug() << "VideoEncoderFactory::createVideoFile --> End (Done)";
                return encoder->getOutputFile();
            }
        }
        break;
    }
    }

    qDebug() << "VideoEncoderFactory::createVideoFile --> End (Nothing)";
    return QString();
}
