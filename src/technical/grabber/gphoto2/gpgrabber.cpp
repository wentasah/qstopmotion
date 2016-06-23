/******************************************************************************
 *  Copyright (C) 2010-2016 by                                                *
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

#include "gpgrabber.h"

#include <QByteArray>
#include <QDebug>
#include <QDir>
// #include <QtGlobal>
#include <QApplication>

#include "gpcontroller.h"
#include "technical/grabber/imagegrabberdevice.h"
#include "technical/util.h"

// Include files of the gphoto library
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


GphotoGrabber::GphotoGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug() << "GphotoGrabber::Constructor --> Start";

    gphotoCamera = NULL;
    gphotoContext = NULL;

    canCaptureImage = false;
    canCaptureAudio = false;
    canCaptureVideo = false;
    canCapturePreview = false;
    canConfigure = false;

    QString rootDir;
    rootDir.append(frontend->getUserDirName());
    rootDir.append("/");
    rootDir.append("capturedfile.jpg");
    filePath.append(rootDir);

    gphotoContext = gp_context_new();
    gphotoErrorId = gp_log_add_func(GP_LOG_ERROR, (GPLogFunc)errorDumper, NULL);

    openCamera();

    // Read the summary of the camera
    int        ret;
    CameraText sum;

    ret = gp_camera_get_summary(gphotoCamera, &sum, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_summary: " << gp_result_as_string(ret);
    }
    else {
        QStringList sumList = QString(sum.text).split('\n');
        for (int i = 0; i < sumList.size(); i++) {
            qDebug() << "GphotoGrabber::Constructor --> Summary: " << sumList.at(i);
        }
        manufacturer.append(sumList.at(0).split(' ').at(1));
        model.append(sumList.at(1).split(' ').at(1));
    }

    // Read the manual of the camera
    CameraText manual;

    ret = gp_camera_get_manual(gphotoCamera, &manual, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_manual: " << gp_result_as_string(ret);
    }
    else {
        qDebug() << "GphotoGrabber::Constructor --> Manual: " << QString(manual.text);
    }

    // Read the about of the camera driver
    CameraText about;

    ret = gp_camera_get_about(gphotoCamera, &about, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_about: " << gp_result_as_string(ret);
    }
    else {
        qDebug() << "GphotoGrabber::Constructor --> About: " << QString(about.text);
    }

    qDebug() << "GphotoGrabber::Constructor --> End";
}


GphotoGrabber::~GphotoGrabber()
{
    qDebug() << "GphotoGrabber::Destructor --> Start (Empty)";

    closeCamera();

    qDebug() << "GphotoGrabber::Destructor --> End";
}


Camera* GphotoGrabber::getCamera()
{
    return gphotoCamera;
}


GPContext* GphotoGrabber::getContext()
{
    return gphotoContext;
}


bool GphotoGrabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug() << "GphotoGrabber::initialization --> Start";

    GphotoController *deviceController = NULL;
    PreferencesTool  *pref = frontend->getPreferences();
    CameraWidget     *gphotoConfig = NULL;
    CameraAbilities   abilities;
    int               ret;
    bool              returnValue = true;
    int               value;

    if (!isInitSuccess) {
        qDebug() << "GphotoGrabber::initialization --> End (Error)";
        return false;
    }

    ImageGrabberDevice *device = NULL;

    qDebug() << "GphotoGrabber::initialization --> Add gphoto2 device";

    device = new ImageGrabberDevice(0,
                                    "",
                                    QString("%1 %2").arg(manufacturer).arg(model),
                                    ImageGrabberDevice::gphoto2Source,
                                    ImageGrabberDevice::video_x_none);
    devices.append(device);

    readImageSettings(device);

    // Create grabber controller
    if (pref->getIntegerPreference("preferences", "gphoto2controller", value) == false) {
        value = false;
    }
    if ((int)true == value) {
        deviceController = new GphotoController(0);
        if (deviceController->initialization(this, device))
        {
            device->setController(deviceController);
        }
        else
        {
            delete deviceController;
            deviceController = NULL;
        }

    }

    isInitSuccess = true;

    ret = gp_camera_get_config(gphotoCamera, &gphotoConfig, gphotoContext);
    if (ret < GP_OK) {
        qDebug() << "GphotoGrabber::initialization --> Error End camera_get_config failed: " << gp_result_as_string(ret);
        returnValue = false;
        goto out2;
    }

    if (manufacturer.compare("canon", Qt::CaseInsensitive) == 0) {
        if (false == canonEnableCapture(gphotoCamera, gphotoContext, gphotoConfig)) {
            returnValue = false;
            goto out;
        }

        populateWithConfigs(gphotoConfig);
    }

    // Abilities
    qDebug() << "GphotoGrabber::initialization --> Checking camera abilities";
    ret = gp_camera_get_abilities (gphotoCamera, &abilities);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::initialization --> Error End gp_camera_get_abilities: " << gp_result_as_string(ret);
        returnValue = false;
        goto out;
    }

    if (ret >= GP_OK)
    {
        // rewrite model
        qDebug() << "GphotoGrabber::initialization --> Abilities.model: " << QString(abilities.model);
        switch (abilities.status)
        {
        case GP_DRIVER_STATUS_PRODUCTION:
            qDebug() << "GphotoGrabber::initialization --> Abilities.driver_status: production";
            break;
        case GP_DRIVER_STATUS_TESTING:
            qDebug() << "GphotoGrabber::initialization --> Abilities.driver_status: testing";
            break;
        case GP_DRIVER_STATUS_EXPERIMENTAL:
            qDebug() << "GphotoGrabber::initialization --> Abilities.driver_status: experimental";
            break;
        case GP_DRIVER_STATUS_DEPRECATED:
            qDebug() << "GphotoGrabber::initialization --> Abilities.driver_status: deprecated";
            break;
        default:
            qDebug() << "GphotoGrabber::initialization --> Abilities.driver_status: unknown";
        }

        qDebug() << "GphotoGrabber::initialization --> Abilities.library: " << abilities.library;
        qDebug() << "GphotoGrabber::initialization --> Abilities.operations: " << abilities.operations;

        if ((abilities.operations & GP_OPERATION_CAPTURE_IMAGE) > 0 ) {
            canCaptureImage = true;
            qDebug() << "GphotoGrabber::initialization --> Camera can capture images!";
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_VIDEO) > 0) {
            canCaptureAudio = true;
            qDebug() << "GphotoGrabber::initialization --> Camera can capture videos!";
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_AUDIO) > 0) {
            canCaptureVideo = true;
            qDebug() << "GphotoGrabber::initialization --> Camera can capture audio!";
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_PREVIEW) > 0) {
            canCapturePreview = true;
            qDebug() << "GphotoGrabber::initialization --> Camera can capture preview images!";
        }
        if ((abilities.operations & GP_OPERATION_CONFIG) > 0) {
            canConfigure = true;
            qDebug() << "GphotoGrabber::initialization --> Camera can be captured!";
        }
    }

out:
    gp_widget_free(gphotoConfig);

out2:

    qDebug() << "GphotoGrabber::initialization --> device count: " << devices.size();

    qDebug() << "GphotoGrabber::initialization --> End (" << returnValue << ")";

    return returnValue;
}


bool GphotoGrabber::setUp()
{
    qDebug() << "GphotoGrabber::setUp --> Start";

    if (!isInitSuccess) {
        qDebug() << "GphotoGrabber::setUp --> End (Error)";
        return false;
    }

    openCamera();

    qDebug() << "GphotoGrabber::setUp --> End";

    return true;
}


bool GphotoGrabber::tearDown()
{
    qDebug() << "GphotoGrabber::tearDown --> Start";

    closeCamera();

    qDebug() << "GphotoGrabber::tearDown --> End";
    return true;
}


bool GphotoGrabber::grab()
{
    return true;
}


const QImage GphotoGrabber::getLiveImage()
{
    qDebug() << "GphotoGrabber::getLiveImage --> Start";

    CameraFile    *cf;
    unsigned long  size = 0;
    const char    *data = NULL;
    int            ret;

    if (false == canCapturePreview) {
        return getRawImage();
    }

    openCamera();

    ret = gp_file_new(&cf);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error End gp_file_new: " << gp_result_as_string(ret);
        return QImage();
    }

    ret = gp_camera_capture_preview(gphotoCamera, cf, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error End gp_camera_capture_preview: " << gp_result_as_string(ret);
        liveImage = QImage();
        goto LiveError;
    }

    // setImage(cf, a);

    ret = gp_file_get_data_and_size(cf, &data, &size);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error: gp_file_get_data_and_size: " << gp_result_as_string(ret);
        liveImage = QImage();
        goto LiveError;
    }

    if (size == 0 ) {
        liveImage = QImage();
        goto LiveError;
    }

    liveImage.loadFromData(QByteArray(data, size));

LiveError:
    ret = gp_file_free(cf);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error: gp_file_free: " << gp_result_as_string(ret);
    }

    while(1) {
        CameraEventType type;
        void*           eventData;

        ret = gp_camera_wait_for_event(gphotoCamera, 100, &type, &eventData, gphotoContext);
        if (GP_EVENT_TIMEOUT == type) {
            break;
        }
        else {
            if (GP_EVENT_CAPTURE_COMPLETE == type) {
                qDebug() << "GphotoGrabber::getLiveImage --> Event: Capture completed!";
            }
            else {
                if (GP_EVENT_UNKNOWN != type) {
                    qDebug() << "GphotoGrabber::getLiveImage --> Unexpected event: " << (int)type;
                }
            }
        }
    }

    qDebug() << "GphotoGrabber::getLiveImage --> End";
    return liveImage;
}


const QImage GphotoGrabber::getRawImage()
{
    qDebug() << "GphotoGrabber::getRawImage --> Start";

    CameraFilePath  cfp;
    CameraFile     *cf;
    unsigned long   size = 0;
    const char     *data = NULL;
    int             ret;
    int             fdesc; // file descriptor

    // NOP: This gets overridden in the library to /capt0000.jpg
    strcpy(cfp.folder, "/");
    strcpy(cfp.name, "foo.jpg");

    // Taking Image
    ret = gp_camera_capture(gphotoCamera, GP_CAPTURE_IMAGE, &cfp, gphotoContext );
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_camera_capture: " << gp_result_as_string(ret);
        return QImage();
    }
    qDebug() << "GphotoGrabber::getRawImage --> Captured file: " << cfp.folder << "/" << cfp.name;

    // Download image from camera to imageFilename
    ret = gp_file_new(&cf);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_file_new_from_fd: " << gp_result_as_string(ret);
        close(fdesc);
        return QImage();
    }

    ret = gp_camera_file_get(gphotoCamera, cfp.folder, cfp.name, GP_FILE_TYPE_NORMAL, cf, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_camera_file_get: " << gp_result_as_string(ret);
        rawImage = QImage();
        goto RawError;
    }

    ret = gp_file_get_data_and_size(cf, &data, &size);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_file_get_data_and_size: " << gp_result_as_string(ret);
        rawImage = QImage();
        goto RawError;
    }

    if (size == 0 ) {
        rawImage = QImage();
        goto RawError;
    }

    // rawImage.load(QString(imageFilename));
    rawImage.loadFromData(QByteArray(data, size));

    // Delete image on camera
    ret = gp_camera_file_delete(gphotoCamera, cfp.folder, cfp.name, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_camera_file_delete: " << gp_result_as_string(ret);
        rawImage = QImage();
        goto RawError;
    }

RawError:
    ret = gp_file_free(cf);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_file_free: " << gp_result_as_string(ret);
    }

    while(1) {
        CameraEventType type;
        void*           eventData;

        ret = gp_camera_wait_for_event(gphotoCamera, 100, &type, &eventData, gphotoContext);
        if (GP_EVENT_TIMEOUT == type) {
            break;
        }
        else {
            if (GP_EVENT_CAPTURE_COMPLETE == type) {
                qDebug() << "GphotoGrabber::getRawImage --> Event: Capture completed!";
            }
            else {
                if (GP_EVENT_UNKNOWN != type) {
                    qDebug() << "GphotoGrabber::getRawImage --> Unexpected event: " << (int)type;
                }
            }
        }
    }

    // This was a test!
    // this->tearDown();
    // this->setUp();

    qDebug() << "GphotoGrabber::getRawImage --> End";
    return rawImage;
}


void GphotoGrabber::openCamera()
{
    qDebug() << "GphotoGrabber::openCamera --> Start";

    int ret;

    if (NULL != gphotoCamera) {
        qDebug() << "GphotoGrabber::openCamera --> End (Nothing to do!)";

        return;
    }

    gp_camera_new(&gphotoCamera);

    qDebug() << "GphotoGrabber::openCamera --> Camera init.  Takes a second or three...";
    ret = gp_camera_init(gphotoCamera, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::openCamera --> Error during camera initialization: " << gp_result_as_string(ret);
        isInitSuccess = false;
        return;
    }
    isInitSuccess = true;

    // canonEnableCapture(gphotoCamera, TRUE, gphotoContext);


    qDebug() << "GphotoGrabber::openCamera --> End";
}


void GphotoGrabber::closeCamera()
{
    qDebug() << "GphotoGrabber::closeCamera --> Start";

    int ret;

    if (NULL == gphotoCamera) {
        qDebug() << "GphotoGrabber::closeCamera --> End (Nothing to do!)";

        return;
    }

    ret = gp_camera_exit(gphotoCamera, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::closeCamera --> Error End gp_camera_exit: " << gp_result_as_string(ret);
    }

    ret = gp_camera_free(gphotoCamera);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::closeCamera --> Error End gp_camera_exit: " << gp_result_as_string(ret);
    }

    gphotoCamera = NULL;

    qDebug() << "GphotoGrabber::closeCamera --> End";
}


void GphotoGrabber::readImageSettings(ImageGrabberDevice *device)
{
    qDebug() << "GphotoGrabber::readImageSettings --> Start";

    int ret;

    CameraWidget* widget_root = NULL;
    CameraWidget* widget_main = NULL;
    CameraWidget* widget_imgsettings = NULL;
    CameraWidget* widget_imgsetting = NULL;
    int           widgetCount = 0;
    const char*      info = NULL;
    const char*      name = NULL;
    const char*      label = NULL;
    CameraWidgetType type;
    float            min, max, increment;
    int              choiceCount = 0;
    int              choiceIndex = 0;
    const char*      choice = NULL;
    char*            value;

    ret = gp_camera_get_config(this->getCamera(), &widget_root, this->getContext());
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::readImageSettings --> Error gp_camera_get_config: " << gp_result_as_string(ret);
    }

    ret = gp_widget_get_child_by_name(widget_root, "main", &widget_main);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_child_by_name(main): " << gp_result_as_string(ret);
    }

    ret = gp_widget_get_child_by_name(widget_main, "imgsettings", &widget_imgsettings);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_child_by_name(capturesettings): " << gp_result_as_string(ret);
    }

    ret = gp_widget_count_children(widget_imgsettings);
    if (ret < GP_OK) {
        qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_count_children: " << gp_result_as_string(ret);
    }
    widgetCount = ret;

    for (int widgetIndex = 0; widgetIndex < widgetCount; widgetIndex++) {

        ret = gp_widget_get_child(widget_imgsettings, widgetIndex, &widget_imgsetting);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_child: " << gp_result_as_string(ret);
        }
        else {
            ret = gp_widget_get_info(widget_imgsetting, &info);
            if (ret != GP_OK) {
                qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_info: " << gp_result_as_string(ret);
            }
            else {
                qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_info: " << info;
            }

            ret = gp_widget_get_name(widget_imgsetting, &name);
            if (ret != GP_OK) {
                qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_name: " << gp_result_as_string(ret);
            }
            else {
                qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_name: " << name;
            }

            ret = gp_widget_get_label(widget_imgsetting, &label);
            if (ret != GP_OK) {
                qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_label: " << gp_result_as_string(ret);
            }
            else {
                qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_label: " << label;
            }

            ret = gp_widget_get_type(widget_imgsetting, &type);
            if (ret != GP_OK) {
                qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_type: " << gp_result_as_string(ret);
            }
            else {
                qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_type: " << type;
            }

            switch (type) {
            case GP_WIDGET_RANGE:
                ret = gp_widget_get_range(widget_imgsetting, &min, &max, &increment);
                if (ret != GP_OK) {
                    qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_range: " << gp_result_as_string(ret);
                }
                else {
                    qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_range: min: " << min << " max: " << max << " increment: " << increment;
                }
                break;

            case GP_WIDGET_RADIO:
                ret = gp_widget_count_choices(widget_imgsetting);
                if (ret < GP_OK) {
                    qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_count_choices: " << gp_result_as_string(ret);
                }
                else {
                    choiceCount = ret;
                    qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_count_choices: count: " << choiceCount;

                    for (choiceIndex = 0; choiceIndex < choiceCount; choiceIndex++) {

                        ret = gp_widget_get_choice(widget_imgsetting, choiceIndex, &choice);
                        if (ret != GP_OK) {
                            qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_choice: " << gp_result_as_string(ret);
                        }
                        else {
                            qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_choice: " << choice;
                        }
                    }
                }
                break;

            default:
                ret = gp_widget_get_value(widget_imgsetting, &value);
                if (ret != GP_OK) {
                    qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_value: " << gp_result_as_string(ret);
                }
                else {
                    qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_value: " << value;
                }

                break;

            }

            // gp_widget_free(widget_imgsetting);
        }

    }    /**
     * Get the active resolution of the controller.
     * @return The index of the active resolution.
     */
    // virtual int getActiveResolution();

    /**
     * Set the active resolution of the controller.
     * @param ac The index of the new active resolution.
     */
    // virtual void setActiveResolution(int ac);



    // Importand image settings widgets:
    // Image size: main/imgsetting/imagesize
    ret = gp_widget_get_child_by_name(widget_imgsettings, "imagesize", &widget_imgsetting);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_child: " << gp_result_as_string(ret);
    }
    else {
        const char*  pos;
        char         widthStr[10];
        unsigned int outputWidth = 0;
        char         hightStr[10];
        unsigned int outputHeight = 0;
        const char*  value = NULL;
        int          readonly = 0;

        // Read the actual selected resolution
        ret = gp_widget_get_value(widget_imgsetting, &value);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_value: " << gp_result_as_string(ret);
        }
        else {
            qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_value: " << value;
        }

        // Read the number of possible resolutions
        ret = gp_widget_count_choices(widget_imgsetting);
        if (ret < GP_OK) {
            qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_count_choices: " << gp_result_as_string(ret);
        }
        else {
            choiceCount = ret;
            qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_count_choices: count: " << choiceCount;

            // Read all possible resolutions
            for (choiceIndex = 0; choiceIndex < choiceCount; choiceIndex++) {

                ret = gp_widget_get_choice(widget_imgsetting, choiceIndex, &choice);
                if (ret != GP_OK) {
                    qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_choice: " << gp_result_as_string(ret);
                }
                else {
                    qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_choice: " << choice;
                }

                // value/choice format: WWWWxHHHH
                pos = strchr(choice, 'x');
                strncpy(widthStr, choice, pos-choice);
                outputWidth = atoi(widthStr);
                strncpy(hightStr, pos+1, strlen(choice)-(pos-choice));
                outputHeight = atoi(hightStr);
                device->addResolution(GrabberResolution(outputWidth, outputHeight, GrabberResolution::unknownFormat, false));
            }

            if (strcmp(value, choice) == 0) {
                // This is the active choice
                // frontend->getProject()->setResolution(choiceIndex);

            }
        }

        ret = gp_widget_get_readonly(widget_imgsetting, &readonly);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::readImageSettings --> Error gp_widget_get_readonly: " << gp_result_as_string(ret);
        }
        else {
            qDebug() << "GphotoGrabber::readImageSettings --> gp_widget_get_readonly: " << readonly;
            if (0 == readonly) {
                device->setResolutionChangeable(false);
            }
        }

        // gp_widget_free(widget_imgsetting);
    }

    // gp_widget_free(widget_imgsettings);

    // gp_widget_free(widget_main);

    gp_widget_free(widget_root);

    qDebug() << "GphotoGrabber::readImageSettings --> End";
}


