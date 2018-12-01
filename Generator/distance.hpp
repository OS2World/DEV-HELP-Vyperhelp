/***************************************************************************
 * File...... Distance.hpp
 * Author.... Mat
 * Date...... 10/5/95
 *
 * Distance and Position with support for multiple units and dynamic
 * positioning.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#ifndef DISTANCE_HPP
#define DISTANCE_HPP

// OpenClass
#include <isynonym.hpp>     // Boolean


/***************************************************************************
 * Class...... Distance
 * Author..... Mat
 * Date....... 9/26/95
 *
 * A distance measurement and units.  The Generator may need to be able to
 * convert between units to get them in proper units for the target format.
 ***************************************************************************/
class _Export Distance
{
  public:
    // enumerators
    enum Units {
      twips,     // 1/1440 inch
      chars,     // average character height/width
      pixels,    // screen pixels
      percent    // percentage of screen or window height/width
    };

    // constructor
    Distance( int length = 0, Units units = twips );

    // query functions
    int   length() const;
    Units units() const;

    // conversion functions
    int asTwips( Boolean isHorizontal = true ) const;
    unsigned asWinCoord() const;

  private:
    Units _units;
    int   _length;
};



/***************************************************************************
 * Class...... Position
 * Author..... Mat
 * Date....... 9/26/95
 *
 * A position is an x- or y-coordinate, which can one of the enumerated
 * Dynamic values, or a distance from the origin.
 ***************************************************************************/
class _Export Position
{
  public:
    // enumerators
    enum Dynamic {
      min,       // flush left/top
      max,       // flush right/bottom
      center,    // centered
      cursor     // positioned at user's cursor
    };

    // constructor
    Position( Dynamic coord = min );
    Position( const Distance & coord );

    // query functions
    Boolean          isDynamic() const;
    Dynamic          asDynamic() const;
    const Distance & asDistance() const;

    // conversion functions
    unsigned asWinCoord() const;

  private:
    Boolean  _isDynamic;
    Dynamic  _dynamic;
    Distance _distance;
};


// inline functions
#include "Distance.ipp"


#endif

