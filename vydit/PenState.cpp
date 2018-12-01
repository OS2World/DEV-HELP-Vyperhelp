/***************************************************************************
 * File...... PenState.cpp
 * Author.... Mat
 * Date...... 4/16/98
 *
 * Implementation of PenState.  Also see inline functions in PenState.ipp
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Standard C
#include <memory.h>

// OpenClass
#include <ifont.hpp>        // IFont

// Editor
#include "PenState.hpp"


PenState::PenState():
    _color( IColor::black ),
    _count( 0 ),
    _maxAscender( 0 ),
    _maxDescender( 0 )
{
  memset( _charWidths, 0, sizeof(_charWidths) );
}


unsigned long PenState::hash( unsigned long modulo ) const
{
  return ( _font.hash( modulo ) + _color.asRGBLong() ) % modulo;
}


void PenState::setFont( const FontInfo & font, IGraphicContext & context )
{
  if ( _font != font || ! _maxAscender )
  {
    // save new font
    _font = font;

    // load font metrics
    IFont * ifont = _font.newFont( context );
    _maxAscender = ifont->maxAscender();
    _maxDescender = ifont->maxDescender();
    for (int c = 0; c <= UCHAR_MAX; c++)
      _charWidths[c] = ifont->charWidth( c );
    delete ifont;
  }
}


