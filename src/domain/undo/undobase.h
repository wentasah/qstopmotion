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

#ifndef UNDOBASE_H
#define UNDOBASE_H

#include <domain/domainfacade.h>

#include <QtGui/QUndoCommand>

/**
 * The UndoBase class for general undo functionality.
 */
class UndoBase : public QObject, public QUndoCommand
{
    Q_OBJECT
protected:

    /**
     * Sets up an Undo command object with the information needed to undo and
     * redo the add command.
     * @param df Domain facade for commands.
     */
    UndoBase(DomainFacade *df);

    /**
     * Destructor
     */
    ~UndoBase();

    /**
     * Abstract function for undoing the command represented by this undo object.
     */
    virtual void undo() = 0;

    /**
     * Abstract function for redoing (performing) the command represented by this
     * undo object.
     */
    virtual void redo() = 0;

protected:

    /**
     * The model to perform the redo command on.
     */
    DomainFacade *facade;

};

#endif
