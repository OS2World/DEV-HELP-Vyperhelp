# makefile
# Created by IBM WorkFrame/2 MakeMake at 13:38:04 on 16 April 1998
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Lib::Library Tool

.SUFFIXES: .cpp .obj .ycc .yhh

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /Ge- /C %s

.ycc.yhh:
    @echo " Compile::Visual Parse++ "
    ssparse.exe /Cpp %s

all: \
    .\gen.lib

.\gen.lib: \
    .\Identifier.obj \
    .\CDefineYacc.obj \
    .\Filename.obj \
    .\UserKey.obj \
    .\Registration.obj \
    .\FontInfo.obj \
    .\FontDefinition.obj \
    .\EnhancedYacc.obj \
    .\ParserException.obj \
    .\ParserIndicator.obj \
    .\ExpirationDate.obj \
    .\CurrentDate.obj \
    .\TraceGenerator.obj \
    .\HeaderGenerator.obj \
    .\TextParser.obj \
    .\WindowDef.obj \
    .\ArtDef.obj \
    .\WindowSet.obj \
    .\ArtSet.obj \
    .\CodePage.obj \
    .\ColumnList.obj \
    .\Distance.obj \
    .\Generator.obj \
    .\Gin.obj \
    .\Parser.obj \
    .\Symbol.obj \
    .\TextGenerator.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol gen.lib @makefile.in;


cdefine.yhh: cdefine.ycc


!include makefile.dep

