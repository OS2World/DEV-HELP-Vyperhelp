/***************************************************************************
 * File...... FlowView.cpp
 * Author.... Mat
 * Date...... 2/6/96
 *
 * Implementation of FlowView
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Standard C
#include <stdlib.h>

// OpenClass
#include <itrace.hpp>                 // ITRACE macros
#include <iexcept.hpp>                // IASSERT macros

// Generator
#include "Gin.hpp"

// Editor
#include "OrderedItemCursor.hpp"
#include "FlowItem.hpp"
#include "WordItem.hpp"
#include "DrawPen.hpp"
#include "LineView.hpp"
#include "Editor.hpp"
#include "FlowView.hpp"
#include "FlowInsertData.hpp"



/***************************************************************************
 * Procedure.. FlowView::FlowView
 * Author..... Mat
 * Date....... 5/16/96
 *
 * The constructor creates a view for each corresponding Item in the subject
 * FlowItem.  A single LineView is created to "hold" all the views until
 * the first format is performed.
 *
 * Assume that the caller will force a reformat after creating the FlowView
 *
 * For now, assume there is only one FlowView and it is owned by a Editor.
 ***************************************************************************/
FlowView::FlowView( FlowItem * subject ):
    SubjectView( subject, 0 ),
    _editor( 0 ),
    _views( 500 ),
    _lines( 25 )
{
  // create a single LineItem to hold them all
  LineView * line = new LineView( this );
  _lines.add( line );

  // add all of the subject's items to the _views list
  ItemCursor * itemCursor = subject->newCursor();
  forCursor( *itemCursor )
    _views.add( itemCursor->item()->newView( line ) );
  delete itemCursor;

  // set Line to include all views
  OrderedViewCursor viewCursor( _views );
  viewCursor.setToFirst();
  IASSERTSTATE( viewCursor.isValid() );
  line->setFirstView( &viewCursor, false );
  viewCursor.setToLast();
  IASSERTSTATE( viewCursor.isValid() );
  line->setLastView( &viewCursor );
}


/***************************************************************************
 * Procedure.. FlowView::newCursor
 * Author..... Mat
 * Date....... 5/16/96
 *
 * Return an OrderedViewCursor for iterating across all LineViews
 ***************************************************************************/
OrderedViewCursor * FlowView::newCursor()
{
  return new OrderedViewCursor( _lines );
}


/***************************************************************************
 * Procedure.. FlowView::draw
 * Author..... Mat
 * Date....... 10/2/97
 *
 * Draw any line in the zone.
 ***************************************************************************/
void FlowView::draw( DrawPen & pen, const IRectangle & zone )
{
  OrderedViewCursor cursor( _lines );

  // assume lines are equal to get a starting point
  Coord zoneMax = zone.maxY();
  Coord flowMax = _size.height();
  if ( zoneMax > flowMax )
    zoneMax = flowMax;
  IPosition start = _lines.numberOfElements() * ( flowMax - zoneMax ) / flowMax;
  if ( start < 1 )
    start = 1;
  IASSERTSTATE( start <= _lines.numberOfElements() );
  _lines.setToPosition( start, *cursor.cursor() );

  // back up if necessary, then draw the lines
  Boolean looking = true;
  Boolean ok;
  do
  {
    LineView * line = (LineView *) cursor.view();
    Coord lineBottom = line->position().y();
    if ( looking )
    {
      Coord lineTop = lineBottom + line->height();
      if ( lineTop < zoneMax && ! cursor.isFirst() )
      {
        ok = cursor.setToPrevious();
        IASSERTSTATE( ok );
      }
      else
        looking = false;
    }
    if ( ! looking )
    {
      if ( lineBottom < zoneMax )
        line->draw( pen, zone.movedBy( - line->position() ) );
      if ( lineBottom < zone.minY() )
        return;
      ok = cursor.setToNext();
    }
  } while ( ok );
}


/***************************************************************************
 * Procedure.. FlowView::dispatchNotificationEvent
 * Author..... Mat
 * Date....... 5/16/96
 *
 * Dispatch notifications received from the subject FlowItem.
 ***************************************************************************/
void FlowView::dispatchNotificationEvent( INotificationId id, const IEventData & event )
{
  if ( id == FlowItem::insertItemId )
  {
    FlowInsertData * data = (FlowInsertData *) (char *) event;
    insertViews( *data->cursor, data->mergeWords );
  }
}


