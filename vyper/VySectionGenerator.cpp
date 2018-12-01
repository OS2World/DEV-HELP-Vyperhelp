/***************************************************************************
 * File...... VySectionGenerator.cpp
 * Author.... Mat
 * Date...... 2/3/98
 *
 * Implementation for VySectionGenerator.  handleSection will set general
 * section info in VySection, other handlers will add specific Items to
 * the VySection's FlowItem.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// Vyper
#include "VySection.hpp"
#include "VySectionGenerator.hpp"


VySectionGenerator::VySectionGenerator( VySection & section, Boolean checkArt ):
    _section( section ),
    _flowGen( *section.flow(), checkArt )
{
  // add art definitions to the flow generator
  if ( checkArt )
    section.doc()->addArtTo( _flowGen );
}


void VySectionGenerator::finish()
{
  _flowGen.finish();
}


void VySectionGenerator::handleSection( const SectionGin & gin )
{
  // set basic section info
  _section.setFromGin( gin );
}


void VySectionGenerator::handleKeyword( const KeywordGin & gin )
{
  // add to collection in section
  VyKeyword * keyword = VyKeyword::newKeyword();
  keyword->setText( gin.text() );
  keyword->enableListed( gin.isListed() );
  keyword->enableExternal( gin.isExternal() );
  _section.addKeyword( keyword, false );
}


void VySectionGenerator::handleLabel( const LabelGin & gin )
{
  // add to collection in section
  VyLabel * label = VyLabel::newLabel();
  label->setName( gin.name() );
  label->setId( gin.id() );
  label->enableExternal( gin.isExternal() );
  _section.addLabel( label, false );
}


void VySectionGenerator::handlePara( const ParaGin & gin )
{
  _flowGen.handlePara( gin );
}


void VySectionGenerator::handleLine( const LineGin & gin )
{
  _flowGen.handleLine( gin );
}


void VySectionGenerator::handlePush( const PushGin & gin )
{
  _flowGen.handlePush( gin );
}


void VySectionGenerator::handlePop( const PopGin & gin )
{
  _flowGen.handlePop( gin );
}


void VySectionGenerator::handleFont( const FontGin & gin )
{
  _flowGen.handleFont( gin );
}


void VySectionGenerator::handleSize( const SizeGin & gin )
{
  _flowGen.handleSize( gin );
}


void VySectionGenerator::handleBold( const BoldGin & gin )
{
  _flowGen.handleBold( gin );
}


void VySectionGenerator::handleItalic( const ItalicGin & gin )
{
  _flowGen.handleItalic( gin );
}


void VySectionGenerator::handleUnderline( const UnderlineGin & gin )
{
  _flowGen.handleUnderline( gin );
}


void VySectionGenerator::handleLink( const LinkGin & gin )
{
  _flowGen.handleLink( gin );
}


void VySectionGenerator::handleArt( const ArtGin & gin )
{
  _flowGen.handleArt( gin );
}


void VySectionGenerator::handleText( const TextGin & gin )
{
  _flowGen.handleText( gin );
}


void VySectionGenerator::handleSymbol( const SymbolGin & gin )
{
  _flowGen.handleSymbol( gin );
}


