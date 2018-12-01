/***************************************************************************
 * File...... DrawPen.cpp
 * Author.... Mat
 * Date...... 9/30/97
 *
 * Implementation of DrawPen
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "Page.hpp"
#include "FastContext.hpp"
#include "DrawPen.hpp"


DrawPen::DrawPen( Page & page ):
  Pen( page )
{}


DrawPen::~DrawPen()
{
  flush();
}


void DrawPen::start( PlacedView & view )
{
  flush();
  Pen::start( view );
}


IPoint DrawPen::baseline() const
{
  return point() + IPair( 0, lineDescent() );
}


IGraphicContext & DrawPen::context()
{
  return page().context();
}


// clear above the given height and below the given descent if necessary
// flushed batched text, so it shouldn't be called for text Views
void DrawPen::clear( Coord width, Coord height, Coord descent )
{
  flush();
  clearFrom( baseline(), width, height, descent );
}


void DrawPen::clearFrom( const IPoint & position, Coord width, Coord height, Coord descent )
{
  if ( height < lineHeight() )
  {
    // clear above the height
    IRectangle rect( position + IPair( 0, height ),
                     ISize( width, lineHeight() - height ) );
    page().clearRect( rect );
  }

  if ( descent < lineDescent() )
  {
    // clear below the descent
    IRectangle rect( position - IPair( 0, lineDescent() ),
                     ISize( width, lineDescent() - descent ) );
    page().clearRect( rect );
  }
}

void DrawPen::drawChar( char ch )
{
  // add to text batch
  if ( ! _batch.length() )
    _batchPosition = baseline();
  _batch += IString( ch );
}


void DrawPen::drawString( const IString & string )
{
  // add to text batch
  if ( ! _batch.length() )
    _batchPosition = baseline();
  _batch += string;
}


void DrawPen::drawBitmap( const IBitmapHandle & bitmap )
{
  flush();
  page().context().drawBitmap( bitmap, baseline() );
}


Boolean DrawPen::setFont( const IString & name, unsigned long pointSize )
{
  flush();
  return Pen::setFont( name, pointSize );
}


Boolean DrawPen::setBold( Boolean isBold )
{
  flush();
  return Pen::setBold( isBold );
}


Boolean DrawPen::setItalic( Boolean isItalic )
{
  flush();
  return Pen::setItalic( isItalic );
}


Boolean DrawPen::setUnderline( Boolean isUnderline )
{
  flush();
  return Pen::setUnderline( isUnderline );
}


Boolean DrawPen::setColor( const IColor * color )
{
  flush();
  return Pen::setColor( color );
}


void DrawPen::setState( const PenState & newState )
{
  flush();
  Pen::setState( newState );
}


void DrawPen::flush()
{
  unsigned n = _batch.length();
  Coord totalWidth;

  if ( n == 0 )
    return;  // no text to flush

  if ( n == 1 )
  {
    // draw a single character
    totalWidth = state().charWidth( _batch[1] );
    page().context().drawChar( _batch[1], totalWidth, _batchPosition );
  }
  else
  {
    // draw a string of characters
    // build array of character widths
    totalWidth = 0;
    unsigned i;
    Coord * widths = new Coord[n];
    unsigned char * pch = _batch;
    for (i = 0; i < n; i++)
    {
      widths[i] = state().charWidth( pch[i] );
      totalWidth += widths[i];
    }

    // draw the string
    page().context().drawString( _batch, widths, _batchPosition );

    // free widths array
    delete widths;
  }

  // clear unclaimed area
  clearFrom( _batchPosition, totalWidth, state().maxAscender(), state().maxDescender() );

  // clear the string
  _batch = IString();
}


