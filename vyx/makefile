# makefile
# Created by IBM WorkFrame/2 MakeMake at 10:50:12 on 28 May 1998
#
# MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /C %s

.\vyx.exe: \
    .\Main.obj \
    .\ConsoleIndicator.obj \
    {$(LIB)}vygen.lib
    @echo " Link::Linker "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS)"
     /Fevyx.exe
     vygen.lib
     cppopa3.obj
     .\Main.obj
     .\ConsoleIndicator.obj
<<

!include makefile.dep

