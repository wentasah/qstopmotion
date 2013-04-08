###############################################################################
# QtCreator file for the qStopMotion project                                  #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2013-04-07                                                     #
#                                                                             #
# Please use CMake and the CMakeLists.txt file to build qStopMotion!          #
###############################################################################

HEADERS += src/config.h \
           src/domain/domainfacade.h \
           src/domain/animation/animationproject.h \
           src/domain/animation/exposure.h \
           src/domain/animation/projectserializer.h \
           src/domain/animation/scene.h \
           src/domain/animation/take.h \
           src/domain/undo/undobase.h \
           src/domain/undo/undoexposureadd.h \
           src/domain/undo/undoexposureinsert.h \
           src/domain/undo/undoexposuremove.h \
           src/domain/undo/undoexposureremove.h \
           src/domain/undo/undoexposureselect.h \
           src/domain/undo/undoprojectnew.h \
           src/domain/undo/undoprojectopen.h \
           src/domain/undo/undosceneadd.h \
           src/domain/undo/undosceneinsert.h \
           src/domain/undo/undoscenemove.h \
           src/domain/undo/undosceneremove.h \
           src/domain/undo/undosceneselect.h \
           src/domain/undo/undotakeadd.h \
           src/domain/undo/undotakeinsert.h \
           src/domain/undo/undotakemove.h \
           src/domain/undo/undotakeremove.h \
           src/domain/undo/undotakeselect.h \
           src/frontends/observer.h \
           src/frontends/frontend.h \
           src/frontends/viewfacade.h \
           src/frontends/qtfrontend/mainwindowgui.h \
           src/frontends/qtfrontend/menuframe.h \
           src/frontends/qtfrontend/qtfrontend.h \
           src/frontends/qtfrontend/toolbar.h \
           src/frontends/qtfrontend/dialogs/aboutdialog.h \
           src/frontends/qtfrontend/dialogs/cameracontrollerdialog.h \
           src/frontends/qtfrontend/dialogs/descriptiondialog.h \
           src/frontends/qtfrontend/dialogs/externalcommanddialog.h \
           src/frontends/qtfrontend/dialogs/helpbrowser.h \
           src/frontends/qtfrontend/dialogs/licence.h \
           src/frontends/qtfrontend/dialogs/startdialog.h \
           src/frontends/qtfrontend/elements/clickablelabel.h \
           src/frontends/qtfrontend/elements/flexiblelineedit.h \
           src/frontends/qtfrontend/elements/flexiblespinbox.h \
           src/frontends/qtfrontend/frameview/frameviewimage.h \
           src/frontends/qtfrontend/frameview/frameviewinterface.h \
           src/frontends/qtfrontend/preferences/exportwidget.h \
           src/frontends/qtfrontend/preferences/generaldialog.h \
           src/frontends/qtfrontend/preferences/generalwidget.h \
           src/frontends/qtfrontend/preferences/importwidget.h \
           src/frontends/qtfrontend/preferences/projectdialog.h \
           src/frontends/qtfrontend/preferences/projectwidget.h \
           src/frontends/qtfrontend/timeline/exposurethumbview.h \
           src/frontends/qtfrontend/timeline/thumbview.h \
           src/frontends/qtfrontend/timeline/timeline.h \
           src/frontends/qtfrontend/tooltabs/compositingtab.h \
           src/frontends/qtfrontend/tooltabs/projecttab.h \
           src/frontends/qtfrontend/tooltabs/recordingtab.h \
           src/frontends/qtfrontend/tooltabs/viewtab.h \
           src/technical/externalchangemonitor.h \
           src/technical/preferenceselement.h \
           src/technical/preferencestool.h \
           src/technical/util.h \
           src/technical/audio/audiodriver.h \
           src/technical/audio/audioformat.h \
           src/technical/grabber/gphotocontroller.h \
           src/technical/grabber/gphotograbber.h \
           src/technical/grabber/grabbercontrolcapabilities.h \
           src/technical/grabber/grabbercontroller.h \
           src/technical/grabber/grabberdirectshowcontroller.h \
           src/technical/grabber/grabberv4l2controller.h \
           src/technical/grabber/gstreamerdevicegrabber.h \
           src/technical/grabber/gstreamerdirectshow1394grabber.h \
           src/technical/grabber/gstreamerdirectshowusbgrabber.h \
           src/technical/grabber/gstreamerdv1394grabber.h \
           src/technical/grabber/gstreamergrabber.h \
           src/technical/grabber/gstreamerv4l2grabber.h \
           src/technical/grabber/gstreamervideotestgrabber.h \
           src/technical/grabber/imagegrabber.h \
           src/technical/grabber/imagegrabberdevice.h \
           src/technical/grabber/imagegrabberfacade.h \
           src/technical/grabber/imagegrabberthread.h \
           src/technical/grabber/windowsuvccontroller.h \
           src/technical/videoencoder/ffmpegencoder.h \
           src/technical/videoencoder/videoencoder.h \
           src/technical/videoencoder/videoencoderfactory.h

