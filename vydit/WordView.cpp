/***************************************************************************
 * File...... WordView.cpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * Implementation of WordView.
 *
 * _length holds the number of characters in the word.
 *
 * NOTE: charWidth doesn't properly handle DBCS
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>                // IASSERT macros
#include <igstring.hpp>               // IGString
#include <ifont.hpp>                  // IFont
#include <igrect.hpp>                 // IGRectangle

// TextEditor
#include "DrawPen.hpp"
#include "FormatPen.hpp"
#include "FlowView.hpp"
#include "OrderedViewCursor.hpp"
#include "WordView.hpp"

// DEBUGGING
//#define DRAW_TEXT_BOX
//#define DRAW_FONT_BOX


WordView::WordView( WordItem * subject, View * parent ):
    PlacedView( subject, parent ),
    _length( subject->string().length() ),
    _cursorSet( 0 )
{}


// 0-based index
Coord WordView::charOffset( unsigned index ) const
{
  IASSERTPARM( index < _length );
  Coord offset = 0;
  char * buffer = word()->string();
  while ( index-- )
    offset += state().charWidth( buffer[index] );
  return offset;
}


// 0-based index
Coord WordView::charWidth( const unsigned index ) const
{
  IASSERTPARM( index < _length );
  char * buffer = word()->string();
  return state().charWidth( buffer[index] );
}


// index is a 0-based index into the WordItem's string where the new item
// should be inserted
void WordView::insertItemAtChar( Item * item, const unsigned index )
{
  IASSERTPARM( index < _length );
  word()->insertItem( item, index );
}


WordViewCursor * WordView::newCursor()
{
  WordViewCursor * cursor = new WordViewCursor( this );
  Boolean ok = _cursorSet.add( cursor );
  IASSERTSTATE( ok );
  return cursor;
}


void WordView::cursorDeleted( WordViewCursor * cursor )
{
  Boolean ok = _cursorSet.remove( cursor );
  IASSERTSTATE( ok );
}


// check only for x-position
ViewCursor * WordView::newCorrelateCursor( IPoint & point )
{
  // get a cursor
  WordViewCursor * cursor = newCursor();
  cursor->setToFirst();
  IASSERTSTATE( cursor->isValid() );

  // find the character which holds the x-position
  char * buffer = word()->string();
  Coord x = point.x();
  Coord secondHalf = 0;
  Coord offset = 0;
  int i = 0;
  while ( cursor->isValid() )
  {
    Coord charWidth = state().charWidth( buffer[i] );
    Coord firstHalf = charWidth / 2;
    offset += ( firstHalf + secondHalf );
    if ( x < offset )
      return cursor;
    // try next character
    secondHalf = charWidth - firstHalf;  // may not divide evenly
    i++;
    cursor->setToNext();
  }

  // x-position is beyond last character, use standard View method
  delete cursor;
  return View::newCorrelateCursor( point );
}


// revert to default View handling
// (override PlacedView)
ViewCursor * WordView::newLeafCursor( const Boolean wantFirst )
{
  return View::newLeafCursor( wantFirst );
}


int WordView::orderViews( View & view1, View & view2 )
{
  // assume Line already checked for same view
  IASSERTSTATE( &view1 != &view2 )

  // check if either view is "this"
  if ( &view1 == this)
    return -1;
  else if ( &view2 == this )
    return 1;

  // get CharView pointers to children
  CharView & char1 = (CharView &) view1;
  CharView & char2 = (CharView &) view2;

  // return result of comparison
  return (int)char1.index() - (int)char2.index();
}


Coord WordView::height() const
{
  return state().maxAscender();
}


Coord WordView::descent() const
{
  return state().maxDescender();
}


Coord WordView::width() const
{
  Coord offset = 0;
  char * buffer = word()->string();
  for ( int i = 0; i < _length; i++ )
    offset += state().charWidth( buffer[i] );
  return offset;
}


PlacedView::FormatChange WordView::format( FormatPen & pen )
{
  // get font metrics
  Coord oldHeight = height();
  Coord oldDescent = descent();
  Coord oldWidth = width();

  // call default formatting to check for change
  FormatChange change = PlacedView::format( pen );

  // check for size change
  if ( change < changeSize )
  {
    Boolean sizeChange = ( height() != oldHeight
                        || descent() != oldDescent
                        || width() != oldWidth );
    if ( sizeChange )
      change = changeSize;
  }

  /// what if text changed, but not size??

  return change;
}


/***************************************************************************
 * Procedure.. WordView::draw
 * Author..... Mat
 * Date....... 2/27/97
 *
 * NOTE: We should be able to draw more than one character at a time, but
 * I found that IGString would not space the characters as determined by
 * the font's charWidths.  It would sometimes add and sometimes subtract
 * space within a word.  Drawing character-by-character is less efficient,
 * but it lets us easily handle selection anchor points, and drawing of
 * partial words based on the repaint zone.
 ***************************************************************************/
