/***************************************************************************
 * File...... SaveGenerator.hpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Generator for saving generator info to a binary file.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef SAVEGENERATOR_HPP
#define SAVEGENERATOR_HPP

// Generator
#include "Generator.hpp"

// VFile
#include "OutputBinaryFile.hpp"


class _Export SaveGenerator: public Generator
{
  public:
    // constructor
    SaveGenerator( const IString & filename, Boolean sectionOnly = false );
    SaveGenerator( ostream & stream, Boolean sectionOnly = false );

    // offset of last section
    unsigned sectionOffset() const;

    // document info
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & setLanguage( Language language );
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
    // helpers
    void writeDistance( const Distance & distance );
    void writePosition( const Position & position );
    void writeColor( const IColor & color );
    void finishSection( Boolean allDone );
    void initialize();

    // output stream
    OutputBinaryFile _output;
    Boolean          _sectionOnly;

    // doc info
    IString  _title;
    IString  _subject;
    IString  _version;
    IString  _copyright;
    IString  _author;
    Language _language;
    CodePage _cp;
    IString  _keywords;
    Boolean  _isInfoSaved;

    // support for maintaining section links
    unsigned _sectionOffset;
};


#endif

