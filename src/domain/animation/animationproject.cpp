/******************************************************************************
 *  Copyright (C) 2005-2016 by                                                *
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

#include "animationproject.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QtGlobal>

#include "domain/domainfacade.h"
// #include "technical/audio/ossdriver.h"
#include "technical/videoencoder/videoencoderfactory.h"


AnimationProject::AnimationProject(Frontend* f)
{
    qDebug() << "AnimationProject::Constructor --> Start";

    frontend                 = f;
    serializer               = new ProjectSerializer(f);

    recordingMode            = 0;
    videoSource              = 0;
    videoResolution          = 0;
    mixMode                  = 0;
    mixCount                 = 2;
    playbackCount            = 0;
    overlayIntensity         = 100;

    encoderApplication       = 0;
    imageFormat              = 0;
    imageQuality             = 100;
    imageSize                = 0;
    imageTransformation      = AnimationProject::ScaleImage;
    imageAdjustment          = 0;
    zoomValue                = 25;
    liveViewFps              = 20;
    videoFormat              = 0;
    videoSize                = 0;
    videoFps                 = 12;

    useDefaultOutputFile     = false;
    unitMode                 = 0;
    activeSceneIndex         = -1;
    nextSceneIndex           = 0;
    nextTotalExposureIndex   = 0;

    settingsChanges          = 0;
    animationChanges         = 0;

    soundsNumber             = -1;
    audioDriver              = NULL; //new OSSDriver("/dev/dsp");
    isAudioDriverInitialized = false;

    qDebug() << "AnimationProject::Constructor --> End";
}


AnimationProject::~AnimationProject()
{
    qDebug() << "AnimationProject::Destructor --> Start";

    unsigned int sceneSize = scenes.size();
    for (unsigned int sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        delete scenes[sceneIndex];
        scenes[sceneIndex] = NULL;
    }
    scenes.clear();

    if (serializer != NULL) {
        serializer->cleanup();
        delete serializer;
    }

    if (audioDriver != NULL) {
        delete audioDriver;
        audioDriver = NULL;
    }

    description.clear();
    frontend = NULL;

    qDebug() << "AnimationProject::Destructor --> End";
}


Frontend* AnimationProject::getFrontend()
{
    Q_ASSERT(frontend != NULL);

    return frontend;
}


ProjectSerializer* AnimationProject::getProjectSerializer()
{
    return serializer;
}


const QString AnimationProject::getAppTempDirName() const
{
    return QString(this->frontend->getTempDirName());
}


const QString AnimationProject::getNewProjectFilePath() const
{
    return serializer->getNewProjectFilePath();
}


const QString AnimationProject::getNewProjectPath() const
{
    return serializer->getNewProjectPath();
}


const QString AnimationProject::getDescription() const
{
    return this->description;
}


const QString AnimationProject::getNewImagePath() const
{
    return serializer->getNewImagePath();
}


const QString AnimationProject::getOldImagePath() const
{
    return serializer->getOldImagePath();
}


const QString AnimationProject::getNewSoundPath() const
{
    return serializer->getNewSoundPath();
}


const QString AnimationProject::getOldSoundPath() const
{
    return serializer->getOldSoundPath();
}


/**************************************************************************
 * Project preferences
 **************************************************************************/

int AnimationProject::getRecordingMode() const
{
    return recordingMode;
}


void AnimationProject::setRecordingMode(int newRecordingMode)
{
    if (recordingMode != newRecordingMode) {
        recordingMode = newRecordingMode;
        incSettingsChanges();
    }
}


int AnimationProject::getVideoSource() const
{
    return videoSource;
}


void AnimationProject::setVideoSource(int newVideoSource)
{
    if (videoSource != newVideoSource) {
        videoSource = newVideoSource;
        incSettingsChanges();
    }
}


int AnimationProject::getResolution() const
{
    return videoResolution;
}


void AnimationProject::setResolution(int newResolution)
{
    if (videoResolution != newResolution) {
        videoResolution = newResolution;
        incSettingsChanges();
    }
}


int AnimationProject::getMixMode() const
{
    return mixMode;
}


void AnimationProject::setMixMode(int newMixMode)
{
    if (mixMode != newMixMode) {
        mixMode = newMixMode;
        incSettingsChanges();
    }
}


int AnimationProject::getMixCount() const
{
    return mixCount;
}


void AnimationProject::setMixCount(int newMixCount)
{
    if (mixCount != newMixCount) {
        mixCount = newMixCount;
        incSettingsChanges();
    }
}


int AnimationProject::getPlaybackCount() const
{
    return playbackCount;
}


void AnimationProject::setPlaybackCount(int newPlaybackCount)
{
    if (playbackCount != newPlaybackCount) {
        playbackCount = newPlaybackCount;
        incSettingsChanges();
    }
}


int AnimationProject::getUnitMode() const
{
    return unitMode;
}


void AnimationProject::setUnitMode(int newUnitMode)
{
    if (unitMode != newUnitMode) {
        unitMode = newUnitMode;
        incSettingsChanges();
    }
}


int AnimationProject::getOverlayIntensity()
{
    return overlayIntensity;
}


void AnimationProject::setOverlayIntensity(int overlay)
{
    if (overlayIntensity != overlay) {
        overlayIntensity = overlay;
        incSettingsChanges();
    }
}

/**************************************************************************
 * Image import preferences
 **************************************************************************/

int AnimationProject::getImageFormat()
{
    return imageFormat;
}


void AnimationProject::setImageFormat(int newIF)
{
    if (imageFormat != newIF) {
        imageFormat = newIF;
        incSettingsChanges();
    }
}


int AnimationProject::getImageQuality()
{
    return imageQuality;
}


void AnimationProject::setImageQuality(int newValue)
{
    if (imageQuality != newValue) {
        imageQuality = newValue;
        incSettingsChanges();
    }
}


int AnimationProject::getImageSize()
{
    return imageSize;
}


void AnimationProject::setImageSize(int newIS)
{
    if (imageSize != newIS) {
        imageSize = newIS;
        incSettingsChanges();
    }
}


int AnimationProject::getImageTransformation()
{
    return imageTransformation;
}


