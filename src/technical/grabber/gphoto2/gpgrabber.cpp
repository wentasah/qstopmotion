/******************************************************************************
 *  Copyright (C) 2010-2014 by                                                *
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

#include "gpcontroller.h"
#include "technical/grabber/imagegrabberdevice.h"
#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

// Include files of the gphoto library
#include <unistd.h>
#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>


GphotoGrabber::GphotoGrabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("GphotoGrabber::Constructor --> Start");

    int ret;

    gphotoCamera = NULL;
    gphotoContext = NULL;

    canCaptureImage = false;
    canCaptureAudio = false;
    canCaptureVideo = false;
    canCapturePreview = false;
    canConfigure = false;

    gphotoContext = gp_context_new();
    // gphotoErrorId = gp_log_add_func(GP_LOG_ERROR, errorDumper, NULL);
    gp_camera_new(&gphotoCamera);

    qDebug("GphotoGrabber::Constructor --> Camera init.  Takes a second or three...");
    ret = gp_camera_init(gphotoCamera, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error during camera initialization: " << gp_result_as_string(ret);
        isInitSuccess = false;
        return;
    }
    isInitSuccess = true;

    // summary
    CameraText sum;
    ret = gp_camera_get_summary(gphotoCamera, &sum, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_summary: " << gp_result_as_string(ret);
    }
    QStringList sumList = QString(sum.text).split('\n');
    for (int i = 0; i < sumList.size(); i++) {
        qDebug() << "GphotoGrabber::Constructor --> Summary: " << sumList.at(i);
    }
    manufacturer.append(sumList.at(0).split(' ').at(1));
    model.append(sumList.at(1).split(' ').at(1));

    // manual
    CameraText manual;
    ret = gp_camera_get_manual(gphotoCamera, &manual, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_manual: " << gp_result_as_string(ret);
    }
    else {
        qDebug() << "GphotoGrabber::Constructor --> Manual: " << QString(manual.text);
    }

    // about
    CameraText about;
    ret = gp_camera_get_about(gphotoCamera, &about, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_about: " << gp_result_as_string(ret);
    }
    qDebug() << "GphotoGrabber::Constructor --> About: " << QString(about.text);

    QString rootDir;
    rootDir.append(frontend->getUserDirName());
    rootDir.append("/");
    rootDir.append("capturedfile.jpg");
    filePath.append(rootDir);

    qDebug("GphotoGrabber::Constructor --> End");
}


GphotoGrabber::~GphotoGrabber()
{
    qDebug("GphotoGrabber::Destructor --> Start (Empty)");
    /*
    int ret;

    ret = gp_log_remove_func(gphotoErrorId);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::Constructor --> Error gp_camera_get_about: " << gp_result_as_string(ret);
    }

    qDebug("GphotoGrabber::Destructor --> End");
    */
}


bool GphotoGrabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GphotoGrabber::initialization --> Start");

    GphotoController *deviceController = NULL;
    PreferencesTool  *pref = frontend->getPreferences();
    int               value;

    if (!isInitSuccess) {
        qDebug("GphotoGrabber::initialization --> End (Error)");
        return false;
    }

    ImageGrabberDevice *device = NULL;

    qDebug() << "GphotoGrabber::initialization --> Add video test device";

    device = new ImageGrabberDevice("",
                                    QString("%1 %2").arg(manufacturer).arg(model),
                                    ImageGrabberDevice::gphoto2Source,
                                    ImageGrabberDevice::video_x_none);
    devices.append(device);

    // Create grabber controller
    if (pref->getIntegerPreference("preferences", "gstreamerdirectshowusbcontroller", value) == false) {
        value = false;
    }
    if ((int)true == value) {
        deviceController = new GphotoController(0);
        if (deviceController->init(device->getDeviceId()))
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

    qDebug() << "GphotoGrabber::initialization --> device count: " << devices.size();

    qDebug("GphotoGrabber::initialization --> End");
    return true;
}


bool GphotoGrabber::setUp()
{
    qDebug() << "GphotoGrabber::setUp --> Start";

    if (!isInitSuccess) {
        qDebug("GphotoGrabber::setUp --> End (Error)");
        return false;
    }

    CameraWidget     *gphotoConfig = NULL;
    CameraAbilities   abilities;
    int               ret;
    bool              returnValue = true;

    ret = gp_camera_get_config(gphotoCamera, &gphotoConfig, gphotoContext);
    if (ret < GP_OK) {
        // fprintf (stderr, "camera_get_config failed: %d\n", ret);
        qDebug() << "GphotoGrabber::setUp --> Error End camera_get_config failed: " << gp_result_as_string(ret);
        return false;
    }

    if (manufacturer.compare("canon", Qt::CaseInsensitive) == 0) {
        if (false == canonEnableCapture(gphotoCamera, gphotoContext, gphotoConfig)) {
            returnValue = false;
            goto out;
        }

        populateWithConfigs(gphotoConfig);
    }

    // Abilities
    qDebug() << "GphotoGrabber::setUp --> Checking camera abilities";
    ret = gp_camera_get_abilities (gphotoCamera, &abilities);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::setUp --> Error End gp_camera_get_abilities: " << gp_result_as_string(ret);
        returnValue = false;
        goto out;
    }

    if (ret >= GP_OK)
    {
        // rewrite model
        qDebug() << "GphotoGrabber::setUp --> Abilities.model: " << QString(abilities.model);
        switch (abilities.status)
        {
        case GP_DRIVER_STATUS_PRODUCTION:
            qDebug() << "GphotoGrabber::setUp --> Abilities.driver_status: production";
            break;
        case GP_DRIVER_STATUS_TESTING:
            qDebug() << "GphotoGrabber::setUp --> Abilities.driver_status: testing";
            break;
        case GP_DRIVER_STATUS_EXPERIMENTAL:
            qDebug() << "GphotoGrabber::setUp --> Abilities.driver_status: experimental";
            break;
        case GP_DRIVER_STATUS_DEPRECATED:
            qDebug() << "GphotoGrabber::setUp --> Abilities.driver_status: deprecated";
            break;
        default:
            qDebug() << "GphotoGrabber::setUp --> Abilities.driver_status: unknown";
        }

        qDebug() << "GphotoGrabber::setUp --> Abilities.library: " << abilities.library;
        qDebug() << "GphotoGrabber::setUp --> Abilities.operations: " << abilities.operations;

        if ((abilities.operations & GP_OPERATION_CAPTURE_IMAGE) > 0 ) {
            canCaptureImage = true;
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_VIDEO) > 0) {
            canCaptureAudio = true;
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_AUDIO) > 0) {
            canCaptureVideo = true;
        }
        if ((abilities.operations & GP_OPERATION_CAPTURE_PREVIEW) > 0) {
            canCapturePreview = true;
        }
        if ((abilities.operations & GP_OPERATION_CONFIG) > 0) {
            canConfigure = true;
        }
    }

    qDebug("GphotoGrabber::setUp --> End");

