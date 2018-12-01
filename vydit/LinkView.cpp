/***************************************************************************
 * File...... LinkView.cpp
 * Author.... Mat
 *
 * Implements the LinkView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "LinkView.hpp"
#include "LinkItem.hpp"
#include "DrawPen.hpp"
#include "FormatPen.hpp"


// static data
IGBitmap LinkView::_bitmap( BMP_LINK );
IColor   LinkView::_color( IColor::blue );


LinkView::LinkView( LinkItem * subject, View * parent ):
  IconView( subject, parent )
{}


void LinkView::setColor( Pen & pen )
{
  if ( attribute()->gin().isOn() )
    pen.setColor( &_color );
  else
    pen.setColor();
}


IString LinkView::info() const
{
  const LinkGin & gin = attribute()->gin();
  IString result;
  if ( gin.target().length() )
  {
    result = IString("Link to ") + gin.target();
    if ( gin.isExternal() )
      result += IString(" in ") + gin.file();
  }
  else
  {
    result = "End link";
  }
  return result;
}


PlacedView::FormatChange LinkView::format( FormatPen & pen )
{
  FormatChange change = PlacedView::format( pen );
  setColor( pen );
  return change;
}


void LinkView::draw( DrawPen & pen, const IRectangle & zone )
{
  setColor( pen );
  IconView::draw( pen, zone );
}


LinkItem * LinkView::attribute() const
{
  return (LinkItem *) subject();
}


