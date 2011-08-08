# Microsoft Developer Studio Project File - Name="ProfSkinLIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ProfSkinLIB - Win32 Static Unicode Debug RDE with MFC DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProfSkinLIB_600.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProfSkinLIB_600.mak" CFG="ProfSkinLIB - Win32 Static Unicode Debug RDE with MFC DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Debug with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Release with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Debug with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Release with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Debug with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Release with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Debug RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Debug RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Release RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static ANSI Release RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Debug RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Debug RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Release RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static MBCS Release RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Debug RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Debug RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Release RDE" (based on "Win32 (x86) Static Library")
!MESSAGE "ProfSkinLIB - Win32 Static Unicode Release RDE with MFC DLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiRelease\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sn.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiDebug\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292snd.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeDebug\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sud.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeRelease\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292su.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MBCS Debug"
# PROP BASE Intermediate_Dir "MBCS Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsDebug\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292smd.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MBCS Release"
# PROP BASE Intermediate_Dir "MBCS Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsRelease\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sm.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Debug with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Debug_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Debug_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiDebugWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292snd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ynd.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Release with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Release_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Release_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiReleaseWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sn.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yn.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Debug with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Unicode_Debug_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Unicode_Debug_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeDebugWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sud.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yud.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Release with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Unicode_Release_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Unicode_Release_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeReleaseWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292su.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yu.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Debug with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_MBCS_Debug_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_MBCS_Debug_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsDebugWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292smd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ymd.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Release with MFC DLL"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_MBCS_Release_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_MBCS_Release_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsReleaseWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sm.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ym.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Debug RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_ANSI_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_ANSI_Debug_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiDebugRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292snd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292snd-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Debug RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_ANSI_Debug_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_ANSI_Debug_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiDebugRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292ynd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ynd-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Release RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_ANSI_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_ANSI_Release_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiReleaseRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sn.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sn-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static ANSI Release RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_ANSI_Release_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_ANSI_Release_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticAnsiReleaseRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292yn.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yn-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Debug RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_MBCS_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_MBCS_Debug_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsDebugRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292smd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292smd-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Debug RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_MBCS_Debug_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_MBCS_Debug_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsDebugRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292ymd.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ymd-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Release RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_MBCS_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_MBCS_Release_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsReleaseRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sm.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sm-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static MBCS Release RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_MBCS_Release_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_MBCS_Release_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticMbcsReleaseRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292ym.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292ym-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Debug RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_Unicode_Debug_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_Unicode_Debug_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeDebugRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292sud.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292sud-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Debug RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_Unicode_Debug_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_Unicode_Debug_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeDebugRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "_DEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "_DEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292yud.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yud-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Release RDE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_Unicode_Release_RDE"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_Unicode_Release_RDE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeReleaseRde\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292su.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292su-RDE.lib"

!ELSEIF  "$(CFG)" == "ProfSkinLIB - Win32 Static Unicode Release RDE with MFC DLL"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProfSkinLIB___Win32_Static_Unicode_Release_RDE_with_MFC_DLL"
# PROP BASE Intermediate_Dir "ProfSkinLIB___Win32_Static_Unicode_Release_RDE_with_MFC_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin_600"
# PROP Intermediate_Dir "..\Bin_600\StaticUnicodeReleaseRdeWithMfcDll\ProfSkinLIB"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\include" /I "..\UsedLibs\LibPNG" /I "..\UsedLibs\ZLib" /D "NDEBUG" /D "_UNICODE" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_SKIN_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /Zm512 /D "WIN32" /D "_WINDOWS" /D "__PROF_UIS_FOR_REGULAR_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
# ADD RSC /l 0x419 /i "..\Include" /d "NDEBUG" /d "_AFXDLL" /d "__STATPROFUIS_WITH_DLLMFC__"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Bin_600\ProfSkin292yu.lib"
# ADD LIB32 /nologo /out:"..\Bin_600\ProfSkin292yu-RDE.lib"

!ENDIF 

# Begin Target

# Name "ProfSkinLIB - Win32 Static ANSI Release"
# Name "ProfSkinLIB - Win32 Static ANSI Debug"
# Name "ProfSkinLIB - Win32 Static Unicode Debug"
# Name "ProfSkinLIB - Win32 Static Unicode Release"
# Name "ProfSkinLIB - Win32 Static MBCS Debug"
# Name "ProfSkinLIB - Win32 Static MBCS Release"
# Name "ProfSkinLIB - Win32 Static ANSI Debug with MFC DLL"
# Name "ProfSkinLIB - Win32 Static ANSI Release with MFC DLL"
# Name "ProfSkinLIB - Win32 Static Unicode Debug with MFC DLL"
# Name "ProfSkinLIB - Win32 Static Unicode Release with MFC DLL"
# Name "ProfSkinLIB - Win32 Static MBCS Debug with MFC DLL"
# Name "ProfSkinLIB - Win32 Static MBCS Release with MFC DLL"
# Name "ProfSkinLIB - Win32 Static ANSI Debug RDE"
# Name "ProfSkinLIB - Win32 Static ANSI Debug RDE with MFC DLL"
# Name "ProfSkinLIB - Win32 Static ANSI Release RDE"
# Name "ProfSkinLIB - Win32 Static ANSI Release RDE with MFC DLL"
# Name "ProfSkinLIB - Win32 Static MBCS Debug RDE"
# Name "ProfSkinLIB - Win32 Static MBCS Debug RDE with MFC DLL"
# Name "ProfSkinLIB - Win32 Static MBCS Release RDE"
# Name "ProfSkinLIB - Win32 Static MBCS Release RDE with MFC DLL"
# Name "ProfSkinLIB - Win32 Static Unicode Debug RDE"
# Name "ProfSkinLIB - Win32 Static Unicode Debug RDE with MFC DLL"
# Name "ProfSkinLIB - Win32 Static Unicode Release RDE"
# Name "ProfSkinLIB - Win32 Static Unicode Release RDE with MFC DLL"
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

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