void AnimationProject::setImageTransformation(int newTransform)
{
    if (imageTransformation != newTransform) {
        imageTransformation = newTransform;
        incSettingsChanges();
    }
}


int AnimationProject::getImageAdjustment()
{
    return imageAdjustment;
}


void AnimationProject::setImageAdjustment(int newIA)
{
    if (imageAdjustment != newIA) {
        imageAdjustment = newIA;
        incSettingsChanges();
    }
}


int AnimationProject::getZoomValue()
{
    return zoomValue;
}


void AnimationProject::setZoomValue(int newZV)
{
    if (zoomValue != newZV) {
        zoomValue = newZV;
        incSettingsChanges();
    }
}


int AnimationProject::getLiveViewFps()
{
    return liveViewFps;
}


void AnimationProject::setLiveViewFps(int newValue)
{
    if (liveViewFps != newValue) {
        liveViewFps = newValue;
        incSettingsChanges();
    }
}

/**************************************************************************
 * Video export preferences
 **************************************************************************/

int AnimationProject::getEncoderApplication() const
{
    return encoderApplication;
}


void AnimationProject::setEncoderApplication(int newEA)
{
    if (encoderApplication != newEA) {
        encoderApplication = newEA;
        incSettingsChanges();
    }
}


int AnimationProject::getVideoFormat() const
{
    return videoFormat;
}


void AnimationProject::setVideoFormat(int newVF)
{
    if (videoFormat != newVF) {
        videoFormat = newVF;
        incSettingsChanges();
    }
}


int AnimationProject::getVideoSize() const
{
    return videoSize;
}


void AnimationProject::setVideoSize(int newVS)
{
    if (videoSize != newVS) {
        videoSize = newVS;
        incSettingsChanges();
    }
}


int AnimationProject::getVideoFps() const
{
    return videoFps;
}


void AnimationProject::setVideoFps(int newFPS)
{
    if (videoFps != newFPS) {
        videoFps = newFPS;
        incSettingsChanges();
    }
}


bool AnimationProject::getUseDefaultOutputFile() const
{
    return useDefaultOutputFile;
}


void AnimationProject::setUseDefaultOutputFile(bool newUDOF)
{
    if (useDefaultOutputFile != newUDOF) {
        useDefaultOutputFile = newUDOF;
        incSettingsChanges();
    }
}


const QString AnimationProject::getDefaultOutputFileName()
{
    return defaultOutputFileName;
}


void AnimationProject::setDefaultOutputFileName(const QString newDOFN)
{
    if (defaultOutputFileName.compare(newDOFN) != 0) {
        defaultOutputFileName = newDOFN;
        incSettingsChanges();
    }
}

/**************************************************************************
 * Project functions
 **************************************************************************/

bool AnimationProject::openProject(const QString &filePath)
{
    qDebug() << "AnimationProject::openProject --> Start";

    serializer->setNewProjectFilePath(filePath);
    serializer->read();

    if (!readSettingsFromProject(serializer->getSettingsElement())) {
        qWarning() << "AnimationProject::openProject --> Read settings failed";
        return false;
    }
    if (!readScenesFromProject(serializer->getAnimationElement())) {
        qWarning() << "AnimationProject::openProject --> Read animation data failed";
        return false;
    }
    if (scenes.size() == 0) {
        qDebug() << "AnimationProject::openProject --> End (without scene)";
        return false;
    }

    qDebug() << "AnimationProject::openProject --> End";
    return true;
}


bool AnimationProject::saveProject(const QString &filePath, bool saveAs)
{
    qDebug() << "AnimationProject::saveProject --> Start";

    if (!filePath.isEmpty()) {
        serializer->setNewProjectFilePath(filePath);
    }

    frontend->showProgress(tr("Saving scenes to disk ..."), frontend->getProject()->getTotalExposureSize());

    if (!serializer->save(this, saveAs)) {
        qWarning() << "AnimationProject::saveProject --> save animation data failed";
        frontend->hideProgress();
        return false;
    }
    // TODO: Differentiation in savig of settings and animation
    settingsChanges = 0;
    animationChanges = 0;

    frontend->hideProgress();

    qDebug() << "AnimationProject::saveProject --> End";
    return true;
}


bool AnimationProject::newProject(const QString &projectDescription)
{
    qDebug() << "AnimationProject::newProject --> Start";

    description.append(projectDescription);

    qDebug() << "AnimationProject::newProject --> End";
    return true;
}


bool AnimationProject::isSettingsChanges() const
{
    if (0 == settingsChanges) {
        return false;
    }
    return true;
}


void AnimationProject::incSettingsChanges()
{
    settingsChanges++;
}


void AnimationProject::decSettingsChanges()
{
    Q_ASSERT(0 < settingsChanges);

    settingsChanges--;
}


bool AnimationProject::isAnimationChanges() const
{
    if (0 == animationChanges) {
        return false;
    }
    return true;
}


void AnimationProject::incAnimationChanges()
{
    animationChanges++;
}


void AnimationProject::decAnimationChanges()
{
    Q_ASSERT(0 < animationChanges);

    animationChanges--;
}


bool AnimationProject::initAudioDevice()
{
    qDebug() << "AnimationProject::initAudioDevice --> Empty";
    /*
    isAudioDriverInitialized = audioDriver->initialize();
    if (!isAudioDriverInitialized && numSounds > -1) {
        frontend->showWarning(tr("Initialization Audio"),
            tr("Cannot play sound. Check that you have the right\n"
            "permissions and other programs do not block\n"
            "the audio device. Audio will be disabled until you\n"
            "have fixed the problem."));
    }

    qDebug() << "AnimationProject::initAudioDevice --> Start";
    return isAudioDriverInitialized;
    */
    return false;
}


void AnimationProject::shutdownAudioDevice()
{
    qDebug() << "AnimationProject::shutdownAudioDevice --> Start";

    audioDriver->shutdown();
    isAudioDriverInitialized = false;

    qDebug() << "AnimationProject::shutdownAudioDevice --> End";
}


