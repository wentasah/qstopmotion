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

#ifndef DESCRIPTIONDIALOG_H
#define DESCRIPTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "frontends/frontend.h"


class DescriptionDialog : public QDialog
{
    Q_OBJECT;

public:
    enum descriptionType {
        ProjectDescription,
        SceneDescription,
        TakeDescription
    };

    /**
     * Constructor
     * @param f The frontend of the application
     * @param type Type of the dialog
     * @param parent Parent of the dialog
     */
    DescriptionDialog(Frontend *f, descriptionType type, QWidget *parent = 0);

    /**
     * Get the content of the project description edit line.
     * @return Project description text.
     */
    const QString getProjectDescription();

    /**
     * Set the content of the project description edit line.
     * @param descr Description of the project.
     */
    void setProjectDescription(const QString &descr);

    /**
     * Get the content of the scene description edit line.
     * @return Scene description text.
     */
    const QString getSceneDescription();

    /**
     * Set the content of the scene description edit line.
     * @param descr Description of the scene.
     */
    void setSceneDescription(const QString &descr);

    /**
     * Get the content of the take description edit line.
     * @return Take description text.
     */
    const QString getTakeDescription();

    /**
     * Set the content of the take description edit line.
     * @param descr Description of the take.
     */
    void setTakeDescription(const QString &descr);

private slots:
    void checkProjectText(const QString &text);
    void checkSceneText(const QString &text);
    void checkTakeText(const QString &text);

private:
    Frontend    *frontend;
    QLabel      *projectDescrLabel;
    QLineEdit   *projectDescrEdit;
    QLabel      *sceneDescrLabel;
    QLineEdit   *sceneDescrEdit;
    QLabel      *takeDescrLabel;
    QLineEdit   *takeDescrEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // DESCRIPTIONDIALOG_H
