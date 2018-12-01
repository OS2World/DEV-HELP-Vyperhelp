/***************************************************************************
 * File...... PenState.hpp
 * Author.... Mat
 * Date...... 10/10/96
 *
 * Holds the state of a Pen, allowing the state to be independently saved
 * and restored as needed.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef PENSTATE_HPP
#define PENSTATE_HPP

// Standard C
#include <limits.h>

// OpenClass
#include <icolor.hpp>       // IColor
class IGraphicContext;

// Editor
#include "FontInfo.hpp"


class _Export PenState
{
  public:
    // constructor
    PenState();

    // comparison
    int operator==( const PenState & state ) const;
    int operator!=( const PenState & state ) const;

    // font
    const FontInfo & font() const;
    void             setFont( const FontInfo & font, IGraphicContext & context );

    // color
    const IColor & color() const;
    void           setColor( const IColor & color );

    // reference counting (managed externally)
    int     count() const;
    void    setCount( int count );
    Boolean decrementCount();
      // returns true if ending count is 0
    void    incrementCount();

    // font metrics
    unsigned long charWidth( unsigned char c ) const;
    unsigned long maxAscender() const;
    unsigned long maxDescender() const;

    // hash function
    unsigned long hash( unsigned long modulo ) const;

  private:
    // data
    FontInfo _font;
    IColor   _color;

    // cached font metrics
    unsigned long _maxAscender;
    unsigned long _maxDescender;
    unsigned long _charWidths[ UCHAR_MAX + 1 ];

    // reference count
    int _count;
};


// global hash function (required for collection)
unsigned long hash( const PenState & state, unsigned long modulo );


// inline functions
#include "PenState.ipp"


#endif

