Building qStopMotion using Qt-Creator IDE on Linux
Last Change: 2018-01-17

1. Preparation of the development environment
================================================================================

1.1 C++ Compiler
--------------------------------------------------------------------------------

* g++ (Version >= 4.9.2)
* pkg-config
* lsb-release / redhat-lsb-core

1.2 CMake
--------------------------------------------------------------------------------

* CMake (Version >= 3.0.2)
  - The version 2.8.7 ist the first that can create working Debian packages
  - If the package in the package manager is an older version install CMake
    from the source

1.3 Mercurial SCM (Developers only)
--------------------------------------------------------------------------------

* Mercurial Client (Version >= 3.1.2)
* or TortoiseHg as a graphical frontend

1.4 Tools
--------------------------------------------------------------------------------

* ffmpeg (Version >= 0.8.6) to generate movie files from the pictures

1.5 Libraries
--------------------------------------------------------------------------------
* libxml2, libxml2-dev, libxml-dbg
* libv4l, libv4l-devel, v4l-utils, v4l-utils-dev
* libgphoto2, libgphoto2-devel
* libqwt-qt5-6, libqwt-qt5-dev

1.6 Qt5 for Linux
--------------------------------------------------------------------------------

* qt5 libraries (64 bit, Version >= 5.3.2)
* qt5-default
* libqt5, libqt5-dev, libqt5-dbg (qt5-core, qt5-gui, qt5-xml)
* qt5-image-formats-plugins
* qtmultimedia5-examples
* qt5-examples / qt5-demos
* qt5-dev-tools
* qt5-qmake
* qt5-help, qt5-doc

1.7 Qt Creator for Linux
--------------------------------------------------------------------------------

* Qt Creator (Version >= 4.2.x)

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
