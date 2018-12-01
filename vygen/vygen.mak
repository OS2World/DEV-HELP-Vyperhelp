# vygen.mak
# Created by IBM WorkFrame/2 MakeMake at 11:21:38 on 26 Feb 1998
#
# MANUALLY MAINTAINED!
#  To include references to selected LIB files in external directory
#  Also need to use linker directly (not via compiler)
#
# The actions included in this make file are:
#  Link::Linker
#  Lib::Import Lib

.SUFFIXES:

.SUFFIXES: \
    .LIB .dll

.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

{D:\proj\vygen}.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

all: \
    .\vygen.LIB

.\vygen.dll: \
    $(PROJOBJ)\htmlgen.lib \
    $(PROJOBJ)\winhelp.lib \
    $(PROJOBJ)\ipfparse.lib \
    $(PROJOBJ)\gen.lib \
    $(PROJOBJ)\ipfgen.lib \
    $(PROJOBJ)\wingen.lib \
    $(PROJOBJ)\vfile.lib \
    {$(LIB)}ssvpioo.lib \
    {$(LIB)}vygen.def
    @echo " Link::Linker "
    ilink.exe /NOFREE @<<
     $(LFLAGS) /nobase /pmtype:pm /nod:DDE4MBS +
     $(PROJOBJ)\htmlgen.lib +
     $(PROJOBJ)\winhelp.lib +
     $(PROJOBJ)\ipfparse.lib +
     $(PROJOBJ)\gen.lib +
     $(PROJOBJ)\ipfgen.lib +
     $(PROJOBJ)\wingen.lib +
     $(PROJOBJ)\vfile.lib +
     cppopa3.obj
     vygen.dll

     ssvpioo.lib
     vygen.def
<<

.\vygen.LIB: \
    .\vygen.dll
