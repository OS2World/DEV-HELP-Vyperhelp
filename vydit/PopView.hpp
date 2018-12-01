/***************************************************************************
 * File...... PopView.hpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * View of a PopItem, which represents the end of a group.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef POPVIEW_HPP
#define POPVIEW_HPP

// TextEditor
#include "IconView.hpp"
class ViewCursor;
class Pen;
class PushView;
class PopItem;
class FlowView;


class PopView: public IconView
{
  public:
    // constructor
    PopView( PopItem * subject, View * parent );

    // buddy PushView
    PushView * push() const;
    void       setOrphan();

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString      info() const;
    virtual FormatChange format( FormatPen & pen );

    // from View
    virtual void     draw( DrawPen & pen, const IRectangle & zone );
    virtual unsigned remove( ViewCursor * fromCursor, ViewCursor * toCursor );
    virtual void     generate( View * from, View * to, unsigned & level );
    virtual Boolean  isOrphan() const;
    virtual IString  dumpString() const;

  private:
    PushView * _push;  // NULL if no link setup

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "PopView.ipp"


#endif

