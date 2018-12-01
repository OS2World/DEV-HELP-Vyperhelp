/***************************************************************************
 * File...... PushView.cpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Implementation of PushView.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString

// TextEditor
#include "Item.hpp"
#include "ViewCursor.hpp"
#include "PopView.hpp"
#include "PushView.hpp"


// static data
IGBitmap PushView::_bitmap( BMP_PUSH );


PushView::PushView( Item * subject, View * parent ):
  IconView( subject, parent ),
  _pop( NULL )
{}


IString PushView::info() const
{
  return IString("Begin group");
}


unsigned PushView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  Boolean orphanCount = 0;

  // orphan the paired PopItem/View if it exists
  if ( _pop )
  {
    _pop->setOrphan();
    orphanCount = 1;
  }

  // remove self with default processing
  SubjectView::remove( fromCursor, toCursor );

  // return 1 if a PopView was left orphaned
  return orphanCount;
}


void PushView::generate( View * from, View * to, unsigned & level )
{
  PlacedView::generate( from, to, level );
  level++;
}


IString PushView::dumpString() const
{
  return debugString( "PushView" );
}


