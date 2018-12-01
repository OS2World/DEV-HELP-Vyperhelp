/***************************************************************************
 * File...... Pen.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for Pen
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const PenPoint & Pen::point() const
{
  return _point;
}


inline Coord Pen::lineHeight() const
{
  return _lineHeight;
}


inline Coord Pen::lineDescent() const
{
  return _lineDescent;
}


inline Coord Pen::lineWidth() const
{
  return _lineWidth;
}


inline const PenState & Pen::state() const
{
  return _state;
}


inline Page & Pen::page()
{
  return _page;
}


inline PenPoint & Pen::point()
{
  return _point;
}


inline void Pen::setLineHeight( Coord height )
{
  _lineHeight = height;
}


inline void Pen::setLineDescent( Coord descent )
{
  _lineDescent = descent;
}


inline void Pen::setLineWidth( Coord width )
{
  _lineWidth = width;
}


