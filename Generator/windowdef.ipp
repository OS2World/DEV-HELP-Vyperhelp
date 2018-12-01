/***************************************************************************
 * File...... WindowDef.ipp
 * Author.... Mat
 * Date...... 12/9/97
 *
 * Inline functions for WindowDef
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// query functions

inline const int &      WindowDef::id() const
{ return _id; }

inline const IString &  WindowDef::title() const
{ return _title; }

inline const Position & WindowDef::originX() const
{ return _originX; }

inline const Position & WindowDef::originY() const
{ return _originY; }

inline const Distance & WindowDef::width() const
{ return _width; }

inline const Distance & WindowDef::height() const
{ return _height; }

inline WindowDef::Border           WindowDef::border() const
{ return _border; }

inline WindowDef::Scrolling        WindowDef::scrolling() const
{ return _scrolling; }

inline Boolean          WindowDef::hasTitleBar() const
{ return _hasTitleBar; }

inline Boolean          WindowDef::hasMinimize() const
{ return _hasMinimize; }

inline Boolean          WindowDef::hasMaximize() const
{ return _hasMaximize; }

inline Boolean          WindowDef::hasSystemMenu() const
{ return _hasSystemMenu; }


// global key function for WindowSet

inline const int & key( const WindowDef & window )
{ return window.id(); }


