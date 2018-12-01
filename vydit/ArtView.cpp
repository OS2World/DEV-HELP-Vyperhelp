/***************************************************************************
 * File...... ArtView.cpp
 * Author.... Mat
 * Date...... 9/28/99
 *
 * Implementation of ArtView
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// TextEditor
#include "ArtView.hpp"
#include "ArtItem.hpp"


ArtView::ArtView( ArtItem * subject, View * parent ):
  IconView( subject, parent )
{}


// IconView is used for drawing and sizing, unless a bitmap is not available.
// If a bitmap is not available,
void ArtView::draw( DrawPen & pen, const IRectangle & zone )
{
  if ( attribute()->bitmap() )
  {
    IconView::draw( pen, zone );
  }
  else
  {
    // default will clear unclaimed area and move pen ahead
    View::draw( pen, zone );
  }
}


Coord ArtView::height() const
{
  return attribute()->bitmap()? IconView::height(): 0;
}


Coord ArtView::width() const
{
  // insert blank space if no bitmap is available
  return attribute()->bitmap()? IconView::width(): state().charWidth( ' ' );
}


const IGBitmap & ArtView::bitmap() const
{
  return *attribute()->bitmap();
}


IString ArtView::info() const
{
  return attribute()->id();
}


ArtItem * ArtView::attribute() const
{
  return (ArtItem *) subject();
}



