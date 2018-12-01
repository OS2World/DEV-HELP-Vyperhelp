/***************************************************************************
 * File...... SmartText.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of SmartText.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Standard C
#include <ctype.h>

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <istrtest.hpp>     // IStringTest

// TextEditor
#include "FlowItem.hpp"
#include "NewLineItem.hpp"
#include "ParagraphItem.hpp"
#include "SpaceItem.hpp"
#include "WordItem.hpp"
#include "SymbolItem.hpp"
#include "SmartText.hpp"


SmartText::SmartText( const IString & string ):
    _string( string ),
    _lastItemIndex( 0 ),
    _lastStringIndex( 1 )
{
  scanString();
}


/***************************************************************************
 * Procedure.. SmartText::createItem
 * Author..... Mat
 * Date....... 9/19/96
 *
 * Returns a new Item corresponding to the given item index into the text.
 * If the index is not valid, an exception is thrown.
 ***************************************************************************/
Item * SmartText::createItem( unsigned itemIndex, FlowItem * parent )
{
  unsigned  indexFound;
  ItemFound found;
  Item *    item;

  // find the item
  found = findItem( itemIndex, indexFound );
  IASSERTSTATE( found != noItem );
  switch ( found )
  {
    case spaceItem:
    {
      item = new SpaceItem( parent );
      break;
    }
    case newLineItem:
    {
      item = new NewLineItem( parent );
      break;
    }
    case paragraphItem:
    {
      item = new ParagraphItem( parent );
      break;
    }
    case symbolItem:
    {
      unsigned char * buffer = _string;
      item = new SymbolItem( Symbol( buffer[ indexFound - 1 ] ), parent );
      break;
    }
    case wordItem:
    {
      IStringTest testAlphanumeric( isalnum );
      unsigned    indexEnd = _string.indexOfAnyBut( testAlphanumeric, indexFound );
      if ( indexEnd == 0 )
        indexEnd = _string.length() + 1;
      item = new WordItem( parent, _string.subString( indexFound, indexEnd - indexFound ) );
      break;
    }
  }

  return item;
}


SmartText & SmartText::removeItem( unsigned itemIndex )
{
  unsigned  index;
  unsigned  nextIndex;
  ItemFound found;

  // find the item
  found = findItem( itemIndex, index );
  IASSERTSTATE( found != noItem );

  // find the item after
  found = findItem( itemIndex + 1, nextIndex );

  // remove that portion of the string
  if ( found == noItem )
  {
    // remove the end of the string
    _string.remove( index );
  }
  else
  {
    // remove a sub-portion of the string
    _string.remove( index, nextIndex - index );
  }

  // decrement item count
  _numItems--;

  return *this;
}


SmartText & SmartText::setString( const IString & string )
{
  _string = string;

  // rescan string and reset indices form last search
  scanString();
  _lastItemIndex = 0;
  _lastStringIndex = 1;

  return *this;
}


/***************************************************************************
 * Procedure.. SmartText::findItem
 * Author..... Mat
 * Date....... 9/17/96
 *
 * Finds the specified item (using 0-based index), returns the type of item
 * found, and sets the string index (1-based) of the item.
 ***************************************************************************/
SmartText::ItemFound SmartText::findItem( unsigned itemIndex, unsigned & stringIndex )
{
  ItemFound found;
  unsigned  count = 0;
  unsigned  index = 1;  // starting string index

  // use indices from last call if possible
  if ( _lastItemIndex <= itemIndex )
  {
    count = _lastItemIndex;
    index = _lastStringIndex;
  }

  // skip all items before itemIndex
  while ( count < itemIndex )
  {
    found = nextItem( index );
    IASSERTSTATE( found != noItem );
    count++;
  } /* endwhile */

  // save these indices
  _lastItemIndex = count;
  _lastStringIndex = index;

  // get this item
  stringIndex = index;
  return nextItem( index );
}


/***************************************************************************
 * Procedure.. SmartText::nextItem
 * Author..... Mat
 * Date....... 9/17/96
 *
 * starting at string index, look for next item
 * if found, return ItemFound and set new index
 * if not found return "noItem"
 * index is a 1-based IString index
 *
 ***************************************************************************/
SmartText::ItemFound SmartText::nextItem( unsigned & index ) const
{
  unsigned len = _string.length();

  // check for end of string
  if ( index > len )
  {
    return noItem;
  }

  // check for space or newline item
  unsigned char * buffer = _string;
  unsigned char ch = buffer[index-1];
  switch ( ch )
  {
    case ' ':
      // space
      index++;
      return spaceItem;
    case '\n':
      // newline
      index++;
      return newLineItem;
    case '\r':
      // carriage return (paragraph)
      index++;
      return paragraphItem;
  }

  // check for non-alphanumeric (symbol)
  if ( ! isalnum( ch ) )
  {
    index++;
    return symbolItem;
  }

  // otherwise skip the word item
  IStringTest testAlphanumeric( isalnum );
  index = _string.indexOfAnyBut( testAlphanumeric, index );
  if ( ! index )
  {
    // last word and no space after
    index = len + 1;
  }
  return wordItem;
}



/***************************************************************************
 * Procedure.. SmartText::scanString
 * Author..... Mat
 * Date....... 9/17/96
 *
 * Called when the working string is changed.  Re-scan the string and
 * convert to blanks where necessary.  Determine and save # items.
 * Convert all whitespace to simple blanks, leave newlines alone.
 * Does not strip leading/trailing whitespace like IString::space().
 ***************************************************************************/
SmartText & SmartText::scanString()
{
  unsigned        i;
  unsigned        len = _string.length();
  unsigned char * buffer = _string;   // get pointer to string buffer
  ItemFound       lastFound = noItem;

  _numItems = 0;
  for ( i = 0; i < len; i++ )
  {
    ItemFound     found = wordItem;
    unsigned char c = buffer[i];

    // check the next char
    if ( c == '\n' )
      found = newLineItem;
    else if ( c == '\r' )
      found = paragraphItem;
    else if ( isspace( c ) )
    {
      // convert white space other than newline to blanks (safe for DBCS?)
      buffer[i] = ' ';
      found = spaceItem;
    }
    else if ( ! isalnum( c ) )
      found = symbolItem;

    // increment item count if applicable
    if ( found != wordItem || lastFound != wordItem )
      _numItems++;

    // save this ItemFound
    lastFound = found;
  }

  return *this;
}

