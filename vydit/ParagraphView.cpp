/***************************************************************************
 * File...... ParagraphView.cpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * Implementation of ParagraphView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString


// TextEditor
#include "DrawPen.hpp"
#include "ParagraphView.hpp"


// static data
IGBitmap ParagraphView::_bitmap( BMP_PARAGRAPH );


ParagraphView::ParagraphView( Item * subject, View * parent ):
  IconView( subject, parent )
{}


/***************************************************************************
 * Procedure.. ParagraphView::descent()
 *
 * Need a non-0 descent to create space between paragraphs.  Not sure what
 * a "standard" space between paragraphs is, but for now, using the icon
 * height should be sufficient.
 ***************************************************************************/
Coord ParagraphView::descent() const
{
  return height();
}


void ParagraphView::draw( DrawPen & pen, const IRectangle & zone )
{
  // claim descent=0 to allow pen to clear area below
  pen.clear( width(), height(), 0 );

  // draw the icon
  IconView::draw( pen, zone );
}


IString ParagraphView::dumpString() const
{
  return debugString( "ParagraphView" );
}


