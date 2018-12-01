# vedit.mak
# Created by IBM WorkFrame/2 MakeMake at 16:20:39 on 2 April 1998
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind
#  Compile::Resource Compiler

.SUFFIXES:

.SUFFIXES: \
    .RC .cpp .obj .res

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /C %s

.RC.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|fF.RES

all: \
    .\vedit.exe

.\vedit.exe: \
    .\VFrame.obj \
    .\VPort.obj \
    .\Main.obj \
    .\vedit.res \
    {$(LIB)}vydit.lib \
    {$(LIB)}vygen.lib
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS) /pmtype:pm"
     /Fevedit.exe
     vydit.lib
     vygen.lib
     .\VFrame.obj
     .\VPort.obj
     .\Main.obj
<<
    rc.exe .\vedit.res vedit.exe

!include vedit.dep

