Building qStopMotion using MS Visual Studio 2010
Last Change: 2012/01/09

1. Preparation of the development environment
================================================================================

1.1 Install MS Visual Studio 2010 Express/Professional
--------------------------------------------------------------------------------

* Install in the default installation directory.

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.6.2 for Windows (VS 2008) from qt.nokia.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.6.2\bin\).

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

1.4 CMake 2.8.x
--------------------------------------------------------------------------------

* Download CMake from "http://www.cmake.org/cmake/resources/software.html"
* Add CMake to the system PATH for all users.
* Install in the default installation directory.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "c:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start the CMake GUI
    "C:\Program Files\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 Mercurial client (Version >= 1.4.1)
--------------------------------------------------------------------------------

* Download TortoiseHG from "http://tortoisehg.bitbucket.org/download/index.html".
* Install in the default installation directory.
* Download VisualHG, the Mercurial extension for Visual Studio, from
  "http://visualhg.codeplex.com/".
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
* Move to the new directory
* Start the TortoiseHG GUI
* hg clone ssh://USERNAME@qstopmotion.hg.sourceforge.net/hgroot/qstopmotion/qstopmotion
* A new directory 'qstopmotion' will be created containing the the last version
  of the sources

2.2 Create the Visual Studio Solution
--------------------------------------------------------------------------------

* Start the CMake GUI using the shortcut to the start script.
* Select the directory with the cmakelist.txt file
* Select the destination directory
* Press the configure button
* Select the generator "Visual Studio 10"
* Press the generate button

2.3 Build the application
--------------------------------------------------------------------------------

* Start MS Visual Studio
* Open the new created solution file (qstopmotion.sln)
* Activate the project "qstopmotion" as the active project
* Build | Build Solution
* Locking for error and warning messages

3. Installation
================================================================================

* Start MS Visual Studio with a administrator account
  (The problem is to create the dircorys in the windows program files directory)
* Open the solution file (qstopmotion.sln)
* Build the project "INSTALL"

4. Starting qStopMotion
================================================================================

* Select the video source
* Have fun.

5. Create source documentation
================================================================================

5.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
