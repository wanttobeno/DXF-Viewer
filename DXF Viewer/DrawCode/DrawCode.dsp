# Microsoft Developer Studio Project File - Name="DrawCode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DrawCode - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DrawCode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DrawCode.mak" CFG="DrawCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DrawCode - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DrawCode - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DrawCode - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "DrawCode - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "DrawCode - Win32 Release"
# Name "DrawCode - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DrwIO.cpp
# End Source File
# Begin Source File

SOURCE=.\MemDrw.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DrawingDef.h
# End Source File
# Begin Source File

SOURCE=.\DrwIO.h
# End Source File
# Begin Source File

SOURCE=.\DXFDef.h
# End Source File
# Begin Source File

SOURCE=.\MemDrw.h
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\View\Colors.h
# End Source File
# Begin Source File

SOURCE=.\View\DimView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\DimView.h
# End Source File
# Begin Source File

SOURCE=.\View\TextView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\TextView.h
# End Source File
# Begin Source File

SOURCE=.\View\View.cpp
# End Source File
# Begin Source File

SOURCE=.\View\View.h
# End Source File
# End Group
# Begin Group "DXFFile"

# PROP Default_Filter ""
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DXFFile\Base\DXFBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Base\DXFBase.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Base\SimpleList.h
# End Source File
# End Group
# Begin Group "Read"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DXFFile\Read\DRBlocks.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DRBlocks.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DREntities.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DREntities.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DRTables.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DRTables.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DXFRead.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Read\DXFRead.h
# End Source File
# End Group
# Begin Group "Write"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DXFFile\Write\DWBlocks.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DWBlocks.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DWEntities.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DWEntities.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DWTables.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DWTables.h
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DXFWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFFile\Write\DXFWrite.h
# End Source File
# End Group
# End Group
# End Target
# End Project
