Building qStopMotion using MS Windows SDK
Last Change: 2013-02-28

1. Preparation of the development environment
================================================================================

1.1 Install the MS Windows SDK
--------------------------------------------------------------------------------

* Download the "Windows SDK for Windows Server 2008 and .NET Framework 3.5"
* This package including the Visual Studio 2008 command line tools and the
  debugging tools for windows.
* Install in the default installation directory
  (C:\Program Files\Microsoft SDKs\Windows\v6.1).

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.6.3 for Windows 32-bit (VS 2008) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.6.3\bin\).

1.3 Gstreamer
--------------------------------------------------------------------------------

* Download Gstreamer GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-GPL-x86.msi
* Install in the Directory C:\Tools\OSSBuild\GStreamer\v0.10.6\...
* Qt and the Visual Studio tools have problems with spaces in the path name in
  the linking step.
* Download Gstreamer SDK GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-SDK-GPL-x86.msi
* Install Gstreamer SDK GPL

1.4 CMake 2.8.7 or higher
--------------------------------------------------------------------------------

* Download CMake from "http://www.cmake.org/cmake/resources/software.html"
* Install in the default installation directory.
* Add CMake to the system PATH for all users.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "c:\Program Files\Microsoft Visual Studio 8.0\VC\vcvarsall.bat"
    rem Start the CMake GUI
    "C:\Program Files\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 Mercurial client (Version >= 1.4.1)
--------------------------------------------------------------------------------

* Download TortoiseHG from "http://tortoisehg.bitbucket.org/download/index.html"
* Install in the default installation directory.

1.6 NSIS 2.46
--------------------------------------------------------------------------------

* Download NSIS from "http://nsis.sourceforge.net/Main_Page"
* Install in the default installation directory.

2. Building qStopMotion
================================================================================

2.1 Get the last version of the sources
--------------------------------------------------------------------------------

There are two possibilities, to get the source code:

2.1.1 Download the sources as a anonymous user
* Download the latest version of the sources as a tar ball from
  http://qstopmotion.hg.sourceforge.net:8000/hgroot/qstopmotion/qstopmotion
* Extract the tar ball

2.1.2 Use the Mercurial SCM as a registered developer
* Registering as a developer on qstopmotion.sourceforge.net
* Create a new directory for the work on qStopMotion
* Create a subdirectory qstopmotion and move to it
* Move to the new directory
* Start the TortoiseHG GUI
* Select 'File - Clone Repository ...'
* Source:
  ssh://USERNAME@qstopmotion.hg.sourceforge.net/hgroot/qstopmotion/qstopmotion
* Destination: your working directory.
* Press the 'Clone' button and add the password.
* The directory now containing the the last version of the sources

2.2 Create the NMake makefile
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

2.3 Build the application
--------------------------------------------------------------------------------

* Start a Visual Studio command prompt.
* Move to the new created destination directory.
* call "nmake".
* Locking for error and warning messages.

3. Installation
================================================================================

* Start a Visual Studio command prompt with administration rights.
* call "nmake install".

4. Starting qStopMotion
================================================================================

* Select the video source.
* Have fun.

5. Creating installer for Windows
================================================================================

5.1 Create the installer for qStopMotion
--------------------------------------------------------------------------------

* Start a Visual Studio command prompt
* Call "nmake package"
* A new installer with the name 'qstopmotion-x.y.z-win32.exe' will be created
  in the destination directory

5.2 Test the new installer
--------------------------------------------------------------------------------

* Copy the new installer to a temporary directory
* Start the installer
* Select the installation directory

6. Create source documentation
================================================================================

6.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
