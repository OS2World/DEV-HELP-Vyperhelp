/***************************************************************************
 * File...... SymbolView.cpp
 * Author.... Mat
 * Date...... 4/1/97
 *
 * Implementation of SymbolView.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <igstring.hpp>     // IGString

// TextEditor
#include "DrawPen.hpp"
#include "FormatPen.hpp"
#include "SymbolView.hpp"
#include "SymbolItem.hpp"

// constant char used to represent symbols that can't map to current code page
static const unsigned char unknownChar = '*';


SymbolView::SymbolView( SymbolItem * subject, View * parent ):
  PlacedView( subject, parent )
{}


IString SymbolView::info() const
{
  return symbol().description();
}


Coord SymbolView::width() const
{
  // map symbol to current code page, if possible
  unsigned char ch = symbol().asChar();
  return state().charWidth( ch? ch: unknownChar );
}


Coord SymbolView::height() const
{
  return state().maxAscender();
}


Coord SymbolView::descent() const
{
  return state().maxDescender();
}


PlacedView::FormatChange SymbolView::format( FormatPen & pen )
{
  // save old metrics
  Coord oldWidth = width();
  Coord oldHeight = height();
  Coord oldDescent = descent();

  // do default preparation and formatting
  FormatChange change = PlacedView::format( pen );

  // check for size change
  if ( oldWidth != width()
    || oldHeight != height()
    || oldDescent != descent() )
    change = changeSize;

  return change;
}


void SymbolView::draw( DrawPen & pen, const IRectangle & zone )
{
  unsigned char ch = symbol().asChar();

  // draw unknown char (changed color)
  if (ch == 0 )
  {
    IColor original( pen.state().color() );
    IColor shaded( IColor::paleGray );
    pen.setColor( &shaded );
    pen.drawChar( unknownChar );
    pen.setColor( &original );
  }
  // draw known char
  else
    pen.drawChar( ch );

  pen.forward( width() );
}


const Symbol & SymbolView::symbol() const
{
  SymbolItem * item = (SymbolItem *) subject();
  return item->symbol();
}


IString SymbolView::dumpString() const
{
  return debugString( IString("SymbolView (") + symbol().description() + IString(")") );
}


