Building qStopMotion using Qt-Creator IDE on Windows with the Windows SDK as host
Last Change: 2014-04-15

1. Preparation of the development environment
================================================================================

1.1 Install the Windows SDK
--------------------------------------------------------------------------------

* Download the "Windows SDK for Windows 7 and .NET Framework 4"
  - If you use a 32bit Windows system download GRMSDK_EN_DVD or the online installer
  - If you use a 64bit Windows system download GRMSDKX_EN_DVD or the online installer
* This package including the Visual Studio 2010 command line tools and the
  debugging tools for windows.
* Install in the default installation directory
  (C:\Program Files\Microsoft SDKs\Windows\v7.1).

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.8.4 for Windows 32-bit (VS 2010) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.8.4\bin\).

1.3 Download and install Qt-Creator for Windows
--------------------------------------------------------------------------------

* Download Qt Creator 2.8.1 for Windows 32-bit (not the full SDK) from
  qt-project.org/downloads
* Install in the Directory C:\Tools\Qt\...
* The installation of the MinGW runtime development tools is not necessary.

1.4 CMake 2.8.12
--------------------------------------------------------------------------------

* Download CMake 2.8.12.x from "http://www.cmake.org/cmake/resources/software.html"
* Add CMake to the system PATH for all users.
* Install in the default installation directory.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start CMake
    "C:\Program Files (x86)\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 NSIS 2.46
--------------------------------------------------------------------------------

* Download NSIS from "http://nsis.sourceforge.net/Main_Page"
* Install in the default installation directory.

1.6 Mercurial SCM (Developers only)
--------------------------------------------------------------------------------

* Download Mercurial or TortoiseHG with Mercurial from "http://mercurial.selenic.com/downloads"
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
  to the new qtruntime directory.
* Copy the directory imageformats from Qt installation plugins directory
  to the new qtruntime directory.

3.3 Build
--------------------------------------------------------------------------------

* Start Qt Creator.
* Open the 'Options...' dialog in the 'Tools' menu and select 'Build & Run'.
* In the tab 'Qt Versions' add a manual entry for the installed Qt version and
  press the 'Apply' button.
* In the tab 'Kits' select the entry 'Manual - Desktop (default)' and  select the
  'Microsoft Visual C++ Compiler 10.0 (x86)' and the installed Qt version.
* Open the CMakeLists.txt file as new project.
* In the dialog of the CMake-Assistent press the 'Run CMake' button.
* Ignore the CMake Warnings 'system runtime library file does not exist' messages.
* If the 'Finisch' button is enabled press this button.
* In the 'Build' menu select 'Build Project qStopMotion'
* Locking for error and warning messages.

3.4 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator.
* Build qStopMotion.
* Start qStopMotion in debugging mode.
