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

#ifndef SCENE_H
#define SCENE_H

#include "domain/animation/animationproject.h"
#include "domain/animation/exposure.h"
#include "domain/animation/take.h"
#include "frontends/frontend.h"
#include "technical/audio/audiodriver.h"
#include "technical/audio/audioformat.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class AnimationProject;
class Scene;
class Take;
class Exposure;
class Frontend;

/**
 * Class representing the scenes in the animation
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class Scene : public QObject
{
    Q_OBJECT
public:
    /**
     * Initializes the scene
     */
    Scene(AnimationProject *project);

    /**
     * Cleans up the scene and delete the takes.
     */
    ~Scene();

    /**
     * Is this a empty Scene.
     */
    bool isEmpty();

    /**
     * Get the id of the scene.
     * @return id of the scene.
     */
    const QString getId() const;

    /**
     * Set the id of the scene. If the new id string is empty,
     * nothing is done.
     */
    void setId(const QString &newId);

    /**
     * Get the description of the scene.
     * @return description of the scene.
     */
    const QString getDescription() const;

    /**
     * Set the description of the scene. If the new description string is empty,
     * nothing is done.
     */
    void setDescription(const QString &newDescription);

    /**
     * Get the new image path of the project
     * @return image path of the project
     */
    const QString getNewImagePath() const;

    /**
     * Get the old image path of the project
     * @return image path of the project
     */
    const QString getOldImagePath() const;

    /**
     * Get the parent of the scene
     * @return parent of the scene
     */
    AnimationProject *getParent();

    /**
     * Get the index of the scene in the animation project
     * @return the index of the scene
     */
    unsigned int getIndex();

    /**
     * Get the frontend.
     * @return the frontend.
     */
    Frontend* getFrontend();

    /**
     * Sets this Exposure as a valid project file.
     */
    void markAsProjectFile();

    /**
     * Get scene data from project file.
     */
    bool readDataFromProject(QDomElement &sceneNode);

    /**
     * Set scene data to project file.
     */
    bool saveDataToProject(QDomDocument &doc, QDomElement &sceneNode, int sceneIndex);

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
     * Take functions
     **************************************************************************/

    /**
     * Retrieves the index of the active take in the scene.
     * @return the index of take in the scene.
     */
    int getActiveTakeIndex() const;

    /**
     * Returns the index of the given take in the scene.
     * Returns -1 if no item matched.
     * @return the index of the given take.
     */
    int getTakeIndex(Take *take) const;

    /**
     * Set the active take index.
     * @param takeIndex the index of the new active take.
     */
    void setActiveTakeIndex(int takeIndex);

    /**
     * Retrieves the active take in the scene.
     * @return the active of take in the scene.
     */
    Take *getActiveTake();

    /**
     * Set the active take.
     * @param newTake the pointer of the new active take.
     */
    void setActiveTake(Take *newTake);

    /**
     * Retrieves the size of takes in the scene.
     * @return the size of takes in the scene.
     */
    int getTakeSize() const;

    /**
     * Retrieves the take at position takeIndex.
     * @param takeIndex the index of the exposure to retrieve.
     * @return the take at position takeIndex.
     */
    Take *getTake(unsigned int takeIndex);

    /**
     * Create a new take and add the take as the last to the scene.
     * @param takeDescription the description of the take or a empty string.
     * @return the new take.
     */
    Take *addTake(const QString &takeDescription);

    /**
     * Createa new take and insert the take bevor the active scene.
     * @param takeDescription the description of the take or a empty string.
     * @return the new take.
     */
    Take *insertTake(const QString &takeDescription);

    /**
     * Moves the take at position takeIndex to the position movePosition.
     * @param takeIndex the index of the take to move.
     * @param movePosition the position to move the take to.
     */
    void moveTake(int takeIndex, int movePosition);

    /**
     * Removes the active take from the scene.
     */
    // void removeActiveTake();

    /**
     * Removes a take from the scene.
     * @param takeIndex The index of the take to remove.
     * @return The removed take.
     */
    Take *removeTake(unsigned int takeIndex);

    /**************************************************************************
     * Exposure functions
     **************************************************************************/

    /**
     * Retrieves the number of exposures in the scene.
     * @return the number of exposures in the scene.
     */
    unsigned int getExposureSize() const;

    /**
     * Retrieves the index of active exposure in the active take.
     * @return the index of exposure in the take.
     */
    int getActiveExposureIndex() const;

    /**
     * Set the active exposure index.
     * @param exposureIndex the index of the new active exposure.
     */
    void setActiveExposureIndex(int exposureIndex);

    /**
     * Retrieves the exposure at position exposureIndex in the take takeIndex.
     * @param takeIndex the index of the take of the exposure to retrieve.
     * @param exposureIndex the index of the exposure to retrieve.
     * @return the exposure at position exposureIndex.
     */
    Exposure* getExposure(unsigned int takeIndex,
                          unsigned int exposureIndex);

    /**
     * Temporary functions to assist in an orderly transfer of functionality from
     * Animation, without having to break the project for big time-periods.
     */
    void getExposures(QVector<Exposure*>& allExposures);

    /**
     * Creates a exposure with the picture at location exposureName and adds it the
     * end in the scene.
     * @param fileName the name fo the image file to the exposure to create.
     * @param location the location of the picture
     * @return the new path to the picture file for the undo object.
     */
    Exposure* addExposure(const QString &fileName, int location);

    /**
     * Creates a exposure with the picture at location exposureName and adds it at position
     * at position index in the scene.
     * @param fileName the name to the image file to the exposure to create.
     * @param location the location of the picture
     * @param index the place to create the exposure.
     * @return the new path to the picture file for the undo object.
     */
    Exposure* insertExposure(const QString &fileName, int location, int &index);

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
     * Removes the exposure with the given tree item from the animation.
     * @arg takeIndex the index of the take with the exposure to remove
     * @arg exposureIndex the index of the exposure to remove
     * @return The removed exposure or NULL if nothing is done
     */
    Exposure *removeExposure(unsigned int takeIndex,
                             unsigned int exposureIndex);

    /**
     * Moves the exposures at the positions from fromExposure to toExposure (inclusive)
     * to the position movePosition inside the scene.
     * @param fromExposure the first exposure to move.
     * @param toExposure the lst exposure to move.
     * @param movePosition the position to move the exposures to.
     */
    void moveExposures(unsigned int fromExposure, unsigned int toExposure,
                       unsigned int movePosition);

    /**
     * Get the position of the modified exposure in the project.
     * @arg filePath The path to the modified exposure.
     * @arg modTakeIndex the index of the take of the modified exposure.
     * @arg modExposureIndex the index of the modified exposure.
     * @return True if the exposure is found and false else.
     */
    bool getModifyedExposure(const QString &filePath,
                             int &modTakeIndex,
                             int &modExposureIndex);

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
     * Get the start position of the scene on th sound file of the scene.
     * @return start position.
     */
    unsigned int getSoundStartPosition() const;

    /**
     * Sets the start position of the take on the sound file of the scene.
     * @param newSoundStartPosition new start position.
     */
    void setSoundStartPosition(unsigned int newSoundStartPosition);

    /**
     * Adds the sound in the file filename to this Exposure.
     * @param filename the name of the file where the sound is.
     * @param soundName the new name of the sound.
     * @return zero on success, less than zero on failure;
     * -1 = file is not readable
     * -2 = not a valid audio file
     */
    int addSound(const QString &filename, const QString &soundName);

    /**
     * Removes sound number soundNumber from this Exposure.
     * @param soundNumber
     */
    void removeSound(unsigned int soundNumber);

    /**
     * Returns the number of sounds in this Exposure.
     * @return the number of sounds in this Exposure.
     */
    unsigned int getNumberOfSounds() const;

    /**
     * Returns the sounds belonging to this Exposure.
     * @return a vector containing audio objects which kan be played
     * with the implemented audio driver
     */
    QVector<AudioFormat*>& getSounds();

    /**
     * Retrieves the name of the sound at index soundNumber in this Exposure.
     * @param soundNumber the sound to return.
     * @return the sound at index soundNumber in this Exposure.
     */
    const QString getSoundName(unsigned int soundNumber) const;

    /**
     * Plays the sounds belonging to this Exposure.
     */
    void playSounds(AudioDriver *driver);

    /**
     * Moves the sounds to a sound directory.
     * @param directory the directory to move the sounds to
     */
    void moveToSoundDir(const QString &directory);

    /**
     * Get sound data from project file.
     */
    bool readSoundFromProject(QDomElement &soundNode);

    /**
     * Set sound data to project file.
     */
    bool saveSoundToProject(QDomDocument &doc, QDomElement &sceneNode);

    /**************************************************************************
     * Operator functions
     **************************************************************************/

    // bool operator!= (const Scene &other) const;