int GphotoGrabber::lookupWidget(CameraWidget  *widget,
                                const char    *key,
                                CameraWidget* *child)
{
    qDebug() << "GphotoGrabber::lookupWidget --> Start";

    int           ret;
    CameraWidget *ch;

    int childrenCount = gp_widget_count_children(widget);
    if (childrenCount < GP_OK) {
        return childrenCount;
    }
    for (int c = 0 ; c < childrenCount ; c++) {
        const char *name;

        ret = gp_widget_get_child(widget, c, &ch);
        if (ret < GP_OK) {
            return ret;
        }
        ret = gp_widget_get_name(ch, &name);
        if (ret < GP_OK) {
            return ret;
        }
        qDebug() << "GphotoGrabber::lookupWidget --> Children (" << c << "): Name: " << name;
    }

    ret = gp_widget_get_child_by_name(widget, key, child);
    if (ret < GP_OK) {
        ret = gp_widget_get_child_by_label(widget, key, child);
    }

    qDebug() << "GphotoGrabber::lookupWidget --> End";

    return ret;
}


/**
 * Reports all gphoto2 messages to the message view.
 *
 * @brief GphotoGrabber::errorDumper
 * @param level
 * @param domain
 * @param format
 * @param args
 * @param data
 */
void GphotoGrabber::errorDumper(GPLogLevel  level,
                                const char *domain,
                                const char *format,
                                va_list     args,
                                void       *data)
{
    char temp[1024];

    if (args != NULL) {
        vsprintf(temp, format, args);
        qDebug() << "GphotoGrabber::errorDumper --> Domain: " << domain << "Error: " << temp;
    }
    else {
        qDebug() << "GphotoGrabber::errorDumper --> Domain: " << domain << "Error: " << format;
    }
}


