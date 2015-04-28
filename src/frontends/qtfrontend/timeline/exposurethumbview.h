/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#ifndef EXPOSURETHUMBVIEW_H
#define EXPOSURETHUMBVIEW_H

#include "frontends/qtfrontend/timeline/timeline.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QPaintEvent>


/**
 * Widget representing a frame in the animation.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ExposureThumbView : public ThumbView
{
public:
    /**
     * Creates and sets up the exposurethumbview.
     * @param line the timeline for communicating with it.
     * @param parent the parent widget.
     * @param number the number of this widget in the timeline.
     */
    ExposureThumbView(TimeLine *line, QWidget *parent = 0, int number = 0, const char *name = 0);

    /**
     * Cleans up after the widget.
     */
    ~ExposureThumbView();

    /**
     * Overloaded event function to recieve mousepress-events.
     * @param e information about the mousepress-event.
     */
    void mousePressEvent(QMouseEvent * e);

    /**
     * Overloaded event function to recieve mouserelease-events.
     * @param e information about the mouserelease-event.
     */
    void mouseReleaseEvent(QMouseEvent * e);

    /**
     * Overloaded event function to recieve mouseMoveEvents.
     * @param me information about the mouseMoveEvent.
     */
    void mouseMoveEvent(QMouseEvent *me);

    /**
     * Overloaded event function to recieve events when the user double clicks
     * in the thumbview. Double clicks here cause the preferences menu for this
     * frame to be shown.
     * @param e information about the mouseDoubleClickEvent.
     */
    void mouseDoubleClickEvent(QMouseEvent * e);

    /**
     * Function to set the index of this ThumbView in the timeline
     * when the ThumbView change position.
     * @param index the new index for the ThumbView in the timeline.
     */
    void setThumbIndex(int index);

    /**
     * Sets whether this thumbview should be selected.
     * @param selected whether this frame is selected.
     */
    void setSelected(bool selected);

    /**
     * Notifies the exposurethumbview that a drop have happened inside its borders.
     * @param event information about the event.
     */
    void contentsDropped(QDropEvent * event);

protected:
    /**
     * Event function which paints the widget.
     */
    void paintEvent(QPaintEvent *);

private:
    QString stringNumber;

    /** The width the text should have. (Cached for efficiency reasons)*/
    int textWidth;

    /** Specifies whether this frame is selected. */
    bool selected;

    /** Starts an uri drag of the picture in this label. */
    void startDrag();
};

#endif
