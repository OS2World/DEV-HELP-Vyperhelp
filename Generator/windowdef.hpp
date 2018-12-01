/***************************************************************************
 * File...... WindowDef.hpp
 * Author.... Mat
 * Date...... 10/5/95
 *
 * Window definitions, including size, position, title, borders and other
 * attributes.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef WINDOWDEF_HPP
#define WINDOWDEF_HPP

// Open Class
#include <istring.hpp>

// Generator
#include "Distance.hpp"


/***************************************************************************
 * Class...... WindowDef
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Defines a window by its position, size and attributes.
 ***************************************************************************/
class _Export WindowDef
{
  public:
    // enumerations
    enum Border {
      noBorder,
      staticBorder,
      sizeBorder
    };
    enum Scrolling {
      noScroll,
      vertScroll,
      horizScroll,
      allScroll
    };

    // constructor
    WindowDef(
      int              id,             // window identifier
      const IString &  title,          // for the titlebar or heading
      const Position & originX,        // x-coord of left side of window
      const Position & originY,        // y-coord of bottom of window
      const Distance & width,          // window width
      const Distance & height,         // window height
      Border           border,         // type of border
      Scrolling        scrolling,      // type of scrolling allowed
      Boolean          hasTitleBar,    // has a title bar across top?
      Boolean          hasMinimize,    // has a minimize button?
      Boolean          hasMaximize,    // has a maximize button?
      Boolean          hasSystemMenu   // has a system menu?
    );

    // query functions
    const int &      id() const;       // return reference for key() function
    const IString &  title() const;
    const Position & originX() const;
    const Position & originY() const;
    const Distance & width() const;
    const Distance & height() const;
    Border           border() const;
    Scrolling        scrolling() const;
    Boolean          hasTitleBar() const;
    Boolean          hasMinimize() const;
    Boolean          hasMaximize() const;
    Boolean          hasSystemMenu() const;

  private:
    int       _id;
    IString   _title;
    Position  _originX;
    Position  _originY;
    Distance  _width;
    Distance  _height;
    Border    _border;
    Scrolling _scrolling;
    Boolean   _hasTitleBar;
    Boolean   _hasMinimize;
    Boolean   _hasMaximize;
    Boolean   _hasSystemMenu;
};


// global key access function
const int & key( const WindowDef & window );


// Inline functions
#include "WindowDef.ipp"


#endif