/**
 * This enables/disables the specific canon capture mode.
 *
 * For non canons this is not required, and will just return
 * with an error (but without negative effects).
 */
bool GphotoGrabber::canonEnableCapture(Camera       *gphotoCamera,
                                       GPContext    *gphotoContext,
                                       CameraWidget *gphotoConfig)
{
    int               onoff = 1;
    CameraWidgetType  type;
    CameraWidget     *child;
    int               ret;

    ret = lookupWidget(gphotoConfig, "capture", &child);
    if (ret < GP_OK) {
        // fprintf (stderr, "lookup widget failed: %d\n", ret);
        qDebug() << "GphotoGrabber::setUp --> Error End - lookup widget failed: " << gp_result_as_string(ret);
        return false;
    }

    ret = gp_widget_get_type(child, &type);
    if (ret < GP_OK) {
        // fprintf (stderr, "widget get type failed: %d\n", ret);
        qDebug() << "GphotoGrabber::setUp --> Error End - widget get type failed: " << gp_result_as_string(ret);
        return false;
    }

    switch (type) {
    case GP_WIDGET_TOGGLE:
        break;
    default:
        // fprintf (stderr, "widget has bad type %d\n", type);
        qDebug() << "GphotoGrabber::setUp --> Error End - widget has bad type";
        ret = GP_ERROR_BAD_PARAMETERS;
        return false;
    }

    /* Now set the toggle to the wanted value */
    ret = gp_widget_set_value(child, &onoff);
    if (ret < GP_OK) {
        // fprintf (stderr, "toggling Canon capture to %d failed with %d\n", onoff, ret);
        qDebug() << "GphotoGrabber::setUp --> Error End - toggling Canon capture failed: " << gp_result_as_string(ret);
        return false;
    }

    // OK
    ret = gp_camera_set_config(gphotoCamera, gphotoConfig, gphotoContext);
    if (ret < GP_OK) {
        // fprintf (stderr, "camera_set_config failed: %d\n", ret);
        qDebug() << "GphotoGrabber::setUp --> Error End - camera_set_config: " << gp_result_as_string(ret);
        return false;
    }

}


