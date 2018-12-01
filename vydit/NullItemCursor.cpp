/***************************************************************************
 * File...... NullItemCursor.cpp
 * Author.... Mat
 * Date...... 2/2/96
 *
 * Default cursor for items which don't support nesting
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <ibexcept.h>

// TextEditor
#include "NullItemCursor.hpp"


void NullItemCursor::accessError() const
{
  ICursorInvalidException::throwIt( IInvalidCursorText, __FILE__, __LINE__ );
}


Item * NullItemCursor::item() const
{
  accessError();
  return NULL;
}


