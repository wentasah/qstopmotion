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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#include "externalcommanddialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextCursor>

#include "technical/util.h"


ExternalCommandDialog::ExternalCommandDialog(Frontend *f, QWidget *parent)
    : QWidget(parent)
{
    frontend = f;

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
    submitButton->setDefault(false);
    submitButton->setText(tr("Submit"));
    hboxLayout->addWidget(submitButton);

    helpButton = new QPushButton(tr("Help"), this);
    helpButton->setDefault(false);
    connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
    helpButton->setShortcut(QKeySequence::HelpContents);
    hboxLayout->addWidget(helpButton);

    closeButton = new QPushButton(this);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    closeButton->setDefault(true);
    closeButton->setEnabled(false);
    closeButton->setText(tr("Close"));
    hboxLayout->addWidget(closeButton);

    vboxLayout->addLayout(hboxLayout);

    process = new QProcess(this);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStandardOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readFromStandardError()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(checkExitStatus(int, QProcess::ExitStatus)));

    resize(QSize(593, 363).expandedTo(minimumSizeHint()));

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Output from external command"));
}


void ExternalCommandDialog::run(const QString &command, const QStringList &arguments)
{
    QList<ExternalCommand> commands;
    commands.append(ExternalCommand(command, arguments));
    run(commands);
}

void ExternalCommandDialog::run(const QList<ExternalCommand> &commands)
{
    if (commands.isEmpty()) {
        qWarning() << "ExternalCommandDialog::run Nothing commands to run";
        return;
    }

    qDebug() << "ExternalCommandDialog::run --> Start";
    externalCommands = commands;
    runNextCommand();

    qDebug() << "ExternalCommandDialog::run --> End";
}


void ExternalCommandDialog::readFromStandardOutput()
{
    textBrowser->ensureCursorVisible();
    QTextCursor curs = textBrowser->textCursor();
    curs.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    const QByteArray processOutput = process->readAllStandardOutput();
    curs.insertText(processOutput);
    qDebug() << processOutput;
}


void ExternalCommandDialog::readFromStandardError()
{
    textBrowser->ensureCursorVisible();
    QTextCursor curs = textBrowser->textCursor();
    curs.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    const QByteArray processError = process->readAllStandardError();
    curs.insertText(processError);
    qDebug() << processError;
}


void ExternalCommandDialog::submitInputToProgram()
{
    if (lineEdit->isModified()) {
        process->write(lineEdit->text().toStdString().c_str());
        lineEdit->setText("");
    }
}


void ExternalCommandDialog::checkExitStatus(int exitCode, QProcess::ExitStatus exitStatus)
{
    if ((exitCode != 0) || (exitStatus != QProcess::NormalExit)) {
        qCritical() << "command has been failed. exitCode:" << exitCode
                    << "exitStatus:" << exitStatus << "error:"
                    << process->errorString();
        displayFinishStatus(false);
        return;
    }

    if (externalCommands.isEmpty()) {
        qDebug() << "all commands have been successfully finished";
        displayFinishStatus(true);
        return;
    }

    bool started = runNextCommand();
    if (!started) {
        qCritical() << "commands execution is interrupted";
        displayFinishStatus(false);
        return;
    }
}

void ExternalCommandDialog::displayFinishStatus(bool successfully)
{
    if (!successfully) {
        QMessageBox::warning(this,
                             tr("Result"),
                             tr("Failed!"));
    } else {
        QMessageBox::information(this,
                                 tr("Result"),
                                 tr("Successful!"));
    }

    lineEdit->setEnabled(false);
    submitButton->setEnabled(false);
    closeButton->setEnabled(true);
    externalCommands.clear();
}


void ExternalCommandDialog::help()
{
    qDebug() << "ExternalCommandDialog::help --> Start";

    frontend->openOnlineHelp("#export");

    qDebug() << "ExternalCommandDialog::help --> End";
}

bool ExternalCommandDialog::runNextCommand()
{
    if (externalCommands.isEmpty()) {
        return false;
    }
    if (process->state() != QProcess::NotRunning) {
        qCritical() << "Coulnd't run next command while previous hasn't been finished."
                    << "State:" << process->state();
        return false;
    }

    ExternalCommand command = externalCommands.takeFirst();
    QString osCommand(Util::convertPathToOsSpecific(command.first));
    qDebug() << "ExternalCommandDialog::runNextCommand --> Command";
    qDebug() << osCommand << command.second;

    process->start(osCommand, command.second);

    return true;
}
