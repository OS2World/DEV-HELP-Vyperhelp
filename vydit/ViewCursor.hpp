/***************************************************************************
 * File...... ViewCursor.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * Interface for traversing sub-views of a view.  Emulates IOrderedCursor
 * interface from OpenClass.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef VIEWCURSOR_HPP
#define VIEWCURSOR_HPP

// OpenClass
#include <icursor.h>        // ICursor

// TextEditor
class View;


class ViewCursor
{
  public:
    // virtual destructor
    virtual ~ViewCursor();

    // access to element, only valid while cursor is not moved or destroyed
    virtual View * view() = 0;
    virtual const View * view() const = 0;

    // additional query functions
    virtual IBoolean isFirst() const = 0;
      // returns true if cursor is valid & first in view
    virtual IBoolean isLast() const = 0;
      // returns true if cursor is valid & last in view
    virtual IBoolean locate( const View * );
      // sets cursor to match supplied View, returns true if found

    // ala IOrderedCursor
    virtual IBoolean setToLast () = 0;
    virtual IBoolean setToPrevious () = 0;

    // ala ICursor
    virtual IBoolean setToFirst () = 0;
    virtual IBoolean setToNext () = 0;
    virtual IBoolean isValid () const = 0;
    virtual void     invalidate () = 0;
};


#endif

