Building qStopMotion using MS Visual Studio 2010 / 2008
Last Change: 2011/07/07

1. Preparation of the development environment

1.1 Install MS Visual Studio 2010 Express/Professional

1.2 Qt for Windows and Visual Studio
* Download Qt libraries 4.6.3 for Windows (VS 2008) from qt.nokia.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.6.3\bin\).

1.3 Gstreamer
* Download Gstreamer GPL from code.google.com/p/ossbuild/:  GStreamer-WinBuilds-GPL-x86.msi
* Install in the Directory C:\Tools\OSSBuild\GStreamer\v0.10.6\...
* Qt and the Visual Studio tools have problems with spaces in the path name in the linking step.
* Download Gstreamer SDK GPL from code.google.com/p/ossbuild/: GStreamer-WinBuilds-SDK-GPL-x86.msi
* Install Gstreamer SDK GPL

1.4 CMake 2.8.x
* Download CMake from "http://www.cmake.org/cmake/resources/software.html"
* Add CMake to the system PATH for all users.
* Install in the default installation directory.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "c:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start the CMake GUI
    "C:\Program Files\CMake 2.8\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.5 NSIS 2.46
* Download NSIS from "http://nsis.sourceforge.net/Main_Page"
* Install in the default installation directory.

2. Building qStopMotion

2.1 Create the Visual Studio Solution
* Start the CMake GUI using the shortcut to the start script.
* Select the directory with the cmakelist.txt file
* Select the destination directory
* Press the configure button
* Select the generator "Visual Studio 10"
* Press the generate button

2.2 Build the application
* Start MS Visual Studio
* Open the new created solution file (qstopmotion.sln)
* Activate the project "qstopmotion" as the active project
* Build | Build Solution
* Locking for error and warning messages

3. Installation
* Start MS Visual Studio with a administrator account
  (The problem is to create the dircorys in the windows program files directory)
* Open the solution file (qstopmotion.sln)
* Build the project "INSTALL"

4. Starting qStopMotion
* Select the video source
* Have fun.

5. Create source documentation

5.1 Download and install Doxygen
* Download installer from ...
