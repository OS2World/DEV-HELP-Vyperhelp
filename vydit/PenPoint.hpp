/***************************************************************************
 * File...... PenPoint.hpp
 * Author.... Mat
 * Date...... 6/24/97
 *
 * Extends IPoint to provide common functions needed by Pen, including
 *   - offsetting by a given y-value
 *   - mainting a set/not state
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PENPOINT_HPP
#define PENPOINT_HPP

// OpenClass
#include <ipoint.hpp>       // IPoint


class PenPoint: public IPoint
{
  public:
    // constructors
    PenPoint();
    PenPoint( const IPair & pair );

    // "set" state
    PenPoint & set( const IPair & pair );
    Boolean    isSet() const;

    // offset operation
    PenPoint & offsetY( Coord yOffset );

  private:
    Boolean _isSet;
};


// inline functions
#include "PenPoint.ipp"


#endif

