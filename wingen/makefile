# makefile
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
    @echo * Compile %s
    icc.exe $(CFLAGS) /Ge- /C %s

.\wingen.lib: \
    .\RtfGenerator.obj \
    .\RtfGeneratorContext.obj \
    .\HpjGenerator.obj
    @echo * Create Library
    ilib.exe  /nol wingen.lib @makefile.in;

!include makefile.dep

