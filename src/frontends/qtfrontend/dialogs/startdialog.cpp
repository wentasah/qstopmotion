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

#include "startdialog.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QVBoxLayout>


StartDialog::StartDialog(Frontend *f,
                         QString   lpp,
                         QWidget  *parent)
    : QDialog(parent)

{
    frontend = f;
    selectedPossibility = 1;
    lastProjectPath.append(lpp);

    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->setSpacing(0);

    QString iconFile(frontend->getGraphicsDirName());

    QLabel *logoImage = new QLabel;
    iconFile.append(QLatin1String("qstopmotion_logo_60.png"));
    logoImage->setPixmap(QPixmap(iconFile));
    logoImage->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    newProjectImage = new QLabel;
    iconFile.clear();
    iconFile.append(frontend->getPicturesDirName());
    iconFile.append(QLatin1String("filenew.png"));
    newProjectImage->setPixmap(QPixmap(iconFile));
    newProjectImage->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    newProjectLabel = new ClickableLabel(this);
    newProjectLabel->setText(tr("<h2>Create a new Project</h2>") +
                             tr("Start a new Project."));
    // newProjectLabel->setBuddy(newProjectImage);

    newProjectImage->setFixedHeight(80);
    newProjectImage->setFixedWidth(80);
    newProjectLabel->setFixedHeight(80);
    newProjectLabel->setFixedWidth(400);

    connect(newProjectLabel, SIGNAL(clicked()), SLOT(newClick()));

    inputLayout->addWidget(newProjectImage, 0, 0);
    inputLayout->addWidget(newProjectLabel, 0, 1);

    if (!lastProjectPath.isEmpty()) {
        lastProjectImage = new QLabel;
        iconFile.clear();
        iconFile.append(frontend->getPicturesDirName());
        iconFile.append(QLatin1String("project1.png"));
        lastProjectImage->setPixmap(QPixmap(iconFile));
        lastProjectImage->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        lastProjectLabel = new ClickableLabel(this);
        lastProjectLabel->setText(tr("<h2>Open last Project</h2>") +
                                  tr("Project path: ") + lastProjectPath );
        // lastProjectLabel->setBuddy(lastProjectImage);

        lastProjectImage->setFixedHeight(80);
        lastProjectImage->setFixedWidth(80);
        lastProjectLabel->setFixedHeight(80);
        lastProjectLabel->setFixedWidth(400);

        connect(lastProjectLabel, SIGNAL(clicked()), SLOT(lastClick()));

        inputLayout->addWidget(lastProjectImage, 1, 0);
        inputLayout->addWidget(lastProjectLabel, 1, 1);
    }

    openProjectImage = new QLabel;
    iconFile.clear();
    iconFile.append(frontend->getPicturesDirName());
    iconFile.append(QLatin1String("fileopen.png"));
    openProjectImage->setPixmap(QPixmap(iconFile));
    openProjectImage->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    openProjectLabel = new ClickableLabel();
    openProjectLabel->setText(tr("<h2>Open project file</h2>") +
                              tr("Open and continue a existing project."));
    // openProjectLabel->setBuddy(openProjectImage);

    openProjectImage->setFixedHeight(80);
    openProjectImage->setFixedWidth(80);
    openProjectLabel->setFixedHeight(80);
    openProjectLabel->setFixedWidth(400);

    connect(openProjectLabel, SIGNAL(clicked()), SLOT(openClick()));

    okButton = new QPushButton(tr("&OK"));
    closeButton = new QPushButton(tr("&Close"));

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    inputLayout->addWidget(openProjectImage, 2, 0);
    inputLayout->addWidget(openProjectLabel, 2, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(logoImage);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    this->newClick();
}


void StartDialog::newClick()
{
    qDebug() << "StartDialog::newClick --> Start";

    newProjectImage->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    newProjectLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    if (!lastProjectPath.isEmpty()) {
        lastProjectImage->setFrameStyle(QFrame::StyledPanel);
        lastProjectLabel->setFrameStyle(QFrame::StyledPanel);
    }
    openProjectImage->setFrameStyle(QFrame::StyledPanel);
    openProjectLabel->setFrameStyle(QFrame::StyledPanel);

    selectedPossibility = 1;

    qDebug() << "StartDialog::newClick --> End";
}


void StartDialog::lastClick()
{
    qDebug() << "StartDialog::lastClick --> Start";

    newProjectImage->setFrameStyle(QFrame::StyledPanel);
    newProjectLabel->setFrameStyle(QFrame::StyledPanel);
    if (!lastProjectPath.isEmpty()) {
        lastProjectImage->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        lastProjectLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    }
    openProjectImage->setFrameStyle(QFrame::StyledPanel);
    openProjectLabel->setFrameStyle(QFrame::StyledPanel);

    selectedPossibility = 2;

    qDebug() << "StartDialog::lastClick --> End";
}


void StartDialog::openClick()
{
    qDebug() << "StartDialog::openClick --> Start";

    newProjectImage->setFrameStyle(QFrame::StyledPanel);
    newProjectLabel->setFrameStyle(QFrame::StyledPanel);
    if (!lastProjectPath.isEmpty()) {
        lastProjectImage->setFrameStyle(QFrame::StyledPanel);
        lastProjectLabel->setFrameStyle(QFrame::StyledPanel);
    }
    openProjectImage->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    openProjectLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    selectedPossibility = 3;

    qDebug() << "StartDialog::openClick --> End";
}


int StartDialog::getSelectedPossibility()
{
    return this->selectedPossibility;
}
