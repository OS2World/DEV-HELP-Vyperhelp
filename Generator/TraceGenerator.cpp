/***************************************************************************
 * File...... TraceGenerator.cpp
 * Author.... Mat
 *
 * TraceGenerator implementation
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// Standard C++
#include <iomanip.h>

// OpenClass
#include <iexcept.hpp>      // ICLibErrorInfo

// Generator
#include "TraceGenerator.hpp"


//
// ---------- constructor ----------
//

TraceGenerator::TraceGenerator( const IString & filename ) :
    Generator( false ),
    _output( filename ),
    _parms( 0 )
{
  _output << "Active Code Page=" << CodePage() << endl;
  check();
}


void TraceGenerator::finish()
{
  check();
  _output.close();
}


//
// ---------- header elements ----------
//

Generator & TraceGenerator::setTitle( const IString & title )
{
  _output << "Title=" << title << endl;
  check();
  return *this;
}

Generator & TraceGenerator::setSubject( const IString & subject )
{
  _output << "Subject=" << subject << endl;
  check();
  return *this;
}

Generator & TraceGenerator::setVersion( const IString & version )
{
  _output << "Version=" << version << endl;
  check();
  return *this;
}

Generator & TraceGenerator::setCopyright( const IString & copyright )
{
  _output << "Copyright=" << copyright << endl;
  check();
  return *this;
}

Generator & TraceGenerator::setAuthor( const IString & author )
{
  _output << "Author=" << author << endl;
  check();
  return *this;
}

Generator & TraceGenerator::setCodePage( CodePage cp )
{
  _output << "Code Page=" << cp << endl;
  return *this;
}


Generator & TraceGenerator::addKeyword( const IString & keyword )
{
  _output << "Keyword=" << keyword << endl;
  check();
  return *this;
}


Generator & TraceGenerator::defineWindow( const WindowDef & window )
{
  sendGin( "WindowDef" );
  sendParm( window.id() );
  sendParm( window.title() );
  sendParm( window.originX().isDynamic() );
  if ( window.originX().isDynamic() )
    sendParm( window.originX().asDynamic() );
  else
  {
    sendParm( window.originX().asDistance().length() );
    sendParm( window.originX().asDistance().units() );
  }
  sendParm( window.originY().isDynamic() );
  if ( window.originY().isDynamic() )
    sendParm( window.originY().asDynamic() );
  else
  {
    sendParm( window.originY().asDistance().length() );
    sendParm( window.originY().asDistance().units() );
  }
  sendParm( window.width().length() );
  sendParm( window.width().units() );
  sendParm( window.height().length() );
  sendParm( window.height().units() );
  sendParm( window.border() );
  sendParm( window.scrolling() );
  sendParm( window.hasTitleBar() );
  sendParm( window.hasMinimize() );
  sendParm( window.hasMaximize() );
  sendParm( window.hasSystemMenu() );
  sendDone();
  return *this;
}


Generator & TraceGenerator::defineArt( const ArtDef & art )
{
  sendGin( "ArtDef" );
  sendParm( art.id() );
  sendDone();
  return *this;
}



//
// ---------- helper functions for stream ops ----------
//

// throw an exception if a file error is detected
void TraceGenerator::check()
{
  // use "void *" operator to check error state
  if ( ! _output )
    ICLibErrorInfo::throwCLibError( "ofstream", IEXCEPTION_LOCATION() );
}


void TraceGenerator::sendGin( const IString & text )
{
  _output << text;
  check();
}


void TraceGenerator::sendParm( const IString & text )
{
  _output << (_parms? ',': '(') << text;
  _parms++;
  check();
}


void TraceGenerator::sendDone()
{
  if ( _parms)
    _output << ')';
  _output << endl;
  _parms = 0;
  check();
}



//
// ---------- stream operators ----------
//

void TraceGenerator::handleSection( const SectionGin      & section )
{
  sendGin( "Section" );
  sendParm( section.title() );
  sendParm( section.level() );
  sendParm( section.isListed() );
  sendParm( section.windowId() );
  sendDone();
}

void TraceGenerator::handleKeyword( const KeywordGin      & keyword )
{
  sendGin( "Keyword" );
  sendParm( keyword.text() );
  sendParm( keyword.isListed() );
  sendParm( keyword.isExternal() );
  sendDone();
}

void TraceGenerator::handleLabel( const LabelGin        & label )
{
  sendGin( "Label" );
  sendParm( label.name() );
  sendParm( label.isExternal() );
  sendParm( label.id() );
  sendDone();
}

void TraceGenerator::handleRow( const RowGin          & row )
{
  sendGin( "Row" );
  sendDone();
}

void TraceGenerator::handleColumn( const ColumnGin       & column )
{
  sendGin( "Column" );
  sendDone();
}

void TraceGenerator::handleItem( const ItemGin         & item )
{
  sendGin( "Item" );
  sendDone();
}

void TraceGenerator::handlePara( const ParaGin         & para )
{
  sendGin( "Para" );
  sendDone();
}

void TraceGenerator::handleLine( const LineGin         & line )
{
  sendGin( "Line" );
  sendDone();
}

void TraceGenerator::handlePush( const PushGin         & push )
{
  sendGin( "Push" );
  sendDone();
}

void TraceGenerator::handlePop( const PopGin          & pop )
{
  sendGin( "Pop" );
  sendDone();
}

void TraceGenerator::handleRightMargin( const RightMarginGin  & rightMargin )
{
  sendGin( "RightMargin" );
  sendParm( rightMargin.isRelative() );
  sendParm( rightMargin.margin().length() );
  sendParm( rightMargin.margin().units() );
  sendDone();
}

void TraceGenerator::handleLeftMargin( const LeftMarginGin   & leftMargin )
{
  sendGin( "LeftMargin" );
  sendParm( leftMargin.isRelative() );
  sendParm( leftMargin.margin().length() );
  sendParm( leftMargin.margin().units() );
  sendDone();
}

void TraceGenerator::handleBorder( const BorderGin       & border )
{
  sendGin( "Border" );
  sendParm( IString( border.borders() ).d2x() );
  sendDone();
}

void TraceGenerator::handleAlign( const AlignGin        & align )
{
  sendGin( "Align" );
  sendParm( align.alignment() );
  sendDone();
}

void TraceGenerator::handleSpacing( const SpacingGin      & spacing )
{
  sendGin( "Spacing" );
  sendParm( spacing.spacing().length() );
  sendParm( spacing.spacing().units() );
  sendDone();
}

void TraceGenerator::handleRuler( const RulerGin        & ruler )
{
  sendGin( "Ruler" );
  sendParm( ruler.mode() );
  sendParm( ruler.lineBreak() );
  for ( int i = 1; i <= ruler.numberOfColumns(); i++ ) {
    sendParm( ruler.column( i ).length() );
    sendParm( ruler.column( i ).units() );
  } /* endfor */
  sendDone();
}

