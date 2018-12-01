/***************************************************************************
 * File...... SpaceView.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of SpaceView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString

// TextEditor
#include "FormatPen.hpp"
#include "DrawPen.hpp"
#include "SpaceView.hpp"


SpaceView::SpaceView( Item * subject, View * parent ):
  PlacedView( subject, parent )
{}


Coord SpaceView::width() const
{
  return state().charWidth( ' ' );
}


PlacedView::FormatChange SpaceView::format( FormatPen & pen )
{
  // save old width
  Coord oldWidth = width();

  // do default preparation and formatting
  FormatChange change = PlacedView::format( pen );

  // check for width change
  if ( oldWidth != width() )
    change = changeSize;

  return change;
}


void SpaceView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.drawChar( ' ' );
  pen.forward( width() );
}


IString SpaceView::dumpString() const
{
  return debugString( "SpaceView" );
}

