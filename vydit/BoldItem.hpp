/***************************************************************************
 * File...... BoldItem.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * BoldItem represents a change in bold text attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef BOLDITEM_HPP
#define BOLDITEM_HPP


// OpenClass
#include <istring.hpp>

// TextEditor
#include "Item.hpp"


class _Export BoldItem: public Item
{
  public:
    // constructor
    BoldItem( Item * parent, Boolean isEnabled );

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
#include "BoldItem.ipp"


#endif