out:
    gp_widget_free(gphotoConfig);

    return returnValue;
}


bool GphotoGrabber::tearDown()
{
    qDebug() << "GphotoGrabber::tearDown --> Start";

    gp_camera_exit(gphotoCamera, gphotoContext);

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
    unsigned long  mysize = 0;
    const char    *data = NULL;
    int            ret;

    if (false == canCapturePreview) {
        return getRawImage();
    }

    ret = gp_file_new(&cf);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error End gp_file_new: " << gp_result_as_string(ret);
        return QImage();
    }

    ret = gp_camera_capture_preview(gphotoCamera, cf, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error End gp_camera_capture_preview: " << gp_result_as_string(ret);
        return QImage();
    }

    // setImage(cf, a);

    ret = gp_file_get_data_and_size(cf, &data, &mysize);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getLiveImage --> Error: gp_file_get_data_and_size: " << gp_result_as_string(ret);
        return QImage();
    }

    if (mysize == 0 ) {
        return QImage();
    }

    liveImage.loadFromData((uchar*)data, (uint)mysize);

    /*
    char output_file[256];
    char temp_file[256];

    sprintf(output_file, "%s", filePath.toAscii().data());
    sprintf(temp_file, "%s.tmp", output_file);

    ret = gp_file_save(cf, temp_file);
    if (ret != GP_OK) {
        // fprintf(stderr,"gp_camera_capture_preview(%d): %d\n", i, ret);
        qDebug() << "GphotoGrabber::getLiveImage --> Error End gp_camera_capture_preview: " << gp_result_as_string(ret);
        return QImage();
    }
    gp_file_unref(cf);

    // When the file name is constant, unlink first...
    unlink(output_file);   // Delete the name from the file system (#include <unistd.h>)

    // Always doing this
    link(temp_file, output_file);  // Create a new link to the existing file (#include <unistd.h>)
    unlink(temp_file);             // Delete the name from the file system (#include <unistd.h>)

    liveImage.load(filePath);
    */

    qDebug() << "GphotoGrabber::getLiveImage --> End";
    return liveImage;
}


const QImage GphotoGrabber::getRawImage()
{
    qDebug() << "GphotoGrabber::getRawImage --> Start";

    CameraFilePath  cfp;
    CameraFile     *cf;
    unsigned long   mysize = 0;
    const char     *data = NULL;
    int             ret;

    // Taking Image
    ret = gp_camera_capture(gphotoCamera, GP_CAPTURE_IMAGE, &cfp, gphotoContext );
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_camera_capture: " << gp_result_as_string(ret);
        return QImage();
    }

    // Downloading Image
    ret = gp_file_new(&cf);
    if (ret != GP_OK) {
        qDebug() << "gp_file_new";
        return QImage();
    }

    ret = gp_camera_file_get(gphotoCamera, cfp.folder, cfp.name, GP_FILE_TYPE_NORMAL, cf, gphotoContext);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_camera_file_get: " << gp_result_as_string(ret);
        return QImage();
    }

    // setImage(cf, a);

    ret = gp_file_get_data_and_size(cf, &data, &mysize);
    if (ret != GP_OK) {
        qDebug() << "GphotoGrabber::getRawImage --> Error: gp_file_get_data_and_size: " << gp_result_as_string(ret);
        return QImage();
    }

    if (mysize == 0 ) {
        return QImage();
    }

    rawImage.loadFromData((uchar*)data, (uint)mysize);

    qDebug() << "GphotoGrabber::getRawImage --> End";
    return rawImage;
}


int GphotoGrabber::lookupWidget(CameraWidget  *widget,
                                const char    *key,
                                CameraWidget* *child)
{
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
    return ret;
}


void GphotoGrabber::errorDumper(GPLogLevel  level,
                                const char *domain,
                                const char *format,
                                va_list     args,
                                void       *data)
{
    char temp[1024];

    vsprintf(temp, format, args);
    qDebug() << "GphotoGrabber::errorDumper --> Error: " << temp;
}


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
