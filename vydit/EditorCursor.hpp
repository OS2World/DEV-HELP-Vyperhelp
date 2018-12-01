/***************************************************************************
 * File...... EditorCursor.hpp
 * Author.... Mat
 * Date...... 10/16/97
 *
 *
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef EDITORCURSOR_HPP
#define EDITORCURSOR_HPP

// OpenClass
#include <isynonym.hpp>     // Boolean

// TextEditor
class Editor;
class ViewCursor;
class PlacedView;


class _Export EditorCursor
{
  public:
    // constructor
    EditorCursor( Editor & editor, Boolean isEndpoint = false );
    ~EditorCursor();

    // assignment
    EditorCursor & operator=( EditorCursor & copy );

    // cursor control
    void         set( ViewCursor * cursor );
      // set the cursor to the given ViewCursor (takes over ownership)
    Boolean      move( Boolean moveRight );
      // move the cursor to the right or left
      // return true if the move was successful and cursor is still set
    void         skip();
      // skip forward past any transient views
    void         unskip();
      // undo the previous skip, if possible
    void         clear();
      // clear/delete the cursor
    ViewCursor * give();
      // return pointer to current cursor, and clear internally
      // give up ownership of cursor

    // query
    ViewCursor * cursor();
      // return pointer to current cursor, 0 if not set
    Boolean      isSet() const;
      // return true if the cursor is set
    View *       leaf();
      // return the cursored leaf View, 0 if cursor is not set
    PlacedView * placed();
      // return the corresponding PlacedView, 0 if cursor is not set
    Boolean      isWord();
      // return true if the cursor is on a word
    Boolean      isPush();
      // return true if the cursor is on a push
    Boolean      isPop();
      // return true if the cursor is on a pop

  private:
    Boolean      _isEndpoint;
    Boolean      _isSkipped;
    Editor &     _editor;
    ViewCursor * _cursor;
};


// inline functions
#include "EditorCursor.ipp"


#endif

