/******************************************************************************
 *  Copyright (C) 2005-2010 by                                                *
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

#include <QtCore/QDir>
#include <QtCore/QString>


FfmpegEncoder::FfmpegEncoder(AnimationProject *ap)
{
    animationProject = ap;

    QString encoderCommand;
    QString stopCommand;

    encoderCommand.append("ffmpeg");

    // Prepare stop command
    stopCommand.append("");

    // Set start and stop command
    setEncoderCommand(encoderCommand);
    setStopCommand(stopCommand);
}


FfmpegEncoder::~FfmpegEncoder()
{
}


const QString FfmpegEncoder::getStartCommand() const
{
    QString startCommand;
    ProjectSerializer *serializer = animationProject->getProjectSerializer();
    QString imagePath = serializer->getImagePath();

    // Prepare start command
    //
    // ffmpeg [[infile options][-i infile]]... {[outfile options] outfile}...
    //
    // see: http://www.ffmpeg.org/ffmpeg-doc.html
    //

    // Encoder
    startCommand.append(getEncoderCommand());

    // Use target option
    // vcd.mpg
    // startCommand.append(" -target vcd");
    // pal-vcd
    // startCommand.append(" -target pal-vcd");

    // Use individual format options
    // Frame rate (default = 25)
    startCommand.append(" -r 10");

    // Bit rate in bit/s (default = 200kb/s)
    startCommand.append(" -b 1800");

    // Input file name
    if (!imagePath.isEmpty()) {
        startCommand.append(" -i");
        startCommand.append(" \"");
        startCommand.append(imagePath);
        startCommand.append("/");
        startCommand.append("%6d.jpg");
        startCommand.append("\"");
    }
    else
    {
        // Error, empty image path
        return QString();
    }

    startCommand.append(" \"");
    startCommand.append(this->getOutputFile());
    startCommand.append("\"");

    return startCommand;
}
