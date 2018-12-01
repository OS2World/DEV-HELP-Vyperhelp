/***************************************************************************
 * File...... ExpirationDate.cpp
 * Author.... Mat
 * Date...... 7/7/98
 *
 * Implemantation of ExpirationDate
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Generator
#include "ExpirationDate.hpp"


ExpirationDate::ExpirationDate( unsigned long code ):
  _code( code )
{
  if ( code )
  {
    int month = code % 15;
    code /= 15;
    int day = code % 35;
    int year = code / 35;
    initialize( IDate::Month(month), day, year );
  }
}


IString ExpirationDate::notice() const
{
  if ( *this )
    return IString("This program expires on ") + asString( "%x." );
  else
    return IString("This program does not expire.");
}


