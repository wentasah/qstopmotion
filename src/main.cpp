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

#include <QBitArray>
#include <QDebug>
#include <QDir>
#include <QTime>

#include "frontends/qtfrontend/qtfrontend.h"

/* g_verboseOutput is used for output verbose information */
bool g_verboseOutput = false;

namespace {
    FILE *logFileStream = Q_NULLPTR;
    FILE *logConsoleStream = Q_NULLPTR;
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QBitArray logLevels(5, true); // see size of enum QtMsgType
#else
    QBitArray logLevels(4, true); // see size of enum QtMsgType
#endif
}

void writeToLog(FILE *stream, const QString &level,
                const QMessageLogContext &context, const QString &msg)
{
    if (stream == Q_NULLPTR) {
        return;
    }

    QTime time = QTime::currentTime();
    QString formatedTime = time.toString("hh:mm:ss.zzz");
    fprintf(stream, "%s ", qPrintable(formatedTime));

#ifdef QT_DEBUG
    fprintf(stream, "%s: %s (`%s:%u, %s`)\n", level.toLocal8Bit().constData(),
            msg.toLocal8Bit().constData(), context.file, context.line, context.function);
#else
    Q_UNUSED(context);
    fprintf(stream, "%s: %s\n", level.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
#endif
}

/** @brief For convenient parsing log files, messages have to be formatted as:
 *      level: message (`placeInSource`)
 *  where:
 *      level - Debug, Warning, Critical, Fatal
 *      message - log message
 *      placeInSource - point, where message was emited in format: (`filename:line, function_signature`)
 */
void logging(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!logLevels.testBit(type)) {
        return;
    }

    QString level;
    switch (type) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    case QtInfoMsg:
        level = "Info";
        break;
#endif
    case QtDebugMsg:
        level = "Debug";
        break;
    case QtWarningMsg:
        level = "Warning";
        break;
    case QtCriticalMsg:
        level = "Critical";
        break;
    case QtFatalMsg:
        level = "Fatal";
        break;
    }

    writeToLog(logConsoleStream, level, context, msg);
    if (logFileStream) {
        writeToLog(logFileStream, level, context, msg);
        fflush(logFileStream);
    }

    if (type == QtFatalMsg) {
        abort();
    }
}

void showHelp()
{
    qDebug("This is an application for creating stop-motion animation movies.\n"
           "Usage:\n"
           "\tqstopmotion [project path] [-s|--silent] [-v|--verbose] [-h|--help]\n"
           "Options:\n"
           "\tproject path - path to project's file (extension .qsmd or .qsma)\n"
           "\t-h|--help - show helps\n"
           "Logging options, which define logging facilities:\n"
           "\t-s|--silent - write to log only important messages\n"
           "\t-v|--verbose - verbose output\n"
           "For more details, visit: www.qstopmotion.org");
}

int main(int argc, char **argv)
{
    int ret = 0;
    bool hasRecovered = false;
    bool hasProjectArg = false;

    QtFrontend qtFrontend(argc, argv);
    const QStringList appArguments = QApplication::arguments();

    // Display the application usage info
    if (appArguments.contains("-h") || appArguments.contains("--help")) {
        showHelp();
        return EXIT_FAILURE;
    }

    //  Configure and redirect log output to text file
    QString g_logFilePath("");
    g_logFilePath = QString("%1/log_qstopmotion.txt").arg(QDir::currentPath());
#ifdef Q_OS_WIN
    logFileStream = _wfopen(g_logFilePath.toStdWString().c_str(), L"w");
#else
    // for Linux - log file will be put in a User's home directory
    logFileStream = fopen(g_logFilePath.toUtf8().data(), "w");
#endif
    logConsoleStream = stderr;
    qInstallMessageHandler(logging);
    qDebug() << "Start application. Write log to" << g_logFilePath;

    // Parse qstopmotion's arguments
    if (appArguments.contains("-s") || appArguments.contains("--silent")) {
        qDebug() << "Write to log only important messages";
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        logLevels.setBit(QtInfoMsg, false);
#endif
        logLevels.setBit(QtDebugMsg, false);
    } else {
        qDebug() << "write to log all messages";
    }
    if (appArguments.contains("-v") || appArguments.contains("--verbose")) {
        qDebug() << "Enable verbose output";
        g_verboseOutput = true;
    } else {
        qDebug() << "Verbose output is disabled";
    }

    // Init application
    ret = qtFrontend.checkApplicationDirectory(argv[0]);
    if (ret) {
        return ret;
    }
    qtFrontend.init();
    qtFrontend.processEvents();

    if (qtFrontend.isRecoveryMode()) {
        hasRecovered = qtFrontend.recoverProject();
        if (hasRecovered == false) {
            qtFrontend.removeApplicationFiles();
        }
    }
    else {
        qtFrontend.makeApplicationDirectories();
    }

    if (hasRecovered == false) {
        hasProjectArg = qtFrontend.openProjectFromArguments(appArguments);
        if (hasProjectArg == false) {
            qtFrontend.startDialog();
        }
    }

    qtFrontend.setupDirectoryMonitoring();

    ret = qtFrontend.run(argc, argv);

    qtFrontend.finalize();

    return ret;
}
