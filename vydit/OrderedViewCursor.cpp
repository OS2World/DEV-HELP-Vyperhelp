/***************************************************************************
 * File...... OrderedViewCursor.cpp
 * Author.... Mat
 * Date...... 4/30/96
 *
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "OrderedViewCursor.hpp"


// constructor from Sequence
OrderedViewCursor::OrderedViewCursor( const ViewSequence & sequence ):
  _sequence( sequence )
{
  _cursor = sequence.newCursor();
}


// copy constructor
OrderedViewCursor::OrderedViewCursor( const OrderedViewCursor & cursor ):
  _sequence( cursor._sequence )
{
  _cursor = _sequence.newCursor();
  _cursor->copy( *cursor.cursor() );
}


OrderedViewCursor::~OrderedViewCursor()
{
  delete _cursor;
}


OrderedViewCursor & OrderedViewCursor::operator=( const OrderedViewCursor & cursor )
{
  _cursor->copy( *cursor.cursor() );
  return *this;
}


IBoolean OrderedViewCursor::isFirst() const
{
  OrderedViewCursor cFirst( *this );
  cFirst.setToFirst();
  return ( cFirst == *this );
}


IBoolean OrderedViewCursor::isLast() const
{
  OrderedViewCursor cLast( *this );
  cLast.setToLast();
  return ( cLast == *this );
}


IBoolean OrderedViewCursor::locate( const View * view )
{
  // IEqualitySequence can't promise not to modify the View being pointed to,
  // so we have to cheat and create a non-const View* to use for the locate.
  // This assumes that locate() will not actually change the View.
  View * nonConstView = (View *) view;
  return _sequence.locate( nonConstView, *_cursor );
}



