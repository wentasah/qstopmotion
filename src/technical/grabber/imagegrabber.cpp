/******************************************************************************
 *  Copyright (C) 2005-2017 by                                                *
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

#include "imagegrabber.h"

#include <QDebug>


ImageGrabber::ImageGrabber(Frontend *f)
{
    qDebug() << "ImageGrabber::Constructor --> Start";

    frontend = f;

    qDebug() << "ImageGrabber::Constructor --> End";
}


ImageGrabber::~ImageGrabber()
{
    qDebug() << "ImageGrabber::Destructor --> Start (Empty)";

    // qDebug() << "ImageGrabber::Destructor --> End");
}


void ImageGrabber::finalize()
{
    qDebug() << "ImageGrabber::finalize --> Start (Empty)";

    // qDebug() << "ImageGrabber::finalize --> End";
}
