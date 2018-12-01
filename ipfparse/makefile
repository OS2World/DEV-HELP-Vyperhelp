# makefile
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Lib::Library Tool

.SUFFIXES: .cpp .obj .ycc .yhh

.all: \
    .\ipfparse.lib

.ycc.yhh:
    @echo *** Compile %s
    ssparse.exe /Cpp %s

.cpp.obj:
    @echo *** Compile %s
    icc.exe $(CFLAGS) /Ge- /C %s

.\ipfparse.lib: \
    .\IpfYacc.obj \
    .\IpfParser.obj \
    .\Footnote.obj \
    .\StringPair.obj \
    .\PanelLink.obj \
    .\SymbolLookup.obj
    @echo *** Update library
    ilib.exe  /nol ipfparse.lib @makefile.in;

ipf.yhh: ipf.ycc


!include "makefile.DEP"