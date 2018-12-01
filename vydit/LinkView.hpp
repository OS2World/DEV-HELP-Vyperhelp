/***************************************************************************
 * File...... LinkView.hpp
 * Author.... Mat
 *
 * LinkView is an icon which represents a hypertext link on/off.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef LINKVIEW_HPP
#define LINKVIEW_HPP

// OpenClass
#include <icolor.hpp>

// Editor
#include "IconView.hpp"
class Pen;
class LinkItem;


class LinkView: public IconView
{
  public:
    // constructor
    LinkView( LinkItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void draw( DrawPen & pen, const IRectangle & zone );

  private:
    // specialized subject access
    LinkItem * attribute() const;

    // helper
    void setColor( Pen & pen );

    // static data
    static IGBitmap _bitmap;
    static IColor   _color;
};


// inline functions
#include "LinkView.ipp"


#endif

