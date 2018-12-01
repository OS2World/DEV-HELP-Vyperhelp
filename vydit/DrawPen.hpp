/***************************************************************************
 * File...... DrawPen.hpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * DrawDrawPen is a DrawPen which supports drawing of Views. It assumes the page
 * is already fully formatted, and it only has to traverse and draw Views,
 * and control the DrawPenState.  It uses FastContext to perform draw operations.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef DRAWPEN_HPP
#define DRAWPEN_HPP

// OpenClass
class IBitmapHandle;
class IGraphicContext;

// TextEditor
#include "Pen.hpp"


class DrawPen: public Pen
{
  public:
    // constructor
    DrawPen( Page & page );
    virtual ~DrawPen();

    // draw support
    IPoint            baseline() const;
      // returns the baseline point (between descent and height)
    IGraphicContext & context();
      // returns an IGraphicContext for drawing, etc
    void              clear( Coord width, Coord height, Coord descent );
      // clears above and below the given region
    void drawChar( char ch );
    void drawString( const IString & string );
      // draw char or string of chars, handles clear() too
      // actually batches text for quicker draw
    void drawBitmap( const IBitmapHandle & bitmap );

    // from Pen (intercepted to support text batching)
    virtual void    start( PlacedView & view );
    virtual Boolean setFont( const IString & name, unsigned long pointSize );
    virtual Boolean setBold( Boolean isBold );
    virtual Boolean setItalic( Boolean isItalic );
    virtual Boolean setUnderline( Boolean isUnderline );
    virtual Boolean setColor( const IColor * color = 0 );
    virtual void    setState( const PenState & newState );

  private:
    void clearFrom( const IPoint & position, Coord width, Coord height, Coord descent );
    void flush();

    IString _batch;
    IPoint  _batchPosition;
};


#endif

