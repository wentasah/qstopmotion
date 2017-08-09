/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "frontends/frontend.h"
#include "frontends/qtfrontend/elements/clickablelabel.h"


class StartDialog : public QDialog
{
    Q_OBJECT

public:
    enum descriptionType {
        ProjectDescription,
        SceneDescription,
        TakeDescription
    };

    /**
     * Constructor
     * @param lastProjectPath Path of the last project
     * @param parent Parent of the dialog
     */
    StartDialog(Frontend *f,
                QString   lastProjectPath,
                QWidget  *parent = 0);

    /**
     * Get the choice of the user.
     * @return Choice of the user.
     */
    int getSelectedPossibility();

private slots:
    void newClick();
    void lastClick();
    void openClick();
    void help();

private:
    Frontend       *frontend;
    QLabel         *newProjectImage;
    ClickableLabel *newProjectLabel;
    QLabel         *lastProjectImage;
    ClickableLabel *lastProjectLabel;
    QLabel         *openProjectImage;
    ClickableLabel *openProjectLabel;
    QPushButton    *okButton;
    QPushButton    *closeButton;
    QPushButton    *helpButton;
    int             selectedPossibility;
    int             defaultFrameStyle;
    QString         lastProjectPath;
};

#endif // STARTDIALOG_H
