/***************************************************************************
 * File...... WordView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for WordView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline unsigned WordView::length() const
{
  return _length;
}


inline PlacedView::LineMode WordView::lineMode() const
{
  return noNewLine;
}


