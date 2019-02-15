cmake_minimum_required(VERSION 2.8.11)
project(QWT)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_definitions(
    -DQWT_NO_SVG
)

IF(WIN32)
    add_definitions(
        -DQT_DLL
        -DQWT_DLL
        -DQWT_MAKEDLL
    )
ENDIF(WIN32)

add_subdirectory(src)
