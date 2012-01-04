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

#ifndef FRONTEND_H
#define FRONTEND_H

#include "domain/domainfacade.h"
#include "frontends/viewfacade.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"
#include "technical/grabber/imagegrabberdevice.h"

#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtGui/QImage>

class PreferencesTool;
class DomainFacade;
class ViewFacade;

/**
 * The frontend interface to be used by the implemented frontend.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
*/
class Frontend
{
public:
    virtual ~Frontend() {}

    /**
     * Abstract function for starting the application through the frontend.
     * @param argc the argc argument from the environment through main.
     * @param argv the argv arguments from the environment through main.
     * @return the return status on exit
     */
    virtual int run(int argc, char **argv) = 0;

    /**
     * Abstract function for geting the domain facade.
     * @return the domain facade.
     */
    virtual DomainFacade* getProject() = 0;

    /**
     * Abstract function for geting the view facade.
     * @return the view facade.
     */
    virtual ViewFacade* getView() = 0;

    /**
     * Abstract function for geting the preferences tool.
     * @return the preferences tool.
     */
    virtual PreferencesTool* getPreferences() = 0;

    /**
     * Abstract function for getting the application directory
     * @return the application directory name string
     */
    virtual const char* getUserDirName() = 0;

    /**
     * Abstract function for getting the temporary directory
     * @return the temporary directory name string
     */
    virtual const char* getTempDirName() = 0;

    /**
     * Abstract function for getting the trash directory
     * @return the trash directory name string
     */
    virtual const char* getTrashDirName() = 0;

    /**
     * Abstract function for getting the packer directory
     * @return the packer directory name string
     */
    virtual const char* getPackerDirName() = 0;

    /**
     * Abstract function for getting the binary directory
     * @return the binary directory name string
     */
    virtual const char* getBinDirName() = 0;

    /**
     * Abstract function for getting the manual directory
     * @return the manual directory name string
     */
    virtual const char* getManualDirName() = 0;

    /**
     * Abstract function for getting the translations directory
     * @return the translations directory name string
     */
    virtual const char* getTranslationsDirName() = 0;

    /**
     * Abstract function for getting the graphics directory
     * @return the graphics directory name string
     */
    virtual const char* getGraphicsDirName() = 0;

    /**
     * Abstract function for getting the icons directory
     * @return the icons directory name string
     */
    virtual const char* getIconsDirName() = 0;

    /**
     * Abstract function for getting the pictures directory
     * @return the pictures directory name string
     */
    virtual const char* getPicturesDirName() = 0;

    /**
     * Abstract function for getting all possible GUI languages
     * @return A vector with all languages
     */
    virtual const QVector<QString> getLanguages() = 0;

    /**
     * Abstract function for getting all possible GUI locales
     * @return A vector with all locales
     */
    virtual const QVector<QString> getLocales() = 0;

    /**
     * A new language is selected in the preferences menu.
     * @param newIndex New selected index.
     */
    virtual void changeLanguage(int newIndex) = 0;

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    virtual void changeCaptureButtonFunction(int newFunction) = 0;

    /**
     * Abstract function for displaying progress on timeconsuming operations.
     * @param infoText the text to display to the user
     * @param numOperations the number of calculated operations to do
     */
    virtual void showProgress(const QString &infoText, unsigned int numOperations = 0) = 0;

    /**
     * Abstract function for hiding the progress info.
     */
    virtual void hideProgress() = 0;

    /**
     * Abstract function for updating the progress.
     * @param numOperationsDone the number of operations done
     */
    virtual void updateProgress(int numOperationsDone) = 0;

    /**
     * Abstract function to display a message in the status bar.
     * @param message the message to display.
     * @param timeout the timeout for the message in mili-seconds.
     */
    virtual void showMessage(const QString &message, int timeout = 0) = 0;

    /**
     * Abstract function to remove any temporary message being shown in the status bar.
     */
    virtual void clearMessage() = 0;

    /**
     * Abstract function for changing the information to display to the user
     * @param infoText the text to display to the user
     */
    virtual void setProgressInfo(const char *infoText) = 0;

    /**
     * Abstract function for changing the prozess id in the status bar
     * @param id the id to display in the status bar
     */
    virtual void setProjectID(const char *id) = 0;

    /**
     * Abstract function for changing the scene id in the status bar
     * @param id the id to display in the status bar
     */
    virtual void setSceneID(const char *id) = 0;

    /**
     * Abstract function for changing the take id in the status bar
     * @param id the id to display in the status bar
     */
    virtual void setTakeID(const char *id) = 0;

    /**
     * Abstract function for changing the exposure id in the status bar
     * @param id the id to display in the status bar
     */
    virtual void setExposureID(const char *id) = 0;

    /**
     * Abstract function for checking if the user has aborted the operation
     * (eg pressed cancel)
     * @return true if the the operation is aborted, false otherwise
     */
    virtual bool isOperationAborted() = 0;

    /**
     * Abstract function for processing GUI events. This is useful on timeconsuming
     * operations which aren't running in separate processes or threads.
     */
    virtual void processEvents() = 0;

    /**
     * Abstract function for asking the user a yes/no question.
     * @param title title of the message dialog
     * @param question the question to ask
     * @return 0 if the user answer yes, 1 if no
     */
    virtual int askQuestion(const QString title, const QString &question) = 0;

    /**
     * Abstract function for informating the user.
     * @param title title of the message dialog
     * @param info the information for the user
     */
    virtual void showInformation(const QString title, const QString &info) = 0;

    /**
     * Abstract function for warning the user.
     * @param title title of the message dialog
     * @param warning the warning for the user
     */
    virtual void showWarning(const QString title, const QString &warning) = 0;

    /**
     * Abstract function for signaling a critical state to the user.
     * @param title title of the message dialog
     * @param message the message for the user
     */
    virtual void showCritical(const QString title, const QString &message) = 0;

    /**
     * Abstract function to run a external command
     * @param command command to run
     * @return return value of the command
     */
    virtual int runExternalCommand(const QString &command) = 0;

    /**
     * Abstract function to turns on the webcamera/video import mode.
     */
    virtual bool startGrabber() = 0;

    /**
     * Abstract function to turns off the webcamera/video import mode.
     */
    virtual void stopGrabber() = 0;

    /**
     * Abstract function to get the possible video devices.
     * @return Vector with the devices.
     */
    virtual const QVector<ImageGrabberDevice*> getDevices() = 0;

    /**
     * Abstract function to get one video devices.
     * @arg deviceIndex The index of the device.
     * @return The video devices.
     */
    virtual ImageGrabberDevice *getDevice(int deviceIndex) = 0;

    /**
     * Abstract function to get the possible video device names.
     * @return Vector with the device names.
     */
    virtual const QVector<QString> getDeviceNames() = 0;

    /**
     * Abstract function to get the actual image from the image grabber.
     * @return actual image.
     */
    virtual const QImage getActualImage() = 0;

    /**
     * Test function for debugging purposes
     */
    virtual void test(QWidget *parentWidget, int level) = 0;

    /**
     * Create all temporary application directories (temp, trash, packer)
     */
    virtual void makeApplicationDirectories() = 0;

    /**
     * Remove all temporary application directories (temp, trash, packer)
     */
    virtual void removeApplicationDirectories() = 0;

    /**
     * Remove all temporary application files
     */
    virtual void removeApplicationFiles() = 0;

    /**
     * Remove all temporary files
     */
    virtual void removeCaptureFiles() = 0;

    /**
     * Test function for debugging purposes
     */
    virtual void testMainWidget() = 0;
};

#endif
