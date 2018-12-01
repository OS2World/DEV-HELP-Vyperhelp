/***************************************************************************
 * File...... SymbolView.hpp
 * Author.... Mat
 * Date...... 4/1/97
 *
 * View of a symbol.  Symbols are any typographic characters not part of
 * words.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef SYMBOLVIEW_HPP
#define SYMBOLVIEW_HPP

// TextEditor
#include "PlacedView.hpp"
class SymbolItem;
class Symbol;


class SymbolView: public PlacedView
{
  public:
    // constructor
    SymbolView( SymbolItem * subject, View * parent );

    // from PlacedView
    virtual IString                  info() const;
    virtual PlacedView::FormatChange format( FormatPen & pen );
    virtual PlacedView::LineMode     lineMode() const;

    // from View
    virtual void    draw( DrawPen & pen, const IRectangle & zone );
    virtual Coord   width() const;
    virtual Coord   height() const;
    virtual Coord   descent() const;
    virtual IString dumpString() const;

  private:
    // access to Symbol
    const Symbol & symbol() const;
};


// inline functions
#include "SymbolView.ipp"


#endif

