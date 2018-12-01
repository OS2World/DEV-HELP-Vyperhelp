/***************************************************************************
 * File...... FormatPen.hpp
 * Author.... Mat
 * Date...... 9/30/97
 *
 * FormatPen provides line-by-line formatting control for Views.
 * FormatPen's lifetime is only as long as the formatting operation.
 * Long-term information is stored in the Page.
 *
 * The FormatPen can be thought of as the interface between the FlowFormatter
 * and the Page.  The FormatPen doesn't know much about the behavior of
 * Views, so FlowFormatter is responsible for that.  FormatPen knows more about
 * the workings of Page.
 *
 * FormatPen is also responsible for clearing the "unclaimed" areas of
 * the Page.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef FORMATPEN_HPP
#define FORMATPEN_HPP

// OpenClass
#include <irect.hpp>        // IRectangle

// TextEditor
#include "Pen.hpp"


class FormatPen: public Pen
{
  public:
    // constructor
    FormatPen( Page & page );

    // initialization
    void startFormat( PlacedView & view, Coord lineSpaceDeleted );
      // start formatting at the given View
      // if lineSpaceDeleted > 0, handle special case with line space deleted above top line

    // pen point control
    Boolean         placeView( PlacedView & view );
      // updates the current line's height and descent, then moves past the view
      // return true if the line extent changed (larger)
    void           deleteLine( Coord extent );
      // delete a horizontal strip of the specified strip (below current point)
    Boolean        endLine();
      // finishes formatting work on current line, including page resize if necessary
      // return true if the line extent changed (smaller)
    Boolean        checkLine( Boolean isDrawing );
      // registers line width
      // clears unclaimed area if isDrawing is true
      // returns true if the longest line (so far) is same as page width
    void           beginLine( Coord height, Coord descent, Coord width );
      // moves the pen point to a new line
      // provides next line's starting height and descent
    Coord          above() const;
      // returns the amount of space on lines above (already formatted or drawn)
    Coord          lineShift() const;
      // returns the amount that lines below have been shifted up (+) or down (-)

    // "zone" support (zone is the region that needs repainting)
    void       endZone();
      // save the current point as the end of the zone
    IRectangle finish( Boolean allFormatted );
      // finishes the formatting
      // allFormatted = true if all lines have been checked for size
      // returns the "invalid rectangle": portion of window which needs drawing

  private:
    // helpers
    void    sizePageBy( const IPair & pair );
    void    fillHorizontal( Coord minY, Coord maxY );
    void    fillVertical( Coord minX );
    Boolean isPointOnLine( const PenPoint & testPoint );

    // page
    ISize  _savedPageSize;

    // metrics for current line
    Coord _descent;           // size of line as it accumulates
    Coord _height;
    Coord _lineShift;         // amount lines below have shifted
    Coord _above;             // amount of space above the top of this line
    Coord _longestLineWidth;  // width of longest line encountered yet

    // repaint zone
    PenPoint _zoneStart;
    PenPoint _zoneStop;
    Boolean  _isMultiLineZone;
};


// inline functions
#include "FormatPen.ipp"


#endif

