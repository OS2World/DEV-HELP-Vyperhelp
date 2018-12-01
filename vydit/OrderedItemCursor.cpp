/***************************************************************************
 * File...... OrderedItemCursor.cpp
 * Author.... Mat
 * Date...... 4/30/96
 *
 * Implementation for OrderedItemCursor
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#include "OrderedItemCursor.hpp"


// constructor from Sequence
OrderedItemCursor::OrderedItemCursor( const ItemSequence & sequence ):
  _sequence( sequence )
{
  _cursor = sequence.newCursor();
}


// copy constructor
OrderedItemCursor::OrderedItemCursor( const OrderedItemCursor & cursor ):
  _sequence( cursor._sequence )
{
  _cursor = _sequence.newCursor();
  _cursor->copy( *cursor.cursor() );
}


OrderedItemCursor::~OrderedItemCursor()
{
  delete _cursor;
}


OrderedItemCursor & OrderedItemCursor::operator=( const OrderedItemCursor & cursor )
{
  _cursor->copy( *cursor.cursor() );
  return *this;
}


