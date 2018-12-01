/***************************************************************************
 * File...... FlowGenerator.cpp
 * Author.... Mat
 * Date...... 2/3/98
 *
 * Implementation for FlowGenerator.  handleSection will set general
 * section info in Flow, other handlers will add specific Items to
 * the Flow's FlowItem.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// MekTek
#include "MekTek.hpp"

// Editor
#include "FlowItem.hpp"
#include "ParagraphItem.hpp"
#include "NewLineItem.hpp"
#include "PushItem.hpp"
#include "PopItem.hpp"
#include "FontItem.hpp"
#include "BoldItem.hpp"
#include "ItalicItem.hpp"
#include "UnderlineItem.hpp"
#include "LinkItem.hpp"
#include "ArtItem.hpp"
#include "SymbolItem.hpp"
#include "FlowGenerator.hpp"


FlowGenerator::FlowGenerator( FlowItem & flow, Boolean checkArt ):
    Generator( false ),
    _flow( flow ),
    _isFontChanged( false ),
    _checkArt( checkArt )
{
  // initialize context
  _context.family = FontInfo::system;
  _context.pointSize = 0;
  _context.push = 0;
}


void FlowGenerator::finish()
{
  flushText();
}


void FlowGenerator::flushText()
{
  if ( _text.length() )
  {
    _flow.appendText( _text );
    _text = IString();
  }
}


void FlowGenerator::flushFont()
{
  if ( _isFontChanged )
  {
    // this must be cleared first to avoid recursion
    _isFontChanged = false;

    // construct font info
    FontInfo info( _context.facename, _context.pointSize );

    // if the specified font isn't installed, use a standard font
    if ( ! info.isInstalled() )
      info.resetName( _context.family );

    appendItem( new FontItem( &_flow, info ) );
  }
}


void FlowGenerator::appendItem( Item * item )
{
  flushFont();
  flushText();
  _flow.appendItem( item );
}


void FlowGenerator::handlePara( const ParaGin & gin )
{
  appendItem( new ParagraphItem( &_flow ) );
}


void FlowGenerator::handleLine( const LineGin & gin )
{
  appendItem( new NewLineItem( &_flow ) );
}


void FlowGenerator::handlePush( const PushGin & gin )
{
  PushItem * push = new PushItem( &_flow );
  appendItem( push );
  _context.push = push;
  _nest.push( _context );
}


void FlowGenerator::handlePop( const PopGin & gin )
{
  if ( ! _nest.isEmpty() )
  {
    _nest.pop( _context );
    appendItem( new PopItem( &_flow, _context.push ) );
  }
}


void FlowGenerator::handleFont( const FontGin & gin )
{
  if ( gin.facename() != _context.facename
    || gin.family() != _context.family
    || gin.codepage() != _context.codePage )
  {
    _context.facename = gin.facename();
    _context.family = gin.family();
    _context.codePage = gin.codepage();
    _isFontChanged = true;
    flushText();
  }
}


void FlowGenerator::handleSize( const SizeGin & gin )
{
  unsigned long pointSize;

  // handle twips (for any other units, use default size)
  if ( gin.height().units() == Distance::twips )
    pointSize = gin.height().length() / 20;
  else
    pointSize = 0;

  if ( pointSize != _context.pointSize )
  {
    _context.pointSize = pointSize;
    _isFontChanged = true;
    flushText();
  }
}


void FlowGenerator::handleBold( const BoldGin & gin )
{
  appendItem( new BoldItem( &_flow, gin.isOn() ) );
}


void FlowGenerator::handleItalic( const ItalicGin & gin )
{
  appendItem( new ItalicItem( &_flow, gin.isOn() ) );
}


void FlowGenerator::handleUnderline( const UnderlineGin & gin )
{
  appendItem( new UnderlineItem( &_flow, gin.isOn() ) );
}


void FlowGenerator::handleLink( const LinkGin & gin )
{
  appendItem( new LinkItem( &_flow, gin ) );
}


void FlowGenerator::handleText( const TextGin & gin )
{
  // make sure font is current
  flushFont();

  // translate string to active codepage
  IString ginText = gin.string();
  CodePage output;  // default is active codepage
  output.translateString( ginText, _context.codePage );

  // add text to cache
  _text += ginText;
}


void FlowGenerator::handleSymbol( const SymbolGin & gin )
{
  appendItem( new SymbolItem( gin.symbol(), &_flow ) );
}


void FlowGenerator::handleArt( const ArtGin & gin )
{
  // only add art item if a corresponding definition is present
  // (or if checkArt flag is false)
  if ( ! _checkArt || hasArtWithId( gin.id() ) )
    appendItem( new ArtItem( &_flow, gin.id() ) );
}


