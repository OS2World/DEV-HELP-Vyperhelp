/***************************************************************************
 * File...... WordItem.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * WordItem represents a single word.  WordItems are always contained within
 * FlowItems.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef WORDITEM_HPP
#define WORDITEM_HPP

// OpenClass
#include <istring.hpp>      // IString

// TextEditor
#include "Item.hpp"
class FlowItem;


class _Export WordItem: public Item
{
  public:
    // constructor
    WordItem( Item * parent, IString const & string = IString() );

    // query functions
    const IString & string() const;

    // manipulators
    WordItem & insertItem( Item * item, unsigned index );
      // inserts item before the specified index, checks for merge with WordItem
    WordItem & appendWord( const IString & word );
      // appends a single word to the current word
    WordItem & deleteText( unsigned index1, unsigned index2 );
      // delete text from index1 through index2
    void       mergeWords( WordItem & previousWord );
      // merge this word into the previous (adjacent) by appending it

    // notifications
    static const INotificationId insertCharsId;
      // IEventData( unsigned short index, unsigned short length )
    static const INotificationId deleteCharsId;
      // IEventData( unsigned short firstIndex, unsigned short lastIndex )
    static const INotificationId mergeWordsId;
      // merge into preceding word
      // IEventData( WordItem * precedingWord )

    // from Item
    virtual View *     newView( View * parent );
    virtual WordItem * asWord();
    virtual void       generate();
    virtual void       generate( unsigned index1, unsigned index2 );
    virtual IString    dumpString() const;

  private:
    IString _string;

    // helpers
    FlowItem * flow();
    WordItem & notifyOfInsertChars( unsigned index, unsigned length );
    WordItem & notifyOfDeleteChars( unsigned firstIndex, unsigned lastIndex );

    /// don't store the IString word here, instead keep
    ///   static dictionary list
    ///   index to dictionary and word

};


// inline functions
#include "WordItem.ipp"


#endif
