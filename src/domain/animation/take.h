/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
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

#ifndef TAKE_H
#define TAKE_H

#include "domain/animation/animationproject.h"
#include "domain/animation/scene.h"
#include "domain/animation/take.h"
#include "domain/animation/exposure.h"
#include "frontends/frontend.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class Scene;
class Take;
class Exposure;
class Frontend;

/**
 * Class representing the scenes in the animation
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class Take : public QObject
{
    Q_OBJECT
public:
    /**
     * Initializes the take
     */
    Take(Scene *s);

    /**
     * Cleans up the take and delete the exposures.
     */
    ~Take();

    /**
     * Is this a empty Take.
     */
    bool isEmpty();

    /**
     * Get the id of the take.
     * @return id of the take.
     */
    const QString getId() const;

    /**
     * Set the id of the take. If the new id string is empty,
     * nothing is done.
     */
    void setId(const QString &newId);

    /**
     * Get the description of the take.
     * @return description of the take.
     */
    const QString getDescription() const;

    /**
     * Set the description of the take. If the new description string is empty,
     * nothing is done.
     */
    void setDescription(const QString &newDescription);

    /**
     * Get the scene object of the take.
     * @return the scene object of the take.
     */
    Scene *getScene();

    /**
     * Get the image path of the project
     * @return image path of the project
     */
    const QString getImagePath() const;

    /**
     * Get the index of the take in the scene
     * @return the index of the take
     */
    unsigned int getIndex();

    /**
     * Get the index of the scene of the take in the animation project
     * @return the index of the scene
     */
    unsigned int getSceneIndex();

    /**
     * Get the frontend.
     * @return the frontend.
     */
    Frontend* getFrontend();

    /**
     * Get the animation project.
     * @return the animation project.
     */
    AnimationProject* getAnimationProject();

    /**
     * Get take data from project file.
     */
    bool readDataFromProject(QDomElement &takeNode);

    /**
     * Set take data to project file.
     */
    bool saveDataToProject(QDomDocument &doc, QDomElement &takeNode,
                           const QString &sceneId, int takeIndex);

    /**
     * Get the temp path of the project
     * @return temp path of the project
     */
    const QString getAppTempDirName() const;

    /**
     * Get the trash path of the project
     * @return trash path of the project
     */
    const QString getAppTrashDirName() const;

    /**************************************************************************
     * Exposure functions
     **************************************************************************/

    /**
     * Returns the index of the currently active exposure in the take.
     * @return the index of the currently active exposure.
     */
    int getActiveExposureIndex() const;

    /**
     * Returns the index of the given exposure in the take.
     * Returns -1 if no item matched.
     * @return the index of the given exposure.
     */
    int getExposureIndex(Exposure *exposure);

    /**
     * Set the active exposure.
     * @param newExposure the pointer of the actual exposure.
     */
    void setActiveExposure(Exposure *newExposure);

    /**
     * Set the active exposure index.
     * @param exposureIndex the index of the actual exposure.
     */
    void setActiveExposureIndex(int exposureIndex);

    /**
     * Retrieves the size of exposures of the take.
     * @return the size of exposures in the take.
     */
    unsigned int getExposureSize() const;

    /**
     * Retrieves the exposure at position exposureIndex in the scene.
     * @param exposureIndex the number of the exposure to retrieve.
     * @return the exposure at position exposureIndex.
     */
    Exposure *getExposure(unsigned int exposureIndex);

    /**
     *Temporary functions to assist in an orderly transfer of functionality from
     *Animation, without having to break the project for big time-periods.
     */
    void getExposures(QVector<Exposure*>& allExposures);

    /**
     * Creates a exposure with the picture at location exposureName and adds it at the
     * end of the take.
     * @param fileName the name of the image file to the exposure to create.
     * @param location the location of the picture
     * @return the new path to the picture file for the undo object.
     */
    Exposure *addExposure(const QString &fileName, int location);

    /**
     * Creates a exposure with the picture at location exposureName and adds it at position
     * at position index in the take.
     * @param fileName the name of the image file of the exposure to create.
     * @param location the location of the picture
     * @param index the place to create the exposure.
     * @return the new path to the picture file for the undo object.
     */
    Exposure *insertExposure(const QString &fileName, int location, int index);

    /**
     * Moves the exposures at the positions from fromExposure to toExposure (inclusive)
     * to the position movePosition inside the scene.
     * @param fromExposure the first exposure to move.
     * @param toExposure the lst exposure to move.
     * @param movePosition the position to move the exposures to.
     */
    void moveExposures(unsigned int fromExposure,
                       unsigned int toExposure,
                       unsigned int movePosition);

    /**
     * Remove the exposures at the positions from fromExposure to toExposure (inclusive)
     * from the scene.
     * @param fromExposure the first exposure to remove from the scene.
     * @param toExposure the last exposure to remove from the scene.
     * @return the new paths where the exposure pictures has been moved. This is
     * for the undo object.
     */
    const QVector<Exposure*> removeExposures(unsigned int fromExposure,
                                             const unsigned int toExposure);

    /**
     * Removes the active exposure from the animation.
     * @return The removed exposure.
     */
    Exposure* removeActiveExposure();

    /**
     * Removes the exposure with the given index from the animation.
     * @arg exposureIndex the index of the exposure to remove
     * @return The removed exposure or NULL if nothing is done
     */
    Exposure* removeExposure(unsigned int exposureIndex);

    /**
     * Cleans exposures from the scene without moving them around. Used when the user
     * aborts while adding exposures.
     * @param fromExposure the first exposure to remove.
     * @param toExposure the last exposure to remove.
     */
    void cleanExposures(unsigned int fromExposure,
                        unsigned int toExposure);

    /**
     * Adds an already saved exposure.
     * @param f the exposure to add
     */
    void addSavedExposure(Exposure *f);

    /**
     * Retrieves the image paths of all the exposures in this scene.
     *(This is an convenience function and is used when opening scenes)
     * @return the image paths of all the exposures in this scene.
     */
    QVector<QString> getImagePaths() const;

    /**************************************************************************
     * Sound functions
     **************************************************************************/

    /**
     * Get the start position of the take on th sound file of the scene.
     * @return start position.
     */
    unsigned int getSoundStartPosition() const;

    /**
     * Sets the start position of the take on the sound file of the scene.
     * @param newSoundStartPosition new start position.
     */
    void setSoundStartPosition(unsigned int newSoundStartPosition);

    /**
     * Get sound data from project file.
     */
    bool readSoundFromProject(QDomElement &soundNode);

    /**
     * Set sound data to project file.
     */
    bool saveSoundToProject(QDomDocument &doc, QDomElement &takeNode);

private:

    /**
     * The parent of the scene object
     */
    Scene *parent;

    /**
     * Id of the take.
     */
    QString id;

    /**
     * Description of the take.
     */
    QString description;

    /**
     * True if this take is saved to a project file. It is also true if the
     * take is loaded from a previously saved project.
     */
    bool isProjectFile;

    /**
     * Start position of the take on the sound file of the scene
     */
    unsigned int soundStartPosition;

    /**
     * Next exposure index to use to create a dummy description string.
     */
    unsigned int nextExposureIndex;

    /**
     * Current active exposure of the take
     */
    int activeExposureIndex;

    /**
     * Id of the active exposure. (Readed from the project file.)
     */
    QString activeExposureId;

    /**
     * Vector containing the exposures of the take.
     */
    QVector<Exposure*> exposures;

    /**************************************************************************
     * Private functions
     **************************************************************************/

    /**
     * Activate the activeExposure.
     */
    // void activateExposure();

};

#endif
