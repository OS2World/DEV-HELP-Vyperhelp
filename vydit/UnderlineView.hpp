/***************************************************************************
 * File...... UnderlineView.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * UnderlineView is an icon which represents underline text attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef UNDERLINEVIEW_HPP
#define UNDERLINEVIEW_HPP

// TextEditor
#include "IconView.hpp"
class UnderlineItem;


class UnderlineView: public IconView
{
  public:
    // constructor
    UnderlineView( UnderlineItem * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PLacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void draw( DrawPen & pen, const IRectangle & zone );

  private:
    // specialized subject access
    UnderlineItem * attribute() const;

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "UnderlineView.ipp"


#endif

