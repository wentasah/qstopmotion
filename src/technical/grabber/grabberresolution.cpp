/******************************************************************************
 *  Copyright (C) 2014-2015 by                                                *
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

#include "grabberresolution.h"


GrabberResolution::GrabberResolution()
{
    width   = 0;
    height  = 0;
    active  = false;
}


GrabberResolution::GrabberResolution(unsigned int  w,
                                     unsigned int  h,
                                     unsigned int  f,
                                     bool          a)
{
    width   = w;
    height  = h;
    format  = f;
    active  = a;
}


GrabberResolution::~GrabberResolution()
{
}


unsigned int GrabberResolution::getWidth()
{
    return width;
}


void GrabberResolution::setWidth(unsigned int w)
{
    width = w;
}


unsigned int GrabberResolution::getHeight()
{
    return height;
}


void GrabberResolution::setHeight(unsigned int h)
{
    height = h;
}


unsigned int GrabberResolution::getFormat()
{
    return format;
}


void GrabberResolution::setFormat(unsigned int f)
{
    format = f;
}


bool GrabberResolution::isActive()
{
    return active;
}


const QString GrabberResolution::getName()
{
    QString formatName;

    switch(format) {
    case GrabberResolution::rgb24Format:
        formatName.append("RGB24");
        break;
    case GrabberResolution::bgr24Format:
        formatName.append("BGR24");
        break;
    case GrabberResolution::ayuvFormat:
        formatName.append("AYUV");
        break;
    case GrabberResolution::yuv2Format:
        formatName.append("YUV2");
        break;
    case GrabberResolution::uyvyFormat:
        formatName.append("UYUY");
        break;
    case GrabberResolution::yu12Format:
        formatName.append("YU12");
        break;
    case GrabberResolution::yuv420mFormat:
        formatName.append("YUV420");
        break;
    case GrabberResolution::yv12Format:
        formatName.append("YV12");
        break;
    case GrabberResolution::yvu420mFormat:
        formatName.append("YVU420");
        break;
    case GrabberResolution::nv12Format:
        formatName.append("NV12");
        break;
    case GrabberResolution::mjpegFormat:
        formatName.append("MJPG");
        break;
    case GrabberResolution::unknownFormat:
    default:
        formatName.append("????");
        break;
    }
    QString name = QString("%1%2%3%4%5")
                   .arg(width)
                   .arg(QLatin1String(" x "))
                   .arg(height)
                   .arg(QLatin1String(" - "))
                   .arg(formatName);

    return name;
}
