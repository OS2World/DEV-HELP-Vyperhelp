/***************************************************************************
 * File...... NullViewCursor.cpp
 * Author.... Mat
 * Date...... 2/2/96
 *
 * Implementation of NullViewCursor
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <ibexcept.h>

// TextEditor
#include "NullViewCursor.hpp"


void NullViewCursor::accessError() const
{
  ICursorInvalidException::throwIt( IInvalidCursorText, __FILE__, __LINE__ );
}


View * NullViewCursor::view()
{
  accessError();
  return NULL;
}


const View * NullViewCursor::view() const
{
  accessError();
  return NULL;
}


