/***************************************************************************
 * File...... IpfParser.hpp
 * Author.... Mat
 *
 * IpfParser facilitates parsing of an IPF and all imbedded IPFs.
 * The generate member function is used to convert the IPF using a Generator.
 *
 * Copyright (C) 1998 MekTek
 *
 ***************************************************************************/
#ifndef IPFPARSER_HPP
#define IPFPARSER_HPP

// OpenClass
#include <iqueue.h>         // IQueue<>
#include <istack.h>         // IStack<>
#include <ikeyset.h>        // IKeySet<>
#include <ikeybag.h>        // IKeyBag<>
#include <istring.hpp>      // IString

// Generator
#include "Gin.hpp"
#include "Parser.hpp"
#include "Filename.hpp"
#include "WindowDef.hpp"

// IPF Parser
#include "SymbolLookup.hpp"
#include "Footnote.hpp"
#include "StringPair.hpp"
#include "PanelLink.hpp"


class _Export IpfParser: public Parser
{
  public:
    // constructor
    IpfParser( const IString & ipfFile );
    virtual ~IpfParser();

    // mode control
    enum Mode
    {
      front,        // before :userdoc.
      limbo,        // after :userdoc., before first :h1
      normal,       // normal section text
      centered,     // centered text (.ce)
      literal,      // CR/LF taken literally
      artlink,      // inside :artlink block
      footnote      // inside :fn block
    };
    Mode mode();
    void setMode( Mode mode );
    void pushMode( Mode mode, const IString * id = 0 );
      // id required for footnote
    void popMode( Mode mode );

    // imbedded content
    void     imbedFile( const Filename & filename, Filename::StandardPath path );
    void     imbedText( const IString & text );

    // non-nested styles
    void     setForegroundColor( const IColor & color );
    void     setBackgroundColor( const IColor & color );
    void     setFont( const FontGin & font, const SizeGin & size );
    void     resetFont( FontInfo::Family family, unsigned pointSize = 0 );
    void     setLeftMargin( unsigned sizeInChars );
    void     setRightMargin( unsigned sizeInChars );
    void     finishSection();
    void     handlePop();

        // non-nested (but contained) styles
    void     setBold( Boolean isOn = true );
    void     setItalic( Boolean isOn = true );
    void     setUnderline( Boolean isOn = true );

    // pass 1 functions
    void    addPanelLink( const PanelLink & link );
    void    addEntry( const StringPair & entry );
    void    addSynonyms( const IString & root, const IString & synonyms );
    IString getPanelName( int id );
    IString getEntryText( const IString & name );
    void    sendSynonyms( const IString & roots );

    // send functions
    void sendGin( Gin * gin );
    void sendText( const IString & text, Boolean isContent = true );
    void sendSymbol( const Symbol & symbol );
    void sendPara();
    void sendLine();

    // symbol lookup
    Boolean addSymbol( const SymbolLookup & lookup );
      // returns true if added
    Boolean locateSymbol( SymbolLookup & lookup );
      // returns true is found

    // table of contents display level
    unsigned tocLevel();
    void     setTocLevel( unsigned level );
    void     setLevel( unsigned level );

    // line flag (keep track whether \n should be converted to space)
    // NOTE: sendText and sendSymbol do an implicit setLineFlag
    void     clearLineFlag();
    void     setLineFlag();
    Boolean  isLineFlagSet() const;

    // table support (no nested tables allowed)
    Boolean tableStart( unsigned maxColumn );
      // returns true if table can be started (not already active)
    Boolean tableRow();
      // returns true if row was expected
    Boolean tableColumn( Boolean & isFirstColumn );
      // returns true if column was expected
    Boolean tableStop();
      // returns true if end of table was expected

    // window management
    void addWindow( const WindowDef & window );
    int  nextWindowId();

    // document title
    void setTitle( const IString & title );

    // bitmap filename collection
    Boolean addBitmap( const IString & filename, IString & id );

    // warnings & progress indicator
    void warning( const IString & text, unsigned line = 0, unsigned col = 0 );

    // context ID management
    Boolean hasContext(int id) const;
    IString getContext(int id) const;

    // from Parser
    virtual void generate( Generator & gen );
    virtual void setId( const IString & contextName, int id );

  private:
    // helpers
    void flushText();
    void finish();
    void checkOutput( Boolean isContent );

    // input and output
    Filename      _ipfFile;
    IString       _homeDir;    // path to IPF file
    int           _imbedText;  // level of imbedded text (0 = none)
    int           _pass;
    Generator *   _gen;

    // header information
    IKeySet <PanelLink, int>      _contexts;  // help context IDs from header file

    // pass 1 information
    IKeyBag <StringPair, IString> _synonyms;  // index synonyms
    IKeySet <StringPair, IString> _entries;   // primary index entries
    IKeySet <PanelLink, int>      _panels;    // panel "link by resource" information

    // general control data
    Mode            _mode;
    IStack <Mode>   _stack;
    IString         _text;
    SymbolLookupSet _symbols;
    Filename        _activeFile;
    FilenameList    _bitmaps;
    int             _windowCounter;
    Boolean         _lineFlag;
    unsigned        _column;
    unsigned        _maxColumn;  // 0 if table is not active
    Boolean         _hasSectionText;

    // footnotes
    IQueue <Footnote *> _footnotes;
    Footnote *          _footnote;
    int                 _footnoteWindowId;
    ColorGin *          _saveForeColor;
    BackColorGin *      _saveBackColor;
    FontGin *           _saveFont;
    SizeGin *           _saveSize;
    RightMarginGin *    _saveRightMargin;
    LeftMarginGin *     _saveLeftMargin;
    Boolean             _saveIsBold;
    Boolean             _saveIsItalic;
    Boolean             _saveIsUnderline;

    // current settings for non-nested settings
    ColorGin *          _foreColor;
    BackColorGin *      _backColor;
    FontGin *           _font;
    SizeGin *           _size;
    RightMarginGin *    _rightMargin;
    LeftMarginGin *     _leftMargin;
    Boolean             _isBold;
    Boolean             _isItalic;
    Boolean             _isUnderline;

    // table of contents display level
    unsigned _tocLevel;
};


// inline functions
#include "IpfParser.ipp"


#endif

