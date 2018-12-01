/***************************************************************************
 * File...... LineEndView.hpp
 * Author.... Mat
 * Date...... 5/9/96
 *
 * LineEndView is a "dummy" view that provides a placeholder for the Kursor
 * when positioned at the end of the line.  It always has a LineView as
 * a parent.  LineEndViews should not be used on the last line (where
 * there is already an EndView)
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef LINEENDVIEW_HPP
#define LINEENDVIEW_HPP

// TextEditor
#include "IconView.hpp"


class LineEndView: public IconView
{
  public:
    // constructor
    LineEndView( View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from View
    virtual Boolean  isTransient() const;
    virtual void     removeSelf();
    virtual LineMode lineMode() const;
    virtual FitMode  fitMode() const;
    virtual IString  dumpString() const;

private:
    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "LineEndView.ipp"


#endif

