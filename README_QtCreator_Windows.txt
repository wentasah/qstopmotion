Building qStopMotion using Qt-Creator IDE on Windows with the Windows SDK as host
Last Change: 2013-03-06

1. Preparation of the development environment
================================================================================

1.1 Install the Windows SDK
--------------------------------------------------------------------------------

* Download the "Windows SDK for Windows 7 and .NET Framework 3.5 Service Pack 1"
* This package including the Visual Studio 2008 command line tools and the
  debugging tools for windows.
* Install in the default installation directory
  (C:\Program Files\Microsoft SDKs\Windows\v6.1).

1.2 Qt for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt libraries 4.6.3 for Windows 32-bit (VS 2008) from qt-project.com/downloads
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\4.6.3\bin).

1.3 Download and install Qt-Creator for Windows
--------------------------------------------------------------------------------

* Download Qt Creator 2.1.0 for Windows 32-bit (not the full SDK) from
  qt-project.org/downloads
* Install in the Directory C:\Tools\Qt\...
* The installation of the MinGW runtime development tools is not necessary.
* Create a start script for the Qt-Creator (C:\Tools\qtcreatorstart.bat)
    rem Set the Windows SDK environment variables
    call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"
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
    call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"
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
* In the dialog of the CMake-Assistent press the 'Run CMake' button.
* Ignore the CMake Warnings 'system runtime library file does not exist' messages.
* If the 'Finisch' button is enabled press this button.
* Build | Build Solution
* Locking for error and warning messages

2.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut to the start script.
* In the dialog of the CMake-Assistent press the 'Run CMake' button.
* Ignore the CMake Warnings 'system runtime library file does not exist' messages.
* If the 'Finisch' button is enabled press this button.
* Open Options | Settings | Debugger
* On the Cdb tab enable the CDB. Normaly the path to the debugger will find
  automatically.
* Start qStopMotion in debugging mode.

3. Create source documentation
================================================================================

3.1 Download and install Doxygen
--------------------------------------------------------------------------------

* Download installer from ...
