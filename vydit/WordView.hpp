/***************************************************************************
 * File...... WordView.hpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * WordView handles the view of an individual word.  The WordViewCursor
 * is used to iterate through characters in the word.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef WORDVIEW_HPP
#define WORDVIEW_HPP

// TextEditor
#include "WordItem.hpp"
#include "PlacedView.hpp"
#include "WordViewCursor.hpp"
#include "WordViewCursorSet.hpp"

class WordView: public PlacedView
{
  public:
    // constructor
    WordView( WordItem * subject, View * parent );

    // support for WordViewCursor and CharView
    // index is 0-based
    Coord      charOffset( unsigned index ) const;
    Coord      charWidth( const unsigned index ) const;
    void       insertItemAtChar( Item * item, const unsigned index );
    unsigned   length() const;
    void       cursorDeleted( WordViewCursor * cursor );
      // called from WordViewCursor's destructor to remove from collection

    // from PlacedView
    virtual PlacedView::FormatChange format( FormatPen & pen );
    virtual PlacedView::LineMode     lineMode() const;

    // from SubjectView
    virtual void dispatchNotificationEvent( INotificationId id, const IEventData & event = IEventData() );

    // from View
    virtual WordViewCursor *   newCursor();
    virtual ViewCursor *       newCorrelateCursor( IPoint & point );
    virtual ViewCursor *       newLeafCursor( const Boolean wantFirst );
    virtual int                orderViews( View & view1, View & view2 );
    virtual void               draw( DrawPen & pen, const IRectangle & rect );
    virtual Coord              width() const;
    virtual Coord              height() const;
    virtual Coord              descent() const;
    virtual unsigned           remove( ViewCursor * fromCursor, ViewCursor * toCursor );
    virtual void               generate( View * from, View * to, unsigned & level );
    virtual IString            dumpString() const;

  private:
    // helpers
    Boolean updateIndex( unsigned & index, INotificationId id, const IEventData & event );
    void    handleMerge( WordItem * previousWord );

    // specialized access to subject
    WordItem * word();
    const WordItem * word() const;

    unsigned          _length;            // number of characters in the word
    WordViewCursorSet _cursorSet;
};


// inline functions
#include "WordView.ipp"


#endif
