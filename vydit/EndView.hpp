/***************************************************************************
 * File...... EndView.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * View for EndItem.  Indicates to the user the end of a FlowView.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ENDVIEW_HPP
#define ENDVIEW_HPP

// TextEditor
#include "IconView.hpp"


class EndView: public IconView
{
  public:
    // constructor
    EndView( Item * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from PlacedView
    virtual IString info() const;

    // from View
    virtual LineMode lineMode() const;
    virtual FitMode  fitMode() const;
    virtual IString  dumpString() const;

private:
    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "EndView.ipp"


#endif

