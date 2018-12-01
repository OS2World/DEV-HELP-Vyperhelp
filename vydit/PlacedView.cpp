/***************************************************************************
 * File...... PlacedView.cpp
 * Author.... Mat
 * Date...... 1/30/97
 *
 * Implementation for virtual PlacedView base class.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "FlowItem.hpp"
#include "FormatPen.hpp"
#include "PlacedView.hpp"


// static collection of active states
PenStateSet PlacedView::_activeStates( 10 );


// constructor
PlacedView::PlacedView( Item * subject, View * parent ):
    SubjectView( subject, parent ),
    _xPosition( -1 ),
    _state( 0 )
{
  // set default state
  setState( PenState() );
}


// destructor
PlacedView::~PlacedView()
{
  removeState();
}


// return size change on first call only
PlacedView::FormatChange PlacedView::format( FormatPen & pen )
{
  // check if formatted yet
  FormatChange change = ( _xPosition == -1 )? changeSize: changeNothing;

  // save the pen's incoming state
  setState( pen.state() );

  // set view position
  Boolean positionChanged = setPosition( pen.point().x() );

  // determine extent of the change
  return positionChanged? changeSize: change;
}


void PlacedView::insertItem( Item * item )
{
  FlowItem * flow = (FlowItem *) subject()->parent();
  flow->insertItem( item, subject() );
}


// most PlacedViews are leafs too, so default returns cursor to this
ViewCursor * PlacedView::newLeafCursor( const Boolean wantFirst )
{
  return newCursorToThis();
}


Boolean PlacedView::setPosition( Coord xPosition )
{
  if ( xPosition != _xPosition )
  {
    _xPosition = xPosition;
    return true;
  }
  return false;
}


void PlacedView::generate( View * from, View * to, unsigned & level )
{
  Item * item = subject();
  if ( item )
    item->generate();
}


IString PlacedView::onOffString( Boolean isOn ) const
{
  return IString( isOn? "on": "off" );
}


// remove the current state from the collection (or decrement reference count)
void PlacedView::removeState()
{
  if ( _state )
  {
    if ( _state->decrementCount() )
    {
      _activeStates.remove( _state );
      delete (PenState *)_state;
    }
    _state = 0;
  }
}


void PlacedView::setState( const PenState & state )
{
  if ( ! _state || ( state != *_state ) )
  {
    // remove current reference
    removeState();

    // see if the state is already in the collection
    PenStateSet::Cursor cursor( _activeStates );
    Boolean found = _activeStates.locate( (PenState *)&state, cursor );
    if ( found )
    {
      // increment reference count of existing state
      _state = cursor.element();
      _state->incrementCount();
    }
    else
    {
      // add new state to the collection
      _state = new PenState();
      *_state = state;
      _state->setCount( 1 );
      _activeStates.add( _state );
    }
  }
}


IString PlacedView::dumpString() const
{
  return debugString( info() );
}

