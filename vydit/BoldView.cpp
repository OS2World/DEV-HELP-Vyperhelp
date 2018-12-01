/***************************************************************************
 * File...... BoldView.cpp
 * Author.... Mat
 *
 * Implements the BoldView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "BoldView.hpp"
#include "BoldItem.hpp"
#include "DrawPen.hpp"
#include "FormatPen.hpp"


// static data
IGBitmap BoldView::_bitmap( BMP_BOLD );


BoldView::BoldView( BoldItem * subject, View * parent ):
  IconView( subject, parent )
{}


IString BoldView::info() const
{
  return IString("Bold ") + onOffString( attribute()->isEnabled() );
}


PlacedView::FormatChange BoldView::format( FormatPen & pen )
{
  FormatChange change = PlacedView::format( pen );
  pen.setBold( attribute()->isEnabled() );
  return change;
}


void BoldView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.setBold( attribute()->isEnabled() );
  IconView::draw( pen, zone );
}


BoldItem * BoldView::attribute() const
{
  return (BoldItem *) subject();
}

