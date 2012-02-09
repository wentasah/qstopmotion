Building qStopMotion using Qt-Creator IDE on Linux
Last Change: 2012/01/09

1. Preparation of the development environment
================================================================================

1.1 Install necessary libraries with the package manager
--------------------------------------------------------------------------------

* qt libraries (32 or 64 bit, Version >= 4.6.2)
  - qt
  - qt-devel
  - qt-examples
  - qt-doc
* CMake (Version >= 2.8.7)
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

1.2 Qt Creator for Linux
--------------------------------------------------------------------------------

* Use the Qt Creator from the repositories of your distribution or
* Download Qt Creator for Linux (32 or 64 bit, Version >= 2.1.0) from
  qt.nokia.com/downloads.
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
