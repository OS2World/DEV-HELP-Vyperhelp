/***************************************************************************
 * File...... Item.cpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * An Item is an abstract class representing an editor item.  Item provides
 * default implementation for most of the functions.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "SubjectView.hpp"
#include "NullItemCursor.hpp"
#include "Item.hpp"


// default implementation, quasi-abstract class


Item::Item( Item * parent ):
    _parent( parent ),
    _observer( 0 )
{}


Item::~Item()
{
  notifyObservers( deleteId );
  if ( _parent )
    _parent->removeChild( this );
}


void Item::change( Item * child )
{
  // notify observers
  notifyObservers( changeId );

  // notify parent
  if ( _parent )
    _parent->change( this );
}


ItemCursor * Item::newCursor()
{
  // default action creates a NullCursor
  return new NullItemCursor;
}


void Item::notifyObservers( INotificationId id, const IEventData & event )
{
  if ( _observer )
    _observer->dispatchNotificationEvent( id, event );
}


const INotificationId Item::deleteId = "Item::delete";
const INotificationId Item::changeId = "Item::change";