bool AnimationProject::exportToVideo(VideoEncoder * encoder)
{
    qDebug() << "AnimationProject::exportToVideo --> Start";

    VideoEncoderFactory factory(this->getFrontend());
    if (factory.createVideoFile(encoder) != NULL) {
        return true;
    }

    qDebug() << "AnimationProject::exportToVideo --> End";
    return false;
}


bool AnimationProject::exportToCinelerra(const QString&)
{
    return false;
}


bool AnimationProject::readSettingsFromProject(QDomElement &settingsNode)
{
    qDebug() << "AnimationProject::readSettingsFromProject --> Start";

    QDomElement currElement = settingsNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        // Save project parameter

        if (nodeName.compare("recordingmode") == 0) {
            QString tmp = currElement.text();
            recordingMode = tmp.toInt();
            frontend->setRecordingMode(tmp.toInt());
        }
        else if (nodeName.compare("videosource") == 0) {
            QString tmp = currElement.text();
            if (frontend->setVideoSource(tmp.toInt())) {
                videoSource = tmp.toInt();
            }
            else {
                setVideoSource(frontend->getVideoSource());
                frontend->setVideoSource(getVideoSource());
            }
        }
        else if (nodeName.compare("videoresolution") == 0) {
            QString tmp = currElement.text();
            if (frontend->setResolution(tmp.toInt())) {
                videoResolution = tmp.toInt();
            }
            else {
                setResolution(frontend->getResolution());
            }
        }
        else if (nodeName.compare("mixmode") == 0) {
            QString tmp = currElement.text();
            mixMode = tmp.toInt();
            frontend->setMixMode(tmp.toInt());
        }
        else if (nodeName.compare("mixcount") == 0) {
            QString tmp = currElement.text();
            mixCount = tmp.toInt();
            frontend->setMixCount(tmp.toInt());
        }
        else if (nodeName.compare("playbackcount") == 0) {
            QString tmp = currElement.text();
            playbackCount = tmp.toInt();
        }
        else if (nodeName.compare("unitmode") == 0) {
            QString tmp = currElement.text();
            unitMode = tmp.toInt();
        }
        else if (nodeName.compare("overlayintensity") == 0) {
            QString tmp = currElement.text();
            overlayIntensity = tmp.toInt();
            frontend->setOverlayIntensity(overlayIntensity);
        }
        // Save image import parameter
        else if (nodeName.compare("imageformat") == 0) {
            QString tmp = currElement.text();
            imageFormat = tmp.toInt();
        }
        else if (nodeName.compare("imagequality") == 0) {
            QString tmp = currElement.text();
            imageQuality = tmp.toInt();
        }
        else if (nodeName.compare("imagesize") == 0) {
            QString tmp = currElement.text();
            imageSize = tmp.toInt();
        }
        else if (nodeName.compare("imagetransformation") == 0) {
            QString tmp = currElement.text();
            imageTransformation = tmp.toInt();
        }
        else if (nodeName.compare("imageadjustment") == 0) {
            QString tmp = currElement.text();
            imageAdjustment = tmp.toInt();
        }
        else if (nodeName.compare("zoomvalue") == 0) {
            QString tmp = currElement.text();
            zoomValue = tmp.toInt();
        }
        else if (nodeName.compare("liveviewfps") == 0) {
            QString tmp = currElement.text();
            liveViewFps = tmp.toInt();
        }
        // Save video export parameter
        else if (nodeName.compare("encoderapplication") == 0) {
            QString tmp = currElement.text();
            encoderApplication = tmp.toInt();
        }
        else if (nodeName.compare("videoformat") == 0) {
            QString tmp = currElement.text();
            videoFormat = tmp.toInt();
        }
        else if (nodeName.compare("videosize") == 0) {
            QString tmp = currElement.text();
            videoSize = tmp.toInt();
        }
        else if (nodeName.compare("videofps") == 0) {
            QString tmp = currElement.text();
            videoFps = tmp.toInt();
            if (videoFps == 0) {
                videoFps = 12;
            }
        }
        else if (nodeName.compare("usedefaultoutputfile") == 0) {
            QString tmp = currElement.text();
            if (1 == tmp.toInt()) {
                useDefaultOutputFile = true;
            }
            else {
                useDefaultOutputFile = false;
            }
        }
        else if (nodeName.compare("defaultoutputfilename") == 0) {
            QString tmp = currElement.text();
            defaultOutputFileName.clear();
            defaultOutputFileName.append(tmp);
        }

        currElement = currElement.nextSiblingElement();
    }

    qDebug() << "AnimationProject::readSettingsFromProject --> End";
    return true;
}


