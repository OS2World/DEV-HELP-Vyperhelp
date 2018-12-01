/***************************************************************************
 * File...... LineView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for LineView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline Boolean LineView::isLine() const
{
  return true;
}


inline OrderedViewCursor * LineView::firstView()
{
  return _firstView;
}


inline OrderedViewCursor * LineView::lastView()
{
  return _lastView;
}


inline Coord LineView::above()
{
  // assumes that the line's parent is the top-level view
  return _above;
}

inline Coord LineView::width() const
{
  return _width;
}


inline Coord LineView::height() const
{
  return _height;
}


inline Coord LineView::descent() const
{
  return _descent;
}


