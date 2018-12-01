/***************************************************************************
 * File...... WindowDef.cpp
 * Author.... Mat
 * Date...... 10/13/95
 *
 * Implementation for WindowDef, which defines a window size, position and
 * attributes.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "WindowDef.hpp"


WindowDef::WindowDef(
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
):
  _id            ( id ),
  _title         ( title ),
  _originX       ( originX ),
  _originY       ( originY ),
  _width         ( width ),
  _height        ( height ),
  _border        ( border ),
  _scrolling     ( scrolling ),
  _hasTitleBar   ( hasTitleBar ),
  _hasMinimize   ( hasMinimize ),
  _hasMaximize   ( hasMaximize ),
  _hasSystemMenu ( hasSystemMenu )
{}



