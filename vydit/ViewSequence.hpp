/***************************************************************************
 * File...... ViewSequence.hpp
 * Author.... Mat
 * Date...... 4/18/96
 *
 * Generic sequence and cursor for views
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef VIEWSEQUENCE_HPP
#define VIEWSEQUENCE_HPP


// OpenClass
#include <ieqseq.h>         // IEqualitySequence

// TextEditor
class View;
typedef IEqualitySequence<View*> ViewSequenceBase;


class ViewSequence: public ViewSequenceBase
{
  public:
    // constructor
    ViewSequence( INumber numberOfElements = 500 );
    ~ViewSequence();

    // delete and remove
    void deleteAndRemove( IOrderedCursor<View*> & cursor );

    // determine ordering
    int order( View & view1, View & view2 );
      // return <0 if view1 less than view2, 0 if equal, >0 if greater
};


#endif

