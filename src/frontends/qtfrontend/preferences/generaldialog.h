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
 *  MERCHANPageILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "frontends/frontend.h"
#include "frontends/qtfrontend/preferences/controlerwidget.h"
#include "frontends/qtfrontend/preferences/exportwidget.h"
#include "frontends/qtfrontend/preferences/generalwidget.h"
#include "frontends/qtfrontend/preferences/grabberwidget.h"
#include "frontends/qtfrontend/preferences/importwidget.h"
#include "frontends/qtfrontend/preferences/projectwidget.h"
#include "frontends/qtfrontend/preferences/transformwidget.h"


/**
 * The general preferences dialog.
 *
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class GeneralDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Sets up the filepreview-label.
     * @param f frontend of the application.
     * @param parent the parent of the widget.
     */
    GeneralDialog(Frontend *f, QWidget *parent = 0);

private:
    Frontend        *frontend;

    QTreeWidget     *pageTree;
    QHBoxLayout     *pageLayout;

    GeneralWidget   *generalSettingsPage;
    ProjectWidget   *projectValuePage;
    ImportWidget    *imageImportPage;
    TransformWidget *imageTransformationPage;
    ExportWidget    *videoExportPage;
    GrabberWidget   *grabberSelectPage;
    ControlerWidget *cameraControlPage;
    QPushButton     *applyButton;
    QPushButton     *closeButton;
    QPushButton     *helpButton;

    int              activePage;

    void makeGeneralSettingsPage();
    void makeProjectValuePage();
    void makeImageImportPage();
    void makeImageTransformPage();
    void makeVideoExportPage();
    void makeGrabberSelectPage();
    void makeCameraControlPage();

private slots:
    /**
     * Click in the page tree
     * @param pageItem The specified item is the item that was clicked, or 0 if no item was clicked.
     * @param column The column is the item's column that was clicked.
     */
    void itemClicked(QTreeWidgetItem *pageItem,
                     int column);

    void apply();
    void close();
    void help();
    void finish(int result);
};

#endif
