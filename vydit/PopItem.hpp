/***************************************************************************
 * File...... PopItem.hpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Represents the end of a group.  Groups are used for nesting control
 * and push/pop control of attributes.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef POPITEM_HPP
#define POPITEM_HPP


// TextEditor
#include "Item.hpp"
class PushItem;


class _Export PopItem: public Item
{
  public:
    // constructor
    PopItem( Item * parent, PushItem * buddy );

    // linking support
    PushItem * buddy();

    // from Item
    virtual PopItem * asPop();
    virtual View *    newView( View * parent );
    virtual void      generate();
    virtual IString   dumpString() const;

  private:
    PushItem * _buddy;
};


// inline functions
#include "PopItem.ipp"


#endif

