/***************************************************************************
 * File...... Page.cpp
 * Author.... Mat
 * Date...... 6/10/97
 *
 * Implements a Page object.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Debugging Variations
//#define DEBUG_DRAW  // to see background drawing in a separate window
//#define DEBUG_ZONE  // to see a box around the redrawn area

// OS/2
#define INCL_GPIREGIONS
#include <os2.h>

// OpenClass
#include <iwindow.hpp>      // IWindow
#include <irect.hpp>        // IRectangle
#include <igbitmap.hpp>     // IGBitmap
#include <ipainevt.hpp>     // IPaintEvent
#include <irect.hpp>        // IRectangle
#include <itrace.hpp>       // ITRACE macros
#include <iexcept.hpp>      // ASSERT macros
#ifdef DEBUG_DRAW
  #include <iframe.hpp>       // IFrameWindow
  #include <icanvas.hpp>      // ICanvas
#endif

// Editor
#include "FastContext.hpp"
#include "Page.hpp"

// static data
IColor Page::_backgroundColor( IColor::white );
IColor Page::_defaultColor( IColor::black );

// constructor
Page::Page():
  _context( 0 ),
  _visibleWidth( 0 ),
  _hasSelection( false )
{
  // set static defaults (affects all instances!)
  IGraphicContext::setDefaultBackgroundColor( _backgroundColor );
  IGraphicContext::setDefaultBackgroundMixMode( IGraphicBundle::backOverPaint );

  // set attributes for region used for showing selected region
  IGraphicBundle selectionBundle;
  selectionBundle.setMixMode( IGraphicBundle::invert );
  _selectionRegion.setGraphicBundle( selectionBundle );

#ifdef DEBUG_DRAW
  _frame = new IFrameWindow( "Vyper Debugger" );
  _canvas = new ICanvas( IC_FRAME_CLIENT_ID, _frame, _frame ),
  _context = new FastContext( _canvas->handle(), _backgroundColor );
  _frame->setClient( _canvas );
  _canvas->setBackgroundColor( IColor( IColor::paleGray ) );
  _frame->setAutoDestroyWindow( true );
  _canvas->setAutoDestroyWindow( true );
#endif

  // create initial context and set initial transform
  createContext( ISize( 500, 500 ) );
  setTransform( ISize() );
}


Page::~Page()
{
  delete _context;
#ifdef DEBUG_DRAW
  delete _canvas;
  delete _frame;
#endif
}


void Page::createContext( const ISize & contextSize )
{
  // create new context
  FastContext * newContext;
#ifdef DEBUG_DRAW
  _canvas->sizeTo( contextSize );
  _frame->moveSizeToClient( IRectangle( contextSize ) ).show();
  newContext = _context;
#else
  newContext = new FastContext( contextSize, _backgroundColor );
#endif

  // transfer contents of old context to new
  if ( _context )
  {
    // copy page image to upper left of new context
    if ( _size.width() && _size.height() )
    {
      IGBitmap bitmap( *_context, transform( _size ) );
      bitmap.moveTo( IPoint( 0, contextSize.height() - _size.height() ) );
      bitmap.drawOn( *newContext );
    }
#ifndef DEBUG_DRAW
    delete _context;
#endif
  }

  // save new context
  _context = newContext;
  _contextSize = contextSize;
}


void Page::sizeBy( const IPair & offset )
{
  // size the page by the given offset
  ISize nextSize = _size + offset;

  // get a larger context if necessary
  if ( nextSize.height() > _contextSize.height() || nextSize.width() > _contextSize.width() )
  {
    // double the width or height as necessary
    ISize nextContextSize( _contextSize );
    while ( nextSize.height() > nextContextSize.height() )
      nextContextSize.setHeight( nextContextSize.height() * 2 );
    while ( nextSize.width() > nextContextSize.width() )
      nextContextSize.setWidth( nextContextSize.width() * 2 );
    ITRACE_DEVELOP( IString("Page size: need ") + nextSize.asString()
                  + IString(", got ") + nextContextSize.asString() );
    createContext( nextContextSize );
  }

  setTransform( nextSize );
}


void Page::setTransform( const ISize & size )
{
  // if height is changing, shift zone and invalid region by difference
  if ( size.height() != _size.height() )
  {
    IPair diff( 0, size.height() - _size.height() );
    _zone.moveBy( diff );
    _invalidRegion.moveBy( diff );
  }

  // save new size
  _size = size;

  // set transform to place user region in upper left
  ITransformMatrix transform;
  transform.translateBy( IPoint( 0, _contextSize.height() - _size.height() ) );
  _context->setWorldTransformMatrix( transform );
}


IRectangle Page::transform( const IRectangle & rect ) const
{
  return rect.movedBy( IPair( 0, _contextSize.height() - _size.height() ) );
}


void Page::resetSize( Coord visibleWidth )
{
  // reset to 0,0 size (keep context same size)
  _size = ISize();
  _visibleWidth = visibleWidth;
  _zone = IRectangle();
  _invalidRegion.clear();
}


void Page::clearRect( const IRectangle & rect )
{
  // set rectangle size and draw on context
  if ( rect.area() )
  {
    _context->clearRect( rect );
    zoneUnion( rect );
  }
}


void Page::shiftRect( const IRectangle & rect, const IPair & offset )
{
  if ( rect.area() && offset != IPair() )
  {
    IRectangle target = rect.movedBy( offset );
    _context->copyRect( rect, target.minXMinY() );
    zoneUnion( target );
  }
}


// IRectangle union operator (|) doesn't support 0-size rectangles
// use area() to check for them and avoid that situation
void Page::zoneUnion( const IRectangle & rect )
{
  if ( rect.area() )
  {
    if ( _zone.area() )
      _zone |= rect;
    else
      _zone = rect;
  }
}


void Page::clearSelection()
{
  _selectionRegion.clear();
  _hasSelection = false;
}


void Page::addToSelection( const IRectangle & rect )
{
  if ( rect.area() )
  {
    _selectionRegion += IGRectangle( rect );
    _hasSelection = true;
  }
}


void Page::paint( IPaintEvent & event )
{
  // get screen context
  IGraphicContext screen( event.presSpaceHandle() );

  // get rectangle to paint
  IRectangle rect = event.rect();

  // if completely outside of page, just flood fill and return
  if ( ! rect.intersects( IRectangle( _size ) ) )
  {
    event.clearBackground( _backgroundColor );
    return;
  }

  // check if wider than page
  if ( rect.maxX() > _size.width() )
  {
    // fill outside area (beyond page width)
    IRectangle outside( _size.width(), rect.minY(), rect.maxX(), rect.maxY() );
    event.clearBackground( outside, _backgroundColor );
    // size paint rectangle down
    rect.sizeTo( IPair( _size.width() - rect.minX(), rect.height() ) );
  }

  // get bitmap of invalid area
  IGBitmap bitmap( *_context, transform( rect ) );

  // copy invalid area from bitmap to screen
  bitmap.moveTo( rect.minXMinY() );
  bitmap.drawOn( screen );

  // invert selected region
  if ( _hasSelection )
    drawRegion( _selectionRegion, screen );

#ifdef DEBUG_ZONE
  {
  // draw a frame around zone for debugging (rotate colors)
  static int colorIndex;
  IGRectangle zone( IRectangle( rect.minX(), rect.minY(), rect.maxX()-1, rect.maxY()-1 ) );
  IGraphicBundle bundle;
  bundle.setDrawOperation( IGraphicBundle::frame );
  bundle.setPenColor( IColor( ( colorIndex++ % 15 ) + 1) );
  zone.setGraphicBundle( bundle );
  zone.drawOn( screen );
  }
#endif
}


void Page::sizeToInvalid( IRectangle & rect )
{
  IGRegion unionRegion = _invalidRegion;
  unionRegion &= IGRectangle( rect );
  sizeRectToRegion( rect, unionRegion );
}


/***************************************************************************
 * Procedure.. Page::sizeRectToRegion
 * Author..... Mat
 * Date....... 2/14/98
 *
 * This function solves two problems with OpenClass:
 *
 * 1. As of CTO308, IGRegion::boundingRect causes a "Bad Stack" exception
 *    because of a recursive call in IGraphicContext::draw( IGRegion ).
 * 2. boundingRect doesn't handle an empty region!  It sets minX to
 *    a very large number ( < -130000000 ).
 *
 * Note: this function is platform dependent!  UGGH!!
 *
 ***************************************************************************/
