/***************************************************************************
 * File...... Pen.cpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * Implementation of Pen
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros
#include <ifont.hpp>        // IFont

// Generator
#include "CodePage.hpp"

// TextEditor
#include "LineView.hpp"
#include "FlowView.hpp"
#include "OrderedViewCursor.hpp"
#include "FontInfo.hpp"
#include "Page.hpp"
#include "FastContext.hpp"
#include "Pen.hpp"


Pen::Pen( Page & page ):
  _page( page )
{}


// null destructor (must be declared virtual)
Pen::~Pen()
{}


void Pen::start( PlacedView & view )
{
  // get line
  LineView * line = (LineView *) view.parent();
  IASSERTSTATE( line );
  IASSERTSTATE( line->isLine() );

  // get line position
  _lineHeight = line->height();
  _lineDescent = line->descent();
  _lineWidth = line->width();
  Coord x = view.isFormatted()? view.position().x(): 0;
  Coord y = _page.size().height() - line->above() - _lineHeight - _lineDescent;
  _point.set( IPair( x, y ) );

  // reset page context to default font and color
  _state.setFont( FontInfo(), _page.context() );
  setContextFont();
  _state.setColor( _page.defaultColor() );
  setContextColor();

  // get saved state from view, if it has one
  if ( view.isFormatted() )
    setState( view.state() );
}


void Pen::forward( Coord width )
{
  // advance the pen point
  _point += IPoint( width, 0 );
}


Boolean Pen::setFont( const IString & name, unsigned long pointSize )
{
  if ( ( name != _state.font().name() )
    || ( pointSize != _state.font().pointSize() ) )
  {
    FontInfo font = _state.font();
    font.setFont( name, pointSize );
    _state.setFont( font, _page.context() );
    setContextFont();
    return true;
  }
  return false;
}


Boolean Pen::setBold( Boolean isBold )
{
  if ( isBold != _state.font().isBold() )
  {
    FontInfo font = _state.font();
    font.setBold( isBold );
    _state.setFont( font, _page.context() );
    setContextFont();
    return true;
  }
  return false;
}


Boolean Pen::setItalic( Boolean isItalic )
{
  if ( isItalic != _state.font().isItalic() )
  {
    FontInfo font = _state.font();
    font.setItalic( isItalic );
    _state.setFont( font, _page.context() );
    setContextFont();
    return true;
  }
  return false;
}


Boolean Pen::setUnderline( Boolean isUnderline )
{
  if ( isUnderline != _state.font().isUnderline() )
  {
    FontInfo font = _state.font();
    font.setUnderline( isUnderline );
    _state.setFont( font, _page.context() );
    setContextFont();
    return true;
  }
  return false;
}


Boolean Pen::setColor( const IColor * color )
{
  IColor defaultColor( _page.defaultColor() );
  if ( ! color )
    color = &defaultColor;
  if ( *color != _state.color() )
  {
    _state.setColor( *color );
    setContextColor();
    return true;
  }
  return false;
}


void Pen::setContextFont()
{
  IGraphicContext & context = _page.context();
  IFont * ifont = _state.font().newFont( context );
  CodePage::setContextFont( context, *ifont );
  delete ifont;
}


void Pen::setContextColor()
{
  _page.context().setPenColor( _state.color() );
}


// set internal _state to match given state
void Pen::setState( const PenState & state )
{
  // font attributes
  setFont( state.font().name(), state.font().pointSize() );
  setBold( state.font().isBold() );
  setItalic( state.font().isItalic() );
  setUnderline( state.font().isUnderline() );
  setColor( &state.color() );
}


