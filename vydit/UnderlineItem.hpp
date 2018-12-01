/***************************************************************************
 * File...... UnderlineItem.hpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * UnderlineItem represents a change in underline text attribute.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef UNDERLINEITEM_HPP
#define UNDERLINEITEM_HPP


// OpenClass
#include <istring.hpp>

// TextEditor
#include "Item.hpp"


class _Export UnderlineItem: public Item
{
  public:
    // constructor
    UnderlineItem( Item * parent, Boolean isEnabled );

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
#include "UnderlineItem.ipp"


#endif