bool AnimationProject::saveSettingsToProject(QDomDocument &doc, QDomElement &settingsNode)
{
    qDebug() << "AnimationProject::saveSettingsToProject --> Start";

    // Save project parameter

    // Save recording mode parameter
    QDomElement recordingModeElement = doc.createElement("recordingmode");
    QDomText recordingModeText = doc.createTextNode(QString("%1").arg(recordingMode));
    recordingModeElement.appendChild(recordingModeText);
    settingsNode.appendChild(recordingModeElement);

    // Save video source parameter
    QDomElement videoSourceElement = doc.createElement("videosource");
    QDomText videoSourceText = doc.createTextNode(QString("%1").arg(videoSource));
    videoSourceElement.appendChild(videoSourceText);
    settingsNode.appendChild(videoSourceElement);

    // Save video resolution parameter
    QDomElement resolutionElement = doc.createElement("videoresolution");
    QDomText resolutionText = doc.createTextNode(QString("%1").arg(videoResolution));
    resolutionElement.appendChild(resolutionText);
    settingsNode.appendChild(resolutionElement);

    // Save mixing mode parameter
    QDomElement mixingModeElement = doc.createElement("mixmode");
    QDomText mixingModeText = doc.createTextNode(QString("%1").arg(mixMode));
    mixingModeElement.appendChild(mixingModeText);
    settingsNode.appendChild(mixingModeElement);

    // Save mix count parameter
    QDomElement mixCountElement = doc.createElement("mixcount");
    QDomText mixCountText = doc.createTextNode(QString("%1").arg(mixCount));
    mixCountElement.appendChild(mixCountText);
    settingsNode.appendChild(mixCountElement);

    // Save playback count parameter
    QDomElement playbackCountElement = doc.createElement("playbackcount");
    QDomText playbackCountText = doc.createTextNode(QString("%1").arg(playbackCount));
    playbackCountElement.appendChild(playbackCountText);
    settingsNode.appendChild(playbackCountElement);

    // Save unit mode parameter
    QDomElement unitModeElement = doc.createElement("unitmode");
    QDomText unitModeText = doc.createTextNode(QString("%1").arg(unitMode));
    unitModeElement.appendChild(unitModeText);
    settingsNode.appendChild(unitModeElement);

    // Save overlay intensity parameter
    QDomElement overlayIntensityElement = doc.createElement("overlayintensity");
    QDomText overlayIntensityText = doc.createTextNode(QString("%1").arg(overlayIntensity));
    overlayIntensityElement.appendChild(overlayIntensityText);
    settingsNode.appendChild(overlayIntensityElement);

    // Save image import preferences

    // Save imageFormat parameter
    QDomElement ifElement = doc.createElement("imageformat");
    QDomText ifText = doc.createTextNode(QString("%1").arg(imageFormat));
    ifElement.appendChild(ifText);
    settingsNode.appendChild(ifElement);

    // Save imageQuality parameter
    QDomElement icElement = doc.createElement("imagequality");
    QDomText icText = doc.createTextNode(QString("%1").arg(imageQuality));
    icElement.appendChild(icText);
    settingsNode.appendChild(icElement);

    // Save imageSize parameter
    QDomElement isElement = doc.createElement("imagesize");
    QDomText isText = doc.createTextNode(QString("%1").arg(imageSize));
    isElement.appendChild(isText);
    settingsNode.appendChild(isElement);

    // Save imageTransformation parameter
    QDomElement itElement = doc.createElement("imagetransformation");
    QDomText itText = doc.createTextNode(QString("%1").arg(imageTransformation));
    itElement.appendChild(itText);
    settingsNode.appendChild(itElement);

    // Save imageAdjustment parameter
    QDomElement iaElement = doc.createElement("imageadjustment");
    QDomText iaText = doc.createTextNode(QString("%1").arg(imageAdjustment));
    iaElement.appendChild(iaText);
    settingsNode.appendChild(iaElement);

    // Save zoomValue parameter
    QDomElement zvElement = doc.createElement("zoomvalue");
    QDomText zvText = doc.createTextNode(QString("%1").arg(zoomValue));
    zvElement.appendChild(zvText);
    settingsNode.appendChild(zvElement);

    // Save liveViewFps parameter
    QDomElement lvfElement = doc.createElement("liveviewfps");
    QDomText lvfText = doc.createTextNode(QString("%1").arg(liveViewFps));
    lvfElement.appendChild(lvfText);
    settingsNode.appendChild(lvfElement);

    // Save video export preferences

    // Save encoderApplication parameter
    QDomElement eaElement = doc.createElement("encoderapplication");
    QDomText eaText = doc.createTextNode(QString("%1").arg(encoderApplication));
    eaElement.appendChild(eaText);
    settingsNode.appendChild(eaElement);

    // Save videoFormat parameter
    QDomElement vfElement = doc.createElement("videoformat");
    QDomText vfText = doc.createTextNode(QString("%1").arg(videoFormat));
    vfElement.appendChild(vfText);
    settingsNode.appendChild(vfElement);

    // Save videoSize parameter
    QDomElement vsElement = doc.createElement("videosize");
    QDomText vsText = doc.createTextNode(QString("%1").arg(videoSize));
    vsElement.appendChild(vsText);
    settingsNode.appendChild(vsElement);

    // Save videoFps parameter
    QDomElement fpsElement = doc.createElement("videofps");
    QDomText fpsText = doc.createTextNode(QString("%1").arg(videoFps));
    fpsElement.appendChild(fpsText);
    settingsNode.appendChild(fpsElement);

    // Save useDefaultOutputFile parameter
    QDomElement udofElement = doc.createElement("usedefaultoutputfile");
    QDomText udofText = doc.createTextNode(QString("%1").arg((true == useDefaultOutputFile ? 1 : 0)));
    udofElement.appendChild(udofText);
    settingsNode.appendChild(udofElement);

    // Save defaultOutputFileName parameter
    QDomElement dofnElement = doc.createElement("defaultoutputfilename");
    QDomText dofnText = doc.createTextNode(defaultOutputFileName);
    dofnElement.appendChild(dofnText);
    settingsNode.appendChild(dofnElement);

    qDebug() << "AnimationProject::saveSettingsToProject --> End";
    return true;
}


/**************************************************************************
 * Scene functions
 **************************************************************************/

int AnimationProject::getActiveSceneIndex() const
{
    return activeSceneIndex;
}


int AnimationProject::getSceneIndex(Scene* scene)
{
    int index;

    index = scenes.indexOf(scene);
    Q_ASSERT(index != -1);

    return index;
}


void AnimationProject::setActiveScene(Scene *newScene)
{
    qDebug() << "AnimationProject::setActiveScene --> Start";

    int sceneSize = scenes.size();
    int sceneIndex;

    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        if (scenes[sceneIndex] == newScene) {
            break;
        }
    }

    Q_ASSERT(sceneIndex != sceneSize);

    if (sceneIndex != activeSceneIndex) {
        this->activeSceneIndex = sceneIndex;
    }

    qDebug() << "AnimationProject::setActiveScene --> End";
}


void AnimationProject::setActiveSceneIndex(int sceneIndex)
{
    qDebug() << "AnimationProject::setActiveSceneIndex --> Start";

    // The new scene index can be the same as the active scene index
    // if the active scene is removed from the scene vector
    // Q_ASSERT(sceneIndex != activeSceneIndex)

    this->activeSceneIndex = sceneIndex;

    qDebug() << "AnimationProject::setActiveSceneIndex --> End";
}


Scene *AnimationProject::getActiveScene()
{
    Q_ASSERT(activeSceneIndex >= 0);

    return scenes[activeSceneIndex];
}


