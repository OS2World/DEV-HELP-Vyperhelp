/***************************************************************************
 * File...... BoldView.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * BoldView is an icon which represents a change to a bold text attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef BOLDVIEW_HPP
#define BOLDVIEW_HPP

// TextEditor
#include "IconView.hpp"
class BoldItem;


class BoldView: public IconView
{
  public:
    // constructor
    BoldView( BoldItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void draw( DrawPen & pen, const IRectangle & zone );

  private:
    // specialized subject access
    BoldItem * attribute() const;

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "BoldView.ipp"


#endif