/***************************************************************************
 * Procedure.. FlowView::insertViews
 * Author..... Mat
 * Date....... 5/14/96
 *
 * itemCursor points to an Item which was newly added to the FlowItem
 * (subject).  This function creates Views for the item and any other new
 * ones that follow.  Rely on format() to do proper line-breaking.
 *
 * Assumes that Views never need to be appended to the end of the last
 * line, because there is always an EndItem which doesn't allow appending.
 ***************************************************************************/
void FlowView::insertViews( OrderedItemCursor & itemCursor, Boolean needMergeCheck )
{
  LineView * line;
    // pointer to line where new views are being inserted
  enum {
    firstOnLine,
    midLine,
    lastOnLine
  } insertPosition;
    // where on the line the new views will be inserted
  OrderedViewCursor viewCursor( _views );
    // used to locate the view nearest to the new Item(s)
  OrderedViewCursor afterCursor( _views );
    // cursor to view immediately *after* the inserted views

  // must have a valid cursor
  IASSERTPARM( itemCursor.isValid() );


  // GET CURSOR TO LAST UNCHANGED ITEM IN THE FLOW
  // (invalidate if itemCursor is first)
  OrderedItemCursor itemCursorBefore = itemCursor;
  itemCursorBefore.setToPrevious();


  // FIND VIEW CORRESPONDING TO LAST UNCHANGED ITEM
  if ( itemCursorBefore.isValid() )
  {
    // look for "item before" in current _views list
    Item * itemBefore = itemCursorBefore.item();
    forCursor( afterCursor )
    {
      PlacedView * placed = (PlacedView *) afterCursor.view();
      if ( placed->subject() == itemBefore )
      {
        // found last unchanged view, save the cursor
        viewCursor = afterCursor;
        // get line
        line = (LineView *) placed->parent();
        // check if last view in the line
        // (can't use LineView cursor.isLast(), because it considers LineEndViews too!)
        insertPosition = ( line->lastView()->view() == placed )? lastOnLine: midLine;
        // move to next view to get the item after any new items
        afterCursor.setToNext();
        break;
      }
    }
    // must always find a corresponding view
    IASSERTSTATE( afterCursor.isValid() );
  }
  else
  {
    OrderedViewCursor lineCursor( _lines );
    // no "item before" -- insert in front of first view
    afterCursor.setToFirst();
    // get pointer to first line
    Boolean found = lineCursor.setToFirst();
    IASSERTSTATE( found );
    line = (LineView *) lineCursor.view();
    insertPosition = firstOnLine;
    // sequenceCursor is invalid to indicate insert at front
  }


  // SAVE POINTER TO ITEM AFTER NEW ONES
  PlacedView * placed = (PlacedView *) afterCursor.view();
  Item * itemAfter = placed->subject();


  // INSERT ALL NEW ITEMS
  Item * item;
  View * view = 0;
  View * firstAffectedView = 0;
  while ( itemCursor.isValid() && ( ( item = itemCursor.item() ) != itemAfter ) )
  {
    // move cursor to the next item before working on this item
    itemCursor.setToNext();

    // create a view pointer
    view = item->newView( line );
    IASSERTSTATE( view );

    // insert view into sequence
    if ( viewCursor.cursor()->isValid() )
    {
      // add in front of specified cursor and advance cursor
      _views.addAsNext( view, *viewCursor.cursor() );

      // check cursor for word merge
      if ( needMergeCheck && checkForWordMerge( &viewCursor ) )
      {
        // this view was merged with the previous!

        // back the cursor up the the merged word
        Boolean ok = viewCursor.setToPrevious();
        IASSERTSTATE( ok );

        // if this was the first view then set merged word as affected view
        if ( ! firstAffectedView )
          firstAffectedView = viewCursor.view();

        // clear view because it was deleted
        view = 0;
      }
    }
    else
    {
      // add as first view and set cursor
      _views.addAsFirst( view, *viewCursor.cursor() );
    }

    // if the view exists, update affected view and line position
    if ( view )
    {
      if ( ! firstAffectedView )
        firstAffectedView = view;

      if ( insertPosition == firstOnLine )
      {
        // reset line's first view if necessary
        // (also reset last view if the line is empty)
        line->setFirstView( &viewCursor, ( line->lastView() == 0 ) );
        insertPosition = midLine;
      }
      else if ( insertPosition == lastOnLine )
      {
        // update line's last view
        line->setLastView( &viewCursor );
      }
    }
  }

  // FINAL CHECK FOR WORD MERGE WITH ITEM AFTER
  if ( needMergeCheck )
    checkForWordMerge( &afterCursor );

  // RE-FORMAT, STARTING WITH THE FIRST VIEW
  if ( firstAffectedView )
    _editor->formatStartingAt( *(PlacedView *) firstAffectedView );
}


