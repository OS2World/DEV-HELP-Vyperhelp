/***************************************************************************
 * File...... LineEndView.cpp
 * Author.... Mat
 * Date...... 5/9/96
 *
 * Implements the LineEndView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <iexcept.hpp>      // IASSERT macros
#include <istring.hpp>      // IString

// TextEditor
#include "LineEndView.hpp"


// static data
IGBitmap LineEndView::_bitmap( BMP_LINEEND );


LineEndView::LineEndView( View * parent ):
  IconView( NULL, parent )
{}


IString LineEndView::dumpString() const
{
  return debugString( "LineEndView" );
}

