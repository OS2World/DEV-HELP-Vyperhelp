/***************************************************************************
 * File...... FontView.cpp
 * Author.... Mat
 * Date...... 2/4/97
 *
 * Implements the FontView.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "FontView.hpp"
#include "FontItem.hpp"
#include "FormatPen.hpp"
#include "DrawPen.hpp"


// static data
IGBitmap FontView::_bitmap( BMP_FONT );


FontView::FontView( FontItem * subject, View * parent ):
  IconView( subject, parent )
{}


IString FontView::info() const
{
  IString result = font()->name();
  unsigned long size = font()->pointSize();
  if ( result.length() && size )
    result += ", ";
  if ( size )
    result += IString(size) + IString(" pt");
  if ( ! result.length() )
    result = "Default";
  return result;
}


PlacedView::FormatChange FontView::format( FormatPen & pen )
{
  FormatChange change = PlacedView::format( pen );
  pen.setFont( font()->name(), font()->pointSize() );
  return change;
}


void FontView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.setFont( font()->name(), font()->pointSize() );
  IconView::draw( pen, zone );
}


FontItem * FontView::font() const
{
  return (FontItem *) subject();
}