int AnimationProject::getSceneSize() const
{
    return scenes.size();
}


int AnimationProject::getSceneTakeSize(int sceneIndex) const
{
    return scenes[sceneIndex]->getTakeSize();
}


int AnimationProject::getSceneExposureSize(int sceneIndex) const
{
    return scenes[sceneIndex]->getExposureSize();
}


int AnimationProject::getSceneTakeExposureSize(int sceneIndex, int takeIndex) const
{
    return scenes[sceneIndex]->getTake(takeIndex)->getExposureSize();
}


Scene *AnimationProject::getScene(int sceneIndex)
{
    if (sceneIndex > -1 && sceneIndex < scenes.size()) {
        return scenes[sceneIndex];
    }
    return NULL;
}


void AnimationProject::addScene(const QString &sceneDescription)
{
    qDebug() << "AnimationProject::addScene --> Start";

    Scene *scene = new Scene(this);

    scene->setId(QString("%1").arg(nextSceneIndex, 3, 10, QLatin1Char('0')));
    this->nextSceneIndex++;

    if (!sceneDescription.isEmpty()) {
        scene->setDescription(sceneDescription);
    }

    scenes.append(scene);

    qDebug() << "AnimationProject::addScene --> End";
}


void AnimationProject::addScene(Scene *scene)
{
    qDebug() << "AnimationProject::addScene --> Start";

    scenes.append(scene);

    qDebug() << "AnimationProject::addScene --> End";
}


void AnimationProject::insertScene(int sceneIndex, const QString &sceneDescription)
{
    qDebug() << "AnimationProject::insertScene --> Start";

    Q_ASSERT(sceneIndex > -1);
    Q_ASSERT(sceneIndex < getSceneSize());

    Scene *scene = new Scene(this);

    scene->setId(QString("%1").arg(nextSceneIndex, 3, 10, QLatin1Char('0')));
    this->nextSceneIndex++;

    if (!sceneDescription.isEmpty()) {
        scene->setDescription(sceneDescription);
    }

    scenes.insert(sceneIndex, scene);
    if (sceneIndex <= activeSceneIndex) {
        setActiveSceneIndex(activeSceneIndex+1);
    }

    qDebug() << "AnimationProject::insertScene --> End";
}


void AnimationProject::insertScene(int sceneIndex, Scene *scene)
{
    qDebug() << "AnimationProject::insertScene --> Start";

    Q_ASSERT(sceneIndex > -1);
    Q_ASSERT(sceneIndex < getSceneSize());

    scenes.insert(sceneIndex, scene);
    if (sceneIndex <= activeSceneIndex) {
        setActiveSceneIndex(activeSceneIndex+1);
    }

    qDebug() << "AnimationProject::insertScene --> End";
}


void AnimationProject::moveScene(int sceneIndex, int movePosition)
{
    if (sceneIndex != movePosition) {
        this->setActiveSceneIndex(-1);

        Scene *scene = scenes[sceneIndex];
        scenes.remove(sceneIndex);
        scenes.insert(movePosition, scene);

        // this->notifyMoveScene(sceneIndex, movePosition);
    }
}

/*
void AnimationProject::removeActiveScene()
{
    if (activeSceneIndex < 0) {
        return;
    }

    int sceneIndex = activeSceneIndex;

    if (sceneIndex < getSceneSize() - 1) {
        activeSceneIndex = sceneIndex + 1;
    } else {
        activeSceneIndex = sceneIndex - 1;
    }

    scenes.remove(sceneIndex);
}
*/

Scene *AnimationProject::removeScene(int sceneIndex)
{
    qDebug() << "AnimationProject::removeScene --> Start";

    Q_ASSERT(sceneIndex != activeSceneIndex);

    if (sceneIndex < activeSceneIndex) {
        // Scene bevor the active scene will be removed
        setActiveSceneIndex(activeSceneIndex-1);
        Q_ASSERT(-1 < activeSceneIndex);
    }
    Scene *removedScene = scenes[sceneIndex];
    scenes.remove(sceneIndex);

    qDebug() << "AnimationProject::removeScene --> End";
    return removedScene;
}


bool AnimationProject::readScenesFromProject(QDomElement &animationNode)
{
    qDebug() << "AnimationProject::readScenesFromProject --> Start";

    QString activeSceneId;

    description.append(animationNode.attributeNode(QLatin1String("descr")).value());

    QDomElement currElement = animationNode.firstChildElement();

    while (!currElement.isNull()) {
        QString nodeName = currElement.nodeName();

        // The node is a scene node
        if (nodeName.compare("scene") == 0) {
            Scene *newScene = new Scene(this);
            scenes.append(newScene);

            if (!newScene->readDataFromProject(currElement)) {
                qDebug() << "AnimationProject::readScenesFromProject --> End (false)";
                return false;
            }
        }
        else if (nodeName.compare("activescene") == 0) {
            activeSceneId.clear();
            activeSceneId.append(currElement.text());
        }
        else if (currElement.nodeName().compare("nextscene") == 0) {
            nextSceneIndex = currElement.text().toUInt();
        }

        currElement = currElement.nextSiblingElement();
    }

    // Search the active scene with the readed id.
    int sceneSize = scenes.size();
    for ( int sceneIndex = 0; sceneIndex < sceneSize; sceneIndex++) {
        if (scenes[sceneIndex]->getId().compare(activeSceneId) == 0) {
            activeSceneIndex = sceneIndex;
            break;
        }
    }

    // Compatibility for old project files.
    if (nextSceneIndex == 0) {
        nextSceneIndex = sceneSize;
    }

    qDebug() << "AnimationProject::readScenesFromProject --> End";
    return true;
}


