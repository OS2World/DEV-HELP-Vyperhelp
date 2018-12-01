/***************************************************************************
 * File...... FlowFormatter.cpp
 * Author.... Mat
 * Date...... 8/27/96
 *
 * Implementation of line-breaking (formatting) algorithm for FlowView.
 * FlowFormatter is a friend of FlowView and directly manipulates its
 * size and line-collection.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros

// TextEditor
#include "Page.hpp"
#include "FlowView.hpp"
#include "LineView.hpp"
#include "OrderedViewCursor.hpp"
#include "FlowFormatter.hpp"


FlowFormatter::FlowFormatter( FlowView & flow, Page & page ):
    _pen( page ),
    _flow( &flow ),
    _page( &page ),
    _lineCursor( flow._lines ),
    _nextLineCursor( flow._lines ),
    _viewCursor( flow._views ),
    _previousViewCursor( flow._views )
{}


/***************************************************************************
 * Procedure.. FlowFormatter::format
 * Author..... Mat
 * Date....... 8/27/96
 *
 * If startingView is not NULL, it is the view where formatting should
 * begin.  This is called when there is a local change confined to this
 * view and subsequent views.  When the formatting engine determines that
 * views have re-synchronized, it will stop formatting.
 *
 * If startingView is NULL, the entire FlowView is formatted.
 *
 * If endingView is not NULL, formatting will not stop before endingView
 * is formatted.
 *
 * Empty lines are removed before the first View is selected.
 ***************************************************************************/
IRectangle FlowFormatter::format( PlacedView * startingView, PlacedView * endingView, Coord visibleWidth )
{
  Boolean found;

  // set ending view
  _endingView = endingView;

  // initialize space deleted
  _lineSpaceDeleted = 0;
  _isExtentChanged = false;

  if ( startingView )
  {
    //
    // FORMAT FROM STARTING VIEW
    //

    // get starting view
    _view = startingView;
    Boolean startAtTop = findStartingView();

    // get starting line and line cursor
    if ( startAtTop )
    {
      // find first line
      found = _lineCursor.setToFirst();
      IASSERTSTATE( found );
      _line = (LineView *) _lineCursor.view();
      IASSERTSTATE( _line );

      // look for first non-empty line (assume it holds the first view)
      if ( _view->parent() != _line )
      {
        _nextLineCursor = _lineCursor;
        getNextLine( false, false );
        // save found line as current line
        // (nextLineCursor will be updated below)
        _lineCursor = _nextLineCursor;
        _line = (LineView *) _lineCursor.view();
        IASSERTSTATE( _line );
        // reset "above" to 0 to indicate first line
        _line->_above = 0;
      }
    }
    else
    {
      // start with first view's line
      _line = (LineView *) _view->parent();
      IASSERTSTATE( _line );
      IASSERTSTATE( _line->isLine() );
      found = _lineCursor.locate( _line );
      IASSERTSTATE( found );
    }

    // initialize mode
    _mode = starting;
  }
  else
  {
    //
    // FORMAT ENTIRE FLOW
    //

    // reset the page size
    _page->resetSize( visibleWidth );

    // reset the flow's size
    _flow->_size = ISize();

    // find first line
    found = _lineCursor.setToFirst();
    IASSERTSTATE( found );
    _line = (LineView *) _lineCursor.view();

    // reset first line's size
    _line->_width = _line->_height = _line->_descent = _line->_above = 0;

    // set first line to include *all* views in the collection
    found = _viewCursor.setToLast();
    IASSERTSTATE( found );
    _line->setLastView( &_viewCursor );
    found = _viewCursor.setToFirst();
    IASSERTSTATE( found );
    _line->setFirstView( &_viewCursor, false );

    // get pointer to first view
    _view = (PlacedView *) _viewCursor.view();

    // remove all lines after the first
    // (be careful not to delete line while the cursor is on it)
    LineView *        line = NULL;
    OrderedViewCursor cursor( _lineCursor );
    do
    {
      found = cursor.setToNext();
      if ( line )
        line->removeSelf();
      if ( found )
        line = (LineView *) cursor.view();
    } while ( found );

    // initialize mode
    _mode = all;
  }

  // initialize pen for first view
  _pen.startFormat( *_view, _lineSpaceDeleted );
  _lineSpaceDeleted = 0;

  // initialize the previous-view cursor
  _previousViewCursor = _viewCursor;
  _previousViewCursor.setToPrevious();

  // initialize next-line cursor
  getNextLine( true, true );

  // initialize scanning variables
  _linesNeedFormat = 0;
  _lineMode = PlacedView::noNewLine;  // need at least one item per line
  _isLineDone = false;

  // get previous view's line mode (if on the same line)
  if ( _previousViewCursor.isValid() )
  {
    PlacedView * previousView = (PlacedView *) _previousViewCursor.view();
    if ( previousView->parent() == _line )
      _lineMode = previousView->lineMode();
  }

  // format and draw views
  do
  {
    formatView();
    if ( _isLineDone )
    {
      // finish current line and get a new one
      finishLine();
    }
    else if ( _mode <= formatting )
    {
      // add view to current line
      placeView();
    }
  }
  while ( _mode <= formatting && _viewCursor.isValid() );

  // scan remaining lines
  if ( _mode <= formatting )
    _mode = exhausted;
  while ( _mode < done )
    finishLine();

  // finish drawing/sizing
  IRectangle zone = _pen.finish( _mode == doneAll );

  // set final size (expand width if necessary)
  _flow->_size = _page->size().maximum( IPair( visibleWidth, 0 ) );

  // return the repaint zone
  return zone;
}


