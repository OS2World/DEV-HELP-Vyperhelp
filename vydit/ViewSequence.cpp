/***************************************************************************
 * File...... ViewSequence.cpp
 * Author.... Mat
 * Date...... 9/12/96
 *
 * Implements ViewSequence, adding functions to delete Views as they are
 * removed from the sequence.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "View.hpp"
#include "ViewSequence.hpp"

// local functions
static IBoolean deleteView( View * & view, void * nothing );


ViewSequence::ViewSequence( INumber numberOfElements ):
    ViewSequenceBase( numberOfElements )
{}


ViewSequence::~ViewSequence()
{
  allElementsDo( deleteView );
}


static IBoolean deleteView( View * & view, void * nothing )
{
  delete view;
  return true;
}


void ViewSequence::deleteAndRemove( IOrderedCursor<View*> & cursor )
{
  delete cursor.element();
  ViewSequenceBase::removeAt( cursor );
}


int ViewSequence::order( View & view1, View & view2 )
{
  // if both views refer to the same, they are equal!
  if ( &view1 == &view2 )
    return 0;

  // locate each line in the collection and record position
  Cursor cursor( *this );
  Boolean found = locate( &view1, cursor );
  IASSERTSTATE( found );
  IPosition position1 = position( cursor );
  found = locate( &view2, cursor );
  IASSERTSTATE( found );
  IPosition position2 = position( cursor );

  // return result of comparison
  return (int)position1 - (int)position2;
}


