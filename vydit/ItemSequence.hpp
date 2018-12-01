/***************************************************************************
 * File...... ItemSequence.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * Cursor for items nested within a FlowItem.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ITEMSEQUENCE_HPP
#define ITEMSEQUENCE_HPP


// OpenClass
#include <ieqseq.h>         // IEqualitySequence

// Editor
class Item;
typedef IEqualitySequence<Item*> ItemSequenceBase;


class _Export ItemSequence: public ItemSequenceBase
{
  public:
    // constructor
    ItemSequence( INumber numberOfElements = 500 );
    ~ItemSequence();

    // delete and remove
    void deleteAndRemove( IOrderedCursor<Item*> & cursor );
};


#endif