void Page::sizeRectToRegion( IRectangle & rect, const IGRegion & region )
{
  RECTL rcl;
  LONG  lResult;

  lResult = GpiQueryRegionBox( _context->handle(), IRegionHandle(region), &rcl );
  if ( lResult == RGN_ERROR )
    ITHROWGUIERROR( "GpiQueryRegionBox" );
  if ( lResult == RGN_NULL )
    rect.sizeTo( IPair() );
}


/***************************************************************************
 * Procedure.. Page::drawRegion
 * Author..... Mat
 * Date....... 2/14/98
 *
 * DRaw the given region on the given context.  This is a workaround for
 * a bug in OpenClass which causes a "Bad Stack" exception because of a
 * recursive call to IGraphic::boundingRect (see sizeRectToRegion also).
 *
 * Assumes that the context is not "recoordinated" and that the region
 * does not have a transform applied to it.
 *
 * Note: this function is platform dependent!  UGGH!!
 *
 ***************************************************************************/
void Page::drawRegion( IGRegion & region, IGraphicContext & context )
{
  IASSERTSTATE( context.recoordinationHeight() == 0 );
  IASSERTSTATE( ! region.hasTransformMatrix() );

  // apply the region's graphic bundle
  IGraphicBundle * savedBundle = 0;
  if ( region.hasGraphicBundle() )
  {
    savedBundle = new IGraphicBundle( context );
    context.setGraphicBundle( region.graphicBundle() );
  }

  // draw the region
  LONG lResult = GpiPaintRegion( context.handle(), IRegionHandle(region) );
  if ( lResult == GPI_ERROR )
    ITHROWGUIERROR( "GpiPaintRegion" );

  // restore the context's bundle
  if ( savedBundle )
  {
    context.setGraphicBundle( *savedBundle );
    delete savedBundle;
  }
}


void Page::invalidate( const IRectangle & rect )
{
  _invalidRegion += IGRectangle( rect );
}


void Page::validate( const IRectangle & rect )
{
  _invalidRegion -= IGRectangle( rect );
}


