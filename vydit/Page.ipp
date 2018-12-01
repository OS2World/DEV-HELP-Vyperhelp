/***************************************************************************
 * File...... Page.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for Page
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const ISize & Page::size() const
{
  return _size;
}


inline Coord Page::visibleWidth() const
{
  return _visibleWidth;
}


inline FastContext & Page::context()
{
  return *_context;
}


inline IColor Page::backgroundColor() const
{
  return _backgroundColor;
}


inline IColor Page::defaultColor() const
{
  return _defaultColor;
}


inline void Page::resetZone()
{
  _zone = IRectangle();
}


inline IRectangle Page::zone() const
{
  return _zone;
}


