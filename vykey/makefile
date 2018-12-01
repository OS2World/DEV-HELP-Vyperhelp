# makefile
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

.\vykey.exe: \
    .\vykey.obj \
    {$(LIB)}vygen.lib
    @echo " Link::Linker "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS)"
     /Fevykey.exe
     vygen.lib
     cppopa3.obj
     .\vykey.obj
<<

!include makefile.dep

