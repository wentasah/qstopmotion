Building qStopMotion using Qt-Creator IDE on Linux
Last Change: 2011/08/21

1. Preparation of the development environment

1.1 Install necessary libraries with the package manager
* qt libraries (32 or 64 bit, Version >= 4.6.3)
  - qt
  - qt-dev
  - qt-examples
  - qt-doc
* CMake (Version 2.8.x)
* Mercurial Client (Version >= 1.4.1)
* gstreamer (Version >= 0.10.19)
  - gstreamer
  - gstreamer-dev
  - gstreamer-dbg
  - gstreamer-plugins-base
  - gstreamer-plugins-base-dev
  - gstreamer-plugins-base-dbg
  - gstreamer-plugins-good
  - gstreamer-plugins-good-dbg
  - gstreamer-plugins-ugly
  - gstreamer-plugins-ugly-dbg
  - gstreamer-plugins-bad
  - gstreamer-plugins-bad-dbg
* ffmpeg (Version x.x.x)

2. Building qStopMotion

2.1 Get the last version of the sources

There are two possibilities, to get the source code:

2.1.1 Download the sources as a anonymous user
* Download the latest version of the sources as a tar ball from http://qstopmotion.hg.sourceforge.net:8000/hgroot/qstopmotion/qstopmotion
* Extract the tar ball

2.1.2 Use the Mercurial SCM as a registered developer
* Registering as a developer on qstopmotion.sourceforge.net
* Create a new directory for the work on qStopMotion
* Move to the new directory
* hg clone ssh://USERNAME@qstopmotion.hg.sourceforge.net/hgroot/qstopmotion/qstopmotion
* A new directory 'qstopmotion' will be created containing the the last version of the sources

2.2 Create the Makefile
* Start the CMake GUI
* Select the file CMakeLists.txt in the qstopmotion directory
* Select xxx as target platform
* Press the 'generate' button

2.3 Build
* Call "make"
* Locking for error and warning messages

3. Installation
* Get super user permissions
* Call "make install"

4. Starting qStopMotion
* Select the video source
* Have fun.

5. Packaging

5.1 Creating source package

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

5.2.1 Download and install ???
* Download installer from ....
* Install with default options.

5.2.2 Create packaging script for qStopMotion
* ...
* ...

5.2.3 Create the package for qStopMotion
* su
* make package
* ...

5.2.4 Test the new package
* ...
* ...

5.3 Creating package for DEB

5.3.1 Download and install ???
* Download installer from ....
* Install with default options.

5.3.2 Create packaging script for qStopMotion
* ...
* ...

5.3.3 Create the package for qStopMotion
* ...
* ...

5.3.4 Test the new package
* ...
* ...

6. Create source documentation

5.1 Download and install Doxygen
* Download installer from
