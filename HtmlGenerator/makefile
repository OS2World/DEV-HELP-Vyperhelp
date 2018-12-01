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
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /Ge- /C %s

.\htmlgen.lib: \
    .\HtmlIndex.obj \
    .\HtmlLabel.obj \
    .\HtmlCharStyle.obj \
    .\HtmlContext.obj \
    .\HtmlControl.obj \
    .\HtmlGenerator.obj \
    .\HtmlParaStyle.obj \
    .\HtmlSend.obj \
    .\HtmlSymbols.obj \
    .\HtmlTag.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol htmlgen.lib @makefile.in;

!include makefile.dep

