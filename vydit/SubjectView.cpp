/***************************************************************************
 * File...... SubjectView.cpp
 * Author.... Mat
 * Date...... 9/9/97
 *
 * Implementation of SubjectView.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "Item.hpp"
#include "ViewCursor.hpp"
#include "SubjectView.hpp"


SubjectView::SubjectView( Item * subject, View * parent ):
    View( parent ),
    _subject( subject )
{
  if ( _subject )
    _subject->addObserver( *this );
}


SubjectView::~SubjectView()
{
  if ( _subject )
    _subject->removeObserver( *this );
}


unsigned SubjectView::remove( ViewCursor * fromCursor, ViewCursor * toCursor )
{
  // delete cursors if they exist
  delete fromCursor;
  delete toCursor;

  // save subject and remove reference to avoid circular deletion
  Item * subject = _subject;
  _subject = 0;

  // now delete the subject (this View will get deleted when it gets notified)
  delete subject;

  // no view was orphaned
  return 0;
}


void SubjectView::dispatchNotificationEvent( INotificationId id, const IEventData & event )
{
  if ( id == Item::deleteId )
    removeSelf();
  else if ( id == Item::changeId )
    parent()->update( *this );
}


