###############################################################################
# CMake file to find the GStreamer files on the Windows platform when the     #
# OSSBuild fof GStreamer for Windows is used.                                 #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2012-10-11                                                     #
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
unset(GSTREAMER_gstconfig_INCLUDE_DIR CACHE)
unset(GSTREAMER_gstreamer_LIBRARY CACHE)
unset(GSTREAMER_gstinterfaces_LIBRARY CACHE)
unset(GSTREAMER_gstapp_LIBRARY CACHE)

unset(GLIB_glib_2_INCLUDE_DIR CACHE)
unset(GLIB_glibconfig_2_INCLUDE_DIR CACHE)
unset(GLIB_glib_2_LIBRARY CACHE)
unset(GLIB_gobject_2_LIBRARY CACHE)

unset(LIBXML2_parser_INCLUDE_DIR CACHE)
unset(LIBXML2_iconv_INCLUDE_DIR CACHE)
unset(LIBXML2_xml_2_LIBRARY CACHE)

###############################################################################
# Read environment variables
###############################################################################

FILE(TO_CMAKE_PATH "$ENV{OSSBUILD_GSTREAMER_SDK_DIR}" TRY1_DIR)
FILE(TO_CMAKE_PATH "${GSTREAMER_DIR}" TRY2_DIR)
FILE(GLOB GSTREAMER_DIR ${TRY1_DIR} ${TRY2_DIR})
message("FindGStreamerOSSBUILD.cmake")
message("TRY1_DIR: ${TRY1_DIR}")
message("TRY2_DIR: ${TRY2_DIR}")
message("GSTREAMER_DIR: ${GSTREAMER_DIR}")

###############################################################################
# GStreamer directories and libraries
###############################################################################

FIND_PATH(
    GSTREAMER_gst_INCLUDE_DIR
    gst/gst.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/gstreamer-0.10
    ENV INCLUDE DOC "Directory containing gst/gst.h include file"
)

FIND_PATH(
    GSTREAMER_gstconfig_INCLUDE_DIR
    gst/gstconfig.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/gstreamer-0.10
    ENV INCLUDE DOC "Directory containing gst/gstconfig.h include file"
)

FIND_LIBRARY(
    GSTREAMER_gstreamer_LIBRARY
    NAMES gstreamer-0.10 gstreamer
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "gstreamer library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

FIND_LIBRARY(
    GSTREAMER_gstinterfaces_LIBRARY
    NAMES gstinterfaces-0.10 gstinterfaces
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "gstinterfaces library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

FIND_LIBRARY(
    GSTREAMER_gstapp_LIBRARY
    NAMES gstapp-0.10 gstapp
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "gstapp library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

###############################################################################
# GLib2 directories and libraries
###############################################################################

FIND_PATH(
    GLIB_glib_2_INCLUDE_DIR
    glib.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/glib-2.0
    ENV INCLUDE DOC "Directory containing glib.h include file"
)

FIND_PATH(
    GLIB_glibconfig_2_INCLUDE_DIR
    glibconfig.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/glib-2.0
    ENV INCLUDE DOC "Directory containing glibconfig.h include file"
)

FIND_LIBRARY(
    GLIB_glib_2_LIBRARY
    NAMES glib-2.0
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "glib library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

FIND_LIBRARY(
    GLIB_gobject_2_LIBRARY
    NAMES gobject-2.0
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "gobject library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

###############################################################################
# LibXml2 directories and libraries
###############################################################################

FIND_PATH(
    LIBXML2_parser_INCLUDE_DIR
    libxml/parser.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/libxml2
    ENV INCLUDE DOC "Directory containing libxml/parser.h include file"
)

FIND_PATH(
    LIBXML2_iconv_INCLUDE_DIR
    iconv.h
    PATHS ${GSTREAMER_DIR}/include ${GSTREAMER_DIR}/include/libxml2
    ENV INCLUDE DOC "Directory containing iconv.h include file"
)

FIND_LIBRARY(
    LIBXML2_xml_2_LIBRARY
    NAMES xml2
    PATHS ${GSTREAMER_DIR}/lib ${GSTREAMER_DIR}/win32/lib
    ENV LIB DOC "xml2 library to link with"
    NO_SYSTEM_ENVIRONMENT_PATH
)

###############################################################################
# All together
###############################################################################

IF(GSTREAMER_gst_INCLUDE_DIR AND GSTREAMER_gstconfig_INCLUDE_DIR AND
   GSTREAMER_gstreamer_LIBRARY AND GSTREAMER_gstinterfaces_LIBRARY AND
   GSTREAMER_gstapp_LIBRARY AND
   GLIB_glib_2_INCLUDE_DIR AND GLIB_glibconfig_2_INCLUDE_DIR AND
   GLIB_glib_2_LIBRARY AND GLIB_gobject_2_LIBRARY AND
   LIBXML2_parser_INCLUDE_DIR AND LIBXML2_iconv_INCLUDE_DIR AND LIBXML2_xml_2_LIBRARY)

    SET(
        GSTREAMER_INCLUDE_DIR
        ${GSTREAMER_gst_INCLUDE_DIR} ${GSTREAMER_gstconfig_INCLUDE_DIR}
        ${GLIB_glib_2_INCLUDE_DIR} ${GLIB_glibconfig_2_INCLUDE_DIR}
        ${LIBXML2_parser_INCLUDE_DIR} ${LIBXML2_iconv_INCLUDE_DIR}
    )
    list(REMOVE_DUPLICATES GSTREAMER_INCLUDE_DIR)
    SET(
        GSTREAMER_LIBRARIES
        ${GSTREAMER_gstreamer_LIBRARY} ${GSTREAMER_gstinterfaces_LIBRARY}
        ${GSTREAMER_gstapp_LIBRARY}
        ${GLIB_glib_2_LIBRARY} ${GLIB_gobject_2_LIBRARY}
        ${LIBXML2_xml_2_LIBRARY}
    )
    list(REMOVE_DUPLICATES GSTREAMER_LIBRARIES)
    SET(GSTREAMER_FOUND TRUE)

ELSE(GSTREAMER_gst_INCLUDE_DIR AND GSTREAMER_gstconfig_INCLUDE_DIR AND
     GSTREAMER_gstreamer_LIBRARY AND GSTREAMER_gstinterfaces_LIBRARY AND
     GSTREAMER_gstapp_LIBRARY AND
     GLIB_glib_2_INCLUDE_DIR AND GLIB_glibconfig_2_INCLUDE_DIR AND
     GLIB_glib_2_LIBRARY AND GLIB_gobject_2_LIBRARY AND
     LIBXML2_parser_INCLUDE_DIR AND LIBXML2_iconv_INCLUDE_DIR AND LIBXML2_xml_2_LIBRARY)

    message("One component not found!!!")

ENDIF(GSTREAMER_gst_INCLUDE_DIR AND GSTREAMER_gstconfig_INCLUDE_DIR AND
      GSTREAMER_gstreamer_LIBRARY AND GSTREAMER_gstinterfaces_LIBRARY AND
      GSTREAMER_gstapp_LIBRARY AND
      GLIB_glib_2_INCLUDE_DIR AND GLIB_glibconfig_2_INCLUDE_DIR AND
      GLIB_glib_2_LIBRARY AND GLIB_gobject_2_LIBRARY AND
      LIBXML2_parser_INCLUDE_DIR AND LIBXML2_iconv_INCLUDE_DIR AND LIBXML2_xml_2_LIBRARY)
