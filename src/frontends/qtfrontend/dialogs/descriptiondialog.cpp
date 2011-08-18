/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
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

#include "descriptiondialog.h"

#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

DescriptionDialog::DescriptionDialog(descriptionType type, QWidget *parent)
    : QDialog(parent)

{
    projectDescrLabel = new QLabel(tr("&Project Description:"));
    projectDescrEdit = new QLineEdit;
    projectDescrLabel->setBuddy(projectDescrEdit);
    sceneDescrLabel = new QLabel(tr("&Scene Description:"));
    sceneDescrEdit = new QLineEdit;
    sceneDescrLabel->setBuddy(sceneDescrEdit);
    takeDescrLabel = new QLabel(tr("&Take Description:"));
    takeDescrEdit = new QLineEdit;
    takeDescrLabel->setBuddy(takeDescrEdit);

    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));

    connect(projectDescrEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableOkButton(const QString &)));
    connect(sceneDescrEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableOkButton(const QString &)));
    connect(takeDescrEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableOkButton(const QString &)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(projectDescrLabel, 0, 0);
    inputLayout->addWidget(projectDescrEdit, 0, 1);
    inputLayout->addWidget(sceneDescrLabel, 1, 0);
    inputLayout->addWidget(sceneDescrEdit, 1, 1);
    inputLayout->addWidget(takeDescrLabel, 2, 0);
    inputLayout->addWidget(takeDescrEdit, 2, 1);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    switch (type) {
    case ProjectDescription:
        setWindowTitle(tr("Project Decription"));
        projectDescrEdit->setEnabled(true);
        sceneDescrEdit->setEnabled(true);
        takeDescrEdit->setEnabled(true);
        break;
    case SceneDescription:
        setWindowTitle(tr("Scene Decription"));
        projectDescrEdit->setEnabled(false);
        sceneDescrEdit->setEnabled(true);
        takeDescrEdit->setEnabled(true);
        break;
    case TakeDescription:
        setWindowTitle(tr("Take Decription"));
        projectDescrEdit->setEnabled(false);
        sceneDescrEdit->setEnabled(false);
        takeDescrEdit->setEnabled(true);
        break;
    default:
        break;
    }

    setFixedHeight(sizeHint().height());
}



const QString DescriptionDialog::getProjectDescription()
{
    return this->projectDescrEdit->text();
}


void DescriptionDialog::setProjectDescription(const QString &descr)
{
    this->projectDescrEdit->setText(descr);
}


const QString DescriptionDialog::getSceneDescription()
{
    return this->sceneDescrEdit->text();
}


void DescriptionDialog::setSceneDescription(const QString &descr)
{
    this->sceneDescrEdit->setText(descr);
}


const QString DescriptionDialog::getTakeDescription()
{
    return this->takeDescrEdit->text();
}


void DescriptionDialog::setTakeDescription(const QString &descr)
{
    this->takeDescrEdit->setText(descr);
}
