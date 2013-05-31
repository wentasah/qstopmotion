Building qStopMotion using Qt-Creator IDE on Linux
Last Change: 2013-02-28

1. Preparation of the development environment
================================================================================

1.1 Install necessary libraries with the package manager
--------------------------------------------------------------------------------

* g++ (Version >= 4.4.3)
* qt libraries (32 or 64 bit, Version >= 4.6.3)
  - qt
  - qt-devel
  - qt-examples
  - qt-doc
* CMake (Version >= 2.8.7)
  - The version 2.8.7 ist the first that can create working Debian packages
  - If the package in the package manager is an older version install CMake
    from the source
* gstreamer (Version >= 0.10.28)
  - gstreamer
  - gstreamer-dev
  - gstreamer-dbg
  - gstreamer-plugins-base (videotestsrc, ffmpegcolorspace, videorate, videoscale)
  - gstreamer-plugins-base-dev
  - gstreamer-plugins-base-dbg
  - gstreamer-plugins-good (v4l2src, dv1394src, dvdemux, dvdec, jpegenc, multifilesink)
  - gstreamer-plugins-good-dbg
  - (gstreamer-plugins-ugly)
  - (gstreamer-plugins-ugly-dbg)
  - (gstreamer-plugins-bad) (dshowvideosrc)
  - (gstreamer-plugins-bad-dbg)
  - gstreamer-ffmpeg (ffdemux_dv, ffdec_dvvideo, ffdeinterlace)
  - gstreamer-ffmpeg-dev
  - gstreamer-ffmpeg-dbg
* ffmpeg (Version >= 0.5.2)
* libxml2, libxml2-dev, libxml-dbg
* libv4l, libv4l-devel, v4l-utils, v4l-utils-dev
* libgphoto2, libgphoto2-devel

1.2 Install necessary libraries direct from the source
--------------------------------------------------------------------------------

* CMake (Version >= 2.8.7)
  - Version 2.8.6 fix a bug in the creation of debian install packages
  - download Unix/Linux source from:
    http://www.cmake.org/cmake/resources/software.html
  - Unpack the tar ball
  - Build the tools from the command line:
    > open terminal window
    > change in the cmake source directory
    > ./boostrap.sh
    > Check the output for additional packages to install and install it.
    > make
    > su to get superuser rigths
    > make install
    > make clean
  - Alternative: Build with older CMake installation
    > open terminal window
    > change in the cmake source directory
    > ./boostrap.sh
    > Check the output for additional packages to install and install it.
    > Create a build directory parallel to the cmake source direcory
    > Start the existing CMake-GUI
    > Select the source directory
    > Select the build directory
    > Press the configure button and select 'Unix Makefile' as target platform.
    > Switch on the entry 'BUILD_QtDialog'
    > Press the configure button again.
    > Press the generate button.
    > Close the CMake-GUI.
    > Open Terminal window and change in the build directory.
    > make
    > su to get superuser rigths
    > make install
    > make clean
    > delete the older CMake

1.3 Qt Creator for Linux
--------------------------------------------------------------------------------

* Use the Qt Creator from the repositories of your distribution or
* Download Qt Creator for Linux (32 or 64 bit, Version >= 2.1.0) from
  qt-project.org/downloads.
* Set execute permissions to the binary installer.
* Start the installer with root permissions.
* Install the Qt Creator to /opt/qtcreator-2.1.0/
* Create a shortcut in the panel.

2. Building qStopMotion
================================================================================

2.1 Build
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Release
* Open the CMakeLists.txt file as new project.
* Build | Build Solution
* Locking for error and warning messages

2.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Debug
* Open Options | Settings | Debugger
* On the Cdb tab enable the CDB. Normaly the path to the debugger will find
  automatically.
* Start qStopMotion in debugging mode.

3. Create source documentation
================================================================================

3.1 Install Doxygen with the package manager.
--------------------------------------------------------------------------------

* ...
