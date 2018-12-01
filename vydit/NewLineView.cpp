/***************************************************************************
 * File...... NewLineView.cpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * Implementation of NewLineView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString


// TextEditor
#include "NewLineView.hpp"


// static data
IGBitmap NewLineView::_bitmap( BMP_NEWLINE );


NewLineView::NewLineView( Item * subject, View * parent ):
  IconView( subject, parent )
{}


IString NewLineView::dumpString() const
{
  return debugString( "NewLineView" );
}


