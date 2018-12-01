/***************************************************************************
 * File...... UnderlineView.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implements the UnderlineView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "UnderlineView.hpp"
#include "UnderlineItem.hpp"
#include "FormatPen.hpp"
#include "DrawPen.hpp"


// static data
IGBitmap UnderlineView::_bitmap( BMP_UNDERLINE );


UnderlineView::UnderlineView( UnderlineItem * subject, View * parent ):
  IconView( subject, parent )
{}


IString UnderlineView::info() const
{
  return IString("Underline ") + onOffString( attribute()->isEnabled() );
}


PlacedView::FormatChange UnderlineView::format( FormatPen & pen )
{
  FormatChange change = PlacedView::format( pen );
  pen.setUnderline( attribute()->isEnabled() );
  return change;
}


void UnderlineView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.setUnderline( attribute()->isEnabled() );
  IconView::draw( pen, zone );
}


UnderlineItem * UnderlineView::attribute() const
{
  return (UnderlineItem *) subject();
}

