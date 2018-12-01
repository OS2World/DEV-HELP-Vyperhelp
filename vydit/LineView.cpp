/***************************************************************************
 * File...... LineView.cpp
 * Author.... Mat
 * Date...... 5/9/96
 *
 * Implementation of LineView.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "DrawPen.hpp"
#include "Item.hpp"
#include "ItemCursor.hpp"
#include "LineSubviewCursor.hpp"
#include "NullViewCursor.hpp"
#include "FlowView.hpp"
#include "PlacedView.hpp"
#include "LineView.hpp"


LineView::LineView( FlowView * parent ):
    View( parent ),
    _firstView( 0 ),
    _lastView( 0 ),
    _width( 0 ),
    _height( 0 ),
    _descent( 0 ),
    _above( 0 ),
    _lineEnd( this ),
    _needLineEnd( true )
{}


LineView::~LineView()
{
  delete _firstView;
  delete _lastView;
}


ViewCursor * LineView::newCursor()
{
  return new LineSubviewCursor( *this, _needLineEnd? &_lineEnd: 0 );
}


ViewCursor * LineView::newLineCursor()
{
  return newCursorToThis();
}


LineView & LineView::setFirstView( const OrderedViewCursor * firstView, Boolean setLast )
{
  delete _firstView;

  if ( firstView )
  {
    _firstView = new OrderedViewCursor( *firstView );
    if ( setLast )
      setLastView( firstView );
  }
  else
  {
    _firstView = 0;
    if ( setLast )
    {
      delete _lastView;
      _lastView = 0;
    }
  }

  return *this;
}


// assumes the view pointed to has already been formatted
LineView & LineView::setLastView( const OrderedViewCursor * lastView )
{
  IASSERTPARM( lastView );
  delete _lastView;
  _lastView = new OrderedViewCursor( *lastView );
  PlacedView * placed = (PlacedView *) lastView->view();
  _needLineEnd = ( placed->lineMode() != PlacedView::forceNewLine );
  return *this;
}



/***************************************************************************
 * Procedure.. LineView::update
 * Author..... Mat
 * Date....... 7/31/96
 *
 * Called when a child view needs reformatting.  Calls up to FlowView,
 * which will invoke a FlowFormatter to re-format the LineView and
 * subsequenct lines if necessary.
 ***************************************************************************/
void LineView::update( View & childView )
{
  parent()->update( childView );
}


/***************************************************************************
 * Procedure.. LineView::draw
 * Author..... Mat
 * Date....... 10/7/97
 *
 * Draw every child View which is in the zone.
 ***************************************************************************/
void LineView::draw( DrawPen & pen, const IRectangle & zone )
{
  Boolean looking = true;
  ViewCursor * cursor = newCursor();
  forCursor( *cursor )
  {
    PlacedView * view = (PlacedView *) cursor->view();
    Coord maxX = view->position().x() + view->width();
    if ( maxX > zone.minX() )
    {
      if ( looking )
      {
        pen.start( *view );
        looking = false;
      }
      view->draw( pen, zone.movedBy( - view->position() ) );
    }
    if ( maxX > zone.maxX() )
      break;
  }
  delete cursor;
}


IPoint LineView::position() const
{
  // convert coordinate systems by inverting y
  return IPoint( 0, parent()->extent() - _above - _height - _descent );
}


/***************************************************************************
 * Procedure.. LineView::newCorrelateCursor
 * Author..... Mat
 * Date....... 5/16/96
 *
 * Correlate a point by locating the x-coordinate among the subViews
 ***************************************************************************/
ViewCursor * LineView::newCorrelateCursor( IPoint & point )
{
  ViewCursor * viewCursor = newCursor();
  Coord        x = point.x();

  // loop through looking for correct x-range
  viewCursor->setToFirst();
  IASSERTSTATE( viewCursor->isValid() );
  Boolean looking = true;
  View * view;
  do
  {
    view = viewCursor->view();
    if ( ! viewCursor->isLast() && ( x >= view->position().x() + view->width() ) )
    {
      // move to next sub-view
      viewCursor->setToNext();
    }
    else
    {
      looking = false;
    }
  } while ( looking );

  // get correlation point from sub-view
  IPoint relativePoint = point - view->position();
  ViewCursor * result = view->newCorrelateCursor( relativePoint );

  // clean-up and return result
  delete viewCursor;
  return result;
}