private:

    /**
     * The parent of the scene object
     */
    AnimationProject *parent;

    /**
     * Id of the scene.
     */
    QString id;

    /**
     * Description of the scene.
     */
    QString description;

    /**
     * True if this scene is saved to a project file. It is also true if the
     * scene is loaded from a previously saved project.
     */
    bool isProjectFile;

    /**
     * Contains the sounds beloning to this Exposure.
     */
    QVector<AudioFormat*> sounds;

    /**
     * Contains the sound names beloning to this Exposure. The names are user
     * defined e.g. Speech 1.
     */
    QVector<QString> soundNames;

    /**
     * Number of sounds belonging to this Exposure.
     */
    int soundNum;

    /**
     * Start position of the take on the sound file of the scene
     */
    unsigned int soundStartPosition;

    /**
     * Absolute path to the sound file. The sound can either be in a project
     * directory, the tmp directory or the trash directory.
     */
    QString soundPath;

    /**
     * Current active Take of the scene
     */
    int activeTakeIndex;

    /**
     * Id of the active take. (Readed from the project file.)
     */
    QString activeTakeId;

    /**
     * next take index to use to create a dummy description string.
     */
    unsigned int nextTakeIndex;

    /**
     * Vector containing all takes of the scene
     */
    QVector<Take*> takes;

    /**************************************************************************
     * Private functions
     **************************************************************************/

    /**
     * Activate the activeTake.
     */
    // void activateTake();

};

#endif
