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

#include "v4l2grabber.h"
#include "v4l2controller.h"

#include "technical/util.h"

#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>


V4L2Grabber::V4L2Grabber(Frontend *f)
    : ImageGrabber(f)
{
    qDebug("V4L2Grabber::Constructor --> Start (Empty)");

    // qDebug("V4L2Grabber::Constructor --> End");
}


V4L2Grabber::~V4L2Grabber()
{
    qDebug("V4L2Grabber::Destructor --> Start (Empty)");

    // qDebug("V4L2Grabber::Destructor --> End");
}


bool V4L2Grabber::initialization(QVector<ImageGrabberDevice*> &devices)
{
    qDebug("V4L2Grabber::initialization --> Start");


    qDebug() << "V4L2Grabber::initialization --> device count: " << devices.size();

    qDebug("V4L2Grabber::initialization --> End (true)");

    return true;
}


bool V4L2Grabber::setUp()
{
    qDebug() << "V4L2Grabber::setUp --> Start";


    qDebug() << "V4L2Grabber::setUp --> End";

    return true;
}


bool V4L2Grabber::grab()
{
    return true;
}


const QImage V4L2Grabber::getImage()
{
    QImage image;

    if (firstImage) {
        firstImage = false;
    }

    Q_ASSERT(image.isNull() == false);

    return image;
}


const QImage V4L2Grabber::getLiveImage()
{
    // liveImage.load(filePath);

    liveImage = getImage();

    return liveImage;
}


const QImage V4L2Grabber::getRawImage()
{
    // rawImage.load(filePath);

    rawImage = getImage();

    return rawImage;
}


bool V4L2Grabber::tearDown()
{
    qDebug() << "V4L2Grabber::tearDown --> Start";

    qDebug() << "V4L2Grabber::tearDown --> End";

    return true;
}