void WordView::draw( DrawPen & pen, const IRectangle & zone )
{
  // draw the string
  pen.drawString( word()->string() );

#ifdef DRAW_TEXT_BOX
  // draw graphic text box
  {
    IGString gstring( word()->string() );
    IRectangle rect(
        pen.baseline() - IPoint( 0, descent() ),
        gstring.boundingRect( pen.context() ).size() );
    IGRectangle grect( rect );
    IGraphicBundle bundle;
    bundle.setDrawOperation( IGraphicBundle::frame );
    bundle.setPenColor( IColor( IColor::red ) );
    grect.setGraphicBundle( bundle );
    grect.drawOn( pen.context() );
  }
#endif

#ifdef DRAW_FONT_BOX
  // draw font box
  {
    IFont * font = pen.state().font().newFont( pen.context() );
    IGraphicBundle bundle;
    bundle.setDrawOperation( IGraphicBundle::frame );
    bundle.setPenColor( IColor( IColor::blue ) );
    IRectangle rect( pen.baseline() - IPoint( 0, descent() ), ISize() );
    IGRectangle grect;
    grect.setGraphicBundle( bundle );
    for ( int i = 1; i <= _length ; i++ )
    {
      ISize size( font->textWidth( word()->string().subString( i, 1 ) ),
          font->maxAscender() + font->maxDescender() );
      rect.sizeTo( size );
      grect.setEnclosingRect( rect );
      grect.drawOn( pen.context() );
      rect.moveBy( IPoint( size.width(), 0 ) );
    }
    delete font;
  }
#endif

  // move pen ahead
  pen.forward( width() );
}


void WordView::generate( View * from, View * to, unsigned & level )
{
  unsigned index1, index2;

  // get starting index for generate (convert to 1-based)
  if ( from )
  {
    CharView * charView = (CharView *) from;
    IASSERTSTATE( charView->parent() == this );
    index1 = charView->index() + 1;
  }
  else
  {
    index1 = 1;
  }

  // get ending index for generate (convert to 1-based)
  if ( to )
  {
    CharView * charView = (CharView *) to;
    IASSERTSTATE( charView->parent() == this );
    index2 = charView->index() + 1;
  }
  else
  {
    index2 = _length;
  }

  // call subject item to generate
  word()->generate( index1, index2 );
}



// assume the cursors are both for CharView children
// also delete the cursors if they are non-NULL
unsigned WordView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  unsigned index1, index2;

  // get starting index for delete (convert to 1-based)
  if ( fromCursor )
  {
    CharView * charView = (CharView *) fromCursor->view();
    IASSERTSTATE( charView->parent() == this );
    index1 = charView->index() + 1;
    delete fromCursor;
  }
  else
  {
    index1 = 1;
  }

  // get ending index for delete (convert to 1-based)
  if ( toCursor )
  {
    CharView * charView = (CharView *) toCursor->view();
    IASSERTSTATE( charView->parent() == this );
    index2 = charView->index() + 1;
    delete toCursor;
  }
  else
  {
    index2 = _length;
  }

  // delete the entire WordItem if full range is selected
  if ( index1 == 1 && index2 == _length )
  {
    delete word();
    return 0;
  }

  // otherwise tell the WordItem to delete text
  word()->deleteText( index1, index2 );
  return 0;
}


