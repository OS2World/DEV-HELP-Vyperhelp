/***************************************************************************
 * File...... ItalicItem.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * ItalicItem represents a change in italic test attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef ITALICITEM_HPP
#define ITALICITEM_HPP


// OpenClass
#include <istring.hpp>

// TextEditor
#include "Item.hpp"


class _Export ItalicItem: public Item
{
  public:
    // constructor
    ItalicItem( Item * parent, Boolean isEnabled );

    // accessor
    Boolean isEnabled() const;

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;

  private:
    Boolean _isEnabled;
};


// inline functions
#include "ItalicItem.ipp"


#endif