bool AnimationProject::saveScenesToProject(QDomDocument &doc, QDomElement &animationNode)
{
    qDebug() << "AnimationProject::saveScenesToProject --> Start";

    unsigned int  sceneSize = scenes.size();
    unsigned int  sceneIndex;
    QDomElement   sceneElement;

    // Save project parameter
    animationNode.setAttribute("descr", description);

    if (isAnimationChanges()) {
        // The Animation is changed, rearrange the frames

        // Removes frames which already are saved. Had to do this to prevent
        // frames to overwrite each other.
        for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
            Scene *scene = scenes[sceneIndex];
            unsigned int takeSize = scene->getTakeSize();
            for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
                Take *take = scene->getTake(takeIndex);
                unsigned int exposureSize = take->getExposureSize();
                for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                    Exposure *exposure = take->getExposure(exposureIndex);
                    exposure->moveToTemp(false);
                }
            }
        }

        // Delete all image file that stay in the image directory
        // (All files that deleted from the project.)
        QString     imagePath = this->getOldImagePath();
        QDir        imageDir(imagePath);
        QStringList fileNameList = imageDir.entryList(QDir::Files);
        for (int fileIndex = 0 ; fileIndex < fileNameList.count() ; fileIndex++) {
            imageDir.remove(fileNameList[fileIndex]);
        }

        nextTotalExposureIndex = 0;

        // Move all frames to the project directory with a new indexed name
        for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
            Scene *scene = scenes[sceneIndex];

            // Create a new id for the scene
            scene->setId(QString("%1").arg((sceneIndex), 3, 10, QLatin1Char('0')));

            unsigned int takeSize = scene->getTakeSize();
            for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
                Take *take = scene->getTake(takeIndex);

                // Create a new id for the take
                take->setId(QString("%1").arg((takeIndex), 2, 10, QLatin1Char('0')));

                unsigned int exposureSize = take->getExposureSize();
                for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                    Exposure *exposure = take->getExposure(exposureIndex);

                    // Create a new id for the Exposure
                    exposure->setId(QString("%1").arg((exposureIndex), 4, 10, QLatin1Char('0')));

                    // Create a new file name for the frame

                    /* This version didn't work with ffmpeg encoder
                    QString newFrameName(scene->getId());
                    newFrameName.append('_');
                    newFrameName.append(take->getId());
                    newFrameName.append('_');
                    newFrameName.append(exposure->getId());
                    newFrameName.append(theFrame.mid(theFrame.lastIndexOf('.')));
                    */

                    QString oldFrameName = exposure->getImagePath();
                    int newTotalExposureIndex = getNextTotalExposureIndex();
                    QString newFrameName(QString("%1").arg(newTotalExposureIndex, 6, 10, QLatin1Char('0')));
                    newFrameName.append(oldFrameName.mid(oldFrameName.lastIndexOf('.')));

                    // Move frame file to project directory
                    exposure->moveToProject(newFrameName);
                }
            }
        }

        // Delete all image file that stay in the temp directory
        // (All files that deleted from the project.)
        QString tempPath = this->getAppTempDirName();
        QDir tempDir(tempPath);
        fileNameList = tempDir.entryList(QDir::Files);
        for (int fileIndex = 0 ; fileIndex < fileNameList.count() ; fileIndex++) {
            tempDir.remove(fileNameList[fileIndex]);
        }
    }

    // Reset the temp file index
    Exposure::tempNumber = 0;

    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        // Scenes
        sceneElement = doc.createElement("scene");
        animationNode.appendChild(sceneElement);

        if (!scenes[sceneIndex]->saveDataToProject(doc, sceneElement)) {
            qDebug() << "AnimationProject::saveScenesToProject --> End (false)";
            return false;
        }
    }

    if (activeSceneIndex >= 0) {
        // Save activeSceneIndex parameter after the scenes, because the names of
        // the scenes are modifyed during the writing of the project file.
        QDomElement asiElement = doc.createElement("activescene");
        QDomText asiText = doc.createTextNode(scenes[activeSceneIndex]->getId());
        asiElement.appendChild(asiText);
        animationNode.appendChild(asiElement);

        // Save nextSceneIndex parameter.
        QDomElement leiElement = doc.createElement("nextscene");
        QDomText leiText = doc.createTextNode(QString("%1").arg(sceneSize));
        leiElement.appendChild(leiText);
        animationNode.appendChild(leiElement);
    }

    qDebug() << "AnimationProject::saveScenesToProject --> End";
    return true;
}


bool AnimationProject::saveAsScenesToProject(QDomDocument &doc, QDomElement &animationNode)
{
    qDebug() << "AnimationProject::saveScenesToProject --> Start";

    unsigned int  sceneSize = scenes.size();
    unsigned int  sceneIndex;
    QDomElement   sceneElement;

    // Save project parameter
    animationNode.setAttribute("descr", description);

    // Removes frames which already are saved. Had to do this to prevent
    // frames to overwrite each other.
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        Scene *scene = scenes[sceneIndex];
        unsigned int takeSize = scene->getTakeSize();
        for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
            Take *take = scene->getTake(takeIndex);
            unsigned int exposureSize = take->getExposureSize();
            for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                Exposure *exposure = take->getExposure(exposureIndex);
                exposure->copyToTemp();
            }
        }
    }

    nextTotalExposureIndex = 0;

    // Move all frames to the new project directory with a new indexed name
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        Scene *scene = scenes[sceneIndex];

        // Create a new id for the scene
        scene->setId(QString("%1").arg((sceneIndex), 3, 10, QLatin1Char('0')));

        unsigned int takeSize = scene->getTakeSize();
        for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
            Take *take = scene->getTake(takeIndex);

            // Create a new id for the take
            take->setId(QString("%1").arg((takeIndex), 2, 10, QLatin1Char('0')));

            unsigned int exposureSize = take->getExposureSize();
            for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                Exposure *exposure = take->getExposure(exposureIndex);

                // Create a new id for the Exposure
                exposure->setId(QString("%1").arg((exposureIndex), 4, 10, QLatin1Char('0')));

                // Create a new file name for the frame

                /* This version didn't work with ffmpeg encoder
                QString newFrameName(scene->getId());
                newFrameName.append('_');
                newFrameName.append(take->getId());
                newFrameName.append('_');
                newFrameName.append(exposure->getId());
                newFrameName.append(theFrame.mid(theFrame.lastIndexOf('.')));
                */

                QString oldFrameName = exposure->getImagePath();
                int newTotalExposureIndex = getNextTotalExposureIndex();
                QString newFrameName(QString("%1").arg(newTotalExposureIndex, 6, 10, QLatin1Char('0')));
                newFrameName.append(oldFrameName.mid(oldFrameName.lastIndexOf('.')));

                // Move frame file to project directory
                exposure->moveToProject(newFrameName);
            }
        }
    }

    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        // Scenes
        sceneElement = doc.createElement("scene");
        animationNode.appendChild(sceneElement);

        if (!scenes[sceneIndex]->saveDataToProject(doc, sceneElement)) {
            qDebug() << "AnimationProject::saveScenesToProject --> End (false)";
            return false;
        }
    }

    if (activeSceneIndex >= 0) {
        // Save activeSceneIndex parameter after the scenes, because the names of
        // the scenes are modifyed during the writing of the project file.
        QDomElement asiElement = doc.createElement("activescene");
        QDomText asiText = doc.createTextNode(scenes[activeSceneIndex]->getId());
        asiElement.appendChild(asiText);
        animationNode.appendChild(asiElement);

        // Save nextSceneIndex parameter.
        QDomElement leiElement = doc.createElement("nextscene");
        QDomText leiText = doc.createTextNode(QString("%1").arg(sceneSize));
        leiElement.appendChild(leiText);
        animationNode.appendChild(leiElement);
    }

    qDebug() << "AnimationProject::saveScenesToProject --> End";
    return true;
}


