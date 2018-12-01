/***************************************************************************
 * File...... PlacedView.hpp
 * Author.... Mat
 * Date...... 1/30/97
 *
 * PlacedView is a type of View which is placed by the FlowFormatter.
 * Most PlacedViews are also "leaf" views.  The notable exception is
 * WordView which has children CharViews which are leafs.
 *
 * PlacedViews have the following properties:
 *  - children of LineView
 *  - member of FlowView's collection (except LineEndView!)
 *  - always have a subject Item (except LineEndView!)
 *  - caches a PenState for efficient formatting and painting
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PLACEDVIEW_HPP
#define PLACEDVIEW_HPP

// OpenClass
#include <isethks.h>        // ISetOnHashKeySet (hash set)
#include <iptr.h>           // IElemPointer

// TextEditor
#include "PenState.hpp"
#include "SubjectView.hpp"
class FormatPen;

// collection types
typedef IElemPointer< PenState >            PenStatePointer;
typedef ISetOnHashKeySet< PenStatePointer > PenStateSet;


class _Export PlacedView: public SubjectView
{
  public:
    // constructor
    PlacedView( Item * subject, View * parent );
    virtual ~PlacedView();

    // info string
    virtual IString info() const;
      // return descritive text for the main window's information line
      // default returns a blank string

    // PenState support
    const PenState & state() const;
      // return reference to state of pen from last format

    // FORMATTING
    enum FormatChange
    {                   // format resulted in ...
      changeNothing,    //   no change
      changeAttribute,  //   attribute change (needs redraw)
      changeSize        //   size change (needs redraw, affects subsequent Views)
    };
    virtual FormatChange format( FormatPen & pen );
      // update size, insert line breaks + other format items
      // default returns changeSize on first call, changeNothing for subsequent calls
    enum LineMode
    {
      noNewLine,       // don't allow a new line after this View
      allowNewLine,    // allow a new line after this View
      forceNewLine     // force a new line after this View
    };
    virtual LineMode lineMode() const;
      // default returns allowNewLine
    enum FitMode
    {
      allowFit,    // OK to break a line before this View
      forceFit     // never break a line before this view
    };
    virtual FitMode fitMode() const;
      // default returns allowFit
    Boolean        isFormatted() const;
      // return true if the View has been formatted yet
    void           clearFormatted();
      // clear the "isFormatted" flag to invalidate the View's format info

    // from View
    virtual void         generate( View * from, View * to, unsigned & level );
    virtual void         insertItem( Item * item );
    virtual ViewCursor * newLeafCursor( const Boolean wantFirst );
    virtual IPoint       position() const;
      // default returns (x,0)
    virtual IString      dumpString() const;

  protected:
    void    setState( const PenState & state );
    Boolean setPosition( Coord xPosition );
    IString onOffString( Boolean isOn ) const;

  private:
    // helpers
    void removeState();

    // static data
    static PenStateSet _activeStates;

    // object data
    PenStatePointer    _state;
    Coord              _xPosition;    // -1 if not formatted
};


// inline functions
#include "PlacedView.ipp"


#endif

