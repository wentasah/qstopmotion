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

#include "externalcommanddialog.h"

#include "technical/util.h"

#include <QtCore/QtDebug>
#include <QtCore/QTextCodec>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>


ExternalCommandDialog::ExternalCommandDialog(QWidget *parent)
    : QWidget(parent)
{
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(9);

    textBrowser = new QTextBrowser(this);
    vboxLayout->addWidget(textBrowser);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);

    label = new QLabel(this);
    label->setText(tr("Input to program:"));
    hboxLayout->addWidget(label);

    lineEdit = new QLineEdit(this);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(submitInputToProgram()));
    lineEdit->setEchoMode(QLineEdit::Normal);
    hboxLayout->addWidget(lineEdit);

    submitButton = new QPushButton(this);
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitInputToProgram()));
    submitButton->setText(tr("Submit"));
    hboxLayout->addWidget(submitButton);

    closeButton = new QPushButton(this);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    closeButton->setEnabled(false);
    closeButton->setText(tr("Close"));
    hboxLayout->addWidget(closeButton);

    vboxLayout->addLayout(hboxLayout);

    process = new QProcess(this);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStandardOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readFromStandardError()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(displayExitStatus(int, QProcess::ExitStatus)));

    resize(QSize(593, 363).expandedTo(minimumSizeHint()));

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Output from external command"));
}


void ExternalCommandDialog::run(const QString &command)
{
    qDebug("ExternalCommandDialog::run --> Start");

    QString osCommand(Util::convertPathToOsSpecific(command));
    qDebug("ExternalCommandDialog::run --> Command");
    qDebug() << osCommand;

    process->start(osCommand);

    qDebug("ExternalCommandDialog::run --> End");

}


void ExternalCommandDialog::readFromStandardOutput()
{
    textBrowser->ensureCursorVisible();
    QTextCursor curs = textBrowser->textCursor();
    curs.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    curs.insertText(process->readAllStandardOutput());
}


void ExternalCommandDialog::readFromStandardError()
{
    textBrowser->ensureCursorVisible();
    QTextCursor curs = textBrowser->textCursor();
    curs.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    curs.insertText(process->readAllStandardError());
}


void ExternalCommandDialog::submitInputToProgram()
{
    if (lineEdit->isModified()) {
        process->write(lineEdit->text().toStdString().c_str());
        lineEdit->setText("");
    }
}


void ExternalCommandDialog::displayExitStatus(int exitCode, QProcess::ExitStatus)
{
    if (exitCode != 0) {
        QMessageBox::warning(this,
                             tr("Result"),
                             tr("Failed!"));
    } else {
        QMessageBox::information(this,
                                 tr("Result"),
                                 tr("Successfull!"));
    }

    lineEdit->setEnabled(false);
    submitButton->setEnabled(false);
    closeButton->setEnabled(true);
}

