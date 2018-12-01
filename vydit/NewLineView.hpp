/***************************************************************************
 * File...... NewLineView.hpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * View for the NewLineItem, which represents a forced line-feed.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef NEWLINEVIEW_HPP
#define NEWLINEVIEW_HPP

// TextEditor
#include "IconView.hpp"


class NewLineView: public IconView
{
  public:
    // constructor
    NewLineView( Item * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from View
    virtual LineMode lineMode() const;
    virtual FitMode  fitMode() const;
    virtual IString  dumpString() const;

  private:
    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "NewLineView.ipp"


#endif

