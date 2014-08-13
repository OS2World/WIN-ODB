# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=odbmv - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to odbmv - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "odbmv - Win32 Release" && "$(CFG)" != "odbmv - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "odbmv.mak" CFG="odbmv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "odbmv - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "odbmv - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "odbmv - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe
F90=fl32.exe

!IF  "$(CFG)" == "odbmv - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\odbmv.exe"

CLEAN : 
	-@erase "$(INTDIR)\Database.obj"
	-@erase "$(INTDIR)\Linkedli.obj"
	-@erase "$(INTDIR)\Lstrlst.obj"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Odbset.obj"
	-@erase "$(INTDIR)\Outbuf.obj"
	-@erase "$(INTDIR)\parestrees.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\Propval.obj"
	-@erase "$(INTDIR)\Q_stream.obj"
	-@erase "$(INTDIR)\S_stream.obj"
	-@erase "$(INTDIR)\scanner.obj"
	-@erase "$(INTDIR)\Sj_strm.obj"
	-@erase "$(INTDIR)\Store.obj"
	-@erase "$(INTDIR)\Strtbl.obj"
	-@erase "$(INTDIR)\Table.obj"
	-@erase "$(INTDIR)\Type.obj"
	-@erase "$(INTDIR)\Valid.obj"
	-@erase "$(OUTDIR)\odbmv.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Ox /I "Release/" /c /nologo
# ADD F90 /Ox /I "Release/" /c /nologo
F90_PROJ=/Ox /I "Release/" /c /nologo /Fo"Release/" 
F90_OBJS=.\Release/
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/odbmv.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/odbmv.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/odbmv.pdb" /machine:I386 /out:"$(OUTDIR)/odbmv.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Database.obj" \
	"$(INTDIR)\Linkedli.obj" \
	"$(INTDIR)\Lstrlst.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Odbset.obj" \
	"$(INTDIR)\Outbuf.obj" \
	"$(INTDIR)\parestrees.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\Propval.obj" \
	"$(INTDIR)\Q_stream.obj" \
	"$(INTDIR)\S_stream.obj" \
	"$(INTDIR)\scanner.obj" \
	"$(INTDIR)\Sj_strm.obj" \
	"$(INTDIR)\Store.obj" \
	"$(INTDIR)\Strtbl.obj" \
	"$(INTDIR)\Table.obj" \
	"$(INTDIR)\Type.obj" \
	"$(INTDIR)\Valid.obj"

"$(OUTDIR)\odbmv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\odbmv.exe"

CLEAN : 
	-@erase "$(INTDIR)\Database.obj"
	-@erase "$(INTDIR)\Linkedli.obj"
	-@erase "$(INTDIR)\Lstrlst.obj"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Odbset.obj"
	-@erase "$(INTDIR)\Outbuf.obj"
	-@erase "$(INTDIR)\parestrees.obj"
	-@erase "$(INTDIR)\parser.obj"
	-@erase "$(INTDIR)\Propval.obj"
	-@erase "$(INTDIR)\Q_stream.obj"
	-@erase "$(INTDIR)\S_stream.obj"
	-@erase "$(INTDIR)\scanner.obj"
	-@erase "$(INTDIR)\Sj_strm.obj"
	-@erase "$(INTDIR)\Store.obj"
	-@erase "$(INTDIR)\Strtbl.obj"
	-@erase "$(INTDIR)\Table.obj"
	-@erase "$(INTDIR)\Type.obj"
	-@erase "$(INTDIR)\Valid.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\odbmv.exe"
	-@erase "$(OUTDIR)\odbmv.ilk"
	-@erase "$(OUTDIR)\odbmv.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Zi /I "Debug/" /c /nologo
