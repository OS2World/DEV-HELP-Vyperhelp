# makefile
# Created by IBM WorkFrame/2 MakeMake at 10:28:48 on 28 May 1998
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Lib::Library Tool

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /Ge- /C %s

.\vfile.lib: \
    .\OutputBinaryFile.obj \
    .\LoadParser.obj \
    .\LoadGenerator.obj \
    .\SaveGenerator.obj \
    .\BinaryFile.obj \
    .\InputBinaryFile.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol vfile.lib @makefile.in;

!include makefile.dep

