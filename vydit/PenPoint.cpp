/***************************************************************************
 * File...... PenPoint.cpp
 * Author.... Mat
 * Date...... 6/24/97
 *
 * Implements PenPoint
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "PenPoint.hpp"


PenPoint::PenPoint():
    _isSet( false )
{}


PenPoint::PenPoint( const IPair & pair ):
    IPoint( pair ),
    _isSet( true )
{}


PenPoint & PenPoint::set( const IPair & pair )
{
  * ( IPair * ) this = pair;
  _isSet = true;
  return *this;
}


PenPoint & PenPoint::offsetY( Coord yOffset )
{
  if ( _isSet )
  {
    IPoint * point = ( IPoint * ) this;
    point->setY( point->y() + yOffset );
  }
  return *this;
}

