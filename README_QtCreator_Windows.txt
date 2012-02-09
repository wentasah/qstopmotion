Building qStopMotion using Qt-Creator IDE on Windows with the Visual Studio as host
Last Change: 2012/01/09

1. Preparation of the development environment
================================================================================

1.1 Install MS Visual Studio tools
--------------------------------------------------------------------------------

There are two possibilities for the MS Visual Studio tools:
* Install the IDE and the debugging tools or
* Install the Windows SDK

1.1.1 Install the IDE and the Debugging Tools

1.1.1.1 Install MS Visual Studio 2010 (Express or Professional)
* Qt creator uses only the command line tools.
* Install in the default installation directory.

1.1.1.2 Download and install the Debugging tools for Windows
* Download the latest version of "Debugging Tools for Windows" from the
  Microsoft Web site
  "http://msdn.microsoft.com/en-us/windows/hardware/gg463016.aspx"
* Install in the default installation directory.

1.1.2 Install the Windows SDK
* Download the Windows SDK for .NET 3.5 including the Visual Studio 2008 command
  line tools and the debugging tools for windows.
* Install in the default installation directory.

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.6.2 for Windows (VS 2008) from qt.nokia.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.6.2\bin).

1.3 Download and install Qt-Creator for Windows
--------------------------------------------------------------------------------

* Download Qt Creator 2.1.0 for Windows (not the full SDK) from
  qt.nokia.com/downloads
* Install in the Directory C:\Tools\Qt\...
* The installation of the MinGW runtime development tools is not necessary.
* Create a start script for the Qt-Creator (C:\Tools\qtcreatorstart.bat)
    rem Set the Visual Studio 10 environment variables
    call "c:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    rem Start the Qt-Creator
    "C:\Tools\Qt\qtcreator-2.1.0\bin\qtcreator.exe"
* Create a shortcut to the start script.

1.4 Gstreamer
--------------------------------------------------------------------------------

* Download Gstreamer GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-GPL-x86.msi
* Install in the Directory C:\Tools\OSSBuild\GStreamer\v0.10.6\...
* Qt and the Visual Studio tools have problems with spaces in the path name in
  the linking step.
* Download Gstreamer SDK GPL from code.google.com/p/ossbuild/:
  GStreamer-WinBuilds-SDK-GPL-x86.msi
* Install Gstreamer SDK GPL

1.5 CMake 2.8.7 or higher
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

1.6 NSIS 2.46
--------------------------------------------------------------------------------

* Download NSIS from "http://nsis.sourceforge.net/Main_Page"
* Install in the default installation directory.

2. Building qStopMotion
================================================================================

2.1 Build
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut to the start script.
* Open the CMakeLists.txt file as new project.
* Build | Build Solution
* Locking for error and warning messages

2.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut to the start script.
* Open Options | Settings | Debugger
* On the Cdb tab enable the CDB. Normaly the path to the debugger will find
  automatically.
* Start qStopMotion in debugging mode.

3. Create source documentation
================================================================================

3.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