void GphotoGrabber::populateWithConfigs(CameraWidget *gphotoConfig)
{
    qDebug() << "GphotoGrabber::populateWithConfigs --> Start";

    int               i;
    int               count;
    int               childrenTotal;
    const char       *namePointer;
    const char       *valuePointer;
    CameraWidget     *child;
    CameraWidgetType  widgetType;
    QString           name;
    QString           value;
    int               date;
    int               ret;

    ret = gp_widget_get_type(gphotoConfig, &widgetType);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_type: " << gp_result_as_string(ret);
    }
    switch (widgetType) {
    case GP_WIDGET_RADIO:

        ret = gp_widget_get_name(gphotoConfig, &namePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_name: " << gp_result_as_string(ret);
        }
        ret = gp_widget_get_value(gphotoConfig, &valuePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_value: " << gp_result_as_string(ret);
        }

        if (valuePointer == NULL)
            break;

        count = gp_widget_count_choices(gphotoConfig);

        name.clear();
        name.append(namePointer);
        value.clear();
        value.append(valuePointer);
        // sdialog.addItem(n,v);
        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_RADIO --> Name: " << name << " --> Value: " << value;

        /*
        string sname = string(name);
        string sval  = string(val);
        printf("%s=%s\n", name, val);
        if ( sname == "iso" )
            iso = val;
        if ( sname == "aperture" )
            aperture = val;
        if ( sname == "shutterspeed" )
            shutter = val;
        if ( sname == "capturetarget" )
            target = val;
        */

        break;
    case GP_WIDGET_TEXT:

        ret = gp_widget_get_name(gphotoConfig, &namePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_name: " << gp_result_as_string(ret);
        }
        ret = gp_widget_get_value(gphotoConfig, &valuePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_value: " << gp_result_as_string(ret);
        }

        //QString log;
        //logMessage(log.sprintf("%s=%s", name, val));
        name.clear();
        name.append(namePointer);
        value.clear();
        value.append(valuePointer);
        // sdialog.addItem(n,v);
        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_TEXT --> Name: " << name << " --> Value: " << value;

        /*
        string sname = string(name);
        string sval  = string(val);

        printf("%s=%s\n", name, val);
        if ( sname == "batterylevel" )
            battery = sval;
        if ( sname == "ownername" )
            ownername = sval;
        if ( sname == "serialnumber" )
            serialnumber = sval;
        if ( sname == "model" )
            model = sval;
        if ( sname == "ptpversion" )
            ptpversion = sval;
        */

        break;
    case GP_WIDGET_RANGE:

        ret = gp_widget_get_name(gphotoConfig, &namePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_name: " << gp_result_as_string(ret);
        }
        name.clear();
        name.append(namePointer);
        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_RANGE --> Name: " << name;

        break;
    case GP_WIDGET_TOGGLE:

        ret = gp_widget_get_name(gphotoConfig, &namePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_name: " << gp_result_as_string(ret);
        }
        name.clear();
        name.append(namePointer);
        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_TOGGLE --> Name: " << name;

        break;
    case GP_WIDGET_DATE:

        ret = gp_widget_get_name(gphotoConfig, &namePointer);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_name: " << gp_result_as_string(ret);
        }
        ret = gp_widget_get_value(gphotoConfig, &date);
        if (ret != GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_value: " << gp_result_as_string(ret);
        }
        name.clear();
        name.append(namePointer);
        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_RADIO --> Name: " << name << " --> Date: " << date;

        break;
    case GP_WIDGET_WINDOW:
    case GP_WIDGET_SECTION:

        qDebug() << "GphotoGrabber::populateWithConfigs --> GP_WIDGET_WINDOW / GP_WIDGET_SECTION";

        childrenTotal = gp_widget_count_children(gphotoConfig);
        if (childrenTotal < GP_OK) {
            qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_count_children: " << gp_result_as_string(ret);
        }
        for (i = 0; i < childrenTotal; i ++) {
            ret = gp_widget_get_child(gphotoConfig, i, &child);
            if (ret != GP_OK) {
                qDebug() << "GphotoGrabber::populateWithConfigs --> Error gp_widget_get_child: " << gp_result_as_string(ret);
            }
            populateWithConfigs(child);
        }

        break;
    default:

        qDebug() << "GphotoGrabber::populateWithConfigs --> default!!!";

        break;
    }

    qDebug() << "GphotoGrabber::populateWithConfigs --> End";
}
