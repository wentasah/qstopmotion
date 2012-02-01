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

#include "frontends/qtfrontend/qtfrontend.h"


int main(int argc, char **argv)
{
    // TODO: What ist this???
    // atexit(cleanup);
    int ret = 0;
    bool hasRecovered = false;
    bool hasProjectArg = false;


    QtFrontend qtFrontend(argc, argv);
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
        hasProjectArg = qtFrontend.handleArguments(argc, argv);
        if (hasProjectArg == false) {
            qtFrontend.startDialog();
        }
    }

    qtFrontend.removeCaptureFiles();

    qtFrontend.setupDirectoryMonitoring();

    // TODO: Repair Directory Monitoring
    // qtFrontend.setupDirectoryMonitoring();
    ret = qtFrontend.run(argc, argv);

    qtFrontend.finalize();

    return ret;
}
