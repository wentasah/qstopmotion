###############################################################################
# CMake file to find the GStreamer files on the Linuc platform.               #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2012-05-24                                                     #
#                                                                             #
###############################################################################
#
# Originally based on the findGStreamer.cmake from the KDE project
#
# - Try to find GStreamer
# Once done this will define
#
#  GSTREAMER_FOUND - system has GStreamer
#  GSTREAMER_INCLUDE_DIR - the GStreamer include directory
#  GSTREAMER_LIBRARIES - the libraries needed to use GStreamer
#  GSTREAMER_STATIC_LIBRARIES - the static libraries needed to use GStreamer
#  GSTREAMER_DEFINITIONS - Compiler switches required for using GStreamer
#  GSTREAMER_VERSION - the version of GStreamer
#
# Copyright (c) 2006, Tim Beaulen <tbscope@gmail.com>
# Copyright (c) 2008 Helio Chissini de Castro, <helio@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# TODO: Other versions --> GSTREAMER_X_Y_FOUND
#       (Example: GSTREAMER_0_8_FOUND and GSTREAMER_0_10_FOUND etc)
#
###############################################################################

message("FindGStreamerPkgConfig.cmake")

IF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
    GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
    GSTREAMER_APP_LIBRARY)
   # in cache already
   SET(
       GStreamer_FIND_QUIETLY
       TRUE
   )
ELSE (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
      GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
      GSTREAMER_APP_LIBRARY)
   SET(
       GStreamer_FIND_QUIETLY
       FALSE
   )
ENDIF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
       GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
       GSTREAMER_APP_LIBRARY)

FIND_PACKAGE(PkgConfig REQUIRED)
# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
# don't make this check required - otherwise you can't use macro_optional_find_package on this one
PKG_CHECK_MODULES(PKG_GSTREAMER gstreamer-0.10)

message("PKG_GSTREAMER_VERSION: ${PKG_GSTREAMER_VERSION}")
message("PKG_GSTREAMER_INCLUDE_DIRS: ${PKG_GSTREAMER_INCLUDE_DIRS}")
message("PKG_GSTREAMER_LIBRARY_DIRS: ${PKG_GSTREAMER_LIBRARY_DIRS}")
message("PKG_GSTREAMER_LIBRARIES: ${PKG_GSTREAMER_LIBRARIES}")
message("PKG_GSTREAMER_STATIC_LIBRARIES: ${PKG_GSTREAMER_STATIC_LIBRARIES}")
message("PKG_GSTREAMER_CFLAGS: ${PKG_GSTREAMER_CFLAGS}")

SET(GSTREAMER_VERSION ${PKG_GSTREAMER_VERSION})
SET(GSTREAMER_DEFINITIONS ${PKG_GSTREAMER_CFLAGS})

FIND_PATH(
    GSTREAMER_INCLUDE_DIR
    gst/gst.h
    PATHS ${PKG_GSTREAMER_INCLUDE_DIRS}
    PATH_SUFFIXES gstreamer-0.10
)

FIND_LIBRARY(
    GSTREAMER_LIBRARY
    NAMES gstreamer-0.10
    PATHS ${PKG_GSTREAMER_LIBRARY_DIRS}
)

FIND_LIBRARY(
    GSTREAMER_BASE_LIBRARY
    NAMES gstbase-0.10
    PATHS ${PKG_GSTREAMER_LIBRARY_DIRS}
)

FIND_LIBRARY(
    GSTREAMER_INTERFACE_LIBRARY
    NAMES gstinterfaces-0.10
    PATHS ${PKG_GSTREAMER_LIBRARY_DIRS}
)

FIND_LIBRARY(
    GSTREAMER_APP_LIBRARY
    NAMES gstapp-0.10
    PATHS ${PKG_GSTREAMER_LIBRARY_DIRS}
)

SET(
    GSTREAMER_LIBRARIES
    ${PKG_GSTREAMER_LIBRARIES}
)

SET(
    GSTREAMER_STATIC_LIBRARIES
    ${PKG_GSTREAMER_STATIC_LIBRARIES}
)

IF (GSTREAMER_INCLUDE_DIR)
ELSE (GSTREAMER_INCLUDE_DIR)
   MESSAGE(STATUS "GStreamer: WARNING: include dir not found")
ENDIF (GSTREAMER_INCLUDE_DIR)

IF (GSTREAMER_LIBRARY)
ELSE (GSTREAMER_LIBRARY)
   MESSAGE(STATUS "GStreamer: WARNING: library not found")
ENDIF (GSTREAMER_LIBRARY)

IF (GSTREAMER_BASE_LIBRARY)
ELSE (GSTREAMER_BASE_LIBRARY)
   MESSAGE(STATUS "GStreamer: WARNING: base library not found")
ENDIF (GSTREAMER_BASE_LIBRARY)

IF (GSTREAMER_INTERFACE_LIBRARY)
ELSE (GSTREAMER_INTERFACE_LIBRARY)
   MESSAGE(STATUS "GStreamer: WARNING: interfaces library not found")
ENDIF (GSTREAMER_INTERFACE_LIBRARY)

IF (GSTREAMER_APP_LIBRARY)
ELSE (GSTREAMER_APP_LIBRARY)
   MESSAGE(STATUS "GStreamer: WARNING: app library not found")
ENDIF (GSTREAMER_APP_LIBRARY)

IF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
    GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
    GSTREAMER_APP_LIBRARY)
   SET(
       GSTREAMER_FOUND
       TRUE
   )
   SET(
       GSTREAMER_LIBRARIES
       ${GSTREAMER_LIBRARIES}
       gstinterfaces-0.10
       gstapp-0.10
   )
ELSE (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
      GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
      GSTREAMER_APP_LIBRARY)
   SET(
       GSTREAMER_FOUND
       FALSE
   )
ENDIF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND
       GSTREAMER_BASE_LIBRARY AND GSTREAMER_INTERFACE_LIBRARY AND
       GSTREAMER_APP_LIBRARY)

MARK_AS_ADVANCED(
    GSTREAMER_INCLUDE_DIR
    GSTREAMER_LIBRARIES
    GSTREAMER_STATIC_LIBRARIES
    GSTREAMER_LIBRARY
    GSTREAMER_BASE_LIBRARY
    GSTREAMER_INTERFACES_LIBRARY
    GSTREAMER_APP_LIBRARY
)

