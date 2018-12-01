/***************************************************************************
 * File...... EndView.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of EndView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <istring.hpp>      // IString

// TextEditor
#include "EndView.hpp"


// static data
IGBitmap EndView::_bitmap( BMP_END );


EndView::EndView( Item * subject, View * parent ):
  IconView( subject, parent )
{}


IString EndView::info() const
{
  return IString("End of text");
}


IString EndView::dumpString() const
{
  return debugString( "EndView" );
}

