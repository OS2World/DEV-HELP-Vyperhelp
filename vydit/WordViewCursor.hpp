/***************************************************************************
 * File...... WordViewCursor.hpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * WordViewCursor allows enumeration of the characters in a word.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef WORDVIEWCURSOR_HPP
#define WORDVIEWCURSOR_HPP

// TextEditor
#include "CharView.hpp"
#include "ViewCursor.hpp"
class WordView;


class WordViewCursor: public ViewCursor
{
  public:
    // constructor
    WordViewCursor( WordView * view );
    virtual ~WordViewCursor();

    // CharView support
    unsigned index() const;
    void     setIndex( unsigned index );
    void     setWord( WordView * word );

    // from ViewCursor
    virtual CharView * view();
    virtual const CharView * view() const;
    virtual IBoolean   isFirst() const;
    virtual IBoolean   isLast() const;
    virtual IBoolean   locate( const View * viewToFind );
    virtual IBoolean   setToLast ();
    virtual IBoolean   setToPrevious ();
    virtual IBoolean   setToFirst ();
    virtual IBoolean   setToNext ();
    virtual IBoolean   isValid () const;
    virtual void       invalidate ();

  private:
    unsigned   _index;        // 1-based, 0 = invalid
    CharView   _charView;
    WordView * _wordView;
};


// inline fuinctions
#include "WordViewCursor.ipp"


#endif

