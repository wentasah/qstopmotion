Building qStopMotion using Qt-Creator IDE on Windows with the Windows SDK as host
Last Change: 2019-06-19

1. Preparation of the development environment
================================================================================

1.1 Install the Tools for Visual Studio 2019
--------------------------------------------------------------------------------

* Download the "Tools for Visual Studio 2019" for free without registration.
* This package including ONLY the Visual Studio 2019 command line tools.
* This package including the Windows 10 SDK
* Install in the default installation directory
  (C:\Program Files\Microsoft Visual Studio).
* Add the binary directory to the path variable ???
  (C:\Program Files\Microsoft Visual Studio\2019\BuildTools\VC\...).

1.2 Install the Windows SDK
--------------------------------------------------------------------------------

* Download the "Windows Software Development Kit for Windows 8.1"
* This package including the debugging tools for windows.
* Install in the default installation directory
  (C:\Program Files\Windows Kits\v8.1).

1.3 CMake
--------------------------------------------------------------------------------

* Download CMake >= 3.11.2 from "https://cmake.org/download/"
* Install in the default installation directory.
* Select "Add CMake to the system PATH for all users" in the installation dialog.
* Create a start script for CMake (C:\Tools\cmakestart.bat)
    rem Set the Visual Studion 10 environment variables
    call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
    rem Start CMake
    "C:\Program Files\CMake\bin\cmake-gui.exe"
* Create a shortcut to the start script.

1.4 Mercurial SCM
--------------------------------------------------------------------------------

* Download Mercurial or TortoiseHG with Mercurial from "http://mercurial.selenic.com/downloads"
* Install in the default installation directory.

1.5 Subversion
--------------------------------------------------------------------------------

* Download TortoiseSVN from "https://tortoisesvn.net/downloads.html"
* Install in the default installation directory.
* Add the installation of the "command line client tools" in the installer dialog


1.6 Qt5 for Windows and Visual Studio
--------------------------------------------------------------------------------

* Download Qt runtime >= 5.5.x for Windows 64-bit (VS 2013) from www.qt.io/download
* Install in the Directory C:\Tools\Qt\...
* Qt need write permissions to this directory during compilation and linking.
* Add the binary directory to the path variable (e.g. C:\Tools\Qt\Qt5.5.1\bin\).
* Set the environment variable 'Qt5_DIR' to 'C:\Tools\Qt\Qt5.5.1\5.5\msvc2013_64\'

1.7 Qt-Creator for Windows
--------------------------------------------------------------------------------

* Download Qt Creator >= 4.2.x for Windows (not the full SDK) from
  www.qt.io/download
* Install in the Directory C:\Tools\Qt\...
* The installation of the MinGW runtime development tools is not necessary.

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

3.1 Build
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

3.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator.
* Build qStopMotion.
* Start qStopMotion in debugging mode.
