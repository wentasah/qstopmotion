###############################################################################
# CMake file for the qStopMotion project                                      #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2019-11-30                                                     #
#                                                                             #
# Usage:                                                                      #
#   * Qt-Creator (Windows, Linux)                                             #
#      + open this file                                                       #
#      + select a destination directory                                       #
#      + press the CMake button                                               #
#   * makefile: (Windows)                                                     #
#      + open Visual Studio Command Prompt                                    #
#        (for 64bit use: 'VS2017 x64 Native Tools Command Prompt'             #
#        (for 32bit use: 'VS2017 x32 Native Tools Command Prompt'             #
#      + cmake -G "NMake Makefiles" .                                         #
#      + nmake                                                                #
#      + nmake install (with administration rights)                           #
#      + nmake package                                                        #
#   * makefile: (Linux)                                                       #
#      + cmake .                                                              #
#      + make                                                                 #
#      + sudo make install                                                    #
#   * MS Visual Studio (Windows)                                              #
#      + for 32bit: cmake -G "Visual Studio 12 2017"                          #
#      + for 64bit: cmake -G "Visual Studio 12 2017 x64"                      #
#      + open the solution file                                               #
#                                                                             #
###############################################################################

project(qStopMotion)

cmake_minimum_required(VERSION 3.0.2)

# option(CMAKE_VERBOSE_MAKEFILE ON)

# set path to additional CMake modules
SET(
    CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}
)

message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

###############################################################################
# Platform check
###############################################################################

IF(WIN32)
    IF(CMAKE_CL_64)
        message("Target platform: Windows 64bit")
    ELSE(CMAKE_CL_64)
        message("Target platform: Windows 32bit")
    ENDIF(CMAKE_CL_64)
ELSEIF(UNIX)
    IF(APPLE)
        message(
            FATAL_ERROR
            "Target platform: MacOS 64bit - Not supported!"
        )
    ENDIF(APPLE)

    IF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message("Target platform: Unix 64bit")
    ELSE("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message("Target platform: Unix 32bit")
    ENDIF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
ELSE(WIN32)
    message(
        FATAL_ERROR
        "Target platform - Not supported!"
    )
ENDIF(WIN32)

###############################################################################
# General project settings
###############################################################################

IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(
        CMAKE_BUILD_TYPE
        "Release"
        CACHE
        STRING
        "Create Release Version"
        FORCE
    )
ENDIF(NOT DEFINED CMAKE_BUILD_TYPE)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_PROJECT_NAME "qstopmotion")
SET(QSTOPMOTION_HOMEPAGE "http://www.qstopmotion.org")
SET(QSTOPMOTION_VERSION_MAJOR "2")
SET(QSTOPMOTION_VERSION_MINOR "5")
SET(QSTOPMOTION_VERSION_PATCH "0")

SET(FILES_TO_TRANSLATE )

###############################################################################
# Qt5 support
###############################################################################

# Enable/disable Qt functionality
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# find and setup Qt5 for this poroject
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Xml)

