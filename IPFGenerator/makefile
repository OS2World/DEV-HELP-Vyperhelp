# makefile
# Created by IBM WorkFrame/2 MakeMake at 11:45:57 on 26 Feb 1998
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

.\ipfgen.lib: \
    .\IpfForwarder.obj \
    .\IpfForwarderSequence.obj \
    .\IPF_Symbols.obj \
    .\IPF_Send.obj \
    .\IPF_Art.obj \
    .\IPF_CharStyle.obj \
    .\IPF_Context.obj \
    .\IPF_Control.obj \
    .\IPF_Distance.obj \
    .\IPF_Generator.obj \
    .\IPF_Keyword.obj \
    .\IPF_ParaStyle.obj \
    .\IPF_Section.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol ipfgen.lib @makefile.in;

!include makefile.dep

