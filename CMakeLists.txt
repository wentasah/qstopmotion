###############################################################################
# CMake file for the qStopMotion project                                      #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2012-04-17                                                     #
#                                                                             #
# Usage:                                                                      #
#   * Qt-Creator (Windows, Linux)                                             #
#      + open this file                                                       #
#      + select a destination directory                                       #
#      + press the CMake button                                               #
#   * makefile: (Windows)                                                     #
#      + cmake -G "NMake Makefile"                                            #
#      + nmake                                                                #
#      + nmake install (with administration rights)                           #
#      + nmake package                                                        #
#   * makefile: (Linux)                                                       #
#      + cmake .                                                              #
#      + make                                                                 #
#      + sudo make install                                                    #
#   * MS Visual Studio (Windows)                                              #
#      + cmake -G "Visual Studio 10 2010"                                     #
#      + open the solution file                                               #
#                                                                             #
###############################################################################

project(qStopMotion)

cmake_minimum_required(VERSION 2.6.2)

###############################################################################
# General project settings
###############################################################################

SET(CMAKE_PROJECT_NAME "qstopmotion")
SET(QSTOPMOTION_HOMEPAGE "http://www.qstopmotion.org")
SET(QSTOPMOTION_VERSION_MAJOR "1")
SET(QSTOPMOTION_VERSION_MINOR "0")
SET(QSTOPMOTION_VERSION_PATCH "1")

SET(FILES_TO_TRANSLATE )

###############################################################################
# Qt4 support
###############################################################################

# find and setup Qt4 for this poroject
find_package(Qt4 REQUIRED)

