/***************************************************************************
 * File...... CharView.hpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * CharView handles the view of a particular character in a Word.  There
 * is NO corresponding CharItem.  CharView is a "lightweight" object
 * which is created on demand to support the WordViewCursor.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef CHARVIEW_HPP
#define CHARVIEW_HPP

// TextEditor
class WordView;
class WordViewCursor;
#include "View.hpp"


class CharView: public View
{
  public:
    // constructor
    CharView( WordView * parent, WordViewCursor * owner );

    // comparison
    virtual int operator==( const CharView & ) const;

    // index support (0-based)
    unsigned index() const;

    // from View
    virtual void         insertItem( Item * item );
    virtual ViewCursor * newLeafCursor( const Boolean wantFirst );
    virtual void         removeSelf();
    virtual IPoint       position() const;
    virtual Coord        width() const;
    virtual Coord        height() const;
    virtual Coord        descent() const;
    virtual IString      dumpString() const;

  private:
    WordView * word() const;

    WordViewCursor * _owner;
};


// inline functions
#include "CharView.ipp"


#endif

