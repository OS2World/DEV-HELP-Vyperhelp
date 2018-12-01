# makefile
# Created by IBM WorkFrame/2 MakeMake at 10:49:30 on 28 May 1998
#
#     MANUALLY MAINTAINED
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

.\GenTest.exe: \
    .\TestTables.obj \
    .\TestWindows.obj \
    .\GenTest.obj \
    .\TestArt.obj \
    .\TestChar.obj \
    .\TestIPF.obj \
    .\TestKeywords.obj \
    .\TestList.obj \
    .\TestMargins.obj \
    .\TestNesting.obj \
    .\TestPara.obj \
    .\TestSections.obj \
    .\TestSymbols.obj \
    {$(LIB)}vygen.lib
    @echo " Link::Linker "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS)"
     /FeGenTest.exe
     vygen.lib
     .\TestTables.obj
     .\TestWindows.obj
     .\GenTest.obj
     .\TestArt.obj
     .\TestChar.obj
     .\TestIPF.obj
     .\TestKeywords.obj
     .\TestList.obj
     .\TestMargins.obj
     .\TestNesting.obj
     .\TestPara.obj
     .\TestSections.obj
     .\TestSymbols.obj
<<

!include makefile.dep