bool AnimationProject::convertImages(int newFormat, int newQuality)
{
    qDebug() << "AnimationProject::saveScenesToProject --> Start";

    unsigned int  sceneSize = scenes.size();
    unsigned int  sceneIndex;

    // Removes frames which already are saved. Had to do this to prevent
    // frames to overwrite each other.
    for (sceneIndex = 0; sceneIndex < sceneSize; ++sceneIndex) {
        Scene *scene = scenes[sceneIndex];
        unsigned int takeSize = scene->getTakeSize();
        for (unsigned int takeIndex = 0; takeIndex < takeSize; ++takeIndex) {
            Take *take = scene->getTake(takeIndex);
            unsigned int exposureSize = take->getExposureSize();
            for (unsigned int exposureIndex = 0; exposureIndex < exposureSize; ++exposureIndex) {
                Exposure *exposure = take->getExposure(exposureIndex);
                exposure->convertToTemp(newFormat, newQuality);
            }
        }
    }
    incAnimationChanges();

    setImageFormat(newFormat);
    setImageQuality(newQuality);

    /* Delete all image file that stay in the image directory
    // (All files that deleted from the project.)
    QString     imagePath = this->getOldImagePath();
    QDir        imageDir(imagePath);
    QStringList fileNameList = imageDir.entryList(QDir::Files);
    for (int fileIndex = 0 ; fileIndex < fileNameList.count() ; fileIndex++) {
        imageDir.remove(fileNameList[fileIndex]);
    }
    */

    return true;
}

/**************************************************************************
 * Sound functions
 **************************************************************************/

int AnimationProject::addSoundToScene(unsigned int sceneIndex, const QString &sound, const QString &soundName)
{
    qDebug() << "AnimationProject::addSoundToScene --> Start";

    ++soundsNumber;
    int ret = scenes[sceneIndex]->addSound(sound, soundName);
    if (ret == -1) {
        frontend->showWarning(tr("Add Sound"),
            tr("Cannot open the selected audio file for reading.\n"
            "Check that you have the right permissions set.\n"
            "The animation will be run without sound if you\n"
            "choose to play."));
        --soundsNumber;
    } else if (ret == -2) {
        frontend->showWarning(tr("Add Sound"),
            tr("The selected audio file is not valid within the\n"
            "given audio format. The animation will be run\n"
            "without sound if you choose to play."));
        --soundsNumber;
    }

    qDebug() << "AnimationProject::addSoundToScene --> End";
    return ret;
}


void AnimationProject::removeSoundFromScene(unsigned int sceneIndex, unsigned int soundIndex)
{
    scenes[sceneIndex]->removeSound(soundIndex);
    --soundsNumber;
}


void AnimationProject::playSound(int sceneIndex)
{
    if (isAudioDriverInitialized) {
        scenes[sceneIndex]->playSounds(audioDriver);
    }
    // notifyPlaySound(sceneIndex);
}


/**************************************************************************
 * Take functions
 **************************************************************************/

int AnimationProject::getActiveTakeIndex() const
{
    Q_ASSERT(activeSceneIndex > -1);

    return scenes[activeSceneIndex]->getActiveTakeIndex();
}


void AnimationProject::setActiveTakeIndex(int takeIndex)
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *activeScene = scenes[activeSceneIndex];

    activeScene->setActiveTakeIndex(takeIndex);
}


Take *AnimationProject::getActiveTake()
{
    Q_ASSERT(activeSceneIndex >= 0);

    return getTake(activeSceneIndex, getActiveTakeIndex());
}


void AnimationProject::addTake(int sceneIndex, const QString &takeDescription)
{
    Scene *scene = getScene(sceneIndex);
    scene->addTake(takeDescription);
}


void AnimationProject::addTake(int sceneIndex, Take *take)
{
    Scene *scene = getScene(sceneIndex);
    scene->addTake(take);
}


void AnimationProject::insertTake(int sceneIndex, int takeIndex, const QString &takeDescription)
{
    Scene *scene = getScene(sceneIndex);
    scene->insertTake(takeIndex, takeDescription);
}


void AnimationProject::insertTake(int sceneIndex, int takeIndex, Take *take)
{
    Scene *scene = getScene(sceneIndex);
    scene->insertTake(takeIndex, take);
}


void AnimationProject::moveTake(int fromSceneIndex, int fromTakeIndex, int toSceneIndex, int toTakeIndex)
{
    Q_ASSERT(fromSceneIndex > -1);
    Q_ASSERT(toSceneIndex > -1);

    // TODO: Implement the move from one scene to another
    Q_ASSERT(fromSceneIndex == toSceneIndex);

    Scene *fromScene = scenes[fromSceneIndex];

    fromScene->moveTake(fromTakeIndex, toTakeIndex);
}

