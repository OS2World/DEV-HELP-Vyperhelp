/***************************************************************************
 * File...... SpaceItem.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * SpaceItem represents a space character.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef SPACEITEM_HPP
#define SPACEITEM_HPP


// TextEditor
#include "Item.hpp"


class _Export SpaceItem: public Item
{
  public:
    // constructor
    SpaceItem( Item * parent );

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;
};


#endif
