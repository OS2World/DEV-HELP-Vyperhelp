/***************************************************************************
 * File...... HtmlGenerator.hpp
 *
 * HtmlGenerator is a generator which will create Html tagged output.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/
#ifndef HTMLGENERATOR_HPP
#define HTMLGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Filename.hpp"
#include "Generator.hpp"
class HeaderGenerator;

// HtmlGenerator (private)
class HtmlContext;
class HtmlContextStack;
class HtmlTag;
class HtmlTagStack;
class HtmlLabelSet;
class HtmlIndexSet;


enum HtmlMode
{
  paragraph,
  quotation,        // <blockquote>
  singleList,       // <ul> <ol>
  lines,            // <pre>
  doubleList,       // <dl>
  table             // <table>
};


enum HtmlType
{
  singleFile,
  multiFile,
  htmlHelp
};


class _Export HtmlGenerator: public Generator
{
  public:

    // constructor
    HtmlGenerator( HtmlType type, const IString & filename, const IString & workDir = IString() );
    virtual ~HtmlGenerator();

    // header elements
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setCodePage( CodePage cp );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & addKeyword( const IString & keyword );

    // Gin handlers
    virtual void handleSection( const SectionGin & );
    virtual void handleLabel( const LabelGin & );
    virtual void handleRow( const RowGin & );
    virtual void handleColumn( const ColumnGin & );
    virtual void handleItem( const ItemGin & );
    virtual void handlePara( const ParaGin & );
    virtual void handleLine( const LineGin & );
    virtual void handlePush( const PushGin & );
    virtual void handlePop( const PopGin & );
    virtual void handleBorder( const BorderGin & );
    virtual void handleAlign( const AlignGin & );
    virtual void handleSpacing( const SpacingGin & );
    virtual void handleRuler( const RulerGin & );
    virtual void handleOutline( const OutlineGin & );
    virtual void handleDivider( const DividerGin & );
    virtual void handleFont( const FontGin & );
    virtual void handleSize( const SizeGin & );
    virtual void handleColor( const ColorGin & );
    virtual void handleBold( const BoldGin & );
    virtual void handleItalic( const ItalicGin & );
    virtual void handleUnderline( const UnderlineGin & );
    virtual void handleComment( const CommentGin & );
    virtual void handleLink( const LinkGin & );
    virtual void handleArt( const ArtGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );
    virtual void handleKeyword( const KeywordGin & );

    // done indicator
    virtual void finish();

  private:

    // output type
    HtmlType _type;

    //
    // HEADER INFO
    //
    IString _title;
    IString _subject;
    IString _version;
    IString _copyright;
    IString _author;
    IString _keywords;

    //
    // CODE PAGE
    //
    static const CodePage outputCodepage;
    CodePage _docCodePage;
    IString translate( const IString & string, Boolean allowSymbols = false ) const;
    Boolean translateChar( unsigned char ch, IString & symbolName ) const;

    //
    // BASIC SEND ROUTINES
    //
    enum HtmlOutputFlags
    {
      noModeCheck   = 0x00000100,
      noSymbolCheck = 0x00000200,
      mapCodePage   = 0x00000800,
      isNotContent  = 0x00001000
    };
    HtmlGenerator
        & sendComment( const IString & comment, int flags = 0 ),
        & sendMetaTag( const IString & name, const IString & content, Boolean isHttpEquiv = false),
        & sendTag( const IString & tag, int flags = 0 ),
        & sendText( const IString & text, int flags = 0 ),
        & sendRawText( IString & text, int flags = 0 ),
        & prepareToSend();
    void sendEnd(Boolean hasBody = true);
    enum HtmlDoctype
    {
      html3,
      html4,
      html4frameset
    };
    void sendHeader(const IString & title, Boolean isTop, HtmlDoctype doctype = html3, Boolean hasBody = true);

    //
    // HTML CONTEXT
    //
    static const HtmlTag tagPara;
    static const HtmlTag tagListItem;
    static const HtmlTag tagListTerm;
    static const HtmlTag tagListDef;
    static const HtmlTag tagTableRow;
    static const HtmlTag tagTableCol;
    HtmlTagStack * _tags;
    void      sendControlTag(const HtmlTag & tag, const HtmlTag & tagNext, Boolean isRow = false);
    void      sendEndTag(const IString & tag, HtmlMode mode = paragraph);
    void      sendStartTag(const HtmlTag & tag, HtmlMode mode = paragraph);
    IPosition findStartTag(const IString & tag);
    IPosition findLastBlock();
    void      undoTags(IPosition pos = 0, IPosition posTop = 0);
    void      redoTags(IPosition pos = 0);
    void      deleteTag(IPosition pos);
    void      insertTag(const HtmlTag & tag, IPosition pos);
    HtmlTag   getModeTag( const HtmlContext & context );
    IString   getFontColorTag(const IColor & color);
    Boolean   modeAttributesDiffer( const HtmlContext &, const HtmlContext & );

    //
    // SECTIONING
    //
    enum Mode
    {
      header,
      start,
      updatingHeading,
      ready,
      updatingContext
    }                _mode;
    HtmlGenerator  & wrapUpSection();

    //
    // STYLE CONTEXT
    //
    HtmlContext      * _active;
    HtmlContext      * _next;
    HtmlContextStack * _contextStack;
    Boolean         needsSwitchTo( const HtmlContext & );
    HtmlGenerator & switchContext( const HtmlContext & );

    //
    // CONTENT OUTPUT
    //
    static const unsigned _fileWidth;
    Filename         _filename;
    int              _fileCount;
    Filename         _activeFilename;
    ofstream         _file;
    IString          _workDir;
    void startFile();

    //
    // MULTI-FILE OUTPUT
    //
    Filename          _tocFilename;
    ofstream          _tocFile;
    unsigned          _tocLevel;
    Filename          _indexFilename;
    HeaderGenerator * _hh;
    Filename          _hhName;
    void finishMultiFile();
    void finishHtmlHelp();
    void startMultiFile();
    void startHtmlHelp();
    void sendSitemap(ofstream & file,
        const IString & name, const IString & local, const IString & keyword = IString());
    IString numberedFile(int fileNum);
    void sendPanelHeading(Boolean isToc);

    //
    // LABELS
    //
    HtmlLabelSet *   _labels;
    void fixupLabels();
    void fixupFile(const IString & filename);
    void setAnchor(const IString & name);
    static IString fixedLabel(const IString & label);

    //
    // INDEX
    //
    int            _indexCount;
    HtmlIndexSet * _index;

    //
    // DEBUGGING
    //
#ifdef IC_TRACE_DEVELOP
    void DumpTagStack() const;    // routine to dump stack contents
#else
    #define DumpTagStack 0        // null macro
#endif
};


#endif

