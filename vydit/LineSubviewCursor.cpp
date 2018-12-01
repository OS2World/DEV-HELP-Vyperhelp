/***************************************************************************
 * File...... LineSubviewCursor.cpp
 * Author.... Mat
 * Date...... 5/16/96
 *
 * Implementation of LineSubviewCursor.  Assumes that Views will not be added
 * to or removed from the FlowView while the cursor is active.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "LineView.hpp"
#include "LineEndView.hpp"
#include "LineSubviewCursor.hpp"


LineSubviewCursor::LineSubviewCursor( LineView & line, LineEndView * lineEnd ):
    _state( invalid ),
    _viewCursor( NULL ),
    _line( &line ),
    _lineEnd( lineEnd )
{
  // if the line has any cursored views, create a cursor for them
  OrderedViewCursor * cursor = line.firstView();
  if ( cursor )
    _viewCursor = new OrderedViewCursor( *cursor );
}


LineSubviewCursor::~LineSubviewCursor()
{
  delete _viewCursor;
}


View * LineSubviewCursor::view()
{
  IASSERTSTATE( _state != invalid );
  if ( _state == end )
    return _lineEnd;
  // otherwise use the view cursor
  return _viewCursor->view();
}


const View * LineSubviewCursor::view() const
{
  IASSERTSTATE( _state != invalid );
  if ( _state == end )
    return _lineEnd;
  // otherwise use the view cursor
  return _viewCursor->view();
}


IBoolean LineSubviewCursor::isFirst() const
{
  switch ( _state )
  {
    case cursor:
      return ( *_viewCursor == *_line->firstView() );
    case end:
      return ( _lineEnd != NULL && _viewCursor == NULL );
    case invalid:
      return false;
  }
  return false;
}


IBoolean LineSubviewCursor::isLast() const
{
  // if a lineEnd is available, must be at the "end" state
  if ( _lineEnd )
    return ( _state == end );

  // otherwise, check if cursor is at the line's lastView
  return ( _state == cursor && *_viewCursor == *_line->lastView() );
}


IBoolean LineSubviewCursor::setToLast ()
{
  // if we have a LineEndView, use it
  if ( _lineEnd )
  {
    _state = end;
    return true;
  }

  // otherwise use the cursor if we have it
  if ( _viewCursor )
  {
    _state = cursor;
    *_viewCursor = *_line->lastView();
    return _viewCursor->isValid();
  }

  // otherwise it's an empty line!
  return false;
}


IBoolean LineSubviewCursor::setToPrevious ()
{
  IASSERTSTATE( _state != invalid );

  // if at end state, move back to cursor state
  if ( _state == end )
  {
    // move back to cursor if you have one
    if ( _viewCursor )
    {
      _state = cursor;
      *_viewCursor = *_line->lastView();
      return _viewCursor->isValid();
    }
    else
    {
      _state = invalid;
      return false;
    }
  }

  // if at start of line, can't go back any further
  if ( *_viewCursor == *_line->firstView() )
  {
    _state = invalid;
    return false;
  }

  // otherwise, move the ICursor back
  return _viewCursor->setToPrevious();
}


IBoolean LineSubviewCursor::setToFirst ()
{
  // if we have a cursor, use it
  if ( _viewCursor )
  {
    _state = cursor;
    *_viewCursor = *_line->firstView();
    return true;
  }

  // otherwise, if we have a LineEndView, use it
  if ( _lineEnd)
  {
    _state = end;
    return true;
  }

  // otherwise it's an empty line!
  return false;
}


IBoolean LineSubviewCursor::setToNext ()
{
  IASSERTSTATE( _state != invalid );

  // if at end state, can't go further
  if ( _state == end )
  {
    _state = invalid;
    return false;
  } /* endif */

  // if at end of this line, try to go to end state
  if ( *_viewCursor == *_line->lastView() )
  {
    if ( _lineEnd )
    {
      _state = end;
      return true;
    }
    else
    {
      _state= invalid;
      return false;
    }
  }

  // otherwise advance the ICursor
  return _viewCursor->setToNext();
}


IBoolean LineSubviewCursor::isValid () const
{
  return ( _state == cursor )? _viewCursor->isValid(): ( _state != invalid );
}


