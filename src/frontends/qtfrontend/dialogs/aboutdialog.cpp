/******************************************************************************
 *  Copyright (C) 2005-2014 by                                                *
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

#include "aboutdialog.h"

#include "licence.h"
#include "technical/preferencestool.h"

#include <QtGui>


AboutDialog::AboutDialog(Frontend *f,
                         QWidget *parent)
    : QDialog(parent)
{
    frontend = f;

    QString iconFile(frontend->getGraphicsDirName());
    iconFile.append(QLatin1String("qstopmotion_logo_60.png"));

    tabWidget = new QTabWidget;

    QWidget *aboutWidget = new QWidget;
    QVBoxLayout *aboutLayout = new QVBoxLayout;
    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(iconFile));
    logoLabel->setScaledContents(true);
    aboutLayout->addWidget(logoLabel);
    aboutLayout->addWidget(new QLabel(
                          "<p>" + tr("This is the qStopMotion application for creating stop motion animations.") + "</p>"
                          "<p>" + tr("Version: ") + PreferencesTool::applicationVersion + "</p>"
                          "<p>&copy; 2010-2014, Ralf Lange, longsoft.de </p>"
                          "<hr />"));
    aboutLayout->addWidget(new QLabel(
                          "<p>" + tr("qStopMotion is a fork of stopmotion for linux.") + "</p>"
                          "<p>http://developer.skolelinux.no/info/studentgrupper/2005-hig-stopmotion/index.php </p>"
                          "<p>" + tr("Version: ") + "0.6.2 </p>"
                          "<p>&copy; 2005-2008, Bjørn Erik Nilsen " + tr("and") + " Fredrik Berg Kjølstad </p>"));
    aboutWidget->setLayout(aboutLayout);
    tabWidget->addTab(aboutWidget, tr("&About"));

    QTextEdit *autorsText = new QTextEdit;
    autorsText->setHtml(
                "<p><b>" + tr("Main developers") + " - qStopMotion</b><br>"
                "Ralf Lange &lt;ralf.lange@longsoft.de&gt;<br>"
                "<p><b>" + tr("Main developers") + " - stopmotion for Linux</b><br>"
                "Bjørn Erik Nilsen &lt;bjoern.nilsen@bjoernen.com&gt;<br>"
                "Fredrik Berg Kjølstad &lt;fredrikbk@hotmail.com&gt;<br>"
                "<p><b>" + tr("Contributors") + " - stopmotion for Linux</b><br>"
                "Eric Stolten &lt;stoltene2@gmail.com&gt;<br>"
                "Josh Green &lt;josh@resonance.org&gt;<br>"
                "</p>"
                );
    autorsText->setReadOnly(true);
    autorsText->setBackgroundRole(backgroundRole());
    tabWidget->addTab(autorsText, tr("A&uthors"));

    QTextEdit *thanksText = new QTextEdit;
    thanksText->setHtml(
                "<p><b>" + tr("Translation") + "</b><br>"
                "Norma Ludwig (" + tr("French") + " - qStopMotion)<br>"
                "Guillaume B &lt;littletux@zarb.org&gt; (" + tr("French") + " - stopmotion for Linux)<br>"
                "David Lamhauge &lt;davidlamhauge@gmail.com&gt; (" + tr("Danish") + ")<br>"
                "David Rylander &lt;david@rylanderanimation.se&gt; (" + tr("Swedish") + ")<br>"
                "Ralf Lange &lt;ralf.lange@longsoft.de&gt; (" + tr("German") + ")<br>"
                /*
                "George Helebrant &lt;helb@skatekralovice.com&gt; (" + tr("Czech") + ")<br>"
                "Gorazd Bizjak and Matej Lavreni &lt;info@zapstudio.net&gt; (" + tr("Slovenian") + ")<br>"
                "José Jorge &lt;jjorge@free.fr&gt; (" + tr("Portuguese") + ")<br>"
                "Manuel Quiñones &lt;manuel.por.aca@gmail.com&gt; (" + tr("Spanish") + ")<br>"
                "Koray Löker &lt;loker@pardus.org.tr&gt; (" + tr("Turkish") + ")<br>"
                "Diego Giordano &lt;fusion_machine@tin.it&gt; (" + tr("Italian") + ")<br>"
                "Matteo Comisso &lt;teomatteo8.9@hotmail.it&gt; (" + tr("Italian") + ")<br>"
                "</p>"
                "<p><b>" + tr("Logo") + "</b><br>"
                "Gorazd Bizjak &lt;gorazd@zapstudio.net&gt;<br>"
                "<p><b>" + tr("Coordinating") + "</b><br>"
                "Herman Robak &lt;herman@skolelinux.no&gt;<br>"
                "Øyvind Kolås &lt;pippin@gimp.org&gt;</p>"
                "<p><b>" + tr("Testing") + "</b><br>"
                "Tore Sinding Bekkedal &lt;toresbe@ifi.uio.no&gt;<br>"
                "Finn Arne Johansen &lt;faj@bzz.no&gt;<br>"
                "Halvor Borgen &lt;halvor.borgen@hig.no&gt;<br>"
                "Bjørn Are Hansen &lt;post@bahansen.net&gt;<br>"
                "John Steinar Bildøy &lt;johnsbil@haldenfriskole.no&gt;<br>"
                "Ole-Anders Andreassen &lt;ole-anders.andreassen@sunndal.kommune.no&gt;<br>"
                */
                "</p>"
                );
    thanksText->setReadOnly(true);
    thanksText->setBackgroundRole(backgroundRole());
    tabWidget->addTab(thanksText, tr("&Thanks To"));

    QWidget *licenceWidget = new QWidget;
    QVBoxLayout *licenceLayout = new QVBoxLayout;
    licenceLayout->addWidget(new QLabel(tr("This program is distributed under the terms of the GPL v2.")));
    QTextEdit *licenceText = new QTextEdit;
    licenceText->setReadOnly(true);
    licenceText->setPlainText(licence);
    licenceLayout->addWidget(licenceText);
    licenceWidget->setLayout(licenceLayout);
    tabWidget->addTab(licenceWidget, tr("&Licence Agreement"));

    QString version;
    QWidget *systemWidget = new QWidget;
    QVBoxLayout *systemLayout = new QVBoxLayout;
    // Add line contaning the Qt version number
    systemLayout->addWidget(new QLabel(tr("Qt runtime version: ") + qVersion()));

    // Add line containing the OS version number
    version.append(tr("Operating system name and version: "));
