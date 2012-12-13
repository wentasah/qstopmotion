/******************************************************************************
 *  Copyright (C) 2012-2012 by                                                *
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

#ifndef GRABBERCONTROLCAPABILITIES_H
#define GRABBERCONTROLCAPABILITIES_H


/**
 * Capabilities of a control of a device.
 *
 * @author Ralf Lange
 */
class GrabberControlCapabilities
{
public:

    /**
     * Device controller capabilities
     */
    enum grabberControlFlags {
        control_none,
        control_Auto,
        control_Manual,
        control_Relative,
        control_Absolute
    };

    /**
     * Constructs and initializes the object.
     */
    GrabberControlCapabilities();

    /**
     * Destructor
     */
    ~GrabberControlCapabilities();

    /**
     * Get the minimum value of the control
     * @return The minimum value of the control.
     */
    long getMinimum();

    /**
     * Set the minimum value of the control
     * @param mi The new minimum value of the control.
     */
    void setMinimum(long mi);

    /**
     * Get the maximum value of the control
     * @return The maximum value of the control.
     */
    long getMaximum();

    /**
     * Set the maximum value of the control
     * @param ma The new maximum value of the control.
     */
    void setMaximum(long ma);

    /**
     * Get the step value of the control
     * @return The step value of the control.
     */
    long getStep();

    /**
     * Set the step value of the control
     * @param st The new step value of the control.
     */
    void setStep(long st);

    /**
     * Get the def value of the control
     * @return The def value of the control.
     */
    long getDef();

    /**
     * Set the flags value of the control
     * @param fl The new flags value of the control.
     */
    void setDef(long de);

    /**
     * Get the flags value of the control
     * @return The flags value of the control.
     */
    long getFlags();

    /**
     * Set the flags value of the control
     * @param fl The new flags value of the control.
     */
    void setFlags(long fl);

private:

    long minimum;
    long maximum;
    long step;
    long def;
    long flags;

};

#endif
