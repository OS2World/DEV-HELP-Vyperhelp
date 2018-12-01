/***************************************************************************
 * File...... NewLineItem.hpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * NewLineItem represents a forced line-feed.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef NEWLINEITEM_HPP
#define NEWLINEITEM_HPP


// TextEditor
#include "Item.hpp"


class _Export NewLineItem: public Item
{
  public:
    // constructor
    NewLineItem( Item * parent );

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;
};



#endif

