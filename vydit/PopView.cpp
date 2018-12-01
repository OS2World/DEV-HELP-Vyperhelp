/***************************************************************************
 * File...... PopView.cpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Implementation of PopView.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString
#include <iexcept.hpp>      // ITRACE macros

// TextEditor
#include "Item.hpp"
#include "FlowView.hpp"
#include "ViewCursor.hpp"
#include "FormatPen.hpp"
#include "DrawPen.hpp"
#include "PushView.hpp"
#include "PopItem.hpp"
#include "PopView.hpp"


// static data
IGBitmap PopView::_bitmap( BMP_POP );


PopView::PopView( PopItem * subject, View * parent ):
  IconView( subject, parent )
{
  // assume that buddy PushView is already in place, search for it
  // assume the parent is a LineView and "grandparent" is a Flow
  FlowView * flow = (FlowView *) parent->parent();
  ViewCursor * cursor = flow->newViewCursor();
  IASSERTSTATE( cursor );
  forCursor( *cursor )
  {
    PlacedView * view = (PlacedView *) cursor->view();
    IASSERTSTATE( view );
    if ( view->subject() == (Item *) subject->buddy() )
    {
      PushView * push = (PushView *) view;
      _push = push;
      push->setPop( this );
      break;
    }
  }
  IASSERTSTATE( cursor->isValid() );
  delete cursor;
}


IString PopView::info() const
{
  return IString("End group");
}


// don't allow deletion, but delete cursors if necessary
unsigned PopView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  // delete cursors if they exist
  delete fromCursor;
  delete toCursor;

  return 0;
}


void PopView::generate( View * from, View * to, unsigned & level )
{
  if ( level )
  {
    PlacedView::generate( from, to, level );
    level--;
  }
}


PlacedView::FormatChange PopView::format( FormatPen & pen )
{
  // do default formatting
  FormatChange change = IconView::format( pen );

  // reset pen state to match buddy's
  if ( _push )
    pen.setState( _push->state() );

  return change;
}


void PopView::draw( DrawPen & pen, const IRectangle & zone )
{
  // reset pen state to match buddy's
  if ( _push )
    pen.setState( _push->state() );

  // draw the icon
  IconView::draw( pen, zone );
}


IString PopView::dumpString() const
{
  return debugString( "PopView" );
}


