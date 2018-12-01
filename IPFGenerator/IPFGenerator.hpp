/***************************************************************************
 * File...... IPFGenerator.hpp
 * Author.... Mat
 * Date...... 9/28/95
 *
 * IPFGenerator is a generator which will create IPF tagged output.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef IPFGENERATOR_HPP
#define IPFGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Generator.hpp"

// IPFGenerator (private)
class LabelList;
class IPFContext;
class ContextStack;
class IndexSet;
class IpfForwarderSequence;


class _Export IPFGenerator: public Generator
{
  public:

    // constructor
    IPFGenerator( const IString & filename, unsigned maxLineLength, const IString & workDir = IString() );
    virtual ~IPFGenerator();

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
    virtual void handleDivider( const DividerGin & );
    virtual void handleFont( const FontGin & );
    virtual void handleSize( const SizeGin & );
    virtual void handleColor( const ColorGin & );
    virtual void handleBackColor( const BackColorGin & );
    virtual void handleBold( const BoldGin & );
    virtual void handleItalic( const ItalicGin & );
    virtual void handleUnderline( const UnderlineGin & );
    virtual void handleComment( const CommentGin & );
    virtual void handleLink( const LinkGin & );
    virtual void handleHide( const HideGin & );
    virtual void handleArt( const ArtGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );

    // done indicator
    virtual void finish();

  private:

    //
    // CODE PAGE
    //
    CodePage _docCodePage;
    IString translate( const IString & string, Boolean allowSymbols = false ) const;
    Boolean translateChar( unsigned char ch, IString & symbolName ) const;

    //
    // BASIC SEND ROUTINES
    //
    enum IPFOutputFlags
    {
      noModeCheck   = 0x00000100,
      noSymbolCheck = 0x00000200,
      controlWord   = 0x00000400,
      mapCodePage   = 0x00000800,
      isNotContent  = 0x00001000,
      isFormat      = 0x00002000
    };
    IPFGenerator
        & sendTag( const IString & tag, int flags ),
        & sendText( const IString & text, int flags ),
        & sendControl( const IString & control, int flags ),
        & sendRawText( IString & text, int flags ),
        & prepareToSend(),
        & sendEnd();

    //
    // UNITS CONVERSION
    //
    IString      convertPosition( const Position & position, Boolean isX );
    IString      convertDistance( const Distance & distance );
    unsigned int charsFromDistance( const Distance & distance );
    unsigned int linesFromDistance( const Distance & distance );
    unsigned int pointsFromDistance( const Distance & distance );

    //
    // SECTIONING
    //
    enum Mode {
      header,
      start,
      waitingForLabel,
      updatingHeading,
      ready,
      updatingContext
    }                _mode;
    SectionGin       _section;
    Boolean          _emptySection;
    LabelGin         _label;
    LabelList      * _moreLabels;
    IpfForwarderSequence * _forwarders;
    IPFGenerator   & wrapUpSection();
    void             tag_h();
    void             sendForwarders();

    //
    // STYLE CONTEXT
    //
    enum TableMode {
      notInTable,
      inLimbo,
      inCell
    }                _tableMode;
    IPFContext     * _active;
    IPFContext     * _next;
    ContextStack   * _contextStack;
    IPFGenerator   & nextCell( Boolean isRow );
    Boolean          needsSwitchTo( const IPFContext & );
    IPFGenerator   & switchContext( const IPFContext & );
    IPFGenerator   & prepForTable();
    void             sendLinkTag( const IPFContext & );

    //
    // INDEX
    //
    IndexSet       * _index;
    int              tag_i1( const IString & text, Boolean isGlobal );
    void             tag_i2( const IString & text, int refid, Boolean isGlobal );

    //
    // OUTPUT
    //
    ofstream         _file;
    IString          _workDir;

    //
    // ID GENERATION
    //
    int              _nextId;
    int              nextId();
};


#endif

