# vydit.mak
# Created by IBM WorkFrame/2 MakeMake at 15:57:36 on 2 June 1998
#
#     MANUALLY MAINTAINED
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker
#  Lib::Import Lib

.SUFFIXES:

.SUFFIXES: \
    .LIB .cpp .dll .obj

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CFLAGS) /Ge- /C %s

.dll.LIB:
    @echo " Lib::Import Lib "
    implib.exe %|dpfF.LIB %s

all: \
    .\vydit.LIB

.\vydit.dll: \
    .\ArtView.obj \
    .\ArtItem.obj \
    .\WordView.obj \
    .\WordViewCursor.obj \
    .\BoldItem.obj \
    .\BoldView.obj \
    .\CharView.obj \
    .\DrawPen.obj \
    .\Editor.obj \
    .\EditorCursor.obj \
    .\EditorTrigger.obj \
    .\EndItem.obj \
    .\EndView.obj \
    .\FastContext.obj \
    .\FlowFormatter.obj \
    .\FlowGenerator.obj \
    .\FlowItem.obj \
    .\FlowView.obj \
    .\FontItem.obj \
    .\FontView.obj \
    .\FormatPen.obj \
    .\IconView.obj \
    .\ItalicItem.obj \
    .\ItalicView.obj \
    .\Item.obj \
    .\ItemCursor.obj \
    .\ItemSequence.obj \
    .\Kursor.obj \
    .\LineEndView.obj \
    .\LineSubviewCursor.obj \
    .\LineView.obj \
    .\LinkItem.obj \
    .\LinkView.obj \
    .\NewLineItem.obj \
    .\NewLineView.obj \
    .\NullItemCursor.obj \
    .\NullViewCursor.obj \
    .\OrderedItemCursor.obj \
    .\OrderedViewCursor.obj \
    .\Page.obj \
    .\ParagraphItem.obj \
    .\ParagraphView.obj \
    .\Pen.obj \
    .\PenPoint.obj \
    .\PenState.obj \
    .\PlacedView.obj \
    .\PopItem.obj \
    .\PopView.obj \
    .\PushItem.obj \
    .\PushView.obj \
    .\SmartText.obj \
    .\SpaceItem.obj \
    .\SpaceView.obj \
    .\SubjectView.obj \
    .\SymbolItem.obj \
    .\SymbolView.obj \
    .\UnderlineItem.obj \
    .\UnderlineView.obj \
    .\View.obj \
    .\ViewCursor.obj \
    .\ViewSequence.obj \
    .\WordItem.obj \
    {$(LIB)}vygen.lib \
    {$(LIB)}vydit.def
    @echo " Link::Linker "
    icc.exe @<<
     $(CFLAGS)
     /B" $(LFLAGS) /nobase /pmtype:pm"
     /Fevydit.dll
     vygen.lib
     vydit.def
     cppopa3.obj
     .\ArtView.obj
     .\ArtItem.obj
     .\WordView.obj
     .\WordViewCursor.obj
     .\BoldItem.obj
     .\BoldView.obj
     .\CharView.obj
     .\DrawPen.obj
     .\Editor.obj
     .\EditorCursor.obj
     .\EditorTrigger.obj
     .\EndItem.obj
     .\EndView.obj
     .\FastContext.obj
     .\FlowFormatter.obj
     .\FlowGenerator.obj
     .\FlowItem.obj
     .\FlowView.obj
     .\FontItem.obj
     .\FontView.obj
     .\FormatPen.obj
     .\IconView.obj
     .\ItalicItem.obj
     .\ItalicView.obj
     .\Item.obj
     .\ItemCursor.obj
     .\ItemSequence.obj
     .\Kursor.obj
     .\LineEndView.obj
     .\LineSubviewCursor.obj
     .\LineView.obj
     .\LinkItem.obj
     .\LinkView.obj
     .\NewLineItem.obj
     .\NewLineView.obj
     .\NullItemCursor.obj
     .\NullViewCursor.obj
     .\OrderedItemCursor.obj
     .\OrderedViewCursor.obj
     .\Page.obj
     .\ParagraphItem.obj
     .\ParagraphView.obj
     .\Pen.obj
     .\PenPoint.obj
     .\PenState.obj
     .\PlacedView.obj
     .\PopItem.obj
     .\PopView.obj
     .\PushItem.obj
     .\PushView.obj
     .\SmartText.obj
     .\SpaceItem.obj
     .\SpaceView.obj
     .\SubjectView.obj
     .\SymbolItem.obj
     .\SymbolView.obj
     .\UnderlineItem.obj
     .\UnderlineView.obj
     .\View.obj
     .\ViewCursor.obj
     .\ViewSequence.obj
     .\WordItem.obj
<<

!include vydit.dep