SOURCES += src/main.cpp \
           src/domain/domainfacade.cpp \
           src/domain/animation/animationproject.cpp \
           src/domain/animation/exposure.cpp \
           src/domain/animation/projectserializer.cpp \
           src/domain/animation/scene.cpp \
           src/domain/animation/take.cpp \
           src/domain/undo/undobase.cpp \
           src/domain/undo/undoexposureadd.cpp \
           src/domain/undo/undoexposureinsert.cpp \
           src/domain/undo/undoexposuremove.cpp \
           src/domain/undo/undoexposureremove.cpp \
           src/domain/undo/undoexposureselect.cpp \
           src/domain/undo/undoprojectnew.cpp \
           src/domain/undo/undoprojectopen.cpp \
           src/domain/undo/undosceneadd.cpp \
           src/domain/undo/undosceneinsert.cpp \
           src/domain/undo/undoscenemove.cpp \
           src/domain/undo/undosceneremove.cpp \
           src/domain/undo/undosceneselect.cpp \
           src/domain/undo/undotakeadd.cpp \
           src/domain/undo/undotakeinsert.cpp \
           src/domain/undo/undotakemove.cpp \
           src/domain/undo/undotakeremove.cpp \
           src/domain/undo/undotakeselect.cpp \
           src/frontends/viewfacade.cpp \
           src/frontends/qtfrontend/mainwindowgui.cpp \
           src/frontends/qtfrontend/menuframe.cpp \
           src/frontends/qtfrontend/qtfrontend.cpp \
           src/frontends/qtfrontend/toolbar.cpp \
           src/frontends/qtfrontend/dialogs/aboutdialog.cpp \
           src/frontends/qtfrontend/dialogs/cameracontrollerdialog.cpp \
           src/frontends/qtfrontend/dialogs/descriptiondialog.cpp \
           src/frontends/qtfrontend/dialogs/externalcommanddialog.cpp \
           src/frontends/qtfrontend/dialogs/helpbrowser.cpp \
           src/frontends/qtfrontend/dialogs/startdialog.cpp \
           src/frontends/qtfrontend/elements/flexiblelineedit.cpp \
           src/frontends/qtfrontend/elements/flexiblespinbox.cpp \
           src/frontends/qtfrontend/frameview/frameviewimage.cpp \
           src/frontends/qtfrontend/frameview/frameviewinterface.cpp \
           src/frontends/qtfrontend/preferences/exportwidget.cpp \
           src/frontends/qtfrontend/preferences/generaldialog.cpp \
           src/frontends/qtfrontend/preferences/generalwidget.cpp \
           src/frontends/qtfrontend/preferences/importwidget.cpp \
           src/frontends/qtfrontend/preferences/projectdialog.cpp \
           src/frontends/qtfrontend/preferences/projectwidget.cpp \
           src/frontends/qtfrontend/timeline/exposurethumbview.cpp \
           src/frontends/qtfrontend/timeline/thumbview.cpp \
           src/frontends/qtfrontend/timeline/timeline.cpp \
           src/frontends/qtfrontend/tooltabs/compositingtab.cpp \
           src/frontends/qtfrontend/tooltabs/projecttab.cpp \
           src/frontends/qtfrontend/tooltabs/recordingtab.cpp \
           src/frontends/qtfrontend/tooltabs/viewtab.cpp \
           src/technical/externalchangemonitor.cpp \
           src/technical/preferenceselement.cpp \
           src/technical/preferencestool.cpp \
           src/technical/util.cpp \
           src/technical/grabber/gphotocontroller.cpp \
           src/technical/grabber/gphotograbber.cpp \
           src/technical/grabber/grabbercontrolcapabilities.cpp \
           src/technical/grabber/grabbercontroller.cpp \
           src/technical/grabber/grabberdirectshowcontroller.cpp \
           src/technical/grabber/grabberv4l2controller.cpp \
           src/technical/grabber/gstreamerdevicegrabber.cpp \
           src/technical/grabber/gstreamerdirectshow1394grabber.cpp \
           src/technical/grabber/gstreamerdirectshowusbgrabber.cpp \
           src/technical/grabber/gstreamerdv1394grabber.cpp \
           src/technical/grabber/gstreamergrabber.cpp \
           src/technical/grabber/gstreamerv4l2grabber.cpp \
           src/technical/grabber/gstreamervideotestgrabber.cpp \
           src/technical/grabber/imagegrabber.cpp \
           src/technical/grabber/imagegrabberdevice.cpp \
           src/technical/grabber/imagegrabberfacade.cpp \
           src/technical/grabber/imagegrabberthread.cpp \
           src/technical/grabber/windowsuvccontroller.cpp \
           src/technical/videoencoder/ffmpegencoder.cpp \
           src/technical/videoencoder/videoencoder.cpp \
           src/technical/videoencoder/videoencoderfactory.cpp

