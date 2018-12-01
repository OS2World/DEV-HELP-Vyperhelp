/***************************************************************************
 * File...... IconView.cpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * Implementation of IconView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "DrawPen.hpp"
#include "IconView.hpp"


IconView::IconView( Item * subject, View * parent ):
  PlacedView( subject, parent )
{}


void IconView::draw( DrawPen & pen, const IRectangle & zone )
{
  pen.drawBitmap( bitmap().handle() );
  // default will clear unclaimed area and move pan ahead
  View::draw( pen, zone );
}


Coord IconView::height() const
{
  return bitmap().size().height();
}


Coord IconView::width() const
{
  return bitmap().size().width();
}


