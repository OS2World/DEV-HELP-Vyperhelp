/***************************************************************************
 * File...... EndItem.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * EndItem is always the last item in a FlowItem collection.  This ensures
 * that the FlowItem is never empty, and the user always has at least one
 * item to manipulate.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ENDITEM_HPP
#define ENDITEM_HPP


// TextEditor
#include "Item.hpp"


class EndItem: public Item
{
  public:
    // constructor
    EndItem( Item * parent );

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;
};


// inline functions
#include "EndItem.ipp"


#endif