// format any view, including LineEndView
PlacedView::FormatChange FlowFormatter::formatAnyView( PlacedView * view )
{
  // check if this was the caller-specified endingView
  if ( view == _endingView )
    _endingView = 0;

  // format the view and determine changes
  return view->format( _pen );
}


/***************************************************************************
 * Procedure.. FlowFormatter::formatView()
 * Author..... Mat
 * Date....... 5/29/97
 *
 * Formats the current view and determines if it fits on the current line,
 * and if further formatting is needed.
 *
 * Formatting (and drawing to memory context) can be stopped when all of
 * the following conditions are true:
 *  - Not at the end of a line (_isLineDone)
 *  - At least one view has been formatted (_mode > starting)
 *  - Not formatting everything (_mode != all)
 *  - The pen is the same as the last time current view was formatted
 *  - No change was detected during formatting of current view
 *  - There are no remaining lines requiring format (due to line merges)
 *  - Line height and descent are unchanged
 ***************************************************************************/
void FlowFormatter::formatView()
{
  _viewNeedsFormat = true;

  // if first time formatting this view, update pointer
  if ( ! _isLineDone )
    _view = (PlacedView *) _viewCursor.view();

  // if previous view forced new line, no need to format to check for fit
  if ( _lineMode == PlacedView::forceNewLine )
  {
    _isLineDone = true;
    return;
  }

  // save view's state for checking
  PenState viewState = _view->state();

  // save pen state before formatting
  PenState state = _pen.state();

  // format the view and determine changes
  PlacedView::FormatChange change = formatAnyView( _view );

  // check if done formatting
  _viewNeedsFormat = ( change != PlacedView::changeNothing ) || ! ( viewState == state );

  // check for fit
  if ( _lineMode == PlacedView::allowNewLine )
    _isLineDone =
      ! ( _view->fitMode() == PlacedView::forceFit ) &&
      ! ( _pen.point().x() + _view->width() <= _page->visibleWidth() );
  else
    _isLineDone = false;

  // restore pen state if no fit
  if ( _isLineDone )
  {
    _pen.setState( state );
    return;
  }

  // check if first view is formatted
  if ( _mode == starting )
  {
    _mode = formatting;
    return;
  }

  // if formatting everything, skip "done" check
  if ( _mode == all )
    return;

  // check if done formatting
  if ( ! _viewNeedsFormat )
    checkIfDone();
  // if size changed, ensure that the rest of this line is formatted
  else if ( change == PlacedView::changeSize && _linesNeedFormat < 1 )
    _linesNeedFormat = 1;
}


/***************************************************************************
 * Procedure.. FlowFormatter::checkIfDone
 * Author..... Mat
 * Date....... 9/11/97
 *
 * Check if ok to stop formatting.  If so, increment the mode.
 ***************************************************************************/
void FlowFormatter::checkIfDone()
{
  IASSERTSTATE( _mode <= formatting );

  // check if done formatting
  if ( ( _endingView == 0 )
    && ( ! _viewNeedsFormat )
    && ( _linesNeedFormat == 0 )
    && ( _line->height() == _pen.lineHeight() )
    && ( _line->descent() == _pen.lineDescent() ) )
  {
    // done formatting
    if ( _isExtentChanged )
      _mode = zoning;
    else
    {
      _pen.endZone();
      _mode = sizing;
    }
  }
}


/***************************************************************************
 * Procedure.. FlowFormatter::placeView()
 * Author..... Mat
 * Date....... 5/29/97
 *
 * Places the view on the current line, checks for line merge, and advances
 * the cursor to the next line.
 ***************************************************************************/