# ADD F90 /Zi /I "Debug/" /c /nologo
F90_PROJ=/Zi /I "Debug/" /c /nologo /Fo"Debug/" /Fd"Debug/odbmv.pdb" 
F90_OBJS=.\Debug/
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/odbmv.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/odbmv.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/odbmv.pdb" /debug /machine:I386 /out:"$(OUTDIR)/odbmv.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Database.obj" \
	"$(INTDIR)\Linkedli.obj" \
	"$(INTDIR)\Lstrlst.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Odbset.obj" \
	"$(INTDIR)\Outbuf.obj" \
	"$(INTDIR)\parestrees.obj" \
	"$(INTDIR)\parser.obj" \
	"$(INTDIR)\Propval.obj" \
	"$(INTDIR)\Q_stream.obj" \
	"$(INTDIR)\S_stream.obj" \
	"$(INTDIR)\scanner.obj" \
	"$(INTDIR)\Sj_strm.obj" \
	"$(INTDIR)\Store.obj" \
	"$(INTDIR)\Strtbl.obj" \
	"$(INTDIR)\Table.obj" \
	"$(INTDIR)\Type.obj" \
	"$(INTDIR)\Valid.obj"

"$(OUTDIR)\odbmv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

################################################################################
# Begin Target

# Name "odbmv - Win32 Release"
# Name "odbmv - Win32 Debug"

!IF  "$(CFG)" == "odbmv - Win32 Release"

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\valid.h

!IF  "$(CFG)" == "odbmv - Win32 Release"

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Linkedli.cpp
DEP_CPP_LINKE=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Linkedli.obj" : $(SOURCE) $(DEP_CPP_LINKE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Lstrlst.cpp
DEP_CPP_LSTRL=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Lstrlst.obj" : $(SOURCE) $(DEP_CPP_LSTRL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Object.cpp
DEP_CPP_OBJEC=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Object.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Odbset.cpp
DEP_CPP_ODBSE=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Odbset.obj" : $(SOURCE) $(DEP_CPP_ODBSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Outbuf.cpp
DEP_CPP_OUTBU=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Outbuf.obj" : $(SOURCE) $(DEP_CPP_OUTBU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\parestrees.cpp
DEP_CPP_PARES=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\parestrees.obj" : $(SOURCE) $(DEP_CPP_PARES) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\parser.cpp
DEP_CPP_PARSE=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\parser.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\parser.h

!IF  "$(CFG)" == "odbmv - Win32 Release"

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Propval.cpp
DEP_CPP_PROPV=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Propval.obj" : $(SOURCE) $(DEP_CPP_PROPV) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Q_stream.cpp
DEP_CPP_Q_STR=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Q_stream.obj" : $(SOURCE) $(DEP_CPP_Q_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\S_stream.cpp
DEP_CPP_S_STR=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\S_stream.obj" : $(SOURCE) $(DEP_CPP_S_STR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\scanner.cpp
DEP_CPP_SCANN=\
	"..\..\Source\parser.h"\
	

"$(INTDIR)\scanner.obj" : $(SOURCE) $(DEP_CPP_SCANN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Sj_strm.cpp
DEP_CPP_SJ_ST=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Sj_strm.obj" : $(SOURCE) $(DEP_CPP_SJ_ST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Store.cpp
DEP_CPP_STORE=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Store.obj" : $(SOURCE) $(DEP_CPP_STORE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Store.h

!IF  "$(CFG)" == "odbmv - Win32 Release"

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Storelib.h

!IF  "$(CFG)" == "odbmv - Win32 Release"

!ELSEIF  "$(CFG)" == "odbmv - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Strtbl.cpp
DEP_CPP_STRTB=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Strtbl.obj" : $(SOURCE) $(DEP_CPP_STRTB) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Table.cpp
DEP_CPP_TABLE=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Table.obj" : $(SOURCE) $(DEP_CPP_TABLE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Type.cpp
DEP_CPP_TYPE_=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Type.obj" : $(SOURCE) $(DEP_CPP_TYPE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Valid.cpp
DEP_CPP_VALID=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Valid.obj" : $(SOURCE) $(DEP_CPP_VALID) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Staffan\odb\Source\Database.cpp
DEP_CPP_DATAB=\
	"..\..\Source\parser.h"\
	"..\..\Source\Store.h"\
	

"$(INTDIR)\Database.obj" : $(SOURCE) $(DEP_CPP_DATAB) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
