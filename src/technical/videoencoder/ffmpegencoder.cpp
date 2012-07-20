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

#include "ffmpegencoder.h"

#include "domain/animation/projectserializer.h"
#include "frontends/frontend.h"
#include "technical/grabber/imagegrabber.h"

#include <QtCore/QDir>
#include <QtCore/QString>


FfmpegEncoder::FfmpegEncoder(AnimationProject *ap)
{
    animationProject = ap;

    QString encoderCommand;
    QString stopCommand;

#ifdef Q_WS_WIN
    // Windows version

    Frontend *frontend = animationProject->getFrontend();
    encoderCommand.append(frontend->getApplicationDirName());
    encoderCommand.append("ffmpeg/bin/ffmpeg.exe");
    if (QFile::exists(encoderCommand)) {
        // Set start command
        setEncoderCommand(encoderCommand, true);
    }
    else {
        // The ffmpeg encoder is not a part of the qStopMotion installation
        // Search in the windows installation
        encoderCommand.clear();
        encoderCommand.append("ffmpeg");
        // Set start command
        setEncoderCommand(encoderCommand, false);
    }

    // Prepare stop command
    stopCommand.append("");
    // Set stop command
    setStopCommand(stopCommand);

#else
    // Linux and Apple OS X version

    encoderCommand.append("ffmpeg");

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


const QString FfmpegEncoder::getStartCommand() const
{
    QString startCommand;
    QString imagePath = animationProject->getNewImagePath();
    PreferencesTool *pref = animationProject->getFrontend()->getPreferences();

    // Prepare start command
    //
    // ffmpeg [[infile options][-i infile]]... {[outfile options] outfile}...
    //
    // see: http://www.ffmpeg.org/ffmpeg-doc.html
    //

    // Encoder
    startCommand.append("\"");
    startCommand.append(getEncoderCommand());
    startCommand.append("\"");

    // ===============================
    // Input options
    // ===============================

    // Input frame rate (default = 25)
    startCommand.append(QString(" -r %1").arg(animationProject->getFramesPerSecond()));

    // ===============================
    // Input files
    // ===============================

    // Input file name
    if (!imagePath.isEmpty()) {
        startCommand.append(" -i ");
        startCommand.append("\"");
        startCommand.append(imagePath);
        startCommand.append("/");
        startCommand.append("%6d.");
        switch (animationProject->getImageFormat()) {
        case ImageGrabber::jpegFormat:
            startCommand.append(PreferencesTool::jpegSuffix);
            break;
        case ImageGrabber::tiffFormat:
            startCommand.append(PreferencesTool::tiffSuffix);
            break;
        case ImageGrabber::bmpFormat:
            startCommand.append(PreferencesTool::bmpSuffix);
            break;
        }
        startCommand.append("\"");
    }
    else
    {
        // Error, empty image path
        return QString();
    }

    // ===============================
    // Output options
    // ===============================

    // Use target option
    // vcd.mpg
    // startCommand.append(" -target vcd");
    // pal-vcd
    // startCommand.append(" -target pal-vcd");

    // Use individual format options
    //Output frame rate (default = 25)
    startCommand.append(QString(" -r 25"));

    // Video size (default = Input size)
    switch(pref->getBasicPreference("videosize", VideoEncoder::defaultSize)) {
    case VideoEncoder::qvgaSize:
        startCommand.append(" -s qvga");
        break;
    case VideoEncoder::vgaSize:
        startCommand.append(" -s vga");
        break;
    case VideoEncoder::svgaSize:
        startCommand.append(" -s svga");
        break;
    case VideoEncoder::paldSize:
        startCommand.append(" -s 4cif");
        break;
    case VideoEncoder::hdreadySize:
        startCommand.append(" -s hd720");
        break;
    case VideoEncoder::fullhdSize:
        startCommand.append(" -s hd1080");
        break;
    }

    // Video format
    switch(pref->getBasicPreference("videoformat", VideoEncoder::noneFormat)) {
    case VideoEncoder::aviFormat:
        startCommand.append(" -f avi");
        break;
    case VideoEncoder::mp4Format:
        startCommand.append(" -f mp4");
        break;
    }

    // Bit rate in bit/s (default = 200kb/s)
    // startCommand.append(" -b 1800");

    // ===============================
    // Output file
    // ===============================

    startCommand.append(" ");
    startCommand.append("\"");
    startCommand.append(this->getOutputFile());
    startCommand.append("\"");

    return startCommand;
}
