/***************************************************************************
 * File...... FlowFormatter.hpp
 * Author.... Mat
 * Date...... 8/27/96
 *
 * FlowFormatter encapsulates the formatting algorithm which places Views
 * onto Lines, creating and reorganizing lines as necessary.
 * The formatter assumes that the View sequence and LineView sequence
 * are properly linked, and it just has to re-organize them.  i.e., it
 * uses LineView.firstView and lastView to determine the current organization
 * and works by modifying that.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef FLOWFORMATTER_HPP
#define FLOWFORMATTER_HPP

// OpenClass
#include <irect.hpp>        // IRectangle

// TextEditor
#include "FormatPen.hpp"
#include "OrderedViewCursor.hpp"
#include "PlacedView.hpp"
class FlowView;
class LineView;


class FlowFormatter
{
  public:
    // constructor
    FlowFormatter( FlowView & flow, Page & page );

    // formatter
    IRectangle format( PlacedView * startingView, PlacedView * endingView, Coord visibleWidth );
      // formats the FlowView starting with the specified View
      // if startingView = NULL, then the entire Flow is formatted
      // if endingView != NULL, format will not stop before the endingView
      // returns the repaint zone

  private:
    // helper sub-routines
    PlacedView::FormatChange formatAnyView( PlacedView * view );
    void    formatView();
    void    checkIfDone();
    void    placeView();
    Boolean findStartingView();
    void    finishLine();
    void    checkForLineMerge();
    void    getNextLine( Boolean startFromCurrent, Boolean isPenStarted );

    // mode of operation
    enum FormatterMode
    {
      starting,    // no view has been formatted yet
      all,         // need to format entire flow
      formatting,  // formatting, placing and drawing views
      exhausted,   // all views have been formatted
      zoning,      // done formatting, expanding repaint zone
      sizing,      // done determing zone, verifying page width
      done,        // done traversing lines
      doneAll      // done: hit every line
    } _mode;

    // provided by constructor
    FlowView *         _flow;
    Page *             _page;

    // used during formatting
    FormatPen          _pen;                 // allocated by FlowFormatter
    OrderedViewCursor  _viewCursor;          // cursur to starting/current view
    OrderedViewCursor  _lineCursor;          // cursor to current line
    LineView *         _line;                // pointer to current line
    OrderedViewCursor  _nextLineCursor;      // cursor to next line
    LineView *         _nextLine;            // NULL if there are no more lines
    View *             _firstViewOnNextLine;
    View *             _lastViewOnNextLine;
    OrderedViewCursor  _previousViewCursor;  // cursor to previous view
    PlacedView *       _view;                // pointer to current view
    PlacedView *       _endingView;          // last view which must be formatted (ignore if 0)
    PlacedView::LineMode _lineMode;
    Boolean            _isLineDone;
    Boolean            _isExtentChanged;     // true if any line extent changes while formatting
    unsigned           _linesNeedFormat;     // # lines which must be formatted
    Boolean            _viewNeedsFormat;     // true if current View needs format
    Coord              _lineSpaceDeleted;    // amount of line space to be deleted
};


#endif

