/***************************************************************************
 * File...... FormatPen.cpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * Implementation of FormatPen
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "LineView.hpp"
#include "ViewCursor.hpp"
#include "Page.hpp"
#include "FormatPen.hpp"


FormatPen::FormatPen( Page & page ):
  Pen( page ),
  _savedPageSize( page.size() ),
  _isMultiLineZone( false ),
  _descent( 0 ),
  _height( 0 ),
  _lineShift( 0 ),
  _longestLineWidth( 0 )
{}


void FormatPen::startFormat( PlacedView & view, Coord lineSpaceDeleted )
{
  // do default Pen setup
  start( view );

  // reset page's zone accumulator
  page().resetZone();

  // handle deleted space above current (top) line
  if ( lineSpaceDeleted )
  {
    // move point to top of page, deleteLine expects it above the deleted area
    point().setY( page().size().height() );
    // delete the space and compress the page
    deleteLine( lineSpaceDeleted );
    // reset the point to bottom of top line
    point().setY( page().size().height() - lineHeight() - lineDescent() );
  }

  // get additional line info
  LineView * line = (LineView *) view.parent();
  _above = line->above();

  // get maximum descent and height from previous views on this line
  ViewCursor * cursor = line->newCursor();
  IASSERTSTATE( cursor );
  Boolean found = cursor->locate( &view );
  IASSERTSTATE( found );
  Boolean foundPreviousView = false;
  while ( cursor->setToPrevious() )
  {
    View * sibling = cursor->view();
    IASSERTSTATE( sibling );
    if ( ! foundPreviousView )
    {
      // set x-position based on previous adjacent view
      // (assumes that line is at x=0)
      point().setX( sibling->position().x() + sibling->width() );
      foundPreviousView = true;
    }
    if ( sibling->height() > _height )
      _height = sibling->height();
    if ( sibling->descent() > _descent )
      _descent = sibling->descent();
  }
  delete cursor;

  // find longest line prior to this one
  ViewCursor * lineCursor = line->newCursorToThis();
  IASSERTSTATE( lineCursor );
  while ( lineCursor->setToPrevious() )
  {
    line = (LineView *) lineCursor->view();
    IASSERTSTATE( line );
    if ( line->width() > _longestLineWidth )
      _longestLineWidth = line->width();
  }
  delete lineCursor;

  // set start of zone to top of starting line
  _zoneStart.set( point() + IPair( 0, lineHeight() + lineDescent() ) );
}


// adjust the line as necessary before drawing the next View
// return true if line extent changed
Boolean FormatPen::placeView( PlacedView & view )
{
  // update running descent and height
  Coord descent = view.descent();
  Coord height = view.height();
  if ( descent > _descent )
    _descent = descent;
  if ( height > _height )
    _height = height;

  // check for taller line
  Coord descentDiff = 0;
  Coord heightDiff = 0;
  if ( _descent > lineDescent() )
    descentDiff = _descent - lineDescent();
  if ( _height > lineHeight() )
    heightDiff = _height - lineHeight();
  Coord verticalDiff = descentDiff + heightDiff;

  // check for wider page
  Coord pageWidth = page().size().width();
  Coord lineWidth = point().x() + view.width();
  Coord horizontalDiff = 0;
  if ( lineWidth > pageWidth )
    horizontalDiff = lineWidth - pageWidth;

  // expand page
  if ( verticalDiff || horizontalDiff )
    sizePageBy( IPair ( horizontalDiff, verticalDiff ) );

  // clear expanded area at right edge of page
  if ( horizontalDiff )
  {
    fillVertical( pageWidth );
    // update pageWidth for local use
    pageWidth = lineWidth;
  }

  // adjust for taller line
  if ( verticalDiff )
  {
    // shift everything below/including current line down
    Coord topOfLine = point().y() + lineDescent() + lineHeight();
    IRectangle rect( 0, verticalDiff, pageWidth, topOfLine );
    page().shiftRect( rect, IPair( 0, - verticalDiff ) );
    _lineShift -= verticalDiff;

    // fill space above line
    if ( heightDiff )
      fillHorizontal( topOfLine - heightDiff, topOfLine );

    // reset points to follow shift
    point().setY( point().y() - verticalDiff );
    topOfLine -= verticalDiff;

    if ( descentDiff )
    {
      // if descent contributed to expansion, shift line back up by that much
      IRectangle lineRect( 0, point().y(), pageWidth, topOfLine );
      page().shiftRect( lineRect, IPoint( 0, descentDiff ) );
      // fill space below line
      fillHorizontal( point().y(), point().y() + descentDiff );
    }

    // update line size
    if ( _descent > lineDescent() )
      setLineDescent( _descent );
    if ( _height > lineHeight() )
      setLineHeight( _height );
  }

  // move past the View
  forward( view.width() );

  // return true if line extent changed
  return ( verticalDiff > 0 );
}


// expand page and adjust any saved y-positions
void FormatPen::sizePageBy( const IPair & pair )
{
  page().sizeBy( pair );
  Coord yOffset = pair.coord2();
  if ( yOffset )
  {
    point().offsetY( yOffset );
    _zoneStart.offsetY( yOffset );
  }
}


// fill a rectangle
// assumes x ranges from 0 to the current point
void FormatPen::fillHorizontal( Coord minY, Coord maxY )
{
  // check if any area to handle
  IASSERTPARM( minY <= maxY );
  Coord maxX = point().x();
  if ( maxX > 0 && maxY > minY )
    page().clearRect( IRectangle( 0, minY, maxX, maxY ) );
}


Boolean FormatPen::isPointOnLine( const PenPoint & testPoint )
{
  return ( testPoint.isSet()
        && testPoint.y() < point().y() + lineDescent() + lineHeight() );
}


void FormatPen::deleteLine( Coord extent )
{
  // move everything below current line up by total extent
  if ( point().y() > extent )
  {
    IRectangle rect( page().size().width(), point().y() - extent );
    page().shiftRect( rect, IPair( 0, extent ) );
    _lineShift += extent;
  }

  // resize page
  sizePageBy( IPair( 0, -extent ) );
}


// return true if extent changed
Boolean FormatPen::endLine()
{
  // finalize line width
  setLineWidth( point().x() );

  // check for line smaller than allocated area
  Coord heightDiff = 0;
  IASSERTSTATE( _height <= lineHeight() );
  if ( _height < lineHeight() )
    heightDiff = lineHeight() - _height;
  Coord descentDiff = 0;
  IASSERTSTATE( _descent <= lineDescent() );
  if ( _descent < lineDescent() )
    descentDiff = lineDescent() - _descent;
  Coord totalDiff = heightDiff + descentDiff;

  // adjust for smaller line
  if ( totalDiff )
  {
    // move current line up by height difference
    // don't move area beyond point.x(), it will be cleared after the shift operations
    IRectangle lineRect( 0, point().y() + descentDiff, lineWidth(), point().y() + _height + _descent + descentDiff );
    page().shiftRect( lineRect, IPair( 0, heightDiff ) );

    // delete the difference
    deleteLine( totalDiff );

    // adjust point for difference
    point().setY( point().y() + totalDiff );

    // finalize this line's height/descent
    setLineHeight( _height );
    setLineDescent( _descent );
  }

  // return true if extent changed
  return ( totalDiff > 0 );
}


// returns true if the running "longest line width" is same as the page width
Boolean FormatPen::checkLine( Boolean isDrawing )
{
  // get page width
  Coord pageWidth = page().size().width();

  // clear unclaimed area after current line
  if ( isDrawing && lineWidth() < pageWidth )
  {
    ISize size( pageWidth - lineWidth(), lineHeight() + lineDescent() );
    page().clearRect( IRectangle( IPoint( lineWidth(), point().y() ), size ) );
  }

  // update longest line width
  if ( lineWidth() > _longestLineWidth )
    _longestLineWidth = lineWidth();

  return ( _longestLineWidth == pageWidth );
}


void FormatPen::beginLine( Coord height, Coord descent, Coord width )
{
  // increase space above to include previous line
  _above += ( lineHeight() + lineDescent() );

  // reset running height and descent
  _height = _descent = 0;

  // save next line's metrics
  setLineHeight( height );
  setLineDescent( descent );
  setLineWidth( width );

  // move down by extent, and to far left (x = 0)
  point() = IPoint( 0, point().y() - lineHeight() - lineDescent() );
}


// fill a vertical rectangle
// assumes y ranges from top of current line to top of page
// assumes x ranges from given x to page width
void FormatPen::fillVertical( Coord minX )
{
  // get boundary points
  Coord maxX = page().size().width();
  IASSERTPARM( minX <= maxX );
  Coord minY = point().y() + lineDescent() + lineHeight();
  Coord maxY = page().size().height();
  IASSERTSTATE( minY <= maxY );

  // check if any area to handle
  if ( minY < maxY && minX < maxX )
    page().clearRect( IRectangle( minX, minY, maxX, maxY ) );
}


void FormatPen::endZone()
{
  IASSERTSTATE( point().isSet() && _zoneStart.isSet() );
  _zoneStop.set( point() );
  _isMultiLineZone = ( _zoneStart.y() != point().y() + lineHeight() + lineDescent() - 1 );
}


IRectangle FormatPen::finish( Boolean allFormatted )
{
  // check for shrinking page width
  Coord   pageWidth = page().size().width();
  if ( pageWidth > _longestLineWidth && allFormatted )
  {
    sizePageBy( IPair( _longestLineWidth - pageWidth, 0 ) );
    pageWidth = _longestLineWidth;
  }
  ITRACE_DEVELOP( IString("page size ")+page().size().asString() );

  // check for wider page that needs to be filled below
  if ( pageWidth > _savedPageSize.width() && ! allFormatted )
  {
    Coord topOfLine = point().y() + lineHeight() + lineDescent();
    page().clearRect( IRectangle( _savedPageSize.width(), 0, pageWidth, topOfLine ) );
  }

  // get rectangle from _zoneStart to _zoneStop
  IASSERTSTATE( _zoneStart.isSet() );
  if ( ! _zoneStop.isSet() )
    endZone();
  IRectangle zone( _zoneStart, _zoneStop );
  ITRACE_DEVELOP( IString("zone ")+zone.asString() );

  // check for multi-line zone
  if ( _isMultiLineZone )
  {
    // for multi-line, need to expand zone to full width of page
    zone.sizeTo( ISize( page().size().width(), zone.height() ) );
    zone.moveTo( IPoint( 0, zone.minY() ) );
    ITRACE_DEVELOP( IString("zone (multi-line) ")+zone.asString() );
  }

  // invalidate the zoned part of the page (needs to be drawn)
  page().invalidate( zone );

  // add page zone (this part doesn't need to be drawn)
  zone |= page().zone();
  ITRACE_DEVELOP( IString("zone (with page) ")+zone.asString() );

  return zone;
}


