/***************************************************************************
 * File...... FlowItem.cpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * Implements FlowItem.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// Generator
#include "Generator.hpp"

// TextEditor
#include "SmartText.hpp"
#include "EndItem.hpp"
#include "WordItem.hpp"
#include "SpaceItem.hpp"
#include "ArtItem.hpp"
#include "FlowView.hpp"
#include "FlowItem.hpp"
#include "FlowInsertData.hpp"

// notification events
const INotificationId FlowItem::insertItemId = "FlowItem::insertItem";


FlowItem::FlowItem( Item * parent ):
    _parent( parent ),
    _children( 500 ),
    _gen( 0 )
{
  // initialize empty item list
  _children.add( new EndItem( this ) );
}


void FlowItem::reset()
{
  _children.removeAll();
}


OrderedItemCursor * FlowItem::newCursor()
{
  return new OrderedItemCursor( _children );
}


View * FlowItem::newView( View * parent )
{
  /// ignore parent for now -- assume top level
  return new FlowView( this );
}


void FlowItem::removeChild( Item * item )
{
  _children.remove( item );
}


INumber FlowItem::numberOfItems() const
{
  return _children.numberOfElements();
}


/***************************************************************************
 * Procedure.. FlowItem::insertItem
 * Author..... Mat
 *
 * Insert the new item in front of the cursored item.
 * Assumes there is always an EndItem which won't allow insertion after.
 *
 * If newItem is a simple (non-flow) item, ownership is handed over to
 * this flow.  If newItem is another FlowItem, then ownerwhip of the
 * contents is handed over to this flow, and the new flow is emptied.
 * (NOTE: the new flow is not deleted)
 *
 * This routine does not check for word merges.  It lets the FlowView do that,
 * since the word merge will not work until the companion views have been created
 * (because WordView cursors are passed between WordViews being merged)
 *
 ***************************************************************************/
void FlowItem::insertItem( Item * newItem, Item * itemAfter, Boolean checkWords )
{
  IASSERTPARM( newItem && itemAfter );

  // look for the itemAfter
  OrderedItemCursor c( _children );
  Boolean found = _children.locate( itemAfter, *c.cursor() );
  if ( ! found )
    return;

  // determine if item to insert is a flow
  FlowItem * flow = newItem->asFlow();
  if ( flow )
  {
    // insert contents of a flow item in reverse order
    OrderedItemCursor cInsert( flow->_children );

    // skip and delete EndItem
    found = cInsert.setToLast();
    IASSERTSTATE( found );
    delete cInsert.item();

    found = cInsert.setToLast();
    while ( found )
    {
      Item * item = cInsert.item();
      found = cInsert.setToPrevious();
      item->setParent( this );
      _children.addAsPrevious( item, *c.cursor() );
    }
    // remove all items from the flow's list
    flow->reset();
  }
  else
  {
    // insert a simple (non-flow) item
    _children.addAsPrevious( newItem, *c.cursor() );
  }

  // tell everyone
  FlowInsertData data;
  data.cursor = &c;
  data.mergeWords = checkWords;
  notifyObservers( insertItemId, IEventData( &data ) );
}


// assumes no "word merge" check is needed
void FlowItem::appendItem( Item * newItem )
{
  // move to the last item (assume it is an EndItem)
  OrderedItemCursor c( _children );
  Boolean found = c.setToLast();
  IASSERTSTATE( found );

  // add the new item and update the cursor
  _children.addAsPrevious( newItem, *c.cursor() );

  // tell everyone
  FlowInsertData data;
  data.cursor = &c;
  data.mergeWords = false;
  notifyObservers( insertItemId, IEventData( &data ) );
}


void FlowItem::appendText( const IString & string )
{
  // move to the last item (assume it is an EndItem)
  OrderedItemCursor c( _children );
  Boolean found = c.setToLast();
  IASSERTSTATE( found );

  // parse text, add in reverse order (cursor follows)
  SmartText text( string );
  unsigned n = text.numItems();
  for ( int i = n - 1; i >= 0; i-- )
    _children.addAsPrevious( text.createItem( i, this ), *c.cursor() );

  // tell everyone
  FlowInsertData data;
  data.cursor = &c;
  data.mergeWords = false;
  notifyObservers( insertItemId, IEventData( &data ) );
}


void FlowItem::generate()
{
  // send a SpacingGin to indicate spacing between lines
  *_gen << SpacingGin( Distance( 1, Distance::chars ) );

  // loop through all children (each will call sendGin or sendText)
  OrderedItemCursor cursor( _children );
  forCursor( cursor )
  {
    cursor.item()->generate();
  }
}


void FlowItem::finishGenerate( Boolean allDone )
{
  // send final text if necessary
  flushText();

  // end generator output
  if ( allDone )
    _gen->finish();

  // clear generator reference
  _gen = 0;
}


void FlowItem::sendGin( const Gin & gin )
{
  IASSERTSTATE( _gen );
  flushText();
  *_gen << gin;
}


void FlowItem::sendText( const IString & string )
{
  IASSERTSTATE( _gen );
  _output += string;
}


void FlowItem::flushText()
{
  IASSERTSTATE( _gen );
  if ( _output.length() )
  {
    *_gen << _output;
    _output = IString();
  }
}


IString FlowItem::dumpString() const
{
  return IString( "FlowItem" );
}


/***************************************************************************
 * ART SUPPORT
 ***************************************************************************/

/***************************************************************************
 * Procedure.. FlowItem::getArt
 * Author..... Mat
 * Date....... 9/28/99
 *
 * Returns a pointer to the art definition, or 0 if not found.
 ***************************************************************************/
const ArtDef * FlowItem::getArt( const IString & id )
{
  ArtSet::Cursor cursor( _arts );
  if ( _arts.locateElementWithKey( id, cursor ) )
    return &(cursor.element());

  // not found
  return 0;
}


/***************************************************************************
 * Procedure.. FlowItem::addArt
 * Author..... Mat
 * Date....... 9/28/99
 *
 * Adds the art definition to the Flow.
 ***************************************************************************/
void FlowItem::addArt( const ArtDef & art )
{
  _arts.add( art );
}


/***************************************************************************
 * Procedure.. FlowItem::deleteArt
 * Author..... Mat
 * Date....... 9/28/99
 *
 * Deletes the specified art definition from the flow, and removes any art
 * which references the specified item.
 *
 * Return true if the ID was in use by an ArtItem (which was subsequently
 * deleted).
 ***************************************************************************/
Boolean FlowItem::deleteArt( const IString & id )
{
  Boolean found = false;

  // look for and delete children ArtItems which use this ID
  OrderedItemCursor cursor( _children );
  cursor.setToFirst();
  while ( cursor.isValid() )
  {
    ArtItem * art = cursor.item()->asArt();
    cursor.setToNext();  // advance cursor before deleting item!
    if ( art && ( art->id() == id ) )
    {
      found = true;
      // NOTE: don't delete ArtItem for now because it could screw up Editor cursors
      //       ArtItem can handle not finding the art definition
      // delete art;
    }
  }

  // remove the art definition
  _arts.removeElementWithKey( id );

  return found;
}


