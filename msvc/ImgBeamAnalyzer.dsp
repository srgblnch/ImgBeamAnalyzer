# Microsoft Developer Studio Project File - Name="ImgBeamAnalyzer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ImgBeamAnalyzer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImgBeamAnalyzer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImgBeamAnalyzer.mak" CFG="ImgBeamAnalyzer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImgBeamAnalyzer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ImgBeamAnalyzer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImgBeamAnalyzer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Z7 /Ox /Gy /X /I "C:\soleil_root\sw-support\adtb\include" /I "C:\soleil_root\sw-support\OpenCV\include" /I "D:\dev\Libraries\ISL\include" /I "C:\PROGRA~1\Microsoft Visual Studio\VC98\INCLUDE" /I "..\src" /I "..\include" /I "$(SOLEIL_ROOT)\omniorb\include" /I "$(SOLEIL_ROOT)\tango\include" /I "$(SOLEIL_ROOT)\dev\include" /D "WIN32" /D "NDEBUG" /D "INTEL86" /D "STRICT" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /FR /FD /Zm1000 /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 $(SOLEIL_ROOT)\sw-support\adtb\lib\static\libADTB.lib $(SOLEIL_ROOT)\tango\lib\mix\log4tango.lib $(SOLEIL_ROOT)\tango\lib\mix\tango.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omniORB405_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omnithread30_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\COS405_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omniDynamic405_rt.lib kernel32.lib user32.lib gdi32.lib advapi32.lib ws2_32.lib comctl32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Release/ds_ImgBeamAnalyzer.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ImgBeamAnalyzer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /MD /W3 /GR /GX /Zi /Ox /Gy /X /I "$(SOLEIL_ROOT)\sw-support\adtb\include" /I "$(SOLEIL_ROOT)\sw-support\opencv\include" /I "D:\dev\Libraries\ISL\include" /I ".." /I "." /I "..\include" /I "..\src" /I "c:\soleil_root\omniorb\include" /I "c:\soleil_root\tango\include" /I "c:\soleil_root\dev\include" /I "C:\PROGRA~1\Microsoft Visual Studio\VC98\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEBUG" /D "INTEL86" /D "STRICT" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /FR /FD /Zm1000 /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 $(SOLEIL_ROOT)\sw-support\adtb\lib\static\libADTB.lib $(SOLEIL_ROOT)\sw-support\opencv\lib\libOpenCV_highgui.lib $(SOLEIL_ROOT)\sw-support\opencv\lib\libOpenCV_cv.lib $(SOLEIL_ROOT)\sw-support\opencv\lib\libOpenCV_cx.lib $(SOLEIL_ROOT)\tango\lib\mix\log4tango.lib $(SOLEIL_ROOT)\tango\lib\mix\tango.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omniORB405_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omnithread30_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\COS405_rt.lib $(SOLEIL_ROOT)\omniorb\lib\x86_win32\omniDynamic405_rt.lib kernel32.lib user32.lib gdi32.lib advapi32.lib ws2_32.lib comctl32.lib /nologo /pdb:none /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"Release/ds_ImgBeamAnalyzer.exe" /fixed:no

!ENDIF 

# Begin Target

# Name "ImgBeamAnalyzer - Win32 Release"
# Name "ImgBeamAnalyzer - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\BIAConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BIAConfig.h
# End Source File
# Begin Source File

SOURCE=..\src\BIAData.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BIAData.h
# End Source File
# Begin Source File

SOURCE=..\src\BIAData.i
# End Source File
# Begin Source File

SOURCE=..\src\BIAProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BIAProcessor.h
# End Source File
# Begin Source File

SOURCE=..\src\BIATask.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BIATask.h
# End Source File
# Begin Source File

SOURCE=..\src\BIATask.i
# End Source File
# Begin Source File

SOURCE=..\src\ClassFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzer.h
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzerClass.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzerClass.h
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzerStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ImgBeamAnalyzerVersion.h
# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# End Group
# End Target
# End Project