void LineView::generate( View * from, View * to, unsigned & level )
{
  View *       first;
  View *       last;
  ViewCursor * cursor = newCursor();
  Boolean      ok;

  // locate first view
  if ( from )
  {
    // start at the specified cursor
    first = locateDescendent( *from );
    IASSERTSTATE( first );
    ok = cursor->locate( first );
    IASSERTSTATE( ok );
  }
  else
  {
    first = 0;
    ok = cursor->setToFirst();
  }

  // locate last view
  if ( to )
  {
    // end at the specified cursor
    last = locateDescendent( *to );
    IASSERTSTATE( last );
  }
  else
  {
    last = 0;
  }

  // use cursor to iterate through lines
  View * view;
  do
  {
    // get the next view
    view = cursor->view();

    // remove a portion or all of the sub-view
    view->generate(
        view == first? from: 0,
        view == last? to: 0,
        level );

    // advance the cursor
    if ( view == last )
      ok = false;
    else
      ok = cursor->setToNext();
  }
  while ( ok );

  // delete the cursor
  delete cursor;
}


/***************************************************************************
 * Procedure.. LineView::remove
 * Author..... Mat
 * Date....... 11/12/96
 *
 * removes sub-views from fromCursor to toCursor.  fromCursor = 0 to
 * remove starting with the first view.  toCursor = 0 to remove up to the
 * last view.  The cursors are also deleted.
 *
 * The firstView and lastView cursors are also updated if they need to be.
 *
 * FlowFormatter will take care of deleting lines when necessary.
 ***************************************************************************/
unsigned LineView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  View *       first;
  View *       last;
  ViewCursor * cursor = newCursor();
  Boolean      ok;
  Boolean      orphanCount = 0;

  // locate first view
  if ( fromCursor )
  {
    // start at the specified cursor
    first = locateDescendent( *fromCursor->view() );
    IASSERTSTATE( first );
    ok = cursor->locate( first );
    IASSERTSTATE( ok );
  }
  else
  {
    first = 0;
    ok = cursor->setToFirst();
  }

  // locate last view
  if ( toCursor )
  {
    // end at the specified cursor
    last = locateDescendent( *toCursor->view() );
    IASSERTSTATE( last );
  }
  else
  {
    last = 0;
  }

  // use cursor to iterate through lines
  View * view;
  do
  {
    // get the next view
    view = cursor->view();

    // advance the cursor before calling remove() because remove()
    // may invalidate the cursor
    if ( view == last )
      ok = false;
    else
      ok = cursor->setToNext();

    // remove a portion or all of the sub-view
    orphanCount += view->remove(
        view == first? fromCursor: 0,
        view == last? toCursor: 0 );
  }
  while ( ok );

  // delete the cursor
  delete cursor;

  return orphanCount;
}


FlowView * LineView::flow() const
{
  return (FlowView *) parent();
}


// need to update the LineView's first/last pointers if necessary
void LineView::removeChild( View * view )
{
  IASSERTSTATE( _firstView && _lastView )
  Boolean isFirst = ( view == _firstView->view() );
  Boolean isLast = ( view == _lastView->view() );

  if ( isFirst && isLast )
  {
    // this was the last view on the line! mark empty line
    setFirstView( 0, true );
  }
  else if ( isFirst )
  {
    // increment line's first view
    Boolean ok = _firstView->setToNext();
    IASSERTSTATE( ok );
  }
  else if ( isLast )
  {
    // decrement line's last view
    Boolean ok = _lastView->setToPrevious();
    IASSERTSTATE( ok );
  }

  // ready to remove from the Flow's collection
  flow()->removePlacedView( view );
}


int LineView::orderViews( View & view1, View & view2 )
{
  // assume Flow already checked for same view
  IASSERTSTATE( &view1 != &view2 )

  // check if either view is "this"
  if ( &view1 == this)
    return -1;
  else if ( &view2 == this )
    return 1;

  // find both sub-views ("placed" views)
  View * placed1 = locateDescendent( view1 );
  IASSERTSTATE( placed1 );
  View * placed2 = locateDescendent( view2 );
  IASSERTSTATE( placed2 );

  // if the same view, pass the request down to the view
  if ( placed1 == placed2 )
    return placed1->orderViews( view1, view2 );

  // check if one of them is the LineEndView
  if ( placed1 == &_lineEnd )
    return 1;
  else if ( placed2 == &_lineEnd )
    return -1;

  // otherwise ask owner FlowView to order the Views (it owns them)
  return flow()->orderPlacedViews( placed1, placed2 );
}


IString LineView::dumpString() const
{
  return debugString( "LineView"
    + IString(" w=") + IString(_width)
    + IString(" h=") + IString(_height)
    + IString(" d=") + IString(_descent)
    + IString(" a=") + IString(_above)
  );
}

