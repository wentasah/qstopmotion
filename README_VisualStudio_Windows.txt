Building qStopMotion using MS Visual Studio
Last Change: 2016-05-10

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

1.3 Qt5 for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt runtime 5.5.x for Windows 64-bit (VS 2013) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\Qt5.5.1\bin\).
* Set the environment variable 'Qt5_DIR' to 'C:\Tools\Qt\Qt5.5.1\5.5\msvc2013_64\'

1.4 CMake
--------------------------------------------------------------------------------

* Download CMake >= 3.3.2 (32bit) from "http://www.cmake.org/cmake/resources/software.html"
* Install in the default installation directory.
* Add CMake to the system PATH for all users.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start CMake
    "C:\Program Files (x86)\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 Mercurial SCM (Developers only)
--------------------------------------------------------------------------------

* Download Mercurial or TortoiseHG with Mercurial from "http://mercurial.selenic.com/downloads"
* Install in the default installation directory.

1.6 NSIS 2.46
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

3.1 Add the ffmpeg encoder to the sources directory
--------------------------------------------------------------------------------

* Download the latest static version from http://ffmpeg.zeranoe.com/builds/
* Unpack the 7z file to the qstopmotion directory
* Rename the ffmpeg directory to 'ffmpeg'

3.2 Add the qtruntime to the sources directory
--------------------------------------------------------------------------------

* Create a new directory qtruntime in the source directory
* Copy QtCore4.dll, QtGui4.dll and QtXml.dll from Qt installation bin directory
  to the new qtruntime directory
* Copy QtCored4.dll, QtGuid4.dll and QtXmld.dll from Qt installation bin directory
  to the new qtruntime directory
* Copy the directory imageformats from Qt installation plugins directory
  to the new qtruntime directory

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