void FlowView::generate( View * from, View * to, unsigned & level )
{
  // assume that both views are provided
  IASSERTPARM( from );
  IASSERTPARM( to );

  // find first and last line
  View * firstLine = locateDescendent( *from );
  IASSERTSTATE( firstLine );
  View * lastLine = locateDescendent( *to );
  IASSERTSTATE( lastLine );
  // assume last line >= first line (no check done)

  // get cursor, starting with first line
  ViewCursor * lineCursor = firstLine->newCursorToThis();
  IASSERTSTATE( lineCursor );

  // use cursor to iterate through lines
  View * line;
  Boolean ok;
  do
  {
    line = lineCursor->view();
    line->generate(
        line == firstLine? from: 0,
        line == lastLine? to: 0,
        level );
    if ( line == lastLine )
      ok = false;
    else
      ok = lineCursor->setToNext();
  }
  while ( ok );

  // insert PopGins to bring level back to 0
  FlowItem * flowItem = subject()->asFlow();
  IASSERTSTATE( flowItem );
  while ( level--)
    flowItem->sendGin( PopGin() );

  // delete the cursor
  delete lineCursor;
}


/***************************************************************************
 * FlowView::remove
 *
 * Remove the range of views specified.  Calls recursively into each View's
 * remove() function.  Both provided cursors are leaf views.
 ***************************************************************************/
unsigned FlowView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  // assume that both cursors are provided
  IASSERTPARM( fromCursor );
  IASSERTPARM( toCursor );

  // find first and last line
  View * firstLine = locateDescendent( *fromCursor->view() );
  IASSERTSTATE( firstLine );
  View * lastLine = locateDescendent( *toCursor->view() );
  IASSERTSTATE( lastLine );
  // assume last line >= first line (no check done)

  // get a cursor to View before the first one removed
  // may need this cursor if we need to search for orphans
  View * firstView = firstLine->locateDescendent( *fromCursor->view() );
  IASSERTSTATE( firstView );
  OrderedViewCursor viewCursor( _views );
  Boolean found = viewCursor.locate( firstView );
  IASSERTSTATE( found );
  viewCursor.setToPrevious();

  // get cursor, starting with first line
  ViewCursor * lineCursor = firstLine->newCursorToThis();
  IASSERTSTATE( lineCursor );

  // use cursor to iterate through lines
  View * line;
  Boolean ok;
  unsigned orphanCount = 0;
  do
  {
    line = lineCursor->view();
    orphanCount += line->remove(
        line == firstLine? fromCursor: NULL,
        line == lastLine? toCursor: NULL );
    if ( line == lastLine )
      ok = false;
    else
      ok = lineCursor->setToNext();
  }
  while ( ok );

  // delete the cursor
  delete lineCursor;

  // search for orphans
  if ( orphanCount )
  {
    // if the view cursor is invalid, start at beginning
    if ( ! viewCursor.isValid() )
      viewCursor.setToFirst();
    // search for orphans and delete them
    while ( orphanCount && viewCursor.isValid() )
    {
      PlacedView * view = (PlacedView *) viewCursor.view();

      // increment cursor before deletion
      viewCursor.setToNext();

      // check for orphan
      if ( view->isOrphan() )
      {
        // delete the orphan item and view
        IASSERTSTATE( view->subject() );
        delete view->subject();
        orphanCount--;

        // check for word merge
        if ( viewCursor.isValid() )
          checkForWordMerge( &viewCursor );

        // trigger format at next view
        if ( viewCursor.isValid() )
        {
          view = (PlacedView *) viewCursor.view();
          if ( ! view->isOrphan() )
            _editor->formatStartingAt( *view );
        }
      }

    }
  }

  // internal orphan search already done
  return 0;
}



/***************************************************************************
 * FlowView::checkForWordMerge
 *
 * Check to see if the given and the previous view are both CharViews in a
 * different word view. If so, the two words need to be combined into one,
 * and the given view is DELETED!
 * The given view is assumed to be a member of the flow's collection.
 * Return true if words were merged and cursor was setToNext().
 ***************************************************************************/
