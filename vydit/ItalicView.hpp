/***************************************************************************
 * File...... ItalicView.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * ItalicView is an icon which represents a change to italic text attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef ITALICVIEW_HPP
#define ITALICVIEW_HPP

// TextEditor
#include "IconView.hpp"
class ItalicItem;


class ItalicView: public IconView
{
  public:
    // constructor
    ItalicView( ItalicItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void draw( DrawPen & pen, const IRectangle & zone );

  private:
    // specialized subject access
    ItalicItem * attribute() const;

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "ItalicView.ipp"


#endif

