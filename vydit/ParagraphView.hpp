/***************************************************************************
 * File...... ParagraphView.hpp
 * Author.... Mat
 *
 * View for the ParagraphItem, which represents the start of a new paragraph.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PARAGRAPHVIEW_HPP
#define PARAGRAPHVIEW_HPP

// TextEditor
#include "IconView.hpp"


class ParagraphView: public IconView
{
  public:
    // constructor
    ParagraphView( Item * subject, View * parent );

    // from IconView
    virtual const IGBitmap & bitmap() const;

    // from View
    virtual LineMode lineMode() const;
    virtual FitMode  fitMode() const;
    virtual Coord    descent() const;
    virtual void     draw( DrawPen & pen, const IRectangle & zone );
    virtual IString  dumpString() const;

  private:
    // static data
    static IGBitmap _bitmap;
};


// inline functions
#include "ParagraphView.ipp"


#endif

