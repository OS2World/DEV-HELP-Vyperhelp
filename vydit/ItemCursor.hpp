/***************************************************************************
 * File...... ItemCursor.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * Interface for traversing sub-items within an item.  Also defined here
 * is a standard sequence of Items
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ITEMCURSOR_HPP
#define ITEMCURSOR_HPP


// OpenClass
#include <icursor.h>        // ICursor

// TextEditor
class Item;


class _Export ItemCursor
{
  public:
    // virtual destructor
    virtual ~ItemCursor();

    // element access, only valid while cursor is not moved or destroyed
    virtual Item * item() const = 0;

    // ala IOrderedCursor
    virtual IBoolean setToLast () = 0;
    virtual IBoolean setToPrevious () = 0;

    // ala ICursor
    virtual IBoolean setToFirst() = 0;
    virtual IBoolean setToNext() = 0;
    virtual IBoolean isValid() const = 0;
    virtual void invalidate() = 0;
};


#endif

