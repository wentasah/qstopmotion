/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#ifndef EXTERNALCOMMANDDIALOG_H
#define EXTERNALCOMMANDDIALOG_H

#include <QtCore/QProcess>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>


class ExternalCommandDialog : public QWidget
{
    Q_OBJECT

public:
    ExternalCommandDialog(QWidget *parent = 0);
    void run(const QString &command, const QStringList &arguments);

private slots:
    void readFromStandardOutput();
    void readFromStandardError();
    void submitInputToProgram();
    void displayExitStatus(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QVBoxLayout *vboxLayout;
    QTextBrowser *textBrowser;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *submitButton;
    QPushButton *closeButton;
    QProcess *process;
};

#endif

