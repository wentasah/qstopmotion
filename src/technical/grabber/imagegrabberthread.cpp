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

#include "imagegrabberthread.h"


ImageGrabberThread::ImageGrabberThread(ImageGrabber *grabber)
    : grabber(grabber)
{
    wasGrabSuccess = true;
}


void ImageGrabberThread::run()
{
    //Don't want the frameview to update unless the grabbing worked.
    while (true) {
        wasGrabSuccess = grabber->grab();
        if (!wasGrabSuccess) {
            break;
        }
        // frameView->redraw();
    }
}


bool ImageGrabberThread::wasGrabbingSuccess()
{
    return wasGrabSuccess;
}