/*
Take *AnimationProject::removeActiveTake()
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene *activeScene = scenes[activeSceneIndex];

    Take *removedTake = activeScene->removeActiveTake();

    return removedTake;
}
*/

Take *AnimationProject::removeTake(int sceneIndex, int takeIndex)
{
    Scene *scene = scenes[sceneIndex];

    Take *removedTake = scene->removeTake(takeIndex);

    return removedTake;
}


int AnimationProject::getTakeExposureSize(unsigned int sceneIndex, unsigned int takeIndex) const
{
    return scenes[sceneIndex]->getTake(takeIndex)->getExposureSize();
}


Take *AnimationProject::getTake(int sceneIndex, int takeIndex)
{
    if (takeIndex > -1 && takeIndex < int(scenes[sceneIndex]->getTakeSize())) {
        return scenes[sceneIndex]->getTake(takeIndex);
    }
    return NULL;
}


/**************************************************************************
 * Exposure functions
 **************************************************************************/

int AnimationProject::getActiveExposureIndex() const
{
    if (-1 == activeSceneIndex) {
        return -1;
    }
    Scene *activeScene = scenes[activeSceneIndex];
    if (-1 == getActiveTakeIndex()) {
        return -1;
    }
    int activeExposureIndex = activeScene->getActiveExposureIndex();
    return activeExposureIndex;
}


void AnimationProject::setActiveExposureIndex(int exposureIndex)
{
    qDebug() << "AnimationProject::setActiveExposureIndex --> Start";

    scenes[this->activeSceneIndex]->setActiveExposureIndex(exposureIndex);

    qDebug() << "AnimationProject::setActiveExposureIndex --> End";
}


unsigned int AnimationProject::getTotalExposureSize() const
{
    unsigned int modelSize = 0;
    unsigned int size = scenes.size();
    for (unsigned int i = 0; i < size; ++i) {
        modelSize += scenes[i]->getExposureSize();
    }
    return modelSize;
}


Exposure *AnimationProject::getExposure(unsigned int sceneIndex,
                                        unsigned int takeIndex,
                                        unsigned int exposureIndex)
{
    // qDebug() << "AnimationProject::getExposure --> Start");

    Q_ASSERT(exposureIndex < scenes[sceneIndex]->getExposureSize());

    Exposure* exposure = scenes[sceneIndex]->getExposure(takeIndex, exposureIndex);

    // qDebug() << "AnimationProject::getExposure --> End");
    return exposure;
}


Exposure *AnimationProject::getActiveExposure()
{
    if (0 > activeSceneIndex) {
        // return a empty exposure
        return NULL;
    }

    int activeTakeIndex = getActiveTakeIndex();
    if (0 > activeTakeIndex) {
        // return a empty exposure
        return NULL;
    }

    int activeExposureIndex = getActiveExposureIndex();
    if (0 > activeExposureIndex) {
        // return a empty exposure
        return NULL;
    }

    return getExposure(activeSceneIndex, getActiveTakeIndex(), getActiveExposureIndex());
}


void AnimationProject::addExposure(int sceneIndex,
                                   int takeIndex,
                                   const QString &fileName,
                                   int location)
{
    Take  *take = getTake(sceneIndex, takeIndex);

    take->addExposure(fileName, location);
}


void AnimationProject::addExposure(int sceneIndex,
                                   int takeIndex,
                                   Exposure *exposure)
{
    Take  *take = getTake(sceneIndex, takeIndex);

    take->addExposure(exposure);
}


void AnimationProject::insertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex,
                                      const QString &fileName,
                                      int location)
{
    Take  *take = getTake(sceneIndex, takeIndex);

    take->insertExposure(exposureIndex, fileName, location);
}


void AnimationProject::insertExposure(int sceneIndex,
                                      int takeIndex,
                                      int exposureIndex,
                                      Exposure *exposure)
{
    Take  *take = getTake(sceneIndex, takeIndex);

    take->insertExposure(exposureIndex, exposure);
}


Exposure *AnimationProject::removeActiveExposure()
{
    Q_ASSERT(activeSceneIndex > -1);

    Scene    *activeScene = scenes[activeSceneIndex];
    Take     *activeTake = activeScene->getTake(activeScene->getActiveTakeIndex());
    Exposure *exposure = activeTake->removeActiveExposure();

    return exposure;
}


Exposure *AnimationProject::removeExposure(int sceneIndex,
                                           int takeIndex,
                                           int exposureIndex)
{
    Scene    *scene = scenes[sceneIndex];
    Exposure *exposure = scene->removeExposure(takeIndex, exposureIndex);

    return exposure;
}


unsigned int AnimationProject::getNextTotalExposureIndex()
{
    unsigned int ret = nextTotalExposureIndex;
    nextTotalExposureIndex++;

    return ret;
}


bool AnimationProject::getModifyedExposure(const QString &filePath,
                                           int &modSceneIndex,
                                           int &modTakeIndex,
                                           int &modExposureIndex)
{
    qDebug() << "AnimationProject::getModifyedExposure --> Start";

    int sceneSize = scenes.size();

    for (modSceneIndex = 0; modSceneIndex < sceneSize; ++modSceneIndex) {
        if (scenes[modSceneIndex]->getModifyedExposure(filePath, modTakeIndex, modExposureIndex)) {
            // Exposure found
            qDebug() << "AnimationProject::getModifyedExposure --> End";
            return true;
        }
    }

    modSceneIndex = -1;

    qDebug() << "AnimationProject::getModifyedExposure --> End (Error)";
    return false;
}

/**************************************************************************
 * Private functions
 **************************************************************************/

/*
void AnimationProject::loadSavedScenes()
{
    qDebug() << "AnimationProject::loadSavedScenes --> Start");

    unsigned int numElem = scenes.size();
    for (unsigned int i = 0; i < numElem; ++i) {
        notifyNewScene(i);
    }
    setActiveSceneIndex(numElem - 1);

    qDebug() << "AnimationProject::loadSavedScenes --> End");
}


void AnimationProject::activateScene()
{
    qDebug() << "AnimationProject::activateScene --> Start");

    this->notifyActivateScene();

    qDebug() << "AnimationProject::activateScene --> End");
}
*/
