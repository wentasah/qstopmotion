Building qStopMotion using Unix makefile on Linux
Last Change: 2014-12-16

1. Preparation of the development environment
================================================================================

1.1 Install necessary libraries with the package manager
--------------------------------------------------------------------------------

* g++ (Version >= 4.4.3)
* qt4 libraries (32 or 64 bit, Version >= 4.8.2)
  - libqt4, libqt4-dev, libqt4-dbg (qt4-core, qt4-gui, qt4-xml)
  - qt4-examples / qt4-demos
  - qt4-dev-tools
  - qt4-qmake
  - qt4-help, qt4-doc
* CMake (Version >= 2.8.7)
  - The version 2.8.7 ist the first that can create working Debian packages
  - If the package in the package manager is an older version install CMake
    from the source
* Mercurial Client (Version >= 2.2.2)
* gstreamer (Version >= 0.10.36)
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
* ffmpeg (Version >= 0.8.6)
* libxml2, libxml2-dev, libxml-dbg
* libv4l, libv4l-devel, v4l-utils, v4l-utils-dev
* libgphoto2, libgphoto2-devel

2. Access to the sources of qStopMotion
================================================================================

2.1 Download the sources (last release of qStopMotion)
--------------------------------------------------------------------------------

* Downoad qstopmotion-<version>-Source.zip or qstopmotion-<version>-Source.tar.Z
  from http://sourceforge.net/projects/qstopmotion/files/

2.2 Anonymous read only access to the repository (latest developer version)
--------------------------------------------------------------------------------

* hg clone http://hg.code.sf.net/p/qstopmotion/code qstopmotion

2.3 Developer access to the repository (read and write access)
--------------------------------------------------------------------------------

2.3.1 Create a Source Forge account
* Create a Source Forge account on https://sourceforge.net/user/registration
* Add your account to the qStopMotion developers on http://sourceforge.net/projects/qstopmotion

2.3.2 Create a .hgrc file
* Create a .hgrc file in your home directory
* Content of the file:
    [ui]
    username = Your Name <your eMail address>
    [auth]
    qstopmotion.prefix = https://hg.code.sf.net/p/qstopmotion/code
    qstopmotion.username = <your Source Forge user name>
    qstopmotion.password = <your Source Forge password>

2.3.3 Create a local repository
* hg clone https://hg.code.sf.net/p/qstopmotion/code qstopmotion

3. Building qStopMotion
================================================================================

3.1 Create the Makefile
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

3.2 Build
--------------------------------------------------------------------------------

* Move to the qstopmotion-build directory.
* Call "make"
* Locking for error and warning messages

4. Installation
================================================================================

* Get super user permissions
* Call "make install"

5. Starting qStopMotion
================================================================================

* Select the video source
* Have fun.

6. Packaging
================================================================================

6.1 Creating package for RPM
--------------------------------------------------------------------------------

6.1.1 Download and install ???
* Download installer from ....
* Install with default options.

6.1.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Add Entry of type string: 'CPACK_GENERATOR' with the value 'RPM'
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

6.1.3 Create the package for qStopMotion
* Open a command line
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.rpm' will be created
  in the directory

6.1.4 Test the new package
* Open the new created package with the software installation tool
* Install qStopMotion
* You will find the an new icon in the applications-multimedia menu

6.2 Creating package for DEB
--------------------------------------------------------------------------------

6.2.1 Download and install ???
* Download installer from ....
* Install with default options.

6.2.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Add Entry of type string: 'CPACK_GENERATOR' with the value 'DEB'
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

6.2.3 Create the package for qStopMotion
* Open a command line and be a super user with 'su'
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.deb' will be created in
  the directory

6.2.4 Test the new package
* Open the new created package with the software installation tool
* Install qStopMotion
* You will find the an new icon in the applications-multimedia menu

6.3 Creating package for Linux X11
--------------------------------------------------------------------------------

6.3.1 Download and install ???
* Download installer from ....
* Install with default options.

6.3.2 Create packaging script for qStopMotion
* Start the CMake GUI
* Select the qstopmotion directory as source locaton
* Select a empthy directory as the build directory
* Press the 'Configure' button and select 'Unix Makefile' as generator
* Press the 'Configure' button again
* Press the 'Generate' button

6.3.3 Create the package for qStopMotion
* Open a command line and be a super user with 'su'
* Move to the build directory
* make package
* A new package with the name 'qstopmotion-x.x.x-Linux.sh' will be created in
  the directory

6.3.4 Test the new package
* Call the new shell file
* Install qStopMotion

6.4 Creating source package
--------------------------------------------------------------------------------

6.4.1 Download and install ???
* Download installer from ....
* Install with default options.

6.4.2 Create packaging script for qStopMotion
* ...
* ...

6.4.3 Create the package for qStopMotion
* make package_source
* ...

7. Create source documentation
================================================================================

7.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from
