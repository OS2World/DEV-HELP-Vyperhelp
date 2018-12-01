/***************************************************************************
 * File...... Pen.hpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * Pen provides line-by-line travesal of a FlowView / Page.
 * The pen moves a "pen point" from upper left across lines and down the
 * virtual page.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef PEN_HPP
#define PEN_HPP

// TextEditor
#include "Types.hpp"
#include "PenState.hpp"
#include "PenPoint.hpp"
class PlacedView;
class Page;


class Pen
{
  public:
    // constructor
    Pen( Page & page );
    virtual ~Pen();

    // initialization
    void start( PlacedView & view );
      // start formatting at the given View

    // pen point control
    void             forward( Coord width );
      // moves the pen point forward (positive x-direction)
    const PenPoint & point() const;
    PenPoint &       point();
      // returns the pen point
    Coord            lineHeight() const;
    Coord            lineDescent() const;
      // returns the current line height and descent
    Coord            lineWidth() const;
      // returns the current line width

    // font attributes
    // return true if the attribute changed
    virtual Boolean setFont( const IString & name, unsigned long pointSize );
      // sets the current font name and size
    virtual Boolean setBold( Boolean isBold );
    virtual Boolean setItalic( Boolean isItalic );
    virtual Boolean setUnderline( Boolean isUnderline );
      // sets the current font attributes
    virtual Boolean setColor( const IColor * color = 0 );
      // sets foreground text color (0 = default)

    // state management
    const PenState &  state() const;
    virtual void      setState( const PenState & newState );

  /// typestyle
  /// paragraph prefix
  /// row
  /// col
  /// # cols
  /// for each col: IRectangle
  /// for each col: margins, width, clip/wrap, borders
  /// rules:
  ///   must be monospaced?
  ///   margin/indent
  ///   nesting
  ///   typestyle

  protected:
    // access to members
    Page &     page();
    void       setLineHeight( Coord height );
    void       setLineDescent( Coord descent );
    void       setLineWidth( Coord width );

  private:
    // helpers
    void setContextFont();
    void setContextColor();

    // page
    Page & _page;

    // metrics for current line
    Coord _lineDescent;       // line's previous size
    Coord _lineHeight;
    Coord _lineWidth;

    // pen point
    PenPoint _point;

    // state
    PenState _state;
};


// inline functions
#include "Pen.ipp"


#endif

