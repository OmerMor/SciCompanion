# Microsoft Developer Studio Project File - Name="ProfSkinDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ProfSkinDLL - Win32 Unicode Debug RDE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProfSkinDLL_600.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProfSkinDLL_600.mak" CFG="ProfSkinDLL - Win32 Unicode Debug RDE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProfSkinDLL - Win32 ANSI Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 ANSI Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 MBCS Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 MBCS Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 ANSI Debug RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 ANSI Release RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 MBCS Debug RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 MBCS Release RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 Unicode Debug RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProfSkinDLL - Win32 Unicode Release RDE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProfSkinDLL - Win32 ANSI Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_ANSI_Debug"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_ANSI_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\AnsiDebug\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DevSys\Prof-UIS\Include" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin\ProfSkin-d.dll" /libpath:"D:\DevSys\Prof-UIS\Bin_600"
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292nd.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 ANSI Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_ANSI_Release"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_ANSI_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\AnsiRelease\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"D:\DevSys\Prof-UIS\Bin_600"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292n.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 MBCS Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_MBCS_Debug"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_MBCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\MbcsDebug\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DevSys\Prof-UIS\Include" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin\ProfSkin-d.dll" /libpath:"D:\DevSys\Prof-UIS\Bin_600"
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292md.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 MBCS Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_MBCS_Release"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_MBCS_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\MbcsRelease\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"D:\DevSys\Prof-UIS\Bin_600"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292m.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\UnicodeDebug\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin_600\ProfSkin292ud.dll" /pdbtype:sept /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\UnicodeRelease\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"D:\DevSys\Prof-UIS\Bin_600"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292u.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 ANSI Debug RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_ANSI_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_ANSI_Debug_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\AnsiDebugRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DevSys\Prof-UIS\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292nd.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292nd-RDE.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 ANSI Release RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_ANSI_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_ANSI_Release_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\AnsiReleaseRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292n.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292n-RDE.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 MBCS Debug RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_MBCS_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_MBCS_Debug_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\MbcsDebugRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DevSys\Prof-UIS\Include" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292md.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\Bin_600\ProfSkin292md-RDE.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 MBCS Release RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_MBCS_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_MBCS_Release_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\MbcsReleaseRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292m.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292m-RDE.dll" /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 Unicode Debug RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_Unicode_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_Unicode_Debug_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\UnicodeDebugRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DevSys\Prof-UIS\Include" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin_600\ProfSkin292ud.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin_600\ProfSkin292ud-RDE.dll" /pdbtype:sept /libpath:"..\Bin_600"

!ELSEIF  "$(CFG)" == "ProfSkinDLL - Win32 Unicode Release RDE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkin_DLL__Win32_Unicode_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkin_DLL__Win32_Unicode_Release_RDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\UnicodeReleaseRDE\ProfSkinDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "D:\DevSys\Prof-UIS\Include" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "__PROF_SKIN_IMPL__" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\Include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292u.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin_600\ProfSkin292u-RDE.dll" /libpath:"..\Bin_600"

!ENDIF 

# Begin Target

# Name "ProfSkinDLL - Win32 ANSI Debug"
# Name "ProfSkinDLL - Win32 ANSI Release"
# Name "ProfSkinDLL - Win32 MBCS Debug"
# Name "ProfSkinDLL - Win32 MBCS Release"
# Name "ProfSkinDLL - Win32 Unicode Debug"
# Name "ProfSkinDLL - Win32 Unicode Release"
# Name "ProfSkinDLL - Win32 ANSI Debug RDE"
# Name "ProfSkinDLL - Win32 ANSI Release RDE"
# Name "ProfSkinDLL - Win32 MBCS Debug RDE"
# Name "ProfSkinDLL - Win32 MBCS Release RDE"
# Name "ProfSkinDLL - Win32 Unicode Debug RDE"
# Name "ProfSkinDLL - Win32 Unicode Release RDE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ExtSkinBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtSkinItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtSkinPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfSkin.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ExtSkinBitmap.h
# End Source File
# Begin Source File

SOURCE=.\ExtSkinItem.h
# End Source File
# Begin Source File

SOURCE=.\ExtSkinPaintManager.h
# End Source File
# Begin Source File

SOURCE=.\ProfSkin.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
