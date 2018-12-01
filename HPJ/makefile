# makefile
# Created by IBM WorkFrame/2 MakeMake at 16:12:36 on 20 Nov 1997
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Lib::Library Tool

.SUFFIXES: .cpp .obj .ycc .yhh

.all: \
    .\winhelp.lib

.ycc.yhh:
    @echo " Compile::Visual Parse++ "
    ssparse.exe /Cpp %s

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /Ge- /C %s

.\winhelp.lib: \
    .\RtfYacc.obj \
    .\RtfContext.obj \
    .\RtfSection.obj \
    .\WinHelpParser.obj \
    .\RtfColor.obj \
    .\RtfFont.obj \
    .\HpjListYacc.obj \
    .\HpjYacc.obj \
    .\CntYacc.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol winhelp.lib @makefile.in;

rtf.yhh: rtf.ycc

hpj.yhh: hpj.ycc

hpjlist.yhh: hpjlist.ycc

cnt.yhh: cnt.ycc


!include "makefile.DEP"