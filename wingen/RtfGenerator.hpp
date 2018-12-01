/***************************************************************************
 * File...... RtfGenerator.hpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * This Generator will create RTF test in one of two modes:
 *     WinHelp
 *     Document (no HpjGenerator given)
 *
 * In WinHelp mode, special overloaded functionss are used (footnotes,
 * strikethrough, hidden text, etc) for the WinHelp compiler.  In Document
 * mode, the document is created for printing.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef RTFGENERATOR_HPP
#define RTFGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Filename.hpp"
#include "Generator.hpp"
#include "RtfFont.hpp"
#include "RtfColor.hpp"
class HpjGenerator;
class RtfGeneratorContext;
class RtfGeneratorContextStack;

class _Export RtfGenerator: public Generator
{
  public:
    // constructor
    RtfGenerator( const IString & filename, HpjGenerator * hpj = 0 );
    RtfGenerator( ostream & stream, HpjGenerator * hpj = 0 );
    virtual ~RtfGenerator();

    // offset of last section
    unsigned sectionOffset() const;

    // document info
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & setCodePage( CodePage cp );
    virtual Generator & addKeyword( const IString & keyword );

    // Gin handlers
    virtual void handleSection( const SectionGin & );
    virtual void handleKeyword( const KeywordGin & );
    virtual void handleLabel( const LabelGin & );
    virtual void handleRow( const RowGin & );
    virtual void handleColumn( const ColumnGin & );
    virtual void handleItem( const ItemGin & );
    virtual void handlePara( const ParaGin & );
    virtual void handleLine( const LineGin & );
    virtual void handlePush( const PushGin & );
    virtual void handlePop( const PopGin & );
    virtual void handleRightMargin( const RightMarginGin & );
    virtual void handleLeftMargin( const LeftMarginGin & );
    virtual void handleBorder( const BorderGin & );
    virtual void handleAlign( const AlignGin & );
    virtual void handleSpacing( const SpacingGin & );
    virtual void handleRuler( const RulerGin & );
    virtual void handleOutline( const OutlineGin & );
    virtual void handleFont( const FontGin & );
    virtual void handleSize( const SizeGin & );
    virtual void handleColor( const ColorGin & );
    virtual void handleBackColor( const BackColorGin & );
    virtual void handleBold( const BoldGin & );
    virtual void handleItalic( const ItalicGin & );
    virtual void handleUnderline( const UnderlineGin & );
    virtual void handleComment( const CommentGin & );
    virtual void handleLink( const LinkGin & );
    virtual void handleArt( const ArtGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );

    // done indicator
    virtual void finish();

  private:
    // helper functions
    void initialize();
    IString translate( const IString & text );
    void sendFontTable();
    void sendColorTable();
    void sendStylesheet();
    void sendInfo();
    void sendInfoItem( const IString & code, const IString & text );
    void sendHeading( const IString & text, unsigned style );
    int  addColor( const RtfColor & color );
    void sendText( const IString & text, unsigned flags = 0 );
    void sendLine( Boolean force = true );
    void sendFootnote( char footnote, const IString & text );
    void endRuler();

    // margin & spacing support
    void setRightMargin( int twips, Boolean isRelative = true );
    void setLeftMargin( int twips, Boolean isRelative = true );
    void setTab( int twips );
    void sendLeftMarginTab();
    void setSpacing( int twips );

    // link support
    void sendLink( const LinkGin & link );
    Boolean linkChange( const LinkGin & link, const LinkGin & test );
    void startLink();

    // table support
    void sendTableBorders( unsigned borders );
    void startTable( const RulerGin & ruler );
    void endTable();
    void endRow( Boolean isLastRow );
    void endCell();

    // paragraph support
    void endPara();
    void resetPara();

    // collaborator (0 for "document" mode)
    HpjGenerator * _hpj;

    // output stream
    ofstream * _file;  // 0 if no filename given
    ostream * _stream; // same as _file if filename was given, otherwise same as constructor

    // header info
    IString _title;
    IString _subject;
    IString _version;
    IString _copyright;
    IString _author;
    IString _keywords;

    // output codepage
    CodePage _cpOut;

    // codepage for titles and index entries
    CodePage _cp;

    // RTF tables
    RtfColorTable _colors;
    unsigned      _colorNum;
    RtfFontTable  _fonts;
    unsigned      _fontNum;

    // context and stack
    RtfGeneratorContext      * _context;
    RtfGeneratorContextStack * _stack;

    // temporary file for RTF contents
    Filename _contentName;
    fstream  _content;

    // output flags
    enum RtfOutputFlags
    {
      delimAfter     = 0x00000100,  // set this flag if a delimiter is needed
      checkSpecial   = 0x00000200   // check for special RTF symbols \{}
    };
    Boolean _needDelimiter;

    // tables
    unsigned _rowCount;
    unsigned _cellCount;

    // general control
    unsigned _sectionCount;
    enum ParaState
    {
      ended,   // paragraph is ended
      reset,   // a style change forced a new paragraph
      started  // text has been written to the current paragraph
    } _para;
};


// inline functions
#include "RtfGenerator.ipp"


#endif

