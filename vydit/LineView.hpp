/***************************************************************************
 * File...... LineView.hpp
 * Author.... Mat
 * Date...... 5/9/96
 *
 * LineView represents one line/row of items, as managed by FlowView.
 * LineView has no corresponding Item, rather it is used by the line-breaking
 * algorithm of FlowView.
 *
 * NOTE: FlowFormatter is a friend class which handles all formatting
 * duties and directly manipulates some of the LineView's private data.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef LINEVIEW_HPP
#define LINEVIEW_HPP


// TextEditor
#include "LineEndView.hpp"
#include "ViewSequence.hpp"
#include "View.hpp"
class FlowView;
class ViewCursor;
class OrderedViewCursor;
class LineEndView;


class LineView: public View
{
  public:
    // constructor
    LineView( FlowView * parent );
    ~LineView();

    // support for FlowView, LineSubviewCursor, LineEndView
    LineView & setFirstView( const OrderedViewCursor * firstView, Boolean setLast = true );
      // used to set the pointer to the firstView
      // if setLast = true, then the lastView will also be set to match the first
      // a copy of the cursor will be made
      // use NULL to set the line to "unused"
    OrderedViewCursor * firstView();
      // returns pointer to the line's first view
      // returns NULL if line is unused
    LineView & setLastView( const OrderedViewCursor * lastView );
      // used to set the pointer to the lastView
      // a copy of the cursor will be made
      // lastView becomes nullified when setFirstView(NULL) is called
    OrderedViewCursor * lastView();
      // returns pointer to the line's last view
      // returns NULL if line is unused

    // formatting support
    Coord above();
      // returns the amount of (absolute) space above top of this line

    // from View
    virtual ViewCursor * newCursor();
    virtual ViewCursor * newLineCursor();
    virtual Boolean      isLine() const;
    virtual IPoint       position() const;
    virtual ViewCursor * newCorrelateCursor( IPoint & point );
    virtual int          orderViews( View & view1, View & view2 );
    virtual void         update( View & childView );
    virtual void         draw( DrawPen & pen, const IRectangle & zone );
    virtual Coord        width() const;
    virtual Coord        height() const;
    virtual Coord        descent() const;
    virtual unsigned     remove( ViewCursor * fromCursor, ViewCursor * toCursor );
    virtual void         generate( View * from, View * to, unsigned & level );
    virtual void         removeChild( View * view );
    virtual IString      dumpString() const;

  private:
    friend class FlowFormatter;       // handles all formatting of Flow and Line

    FlowView * flow() const;          // specialized parent access

    OrderedViewCursor * _firstView;
    OrderedViewCursor * _lastView;
    Coord               _width;       // sum of widths of all items on line
    Coord               _height;      // largest height of all items on line
    Coord               _descent;     // largest descent of all items on line
    Coord               _above;       // space above in the FlowView
    LineEndView         _lineEnd;
    Boolean             _needLineEnd; // true if lastView is not a valid line end
};


// inline functions
#include "LineView.ipp"


#endif

