/***************************************************************************
 * File...... Distance.cpp
 * Author.... Mat
 * Date...... 10/20/95
 *
 * Implementation for Distance and Position classes
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// MekTek
#include "mektek.hpp"

// Generator
#include "Distance.hpp"


/***************************************************************************
 * Class...... Distance
 ***************************************************************************/
Distance::Distance( int length, Units units ):
    _units( units ),
    _length( length )
{}


/***************************************************************************
 * Procedure.. Distance::asTwips
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Convert the given Distance to twips, using the best guess when enough
 * information is not known.
 ***************************************************************************/
int Distance::asTwips( Boolean isHorizontal ) const
{
  // convert distance to twips
  switch ( _units )
  {
  case Distance::twips:
    return _length;

  case Distance::chars:
    // assume 10 pt text
    // assume width = height/2
    return isHorizontal? (_length * 100): (_length * 200);

  case Distance::pixels:
    // assume 20 twips per pixel (72 dpi)
    return _length * 20;

  case Distance::percent:
    // assume about 7 inch full width/height (10080 twips)
    return _length * 100;

  default:
    BADSWITCH;
    break;
  }

  return 0;
}


/***************************************************************************
 * Procedure.. Distance::asWinCoord
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Convert the given to WinHelp's 0-1023 coordinate system.
 ***************************************************************************/
unsigned Distance::asWinCoord() const
{
  unsigned full;

  // convert distance to 1023 coordinates
  switch ( _units )
  {
  case Distance::twips:
    // assume about 7 inch full width/height (10080 twips)
    full = 10080;
    break;

  case Distance::chars:
    // assume 50 char width/height
    full = 50;
    break;

  case Distance::pixels:
    // assume 500 pixel height/width
    full = 500;
    break;

  case Distance::percent:
    full = 100;
    break;

  default:
    BADSWITCH;
    return 0;
  }

  // scale the result
  unsigned result = _length * 1024 / full;
  if ( result > 1023 )
    result = 1023;

  return result;
}

/***************************************************************************
 * Class...... Position
 ***************************************************************************/
Position::Position( Dynamic coord ):
    _isDynamic( true ),
    _dynamic( coord ),
    _distance( 0, Distance::twips )  // dummy distance
{}

Position::Position( const Distance & coord ):
    _isDynamic( false ),
    _dynamic( min ),      // dummy dynamic setting
    _distance( coord )
{}


/***************************************************************************
 * Procedure.. Position::asWinCoord
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Convert the given to WinHelp's 0-1023 coordinate system.
 ***************************************************************************/
unsigned Position::asWinCoord() const
{
  static const maxCoord = 1023;

  // handle Distance
  if ( ! _isDynamic )
    return _distance.asWinCoord();

  switch ( _dynamic )
  {
  case Position::min:
    return 0;

  case Position::max:
    return maxCoord;

  case Position::center:
  case Position::cursor:
    // assume cursor = center
    return maxCoord / 2;

  default:
    BADSWITCH;
  }

  return 0;
}



