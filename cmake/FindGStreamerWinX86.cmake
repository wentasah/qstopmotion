###############################################################################
# CMake file to find the GStreamer files on the Windows platform when the     #
# x86 buid of GStreamer 1.x.x for Windows from the gstreamer project is used. #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2014-01-31                                                     #
#                                                                             #
###############################################################################
#
# Originally based on the findGStreamer.cmake from the OSSBuild project
#
#  GSTREAMER_FOUND - system has GStreamer
#  GSTREAMER_INCLUDE_DIR - the GStreamer include directory
#  GSTREAMER_LIBRARIES - the libraries needed to use GStreamer
#
###############################################################################

###############################################################################
# Unset variables
###############################################################################

unset(GSTREAMER_gst_INCLUDE_DIR CACHE)
unset(GSTREAMER_gstreamer_LIBRARY CACHE)

unset(GLIB_glib_2_INCLUDE_DIR CACHE)
unset(GLIB_glibconfig_2_INCLUDE_DIR CACHE)
unset(GLIB_glib_2_LIBRARY CACHE)

###############################################################################
# Read environment variables
###############################################################################

FILE(TO_CMAKE_PATH "$ENV{GSTREAMER_1_0_ROOT_X86}" TRY1_DIR)
FILE(TO_CMAKE_PATH "${GSTREAMER_DIR}" TRY2_DIR)
FILE(GLOB GSTREAMER_DIR ${TRY1_DIR} ${TRY2_DIR})
message("FindGStreamerWinX86.cmake - Start")
message("TRY1_DIR: ${TRY1_DIR}")
message("TRY2_DIR: ${TRY2_DIR}")
message("GSTREAMER_DIR: ${GSTREAMER_DIR}")

###############################################################################
# GStreamer directories and libraries
###############################################################################

FIND_PATH(
    GSTREAMER_gst_INCLUDE_DIR
    gst/gst.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/gstreamer-1.0
    ENV INCLUDE DOC "Directory containing gst/gst.h include file"
)
message("GSTREAMER_gst_INCLUDE_DIR: ${GSTREAMER_gst_INCLUDE_DIR}")

FIND_LIBRARY(
    GSTREAMER_gstreamer_LIBRARY
    NAMES gstreamer-1.0 gstreamer
    PATHS ${GSTREAMER_DIR}/lib
    ENV LIB DOC "gstreamer library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)
message("GSTREAMER_gstreamer_LIBRARY: ${GSTREAMER_gstreamer_LIBRARY}")

###############################################################################
# GLib2 directories and libraries
###############################################################################

FIND_PATH(
    GLIB_glib_2_INCLUDE_DIR
    glib.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/glib-2.0
    ENV INCLUDE DOC "Directory containing glib.h include file"
)
message("GLIB_glib_2_INCLUDE_DIR: ${GLIB_glib_2_INCLUDE_DIR}")

FIND_PATH(
    GLIB_glibconfig_2_INCLUDE_DIR
    glibconfig.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/glib-2.0 ${GSTREAMER_DIR}/lib/glib-2.0/include
    ENV INCLUDE DOC "Directory containing glibconfig.h include file"
)
message("GLIB_glibconfig_2_INCLUDE_DIR: ${GLIB_glibconfig_2_INCLUDE_DIR}")

FIND_LIBRARY(
    GLIB_glib_2_LIBRARY
    NAMES glib-2.0
    PATHS ${GSTREAMER_DIR}/lib
    ENV LIB DOC "glib library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)
message("GLIB_glib_2_LIBRARY: ${GLIB_glib_2_LIBRARY}")

###############################################################################
# All together
###############################################################################

IF(GSTREAMER_gst_INCLUDE_DIR AND
   GSTREAMER_gstreamer_LIBRARY AND
   GLIB_glib_2_INCLUDE_DIR AND
   GLIB_glibconfig_2_INCLUDE_DIR AND
   GLIB_glib_2_LIBRARY)

    SET(
        GSTREAMER_INCLUDE_DIR
        ${GSTREAMER_gst_INCLUDE_DIR}
        ${GLIB_glib_2_INCLUDE_DIR}
        ${GLIB_glibconfig_2_INCLUDE_DIR}
    )
    list(REMOVE_DUPLICATES GSTREAMER_INCLUDE_DIR)
    SET(
        GSTREAMER_LIBRARIES
        ${GSTREAMER_gstreamer_LIBRARY}
        ${GLIB_glib_2_LIBRARY}
    )
    list(REMOVE_DUPLICATES GSTREAMER_LIBRARIES)
    SET(GSTREAMER_FOUND TRUE)

ELSE(GSTREAMER_gst_INCLUDE_DIR AND
     GSTREAMER_gstreamer_LIBRARY AND
     GLIB_glib_2_INCLUDE_DIR AND
     GLIB_glibconfig_2_INCLUDE_DIR AND
     GLIB_glib_2_LIBRARY)

    message("One component not found!")

ENDIF(GSTREAMER_gst_INCLUDE_DIR AND
      GSTREAMER_gstreamer_LIBRARY AND
      GLIB_glib_2_INCLUDE_DIR AND
      GLIB_glibconfig_2_INCLUDE_DIR AND
      GLIB_glib_2_LIBRARY)

message("FindGStreamerWinX86.cmake - End")
