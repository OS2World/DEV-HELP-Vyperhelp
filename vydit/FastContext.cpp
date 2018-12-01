/***************************************************************************
 * File...... FastContext.cpp
 * Author.... Mat
 * Date...... 9/23/97
 *
 * Implementation of FastContext
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// compile options
#define DRAW_OS2      // use native OS/2 calls
#define FAST_STRING   // use fast (but inaccurate) OpenClass string


// OS/2
#ifdef DRAW_OS2
  #define INCL_GPI
  #include <os2.h>
#endif

// OpenClass
#include <igstring.hpp>     // IGString
#include <igbitmap.hpp>     // IGBitmap
#include <igrect.hpp>       // IGRectangle
#include <iexcept.hpp>      // ITHROW and IASSERT macros
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "FastContext.hpp"



FastContext::FastContext( const IWindowHandle & windowHandle, const IColor & backgroundColor ):
  IGraphicContext( windowHandle ),
  _backgroundColor( backgroundColor )
{}


FastContext::FastContext( const ISize & contextSize, const IColor & backgroundColor ):
  IGraphicContext( contextSize ),
  _backgroundColor( backgroundColor )
{
  // OS/2 apparently doesn't like 32-bit coordinates!
  if ( ( contextSize.width() > 0x7fff ) || ( contextSize.height() > 0x7fff ) )
    ITHROW( IException( "Too much text to edit." ) );
}


#ifdef DRAW_OS2

//
// OS/2
//

// NOTE: Found that GpiCharStringAt would not correctly erase background for some fonts (e.g., Courier)
//       The CHS_VECTOR flag and a width table is required for a clean background.

void FastContext::drawChar( char ch, Coord width, const IPoint & position )
{
  POINTL ptl = position.asPOINTL();
  LONG gpi = GpiCharStringPosAt( handle(), &ptl, NULL, CHS_VECTOR, 1, &ch, (PLONG)&width );
  if ( gpi == GPI_ERROR )
    ITHROWGUIERROR( "GpiCharStringPosAt" );
}


void FastContext::drawString( const IString & string, const Coord * widths, const IPoint & position )
{
  POINTL ptl = position.asPOINTL();
  LONG gpi = GpiCharStringPosAt( handle(), &ptl, NULL, CHS_VECTOR, string.length(), (PCH)string, (PLONG)widths );
  if ( gpi == GPI_ERROR )
    ITHROWGUIERROR( "GpiCharStringPosAt" );
}


void FastContext::drawBitmap( const IBitmapHandle & bitmap, const IPoint & position )
{
  // get bitmap size
  BITMAPINFOHEADER bmi;
  bmi.cbFix = sizeof( bmi );
  BOOL ok = GpiQueryBitmapParameters( bitmap, &bmi );
  if ( ! ok )
    ITHROWGUIERROR( "GpiQueryBitmapParameters" );
  POINTL aptl[4];

  // get target coordinates in context (world coordinates, inclusive boundary)
  IPoint base = position;
  aptl[0] = base.asPOINTL();
  base += ISize( bmi.cx, bmi.cy );
  aptl[1] = base.asPOINTL();

  // get source coordinates in bitmap (device coordinates, non-inclusive boundary)
  IPoint origin( 0, 0 );
  aptl[2] = origin.asPOINTL();
  origin += ISize( bmi.cx + 1, bmi.cy + 1 );
  aptl[3] = origin.asPOINTL();

  // copy bitmap to context
  LONG gpi = GpiWCBitBlt( handle(), bitmap, 4L, aptl, ROP_SRCCOPY, 0 );
  if ( gpi == GPI_ERROR )
    ITHROWGUIERROR( "GpiWCBitBlt" );
}


// currently assumes background is white! (ROP_ONE)
void FastContext::clearRect( const IRectangle & rect )
{
  HPS hps = handle();
  POINTL aptl[2];
  aptl[0] = rect.minXMinY().asPOINTL();
  aptl[1] = rect.maxXMaxY().asPOINTL();
  // convert to device coordinates
  BOOL ok = GpiConvert( hps, CVTC_WORLD, CVTC_DEVICE, 2, aptl );
  if ( ! ok )
    ITHROWGUIERROR( "GpiConvert" );
  // clear the rectangle
  LONG gpi = GpiBitBlt( hps, NULL, 2, aptl, ROP_ONE, 0 );
  if ( gpi == GPI_ERROR )
    ITHROWGUIERROR( "GpiBitBlt" );
}


void FastContext::copyRect( const IRectangle & rect, const IPoint & position )
{
  HPS hps = handle();
  IPoint max = position + rect.size();
  POINTL aptl[3];
  aptl[0] = position.asPOINTL();
  aptl[1] = max.asPOINTL();
  aptl[2] = rect.minXMinY().asPOINTL();
  // convert to device coordinates
  BOOL ok = GpiConvert( hps, CVTC_WORLD, CVTC_DEVICE, 3, aptl );
  if ( ! ok )
    ITHROWGUIERROR( "GpiConvert" );
  // copy the rectnagle
  LONG gpi = GpiBitBlt(  hps, hps, 3, aptl, ROP_SRCCOPY, 0 );
  if ( gpi == GPI_ERROR )
    ITHROWGUIERROR( "GpiBitBlt" );
}

#else

//
// PORTABLE (BUT SLOW)
//



void FastContext::drawChar( char ch, const IPoint & position )
{
  IGString gstring( ch );
  gstring.moveTo( position ).drawOn( *this );
}


void FastContext::drawString( const IString & string, const Coord * widths, const IPoint & position )
{
#ifdef FAST_STRING
  // draw the string as a unit (some fonts will not be spaced correctly)
  IGString gstring( string );
  gstring.moveTo( position ).drawOn( *this );
#else
  // draw the string char-by-char
  IPoint charPosition = position;
  unsigned i;
  unsigned n = string.length();
  unsigned char * pch = string;
  for ( i = 0; i < n; i++ )
  {
    drawChar( pch[i], charPosition );
    charPosition += IPair( widths[i], 0 );
  }
#endif
}


void FastContext::drawBitmap( const IBitmapHandle & bitmap, const IPoint & position )
{
  IGBitmap gbitmap( bitmap );
  gbitmap.moveTo( position ).drawOn( *this );
}


void FastContext::clearRect( const IRectangle & rect )
{
  // set attributes for the rectangle to background color
  IGraphicBundle fillBundle;
  fillBundle.setPenColor( _backgroundColor );
  fillBundle.setFillColor( _backgroundColor );
  // get inner rectangle
  IRectangle inside( rect.minX(), rect.minY(), rect.maxX() - 1, rect.maxY() - 1 );
  IGRectangle fillRect( inside );
  fillRect.setGraphicBundle( fillBundle ).drawOn( *this );
}


void FastContext::copyRect( const IRectangle & rect, const IPoint & position )
{
  IGBitmap gbitmap( *this, rect );
  gbitmap.moveTo( position ).drawOn( *this );
}

#endif