#ifdef Q_WS_WIN
    // Windows version
    switch(QSysInfo::WindowsVersion) {
    case QSysInfo::WV_NT:
        version.append("Windows NT (4.0)");
        break;
    case QSysInfo::WV_2000:
        version.append("Windows 2000 (5.0)");
        break;
    case QSysInfo::WV_XP:
        version.append("Windows XP (5.1)");
        break;
    case QSysInfo::WV_2003:
        version.append("Windows 2003 (5.2)");
        break;
    case QSysInfo::WV_VISTA:
        version.append("Windows Vista (6.0)");
        break;
    case QSysInfo::WV_WINDOWS7:
        version.append("Windows 7 (6.1)");
        break;
    }
#endif
#ifdef Q_WS_MAC
    // Apple OS X version
    switch(QSysInfo::MacintoshVersion) {
    case QSysInfo::MV_10_3:
        version.append("Mac OS X 10.3 (Panther)");
        break;
    case QSysInfo::MV_10_3:
        version.append("Mac OS X 10.4 (Tiger)");
        break;
    case QSysInfo::MV_10_3:
        version.append("Mac OS X 10.5 (Leopard)");
        break;
    case QSysInfo::MV_10_3:
        version.append("Mac OS X 10.6 (Snowleopard)");
        break;
    }
#endif
#ifdef Q_WS_X11
    // Linux version
    QProcess uname;
    uname.start("uname -ro");
    if (uname.waitForStarted()) {
        if (uname.waitForFinished()) {
            version.append(uname.readAll());
        }
    }
#endif
    systemLayout->addWidget(new QLabel(version));
    QString buildTime(QString(tr("Build time: %1 %2")).arg(__DATE__).arg(__TIME__));
    systemLayout->addWidget(new QLabel(buildTime));
    systemLayout->addStretch();
    systemWidget->setLayout(systemLayout);
    tabWidget->addTab(systemWidget, tr("&System Info"));

    QPushButton *okButton = new QPushButton(tr("OK"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("About"));
    setMinimumWidth(550);
}

