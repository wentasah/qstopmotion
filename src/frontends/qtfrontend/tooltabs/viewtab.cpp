/******************************************************************************
 *  Copyright (C) 2010-2013 by                                                *
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

#include "viewtab.h"

#include "domain/domainfacade.h"

#include <QtCore/QtDebug>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>


ViewTab::ViewTab(Frontend *f,
                 QWidget *parent) :
    QWidget(parent)
{
    frontend           = f;
    firstGroupBox      = 0;
    firstIcon          = 0;
    firstSlider        = 0;
    firstButton        = 0;

    secondGroupBox     = 0;
    secondButton       = 0;

    thirdGroupBox      = 0;
    thirdButton        = 0;

    this->setObjectName("ViewTab");

    makeGUI();
}


void ViewTab::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;

    firstGroupBox = new QGroupBox(QString("firstGroupBox"));
    // firstGroupBox->setFlat(true);

    firstIcon = new QLabel();
    iconFile.append(QLatin1String("frames.png"));
    firstIcon->setPixmap(QPixmap(iconFile));

    firstSlider = new QSlider(Qt::Horizontal);

    firstButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("tobegin.png"));
    firstButton->setIcon(QPixmap(iconFile));
    // firstButton->setFlat(true);
    // firstButton->setFocusPolicy( Qt::NoFocus );
    connect(firstButton, SIGNAL(clicked()), this, SLOT(firstSlot()));
    firstButton->setEnabled(false);

    QVBoxLayout *firstLayout = new QVBoxLayout;
    firstLayout->setMargin(4);
    // firstLayout->setSpacing(2);
    // firstLayout->addStretch(1);
    firstLayout->addWidget(firstIcon);
    firstLayout->addWidget(firstSlider);
    firstLayout->addStretch(10);
    firstLayout->addWidget(firstButton);
    firstLayout->addStretch(10);
    firstGroupBox->setLayout(firstLayout);

    secondGroupBox = new QGroupBox(QString("secondGroupBox"));
    // secondGroupBox->setFlat(true);

    secondButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("tobegin.png"));
    secondButton->setIcon(QPixmap(iconFile));
    // secondButton->setFlat(true);
    // secondButton->setFocusPolicy( Qt::NoFocus );
    connect(secondButton, SIGNAL(clicked()), this, SLOT(secondSlot()));
    secondButton->setEnabled(false);

    QVBoxLayout *secondLayout = new QVBoxLayout;
    secondLayout->setMargin(4);
    // secondLayout->setSpacing(2);
    // secondLayout->addStretch(1);
    secondLayout->addWidget(secondButton);
    secondLayout->addStretch(10);
    secondGroupBox->setLayout(secondLayout);

    thirdGroupBox = new QGroupBox(QString("thirdGroupBox"));
    // thirdGroupBox->setFlat(true);

    thirdButton = new QPushButton;
    iconFile.clear();
    iconFile.append(frontend->getIconsDirName());
    iconFile.append(QLatin1String("tobegin.png"));
    thirdButton->setIcon(QPixmap(iconFile));
    // thirdButton->setFlat(true);
    // thirdButton->setFocusPolicy( Qt::NoFocus );
    connect(thirdButton, SIGNAL(clicked()), this, SLOT(thirdSlot()));
    thirdButton->setEnabled(false);

    QVBoxLayout *thirdLayout = new QVBoxLayout;
    thirdLayout->setMargin(4);
    // thirdLayout->setSpacing(2);
    // thirdLayout->addStretch(1);
    thirdLayout->addWidget(thirdButton);
    thirdLayout->addStretch(10);
    thirdGroupBox->setLayout(thirdLayout);

    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(firstGroupBox);
    tabLayout->addWidget(secondGroupBox);
    tabLayout->addWidget(thirdGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

}


void ViewTab::retranslateStrings()
{
    /*
    QString infoText = tr("<h4>First Button</h4>");
    firstButton->setToolTip(infoText);
    firstButton->setWhatsThis(infoText);

    infoText = tr("<h4>Second Button</h4>");
    secondButton->setToolTip(infoText);
    secondButton->setWhatsThis(infoText);

    infoText = tr("<h4>Third Button</h4>");
    thirdButton->setToolTip(infoText);
    thirdButton->setWhatsThis(infoText);
    */
}


void ViewTab::initialize()
{
    /*
    PreferencesTool *pref = PreferencesTool::get();
    int active = -1;

    int numEncoders = pref->getNumberEncoders();

    encoderTable->setRowCount(numEncoders);

    if (numEncoders > 0 ) {
        QString activeId = pref->getActiveEncoder();
        int idx = 0;

        for (int i = 0; i < numEncoders; ++i) {
            QString id(QString("encoder%1").arg(i));
            QString name;
            QString desc;
            QString start;
            QString stop;
            QString filename;

            if ( pref->getEncoder(id, name, desc, start, stop, filename) ) {
                encoderTable->setItem(i, 0, new QTableWidgetItem(name) );
                encoderTable->setItem(i, 1, new QTableWidgetItem(desc) );
                startEncoderStrings.append(start);
                stopEncoderStrings.append(stop);
                outputFiles.append(filename);

                if ( id.compare(activeId) == 0 )
                    active = idx;

                idx++;
            }
        }
    }

    if (active > -1) {
        encoderTable->setCurrentCell(active, 0);
    }
    */
}

/*
void ViewTab::resizeEvent(QResizeEvent *event)
{
    qDebug("ViewTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ViewTab::resizeEvent --> End");
}
*/

void ViewTab::apply()
{
    /*
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("encoder%1").arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("encoder%1").arg(i),
                             encoderTable->item(i, 0)->text(),
                             encoderTable->item(i, 1)->text(),
                             startEncoderStrings[i],
                             stopEncoderStrings[i],
                             outputFiles[i]);
        }
    }
    else {
        pref->setNumberEncoders(0);
        pref->setActiveEncoder(QString());
    }
    */
}


void ViewTab::firstSlot()
{
}


void ViewTab::secondSlot()
{
}


void ViewTab::thirdSlot()
{
}
