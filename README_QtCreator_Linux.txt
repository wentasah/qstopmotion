Building qStopMotion using Qt-Creator IDE on Linux
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

1.2 Qt Creator for Linux
--------------------------------------------------------------------------------

* Use the Qt Creator from the repositories of your distribution or
* Download Qt Creator for Linux (32 or 64 bit, Version >= 2.5.0) from
  qt-project.org/downloads.
* Set execute permissions to the binary installer.
* Start the installer with root permissions.
* Install the Qt Creator to /opt/qtcreator-2.5.0/
* Create a shortcut in the panel.

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

3.1 Build
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Release
* Open the CMakeLists.txt file as new project.
* Build | Build Project qStopMotion
* Locking for error and warning messages

3.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Debug
* Build | Build Project qStopMotion
* Locking for error and warning messages
* Open Options | Settings | Debugger
* On the Cdb tab enable the CDB. Normaly the path to the debugger will find
  automatically.
* Start qStopMotion in debugging mode.

4. Create source documentation
================================================================================

4.1 Install Doxygen with the package manager.
--------------------------------------------------------------------------------

* ...
