###############################################################################
# CMake file to find the Video4Linux2 files on the Linux platform.            #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2014-12-28                                                     #
#                                                                             #
###############################################################################
#
# cmake macro to test if we use Video4Linux2
#  V4L2_FOUND - system has the Video4Linux2 library
#  V4L2_INCLUDE_DIR - the Video4Linux2 include directory
#  V4L2_LIBRARIES - The libraries needed to use Video4Linux2
#
###############################################################################

MESSAGE("FindV4L2PkgConfig.cmake")

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(PKG_V4L2 QUIET libv4l2)

IF(PKG_V4L2_FOUND)
    MESSAGE(STATUS "FOUND: Video4Linux2 !")
ELSE(PKG_V4L2_FOUND)
    MESSAGE(SEND_ERROR "Not found: Video4Linux2 !")
    RETURN()
ENDIF(PKG_V4L2_FOUND)

FIND_PATH(
    V4L2_INCLUDE_DIR
    NAMES videodev2.h libv4l2.h libv4lconvert.h
    HINTS ${PKG_V4L2_INCLUDEDIR} ${PKG_V4L2_INCLUDE_DIRS}
    PATH_SUFFIXES v4l2 video4linux
)

FIND_LIBRARY(
    V4L2_LIBRARY
    NAMES v4l2 v4lconvert
    HINTS ${PKG_V4L2_LIBDIR} ${PKG_V4L2_LIBRARY_DIRS}
)

SET(
    V4L2_LIBRARIES
    ${V4L2_LIBRARY}
)

SET(
    V4L2_INCLUDE_DIRS
    ${V4L2_INCLUDE_DIR}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set V4L2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    Libv4l2
    DEFAULT_MSG
    V4L2_LIBRARY
    V4L2_INCLUDE_DIR
)

IF(PKG_V4L2_FOUND)
    MESSAGE(STATUS "V4L2_LIBRARY: ${V4L2_LIBRARY}")
    MESSAGE(STATUS "V4L2_INCLUDE_DIR: ${V4L2_INCLUDE_DIR}")
ENDIF(PKG_V4L2_FOUND)

MARK_AS_ADVANCED(
    V4L2_LIBRARIES
    V4L2_INCLUDE_DIRS
)

