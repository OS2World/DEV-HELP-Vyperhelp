/***************************************************************************
 * File...... IPFDistance.cpp
 * Author.... Mat
 * Date...... 11/2/95
 *
 * Routines for converting generic Distances to IPF recognized units.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include <stdlib.h>
#include "IPF_Common.hpp"


/***************************************************************************
 * Procedure.. IPFGenerator::convertPosition
 * Author..... Mat
 * Date....... 10/20/95
 *
 * Converts a generic Position into an IPF acceptable position in
 * character form.  isX tels whether the position is on the x-axis
 * (horizontal).  Otherwise, a vertical position is returned.
 ***************************************************************************/
IString IPFGenerator::convertPosition( const Position & position, Boolean isX )
{
  if ( position.isDynamic() )
  {
    switch ( position.asDynamic() )
    {
    case Position::min:
      return isX? "left": "bottom";
    case Position::max:
      return isX? "right": "top";
    case Position::center:
    case Position::cursor:
      return "center";
    }
  }
  else
    return convertDistance( position.asDistance() );

  BADSWITCH;
  return IString();  // should never get here
}


/***************************************************************************
 * Procedure.. IPFGenerator::convertDistance
 * Author..... Mat
 * Date....... 10/20/95
 *
 * Converts a generic Distance to IPF acceptable distance in char form.
 ***************************************************************************/
IString IPFGenerator::convertDistance( const Distance & distance )
{
  switch ( distance.units() )
  {
  case Distance::twips:
    return IString( distance.length() / 20 ) + "p";  // points = twips/20
  case Distance::chars:
    return IString( distance.length() ) + "c";
  case Distance::pixels:
    return IString( distance.length() ) + "x";
  case Distance::percent:
    return IString( distance.length() ) + "%";
  }

  BADSWITCH;
  return IString();  // should never get here
}


/***************************************************************************
 * Procedure.. NearestDivide
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Divides to integers and rounds to the nearest result.
 ***************************************************************************/
static int NearestDivide( int top, int bottom )
{
  div_t result = div( top, bottom );
  if ( result.rem > bottom / 2 )
    return result.quot + 1;
  else
    return result.quot;
}


/***************************************************************************
 * Procedure.. IPFGenerator::charFromDistance
 * Author..... Mat
 * Date....... 10/31/95
 *
 * Given a Distance, determine the number of IPF chars (average character
 * width) it represents, using best guess.  Return a minimum of 1.
 ***************************************************************************/
unsigned int IPFGenerator::charsFromDistance( const Distance & distance )
{
  int divisor, chars;

  switch ( distance.units() )
  {
  case Distance::twips:
    divisor = 100;  // assume 10 pt text, width = height/2
    break;
  case Distance::chars:
    divisor = 1;
    break;
  case Distance::pixels:
    divisor = 20;   // assume 20 pixel width
    break;
  case Distance::percent:
    divisor = 2;    // assume 50 chars screen width
    break;
  default:
    BADSWITCH;
    break;
  }

  chars = NearestDivide( distance.length(), divisor );
  return chars < 1? 1: chars;
}


/***************************************************************************
 * Procedure.. IPFGenerator::linesFromDistance
 * Author..... Mat
 * Date....... 11/2/95
 *
 * Given a distance, determine the equivalent in text lines.  Return a
 * number >= 0, use 1 for default.
 ***************************************************************************/
unsigned int IPFGenerator::linesFromDistance( const Distance & distance )
{
  int divisor, lines;

  switch ( distance.units() )
  {
  case Distance::twips:
    divisor = 200;  // assume 10 pt text
    break;
  case Distance::chars:
    divisor = 1;
    break;
  case Distance::pixels:
  case Distance::percent:
    return 1;
  default:
    BADSWITCH;
    break;
  }

  lines = NearestDivide( distance.length(), divisor );
  return lines < 0? 1: lines;  // use default if <0
}


/***************************************************************************
 * Procedure.. IPFGenerator::pointsFromDistance
 * Author..... Mat
 * Date....... 11/2/95
 *
 * Given a distance, determine the equivalent value in points (1/72 inch).
 * If a mapping cannot be made, return 0 for default point size.
 ***************************************************************************/
unsigned int IPFGenerator::pointsFromDistance( const Distance & distance )
{
  int points = distance.length();

  // return 0 for default
  if ( ! points )
    return 0;

  switch ( distance.units() )
  {
  case Distance::twips:
    points = NearestDivide( points, 20 );
    break;
  case Distance::chars:
    points *= 10;  // assumes 10 pt text
    break;
  case Distance::pixels:
  case Distance::percent:
    return 0;
  default:
    BADSWITCH;
    break;
  }

  return points < 1? 1: points;
}


