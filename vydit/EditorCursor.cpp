/***************************************************************************
 * File...... EditorCursor.cpp
 * Author.... Mat
 * Date...... 10/16/97
 *
 * Implementation of EditorCursor.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// TextEditor
#include "Item.hpp"
#include "LineView.hpp"
#include "Editor.hpp"
#include "ViewCursor.hpp"
#include "EditorCursor.hpp"


EditorCursor::EditorCursor( Editor & editor, Boolean isEndpoint ):
  _editor( editor ),
  _isEndpoint( isEndpoint ),
  _cursor( 0 )
{}


EditorCursor::~EditorCursor()
{
  delete _cursor;
}


EditorCursor & EditorCursor::operator=( EditorCursor & copy )
{
  set( copy.isSet()? copy.leaf()->newCursorToThis(): 0 );
  return *this;
}


void EditorCursor::set( ViewCursor * cursor )
{
  if ( _cursor )
  {
    if ( _isEndpoint && cursor )
      _editor.changeSelection( *_cursor, *cursor );
    delete _cursor;
  }

  _cursor = cursor;

  if ( _isEndpoint )
    _editor.refreshSelection();
}


Boolean EditorCursor::move( Boolean moveRight )
{
  if ( ! _cursor )
    return false;

  ViewCursor * leafCursor = 0;

  // get a copy of this cursor
  ViewCursor * viewCursor = leaf()->newCursorToThis();
  IASSERTSTATE( viewCursor );

  // clear existing cursor
  clear();

  do
  {
    Boolean noMove = moveRight? viewCursor->isLast(): viewCursor->isFirst();

    // try to move the current cursor
    if ( noMove )
    {
      // can't move, move up a level on the tree
      View * parent;
      ViewCursor * parentCursor = 0;
      parent = viewCursor->view()->parent();
      if ( parent )
        parentCursor = parent->newCursorToThis();

      // free the old cursor
      delete viewCursor;

      // if found a parent go to it, otherwise get out
      if ( parentCursor )
        viewCursor = parentCursor;
      else
        return false;
    }
    else
    {
      // move ahead/back and look for a leaf
      if ( moveRight )
        viewCursor->setToNext();
      else
        viewCursor->setToPrevious();
      leafCursor = viewCursor->view()->newLeafCursor( moveRight );
    }

  } while ( ! leafCursor );

  delete viewCursor;

  // save this cursor
  set( leafCursor );
  return true;
}


void EditorCursor::skip()
{
  if ( _cursor )
  {
    while ( leaf()->isTransient() )
    {
      Boolean ok = move( true );
      IASSERTSTATE( ok );
      _isSkipped = true;
    }
  }
}


/***************************************************************************
 * EditorCursor::unskip
 *
 * Undoes a previous skip() call.  CURRENTLY ASSUMES THE FOLLOWING:
 *   1. Only one skip needs to be undone.
 *   2. If the previous view is transient, then it is the skipped view
 ***************************************************************************/
void EditorCursor::unskip()
{
  if ( _isSkipped && _cursor )
  {
    EditorCursor test( _editor );
    test = *this;
    test.move( false );
    if ( test.leaf() && test.leaf()->isTransient() )
      *this = test;
  }
  _isSkipped = false;
}


void EditorCursor::clear()
{
  delete _cursor;
  _cursor = 0;
}


ViewCursor * EditorCursor::give()
{
  ViewCursor * result = _cursor;
  _cursor = 0;
  return result;
}


View * EditorCursor::leaf()
{
  return _cursor? _cursor->view(): 0;
}


// given a leaf view, find the placed view which it belongs to
// in many cases the leaf view is itself a placed view (IconView, SpaceView)
// an example where they are not the same is CharView and WordView
// find line, and then direct descendent, to determine if it is earlier
PlacedView * EditorCursor::placed()
{
  if ( _cursor )
  {
    View * leaf = _cursor->view();
    IASSERTSTATE( leaf );
    ViewCursor * lineCursor = leaf->newLineCursor();
    IASSERTSTATE( lineCursor );
    LineView * line = (LineView *) lineCursor->view();
    PlacedView * placedView = (PlacedView *) line->locateDescendent( *leaf );
    IASSERTSTATE( placedView );
    delete lineCursor;
    return placedView;
  }

  return 0;
}


Boolean EditorCursor::isWord()
{
  PlacedView * view = placed();
  if ( view )
  {
    Item * subject = view->subject();
    if ( subject )
      return ( subject->asWord() != 0 );
  }
  return false;
}


Boolean EditorCursor::isPush()
{
  PlacedView * view = placed();
  if ( view )
  {
    Item * subject = view->subject();
    if ( subject )
      return ( subject->asPush() != 0 );
  }
  return false;
}


Boolean EditorCursor::isPop()
{
  PlacedView * view = placed();
  if ( view )
  {
    Item * subject = view->subject();
    if ( subject )
      return ( subject->asPop() != 0 );
  }
  return false;
}


