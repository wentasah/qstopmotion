Building qStopMotion using MS Windows SDK
Last Change: 2014-04-15

1. Preparation of the development environment
================================================================================

1.1 Install the MS Windows SDK
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

* Download Qt libraries 4.8.2 for Windows 32-bit (VS 2010) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.8.2\bin\).

1.3 CMake 2.8.12
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

1.4 Mercurial SCM (Developers only)
--------------------------------------------------------------------------------

* Download Mercurial or TortoiseHG with Mercurial from "http://mercurial.selenic.com/downloads"
* Install in the default installation directory.

1.5 NSIS 2.46
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

3.3 Create the NMake makefile
--------------------------------------------------------------------------------

* Start the CMake GUI using the shortcut to the start script.
* Select the directory with the cmakelist.txt file as the source code directory.
* Select a separate directory as the destination directory.
* Set the 'Grouped' and 'Advanced' checkboxes.
* To build the release version add a new string entry with the name
  'CMAKE_BUILD_TYPE' and the value 'Release'. The default build type is 'Debug'.
* Press the configure button.
* Select the generator "NMake Makefiles".
* Select the entry CPACK - CPACK_BINARY_NSIS.
* Select the entry CPACK - CPACK_BINARY_ZIP.
* Select the entry CPACK - CPACK_SOURCE_ZIP.
* Press the configure button again.
* Press the generate button.

3.4 Build the application
--------------------------------------------------------------------------------

* Start the "Qt 4.8.4 Command Prompt".
* Move to the new created destination directory.
* Call "nmake".
* Locking for error and warning messages.

4. Installation
================================================================================

* Start a Visual Studio command prompt with administration rights.
* call "nmake install".

5. Starting qStopMotion
================================================================================

* Select the video source.
* Have fun.

6. Creating packages for Windows
================================================================================

6.1 Create the installer for qStopMotion
--------------------------------------------------------------------------------

* Start the "Qt 4.8.4 Command Prompt".
* Move to the new created destination directory.
* Call "nmake package".
* The Windows installer with the name 'qstopmotion-x.y.z-win32.exe' will be
  created in the destination directory.
* The portable application with the name 'qstopmotion-x.y.z-win32.zip' will be
  created in the destination directory.

6.2 Test the installer
--------------------------------------------------------------------------------

* Copy the new installer to a temporary directory.
* Start the installer.
* Select the installation directory.
* Start qStopMotion in the Windows 'Start' menu.

6.3 Test the portable application
--------------------------------------------------------------------------------

* Unpack the zip file of the portable application to a temporary directory.
* Move to the new directory and go to the 'bin' subdirectory.
* Start the file 'qstopmotion.exe'.

6.4 Create the source package
--------------------------------------------------------------------------------

* Start the "Qt 4.8.4 Command Prompt".
* Move to the new created destination directory.
* Call "nmake package_source"
* A new zip package with the name 'qstopmotion-x.y.z-Source.zip' will be created
  in the destination directory

7. Create source documentation
================================================================================

7.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
