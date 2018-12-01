/***************************************************************************
 * File...... PushView.hpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * View of a PushItem, which represents the beginning of a group.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PUSHVIEW_HPP
#define PUSHVIEW_HPP

// TextEditor
#include "IconView.hpp"
class ViewCursor;
class PopView;


class PushView: public IconView
{
  public:
    // constructor
    PushView( Item * subject, View * parent );

    // link to pop
    PopView * pop() const;
    void      setPop( PopView * pop );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString info() const;

    // from View
    virtual unsigned remove( ViewCursor * fromCursor, ViewCursor * toCursor );
    virtual void     generate( View * from, View * to, unsigned & level );
    virtual IString  dumpString() const;

  private:
    PopView * _pop;  // NULL if no link setup

    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "PushView.ipp"


#endif

