/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include "libavencoder.h"

#include "domain/animation/projectserializer.h"
#include "frontends/frontend.h"
#include "technical/grabber/imagegrabber.h"

#include <QtCore/QDir>
#include <QtCore/QString>


/**
 * Prepare start command
 *
 * ffmpeg [[infile options][-i infile]]... {[outfile options] outfile}...
 *
 * see: http://www.ffmpeg.org/ffmpeg-doc.html
 */
LibavEncoder::LibavEncoder(AnimationProject *ap)
{
    animationProject = ap;

    QString encoderCommand;
    QString stopCommand;

#ifdef Q_WS_WIN
    // Windows version

    Frontend *frontend = animationProject->getFrontend();
    encoderCommand.append(frontend->getApplicationDirName());
    encoderCommand.append(QLatin1String("libav/bin/avconv.exe"));
    if (QFile::exists(encoderCommand)) {
        // Set start command
        setEncoderCommand(encoderCommand, true);
    }
    else {
        // The linav encoder is not a part of the qStopMotion installation
        // Search in the windows installation
        encoderCommand.clear();
        encoderCommand.append(QLatin1String("avconv"));
        // Set start command
        setEncoderCommand(encoderCommand, false);
    }

    // Prepare stop command
    stopCommand.append("");
    // Set stop command
    setStopCommand(stopCommand);

#else
    // Linux and Apple OS X version

    encoderCommand.append(QLatin1String("avconv"));

    // Prepare stop command
    stopCommand.append("");

    // Set start and stop command
    setEncoderCommand(encoderCommand, false);
    setStopCommand(stopCommand);

#endif

}


LibavEncoder::~LibavEncoder()
{
}


const QStringList LibavEncoder::getEncoderArguments() const
{
    QStringList      arguments;
    QString          imagePath = animationProject->getNewImagePath();
    QString          imageFiles = QString("");
    QString          outputFile = QString("");

    // ===============================
    // Input options
    // ===============================

    // Input frame rate (default = 25)
    arguments << QLatin1String("-r");
    arguments << QString("%1").arg(animationProject->getVideoFps());

    // ===============================
    // Input files
    // ===============================

    // Input file name
    if (!imagePath.isEmpty()) {
        arguments << QLatin1String("-i");
        // imageFiles.append(QLatin1String("\""));
        imageFiles.append(imagePath);
        imageFiles.append(QLatin1String("/"));
        imageFiles.append(QLatin1String("%6d."));
        switch (animationProject->getImageFormat()) {
        case ImageGrabber::jpegFormat:
            imageFiles.append(PreferencesTool::jpegSuffix);
            break;
        case ImageGrabber::tiffFormat:
            imageFiles.append(PreferencesTool::tiffSuffix);
            break;
        case ImageGrabber::bmpFormat:
            imageFiles.append(PreferencesTool::bmpSuffix);
            break;
        }
        // imageFiles.append(QLatin1String("\""));
        arguments << imageFiles;
    }
    else
    {
        // Error, empty image path
        return QStringList();
    }

    // ===============================
    // Output options
    // ===============================

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
    case VideoEncoder::qvgaSize:
        arguments << QLatin1String("qvga");
        break;
    case VideoEncoder::vgaSize:
        arguments << QLatin1String("vga");
        break;
    case VideoEncoder::svgaSize:
        arguments << QLatin1String("svga");
        break;
    case VideoEncoder::paldSize:
        arguments << QLatin1String("4cif");
        break;
    case VideoEncoder::hdreadySize:
        arguments << QLatin1String("hd720");
        break;
    case VideoEncoder::fullhdSize:
        arguments << QLatin1String("hd1080");
        break;
    default:
        // Default value is VGA output
        arguments << QLatin1String("vga");
        break;
    }

    // Video format
    arguments << QLatin1String("-f");
    switch(animationProject->getVideoFormat()) {
    case VideoEncoder::aviFormat:
        arguments << QLatin1String("avi");
        break;
    case VideoEncoder::mp4Format:
        arguments << QLatin1String("mp4");
        break;
    default:
        // Default value is AVI output
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

    // outputFile.append(QLatin1String("\""));
    outputFile.append(this->getOutputFile());
    // outputFile.append(QLatin1String("\""));
    arguments << outputFile;

    return arguments;
}