void TraceGenerator::handleOutline( const OutlineGin      & outline )
{
  sendGin( "Outline" );
  sendParm( outline.style() );
  sendParm( outline.prefix() );
  sendParm( outline.suffix() );
  sendDone();
}

void TraceGenerator::handleDivider( const DividerGin      & divider )
{
  sendGin( "Divider" );
  sendDone();
}

void TraceGenerator::handleFont( const FontGin         & font )
{
  sendGin( "Font" );
  sendParm( font.family() );
  sendParm( font.facename() );
  sendParm( (long)font.codepage() );
  sendDone();
}

void TraceGenerator::handleSize( const SizeGin         & size )
{
  sendGin( "Size" );
  sendParm( size.height().length() );
  sendParm( size.height().units() );
  sendDone();
}

void TraceGenerator::handleColor( const ColorGin        & color )
{
  sendGin( "Color" );
  sendParm( color.color().index() );
  sendDone();
}

void TraceGenerator::handleBackColor( const BackColorGin    & backColor )
{
  sendGin( "BackColor" );
  sendParm( backColor.color().index() );
  sendDone();
}

void TraceGenerator::handleBold( const BoldGin         & bold )
{
  sendGin( "Bold" );
  sendParm( bold.isOn() );
  sendDone();
}

void TraceGenerator::handleItalic( const ItalicGin       & italic )
{
  sendGin( "Italic" );
  sendParm( italic.isOn() );
  sendDone();
}

void TraceGenerator::handleUnderline( const UnderlineGin    & underline )
{
  sendGin( "Underline" );
  sendParm( underline.isOn() );
  sendDone();
}

void TraceGenerator::handleComment( const CommentGin      & comment )
{
  sendGin( "Comment" );
  sendParm( comment.isOn() );
  sendDone();
}

void TraceGenerator::handleLink( const LinkGin         & link )
{
  sendGin( "Link" );
  sendParm( link.isOn() );
  sendParm( link.isExternal() );
  sendParm( link.file() );
  sendParm( link.target() );
  sendDone();
}

void TraceGenerator::handleHide( const HideGin         & hide )
{
  sendGin( "Hide" );
  sendParm( hide.isOn() );
  sendParm( hide.key() );
  sendDone();
}

void TraceGenerator::handleArt( const ArtGin          & art )
{
  sendGin( "Art" );
  sendParm( art.id() );
  sendParm( art.spot() );
  sendDone();
}

void TraceGenerator::handleText( const TextGin         & text )
{
  sendGin( "Text" );
  IString shortString( text.string() );
  if ( shortString.length() > 40 )
    shortString.remove( 38 ).insert( "...", 37 );
  sendParm( shortString );
  sendDone();
}

void TraceGenerator::handleSymbol( const SymbolGin       & symbol )
{
  sendGin( "Symbol" );
  sendParm( symbol.symbol().id() );
  unsigned char ch;
  CodePage cp;
  symbol.symbol().getDetails( ch, cp );
  sendParm( ch );
  sendParm( (long)cp );
  sendDone();
}


