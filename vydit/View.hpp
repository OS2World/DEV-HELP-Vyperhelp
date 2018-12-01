/***************************************************************************
 * File...... View.hpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * Every Item has a View component which contains the user interface aspect
 * of the item.  Some Views exist which don't have a corresponding Item
 * (for example, LineView).
 *
 * Views also map commands from the user interface into methods for the
 * corresponding Items.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef VIEW_HPP
#define VIEW_HPP


// OpenClass
#include <istring.hpp>       // IString
#include <ipoint.hpp>        // IPoint
#include <irect.hpp>         // IRectangle

// Generator
class Generator;

// TextEditor
#include "Types.hpp"
class Item;
class ViewCursor;
class DrawPen;


class _Export View
{

  public:
    // CONSTRUCTOR
    View( View * parent );
      // parent is NULL for the top level View
    virtual ~View();

    // RELATIONSHIPS
    virtual View * parent() const;
      // returns the parent view
    void           setParent( View * view );
    virtual ViewCursor * newCursor();
      // default returns a NullViewCursor
    virtual Boolean isLine() const;
      // returns true if this is a LineView
      // default returns false
    virtual void removeChild( View * view );
      // remove the child from the collection
      // default does nothing (assumes it has no children)
    virtual void removeSelf();
      // remove self from parent's collection, then "delete this"
    virtual Boolean isTransient() const;
      // returns true is the view is "transient", i.e., created on the
      //   fly by the editor and not representing a true user item
      // default returns false
    virtual Boolean isOrphan() const;
      // returns true if the view has been orphaned by a related deletion
      //  (e.g., a PopView is orphaned when its PushView is deleted)
      // default returns false

    // CURSORS
    View * locateDescendent( View & descendent );
      // return pointer to child which is in the tree of the given descendent
      // NULL if not a descendent
    ViewCursor * newCursorToThis();
      // returns a new cursor this View (using a cursor supplied by parent)
    virtual ViewCursor * newLeafCursor( const Boolean wantFirst );
      // returns cursor to first leaf of the View or NULL if none is found
      // if the View itself is a leaf, then it returns cursor to itself
      // default searches recursively for a leaf
    virtual ViewCursor * newLineCursor();
      // returns cursor to line of the View or NULL if none is found
      // if the View itself is a LineView, then it returns cursor to itself
      // default searches recursively for a line
    virtual int orderViews( View & view1, View & view2 );
      // returns <0 if view1 < view2, 0 if equal, >0 if view1 > view2
      // "<" means "appears before" or "is an ancestor"
      // default returns 0

    // HIT CORRELATION
    virtual ViewCursor * newCorrelateCursor( IPoint & point );
      // return lowest level view with a hit

    // LEAF-VIEW SUPPORT
    virtual void insertItem( Item * item );
      // default does nothing

    // UPDATE SUPPORT
    virtual void update( View & childView );
      // called when a child needs to be formatted
      // default calls parent->update, until someone can act on it
    virtual void draw( DrawPen & pen, const IRectangle & zone );
      // uses the pen to draw the item and move forward beyond the View
      // assumes left-to-right movement
      // responsible for clearing owned width from top to bottom of line
      // default clears above/below then advances pen

    // METRICS
    virtual Coord  width() const;
      // default returns 0
    virtual Coord  height() const;
      // default returns 1 (to ensure non-0 area)
    virtual Coord  descent() const;
      // default returns 0
    virtual IPoint position() const;
      // position of the view in its parent's space
      // default returns (0,0)
    Coord          extent() const;
      // returns height() + descent()
    ISize          size() const;
      // returns the size of the view, using width() and extent()
    IRectangle     rect() const;
      // returns rectangle coordinates within parent,
      // using position(), width() and extent()

    // SUPPORT FOR TOP-LEVEL
    Coord       fullExtent();
      // extent of the line on which this view belongs
    IPoint      fullPosition() const;
      // position of the view in the top-level view's space
    IRectangle  fullRect();
      // returns rectangle coordinates within top-level view's space,
      // using fullPosition(), width() and fullExtent()

    // COMMANDS
    virtual unsigned remove( ViewCursor * fromCursor, ViewCursor * toCursor );
      // removes all sub-views in the specified range, called recursively
      // also responsible for deleting the cursor if it doesn't need to pass it down
      // returns true if search for orphan PopViews is needed
      // default removes self and deletes cursors, returns true
    virtual void     generate( View * from, View * to, unsigned & level );
      // generates Gins corresponding to the range of views specified, called recursively
      // level maintains the level of Push/Pops
      // default does nothing

    // command handler?
    // Boolean allowOvertype
    // Boolean isVisible  or  Boolean isMarkup
      // (invisible implies no overtype)

    // DEBUGGING
    virtual IString dumpString() const = 0;

  protected:
    IString debugString( const IString & name ) const;
      // appends object address to the given name

  private:
    View *  _parent;
};


// inline functions
#include "View.ipp"


#endif

