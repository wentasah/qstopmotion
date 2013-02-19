/******************************************************************************
 *  Copyright (C) 2012-2013 by                                                *
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

#include "grabbercontrolcapabilities.h"


GrabberControlCapabilities::GrabberControlCapabilities()
{
    id       = 0;
    minimum  = 0L;
    maximum  = 0L;
    step     = 0L;
    defaultt = 0L;
    flags    = control_none;
}


GrabberControlCapabilities::~GrabberControlCapabilities()
{
}


bool GrabberControlCapabilities::isCapability()
{
    if (getFlags() != GrabberControlCapabilities::control_none) {
        return true;
    }
    return false;
}


bool GrabberControlCapabilities::isAutomatic()
{
    if (getFlags() == GrabberControlCapabilities::control_Auto) {
        return true;
    }

    return false;
}


unsigned int GrabberControlCapabilities::getId()
{
    return id;
}


void GrabberControlCapabilities::setId(unsigned int i)
{
    id = i;
}


long GrabberControlCapabilities::getMinimum()
{
    return minimum;
}


void GrabberControlCapabilities::setMinimum(long mi)
{
    minimum = mi;
}


long GrabberControlCapabilities::getMaximum()
{
    return maximum;
}


void GrabberControlCapabilities::setMaximum(long ma)
{
    maximum = ma;
}


long GrabberControlCapabilities::getStep()
{
    return step;
}


void GrabberControlCapabilities::setStep(long st)
{
    step = st;
}


long GrabberControlCapabilities::getDefault()
{
    return defaultt;
}


void GrabberControlCapabilities::setDefault(long de)
{
    defaultt = de;
}


long GrabberControlCapabilities::getFlags()
{
    return flags;
}


void GrabberControlCapabilities::setFlags(long fl)
{
    flags = fl;
}
