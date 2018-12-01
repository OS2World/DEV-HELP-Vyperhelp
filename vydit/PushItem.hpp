/***************************************************************************
 * File...... PushItem.hpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Represents the start of a group.  Groups are used for nesting control
 * and push/pop control of attributes.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PUSHITEM_HPP
#define PUSHITEM_HPP


// TextEditor
#include "Item.hpp"


class _Export PushItem: public Item
{
  public:
    // constructor
    PushItem( Item * parent );

    // from Item
    virtual PushItem * asPush();
    virtual View *     newView( View * parent );
    virtual void       generate();
    virtual IString    dumpString() const;
};


// inline functions
#include "PushItem.ipp"


#endif

