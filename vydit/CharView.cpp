/***************************************************************************
 * File...... CharView.cpp
 * Author.... Mat
 * Date...... 5/2/96
 *
 * Implementation of CharView.  _index is the 0-based index into the
 * WordView's string.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "WordView.hpp"
#include "CharView.hpp"


CharView::CharView( WordView * parent, WordViewCursor * owner ):
    View( parent ),
    _owner( owner )
{}


// return true if parent and index are both the same
int CharView::operator==( const CharView & view ) const
{
  return ( ( parent() == view.parent() ) && ( index() == view.index() ) );
}


// return 0-based index (owner provide 1-based index)
unsigned CharView::index() const
{
  return _owner->index() - 1;
}


// cast to a WordView
WordView * CharView::word() const
{
  return (WordView *) parent();
}


IPoint CharView::position() const
{
  return IPoint( word()->charOffset( index() ), 0 );
}


Coord CharView::width() const
{
  return word()->charWidth( index() );
}


Coord CharView::height() const
{
  return parent()->height();
}


Coord CharView::descent() const
{
  return parent()->descent();
}


void CharView::insertItem( Item * item )
{
  word()->insertItemAtChar( item, index() );
}


ViewCursor * CharView::newLeafCursor( const Boolean wantFirst )
{
  return newCursorToThis();
}


IString CharView::dumpString() const
{
  return debugString( "CharView" );
}

