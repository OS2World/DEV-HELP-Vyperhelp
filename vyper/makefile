# makefile
# Created by IBM WorkFrame/2 MakeMake at 11:03:13 on 2 June 1998
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind
#  Compile::Resource Precompile
#  Compile::Resource Compiler

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj .rc .rcx .res .ipf .hlp

.ipf.hlp:
    @echo " Compile::IPF Compiler "
    ipfc %s

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /C %s

.rcx.rc:
    @echo " Compile::Resource Precompile "
    icc.exe -c /Pc+ /Pe+ /Pd /I. %s > %|dpfF.rc

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|fF.RES

.all: .\vyper.exe .\vyper.hlp

.\vyper.exe: \
    .\VyArt.obj \
    .\VyArtProperties.obj \
    .\VySymbol.obj \
    .\VySymbolSelector.obj \
    .\VyWait.obj \
    .\VyIndicator.obj \
    .\VySettings.obj \
    .\BooleanString.obj \
    .\CloseHandler.obj \
    .\Confirmation.obj \
    .\ExceptionNotification.obj \
    .\LogicalNot.obj \
    .\SectionEditHandler.obj \
    .\ContainerHelpHandler.obj \
    .\vbmain.obj \
    .\VyDoc.obj \
    .\VyDocParser.obj \
    .\VyDocGenerator.obj \
    .\VyDocProperties.obj \
    .\VyFont.obj \
    .\VyInfo.obj \
    .\VyRegistration.obj \
    .\VyKeyword.obj \
    .\VyKeywordProperties.obj \
    .\VyLabel.obj \
    .\VyLabelProperties.obj \
    .\VyLink.obj \
    .\VyLinkProperties.obj \
    .\Vyper.obj \
    .\VySection.obj \
    .\VySectionGenerator.obj \
    .\VySectionProperties.obj \
    .\LoadThread.obj \
    .\SaveThread.obj \
    .\Vyper.res \
    {$(LIB)}vydit.lib \
    {$(LIB)}vygen.lib
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS) /pmtype:pm"
     /Fevyper.exe
     vydit.lib
     vygen.lib
     cppopa3.obj
     .\VyArt.obj
     .\VyArtProperties.obj
     .\VySymbol.obj
     .\VySymbolSelector.obj
     .\VyWait.obj
     .\VyIndicator.obj
     .\VySettings.obj
     .\BooleanString.obj
     .\CloseHandler.obj
     .\Confirmation.obj
     .\ExceptionNotification.obj
     .\LogicalNot.obj
     .\SectionEditHandler.obj
     .\ContainerHelpHandler.obj
     .\vbmain.obj
     .\VyDoc.obj
     .\VyDocParser.obj
     .\VyDocGenerator.obj
     .\VyDocProperties.obj
     .\VyFont.obj
     .\VyInfo.obj
     .\VyRegistration.obj
     .\VyKeyword.obj
     .\VyKeywordProperties.obj
     .\VyLabel.obj
     .\VyLabelProperties.obj
     .\VyLink.obj
     .\VyLinkProperties.obj
     .\Vyper.obj
     .\VySection.obj
     .\VySectionGenerator.obj
     .\VySectionProperties.obj
     .\LoadThread.obj
     .\SaveThread.obj
<<
    rc.exe .\Vyper.res vyper.exe


.\vyper.hlp: .\vyper.ipf


!include "makefile.DEP"