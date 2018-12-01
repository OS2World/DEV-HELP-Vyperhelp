/***************************************************************************
 * File...... VySectionGenerator.hpp
 * Author.... Mat
 * Date...... 2/3/98
 *
 * This class works with a VySection to generate settings and Items which
 * are part of the VySection.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef VYSECTIONGENERATOR_HPP
#define VYSECTIONGENERATOR_HPP

// OpenClass
#include <istack.h>

// Generator
#include "LoadGenerator.hpp"

// Editor
#include "FlowGenerator.hpp"

// Vyper
class VySection;


class VySectionGenerator: public LoadGenerator
{
  public:
    // constructor
    VySectionGenerator( VySection & section, Boolean checkArt = true );

    // from LoadGenerator
    virtual void registerSection( unsigned long offset, const SectionGin & section );

    // from Generator
    virtual void handleSection( const SectionGin & );
    virtual void handleKeyword( const KeywordGin & );
    virtual void handleLabel( const LabelGin & );
    virtual void handlePara( const ParaGin & );
    virtual void handleLine( const LineGin & );
    virtual void handlePush( const PushGin & );
    virtual void handlePop( const PopGin & );
    virtual void handleFont( const FontGin & );
    virtual void handleSize( const SizeGin & );
    virtual void handleBold( const BoldGin & );
    virtual void handleItalic( const ItalicGin & );
    virtual void handleUnderline( const UnderlineGin & );
    virtual void handleLink( const LinkGin & );
    virtual void handleArt( const ArtGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );
    virtual void finish();

private:
    // section data
    VySection &   _section;
    FlowGenerator _flowGen;
};


// inline functions
#include "VySectionGenerator.ipp"


#endif

