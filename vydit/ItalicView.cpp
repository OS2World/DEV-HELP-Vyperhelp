/***************************************************************************
 * File...... ItalicView.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implements the ItalicView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "ItalicView.hpp"
#include "ItalicItem.hpp"
#include "FormatPen.hpp"
#include "DrawPen.hpp"


// static data
IGBitmap ItalicView::_bitmap( BMP_ITALIC );


ItalicView::ItalicView( ItalicItem * subject, View * parent ):
  IconView( subject, parent )
{}


IString ItalicView::info() const
{
  return IString("Italic ") + onOffString( attribute()->isEnabled() );
}


PlacedView::FormatChange ItalicView::format( FormatPen & pen )
{
  FormatChange change = PlacedView::format( pen );
  pen.setItalic( attribute()->isEnabled() );
  return change;
}


void ItalicView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.setItalic( attribute()->isEnabled() );
  IconView::draw( pen, zone );
}


ItalicItem * ItalicView::attribute() const
{
  return (ItalicItem *) subject();
}

