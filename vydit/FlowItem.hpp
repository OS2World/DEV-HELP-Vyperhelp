/***************************************************************************
 * File...... FlowItem.hpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * Flow is the basic "meta-item" which handles a sequence of other items
 * and flows them into a specified area.  Flow is used as the top-level
 * Item for the editor.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef FLOWITEM_HPP
#define FLOWITEM_HPP

// OpenClass
#include <istring.hpp>      // IString

// Generator
#include "ArtSet.hpp"
class Generator;
class Gin;

// TextEditor
#include "Types.hpp"
#include "OrderedItemCursor.hpp"
#include "Item.hpp"
class View;
class FlowView;
class SmartText;
class WordItem;


class _Export FlowItem: public Item
{
  public:
    // constructor
    FlowItem( Item * parent );

    // manipulator
    void insertItem( Item * newItem, Item * itemAfter, Boolean checkWords = true );
    void appendItem( Item * newItem );
    void appendText( const IString & text );
    void reset();

    // art support
    const ArtDef * getArt( const IString & id );
    void           addArt( const ArtDef & art );
    Boolean        deleteArt( const IString & id );

    // helpers for Generator
    void sendGin( const Gin & gin );
    void sendText( const IString & string );
    void setGenerator( Generator * gen );
    void flushText();
    void finishGenerate( Boolean allDone = true );

    // insert item notification
    static const INotificationId insertItemId;
      // IEventData( OrderedItemCursor * cursorToFirstNewItem )

    // size
    INumber numberOfItems() const;

    // from Item
    virtual OrderedItemCursor * newCursor();
    virtual View *              newView( View * parent );
    virtual FlowItem *          asFlow();
    virtual void                removeChild( Item * item );
    virtual void                generate();
    virtual IString             dumpString() const;

  private:
    // contents
    Item *       _parent;  // NULL if top item
    ItemSequence _children;

    // art collection
    ArtSet       _arts;

    // output
    IString      _output;
    Generator *  _gen;
};


// Inline functions
#include "FlowItem.ipp"


#endif