TRANSLATIONS += translations/qstopmotion_fr.ts \
#                translations/qt_fr.ts \
#                translations/qstopmotion_br.ts \
#                translations/qstopmotion_cz.ts \
                translations/qstopmotion_da.ts \
#                translations/qt_da.ts \
#                translations/qstopmotion_es.ts \
#                translations/qstopmotion_fi.ts \
#                translations/qstopmotion_gr.ts \
#                translations/qstopmotion_it.ts \
#                translations/qstopmotion_kl.ts \
#                translations/qstopmotion_no_nb.ts \
#                translations/qstopmotion_no_nn.ts \
#                translations/qstopmotion_no_se.ts \
#                translations/qstopmotion_pt.ts \
#                translations/qstopmotion_ru.ts \
#                translations/qstopmotion_se.ts \
#                translations/qstopmotion_sl.ts \
#                translations/qstopmotion_tr.ts \
#                translations/qt_de.ts \
                translations/qstopmotion_de.ts

DEPENDPATH += src

INCLUDEPATH += src
unix {
INCLUDEPATH += $$system(pkg-config --cflags gstreamer-0.10 | sed -e 's/-I//g')
}
win32 {
INCLUDEPATH += $$quote($(GST_INCLUDE)) \
               $$quote($(GST_GDL_INCLUDE)) \
               $$quote($(GST_GST_INCLUDE)) \
               $$quote($(GST_XML_INCLUDE))
}

unix {
LIBS += $$system(pkg-config --libs gstreamer-0.10) \
        -lX11
}
win32 {
LIBS += $$quote($(GST_GDL_LIB)) \
        $$quote($(GST_GOBJ_LIB)) \
        $$quote($(GST_GST_LIB)) \
        $$quote($(GST_XML_LIB))
}

DISTFILES += src/config.cpp.in \
             graphics/qstopmotion.png \
             qstopmotion.pro.in \
             qstopmotion.desktop \
             qstopmotion.mime \
             qstopmotion.1 \
             configure \
             AUTHORS \
             README \
             COPYING
unix {
DISTFILES += $$system(ls -x manual/*.html) \
             $$system(ls -x manual/graphic/*.png) \
             $$system(ls -x manual/icons/*.png) \
             $$system(ls -x manual/screenshots/*.png) \
             $$system(ls -x translations/*.qm)
}

DISTFILES -= qstopmotion.pro \
             src/config.cpp
             
CONFIG += debug
# CONFIG += release warn_off
# DEFINES += NO_DEBUG
TEMPLATE = app
QT += xml

target.path = /usr/local/bin

translations.path = /usr/local/share/qstopmotion/translations
translations.files = translations/*.qm

htmldoc.path = /usr/local/share/doc/qstopmotion/html
htmldoc.files = manual/*.html
htmldoc.extra = $(INSTALL_DIR) manual/graphic $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html;
htmldoc.extra += $(INSTALL_DIR) manual/icons $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html;
htmldoc.extra += $(INSTALL_DIR) manual/screenshots $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html;

# Dummy target to fix permissions. 
dummy.path = /usr/local/bin
dummy.extra += chmod 644 $(INSTALL_ROOT)/usr/local/share/qstopmotion/translations/*.qm \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/*.html \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/graphic/* \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/icons/* \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/screenshots/*;
dummy.extra += chmod 755 $(INSTALL_ROOT)/usr/local/share/qstopmotion/translations \
    $(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET) \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/graphic \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/icons \
    $(INSTALL_ROOT)/usr/local/share/doc/qstopmotion/html/screenshots;

INSTALLS += target translations htmldoc dummy

