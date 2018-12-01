/***************************************************************************
 * File...... SymbolItem.hpp
 * Author.... Mat
 * Date...... 4/1/97
 *
 * Item support for symbols (typographical characters which aren't part of
 * words).
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef SYMBOLITEM_HPP
#define SYMBOLITEM_HPP

// Generator
#include "Symbol.hpp"

// TextEditor
#include "Item.hpp"


class _Export SymbolItem: public Item
{
  public:
    // constructor
    SymbolItem( const Symbol & symbol, Item * parent );

    // accessor
    const Symbol & symbol() const;

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;

  private:
    Symbol _symbol;
};


// inline functions
#include "SymbolItem.ipp"


#endif

