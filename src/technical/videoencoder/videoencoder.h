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

#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <QString>
#include <QStringList>

/**
 * Class for gathering information needed to export a qstopmotion project to
 * a video file.
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class VideoEncoder
{
public:

    /**
     * Enum with all possible encoder applications.
     */
    enum encoderApplication {
        ffmpegApplication,
        mencoderApplication,
        noneApplication
    };

    /**
     * Enum with all possible video output formats
     */
    enum videoFormat {
        aviFormat,            // AVI format
        mp4Format,            // MP4 format
        noneFormat
    };

    /**
     * Enum with all possible video sizes
     */
    enum videoSize {
        defaultSize,          // Source size
        qvgaSize,             // 320x240 (QVGA)
        vgaSize,              // 640x480 (VGA)
        svgaSize,             // 800x600 (SVGA)
        paldSize,             // 704x576 (PAL D)
        hdreadySize,          // 1280x720 (HD Ready)
        fullhdSize            // 1900x1080 (Full HD)
    };

    /**
     * Destrictor
     */
    ~VideoEncoder();

    /**
     * Gets the encoder command registered by the user in the preferences menu.
     * This is the command which should be used to start the encoder.
     * @return the command
     */
    const QString getEncoderCommand() const;

    /**
     * Gets the arguments for the encoder.
     * @return the encoder argument list
     */
    virtual const QStringList getEncoderArguments() const = 0;

    /**
     * Gets the command line registerd by the user in the preferences menu.
     * This is the command which should be used to stop the encoder.
     * @return the stop command
     */
    const QString getStopCommand() const;

    /**
     * Gets the output file generated by the encoder.
     * @return the output file
     */
    const QString getOutputFile() const;

    /**
     * Sets the file which should be genereated by the encoder.
     */
    void setOutputFile(const QString &file);

    /**
     * Checks if the registered encoder is a valid one. Checks if the encoder can
     * be excuted, the output file is a valid file etc.
     * @return true if valid, false otherwise
     */
    bool isValid();

protected:

    /**
     * Constructor
     */
    VideoEncoder();

    /**
     * Sets the encoder command to be used.
     * @param command the encode command.
     * @param v Is the command valid.
     */
    void setEncoderCommand(const QString &command, bool v);

    /**
     * Sets the stop command to be used for stopping the encoder.
     * @param command the stop command to be used in case the user want to
     * cancel the export
     */
    void setStopCommand(const QString &command);

protected:
    QString encoderCommand;
    QString stopCommand;
    QString outputFile;
    int     valid;
};

#endif
