/***************************************************************************
 * File...... View.cpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * Default implementation for virtual View class
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "DrawPen.hpp"
#include "NullViewCursor.hpp"
#include "View.hpp"


View::View( View * parent ):
    _parent( parent )
{}


// need to declare virtual destructor for inheritance
View::~View()
{}


ViewCursor * View::newCursor()
{
  return new NullViewCursor;
}


// return pointer to child which is in the tree of the given descendent
// 0 if thie given view is not a descendent
View * View::locateDescendent( View & descendent )
{
  View * view = &descendent;

  do
  {
    View * parent = view->parent();
    if ( parent == this )
      return view;
    view = parent;  // move up the tree
  } while ( view );

  return 0;
}


// recursively search for the first or last leaf, and return a new cursor to it
ViewCursor * View::newLeafCursor( const Boolean wantFirst )
{
  ViewCursor * leafCursor = 0;
  ViewCursor * viewCursor = newCursor();
  // for loop scans through all children either forward or backwards
  for (
    wantFirst? viewCursor->setToFirst(): viewCursor->setToLast();
    viewCursor->isValid() && ! leafCursor;
    wantFirst? viewCursor->setToNext(): viewCursor->setToPrevious() )
  {
    leafCursor = viewCursor->view()->newLeafCursor( wantFirst );
  }

  delete viewCursor;
  return leafCursor;
}


// search for the line on which this View resides
// recursively call up the tree, return 0 if at top
ViewCursor * View::newLineCursor()
{
  View * view = parent();
  return view? view->newLineCursor(): 0;
}


void View::removeSelf()
{
  if ( _parent )
    _parent->removeChild( this );
  delete this;
}


void View::update( View & childView )
{
  View * parentView = parent();
  IASSERTSTATE( parentView );
  parentView->update( *this );
}


// default clears above and below view, then advances pen
void View::draw( DrawPen & pen, const IRectangle & rect )
{
  pen.clear( width(), height(), descent() );
  pen.forward( width() );
}


// default correlation works for Views with no sub-Views
// (but not the top-level view)
// if correlation x-point is past the halfway point, pick the next view
// (if one is available)
// the next view may have sub-view, so need to call its correlate routine
ViewCursor * View::newCorrelateCursor( IPoint & point )
{
  // get a cursor to this view
  ViewCursor * viewCursor = newCursorToThis();
  IASSERTSTATE( viewCursor );
  ViewCursor * result = viewCursor;

  // default action is to determine which side
  if ( point.x() > width() / 2 )
  {
    if ( ! viewCursor->isLast() )
    {
      // get new cursor from the next (adjacent) View
      viewCursor->setToNext();
      View * view = viewCursor->view();
      IASSERTSTATE( view );
      // adjust point so that it is at next view's x=0
      IPoint next = IPoint( 0, point.y() );
      result = view->newCorrelateCursor( next );
      IASSERTSTATE( result );
      delete viewCursor;
    }
  }

  return result;
}


Coord View::extent() const
{
  return height() + descent();
}


ISize View::size() const
{
  return ISize( width(), extent() );
}


IRectangle View::rect() const
{
  return IRectangle( position(), size() );
}


Coord View::fullExtent()
{
  ViewCursor * cursor = newLineCursor();
  IASSERTSTATE( cursor );
  Coord lineExtent = cursor->view()->extent();
  delete cursor;
  return lineExtent;
}


// returns the position of the View in the top-level view's space
IPoint View::fullPosition() const
{
  IPoint position( 0, 0 );
  const View * view = this;

  // search up the tree to accumulate x,y position
  while ( view->parent() )
  {
    position += view->position();
    view = view->parent();
  }

  return position;
}


IRectangle View::fullRect()
{
  return IRectangle( fullPosition(), ISize( width(), fullExtent() ) );
}


// get a cursor to this view, using cursor supplied by parent
// return 0 if the View can't be cursored
ViewCursor * View::newCursorToThis()
{
  View * parentView = parent();
  if ( ! parentView )
    return 0;
  ViewCursor * viewCursor = parentView->newCursor();
  Boolean found = viewCursor->locate( this );
  if ( found )
  {
    return viewCursor;
  }
  else
  {
    delete viewCursor;
    return 0;
  }
}


unsigned View::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  // delete cursors if they exist
  delete fromCursor;
  delete toCursor;

  // delete this View (no subject Item to worry about)
  removeSelf();

  // no view was orphaned
  return 0;
}


IString View::debugString( const IString & name ) const
{
  return name
       + IString(" [")
       + IString((unsigned)this).d2x()
       + IString("]");
}


