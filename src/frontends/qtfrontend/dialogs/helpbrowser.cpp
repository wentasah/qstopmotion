/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#include "helpbrowser.h"

#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


HelpBrowser::HelpBrowser(Frontend *f,
                         QWidget *parent)
    : QDialog(parent)
{
    QUrl url;

    frontend = f;

    this->setWindowTitle(tr("qStopMotion Help Browser"));
    this->setMinimumSize(500, 300);
    // Enable help window for modal dialoges
    this->setAttribute(Qt::WA_GroupLeader);

    backwardButton = new QPushButton(tr("&Backward"));
    homeButton = new QPushButton(tr("&Home"));
    forwardButton = new QPushButton(tr("&Forward"));
    searchEdit = new QLineEdit;
    textBrowser = new QTextBrowser;
    closeButton = new QPushButton(tr("&Close"));

    connect(backwardButton, SIGNAL(clicked()), textBrowser, SLOT(backward()));
    connect(homeButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
    connect(forwardButton, SIGNAL(clicked()), textBrowser, SLOT(forward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(backwardButton);
    topLayout->addWidget(homeButton);
    topLayout->addWidget(forwardButton);
    topLayout->addWidget(searchEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    QString activeLocale;
    frontend->getPreferences()->getStringPreference("preferences", "language", activeLocale);
    QString localeManualFile(frontend->getManualDirName());
    // localeManualFile.append("/");
    localeManualFile.append(activeLocale);
    localeManualFile.append("/");
    localeManualFile.append(QLatin1String("index.html"));

    if (QFile::exists(localeManualFile)) {
        // A translation of the manual for the selected language exists
        url = QUrl::fromLocalFile(localeManualFile);
    }
    else {
        // Use the english manual as default
        QString defaultManualFile(frontend->getManualDirName());
        defaultManualFile.append("/");
        defaultManualFile.append(QLatin1String("index.html"));
        url = QUrl::fromLocalFile(defaultManualFile);
    }

    textBrowser->setSource(url);
}
