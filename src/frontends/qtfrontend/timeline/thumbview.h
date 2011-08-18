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

#ifndef THUMBVIEW_H
#define THUMBVIEW_H

#include "timeline.h"

#include <QDropEvent>
#include <QLabel>


class TimeLine;

/**
 * This abstract class represents the thumbviews in the timeline. It has
 * two subclasses: the ExposureThumbView and the SceneThumbView.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ThumbView : public QLabel
{
public:

    /**
     * Creates and sets up the thumbview class.
     * @param line the timeline for communicating.
     * @param parent the parent widget.
     * @param number the number of this thumbview in the timeline.
     */
    ThumbView(TimeLine *line, QWidget *parent, int number, const char *name = 0);

    /**
     * Cleans up after the widget.
     */
    virtual ~ThumbView();

    /**
     * Function to set the index of this ThumbView in the timeline
     * when the ThumbView change position.
     * @param index the new index for the ThumbView in the timeline.
     */
    virtual void setThumbIndex(int index);

    /**
     * Retrieves the index of this ThumbView in the timeline
     * @return the index for this ThumbView in the timeline
     */
    int getThumbIndex() const;

    /**
     * Abstract function for telling a exposurethumbview that it has one or more sounds
     * attatched to it.
     * @param hasSounds true if the frame has one or more sounds attatched to it.
     */
    virtual void setHasSounds(bool hasSounds);

    /**
     * Sets whether a exposurethumbview should be marked as selected.
     * @param selected whether this exposurethumbview should be showed as selected.
     */
    virtual void setSelected(bool selected);

    /**
     * Abstract function for telling a scenethumbview whether the scene is opened or
     * closed
     * @param isOpened true if the scene is opened.
     */
    virtual void setOpened(bool isOpened);

    /**
     * Notifies the thumbview that a drop have happened inside its borders.
     *
     * The reason it doesn't accept it itself is that the timeline need the information
     * so that it can autoscroll.
     * @param event information about the event.
     */
    virtual void contentsDropped(QDropEvent * event);

protected:
    /**
     * The timeline for communicating with it
     */
    TimeLine *timeLine;

    /**
     * Coordinate for calculating when a drag should start to give some slack
     */
    QPoint dragPos;

    /**
     * The index this thumb has in the timeline
     */
    int thumbIndex;
};

#endif
