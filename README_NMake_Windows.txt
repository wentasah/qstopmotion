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
* Create a shortcut to "Windows SDK 7.1 Command Prompt".
  - Open the properties of the shortcut.
  - Add the following options to the command line: " /Release /x86".

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.8.2 for Windows 32-bit (VS 2010) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.8.2\bin\).

1.3 GStreamer
--------------------------------------------------------------------------------

* Download GStreamer GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-GPL-x86-Beta04-0.10.7.msi
* Install in the default installation directory.
* Download GStreamer SDK GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-SDK-GPL-x86-Beta04-0.10.7.msi
* Install GGtreamer SDK GPL
* Note: The Windows version of GStreamer from the GStreamer project
  (http://gstreamer.freedesktop.org/) are not supported!

1.4 CMake 2.8.12 or newer
--------------------------------------------------------------------------------

* Download CMake from "http://www.cmake.org/cmake/resources/software.html"
* Add CMake to the system PATH for all users.
* Install in the default installation directory.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    rem Usage : Setenv [/Debug | /Release][/x86 | /x64 | /ia64 ][/vista | /xp | /2003 | /2008 | /win7][-h | /?]
    call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /Release /x86
    rem Start the CMake
    "C:\Program Files (x86)\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 Mercurial client (Version >= 1.4.1)
--------------------------------------------------------------------------------

* Download TortoiseHG from "http://tortoisehg.bitbucket.org/download/index.html"
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

3.1 Create the NMake makefile
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
* Press the configure button again.
* Press the generate button.

3.2 Build the application
--------------------------------------------------------------------------------

* Start the modifyed shortcut to the "windows SDK 7.1 Command Prompt".
* Move to the new created destination directory.
* call "nmake".
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

* Start the modifyed shortcut to the "windows SDK 7.1 Command Prompt".
* Call "nmake package"
* A new installer with the name 'qstopmotion-x.y.z-win32.exe' will be created
  in the destination directory

6.2 Test the new installer
--------------------------------------------------------------------------------

* Copy the new installer to a temporary directory
* Start the installer
* Select the installation directory

6.3 Create the source package
--------------------------------------------------------------------------------

* Start the modifyed shortcut to the "windows SDK 7.1 Command Prompt".
* Call "nmake package_source"
* A new zip package with the name 'qstopmotion-x.y.z-Source.zip' will be created
  in the destination directory

7. Create source documentation
================================================================================

7.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
