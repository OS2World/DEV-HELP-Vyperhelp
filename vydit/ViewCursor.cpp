/***************************************************************************
 * File...... ViewCursor.cpp
 * Author.... Mat
 * Date...... 5/9/96
 *
 * Partial implementation of ViewCursor.  Most functions are pure virtual.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#include "ViewCursor.hpp"


// needed to be declared virtual
ViewCursor::~ViewCursor()
{}


IBoolean ViewCursor::locate( const View * viewToFind )
{
  setToFirst();
  while ( isValid() ) {
    if ( view() == viewToFind ) {
      return true;
    } /* endif */
    setToNext();
  } /* endwhile */

  return false;
}


