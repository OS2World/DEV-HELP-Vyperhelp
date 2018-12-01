/***************************************************************************
 * File...... OrderedViewCursor.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * ViewCursor for an ordered collection.  Wraps an OpenClass cursor (see
 * the "Decorator" pattern in Design Patterns).
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ORDEREDVIEWCURSOR_HPP
#define ORDEREDVIEWCURSOR_HPP


// OpenClass
#include <icursor.h>

// TextEditor
#include "View.hpp"
#include "ViewSequence.hpp"
#include "ViewCursor.hpp"


class OrderedViewCursor: public ViewCursor
{
  public:
    // constructor
    OrderedViewCursor( const ViewSequence & );
    OrderedViewCursor( const OrderedViewCursor & );
    virtual ~OrderedViewCursor();

    // copy assignment
    OrderedViewCursor & operator=( const OrderedViewCursor & );

    // cursor access
    IOrderedCursor<View*>       * cursor();
    const IOrderedCursor<View*> * cursor() const;

    // comparison
    IBoolean operator == ( OrderedViewCursor const & c ) const;

    // from ViewCursor
    virtual View * view();
    virtual const View * view() const;
    virtual IBoolean isFirst() const;
    virtual IBoolean isLast() const;
    virtual IBoolean locate( const View * );

    // ala IOrderedCursor
    virtual IBoolean setToLast ();
    virtual IBoolean setToPrevious ();

    // ala ICursor
    virtual IBoolean setToFirst ();
    virtual IBoolean setToNext ();
    virtual IBoolean isValid () const;
    virtual void     invalidate ();

  private:
    const ViewSequence &    _sequence;
    IOrderedCursor<View*> * _cursor;
};


// inline functions
#include "OrderedViewCursor.ipp"


#endif

