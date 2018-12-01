/***************************************************************************
 * File...... SmartText.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * SmartText holds an IString and "knows" about Items and is able
 * to parse the string into Items.  SmartText recognizes the following
 * items in a string:
 *   SpaceItem (any white space)*
 *   NewLineItem (\n)
 *   ParagraphItem (\r)
 *   WordItem (alphanumeric string)
 *   SymbolItem (none of the above)
 *
 * (*) When the string is set, all white space is converted to the space
 * character.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef SMARTTEXT_HPP
#define SMARTTEXT_HPP


// OpenClass
#include <istring.hpp>      // IString

// TextEditor
class Item;
class FlowItem;


class SmartText
{
  public:
    // constructor
    SmartText( const IString & string = IString() );

    // item parser
    unsigned    numItems() const;
      // returns the number of Items which can be parsed from the string
    Item *      createItem( unsigned index, FlowItem * parent );
      // create an Item to represent the nth element (0-based index)
      // this is optimized for being called with increasing index
    SmartText & removeItem( unsigned index );
      // removes the nth element from the string

    // string holder
    const IString & string() const;
      // returns the working string
    SmartText &     setString( const IString & string );
      // changes the working string

  private:
    // parse helpers
    enum ItemFound {
      noItem,
      spaceItem,
      newLineItem,
      paragraphItem,
      symbolItem,
      wordItem
    };
    ItemFound   findItem( unsigned itemIndex, unsigned & stringIndex );
    ItemFound   nextItem( unsigned & stringIndex ) const;
    SmartText & scanString();

    // string holder
    IString  _string;
    unsigned _numItems;

    // matching item and string index for last createItem() call
    unsigned _lastItemIndex;    // 0-based item index
    unsigned _lastStringIndex;  // 1-based string index
};


// inline functions
#include "SmartText.ipp"


#endif

