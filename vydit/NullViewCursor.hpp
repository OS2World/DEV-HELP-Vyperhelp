/***************************************************************************
 * File...... NullViewCursor.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * Default simple cursor for views which have no sub-views.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef NULLVIEWCURSOR_HPP
#define NULLVIEWCURSOR_HPP


#include "ViewCursor.hpp"


class NullViewCursor: public ViewCursor
{
  public:
    // exception throwing
    void accessError() const;

    // from ViewCursor
    virtual View * view();
    virtual const View * view() const;
    virtual IBoolean isFirst() const;
    virtual IBoolean isLast() const;

    // from IOrderedCursor
    virtual IBoolean setToLast();
    virtual IBoolean setToPrevious();

    // from ICursor
    virtual IBoolean setToFirst();
    virtual IBoolean setToNext();
    virtual IBoolean isValid() const;
    virtual void invalidate();
};


// inline functions
#include "NullViewCursor.ipp"


#endif

