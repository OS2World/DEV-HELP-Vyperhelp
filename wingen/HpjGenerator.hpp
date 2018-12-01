/***************************************************************************
 * File...... HpjGenerator.hpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * This Generator will create a WinHelp project, including HPJ (project
 * file), CNT (table of contents), HH (symbol definitions), and RTF
 * (formatted text).
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef HPJGENERATOR_HPP
#define HPJGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Generator.hpp"
#include "Filename.hpp"
class HeaderGenerator;
class RtfGenerator;


class _Export HpjGenerator: public Generator
{
  public:
    // constructor
    HpjGenerator( const Filename & filename );
    ~HpjGenerator();

    // for RtfGenerator
    Filename artFilename( const IString & id ) const;

    // from Generator
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & setCodePage( CodePage cp );
    virtual Generator & addKeyword( const IString & keyword );

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

    virtual void finish();

  private:
    // helpers
    void start();
    void check( ofstream & stream ) const;
    IString sanitize( const IString & topic ) const;
    IString translate( const IString & text, Boolean forCnt ) const;
    void sendCntEntry( unsigned nextLevel );
#ifdef GEN_WINDOWS
    IString windowMetrics( const WindowDef & win ) const;
#endif

    // general control
    enum
    {
      ready,
      started,
      done
    } _state;
    CodePage _cpOut;
    CodePage _cpIn;
    unsigned _tocCount;

    // output files
    Filename _baseName;
    ofstream _hpj;
    ofstream _cnt;

    // header file
    Filename _hhName;
    HeaderGenerator * _hh;

    // RTF content
    Filename _rtfName;
    RtfGenerator * _rtf;

    // section info
    IString _sectionTitle;
    IString _sectionLabel;
    unsigned _sectionLevel;
    unsigned _cntLevel;         // section level for contents file (supports workarounds)
    unsigned _maxCntLevel;
    unsigned _sectionWindowId;

    // options
    IString _copyright;
    IString _title;

};


#endif