message("Qt5 Version: ${Qt5Widgets_VERSION_STRING}")
message("QT_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
message("QT_DEFINITIONS: ${QT_DEFINITIONS}")

###############################################################################
# File definitions
###############################################################################

# add all header files of the project to HEADERS
SET(
    HEADERS
    src/frontends/observer.h
    src/frontends/frontend.h
    src/frontends/viewfacade.h
    src/frontends/qtfrontend/dialogs/licence.h
    src/frontends/qtfrontend/timeline/exposurethumbview.h
    src/frontends/qtfrontend/timeline/thumbview.h
    src/technical/util.h
    src/technical/audio/audiodriver.h
    src/technical/audio/audioformat.h
    src/technical/grabber/grabbercontrolcapabilities.h
    src/technical/grabber/grabbercontroller.h
    src/technical/grabber/grabberresolution.h
    src/technical/grabber/imageconverter.h
    src/technical/grabber/imagegrabberdevice.h
    src/technical/grabber/imagegrabberthread.h
    src/technical/videoencoder/ffmpegencoder.h
    src/technical/videoencoder/libavencoder.h
    src/technical/videoencoder/videoencoder.h
    src/technical/videoencoder/videoencoderfactory.h
)

IF(WIN32)
    SET(
        HEADERS
        ${HEADERS}
        src/technical/grabber/mediafoundation/mfcontroller.h
        src/technical/grabber/mediafoundation/mfutil.h
    )
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    SET(
        HEADERS
        ${HEADERS}
        src/technical/grabber/gphoto2/gpcontroller.h
        src/technical/grabber/video4linux2/v4l2controller.h
    )
ENDIF(UNIX AND NOT APPLE)

# add all header files of the project to MOC to MOC_HEADERS
SET(
    MOC_HEADERS
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
    src/domain/undo/undoprojectnew.h
    src/domain/undo/undoprojectopen.h
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
    src/frontends/qtfrontend/dialogs/cameracontrollerdialog.h
    src/frontends/qtfrontend/dialogs/descriptiondialog.h
    src/frontends/qtfrontend/dialogs/externalcommanddialog.h
    src/frontends/qtfrontend/dialogs/helpbrowser.h
    src/frontends/qtfrontend/dialogs/startdialog.h
    src/frontends/qtfrontend/elements/clickablelabel.h
    src/frontends/qtfrontend/elements/flexiblelineedit.h
    src/frontends/qtfrontend/elements/flexiblespinbox.h
    src/frontends/qtfrontend/frameview/frameviewimage.h
    src/frontends/qtfrontend/frameview/frameviewinterface.h
    src/frontends/qtfrontend/preferences/controllerwidget.h
    src/frontends/qtfrontend/preferences/exportwidget.h
    src/frontends/qtfrontend/preferences/generaldialog.h
    src/frontends/qtfrontend/preferences/generalwidget.h
    src/frontends/qtfrontend/preferences/grabberwidget.h
    src/frontends/qtfrontend/preferences/importwidget.h
    src/frontends/qtfrontend/preferences/projectdialog.h
    src/frontends/qtfrontend/preferences/projectwidget.h
    src/frontends/qtfrontend/preferences/transformwidget.h
    src/frontends/qtfrontend/timeline/timeline.h
    src/frontends/qtfrontend/tooltabs/compositingtab.h
    src/frontends/qtfrontend/tooltabs/projecttab.h
    src/frontends/qtfrontend/tooltabs/recordingtab.h
    src/frontends/qtfrontend/tooltabs/viewtab.h
    src/technical/externalchangemonitor.h
    src/technical/preferenceselement.h
    src/technical/preferencestool.h
    src/technical/grabber/imagegrabber.h
    src/technical/grabber/imagegrabberfacade.h
)

IF(WIN32)
    SET(
        MOC_HEADERS
        ${MOC_HEADERS}
        src/technical/grabber/mediafoundation/mfgrabber.h
    )
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    SET(
        MOC_HEADERS
        ${MOC_HEADERS}
        src/technical/grabber/gphoto2/gpgrabber.h
        src/technical/grabber/video4linux2/v4l2grabber.h
    )
ENDIF(UNIX AND NOT APPLE)

# add all source files to SOURCES
SET(
    SOURCES
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
    src/domain/undo/undoprojectnew.cpp
    src/domain/undo/undoprojectopen.cpp
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
    src/frontends/qtfrontend/dialogs/cameracontrollerdialog.cpp
    src/frontends/qtfrontend/dialogs/descriptiondialog.cpp
    src/frontends/qtfrontend/dialogs/externalcommanddialog.cpp
    src/frontends/qtfrontend/dialogs/helpbrowser.cpp
    src/frontends/qtfrontend/dialogs/startdialog.cpp
    src/frontends/qtfrontend/elements/clickablelabel.cpp
    src/frontends/qtfrontend/elements/flexiblelineedit.cpp
    src/frontends/qtfrontend/elements/flexiblespinbox.cpp
    src/frontends/qtfrontend/frameview/frameviewimage.cpp
    src/frontends/qtfrontend/frameview/frameviewinterface.cpp
    src/frontends/qtfrontend/preferences/controllerwidget.cpp
    src/frontends/qtfrontend/preferences/exportwidget.cpp
    src/frontends/qtfrontend/preferences/generaldialog.cpp
    src/frontends/qtfrontend/preferences/generalwidget.cpp
    src/frontends/qtfrontend/preferences/grabberwidget.cpp
    src/frontends/qtfrontend/preferences/importwidget.cpp
    src/frontends/qtfrontend/preferences/projectdialog.cpp
    src/frontends/qtfrontend/preferences/projectwidget.cpp
    src/frontends/qtfrontend/preferences/transformwidget.cpp
    src/frontends/qtfrontend/timeline/timeline.cpp
    src/frontends/qtfrontend/timeline/exposurethumbview.cpp
    src/frontends/qtfrontend/timeline/thumbview.cpp
    src/frontends/qtfrontend/tooltabs/compositingtab.cpp
    src/frontends/qtfrontend/tooltabs/projecttab.cpp
    src/frontends/qtfrontend/tooltabs/recordingtab.cpp
    src/frontends/qtfrontend/tooltabs/viewtab.cpp
    src/technical/externalchangemonitor.cpp
    src/technical/preferenceselement.cpp
    src/technical/preferencestool.cpp
    src/technical/util.cpp
    src/technical/grabber/grabbercontrolcapabilities.cpp
    src/technical/grabber/grabbercontroller.cpp
    src/technical/grabber/grabberresolution.cpp
    src/technical/grabber/imageconverter.cpp
    src/technical/grabber/imagegrabber.cpp
    src/technical/grabber/imagegrabberdevice.cpp
    src/technical/grabber/imagegrabberfacade.cpp
    src/technical/grabber/imagegrabberthread.cpp
    src/technical/videoencoder/ffmpegencoder.cpp
    src/technical/videoencoder/libavencoder.cpp
    src/technical/videoencoder/videoencoder.cpp
    src/technical/videoencoder/videoencoderfactory.cpp
)

IF(WIN32)
    SET(
        SOURCES
        ${SOURCES}
        src/technical/grabber/mediafoundation/mfcontroller.cpp
        src/technical/grabber/mediafoundation/mfgrabber.cpp
        src/technical/grabber/mediafoundation/mfutil.cpp
    )
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    SET(
        SOURCES
        ${SOURCES}
        src/technical/grabber/gphoto2/gpcontroller.cpp
        src/technical/grabber/gphoto2/gpgrabber.cpp
        src/technical/grabber/video4linux2/v4l2controller.cpp
        src/technical/grabber/video4linux2/v4l2grabber.cpp
    )
ENDIF(UNIX AND NOT APPLE)

SET(
    TRANSLATIONS
#    translations/qstopmotion_br.ts       # Brazilian
    translations/qstopmotion_cz.ts       # Czech
    translations/qstopmotion_da.ts       # Danish
    translations/qstopmotion_de.ts       # German
    translations/qstopmotion_el.ts       # Greek
    translations/qstopmotion_fr.ts       # French
    translations/qstopmotion_es.ts       # Spanish
#    translations/qstopmotion_fi.ts       # Finnish
    translations/qstopmotion_it.ts       # Italian
#    translations/qstopmotion_kl.ts       #
#    translations/qstopmotion_nb.ts       # Norwegian
    translations/qstopmotion_pt.ts       # Portuguese
    translations/qstopmotion_ru.ts       # Russian
    translations/qstopmotion_se.ts       # Swedish
#    translations/qstopmotion_sl.ts       # Slovene
#    translations/qstopmotion_tr.ts       # Turkish
)

SET(
    GENERAL_FILES
    AUTHORS               # The authors
    README.txt            # The readme for the users
    COPYING               # GPL2 License
    Changes.txt           # Change log
)

SET(
    UNIX_FILES
    linux/qstopmotion.desktop   # .desktop is installed in a separate step Unix systems
    linux/qstopmotion.mime      # ???
    linux/qstopmotion.1         # Linux man page
    linux/qstopmotion.1.gz      # Linux man page compressed
)

SET(
    DEB_FILES
    deb/qstopmotion.xpm       # DEB Icon is installed in a separate step
    deb/changelog             # DEB changelog file
    deb/changelog.gz          # DEB changelog file compressed
    deb/copyright             # DEB copyright file
    deb/qstopmotion.menu      # DEB .menu is installed in a separate step for Debian systems
    deb/postinst              # DEB post installation script file, call update-menus
)

SET(
    RPM_FILES
    rpm/qstopmotion.png       # RPM Icon is installed in a separate step
)

###############################################################################
# Qwt support
#
# (UNIX and Windows)
#
# used libraries: libqwt
###############################################################################


IF(UNIX AND NOT APPLE)
    # Linux

    SET(
        QT_INCLUDE_DIR
        ${Qt5Widgets_INCLUDE_DIRS}
    )

    FIND_PACKAGE(Qwt REQUIRED)
    message("Qwt Version: ${QWT_VERSION_STRING}")
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
    # Windows

    SET(CONFIG_DIR "")
    SET(
        3RD_PARTY_DIR
        ${CMAKE_SOURCE_DIR}/3rd-party
    )

    message("========== External Project qwt Start ==========")
    include(ExternalProject)

    SET(
        QWT_TARGET
        qwt2
    )
    SET(
        QWT_PREFIX
        ${3RD_PARTY_DIR}/qwt
    )
    SET(
        QWT_BUILD_SCRIPT
        ${QWT_PREFIX}/build.bat
    )

    ExternalProject_Add(
        ${QWT_TARGET}
        PREFIX ${QWT_PREFIX}
        SVN_REPOSITORY svn://svn.code.sf.net/p/qwt/code/branches/qwt-6.1
        # SVN_REVISION   -r12345
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${QWT_BUILD_SCRIPT}
        INSTALL_COMMAND ""
        TEST_COMMAND ""
    )

    ExternalProject_Get_Property(
        ${QWT_TARGET}
        SOURCE_DIR
    )
    message("qwt source dir: ${SOURCE_DIR}")

    FIND_PROGRAM(
        QT_QMAKE_EXECUTABLE
        NAMES
        qmake
    )
    message("qmake executable: ${QT_QMAKE_EXECUTABLE}")

    IF(CMAKE_SIZEOF_VOID_P GREATER 4)
        # 64bit system
        SET(
            VC_ARG
            amd64
        )
    ELSE(CMAKE_SIZEOF_VOID_P GREATER 4)
        # 32bit system
        SET(
            VC_ARG
            x86
        )
    ENDIF(CMAKE_SIZEOF_VOID_P GREATER 4)
    message("vccvarall argument: ${VC_ARG}")

    FILE(
        TO_NATIVE_PATH
        "${QT_QMAKE_EXECUTABLE}"
        QMAKE_PATH
    )
    message("qmake path: ${QMAKE_PATH}")

    FILE(
        TO_NATIVE_PATH
        "${SOURCE_DIR}"
        QWT_SOURCE_DIR
    )
    message("qwt source dir: ${QWT_SOURCE_DIR}")

    FILE(
        WRITE
        ${QWT_BUILD_SCRIPT}
        "@call \"$ENV{VCINSTALLDIR}\\Auxiliary\\Build\\vcvarsall.bat\" ${VC_ARG}\n@cd ${QWT_SOURCE_DIR}\n@${QMAKE_PATH}\n@nmake"
    )

    SET(
        QWT_INCLUDE_DIRS
        ${SOURCE_DIR}/src
    )

    IF(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Release build
        SET(
            QWT_LIB_PATH
            ${SOURCE_DIR}/lib/qwt
        )
    ELSE(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Debug build
        SET(
            QWT_LIB_PATH
            ${SOURCE_DIR}/lib/qwtd
        )
    ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

    message("========== External Project qwt End ==========")

ENDIF(WIN32)

message("QWT_INCLUDE_DIRS: ${QWT_INCLUDE_DIRS}")
message("QWT_LIBRARY: ${QWT_LIBRARY}")
message("QWT_LIBRARIES: ${QWT_LIBRARIES}")
message("QWT_LIB_PATH: ${QWT_LIB_PATH}")

###############################################################################
# gPhoto2 support
#
# (Only UNIX)
#
# used libraries: libgphoto2
###############################################################################

IF(UNIX AND NOT APPLE)
    find_package(PkgConfig)
    pkg_check_modules(GPHOTO2 REQUIRED libgphoto2)
    message("GPHOTO2_INCLUDE_DIR: ${GPHOTO2_INCLUDE_DIR}")
    message("GPHOTO2_LIBRARIES: ${GPHOTO2_LIBRARIES}")
ELSE(UNIX AND NOT APPLE)
    SET(GPHOTO2_INCLUDE_DIR)
    SET(GPHOTO2_LIBRARIES)
ENDIF(UNIX AND NOT APPLE)

###############################################################################
# Video4Linux2 support
#
# (Only UNIX)
#
# used libraries: libv4l2
###############################################################################

IF(UNIX AND NOT APPLE)
    find_package(V4L2 REQUIRED)
    message("V4L2_INCLUDE_DIRS: ${V4L2_INCLUDE_DIRS}")
    message("V4L2_LIBRARIES: ${V4L2_LIBRARIES}")
ELSE(UNIX AND NOT APPLE)
    SET(V4L2_INCLUDE_DIRS)
    SET(V4L2_LIBRARIES)
ENDIF(UNIX AND NOT APPLE)

###############################################################################
# Microsoft Media Foundation support
#
# (Only Windows form Windows Vista and newer)
#
# used libraries: evr.lib, mf.lib, mfplat.lib
###############################################################################

IF(WIN32)
    SET(
        MEDIAFOUNDATION_LIBRARIES
        evr.lib          # ??? MFCreateVideoMixer, MFCreateVideoOTA, MFCreateVideoPresenter, MFCreateVideoSampleAllocator
        mf.lib           # MFEnumDeviceSources
        mfplat.lib       # MFCreateMediaType, MFCreateAttributes, MFStartup, MFShutdown
        mfuuid.lib       # ???
        mfreadwrite.lib  # Read and write media stream: MFCreateSourceReaderFromMediaSource
        # strmiids.lib      # Old Direct Show library
    )
ELSE(WIN32)
    SET(MEDIAFOUNDATION_LIBRARIES)
ENDIF(WIN32)

###############################################################################
# Directory definitions
###############################################################################

# tell cmake where to search for headers:
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${GPHOTO2_INCLUDE_DIR}
    ${V4L2_INCLUDE_DIRS}
    ${QWT_INCLUDE_DIRS}
)

# tell cmake where to search for libraries:
IF(NOT WIN32)
    link_directories(
        ${QWT_LIBRARY}
    )
ENDIF(NOT WIN32)

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

IF(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
ENDIF(WIN32)

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

MARK_AS_ADVANCED(
    EXECUTABLE_OUTPUT_PATH
    RUNTIME_OUTPUT_DIRECTORY
)

IF(UNIX AND NOT APPLE)
    # Linux

    # Create a executable
    add_executable(
        qstopmotion
        ${SOURCES}
        ${MOC_SOURCES}
    )

    # tell cmake which libraries to link
    target_link_libraries(
        qstopmotion
        ${QWT_LIBRARIES}
        ${GPHOTO2_LIBRARIES}
        ${V4L2_LIBRARIES}
        Qt5::Widgets
    )
ELSEIF(APPLE)
    # MacOSX

    # Create a executable
    add_executable(
        qstopmotion
        MACOSX_BUNDLE
        ${SOURCES}
        ${MOC_SOURCES}
    )

    # tell cmake which libraries to link
    target_link_libraries(
        qstopmotion
        ${QWT_LIBRARIES}
        ${GPHOTO2_LIBRARIES}
        ${V4L2_LIBRARIES}
        Qt5::Widgets
    )
ELSEIF(WIN32)
    # Windows

    # Create a executable
    add_executable(
        qstopmotion
        WIN32
        src/qstopmotion.rc
        ${SOURCES}
        ${MOC_SOURCES}
    )

    add_dependencies(
        qstopmotion
        ${QWT_TARGET}
    )

    # Copy compiled Qwt library to bin/ folder - it needs for run application
    ADD_CUSTOM_COMMAND(
        TARGET qstopmotion
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${QWT_LIB_PATH}.dll ${EXECUTABLE_OUTPUT_PATH}/${CONFIG_DIR}
        COMMENT "copy ${QWT_LIB_PATH}.dll to bin/${CONFIG_DIR}"
    )

    # tell cmake which libraries to link
    target_link_libraries(
        qstopmotion
        ${QWT_LIB_PATH}.lib
        ${MEDIAFOUNDATION_LIBRARIES}
        Qt5::WinMain
        Qt5::Widgets
    )
ENDIF()

# Use the Qt5 modules
qt5_use_modules(
    qstopmotion
    Core
    Gui
    Widgets
    Xml
    Multimedia
)

add_definitions(
    ${QT_DEFINITIONS}
)

IF(WIN32)
    # Copy data to build directory
    file(COPY graphics DESTINATION ".")
    file(COPY manual DESTINATION ".")
    file(COPY translations DESTINATION ".")
    file(COPY ${ICON_FILES} DESTINATION ".")
    file(COPY sounds DESTINATION ".")
    file(COPY styles DESTINATION ".")
    file(COPY ${GENERAL_FILES} DESTINATION ".")
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    file(COPY graphics DESTINATION "share/qstopmotion/")
    file(COPY manual/ DESTINATION "share/doc/qstopmotion/")
    file(COPY translations DESTINATION "share/qstopmotion/")
    file(COPY icons DESTINATION "share/qstopmotion/")
    file(COPY sounds DESTINATION "share/qstopmotion/")
    file(COPY styles DESTINATION "share/qstopmotion/")
    file(COPY ${ICON_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${GENERAL_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${UNIX_FILES} DESTINATION "share/qstopmotion/")
    file(COPY ${DEB_FILES} DESTINATION "share/qstopmotion/deb/")
    file(COPY ${RPM_FILES} DESTINATION "share/qstopmotion/rpm/")
ENDIF(UNIX AND NOT APPLE)

###############################################################################
# Fetch Content
###############################################################################

IF(WIN32)
    include(FetchContent)

    SET(
        FETCHCONTENT_BASE_DIR
        ${3RD_PARTY_DIR}
    )

    # Only one download, no updates
    SET(
        FETCHCONTENT_UPDATES_DISCONNECTED
        ON
    )
ENDIF(WIN32)

###############################################################################
# ffmpeg Handling
###############################################################################

IF(WIN32)
    message("========== Fetch FFMPEG Content Start ==========")

    IF(CMAKE_CL_64)
        SET(
            FFMPEG_URL
            "https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-latest-win64-static.zip"
        )
        SET(
            FFMPEG_TARGET
            ffmpeg64
        )
    ELSE(CMAKE_CL_64)
        SET(
            FFMPEG_URL
            "https://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-latest-win32-static.zip"
        )
        SET(
            FFMPEG_TARGET
            ffmpeg32
        )
    ENDIF(CMAKE_CL_64)

    FetchContent_Declare(
        ${FFMPEG_TARGET}
        URL ${FFMPEG_URL}
    )

    # Check if population has already been performed
    FetchContent_GetProperties(
        ${FFMPEG_TARGET}
    )
    string(TOLOWER ${FFMPEG_TARGET} ffmpegName)
    IF(NOT ${ffmpegName}_POPULATED)
        # Fetch the content using previously declared details
        message("FetchContent - Populate")
        FetchContent_Populate(
            ${FFMPEG_TARGET}
        )

        # Bring the populated content into the build
        message("ffmpeg source dir: ${${ffmpegName}_SOURCE_DIR}")
        message("ffmpeg binary dir: ${${ffmpegName}_BINARY_DIR}")
        message("ffmpeg populated state: ${${ffmpegName}_POPULATED}")
        # add_subdirectory(${${ffmpegName}_SOURCE_DIR} ${${ffmpegName}_BINARY_DIR})
    ENDIF()

    message("========== Fetch FFMPEG Content End ==========")
ENDIF(WIN32)

###############################################################################
# Libav Handling
###############################################################################

IF(WIN32)
    message("========== Fetch LIBAV Content Start ==========")

    IF(CMAKE_CL_64)
        SET(
            LIBAV_URL
            "http://builds.libav.org/windows/release-gpl/libav-11.3-win64.7z"
        )
        SET(
            LIBAV_TARGET
            libav64
        )
    ELSE(CMAKE_CL_64)
        SET(
            LIBAV_URL
            "http://builds.libav.org/windows/release-gpl/libav-11.3-win32.7z"
        )
        SET(
            LIBAV_TARGET
            libav32
        )
    ENDIF(CMAKE_CL_64)

    FetchContent_Declare(
        ${LIBAV_TARGET}
        URL ${LIBAV_URL}
    )

    # Check if population has already been performed
    FetchContent_GetProperties(
        ${LIBAV_TARGET}
    )
    string(TOLOWER ${LIBAV_TARGET} libavName)
    IF(NOT ${libavName}_POPULATED)
        # Fetch the content using previously declared details
        message("FetchContent - Populate")
        FetchContent_Populate(
            ${LIBAV_TARGET}
        )

        # Bring the populated content into the build
        message("libav source dir: ${${libavName}_SOURCE_DIR}")
        message("libav binary dir: ${${libavName}_BINARY_DIR}")
        message("libav populated state: ${${libavName}_POPULATED}")
        # add_subdirectory(${${libavName}_SOURCE_DIR} ${${libavName}_BINARY_DIR})
    ENDIF()

    message("========== Fetch LIBAV Content End ==========")
ENDIF(WIN32)

###############################################################################
# Installation
###############################################################################

IF(WIN32)
    # The predefined windows installer path include the qstopmotion subdirectory

    SET(QSTOPMOTION_GENERALS_INSTALL_DIR ".")
    SET(QSTOPMOTION_BIN_INSTALL_DIR "bin")
    SET(QSTOPMOTION_GRAPHICS_INSTALL_DIR "graphics")
    SET(QSTOPMOTION_GRAPHICS_ICONS_INSTALL_DIR "graphics/icons")
    SET(QSTOPMOTION_GRAPHICS_PICTURES_INSTALL_DIR "graphics/pictures")
    SET(QSTOPMOTION_SOUNDS_INSTALL_DIR "sounds")
    SET(QSTOPMOTION_STYLES_INSTALL_DIR "styles")
    SET(QSTOPMOTION_MANUAL_INSTALL_DIR "manual")
    SET(QSTOPMOTION_TRANSLATIONS_INSTALL_DIR "translations")
    SET(QSTOPMOTION_PIXMAP_INSTALL_DIR "pixmaps")
    SET(QSTOPMOTION_ICONS_INSTALL_DIR "icons")
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    # The predefined CMake-Linux installer path is "/usr/local"
    # The predefined RPM installer path is "/usr"
    # The predefined DEB installer path is "/usr"

    SET(QSTOPMOTION_GENERALS_INSTALL_DIR "share/qstopmotion")
    SET(QSTOPMOTION_BIN_INSTALL_DIR "bin")
    SET(QSTOPMOTION_GRAPHICS_INSTALL_DIR "share/qstopmotion/graphics")
    SET(QSTOPMOTION_GRAPHICS_ICONS_INSTALL_DIR "share/qstopmotion/graphics/icons")
    SET(QSTOPMOTION_GRAPHICS_PICTURES_INSTALL_DIR "share/qstopmotion/graphics/pictures")
    SET(QSTOPMOTION_SOUNDS_INSTALL_DIR "share/qstopmotion/sounds")
    SET(QSTOPMOTION_STYLES_INSTALL_DIR "share/qstopmotion/styles")
    SET(QSTOPMOTION_TRANSLATIONS_INSTALL_DIR "share/qstopmotion/translations")
    SET(QSTOPMOTION_MANUAL_INSTALL_DIR "share/doc/qstopmotion")
    SET(QSTOPMOTION_DESKTOP_INSTALL_DIR "share/applications")
    SET(QSTOPMOTION_MENUFILE_INSTALL_DIR "share/menu")
    SET(QSTOPMOTION_PIXMAP_INSTALL_DIR "share/pixmaps")
    SET(QSTOPMOTION_ICONS_INSTALL_DIR "share/icons")
    SET(QSTOPMOTION_MAN1_INSTALL_DIR "share/man/man1")
ENDIF(UNIX AND NOT APPLE)

# installation of the executables
install(
    TARGETS qstopmotion
    RUNTIME DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}
)

# installation of the content of the grapics directory
install(
    DIRECTORY graphics/
    DESTINATION ${QSTOPMOTION_GRAPHICS_INSTALL_DIR}
)

# installation of the content of the sounds directory
install(
    DIRECTORY sounds/
    DESTINATION ${QSTOPMOTION_SOUNDS_INSTALL_DIR}
)

# installation of the content of the styles directory
install(
    DIRECTORY styles/
    DESTINATION ${QSTOPMOTION_STYLES_INSTALL_DIR}
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

# installation only on Windows systems
IF(WIN32)
    message("Special task for windows package!")

    # installation of the general-purpose files
    install(
        FILES ${GENERAL_FILES}
        DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
    )
ENDIF(WIN32)

# installation only on UNIX systems
IF(UNIX AND NOT APPLE)
    # installation of the general unix files
    install(
        FILES "linux/qstopmotion.mime"
        DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
    )

    # installation of the icons
    install(
        DIRECTORY icons/
        DESTINATION ${QSTOPMOTION_ICONS_INSTALL_DIR}
    )

    # installation of the .desktop file
    install(
        FILES "linux/qstopmotion.desktop"
        DESTINATION ${QSTOPMOTION_DESKTOP_INSTALL_DIR}
    )

    # installation of the .1.gz file
    install(
        FILES "linux/qstopmotion.1.gz"
        DESTINATION ${QSTOPMOTION_MAN1_INSTALL_DIR}
    )
ENDIF(UNIX AND NOT APPLE)

# installation on DEB systems
IF(CPACK_BINARY_DEB)
    message("Special task for deb package!")

    # installation of the icon file
    install(
        FILES "deb/qstopmotion.xpm"
        DESTINATION ${QSTOPMOTION_PIXMAP_INSTALL_DIR}
    )

    # installation of the .menufile file
    install(
        FILES "deb/qstopmotion.menu"
        DESTINATION ${QSTOPMOTION_MENUFILE_INSTALL_DIR}
    )

    # installation of the changelog file
    install(
        FILES "deb/changelog.gz"
        DESTINATION ${QSTOPMOTION_MANUAL_INSTALL_DIR}
    )

    # installation of the copyright file
    install(
        FILES "deb/copyright"
        DESTINATION ${QSTOPMOTION_MANUAL_INSTALL_DIR}
    )
ENDIF(CPACK_BINARY_DEB)

# installation on RPM systems
IF(CPACK_BINARY_RPM)
    message("Special task for rpm package!")

    # installation of the general-purpose files
    install(
        FILES ${GENERAL_FILES}
        DESTINATION ${QSTOPMOTION_GENERALS_INSTALL_DIR}
    )

    # installation of the icon file
    install(
        FILES "rpm/qstopmotion.png"
        DESTINATION ${QSTOPMOTION_PIXMAP_INSTALL_DIR}
    )
ENDIF(CPACK_BINARY_RPM)

# installation on Windows systems
IF(WIN32)
    # Installing Qt runtime libraries in the same directory as the executables
    SET(QSTOPMOTION_QTRUNTIME_INSTALL_DIR "${CMAKE_BINARY_DIR}/qtruntime")
    MESSAGE("QSTOPMOTION_QTRUNTIME_INSTALL_DIR: ${QSTOPMOTION_QTRUNTIME_INSTALL_DIR}")
    INSTALL(
        CODE "MESSAGE(\"Prepare required Qt libraries.\")"
        CODE "execute_process(COMMAND windeployqt --no-translations -opengl --dir ${QSTOPMOTION_QTRUNTIME_INSTALL_DIR} ${EXECUTABLE_OUTPUT_PATH}/${CONFIG_DIR}qstopmotion.exe
                              WORKING_DIRECTORY ${QSTOPMOTION_BIN_INSTALL_DIR})"
    )
    INSTALL(
        DIRECTORY ${QSTOPMOTION_QTRUNTIME_INSTALL_DIR}/
        DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}/
    )

    # installation of Qwt
    install(
        FILES ${QWT_LIB_PATH}.dll
        DESTINATION "${QSTOPMOTION_BIN_INSTALL_DIR}"
    )

    # installation of ffmpeg and libav
    install(
        DIRECTORY ${${ffmpegName}_SOURCE_DIR}/
        DESTINATION "${QSTOPMOTION_GENERALS_INSTALL_DIR}/ffmpeg"
    )
    install(
        DIRECTORY ${${libavName}_SOURCE_DIR}/usr/
        DESTINATION "${QSTOPMOTION_GENERALS_INSTALL_DIR}/libav"
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

SET(
    CPACK_PACKAGE_NAME
    "qstopmotion"
)
SET(
    CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Create stop motion animations"
)
SET(
    CPACK_PACKAGE_VENDOR
    "Ralf Lange, qstopmotion.org"
)
SET(
    CPACK_PACKAGE_CONTACT
    "Ralf Lange <ralf.lange@qstopmotion.org>"
)
SET(
    CPACK_PACKAGE_DESCRIPTION_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/README.txt"
)
SET(
    CPACK_RESOURCE_FILE_LICENSE
    "${CMAKE_CURRENT_SOURCE_DIR}/COPYING"
)
SET(
    CPACK_PACKAGE_ICON
    "${CMAKE_CURRENT_SOURCE_DIR}\\\\graphics\\\\qstopmotion_logo_installer.bmp"
)
SET(
    CPACK_PACKAGE_INSTALL_DIRECTORY
    "qStopMotion ${QSTOPMOTION_VERSION_MAJOR}.${QSTOPMOTION_VERSION_MINOR}.${QSTOPMOTION_VERSION_PATCH}"
)
SET(
    CPACK_PACKAGE_EXECUTABLES
    "qstopmotion"
    "qStopMotion"
)
SET(
    CPACK_STRIP_FILES
    "qstopmotion"
)
SET(
    CPACK_SOURCE_STRIP_FILES
    ""
)
SET(
    CPACK_PACKAGE_VERSION_MAJOR
    "${QSTOPMOTION_VERSION_MAJOR}"
)
SET(
    CPACK_PACKAGE_VERSION_MINOR
    "${QSTOPMOTION_VERSION_MINOR}"
)
SET(
    CPACK_PACKAGE_VERSION_PATCH
    "${QSTOPMOTION_VERSION_PATCH}"
)
SET(
    CPACK_TOPLEVEL_TAG
    "/user"
)
# CPACK_PACKAGE_VERSION - generated from major, minor, patch

IF(UNIX AND NOT APPLE)
    # Try to find architecture
    execute_process (COMMAND uname -m OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE)
    string (STRIP "${CPACK_PACKAGE_ARCHITECTURE}" CPACK_PACKAGE_ARCHITECTURE)
    # Try to find distro name and distro-specific arch
    execute_process (COMMAND lsb_release -is OUTPUT_VARIABLE LSB_ID)
    execute_process (COMMAND lsb_release -rs OUTPUT_VARIABLE LSB_RELEASE)
    string (STRIP "${LSB_ID}" LSB_ID)
    string (STRIP "${LSB_RELEASE}" LSB_RELEASE)
    SET (LSB_DISTRIB "${LSB_ID}${LSB_RELEASE}")
    IF (NOT LSB_DISTRIB)
        SET(
            LSB_DISTRIB
            "unix"
        )
    ENDIF (NOT LSB_DISTRIB)
    SET(
        CPACK_SYSTEM_NAME
        "${LSB_DISTRIB}-${CPACK_PACKAGE_ARCHITECTURE}"
    )

MESSAGE("UNAME Infos =========================================================")
MESSAGE("UNAME_ARCHITECTURE: ${UNAME_ARCHITECTURE}")
MESSAGE("UNAME_LINUX_VERSION: ${UNAME_LINUX_VERSION}")
MESSAGE("LSB Infos ===========================================================")
MESSAGE("LSB_ID: ${LSB_ID}")
MESSAGE("LSB_RELEASE: ${LSB_RELEASE}")
MESSAGE("LSB_DISTRIB: ${LSB_DISTRIB}")
MESSAGE("Infos ===============================================================")
MESSAGE("CPACK_SYSTEM_NAME: ${CPACK_SYSTEM_NAME}")
MESSAGE("CPACK_PACKAGE_ARCHITECTURE: ${CPACK_PACKAGE_ARCHITECTURE}")
MESSAGE("End Infos ===========================================================")

ENDIF(UNIX AND NOT APPLE)

###############################################################################
# Source Packages
###############################################################################

IF(WIN32)
    # Exclude:
    # * Mercurial SMC files
    SET(
        CPACK_SOURCE_IGNORE_FILES
        "${CPACK_SOURCE_IGNORE_FILES}"
        "${PROJECT_SOURCE_DIR}/.hg/"
        "${PROJECT_SOURCE_DIR}/.hgignore"
        "${PROJECT_SOURCE_DIR}/.hgtags"
        "${PROJECT_SOURCE_DIR}/.hgtimestamp"
        "${PROJECT_SOURCE_DIR}/3rd-party/"
        "${PROJECT_SOURCE_DIR}/cmake-old/"
    )
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    # Exclude:
    # * Mercurial SMC files
    SET(
        CPACK_SOURCE_IGNORE_FILES
        "${CPACK_SOURCE_IGNORE_FILES}"
        "${PROJECT_SOURCE_DIR}/.hg/"
        "${PROJECT_SOURCE_DIR}/.hgignore"
        "${PROJECT_SOURCE_DIR}/.hgtags"
        "${PROJECT_SOURCE_DIR}/.hgtimestamp"
    )
ENDIF(UNIX AND NOT APPLE)

###############################################################################
# NSIS - Windows only
###############################################################################

IF(WIN32)
    # Installers for 32- vs. 64-bit CMake:
    #  - Root install directory (displayed to end user at installer-run time)
    #  - "NSIS package/display name" (text used in the installer GUI)
    #  - Registry key used to store info about the installation
    IF(CMAKE_CL_64)
        # 64-bit Windows
        SET(
            CPACK_NSIS_INSTALL_ROOT
            "$PROGRAMFILES64"
        )
        SET(
            CPACK_NSIS_PACKAGE_NAME
            "${CPACK_PACKAGE_INSTALL_DIRECTORY} (Win64)"
        )
        SET(
            CPACK_PACKAGE_INSTALL_REGISTRY_KEY
            "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION} (Win64)"
        )
    ELSE(CMAKE_CL_64)
        # 32-bit Windows
        SET(
            CPACK_NSIS_INSTALL_ROOT
            "$PROGRAMFILES"
        )
        SET(
            CPACK_NSIS_PACKAGE_NAME
            "${CPACK_PACKAGE_INSTALL_DIRECTORY}"
        )
        SET(
            CPACK_PACKAGE_INSTALL_REGISTRY_KEY
            "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}"
        )
    ENDIF(CMAKE_CL_64)

    MESSAGE("CPACK_NSIS_INSTALL_ROOT: ${CPACK_NSIS_INSTALL_ROOT}")
    MESSAGE("CPACK_NSIS_PACKAGE_NAME: ${CPACK_NSIS_PACKAGE_NAME}")
    MESSAGE("CPACK_PACKAGE_INSTALL_REGISTRY_KEY: ${CPACK_PACKAGE_INSTALL_REGISTRY_KEY}")

    # Installing Microsoft run time libraries in the same directory as the executables
    SET(
        CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP
        true
    )
    INCLUDE(InstallRequiredSystemLibraries)
    MESSAGE("CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS: ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")
    INSTALL(
        PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION ${QSTOPMOTION_BIN_INSTALL_DIR}
    )

    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.

    # Set the icons for the installer and the uninstaller
    SET(
        CPACK_NSIS_MUI_ICON
        "${CMAKE_CURRENT_SOURCE_DIR}/graphics/qstopmotion.ico"
    )
    SET(
        CPACK_NSIS_MUI_UNIICON
        "${CMAKE_CURRENT_SOURCE_DIR}/graphics/qstopmotion.ico"
    )
    # Set the project name on the first page
    # Use CPACK_PACKAGE_INSTALL_DIRECTORY
    # Set the bitmap for the second and following pages
    # Use CPACK_PACKAGE_ICON
    # Set the licence text for the second page
    # Use CPACK_RESOURCE_FILE_LICENSE
    # Enable modify path environment variable on third page
    SET(
        CPACK_NSIS_MODIFY_PATH
        ON
    )
    # Set the executable name for the desktop icon on third page
    SET(
        CPACK_CREATE_DESKTOP_LINKS
        qstopmotion
    )
    # Create shortcuts in the start menu
    # Use CPACK_PACKAGE_EXECUTABLES
    SET(CPACK_NSIS_MENU_LINKS
        "manual/index.html" "qStopMotion Help"
        "http://www.qstopmotion.org" "qStopMotion Web Site"
    )

    # SET(CPACK_NSIS_INSTALLED_ICON_NAME "qstopmotion.exe")
    # SET(CPACK_NSIS_DISPLAY_NAME "qStopMotion")
    # SET(CPACK_NSIS_PACKAGE_NAME "qStopMotion")
    # SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.qstopmotion.org")
    # SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.longsoft.de")
    SET(
        CPACK_NSIS_CONTACT
        ${CPACK_PACKAGE_VENDOR}
    )

    # Run the application on the finish page
    # SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "qstopmotion.exe"

ENDIF(WIN32)

###############################################################################
# DEB package - Linux only (Debian, Ubuntu, Mint, ...)
###############################################################################

IF ("${LSB_DISTRIB}" MATCHES "Ubuntu|Debian|LinuxMint")
    # We need to alter the architecture names as per distro rules
    IF ("${CPACK_PACKAGE_ARCHITECTURE}" MATCHES "i[3-6]86")
        SET(
            CPACK_PACKAGE_ARCHITECTURE
            "i386"
        )
    ENDIF ("${CPACK_PACKAGE_ARCHITECTURE}" MATCHES "i[3-6]86")
    IF ("${CPACK_PACKAGE_ARCHITECTURE}" MATCHES "x86_64")
        SET(
            CPACK_PACKAGE_ARCHITECTURE
            "amd64"
        )
    ENDIF ("${CPACK_PACKAGE_ARCHITECTURE}" MATCHES "x86_64")
    SET(
        CPACK_SYSTEM_NAME
        "${LSB_DISTRIB}-${CPACK_PACKAGE_ARCHITECTURE}"
    )

    # Set the dependencies based on the distro version
    # thus only one package is build you must list here *any* depending libraries,
    # even if they are only used in one module
    IF ("${LSB_DISTRIB}" MATCHES "Ubuntu16.04")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), ffmpeg (>=7:3.2), libqt5core5 (>=5.5.0) | libqt5core5a (>=5.5.0), libqt5gui5 (>=5.5.0), libqt5multimedia5 (>=5.5.0), libqt5network5 (>=5.5.0), libqt5svg5 (>=5.5.0), libqt5widgets5 (>=5.5.0), libqt5xml5 (>=5.5.0), libqwt-qt5-6 (>=6.1.2), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.5.0)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "Ubuntu16.04")
    IF ("${LSB_DISTRIB}" MATCHES "Ubuntu18.04")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), ffmpeg (>=7:3.2), libqt5core5 (>=5.7.0) | libqt5core5a (>=5.7.0), libqt5gui5 (>=5.7.0), libqt5multimedia5 (>=5.7.0), libqt5network5 (>=5.7.0), libqt5svg5 (>=5.7.0), libqt5widgets5 (>=5.7.0), libqt5xml5 (>=5.7.0), libqwt-qt5-6 (>=6.1.2), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.7.0)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "Ubuntu18.04")
    IF ("${LSB_DISTRIB}" MATCHES "LinuxMint19.2")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), ffmpeg (>=7:2.8.6), libqt5core5 (>=5.3.2) | libqt5core5a (>=5.3.2), libqt5gui5 (>=5.3.2), libqt5widgets5 (>=5.3.2), libqt5xml5 (>=5.3.2), libqwt-qt5-6 (>=6.1.0), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.3.2)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "LinuxMint19.2")
    IF ("${LSB_DISTRIB}" MATCHES "Debian8.*")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), libav-tools (>=6:11.3), libqt5core5 (>=5.3.2) | libqt5core5a (>=5.3.2), libqt5gui5 (>=5.3.2), libqt5widgets5 (>=5.3.2), libqt5xml5 (>=5.3.2), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.3.2)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "Debian8.*")
    IF ("${LSB_DISTRIB}" MATCHES "Debian9.*")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), ffmpeg (>=7:3.2), libqt5core5 (>=5.7.0) | libqt5core5a (>=5.7.0), libqt5gui5 (>=5.7.0), libqt5multimedia5 (>=5.7.0), libqt5network5 (>=5.7.0), libqt5svg5 (>=5.7.0), libqt5widgets5 (>=5.7.0), libqt5xml5 (>=5.7.0), libqwt-qt5-6 (>=6.1.2), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.7.0)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "Debian9.*")
    IF ("${LSB_DISTRIB}" MATCHES "Debian10.*")
        SET(
            CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>=2.11), ffmpeg (>=7:4.1), libqt5core5 (>=5.11.0) | libqt5core5a (>=5.11.0), libqt5gui5 (>=5.11.0), libqt5multimedia5 (>=5.11.0), libqt5network5 (>=5.11.0), libqt5svg5 (>=5.11.0), libqt5widgets5 (>=5.11.0), libqt5xml5 (>=5.11.0), libqwt-qt5-6 (>=6.1.4), libv4l-0 (>=1.0.0), libgphoto2-6 (>=2.5.3), qt5-image-formats-plugins (>=5.11.0)"
        )
    ENDIF ("${LSB_DISTRIB}" MATCHES "Debian10.*")

    # Test unsupported Debian/Ubuntu/Mint version
    IF (NOT CPACK_DEBIAN_PACKAGE_DEPENDS)
        MESSAGE ("WARNING: ${LSB_DISTRIB} not supported yet.\nPlease set deps in CMakeLists.txt before packaging.")
    ENDIF (NOT CPACK_DEBIAN_PACKAGE_DEPENDS)
    IF ("${CPACK_DEBIAN_PACKAGE_DEPENDS}" STREQUAL "")
        MESSAGE ("WARNING: ${LSB_DISTRIB} not supported yet.\nPlease set deps in CMakeLists.txt before packaging.")
    ENDIF ("${CPACK_DEBIAN_PACKAGE_DEPENDS}" STREQUAL "")

    MESSAGE("CPACK_DEBIAN_PACKAGE_DEPENDS: ${CPACK_DEBIAN_PACKAGE_DEPENDS}")

    # Mandatory
    # CPACK_DEBIAN_PACKAGE_NAME = CPACK_PACKAGE_NAME
    MESSAGE("CPACK_DEBIAN_PACKAGE_NAME: ${CPACK_DEBIAN_PACKAGE_NAME}")
    # CPACK_DEBIAN_PACKAGE_VERSION = CPACK_PACKAGE_VERSION
    MESSAGE("CPACK_DEBIAN_PACKAGE_VERSION: ${CPACK_DEBIAN_PACKAGE_VERSION}")
    # CPACK_DEBIAN_PACKAGE_ARCHITECTURE = dpkg output       # dpkg --print-architecture ???
    MESSAGE("CPACK_DEBIAN_PACKAGE_ARCHITECTURE: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    # CPACK_DEBIAN_PACKAGE_MAINTAINER = CPACK_PACKAGE_CONTACT
    MESSAGE("CPACK_DEBIAN_PACKAGE_MAINTAINER: ${CPACK_DEBIAN_PACKAGE_MAINTAINER}")
    SET(
        CPACK_DEBIAN_PACKAGE_DESCRIPTION
        "create stop motion animations\n qStopMotion can be used to create stop motion animations."
    )
    MESSAGE("CPACK_DEBIAN_PACKAGE_DESCRIPTION: ${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
    SET(
        CPACK_DEBIAN_PACKAGE_SECTION
        "graphics"
    )
    MESSAGE("CPACK_DEBIAN_PACKAGE_SECTION: ${CPACK_DEBIAN_PACKAGE_SECTION}")
    SET(
        CPACK_DEBIAN_PACKAGE_PRIORITY
        "optional"
    )
    MESSAGE("CPACK_DEBIAN_PACKAGE_PRIORITY: ${CPACK_DEBIAN_PACKAGE_PRIORITY}")

    # Options

    SET(
        CPACK_DEBIAN_PACKAGE_HOMEPAGE
        ${QSTOPMOTION_HOMEPAGE}
    )

    # CPACK_DEBIAN_PACKAGE_SHLIBDEPS
    # SET(CPACK_DEBIAN_PACKAGE_DEBUG "ON")
    # CPACK_DEBIAN_PACKAGE_DEBUG
    # CPACK_DEBIAN_PACKAGE_ENHANCES
    # CPACK_DEBIAN_PACKAGE_BREAKS
    # CPACK_DEBIAN_PACKAGE_CONFLICTS
    # CPACK_DEBIAN_PACKAGE_PROVIDES
    # CPACK_DEBIAN_PACKAGE_REPLACES

    SET(
        CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
        "${CMAKE_SOURCE_DIR}/deb/postinst"
    )

ENDIF ("${LSB_DISTRIB}" MATCHES "Ubuntu|Debian|LinuxMint")

###############################################################################
# RPM package - Linux only (Fedora, Red Hat, CentOS, Mandriva, OpenSuse, ...)
###############################################################################

IF ("${LSB_DISTRIB}" MATCHES "Fedora|CentOS")
    # Mandatory
    SET(
        CPACK_RPM_PACKAGE_SUMMARY
        ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}
    )
    MESSAGE("CPACK_RPM_PACKAGE_SUMMARY: ${CPACK_RPM_PACKAGE_SUMMARY}")
    # CPACK_RPM_PACKAGE_NAME = CPACK_PACKAGE_NAME
    MESSAGE("CPACK_RPM_PACKAGE_NAME: ${CPACK_RPM_PACKAGE_NAME}")
    # CPACK_RPM_PACKAGE_VERSION = CPACK_PACKAGE_VERSION
    MESSAGE("CPACK_RPM_PACKAGE_VERSION: ${CPACK_RPM_PACKAGE_VERSION}")
    # SET(CPACK_RPM_PACKAGE_RELEASE "1")
    MESSAGE("CPACK_RPM_PACKAGE_RELEASE: ${CPACK_RPM_PACKAGE_RELEASE}")
    SET(
        CPACK_RPM_PACKAGE_LICENSE
        "GPLv2"
    )
    MESSAGE("CPACK_RPM_PACKAGE_LICENSE: ${CPACK_RPM_PACKAGE_LICENSE}")
    SET(
        CPACK_RPM_PACKAGE_GROUP
        "Applications/Multimedia"
    )
    MESSAGE("CPACK_RPM_PACKAGE_GROUP: ${CPACK_RPM_PACKAGE_GROUP}")
    # CPACK_RPM_PACKAGE_VENDOR = CPACK_PACKAGE_VENDOR
    MESSAGE("CPACK_RPM_PACKAGE_VENDOR: ${CPACK_RPM_PACKAGE_VENDOR}")
    # CPACK_RPM_PACKAGE_MAINTAINER = CPACK_PACKAGE_CONTACT
    MESSAGE("CPACK_RPM_PACKAGE_MAINTAINER: ${CPACK_RPM_PACKAGE_MAINTAINER}")

    # Options
    # SET(CPACK_RPM_PACKAGE_ARCHITECTURE "noarch")
    SET(
        CPACK_RPM_PACKAGE_URL
        ${QSTOPMOTION_HOMEPAGE}
    )
    # CPACK_RPM_PACKAGE_DESCRIPTION = CPACK_PACKAGE_DESCRIPTION_FILE
    # CPACK_RPM_COMPRESSION_TYPE
    # requireses must be in a aphabetic order
    # Qt5: qt5-qtbase - Core, Concurrent, DBus, Network, Sql, Test, Xml
    # Qt5: qt5-qtbase-gui - Gui, OpenGL, PrintSupport, Widgets, XcbQpa
    # Qt5: qt5-qtimageformats - Qt5 image format plugins
    # Qt5: qt5-qtmultimedia - Multimedia, MultimediaQuick, MultimediaWidgets
    # Qwt: qwt-qt5 - Qwt for Qt5
    SET(
        CPACK_RPM_PACKAGE_REQUIRES
        "ffmpeg >= 3.4.0, glibc >= 2.12, libgphoto2 >= 2.4.7, libv4l >= 0.6.3, qt5-qtbase >= 5.9.0, qt5-qtbase-gui >= 5.9.0, qt5-qtimageformats >= 5.9.0, qt5-qtsvg >= 5.9.0, qwt-qt5 >= 6.1.0"
    )
    MESSAGE("CPACK_RPM_PACKAGE_REQUIRES: ${CPACK_RPM_PACKAGE_REQUIRES}")
    # CPACK_RPM_PACKAGE_SUGGESTS
    # CPACK_RPM_PACKAGE_PROVIDES

    # SET(CPACK_RPM_SPEC_INSTALL_POST "desktop-file-validate /usr/share/applications/qstopmotion.desktop")

    # SET(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "pre_install.sh")
    # SET(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "post_install.sh")
    # SET(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE "pre_install.sh")
    # SET(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "post_install.sh")

    # Prevent conflicts with the filesystem package
    SET(
        CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
        "/usr/bin"
        "/usr/share/applications"
        "/usr/share/icons"
        "/usr/share/pixmaps"
        "/usr/share/man"
        "/usr/share/man/man1"
    )

ENDIF ("${LSB_DISTRIB}" MATCHES "Fedora|CentOS")

###############################################################################
# Activate packaging functionality
###############################################################################

INCLUDE(CPack)

###############################################################################
# "this is the end"
###############################################################################