Boolean FlowView::checkForWordMerge( OrderedViewCursor * cursor )
{
  // get view
  IASSERTPARM( cursor );
  PlacedView * view = (PlacedView *) cursor->view();
  IASSERTSTATE( view );

  // check if item is a word
  Item * item = view->subject();
  if ( item && item->asWord() )
  {
    // now check if the previous view is also a Word
    // use internal cursor to avoid line boundaries and transient views
    OrderedViewCursor cursorPrevious( _views );
    Boolean found = cursorPrevious.locate( view );
    IASSERTSTATE( found );
    found = cursorPrevious.setToPrevious();
    if ( found )
    {
      PlacedView * viewPrevious = (PlacedView *) cursorPrevious.view();
      IASSERTSTATE( viewPrevious );
      // check if this item is a word, too
      Item * itemPrevious = viewPrevious->subject();
      if ( itemPrevious && itemPrevious->asWord() )
      {
        // merge the words!
        // first move the given cursor forward to avoid invalidation
        cursor->setToNext();
        // now merge the two words ("word" will delete itself)
        WordItem * word = item->asWord();
        WordItem * wordPrevious = itemPrevious->asWord();
        word->mergeWords( *wordPrevious );
        return true;
      }
    }
  }

  return false;
}



/***************************************************************************
 * Procedure.. FlowView::update
 * Author..... Mat
 * Date....... 6/18/96
 *
 * Handle notification from a child View which has changed its representation.
 * Update the display to reflect changes.
 ***************************************************************************/
void FlowView::update( View & childView )
{
  // initiate format starting at the given View (child of a LineView)
  _editor->formatStartingAt( (PlacedView &) childView );
  // set "modified" flag of editor
  _editor->setModified();
}


/***************************************************************************
 * Procedure.. FlowView::changeView
 * Author..... Mat
 * Date....... 6/22/99
 *
 * Called when a lower-level PlacedView
 ***************************************************************************/
void FlowView::changeView( PlacedView * viewOld, PlacedView * viewNew )
{
  IASSERTPARM( viewOld && viewNew );
  _editor->changeFormatView( viewOld, viewNew );
}


/***************************************************************************
 * Procedure.. FlowView::newCorrelateCursor
 * Author..... Mat
 * Date....... 5/16/96
 *
 * Correlate a point by locating the correct line (y-position) and then
 * correlating x-position within that line.
 *
 * Could change to use a faster search (binary?)
 ***************************************************************************/
ViewCursor * FlowView::newCorrelateCursor( IPoint & point )
{
  ViewCursor * cursor = 0;

  // clip the given point to fall in the FlowView
  if ( point.x() < 0 )
    point.setX( 0 );
  else if ( point.x() > width() )
    point.setX( width() );
  if ( point.y() < 0 )
    point.setY( 0 );
  else if ( point.y() > height() )
    point.setY( height() );

  // find the line which contains the point
  OrderedViewCursor lineCursor( _lines );
  forCursor( lineCursor )
  {
    View * line = lineCursor.view();
    Coord bottom = line->position().y();
    Coord top = bottom + line->extent();
    if ( ( point.y() <= top ) && ( point.y() >= bottom ) )
    {
      // found the correct line
      IPoint relativePoint = point - IPoint( 0, bottom );
      cursor = line->newCorrelateCursor( relativePoint );
      break;
    }
  }

  IASSERTSTATE( cursor );
  return cursor;
}


void FlowView::removeChild( View * view )
{
  _lines.remove( view );
}


void FlowView::removePlacedView( View * view )
{
  _views.remove( view );
}


int FlowView::orderViews( View & view1, View & view2 )
{
  // check for same view
  if ( &view1 == &view2 )
    return 0;

  // check if either view is "this"
  if ( &view1 == this)
    return -1;
  else if ( &view2 == this )
    return 1;

  // find both lines
  View * line1 = locateDescendent( view1 );
  IASSERTSTATE( line1 );
  View * line2 = locateDescendent( view2 );
  IASSERTSTATE( line2 );

  // if on the same line, pass the request down to the line
  if ( line1 == line2 )
    return line1->orderViews( view1, view2 );

  // otherwise find out which line is first
  return _lines.order( *line1, *line2 );
}


int FlowView::orderLines( LineView & line1, LineView & line2 )
{
  return _lines.order( line1, line2 );
}


// assumes both children are in its collection
// used by child LineViews to ask about View ordering
int FlowView::orderPlacedViews( View * view1, View * view2 )
{
  return _views.order( *view1, *view2 );
}


OrderedViewCursor * FlowView::newViewCursor( View * view )
{
  OrderedViewCursor * cursor = new OrderedViewCursor( _views );
  if ( view )
    cursor->locate( view );
  else
    cursor->setToFirst();
  return cursor;
}


IString FlowView::dumpString() const
{
  return debugString( IString("FlowView ") + _size.asString() );
}

