/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#ifndef EXPOSURE_H
#define EXPOSURE_H

#include "domain/animation/animationproject.h"
// #include "domain/animation/scene.h"
#include "domain/animation/take.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// class AnimationProject;
// class Scene;
class Take;
// class Exposure;

/**
 * Class representing the Exposures in the animation
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class Exposure : public QObject
{
    Q_OBJECT
public:

    /**
     * Number of files in the temporary directory.
     */
    static unsigned int tempNumber;

    /**
     * Default constructor
     * @param take The parent of the exposure
     */
    Exposure(Take *take);

    /**
     * Copy constructor
     *
     * @param f Exposure to copy
     */
    Exposure(Exposure *exposure);

    /**
     * Creates a Exposure with the picture in the file with name
     * filename,
     * @param take The parent of the exposure
     * @param fileName the name of the image file for this Exposure.
     * @param location the location of the picture
     */
    Exposure(Take *take, const QString &fileName, int location);

    /**
     * Cleans up after the Exposure
     */
    ~Exposure();

    /**
     * Is this a empty Exposure.
     */
    bool isEmpty();

    /**
     * Get the id of the exposure.
     * @return id of the exposure.
     */
    const QString getId() const;

    /**
     * Set the id of the exposure. If the new id string is empty,
     * nothing is done.
     */
    void setId(const QString &newId);

    /**
     * Get the take object of the exposure.
     * @return Take object of the exposure.
     */
    Take *getTake() const;

    /**
     * Get the index of the exposure in the take
     * @return the index of the exposure
     */
    unsigned int getIndex();

    /**
     * Get the index of the take of the exposure in the scene
     * @return the index of the take
     */
    unsigned int getTakeIndex() const;

    /**
     * Get the index of the scene of the exposure in the animation project
     * @return the index of the scene
     */
    unsigned int getSceneIndex() const;

    /**
     * Get the exposure location.
     * @return the exposure location
     */
    // Exposure::fileLocation getExposureLocation();

    /**
     * Retrieves the absolute path to the picture of this Exposure.
     * @return the absolute path to the picture of this Exposure.
     */
    const QString getImagePath() const;

    /**
     * Use moveToImageDir!!!!!
     *
     * Moves sounds and images belonging to this Exposure into project directories.
     * @param imageDir the image directory to move images into
     * @param imageIndex a index describing the position of this Exposure relative
     * to the other Exposures. E.g. 000005 if this Exposure is position five in the sequence
     * of Exposures.
     */
    // void moveToProjectDir(const QString &imageDir, unsigned int imageIndex);

    /**
     * Moves the images to an image directory.
     * @param directory the directory where the project files are stored
     * @param imageIndex the index of the image which is used to set a filename
     */
    // void moveToImageDir(const QString &directory, unsigned int imageIndex);

    /**
     * Moves the files belonging to this Exposure from the current directory
     * to the temporary directory.
     * @param isRecovery The project is in the recovery mode.
     */
    void moveToTemp(bool isRecovery);

    /**
     * Copy the files belonging to this Exposure from the old directory
     * to the temporary directory.
     */
    void copyToTemp();

    /**
     * Convert the files belonging to this Exposure to a new image format
     * @param newFormat The new format of the images
     * from the directory to the temporary directory.
     * @param quality The quality of the image.
     */
    void convertToTemp(int newFormat, int quality);

    /**
     * Moves the files belonging to this Exposure from the current directory
     * to the project directory.
     */
    void moveToProject(const QString newName);

    /**
     * Get frame data from project file
     */
    bool readDataFromProject(QDomElement &exposureNode);

    /**
     * Set frame data to project file.
     */
    bool saveDataToProject(QDomDocument &doc, QDomElement &exposureNode);

private:

    /**
     * The parent of the scene object
     */
    Take *parent;

    /**
     * Id of the exposure.
     */
    QString id;

    /**
     * Absolute path to the image file. The image can either be in a project
     * directory, the tmp directory or the trash directory.
     */
    // QString imagePath;

    /**
     * File name of the frame
     */
    QString theFrame;

    /**
     * True if this Exposure is saved to a project file. It is also true if the
     * Exposure is loaded from a previously saved project.
     */
    int exposureLocation;

    /**************************************************************************
     * Private functions
     **************************************************************************/

    /**
     * Gets the id of an image. This is just the filename of the image without
     * extension.
     * @return an id for the image
     */
    const QString getExposureId();

};

#endif
