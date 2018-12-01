/***************************************************************************
 * File...... LineSubviewCursor.hpp
 * Author.... Mat
 * Date...... 5/16/96
 *
 * This ViewCursor supports traversing the Views within a LineView.  The
 * Views are actually held by the LineView's parent FlowView and assigned
 * to different lines by the formatting routine.
 *
 * Also automatically handles insertion of the LineEndView after all other
 * views in the line. Assumes that the LineView has at least one subview other
 * than the LineEndView.
 *
 * NOTE: LineSubviewCursor can't be used on "empty" LineViews.  It assumes
 * that the line has a first/last subview.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef LINESUBVIEWCURSOR_HPP
#define LINESUBVIEWCURSOR_HPP


// TextEditor
#include "OrderedViewCursor.hpp"
class LineView;
class LineEndView;


class LineSubviewCursor: public ViewCursor
{
  public:
    // constructor
    LineSubviewCursor( LineView & line, LineEndView * lineEnd );
      // set lineEndView to NULL if the line.lastView stands as a line end
    ~LineSubviewCursor();

    // from ViewCursor
    virtual View * view();
    virtual const View * view() const;
    virtual IBoolean isFirst() const;
    virtual IBoolean isLast() const;

    // ala IOrderedCursor
    virtual IBoolean setToLast ();
    virtual IBoolean setToPrevious ();

    // ala ICursor
    virtual IBoolean setToFirst ();
    virtual IBoolean setToNext ();
    virtual IBoolean isValid () const;
    virtual void     invalidate ();

  private:
    LineView *          _line;
    OrderedViewCursor * _viewCursor;  // NULL if there are no cursored views
    LineEndView *       _lineEnd;     // NULL if there is no LineEndView
    enum {
      cursor,   // not valid if _viewCursor == NULL
      end,      // not valid if _lineEnd == NULL
      invalid
    }                   _state;
};


// inline functions
#include "LineSubviewCursor.ipp"


#endif

