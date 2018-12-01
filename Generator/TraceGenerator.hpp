/***************************************************************************
 * File...... TraceGenerator.hpp
 * Author.... Mat
 *
 * TraceGenerator is a generator which will dumps a list of Gins as they
 * are encountered.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef TRACEGENERATOR_HPP
#define TRACEGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Generator.hpp"


class _Export TraceGenerator: public Generator
{
  public:
    // constructor
    TraceGenerator( const IString & filename );

    // header elements
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & setCodePage( CodePage cp );
    virtual Generator & addKeyword( const IString & keyword );
    virtual Generator & defineWindow( const WindowDef & window );
    virtual Generator & defineArt( const ArtDef & art );

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
    virtual void finish();

  private:
    void sendGin( const IString & text );
    void sendParm( const IString & text );
    void sendDone();
    void check();

    ofstream _output;
    int      _parms;
};


#endif

