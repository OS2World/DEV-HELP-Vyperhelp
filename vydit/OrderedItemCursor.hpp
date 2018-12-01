/***************************************************************************
 * File...... OrderedItemCursor.hpp
 * Author.... Mat
 * Date...... 4/30/96
 *
 * ItemCursor for an ordered collection.  Wraps an OpenClass cursor (see
 * the "Decorator" pattern in Design Patterns).
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ORDEREDITEMCURSOR_HPP
#define ORDEREDITEMCURSOR_HPP


// OpenClass
#include <icursor.h>

// TextEditor
#include "Item.hpp"
#include "ItemSequence.hpp"
#include "ItemCursor.hpp"


class _Export OrderedItemCursor: public ItemCursor
{
  public:
    // constructor
    OrderedItemCursor( const ItemSequence & sequence );
    OrderedItemCursor( const OrderedItemCursor & cursor );
    virtual ~OrderedItemCursor();

    // copy assignment
    OrderedItemCursor & operator=( const OrderedItemCursor & cursor );

    // cursor access
    IOrderedCursor<Item*> * cursor();
    const IOrderedCursor<Item*> * cursor() const;

    // element access
    virtual Item * item() const;

    // ala IOrderedCursor
    virtual IBoolean setToLast ();
    virtual IBoolean setToPrevious ();

    // ala ICursor
    virtual IBoolean setToFirst();
    virtual IBoolean setToNext();
    virtual IBoolean isValid() const;
    virtual void invalidate();

  private:
    const ItemSequence      & _sequence;
    IOrderedCursor<Item*> *   _cursor;
};


// inline functions
#include "OrderedItemCursor.ipp"


#endif