void FlowFormatter::placeView()
{
  // prepare for and draw view
  if ( _pen.placeView( *_view ) )
    _isExtentChanged = true;

  // check for merging lines
  if ( _nextLine )
    checkForLineMerge();

  // reset parent to current line
  _view->setParent( _line );

  // save the view's line mode
  _lineMode = _view->lineMode();

  // move to next view
  _previousViewCursor = _viewCursor;
  _viewCursor.setToNext();
}


/***************************************************************************
 * Procedure.. FlowFormatter::finishLine
 * Author..... Mat
 * Date....... 5/29/97
 *
 * Called to finish processing of the current line and update the repaint
 * zone.  Also gets the next available line or creates a new one.
 * This function is used after view formatting is done to update line
 * positions and build the repaint zone.
 ***************************************************************************/
void FlowFormatter::finishLine()
{
  // if the line is complete, set new last view and format LineEndView
  if ( _mode <= exhausted )
  {
    // set the line's last view
    IASSERTSTATE( _previousViewCursor.isValid() );
    _line->setLastView( &_previousViewCursor );

    // format the line end view
    // (always format, even if it is not drawn, to maintain the pen state)
    formatAnyView( &_line->_lineEnd );

    // draw the line end view, if needed
    if ( _line->_needLineEnd )
    {
      if ( _pen.placeView( _line->_lineEnd ) )
        _isExtentChanged = true;
    }

    // decrement count of lines needing format
    if ( _linesNeedFormat )
      _linesNeedFormat--;

    // need at least one View on the next line
    _lineMode = PlacedView::noNewLine;

    // end formatting of views on this line
    // check for line extent change
    if ( _pen.endLine() )
      _isExtentChanged = true;

    // update line size
    _line->_width = _pen.lineWidth();
    _line->_height = _pen.lineHeight();
    _line->_descent = _pen.lineDescent();
  }

  // save space above for determining line's y-position
  if ( _mode <= zoning )
    _line->_above = _pen.above();

  // finish processing of current line
  if ( _mode <= sizing )
  {
    // register line width, check for full width occupied
    Boolean pageFull = _pen.checkLine( _mode <= zoning );
    // check if sizing is done
    if ( _mode == sizing && pageFull )
    {
      _mode = done;
      return;
    }
  }

  // get the next line if one is available
  Boolean isFirstViewChanged = true;
  if ( _nextLine )
  {
    isFirstViewChanged = ( _view != _firstViewOnNextLine );
    _lineCursor = _nextLineCursor;
    _line = _nextLine;
    getNextLine( true, true );
  }
  else
  {
    // create new line
    if ( _mode <= formatting )
    {
      _line = new LineView( _flow );
      _flow->_lines.add( _line, *_lineCursor.cursor() );
    }
    else
      _mode = doneAll;
  }

  // setup the next line
  if ( _mode <= formatting )
  {
    // need to check conditions for line needing format
    Boolean needLineFormat = false;

    // first check if first view on this line is changed
    if ( isFirstViewChanged )
    {
      _line->setFirstView( & _viewCursor, false );
      needLineFormat = true;
    }

    // next check if line has moved vertically
    if ( _line->above() != _pen.above() + _pen.lineShift() + _pen.lineHeight() + _pen.lineDescent() )
      needLineFormat = true;

    // flag line for format, or check if done
    if ( needLineFormat )
    {
      if ( _linesNeedFormat < 1 )
        _linesNeedFormat = 1;
    }
    else
      checkIfDone();
  }

  // advance to next line
  if ( _mode <= sizing )
    _pen.beginLine( _line->_height, _line->_descent, _line->_width );
}


/***************************************************************************
 * Procedure.. FlowFormatter::checkForLineMerge
 * Author..... Mat
 * Date....... 8/29/96
 *
 * Called after a view is added to a line to see if the next line needs
 * to be adjusted or completely removed (if it was swallowed up by the
 * current line).
 *
 * _linesNeedFormat is maintained as a count of lines which must be
 * formatted to resolve merging or lines.  It is set to 2 whenever a line
 * is "taken" from its successor to indicate that both the line and
 * its successor need to be completely formatted.  It is decremented when
 * a line is removed due to a merge, or when the formatter moves to a new
 * line.
 ***************************************************************************/
void FlowFormatter::checkForLineMerge()
{
  IASSERTSTATE( _nextLine );

  // only concerned if current view = first view of next line
  if ( _view == _firstViewOnNextLine )
  {
    // current line has taken first view of next line
    if ( _firstViewOnNextLine == _lastViewOnNextLine )
    {
      // next line has no views left, delete it and advance nextLineCursor
      _nextLine->setFirstView( NULL );
      getNextLine( false, true );
      _linesNeedFormat = 1;
    }
    else
    {
      // advance to next line's first view to leave this one behind
      _nextLine->firstView()->setToNext();
      _firstViewOnNextLine = _nextLine->firstView()->view();
      _linesNeedFormat = 2;
    }
  }
}



