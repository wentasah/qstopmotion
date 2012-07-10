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

#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include "frontends/frontend.h"
#include "frontends/qtfrontend/preferences/exportwidget.h"
// #include "frontends/qtfrontend/preferences/generalwidget.h"
#include "frontends/qtfrontend/preferences/importwidget.h"
#include "frontends/qtfrontend/preferences/projectwidget.h"

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>


/**
 * The project preferences dialog.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class ProjectDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Sets up the filepreview-label.
     * @param f frontend of the application.
     * @param parent the parent of the widget.
     */
    ProjectDialog(Frontend *f, QWidget *parent = 0);

private:
    Frontend      *frontend;
    // GeneralWidget *generalSettingsTab;
    // ProjectWidget *projectValueTab;
    ImportWidget  *imageImportTab;
    ExportWidget  *videoExportTab;
    QTabWidget    *tabWidget;
    QPushButton   *applyButton;
    QPushButton   *closeButton;

    // void makeGeneralSettingsTab();
    // void makeProjectValueTab();
    void makeImageImportTab();
    void makeVideoExportTab();

private slots:
    void apply();
    void close();
    void finish(int result);
};

#endif
