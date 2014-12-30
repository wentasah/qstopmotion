###############################################################################
# CMake file to find the GStreamer files on the Linux platform.               #
# (c) Ralf Lange, longsoft.de                                                 #
# Last update: 2014-12-28                                                     #
#                                                                             #
###############################################################################
#
# Originally based on the findGPHOTO2.cmake from the KDE project
#
# cmake macro to test if we use gphoto2
#  GPHOTO2_FOUND - system has the GPHOTO2 library
#  GPHOTO2_INCLUDE_DIR - the GPHOTO2 include directory
#  GPHOTO2_LIBRARIES - The libraries needed to use GPHOTO2
#
# Copyright (c) 2006, 2007 Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################

MESSAGE("FindGPhoto2PkgConfig.cmake")

IF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIR)
    # In cache already
    SET(
        GPHOTO2_FOUND
        TRUE
    )
    MESSAGE(STATUS "Found in cache: gphoto2 !")
    RETURN()
ENDIF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIR)

FIND_PROGRAM(
    GHOTO2CONFIG_EXECUTABLE
    NAMES
    gphoto2-config 
)
FIND_PROGRAM(
    GHOTO2PORTCONFIG_EXECUTABLE
    NAMES
    gphoto2-port-config
)

SET(GPHOTO2_LIBRARIES)
SET(GPHOTO2_INCLUDE_DIRS)

# If gphoto2-port-config and gphoto2-config have been found
IF (GHOTO2PORTCONFIG_EXECUTABLE AND GHOTO2CONFIG_EXECUTABLE)
    EXEC_PROGRAM(
        ${GHOTO2PORTCONFIG_EXECUTABLE}
        ARGS --libs
        RETURN_VALUE _return_VALUE
        OUTPUT_VARIABLE GPHOTO2PORT_LIBRARY
    )
    
    EXEC_PROGRAM(
        ${GHOTO2CONFIG_EXECUTABLE}
        ARGS --libs
        RETURN_VALUE _return_VALUE
        OUTPUT_VARIABLE GPHOTO2_LIBRARY
    )
    
    EXEC_PROGRAM(
        ${GHOTO2PORTCONFIG_EXECUTABLE}
        ARGS --cflags
        RETURN_VALUE _return_VALUE
        OUTPUT_VARIABLE _GPHOTO2PORT_RESULT_INCLUDE_DIR
    )
    
    EXEC_PROGRAM(
        ${GHOTO2CONFIG_EXECUTABLE}
        ARGS --cflags
        RETURN_VALUE _return_VALUE
        OUTPUT_VARIABLE _GPHOTO2_RESULT_INCLUDE_DIR
    )
    
    SET(
        GPHOTO2_LIBRARIES
        ${GPHOTO2PORT_LIBRARY}
        ${GPHOTO2_LIBRARY}
    )
    
    # the cflags for poppler-qt4 can contain more than one include path
    SEPARATE_ARGUMENTS(_GPHOTO2_RESULT_INCLUDE_DIR)
    FOREACH(_includedir ${_GPHOTO2_RESULT_INCLUDE_DIR})
        STRING(
            REGEX REPLACE "-I(.+)" "\\1"
            _includedir
            "${_includedir}"
        )
        SET(
            GPHOTO2_INCLUDE_DIR
            ${GPHOTO2_INCLUDE_DIR}
            ${_includedir}
        )
    ENDFOREACH(_includedir)
    
    SEPARATE_ARGUMENTS(_GPHOTO2PORT_RESULT_INCLUDE_DIR)
    FOREACH(_includedir ${_GPHOTO2PORT_RESULT_INCLUDE_DIR})
        STRING(
            REGEX REPLACE "-I(.+)" "\\1"
            _includedir
            "${_includedir}"
        )
        SET(
            GPHOTO2PORT_INCLUDE_DIR
            ${GPHOTO2PORT_INCLUDE_DIR}
            ${_includedir}
        )
    ENDFOREACH(_includedir)

    SET(
        GPHOTO2_INCLUDE_DIRS
        ${GPHOTO2PORT_INCLUDE_DIR}
        ${GPHOTO2_INCLUDE_DIR}
    )
ENDIF(GHOTO2PORTCONFIG_EXECUTABLE AND GHOTO2CONFIG_EXECUTABLE)

IF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIRS)
    SET(
        GPHOTO2_FOUND
        TRUE
    )
    MESSAGE(STATUS "Found: gphoto2 !")
    MESSAGE(STATUS "GPHOTO2_LIBRARIES: ${GPHOTO2_LIBRARIES}")
    MESSAGE(STATUS "GPHOTO2_INCLUDE_DIRS: ${GPHOTO2_INCLUDE_DIRS}")
ELSE (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIRS)
    SET(
        GPHOTO2_FOUND
        FALSE
    )
    MESSAGE(SEND_ERROR "Not found: gphoto2 !")
ENDIF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIRS)

MARK_AS_ADVANCED(
    GPHOTO2_LIBRARIES
    GPHOTO2_INCLUDE_DIRS
)

