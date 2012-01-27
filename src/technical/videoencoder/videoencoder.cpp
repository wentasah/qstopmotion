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

#include "videoencoder.h"

#include "technical/util.h"

#include <stdlib.h>
#include <string.h>


VideoEncoder::VideoEncoder()
{
    valid = -1;
}


VideoEncoder::~VideoEncoder()
{
    encoderCommand.clear();
    stopCommand.clear();
    outputFile.clear();
}


const QString VideoEncoder::getEncoderCommand() const
{
    return encoderCommand;
}


void VideoEncoder::setEncoderCommand(const QString &command, bool v)
{
    if (!encoderCommand.isEmpty()) {
        encoderCommand.clear();
    }
    encoderCommand.append(command);
    if (v) {
        valid = 1;
    }
}


const QString VideoEncoder::getStopCommand() const
{
    return stopCommand;
}


void VideoEncoder::setStopCommand(const QString &command)
{
    if (!stopCommand.isEmpty()) {
        stopCommand.clear();
    }
    stopCommand.append(command);
}


const QString VideoEncoder::getOutputFile() const
{
    return outputFile;
}


void VideoEncoder::setOutputFile(const QString &file)
{
    if (!outputFile.isEmpty()) {
        outputFile.clear();
    }
    outputFile.append(file);
}


bool VideoEncoder::isValid()
{
    if (valid == -1)
    {
        const QString path = Util::checkCommand(encoderCommand);
        if (path.isEmpty())
        {
            valid = 0;
        }
        else
        {
            encoderCommand.clear();
            encoderCommand.append(path);
            valid = 1;
        }
    }
    if (valid == 0)
    {
        return false;
    }
    return true;
}
