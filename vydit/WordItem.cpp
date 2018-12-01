/***************************************************************************
 * File...... WordItem.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of WordItem.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// ANSI C
#include <limits.h>

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "FlowItem.hpp"
#include "WordView.hpp"
#include "WordItem.hpp"


WordItem::WordItem( Item * parent, const IString & string ):
  Item( parent ),
  _string( string )
{}


// appends a single word
WordItem & WordItem::appendWord( const IString & word )
{
  // append new text
  unsigned insertionIndex = _string.length();
  _string += word;
  notifyOfInsertChars( insertionIndex, word.length() );
  change( 0 );

  return *this;
}


void WordItem::mergeWords( WordItem & previousWord )
{
  // notify of "insert" to update all cursor indices
  notifyOfInsertChars( 0, previousWord.string().length() );

  // append the text from this word to the previous word
  previousWord.appendWord( _string );

  // send "merge" notification to transfer ownership of cursors
  notifyObservers( mergeWordsId, IEventData( &previousWord ) );

  // delete self
  delete this;
}


// index gives index into the string after which the new item should be inserted
// index = 0 is used to insert at front
WordItem & WordItem::insertItem( Item * item, unsigned index )
{
  // assume that a valid index was given
  // (last index is not valid)
  IASSERTPARM( index < _string.length() );

  // for another WordItem, string can simply be inserted (much faster!)
  WordItem * word = item->asWord();
  if ( word )
  {
    // insert text and notify of insertion
    _string.insert( word->string(), index );
    notifyOfInsertChars( index, word->string().length() );
    // delete the new item (it's no longer needed)
    delete item;
    // nothing else to do in this case
    change( 0 );
    return *this;
  }

  // otherwise check if we need to split the current word
  if ( index )
  {
    // ask parent to insert first part as a separate WordItem
    flow()->insertItem( new WordItem( flow(), _string.subString( 1, index ) ), this, false );
    // now remove it from this WordItem
    deleteText( 1, index );
  }

  // pass item up to parent FlowView
  flow()->insertItem( item, this );

  return *this;
}


// delete text from index1 through index2 (1-based indices)
WordItem & WordItem::deleteText( unsigned index1, unsigned index2 )
{
  IASSERTPARM( index1 <= index2 );

  _string.remove( index1, index2 - index1 + 1);
  notifyOfDeleteChars( index1, index2 );
  change( 0 );

  return *this;
}



// index gives index into the string after which the text should be inserted
// index = 0 is used to insert at front
WordItem & WordItem::notifyOfInsertChars( unsigned index, unsigned length )
{
  // assume all strings < 64K
  IASSERTPARM( index <= USHRT_MAX );
  IASSERTPARM( length <= USHRT_MAX );

  // cast to shorts
  unsigned short insertionIndex = index;
  unsigned short insertionLength = length;

  // notify of character insertion
  IEventData eventData( insertionIndex, insertionLength );
  notifyObservers( insertCharsId, eventData );

  return *this;
}


WordItem & WordItem::notifyOfDeleteChars( unsigned firstIndex, unsigned lastIndex )
{
  // assume all strings < 64K
  IASSERTPARM( firstIndex <= USHRT_MAX );
  IASSERTPARM( lastIndex <= USHRT_MAX );

  // cast to shorts
  unsigned short index1 = firstIndex;
  unsigned short index2 = lastIndex;

  // notify of character insertion
  IEventData eventData( index1, index2 );
  notifyObservers( deleteCharsId, eventData );

  return *this;
}


View * WordItem::newView( View * parent )
{
  return new WordView( this, parent );
}


FlowItem * WordItem::flow()
{
  FlowItem * flow = parent()->asFlow();
  IASSERTSTATE( flow );
  return flow;
}


void WordItem::generate()
{
  flow()->sendText( _string );
}


// generate a substring
void WordItem::generate( unsigned index1, unsigned index2 )
{
  IASSERTSTATE( index1 <= index2 );
  flow()->sendText( _string.subString( index1, index2 - index1 + 1 ) );
}


IString WordItem::dumpString() const
{
  return IString( "WordItem(" ) + _string + IString( ")" );
}


const INotificationId WordItem::insertCharsId = "WordItem::insertChars";
const INotificationId WordItem::deleteCharsId = "WordItem::deleteChars";
const INotificationId WordItem::mergeWordsId = "WordItem::mergeWords";

