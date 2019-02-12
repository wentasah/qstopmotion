Building qStopMotion using MS Visual Studio
Last Change: 2018-01-17

1. Preparation of the development environment
================================================================================

1.1 Install the Visual Studio 2013
--------------------------------------------------------------------------------

* Download the "Visual Studio 2013 Community Edition"
* This package including the Visual Studio 2010 command line tools.
* Install in the default installation directory
  (C:\Program Files\Microsoft Visual Studio 12.0).
  - Switch off all optional features
  - Start the IDE and search for updates
* Add the binary directory to the path variable
  (C:\Program Files\Microsoft Visual Studio 12.0\VC\bin\).

1.2 Install the Windows SDK
--------------------------------------------------------------------------------

* Download the "Windows Software Development Kit for Windows 8.1"
* This package including the debugging tools for windows.
* Install in the default installation directory
  (C:\Program Files\Windows Kits\v8.1).

1.3 CMake
--------------------------------------------------------------------------------

* Download CMake >= 3.4.x from "http://www.cmake.org/cmake/resources/software.html"
* Install in the default installation directory.
* Add CMake to the system PATH for all users.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start CMake
    "C:\Program Files (x86)\CMake\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.4 Mercurial SCM (Developers only)
--------------------------------------------------------------------------------

* Download Mercurial or TortoiseHG with Mercurial from "http://mercurial.selenic.com/downloads"
* Install in the default installation directory.

1.5 Qt5 for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt runtime >= 5.5.x for Windows 64-bit (VS 2013) from www.qt.io/download
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\Qt5.5.1\bin\).
* Set the environment variable 'Qt5_DIR' to 'C:\Tools\Qt\Qt5.5.1\5.5\msvc2013_64\'

1.6 Qwt
--------------------------------------------------------------------------------

* Download Qwt >= 6.1.3 from "https://sourceforge.net/projects/qwt/files/"
* Unpack the file to a temporary directory
* Configure the the installation directory in the file qwtconfig.pri
  (QWT_INSTALL_PREFIX = C:/Tools/Qwt-$$QWT_VERSION)
* Disable not necessary modules in the file qwtconfig.pri
  (Plot, SVG, OpenGL, Designer)
* Open a command prompt and change to the temporary qwt directory
* Create the make file
  cmake qwt.pro
* Buiild the library
  nmake
* Install the library
  nmake install
* Set the environment variables
  QWT_INSTALL_DIR = C:\Tools\Qwt-6.x.y\include
  QWT_LIBRARY_DIR = C:\Tools\Qwt-6.x.y\lib

1.7 NSIS 2.46
--------------------------------------------------------------------------------

* Download NSIS from "http://nsis.sourceforge.net/Main_Page"
* Install in the default installation directory.

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

3.1 Add the qwt runtime to the source directory
--------------------------------------------------------------------------------

* Create a new directory 'qwt64bit' in the source directory
* Create a new directory 'release'
  - Copy qwt.dll from Qwt installation lib directory to the 'release' directory
* Create a new directory 'debug'
  - Copy qwtd.dll and qwtd.pdb from Qwt installation lib directory to
    the 'debug' directory

3.2 Add the ffmpeg encoder to the sources directory
--------------------------------------------------------------------------------

* Download the latest static version from http://ffmpeg.zeranoe.com/builds/
* Unpack the 7z file to the qstopmotion directory
* Rename the ffmpeg directory to 'ffmpeg64bit'

3.3 Create the Visual Studio Solution
--------------------------------------------------------------------------------

* Start the CMake GUI using the shortcut to the start script.
* Select the directory with the cmakelist.txt file
* Select the destination directory
* Press the configure button
* Select the generator "Visual Studio 12 2013 Win64"
* Press the generate button

3.4 Build the application
--------------------------------------------------------------------------------

* Start MS Visual Studio
* Open the new created solution file (qstopmotion.sln)
* Activate the project "qstopmotion" as the active project
* Build | Build Solution
* Locking for error and warning messages

4. Installation
================================================================================

* Start MS Visual Studio with a administrator account
  (The problem is to create the dircorys in the windows program files directory)
* Open the solution file (qstopmotion.sln)
* Build the project "INSTALL"

5. Starting qStopMotion
================================================================================

* Select the video source
* Have fun.
