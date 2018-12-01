/***************************************************************************
 * File...... FlowView.hpp
 * Author.... Mat
 * Date...... 2/8/96
 *
 * FlowView is the view corresponding to a FlowItem.  During formatting,
 * it interjects LineViews to implemement the line breaking algorithm.
 *
 * NOTE: FlowFormatter is a friend class which handles all formatting
 * duties and directly manipulates some of the FlowView's private data.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef FLOWVIEW_HPP
#define FLOWVIEW_HPP


// TextEditor
#include "ViewSequence.hpp"
#include "OrderedViewCursor.hpp"
#include "SubjectView.hpp"
class PlacedView;
class OrderedItemCursor;
class FlowItem;
class ItemCursor;
class Editor;
class LineView;
class FlowFormatter;


class FlowView: public SubjectView
{
  public:
    // constructor
    FlowView( FlowItem * subject );

    // support for top-level View
    void    setEditor( Editor * editor );
    int     orderLines( LineView & line1, LineView & line2 );
      // return <0 if line1 less than line2, 0 if equal, >0 if greater
    Boolean checkForWordMerge( OrderedViewCursor * cursor );
      // if given view is a WordView, check previous for WordView and merge if necessary
      // return true if words were merged and cursor was setToNext

    // resizing by FlowFormatter
    void sizeTo( const ISize & size );

    // support for PlacedViews (LineView children)
    int                 orderPlacedViews( View * view1, View * view2 );
    void                removePlacedView( View * view );
    OrderedViewCursor * newViewCursor( View * view = NULL );
      // gives the a cursor into the Flow's collection
      // points it to the given View (or to the first if view == NULL)

    // support for changing Views (from word merge)
    void changeView( PlacedView * viewOld, PlacedView * viewNew );

    // from SubjectView
    virtual void dispatchNotificationEvent( INotificationId id, const IEventData & event = IEventData() );

    // from View
    virtual OrderedViewCursor * newCursor();
    virtual void                update( View & childView );
    virtual void                draw( DrawPen & pen, const IRectangle & zone );
    virtual void                removeChild( View * view );
    virtual ViewCursor *        newCorrelateCursor( IPoint & point );
    virtual int                 orderViews( View & view1, View & view2 );
    virtual Coord               width() const;
    virtual Coord               height() const;
    virtual unsigned            remove( ViewCursor * fromCursor, ViewCursor * toCursor );
    virtual void                generate( View * from, View * to, unsigned & level );
    virtual IString             dumpString() const;

  private:
    friend class FlowFormatter;  // handles all formatting duties

    // event handlers
    void insertViews( OrderedItemCursor & itemCursor, Boolean checkWordMerge );

    // private data
    Editor *      _editor;
    ViewSequence  _views;
    ViewSequence  _lines;
    ISize         _size;
};


// inline functions
#include "FlowView.ipp"


#endif

