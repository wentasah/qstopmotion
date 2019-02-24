/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include "ffmpegencoder.h"

#include <QDebug>
#include <QDir>
#include <QString>

#include "domain/animation/projectserializer.h"
#include "frontends/frontend.h"

extern bool g_verboseOutput;

namespace {
const QString MOVIENAMEPREFIX = "movie_";
}

/**
 * Prepare start command
 *
 * ffmpeg [[infile options][-i infile]]... {[outfile options] outfile}...
 *
 * see: http://www.ffmpeg.org/ffmpeg-doc.html
 */
FfmpegEncoder::FfmpegEncoder(AnimationProject *ap)
{
    animationProject = ap;

    QString encoderCommand;
    QString stopCommand;

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // Windows version

    Frontend *frontend = animationProject->getFrontend();
    encoderCommand.append(frontend->getApplicationDirName());
    encoderCommand.append(QLatin1String("ffmpeg/bin/ffmpeg.exe"));
    if (QFile::exists(encoderCommand)) {
        // Set start command
        setEncoderCommand(encoderCommand, true);
    }
    else {
        // The ffmpeg encoder is not a part of the qStopMotion installation
        // Search in the windows installation
        encoderCommand.clear();
        encoderCommand.append(QLatin1String("ffmpeg"));
        // Set start command
        setEncoderCommand(encoderCommand, false);
    }

    // Prepare stop command
    stopCommand.append("");
    // Set stop command
    setStopCommand(stopCommand);

#else
    // Linux and Apple OS X version

    encoderCommand.append(QLatin1String("ffmpeg"));

    // Prepare stop command
    stopCommand.append("");

    // Set start and stop command
    setEncoderCommand(encoderCommand, false);
    setStopCommand(stopCommand);

#endif

}


FfmpegEncoder::~FfmpegEncoder()
{
}


const QStringList FfmpegEncoder::getEncoderArguments(const QString inputFilelistPath,
                                                     const QString outputDirectory) const
{
    QStringList arguments;

    // ===============================
    // Input options
    // ===============================
    arguments << inputOptions();

    // ===============================
    // Input files
    // @see https://trac.ffmpeg.org/wiki/Concatenate
    // ===============================
    const QString fileListPath = QDir::toNativeSeparators(inputFilelistPath);
    arguments << QLatin1String("-f");
    arguments << QLatin1String("concat");
    arguments << QLatin1String("-safe");
    arguments << QLatin1String("0");
    arguments << QLatin1String("-i");
    arguments << fileListPath;

    // ===============================
    // Output options
    // ===============================
    const int namePos = inputFilelistPath.indexOf(MOVIENAMEPREFIX);
    if (namePos <= 0) {
        qCritical() << "couldn't extract movie's name from file name:" << inputFilelistPath;
        return QStringList();
    }

    const QString movieName = inputFilelistPath.mid(namePos + MOVIENAMEPREFIX.length());
    const QFileInfo outputFileInfo(outputDirectory, movieName);
    arguments << outputOptions(outputFileInfo.absoluteFilePath());

    return arguments;
}

const QStringList FfmpegEncoder::createInputFilelists(QMap<QString, QStringList> moviesFiles, QString tmpDir) const
{
    QDir tempDir(tmpDir);
    qDebug() << "clear previous filelists from" << tempDir;

    QStringList tmpFiles = tempDir.entryList(QDir::Files);
    foreach (const QString &tmpFile, tmpFiles) {
        if (tmpFile.contains(MOVIENAMEPREFIX)) {
            tempDir.remove(tmpFile);
        }
    }

    qDebug() << "generate list of input files for" << moviesFiles.count()
             << "movies";
    QStringList filelists;
    const QStringList outputFiles = moviesFiles.keys();
    foreach (const QString &movieName, outputFiles) {
        const QString filelistPath = tmpDir + "/" + MOVIENAMEPREFIX + movieName;
        QFile file(filelistPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "couldn't create filelist" << filelistPath;
            return QStringList();
        }

        QTextStream out(&file);
        const QStringList imagesPaths = moviesFiles.value(movieName);
        foreach (const QString &imagePath, imagesPaths) {
            QString line = QString("file '%1'\n").arg(imagePath);
            out << line;
        }
        file.flush();
        file.close();

        filelists << filelistPath;
    }

    return filelists;
}

const QStringList FfmpegEncoder::inputOptions() const
{
    QStringList      arguments;

    // Suppress printing banner (copyright, build options and library versions)
    arguments << QLatin1String("-hide_banner");

    // Verbose output - for debugging only
    if (g_verboseOutput) {
        arguments << QLatin1String("-loglevel");
        arguments << QLatin1String("trace"); // info - default; debug; trace
    }

    // Overwrite output files without asking.
    arguments << QLatin1String("-y");

    // Input frame rate (default = 25)
    arguments << QLatin1String("-r");
    arguments << QString("%1").arg(animationProject->getVideoFps());

    return arguments;
}


const QStringList FfmpegEncoder::outputOptions(const QString &movieName) const
{
    QStringList      arguments;

    // Use target option
    // arguments << QLatin1String("-target"));
    //
    // vcd.mpg
    // arguments << QLatin1String("vcd"));
    // pal-vcd
    // arguments << QLatin1String("pal-vcd"));

    // Use individual format options
    //Output frame rate (default = 25)
    arguments << QLatin1String("-r");
    arguments << QLatin1String("25");

    // Video size (default = Input size)
    arguments << QLatin1String("-s");
    switch(animationProject->getVideoSize()) {
    case DomainFacade::qvgaVideoSize:
        arguments << QLatin1String("qvga");
        break;
    case DomainFacade::vgaVideoSize:
        arguments << QLatin1String("vga");
        break;
    case DomainFacade::svgaVideoSize:
        arguments << QLatin1String("svga");
        break;
    case DomainFacade::paldVideoSize:
        arguments << QLatin1String("4cif");
        break;
    case DomainFacade::hdreadyVideoSize:
        arguments << QLatin1String("hd720");
        break;
    case DomainFacade::fullhdVideoSize:
        arguments << QLatin1String("hd1080");
        break;
    default:
        // Default value is VGA output
        arguments << QLatin1String("vga");
        break;
    }

    // Video format
    arguments << QLatin1String("-f");
    QString fileExtension("");
    switch(animationProject->getVideoFormat()) {
    case DomainFacade::aviFormat:
        fileExtension = QLatin1String(".avi");
        arguments << QLatin1String("avi");
        break;
    case DomainFacade::mp4Format:
        fileExtension = QLatin1String(".mp4");
        arguments << QLatin1String("mp4");
        break;
    default:
        // Default value is AVI output
        fileExtension = QLatin1String(".avi");
        arguments << QLatin1String("avi");
        break;
    }

    // Video quality
    // 1 = excellent quality, 31 worst quality
    arguments << QLatin1String("-qscale:v");
    arguments << QLatin1String("1");

    // Bit rate in bit/s (default = 200kb/s)
    arguments << QLatin1String("-b:v");
    arguments << QLatin1String("1000k");

    // ===============================
    // Output file
    // ===============================
    QString fullName = movieName;
    if (!fullName.endsWith(fileExtension, Qt::CaseInsensitive)) {
        fullName.append(fileExtension);
    }
    arguments << fullName;

    return arguments;
}