/***************************************************************************
 * Procedure.. WordView::dispatchNotificationEvent
 * Author..... Mat
 * Date....... 5/16/96
 *
 * Dispatch notifications received from the subject WordItem.
 * Since children (CharView) are created on the fly, no need to create them here
 ***************************************************************************/
void WordView::dispatchNotificationEvent( INotificationId id, const IEventData & event )
{
  // update length and indices
  if ( id == WordItem::insertCharsId || id == WordItem::deleteCharsId )
  {
    // update length
    // convert to index so that updateIndex can be used
    unsigned index = _length + 1;
    Boolean changed = updateIndex( index, id, event );
    if ( changed )
      _length = index - 1;

    // update all cursor indexes
    WordViewCursorSet::Cursor setCursor( _cursorSet );
    forCursor( setCursor )
    {
      WordViewCursor * cursor = setCursor.element();
      index = cursor->index();
      if ( updateIndex( index, id, event ) )
        cursor->setIndex( index );
    }
    return;
  }

  // merge: transfer ownership of cursors to previous word
  if ( id == WordItem::mergeWordsId )
  {
    handleMerge( (WordItem *) (char *) event );
    return;
  }

  // default: pass to parent class for handling
  SubjectView::dispatchNotificationEvent( id, event );
}


/***************************************************************************
 * Procedure.. WordView::handleMerge
 * Author..... Mat
 * Date....... 6/22/99
 *
 * This is called when a WordItem is about to be merged with the previous
 * adjacent WordItem.  All cursors for this WordView should be handed over
 * to the previous WordView.  Also, FlowView::changeView is called to notify
 * the FlowView (and Editor) that the View is being transfered.
 ***************************************************************************/
void WordView::handleMerge( WordItem * previousWord )
{
  // get parent line
  View * line = parent();
  IASSERTSTATE( line->isLine() );

  // get grand-parent flow
  FlowView * flow = (FlowView *) line->parent();
  IASSERTSTATE( flow );
  // assume flow is a FlowView!

  // locate a cursor to this word view
  OrderedViewCursor * cursor = flow->newViewCursor( this );
  IASSERTSTATE( cursor );

  // move to previous word view
  if ( cursor->setToPrevious() )
  {
    PlacedView * view = (PlacedView *) cursor->view();

    // transfer cursor ownership to previous word
    if ( view->subject() == previousWord )
    {
      WordView * sibling = (WordView *) view;
      sibling->_cursorSet.addAllFrom( _cursorSet );
      WordViewCursorSet::Cursor setCursor( _cursorSet );
      forCursor( setCursor )
        setCursor.element()->setWord( sibling );
    }

    // notify FlowView
    flow->changeView( this, view );
  }

  delete cursor;
}


/***************************************************************************
 * Procedure.. WordView::updateIndex
 * Author..... Mat
 * Date....... 3/4/97
 *
 * Given a notification event and a reference to an index, update the
 * index based on the event.   Return true if the index was changed.
 ***************************************************************************/
Boolean WordView::updateIndex( unsigned & index, INotificationId id, const IEventData & event )
{
  if ( id == WordItem::insertCharsId )
  {
    // update index based on inserted text
    unsigned insertionIndex = event.lowNumber();
    unsigned insertionLength = event.highNumber();
    if ( index > insertionIndex )
    {
      index += insertionLength;
      return true;
    }
  }
  else if ( id == WordItem::deleteCharsId )
  {
    // update index based on deleted text
    unsigned firstIndex = event.lowNumber();
    unsigned lastIndex = event.highNumber();
    IASSERTSTATE( firstIndex <= lastIndex );
    if ( index > firstIndex )
    {
      if ( index > lastIndex )
        index -= ( lastIndex - firstIndex + 1 );
      else
        index = min( firstIndex, length() );
      return true;
    }
  }

  return false;
}


WordItem * WordView::word()
{
  return (WordItem *) subject();
}


const WordItem * WordView::word() const
{
  return (const WordItem *) subject();
}


IString WordView::dumpString() const
{
  const WordItem * word = (const WordItem *) subject();
  return debugString( IString( "WordView:" ) + word->string() );
}

