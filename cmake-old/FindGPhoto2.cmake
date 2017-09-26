###############################################################################
# CMake file to find the GPhoto2 files on the Linux platform.                 #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2017-09-15                                                     #
#                                                                             #
###############################################################################
#                                                                             #
# cmake macro to test if we use GPhoto2                                       #
#  GPHOTO2_FOUND - system has the GPhoto2 library                             #
#  GPHOTO2_INCLUDE_DIR - the GPhoto2 include directory                        #
#  GPHOTO2_LIBRARIES - The libraries needed to use GPhoto2                    #
#                                                                             #
###############################################################################

MESSAGE("GPhoto2.cmake")

# FIND_PACKAGE(PkgConfig REQUIRED)
find_package(PkgConfig)

# PKG_CHECK_MODULES(PKG_GPHOTO2 QUIET libgphoto2)
pkg_check_modules(GPHOTO2 REQUIRED libgphoto2)

IF(PKG_GPHOTO2_FOUND)
    MESSAGE(STATUS "xxx FOUND: GPhoto2 !")
ELSE(PKG_GPHOTO2_FOUND)
    MESSAGE(SEND_ERROR "Not found: GPhoto2 !")
    RETURN()
ENDIF(PKG_GPHOTO2_FOUND)

# FIND_PATH(
#     GPHOTO2_INCLUDE_DIR
#     NAMES videodev2.h libv4l2.h libv4lconvert.h
#     HINTS ${PKG_GPHOTO2_INCLUDEDIR} ${PKG_GPHOTO2_INCLUDE_DIRS}
#     PATH_SUFFIXES v4l2 video4linux
# )

# FIND_LIBRARY(
#     GPHOTO2_LIBRARY
#     NAMES v4l2 v4lconvert
#     HINTS ${PKG_GPHOTO2_LIBDIR} ${PKG_GPHOTO2_LIBRARY_DIRS}
# )

# SET(
#     GPHOTO2_LIBRARIES
#     ${GPHOTO2_LIBRARY}
# )

# SET(
#     GPHOTO2_INCLUDE_DIRS
#     ${GPHOTO2_INCLUDE_DIR}
# )

# include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set V4L2_FOUND to true
# if all listed variables are true
# find_package_handle_standard_args(
#    Libgphoto2
#    DEFAULT_MSG
#    GPHOTO2_LIBRARY
#    GPHOTO2_INCLUDE_DIR
#)

IF(PKG_GPHOTO2_FOUND)
    MESSAGE(STATUS "GPHOTO2_LIBRARY: ${GPHOTO2_LIBRARY}")
    MESSAGE(STATUS "GPHOTO2_INCLUDE_DIR: ${GPHOTO2_INCLUDE_DIR}")
ENDIF(PKG_GPHOTO2_FOUND)

MARK_AS_ADVANCED(
    GPHOTO2_LIBRARIES
    GPHOTO2_INCLUDE_DIRS
)