/***************************************************************************
 * Procedure.. FlowFormatter::getNextLine
 * Author..... Mat
 * Date....... 7/10/97
 *
 * Search for the next non-empty line and set the cursor, the LineView
 * pointer and pointers to the first and last Views on that line.
 *
 * If startFromCurrent is true, then the search is started with the
 * line after the current line (_lineCursor).
 *
 * If isPenStarted is true AND lines were deleted, then the line space will
 * be deleted from the FormatPen, too.  Otherwise _lineSpaceDeleted, will
 * hold the amount of space to be deleted on return.
 *
 * The current pen point is assumed to be above any deleted lines!
 ***************************************************************************/
void FlowFormatter::getNextLine( Boolean startFromCurrent, Boolean isPenStarted )
{
  // initialize search
  Boolean found = false;
  Boolean valid = true;
  if ( startFromCurrent )
  {
    IASSERTSTATE( _lineCursor.isValid() );
    _nextLineCursor = _lineCursor;
    valid = _nextLineCursor.setToNext();
  }

  // search for the next non-empty line, deleting empty lines
  Boolean needBeginLine = false;
  while ( ! found && valid )
  {
    if ( valid )
    {
      // get pointer to next line
      _nextLine = (LineView *) _nextLineCursor.view();
      IASSERTSTATE( _nextLine );

      // check for non-empty line, or delete empty line
      if ( _nextLine->firstView() )
        found = true;
      else
      {
        // advance cursor before deleting line
        valid = _nextLineCursor.setToNext();
        // add to space deleted
        _lineSpaceDeleted += ( _nextLine->_height + _nextLine->_descent );
        // delete the line
        _nextLine->removeSelf();
        // mark that the Flow extent has changed
        _isExtentChanged = true;
      }
    }
  }

  // visually delete the lines
  if ( isPenStarted && _lineSpaceDeleted )
  {
    _pen.deleteLine( _lineSpaceDeleted );
    _lineSpaceDeleted = 0;
  }

  // save shortcut pointer
  if ( found )
  {
    _firstViewOnNextLine = _nextLine->firstView()->view();
    _lastViewOnNextLine = _nextLine->lastView()->view();
  }

  // set _nextLine to NULL to signal no more lines
  if ( ! found )
    _nextLine = NULL;
}



/***************************************************************************
 * Procedure.. FlowFormatter::findStartingView
 * Author..... Mat
 * Date....... 3/6/97
 *
 * Given _view as the requested starting view for formatting, search
 * backwards as necessary to find a valid starting view for formatting.
 * The following Views cannot be used as a starting point:
 *   1. a view without a subject (e.g., LineEndView)
 *   2. an "unformatted" view (as determied by isFormatted()
 *
 * Returns true if formatting needs to start at the top (there is no valid
 * view before the specified one).
 ***************************************************************************/
Boolean FlowFormatter::findStartingView()
{
  ViewCursor * cursor = _view->newCursorToThis();
  LineView *   line = (LineView *) _view->parent();
  ViewCursor * lineCursor = line->newCursorToThis();
  Boolean      startAtTop = false;
  Boolean      isValidStart;
  Boolean      found;

  /// should be able to use _viewCursor and avoid lines entirely!
  /// (LineEndViews would be skipped -- is this OK?)
  /// does view->subject() need to be checked if using _viewCursor?

  // search backwards for a valid starting view
  do
  {
    isValidStart = _view->isFormatted() && _view->subject();
    if ( ! isValidStart )
    {
      // see if we can go backwards further
      found = cursor->setToPrevious();
      if (! found )
      {
        // try to move to previous line
        // skip empty lines (firstView = NULL)
        do
        {
          found = lineCursor->setToPrevious();
          if ( found )
            line = (LineView *) lineCursor->view();
        } while ( found && line->firstView() == NULL );

        // get last view on current line
        if ( found )
        {
          delete cursor;
          cursor = line->newCursor();
          IASSERTSTATE( cursor )
          found = cursor->setToLast();
          IASSERTSTATE( found );
        }
        else
        {
          startAtTop = true;
          break;
        }
      }
      _view = (PlacedView *) cursor->view();
    }
  } while( ! isValidStart );
  delete lineCursor;
  delete cursor;

  // set the FlowFormatter's cursor
  if ( startAtTop )
  {
    // can't go further back, reset to start at very first view
    found = _viewCursor.setToFirst();
    IASSERTSTATE( found );
    _view = (PlacedView *) _viewCursor.view();
  }
  else
  {
    found = _viewCursor.locate( _view );
    IASSERTSTATE( found );
  }

  return startAtTop;
}


