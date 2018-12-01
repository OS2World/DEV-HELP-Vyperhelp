/***************************************************************************
 * File...... ArtView.hpp
 * Author.... Mat
 * Date...... 9/28/99
 *
 * View for a user bitmap (art).
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef ARTVIEW_HPP
#define ARTVIEW_HPP

// Editor
#include "IconView.hpp"
class Pen;
class ArtItem;


class ArtView: public IconView
{
  public:
    // constructor
    ArtView( ArtItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString info() const;

    // from View (override when no bitmap is available)
    virtual void  draw( DrawPen & pen, const IRectangle & zone );
    virtual Coord height() const;
    virtual Coord width() const;

  private:
    // specialized subject access
    ArtItem * attribute() const;
};


#endif

