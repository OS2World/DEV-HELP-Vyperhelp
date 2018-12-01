/***************************************************************************
 * File...... VyDocGenerator.hpp
 * Author.... Mat
 * Date...... 2/3/98
 *
 * This class works with a VyDoc to generate settings and Items which
 * are part of the VyDoc.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef VYDOCGENERATOR_HPP
#define VYDOCGENERATOR_HPP

// VFile
#include "LoadGenerator.hpp"
class SaveGenerator;

class VyDoc;


class VyDocGenerator: public LoadGenerator
{
  public:
    // constructor
    VyDocGenerator( VyDoc & doc );

    // from LoadGenerator
    virtual void registerSection( unsigned long offset, const SectionGin & section );

    // from Generator
    virtual Generator & setCodePage( CodePage cp );
    virtual Generator & setTitle( const IString & title );
    virtual Generator & setSubject( const IString & subject );
    virtual Generator & setVersion( const IString & version );
    virtual Generator & setCopyright( const IString & copyright );
    virtual Generator & setAuthor( const IString & author );
    virtual Generator & defineArt( const ArtDef & art );

    // Gin handlers are used during import, but not load
    virtual void handleSection( const SectionGin & gin );
    virtual void handleKeyword( const KeywordGin & gin );
    virtual void handleLabel( const LabelGin & gin );
    virtual void handleRow( const RowGin & gin );
    virtual void handleColumn( const ColumnGin & gin );
    virtual void handleItem( const ItemGin & gin );
    virtual void handlePara( const ParaGin & gin );
    virtual void handleLine( const LineGin & gin );
    virtual void handlePush( const PushGin & gin );
    virtual void handlePop( const PopGin & gin );
    virtual void handleRightMargin( const RightMarginGin & gin );
    virtual void handleLeftMargin( const LeftMarginGin & gin );
    virtual void handleBorder( const BorderGin & gin );
    virtual void handleAlign( const AlignGin & gin );
    virtual void handleSpacing( const SpacingGin & gin );
    virtual void handleRuler( const RulerGin & gin );
    virtual void handleOutline( const OutlineGin & gin );
    virtual void handleDivider( const DividerGin & gin );
    virtual void handleFont( const FontGin & gin );
    virtual void handleSize( const SizeGin & gin );
    virtual void handleColor( const ColorGin & gin );
    virtual void handleBackColor( const BackColorGin & gin );
    virtual void handleBold( const BoldGin & gin );
    virtual void handleItalic( const ItalicGin & gin );
    virtual void handleUnderline( const UnderlineGin & gin );
    virtual void handleComment( const CommentGin & gin );
    virtual void handleLink( const LinkGin & gin );
    virtual void handleHide( const HideGin & gin );
    virtual void handleArt( const ArtGin & gin );
    virtual void handleText( const TextGin & gin );
    virtual void handleSymbol( const SymbolGin & gin );
    virtual void finish();

private:
    void closeSection();
    IString translate( const IString & string );

    VyDoc &         _doc;
    SaveGenerator * _save;
    CodePage        _cp;
};


#endif

