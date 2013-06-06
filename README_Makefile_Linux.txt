Building qStopMotion using Unix makefile on Linux
Last Change: 2013-02-28

1. Preparation of the development environment
================================================================================

1.1 Install necessary libraries with the package manager
--------------------------------------------------------------------------------

* g++ (Version >= 4.4.3)
* qt4 libraries (32 or 64 bit, Version >= 4.6.3)
  - libqt4, libqt4-dev, libqt4-dbg (qt4-core, qt4-gui, qt4-xml)
  - qt4-examples / qt4-demos
  - qt4-dev-tools
  - qt4-qmake
  - qt4-help, qt4-doc
* CMake (Version >= 2.8.7)
  - The version 2.8.7 ist the first that can create working Debian packages
  - If the package in the package manager is an older version install CMake
    from the source
* Mercurial Client (Version >= 1.4.3)
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

2. Building qStopMotion
================================================================================

2.1 Get the last version of the sources
--------------------------------------------------------------------------------

There are two possibilities, to get the source code:

2.1.1 Download the sources as a anonymous user
* Download the latest version of the sources as a tar ball from
  http://qstopmotion.hg.sourceforge.net:8000/hgroot/qstopmotion/qstopmotion
* Extract the tar ball

2.1.2 Use the Mercurial SCM read only
* Create a new directory for the work on qStopMotion
* Move to the new directory
* hg clone http://hg.code.sf.net/p/qstopmotion/code qstopmotion
* A new directory 'qstopmotion' will be created containing the the last version
  of the sources

2.1.3 Use the Mercurial SCM as a registered developer
* Registering as a developer on qstopmotion.sourceforge.net
* Create a new directory for the work on qStopMotion
* Move to the new directory
* hg clone ssh://USERNAME@hg.code.sf.net/p/qstopmotion/code qstopmotion
* A new directory 'qstopmotion' will be created containing the the last version
  of the sources

2.2 Create the Makefile
--------------------------------------------------------------------------------

* Create a build directory 'qstopmotion-build' parallel to the qstopmotion
  directory.
* Start the CMake GUI
* Select the qstopmotion directory as the source directory
* Select the qstopmotion-build directory as the build directory
* Press the 'Configure' button
* Select 'Unix Makefile' as target platform
* Press the 'Configure' button again
* Press the 'Generate' button

2.3 Build
--------------------------------------------------------------------------------

* Move to the qstopmotion-build directory.
* Call "make"
* Locking for error and warning messages

3. Installation
================================================================================

* Get super user permissions
* Call "make install"

4. Starting qStopMotion
================================================================================

* Select the video source
* Have fun.

5. Packaging
================================================================================

5.1 Creating source package
--------------------------------------------------------------------------------

5.1.1 Download and install ???
* Download installer from ....
* Install with default options.

5.1.2 Create packaging script for qStopMotion
* ...
* ...

5.1.3 Create the package for qStopMotion
* make package_source
* ...

5.2 Creating package for RPM
--------------------------------------------------------------------------------

5.2.1 Download and install ???
* Download installer from ....
* Install with default options.

5.2.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Add Entry of type string: 'CPACK_GENERATOR' with the value 'RPM'
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

5.2.3 Create the package for qStopMotion
* Open a command line
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.rpm' will be created
  in the directory

5.2.4 Test the new package
* Open the new created package with the software installation tool
* Install qStopMotion
* You will find the an new icon in the applications-multimedia menu

5.3 Creating package for DEB
--------------------------------------------------------------------------------

5.3.1 Download and install ???
* Download installer from ....
* Install with default options.

5.3.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Add Entry of type string: 'CPACK_GENERATOR' with the value 'DEB'
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

5.3.3 Create the package for qStopMotion
* Open a command line and be a super user with 'su'
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.deb' will be created in
  the directory

5.3.4 Test the new package
* Open the new created package with the software installation tool
* Install qStopMotion
* You will find the an new icon in the applications-multimedia menu

5.4 Creating package for Linux X11
--------------------------------------------------------------------------------

5.4.1 Download and install ???
* Download installer from ....
* Install with default options.

5.4.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

5.4.3 Create the package for qStopMotion
* Open a command line and be a super user with 'su'
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.sh' will be created in
  the directory

5.4.4 Test the new package
* Call the new shell file
* Install qStopMotion

6. Create source documentation
================================================================================

6.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from
