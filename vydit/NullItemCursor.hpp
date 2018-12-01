/***************************************************************************
 * File...... NullItemCursor.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * NullItemCursor is used as an cursor for items which do not support nesting.
 * The cursor is always in an "invalid" state.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef NULLITEMCURSOR_HPP
#define NULLITEMCURSOR_HPP


#include "ItemCursor.hpp"


class NullItemCursor: public ItemCursor
{
  public:
    // exception throwing
    void accessError() const;

    // from ItemCursor
    virtual Item * item() const;

    // ala IOrderedCursor
    virtual IBoolean setToLast ();
    virtual IBoolean setToPrevious ();

    // ala ICursor
    virtual IBoolean setToFirst();
    virtual IBoolean setToNext();
    virtual IBoolean isValid() const;
    virtual void invalidate();
};


// inline functions
#include "NullItemCursor.ipp"


#endif

