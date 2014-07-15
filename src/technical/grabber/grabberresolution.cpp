/******************************************************************************
 *  Copyright (C) 2014-2014 by                                                *
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
    id.clear();
    width   = 0;
    height  = 0;
    active  = false;
}


GrabberResolution::GrabberResolution(const QString i,
                                     unsigned int  w,
                                     unsigned int  h,
                                     bool          a)
{
    id.append(i);
    width   = w;
    height  = h;
    active  = a;
}


GrabberResolution::~GrabberResolution()
{
}


const QString GrabberResolution::getId()
{
    return id;
}


void GrabberResolution::setId(const QString i)
{
    id.clear();
    id.append(i);
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


bool GrabberResolution::isActive()
{
    return active;
}
