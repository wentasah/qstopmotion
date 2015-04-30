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

#ifndef VIDEOENCODERFACTORY_H
#define VIDEOENCODERFACTORY_H

#include "frontends/frontend.h"
#include "technical/videoencoder/videoencoder.h"


/**
 * Class for exporting a qstopmotion project containing images and sounds to
 * to a video file.
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class VideoEncoderFactory
{
public:

    /**
     * Registers a serializer that will be used to gather information about
     * image paths etc.
     * @param f the frontend to display information to
     */
    VideoEncoderFactory(Frontend *f);

    /**
     * Destructor
     */
    ~VideoEncoderFactory();

    /**
     * Creates the video file based on the passed encoder. The encoder holds
     * information needed to complete the export.
     * @param encoder the encoder to be used for video export
     * @return path to the created video file
     */
    const QString createVideoFile(VideoEncoder *encoder);

private:
    Frontend *frontend;
};

#endif
