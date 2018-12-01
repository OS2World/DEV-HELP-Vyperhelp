/***************************************************************************
 * File...... FontView.hpp
 * Author.... Mat
 * Date...... 2/4/97
 *
 * FontView is the view of a FontItem, which represents a change in the
 * current font.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef FONTVIEW_HPP
#define FONTVIEW_HPP

// TextEditor
#include "IconView.hpp"
class FontItem;


class FontView: public IconView
{
  public:
    // constructor
    FontView( FontItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void draw( DrawPen & pen, const IRectangle & zone );

  private:
    // specialized access to subject
    FontItem * font() const;

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "FontView.ipp"


#endif

