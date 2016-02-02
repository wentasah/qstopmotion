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

#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include <QString>

#include "domain/animation/animationproject.h"
#include "technical/videoencoder/videoencoder.h"

class AnimationProject;


/**
 * Class for gathering information needed to export a qstopmotion project to
 * a video file.
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class FfmpegEncoder : public VideoEncoder
{
public:

    /**
     * Constructor
     */
    FfmpegEncoder(AnimationProject *ap);

    /**
     * Destructor
     */
    ~FfmpegEncoder();

    /**
     * Gets the encoder command registered by the user in the preferences menu.
     * This is the command which should be used to start the encoder.
     * @return the command
     */
    const QString getCommand();

    /**
     * Gets the arguments for the encoder.
     * @return the encoder argument list
     */
    const QStringList getEncoderArguments() const;

private:
    AnimationProject *animationProject;
};

#endif
