/***************************************************************************
 * File...... IconView.hpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * Abstract class which supports display of an icon or bitmap to represent
 * a control Item.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ICONVIEW_HPP
#define ICONVIEW_HPP

// OpenClass
#include <igbitmap.hpp>     // IGBitmap

// TextEditor
#include "vydit.h"          // for BMP resource defines
#include "PlacedView.hpp"


class IconView: public PlacedView
{
  public:
    // constructor
    IconView( Item * subject, View * parent );

    // bitmap provided by subclass
    virtual const IGBitmap & bitmap() const = 0;

    // from View
    virtual void  draw( DrawPen & pen, const IRectangle & zone );
    virtual Coord height() const;
    virtual Coord width() const;
};


#endif

