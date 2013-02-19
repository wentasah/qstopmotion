/******************************************************************************
 *  Copyright (C) 2010-2012 by                                                *
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

#include "gphotograbber.h"

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

    gphotoCamera = NULL;
    gphotoContext = NULL;

    int retval;

    gphotoContext = gp_context_new();
    gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
    gp_camera_new(&gphotoCamera);

    qDebug("GphotoGrabber::Constructor --> Camera init.  Takes a second or three...");
    retval = gp_camera_init(gphotoCamera, gphotoContext);
    if (retval != GP_OK) {
        qDebug("GphotoGrabber::Constructor --> Error during camera initialization!");
        isInitSuccess = false;
    }
    isInitSuccess = true;

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

    // qDebug("GphotoGrabber::Destructor --> End");
}


bool GphotoGrabber::initializationSubclass(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("GphotoGrabber::initializationSubclass --> Start");

    if (!isInitSuccess) {
        qDebug("GphotoGrabber::initializationSubclass --> End (Error)");
        return false;
    }

    ImageGrabberDevice *device = NULL;

    qDebug() << "GphotoGrabber::initializationSubclass --> Add video test device";

    device = new ImageGrabberDevice("",
                                    QApplication::translate("GphotoGrabber", "Gphoto2 device"),
                                    ImageGrabberDevice::gphoto2Source,
                                    ImageGrabberDevice::video_x_none);
    devices.append(device);

    qDebug() << "GphotoGrabber::initializationSubclass --> device count: " << devices.size();

    qDebug("GphotoGrabber::initializationSubclass --> End");
    return true;
}


bool GphotoGrabber::initSubclass()
{
    qDebug() << "GphotoGrabber::initSubclass --> Start";

    if (!isInitSuccess) {
        qDebug("GphotoGrabber::initSubclass --> End (Error)");
        return false;
    }

    CameraWidget     *widget = NULL;
    CameraWidget     *child = NULL;
    CameraWidgetType  type;
    int               ret;
    int               onoff = true;

    ret = gp_camera_get_config(gphotoCamera, &widget, gphotoContext);
    if (ret < GP_OK) {
        // fprintf (stderr, "camera_get_config failed: %d\n", ret);
        qDebug("GphotoGrabber::initSubclass --> End (camera_get_config failed)");
        return false;
    }
    ret = lookup_widget(widget, "capture", &child);
    if (ret < GP_OK) {
        // fprintf (stderr, "lookup widget failed: %d\n", ret);
        qDebug("GphotoGrabber::initSubclass --> End (lookup widget failed)");
        goto out;
    }

    ret = gp_widget_get_type(child, &type);
    if (ret < GP_OK) {
        // fprintf (stderr, "widget get type failed: %d\n", ret);
        qDebug("GphotoGrabber::initSubclass --> End (widget get type failed)");
        goto out;
    }
    switch (type) {
    case GP_WIDGET_TOGGLE:
        break;
    default:
        // fprintf (stderr, "widget has bad type %d\n", type);
        qDebug("GphotoGrabber::initSubclass --> End (widget has bad type)");
        ret = GP_ERROR_BAD_PARAMETERS;
        goto out;
    }
    /* Now set the toggle to the wanted value */
    ret = gp_widget_set_value (child, &onoff);
    if (ret < GP_OK) {
        // fprintf (stderr, "toggling Canon capture to %d failed with %d\n", onoff, ret);
        qDebug("GphotoGrabber::initSubclass --> End (toggling Canon capture failed)");
        goto out;
    }
    // OK
    ret = gp_camera_set_config (gphotoCamera, widget, gphotoContext);
    if (ret < GP_OK) {
        // fprintf (stderr, "camera_set_config failed: %d\n", ret);
        qDebug("GphotoGrabber::initSubclass --> End (camera_set_config failed)");
        return false;
    }
out:
    gp_widget_free (widget);

    qDebug("GphotoGrabber::initSubclass --> End");
    return true;
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

    CameraFile *file;
    char output_file[256];
    char temp_file[256];
    int retval;

    retval = gp_file_new(&file);
    if (retval != GP_OK) {
        // fprintf(stderr,"gp_file_new: %d\n", retval);
        qDebug() << "GphotoGrabber::getLiveImage --> End (gp_file_new)";
        return QImage();
    }
    retval = gp_camera_capture_preview(gphotoCamera, file, gphotoContext);
    if (retval != GP_OK) {
        // fprintf(stderr,"gp_camera_capture_preview(%d): %d\n", i, retval);
        qDebug() << "GphotoGrabber::getLiveImage --> End (gp_camera_capture_preview)";
        return QImage();
    }

    sprintf(output_file, "%s", filePath.toAscii().data());
    sprintf(temp_file, "%s.tmp", output_file);

    retval = gp_file_save(file, temp_file);
    if (retval != GP_OK) {
        // fprintf(stderr,"gp_camera_capture_preview(%d): %d\n", i, retval);
        qDebug() << "GphotoGrabber::getLiveImage --> End (gp_camera_capture_preview)";
        return QImage();
    }
    gp_file_unref(file);

    /* When the file name is constant, unlink first... */
    unlink(output_file);   // Delete the name from the file system (#include <unistd.h>)

    /* Always doing this */
    link(temp_file, output_file);  // Create a new link to the existing file (#include <unistd.h>)
    unlink(temp_file);   // Delete the name from the file system (#include <unistd.h>)


    liveImage.load(filePath);

    qDebug() << "GphotoGrabber::getLiveImage --> End";
    return liveImage;
}


const QImage GphotoGrabber::getRawImage()
{
    // rawImage.load(filePath);

    // rawImage = getImage();

    return rawImage;
}


int GphotoGrabber::lookup_widget(CameraWidget *widget,
                                 const char *key,
                                 CameraWidget **child)
{
    int ret;

    ret = gp_widget_get_child_by_name(widget, key, child);
    if (ret < GP_OK) {
        ret = gp_widget_get_child_by_label(widget, key, child);
    }
    return ret;
}


void GphotoGrabber::errordumper(GPLogLevel level,
                                const char *domain,
                                const char *format,
                                va_list args,
                                void *data)
{
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
}
