/***************************************************************************
 * File...... ItemSequence.cpp
 * Author.... Mat
 * Date...... 9/12/96
 *
 * Implements ItemSequence.  Adds support for deleting Items before their
 * pointers are removed
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "Item.hpp"
#include "ItemSequence.hpp"


ItemSequence::ItemSequence( INumber numberOfElements ):
    ItemSequenceBase( numberOfElements )
{}


ItemSequence::~ItemSequence()
{
  // don't use built in iterator such as allElementsDo()
  // Item destructors will remove themselves from the collection
  INumber num = numberOfElements();
  while ( num-- )
  {
    Item * item = firstElement();
    delete item;
  }
}


void ItemSequence::deleteAndRemove( IOrderedCursor<Item*> & cursor )
{
  delete cursor.element();
  ItemSequenceBase::removeAt( cursor );
}