message("Qt Version: ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

# Enable/disable Qt functionality
set(QT_USE_QTXML TRUE)

# include a cmake listfile
include(${QT_USE_FILE})

IF(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("No Qt debug and warning output!")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    add_definitions(-DQT_NO_WARNING_OUTPUT)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

###############################################################################
# File definitions
###############################################################################

# add all header files of the project to HEADERS
set(HEADERS
    src/frontends/observer.h
    src/frontends/frontend.h
    src/frontends/viewfacade.h
    src/frontends/qtfrontend/dialogs/licence.h
    src/frontends/qtfrontend/timeline/exposurethumbview.h
    src/frontends/qtfrontend/timeline/thumbview.h
    src/technical/util.h
    src/technical/audio/audiodriver.h
    src/technical/audio/audioformat.h
    src/technical/grabber/imagegrabberdevice.h
    src/technical/grabber/imagegrabberthread.h
    src/technical/videoencoder/ffmpegencoder.h
    src/technical/videoencoder/videoencoder.h
    src/technical/videoencoder/videoencoderfactory.h
)

# add all header files of the project to MOC to MOC_HEADERS
set(MOC_HEADERS
    src/domain/domainfacade.h
    src/domain/animation/animationproject.h
    src/domain/animation/exposure.h
    src/domain/animation/projectserializer.h
    src/domain/animation/scene.h
    src/domain/animation/take.h
    src/domain/undo/undobase.h
    src/domain/undo/undoexposureadd.h
    src/domain/undo/undoexposureinsert.h
    src/domain/undo/undoexposuremove.h
    src/domain/undo/undoexposureremove.h
    src/domain/undo/undoexposureselect.h
    src/domain/undo/undoprojectclose.h
    src/domain/undo/undoprojectnew.h
    src/domain/undo/undoprojectopen.h
    src/domain/undo/undoprojectsave.h
    src/domain/undo/undosceneadd.h
    src/domain/undo/undosceneinsert.h
    src/domain/undo/undoscenemove.h
    src/domain/undo/undosceneremove.h
    src/domain/undo/undosceneselect.h
    src/domain/undo/undotakeadd.h
    src/domain/undo/undotakeinsert.h
    src/domain/undo/undotakemove.h
    src/domain/undo/undotakeremove.h
    src/domain/undo/undotakeselect.h
    src/frontends/qtfrontend/mainwindowgui.h
    src/frontends/qtfrontend/menuframe.h
    src/frontends/qtfrontend/qtfrontend.h
    src/frontends/qtfrontend/toolbar.h
    src/frontends/qtfrontend/dialogs/aboutdialog.h
    src/frontends/qtfrontend/dialogs/descriptiondialog.h
    src/frontends/qtfrontend/dialogs/externalcommanddialog.h
    src/frontends/qtfrontend/dialogs/helpbrowser.h
    src/frontends/qtfrontend/dialogs/startdialog.h
    src/frontends/qtfrontend/elements/clickablelabel.h
    src/frontends/qtfrontend/elements/flexiblelineedit.h
    src/frontends/qtfrontend/elements/flexiblespinbox.h
    src/frontends/qtfrontend/frameview/frameviewimage.h
    src/frontends/qtfrontend/frameview/frameviewinterface.h
    src/frontends/qtfrontend/preferences/defaulttab.h
    src/frontends/qtfrontend/preferences/exporttab.h
    src/frontends/qtfrontend/preferences/generaltab.h
    src/frontends/qtfrontend/preferences/preferencesdialog.h
    src/frontends/qtfrontend/timeline/timeline.h
    src/frontends/qtfrontend/tooltabs/compositingtab.h
    src/frontends/qtfrontend/tooltabs/projecttab.h
    src/frontends/qtfrontend/tooltabs/recordingtab.h
    src/frontends/qtfrontend/tooltabs/viewtab.h
    src/technical/externalchangemonitor.h
    src/technical/preferencestool.h
    src/technical/grabber/gstreamergrabber.h
    src/technical/grabber/imagegrabber.h
)

# add all source files to SOURCES
set(SOURCES
    src/main.cpp
    src/domain/domainfacade.cpp
    src/domain/animation/animationproject.cpp
    src/domain/animation/exposure.cpp
    src/domain/animation/projectserializer.cpp
    src/domain/animation/scene.cpp
    src/domain/animation/take.cpp
    src/domain/undo/undobase.cpp
    src/domain/undo/undoexposureadd.cpp
    src/domain/undo/undoexposureinsert.cpp
    src/domain/undo/undoexposuremove.cpp
    src/domain/undo/undoexposureremove.cpp
    src/domain/undo/undoexposureselect.cpp
    src/domain/undo/undoprojectclose.cpp
    src/domain/undo/undoprojectnew.cpp
    src/domain/undo/undoprojectopen.cpp
    src/domain/undo/undoprojectsave.cpp
    src/domain/undo/undosceneadd.cpp
    src/domain/undo/undosceneinsert.cpp
    src/domain/undo/undoscenemove.cpp
    src/domain/undo/undosceneremove.cpp
    src/domain/undo/undosceneselect.cpp
    src/domain/undo/undotakeadd.cpp
    src/domain/undo/undotakeinsert.cpp
    src/domain/undo/undotakemove.cpp
    src/domain/undo/undotakeremove.cpp
    src/domain/undo/undotakeselect.cpp
    src/frontends/viewfacade.cpp
    src/frontends/qtfrontend/mainwindowgui.cpp
    src/frontends/qtfrontend/menuframe.cpp
    src/frontends/qtfrontend/qtfrontend.cpp
    src/frontends/qtfrontend/toolbar.cpp
    src/frontends/qtfrontend/dialogs/aboutdialog.cpp
    src/frontends/qtfrontend/dialogs/descriptiondialog.cpp
    src/frontends/qtfrontend/dialogs/externalcommanddialog.cpp
    src/frontends/qtfrontend/dialogs/helpbrowser.cpp
    src/frontends/qtfrontend/dialogs/startdialog.cpp
    src/frontends/qtfrontend/elements/flexiblelineedit.cpp
    src/frontends/qtfrontend/elements/flexiblespinbox.cpp
    src/frontends/qtfrontend/frameview/frameviewimage.cpp
    src/frontends/qtfrontend/frameview/frameviewinterface.cpp
    src/frontends/qtfrontend/preferences/defaulttab.cpp
    src/frontends/qtfrontend/preferences/exporttab.cpp
    src/frontends/qtfrontend/preferences/generaltab.cpp
    src/frontends/qtfrontend/preferences/preferencesdialog.cpp
    src/frontends/qtfrontend/timeline/timeline.cpp
    src/frontends/qtfrontend/timeline/exposurethumbview.cpp
    src/frontends/qtfrontend/timeline/thumbview.cpp
    src/frontends/qtfrontend/tooltabs/compositingtab.cpp
    src/frontends/qtfrontend/tooltabs/projecttab.cpp
    src/frontends/qtfrontend/tooltabs/recordingtab.cpp
    src/frontends/qtfrontend/tooltabs/viewtab.cpp
    src/technical/externalchangemonitor.cpp
    src/technical/preferencestool.cpp
    src/technical/util.cpp
    src/technical/grabber/gstreamergrabber.cpp
    src/technical/grabber/imagegrabber.cpp
    src/technical/grabber/imagegrabberdevice.cpp
    src/technical/grabber/imagegrabberthread.cpp
    src/technical/videoencoder/ffmpegencoder.cpp
    src/technical/videoencoder/videoencoder.cpp
    src/technical/videoencoder/videoencoderfactory.cpp
)

set(TRANSLATIONS
    translations/qstopmotion_de.ts
    translations/qstopmotion_fr.ts
#    translations/qstopmotion_br.ts
#    translations/qstopmotion_cz.ts
#    translations/qstopmotion_dk.ts
#    translations/qstopmotion_es.ts
#    translations/qstopmotion_fi.ts
#    translations/qstopmotion_gr.ts
#    translations/qstopmotion_it.ts
#    translations/qstopmotion_kl.ts
#    translations/qstopmotion_no_nb.ts
#    translations/qstopmotion_no_nn.ts
#    translations/qstopmotion_no_se.ts
#    translations/qstopmotion_pt.ts
#    translations/qstopmotion_ru.ts
#    translations/qstopmotion_se.ts
#    translations/qstopmotion_sl.ts
#    translations/qstopmotion_tr.ts
)

set(PIXMAP_FILES
    qstopmotion.png       # RPM Icon is installed in a separate step
    qstopmotion.xpm       # DEB Icon is installed in a separate step
)

set(GENERAL_FILES
    AUTHORS               # The authors
    README.txt            # The readme for the users
    COPYING               # GPL2 License
    Changes.txt           # Change log
)

set(UNIX_FILES
    qstopmotion.desktop   # .desktop is installed in a separate step Unix systems
    qstopmotion.mime      # ???
#     qstopmotion.1         # Linux man page
)

set(DEBIAN_FILES
    debian/changelog             # changelog file
    debian/copyright             # copyright file
    debian/qstopmotion.menu      # .menu is installed in a separate step for Debian systems
)

###############################################################################
# GStreamer support
#
# used libraries: glib-2.0.lib, gobject-2.0.lib, gstreamer-0.10.lib, xml2.lib
###############################################################################

IF(WIN32)

    include(${CMAKE_SOURCE_DIR}/cmake/FindGStreamerOSSBUILD.cmake)
    message("GSTREAMER_INCLUDE_DIR: ${GSTREAMER_INCLUDE_DIR}")
    message("GSTREAMER_LIBRARIES: ${GSTREAMER_LIBRARIES}")
    message("GSTREAMER_FOUND: ${GSTREAMER_FOUND}")

ELSE(WIN32)

    include(${CMAKE_SOURCE_DIR}/cmake/FindGStreamerPkgConfig.cmake)
    message("GSTREAMER_INCLUDE_DIR: ${GSTREAMER_INCLUDE_DIR}")
    message("GSTREAMER_LIBRARY: ${GSTREAMER_LIBRARY}")
    message("GSTREAMER_BASE_LIBRARY: ${GSTREAMER_BASE_LIBRARY}")
    message("GSTREAMER_LIBRARIES: ${GSTREAMER_LIBRARIES}")
    message("GSTREAMER_STATIC_LIBRARIES: ${GSTREAMER_STATIC_LIBRARIES}")
    message("GSTREAMER_VERSION: ${GSTREAMER_VERSION}")
    message("GSTREAMER_DEFINITIONS: ${GSTREAMER_DEFINITIONS}")

ENDIF(WIN32)

###############################################################################
# Directory definitions
###############################################################################

# tell cmake where to search for headers:
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${QT_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${GSTREAMER_INCLUDE_DIR}
)

###############################################################################
# Additional definitions
###############################################################################

###############################################################################
#
# CONFIG += debug
# CONFIG += release warn_off
# DEFINES += NO_DEBUG
# TEMPLATE = app
#
###############################################################################

###############################################################################
# Create binaries
###############################################################################

# Set the output directory for the executable - old variable (CMake <= 2.8.0)
SET(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all executables"
)

# Set the output directory for the executable - new variable (CMake > 2.8.0)
SET(RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all executables"
)

# Set the output directory for the static libraries
# SET(LIBRARY_OUTPUT_DIRECTORY
#     ${PROJECT_BINARY_DIR}/bin
#     CACHE PATH
#     "Single directory for all executables"
# )

# Set the output directory for the shared libraries
# SET(ARCHIVE_OUTPUT_DIRECTORY
#     ${PROJECT_BINARY_DIR}/bin
#     CACHE PATH
#     "Single directory for all executables"
# )

MARK_AS_ADVANCED(
    EXECUTABLE_OUTPUT_PATH
    RUNTIME_OUTPUT_DIRECTORY
#     LIBRARY_OUTPUT_DIRECTORY
#     ARCHIVE_OUTPUT_DIRECTORY
)
# Create .moc files
qt4_wrap_cpp(
    MOC_SOURCES
    ${MOC_HEADERS}
)

# Create a executable
IF(UNIX)

    add_executable(
        qstopmotion
        ${SOURCES}
        ${MOC_SOURCES}
    )

ELSEIF(APPLE)

    add_executable(
        qstopmotion
        MACOSX_BUNDLE
        ${SOURCES}
        ${MOC_SOURCES}
    )

ELSEIF(WIN32)

    add_executable(
        qstopmotion
        WIN32
        src/qstopmotion.rc
        ${SOURCES}
        ${MOC_SOURCES}
    )

ENDIF()
 
# tell cmake which libraries to link
target_link_libraries(
    qstopmotion
    ${QT_LIBRARIES}
    ${QT_QTMAIN_LIBRARY}
    ${QT_QTXML_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
)

add_definitions(${QT_DEFINITIONS})

IF(WIN32)

ELSE(WIN32)

    add_definitions(${GSTREAMER_DEFINITIONS})

ENDIF(WIN32)

IF(WIN32)

    file(COPY graphics DESTINATION ".")
    file(COPY manual DESTINATION ".")
    file(COPY translations DESTINATION ".")
    file(COPY ${ICON_FILES} DESTINATION ".")
    file(COPY ${GENERAL_FILES} DESTINATION ".")

    file(COPY ffmpeg DESTINATION ".")

    file(COPY qtruntime DESTINATION ".")

ELSE(WIN32)

    file(COPY graphics DESTINATION "share/qstopmotion/")
    file(COPY manual/ DESTINATION "share/doc/qstopmotion/")
    file(COPY translations DESTINATION "share/qstopmotion/")
    file(COPY icons DESTINATION "share/qstopmotion/")
    file(COPY ${ICON_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${GENERAL_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${UNIX_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${DEBIAN_FILES} DESTINATION "share/qstopmotion/debian/")
    file(COPY ${FEDORA_FILES} DESTINATION "share/qstopmotion/rpm/")

ENDIF(WIN32)

###############################################################################
# Translation
###############################################################################

set (FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${HEADERS} ${MOC_HEADERS} ${SOURCES})

file (GLOB TRANSLATIONS_FILES ${TRANSLATIONS})

option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)")
if (UPDATE_TRANSLATIONS)
  qt4_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TRANSLATIONS_FILES})
else (UPDATE_TRANSLATIONS)
  qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
endif (UPDATE_TRANSLATIONS) 

add_custom_target (translations_target DEPENDS ${QM_FILES})

###############################################################################
# Installation
###############################################################################

IF(WIN32)
    # The predefined windows installer path include the qstopmotion subdirectory
    
    set(QSTOPMOTION_BIN_INSTALL_DIR "bin")
    set(QSTOPMOTION_GRAPHICS_INSTALL_DIR "graphics")
    set(QSTOPMOTION_GRAPHICS_ICONS_INSTALL_DIR "graphics/icons")
    set(QSTOPMOTION_GRAPHICS_PICTURES_INSTALL_DIR "graphics/pictures")
    set(QSTOPMOTION_MANUAL_INSTALL_DIR "manual")
    set(QSTOPMOTION_TRANSLATIONS_INSTALL_DIR "translations")
    set(QSTOPMOTION_GENERALS_INSTALL_DIR ".")
    set(QSTOPMOTION_PIXMAP_INSTALL_DIR "pixmaps")
    set(QSTOPMOTION_ICONS_INSTALL_DIR "icons")

ELSE(WIN32)
    # The predefined CMake-Linux installer path is "/usr/local"
    # The predefined RPM installer path is "/usr"
    # The predefined DEB installer path is "/usr"

    set(QSTOPMOTION_BIN_INSTALL_DIR "bin")
    set(QSTOPMOTION_GRAPHICS_INSTALL_DIR "share/qstopmotion/graphics")
    set(QSTOPMOTION_GRAPHICS_ICONS_INSTALL_DIR "share/qstopmotion/graphics/icons")
    set(QSTOPMOTION_GRAPHICS_PICTURES_INSTALL_DIR "share/qstopmotion/graphics/pictures")
    set(QSTOPMOTION_TRANSLATIONS_INSTALL_DIR "share/qstopmotion/translations")
    set(QSTOPMOTION_GENERALS_INSTALL_DIR "share/qstopmotion")
    set(QSTOPMOTION_MANUAL_INSTALL_DIR "share/doc/qstopmotion")
    set(QSTOPMOTION_DESKTOP_INSTALL_DIR "share/applications")
    set(QSTOPMOTION_MENUFILE_INSTALL_DIR "share/menu")
    set(QSTOPMOTION_PIXMAP_INSTALL_DIR "share/pixmaps")
    set(QSTOPMOTION_ICONS_INSTALL_DIR "share/icons")

ENDIF(WIN32)

# installation of the executables
install(
    TARGETS qstopmotion
    RUNTIME DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}
)

# installation of the general-purpose files
install(
    FILES ${GENERAL_FILES}
    DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
)

# installation of the content of the grapics directory
install(
    DIRECTORY graphics/
    DESTINATION ${QSTOPMOTION_GRAPHICS_INSTALL_DIR}
)

# installation of the content of the manual directory
install(
    DIRECTORY manual/
    DESTINATION ${QSTOPMOTION_MANUAL_INSTALL_DIR}
)

# installation of the content of the translations directory
install(
    DIRECTORY translations/
    DESTINATION ${QSTOPMOTION_TRANSLATIONS_INSTALL_DIR}
)

# installation only on UNIX systems
IF(UNIX)
    # installation of the general unix files
    install(
        FILES ${UNIX_FILES}
        DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
    )

    # installation of the icons
    install(
        DIRECTORY icons/
        DESTINATION ${QSTOPMOTION_ICONS_INSTALL_DIR}
    )

    # installation of the icon file
    install(
        FILES "qstopmotion.png"
        DESTINATION ${QSTOPMOTION_PIXMAP_INSTALL_DIR}
    )

    # installation of the .desktop file
    install(
        FILES "qstopmotion.desktop"
        DESTINATION ${QSTOPMOTION_DESKTOP_INSTALL_DIR}
    )
ENDIF(UNIX)

# installation on DEBIAN systems
IF(CPACK_GENERATOR STREQUAL "DEB")
    message("Special task for Debian package!")
    # installation of the icon file
    install(
        FILES "debian/qstopmotion.xpm"
        DESTINATION ${QSTOPMOTION_PIXMAP_INSTALL_DIR}
    )

    # installation of the .menufile file
    install(
        FILES "debian/qstopmotion.menu"
        DESTINATION ${QSTOPMOTION_MENUFILE_INSTALL_DIR}
    )
ENDIF(CPACK_GENERATOR STREQUAL "DEB")

# installation on Windows systems
IF(WIN32)
    # installation of ffmpeg
    install(
        DIRECTORY ffmpeg
        DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
    )
    # installation of Qt runtime
    install(
        DIRECTORY qtruntime/
        DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}
    )
ENDIF(WIN32)

###############################################################################
###############################################################################
#
# Packaging
#
###############################################################################
###############################################################################

###############################################################################
# General packaging settings
###############################################################################

SET(CPACK_PACKAGE_NAME "qstopmotion")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "qStopMotion is a program to create stop motion animations.")
SET(CPACK_PACKAGE_VENDOR "Ralf Lange, qstopmotion.org")
SET(CPACK_PACKAGE_CONTACT "Ralf.Lange@qstopmotion.org")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\graphics\\\\qstopmotion_logo_installer.bmp")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "qStopMotion ${QSTOPMOTION_VERSION_MAJOR}.${QSTOPMOTION_VERSION_MINOR}.${QSTOPMOTION_VERSION_PATCH}")
SET(CPACK_PACKAGE_EXECUTABLES "qstopmotion" "qStopMotion")
SET(CPACK_STRIP_FILES "qstopmotion")
SET(CPACK_SOURCE_STRIP_FILES "")
SET(CPACK_PACKAGE_VERSION_MAJOR "${QSTOPMOTION_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${QSTOPMOTION_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${QSTOPMOTION_VERSION_PATCH}")
SET(CPACK_TOPLEVEL_TAG "/user")
# CPACK_PACKAGE_VERSION - generated from major, minor, patch

