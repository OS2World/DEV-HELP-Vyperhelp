/***************************************************************************
 * File...... WordViewCursor.cpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * Implementation for WordViewCursor
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "CharView.hpp"
#include "WordView.hpp"
#include "WordViewCursor.hpp"


WordViewCursor::WordViewCursor( WordView * word ):
    _index( 0 ),
    _charView( word, this ),
    _wordView( word )
{}


WordViewCursor::~WordViewCursor()
{
  _wordView->cursorDeleted( this );
}


void WordViewCursor::setWord( WordView * word )
{
  _wordView = word;
  _charView.setParent( word );
}


IBoolean WordViewCursor::isLast() const
{
  return ( _index && _index == _wordView->length() );
}


// assumes that the viewToFind is a CharView
// looks for matching parent and index
IBoolean WordViewCursor::locate( const View * viewToFind )
{
  setToFirst();
  while ( isValid() )
  {
    if ( *view() == *(const CharView *) viewToFind )
      return true;
    setToNext();
  }
  return false;
}


IBoolean WordViewCursor::setToLast ()
{
  if ( _wordView->length() )
    _index = _wordView->length();
  else
    _index = 0;  // invalidate
  return ( _index > 0 );
}


IBoolean WordViewCursor::setToPrevious ()
{
  if ( _index )
    _index--;
  return ( _index > 0 );
}


IBoolean WordViewCursor::setToFirst ()
{
  if ( _wordView->length() )
    _index = 1;
  else
    _index = 0;  // invalidate
  return ( _index > 0 );
}


IBoolean WordViewCursor::setToNext ()
{
  if ( _index && _index < _wordView->length() )
    _index++;
  else
    _index = 0;
  return ( _index > 0 );
}


