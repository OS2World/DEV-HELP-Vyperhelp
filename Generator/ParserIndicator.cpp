/***************************************************************************
 * File...... ParserIndicator.cpp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * Implementation for ParserIndicator.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// MekTek
#include "MekTek.hpp"

// Generator
#include "ParserIndicator.hpp"


ParserIndicator::ParserIndicator():
  _total( 100 ),
  _current( 0 ),
  _percent( 0 )
{}

ParserIndicator::~ParserIndicator()
{}


void ParserIndicator::setTotal( unsigned total )
{
  if ( total != _total )
  {
    IASSERTSTATE( total );
    _total = total;
    updatePercent();
  }
}


void ParserIndicator::setCurrent( unsigned current )
{
  // cap at maximum
  if ( current > _total)
    current = _total;

  // check for change
  if ( current != _current )
  {
    _current = current;
    updatePercent();
  }
}


void ParserIndicator::updatePercent()
{
  unsigned percent = _current * 100 / _total;
  if ( percent != _percent )
  {
    _percent = percent;
    meterUpdate();
  }
}