###############################################################################
# Source Packages
###############################################################################

IF(WIN32)
    # Exclude only Mercurial SMC files
    SET(
        CPACK_SOURCE_IGNORE_FILES
        "${CPACK_SOURCE_IGNORE_FILES}"
        "${PROJECT_SOURCE_DIR}/.hg/"
        "${PROJECT_SOURCE_DIR}/.hgtags"
    )

ELSE(WIN32)
    # Exclude Mercurial SMC files, ffmpeg Windows binarys and Qt Windows runtime
    SET(
        CPACK_SOURCE_IGNORE_FILES
        "${CPACK_SOURCE_IGNORE_FILES}"
        "${PROJECT_SOURCE_DIR}/.hg/"
        "${PROJECT_SOURCE_DIR}/ffmpeg/"
        "${PROJECT_SOURCE_DIR}/qtruntime/"
        "${PROJECT_SOURCE_DIR}/.hgtags"
    )

ENDIF(WIN32)

###############################################################################
# NSIS - Windows only
###############################################################################

# Installing Microsoft run time libraries in the same directory as the executables
SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
INCLUDE(InstallRequiredSystemLibraries)
INSTALL(
    PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
    DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}
)

# There is a bug in NSI that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backlasshes.
  
# Set the icons for the installer and the uninstaller
SET(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/graphics/qstopmotion.ico")
SET(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/graphics/qstopmotion.ico")
# Set the project name on the first page
# Use CPACK_PACKAGE_INSTALL_DIRECTORY
# Set the bitmap for the second and following pages
# Use CPACK_PACKAGE_ICON
# Set the licence text for the second page
# Use CPACK_RESOURCE_FILE_LICENSE
# Enable modify path environment variable on third page
SET(CPACK_NSIS_MODIFY_PATH ON)
# Set the executable name for the desktop icon on third page
SET(CPACK_CREATE_DESKTOP_LINKS qstopmotion)
# Create shortcuts in the start menu
# Use CPACK_PACKAGE_EXECUTABLES
SET(CPACK_NSIS_MENU_LINKS
    "manual/index.html" "qStopMotion Help"
    "http://www.qstopmotion.org" "qStopMotion Web Site"
)

# Additional installation command
# Install the GStreamer OSS Build
SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    NSISdl::download http://ossbuild.googlecode.com/files/GStreamer-WinBuilds-GPL-x86.msi $INSTDIR\\\\GStreamer-WinBuilds-GPL-x86.msi
    ExecWait 'msiexec /i \\\"$INSTDIR\\\\GStreamer-WinBuilds-GPL-x86.msi\\\" '
")

# Additional uninstallation command
# Uninstall the GStreamer OSS Build
SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    ExecWait 'msiexec /x \\\"$INSTDIR\\\\GStreamer-WinBuilds-GPL-x86.msi\\\" '
    Delete \\\"$INSTDIR\\\\GStreamer-WinBuilds-GPL-x86.msi\\\"
")

# SET(CPACK_NSIS_INSTALLED_ICON_NAME "qstopmotion.exe")
# SET(CPACK_NSIS_DISPLAY_NAME "qStopMotion")
# SET(CPACK_NSIS_PACKAGE_NAME "qStopMotion")
# SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.qstopmotion.org")
# SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.longsoft.de")
SET(CPACK_NSIS_CONTACT ${CPACK_PACKAGE_VENDOR})

# Run the application on the finish page
# SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "qstopmotion.exe"

###############################################################################
# DEB package - Linux only (Debian, OpenSuse, Ubuntu, ...)
###############################################################################

# Mandatory
# CPACK_DEBIAN_PACKAGE_NAME = CPACK_PACKAGE_NAME
# CPACK_DEBIAN_PACKAGE_VERSION = CPACK_PACKAGE_VERSION
# CPACK_DEBIAN_PACKAGE_ARCHITECTURE = dpkg output       # dpkg --print-architecture ???
# CPACK_DEBIAN_PACKAGE_MAINTAINER = CPACK_PACKAGE_CONTACT
# CPACK_DEBIAN_PACKAGE_DESCRIPTION = CPACK_PACKAGE_DESCRIPTION_SUMMARY
SET(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")


# Options
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>=2.11), ffmpeg (>=0.5.2), gstreamer-tools (>=0.10.28), libqt4-core (>=4.6.2), libqt4-gui (>=4.6.2), libqt4-xml (>=4.6.2)")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${QSTOPMOTION_HOMEPAGE})
# CPACK_DEBIAN_PACKAGE_SHLIBDEPS
# SET(CPACK_DEBIAN_PACKAGE_DEBUG "ON")
# CPACK_DEBIAN_PACKAGE_DEBUG
# CPACK_DEBIAN_PACKAGE_ENHANCES
# CPACK_DEBIAN_PACKAGE_BREAKS
# CPACK_DEBIAN_PACKAGE_CONFLICTS
# CPACK_DEBIAN_PACKAGE_PROVIDES
# CPACK_DEBIAN_PACKAGE_REPLACES

###############################################################################
# RPM package - Linux only (Fedora, Red Hat, CentOS, Mandriva, ...)
###############################################################################

# Mandatory
# CPACK_RPM_PACKAGE_SUMMARY = CPACK_PACKAGE_DESCRIPTION_SUMMARY
# CPACK_RPM_PACKAGE_NAME = CPACK_PACKAGE_NAME
# CPACK_RPM_PACKAGE_VERSION = CPACK_PACKAGE_VERSION
# SET(CPACK_RPM_PACKAGE_RELEASE "1")
SET(CPACK_RPM_PACKAGE_LICENSE "GPLv2")
SET(CPACK_RPM_PACKAGE_GROUP "Applications/Multimedia")
# CPACK_RPM_PACKAGE_VENDOR = CPACK_PACKAGE_VENDOR

# CPACK_DEBIAN_PACKAGE_MAINTAINER = CPACK_PACKAGE_CONTACT

# Options
# SET(CPACK_RPM_PACKAGE_ARCHITECTURE "noarch")
SET(CPACK_RPM_PACKAGE_URL ${QSTOPMOTION_HOMEPAGE})
# CPACK_RPM_PACKAGE_DESCRIPTION = CPACK_PACKAGE_DESCRIPTION_FILE
# CPACK_RPM_COMPRESSION_TYPE
# requireses must be in a aphabetic order
SET(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.14, ffmpeg >= 0.6, gstreamer >= 0.10.28, qt >= 4.6.2")
# CPACK_RPM_PACKAGE_SUGGESTS
# CPACK_RPM_PACKAGE_PROVIDES

# SET(CPACK_RPM_SPEC_INSTALL_POST "desktop-file-validate /usr/share/applications/qstopmotion.desktop")

# SET(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "pre_install.sh")
# SET(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "post_install.sh")
# SET(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE "pre_install.sh")
# SET(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "post_install.sh")


###############################################################################
# Activate packaging functionality
###############################################################################

INCLUDE(CPack)

###############################################################################
# "this is the end"
###############################################################################

