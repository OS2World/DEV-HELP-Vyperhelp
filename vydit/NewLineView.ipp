/***************************************************************************
 * File...... NewLineView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for NewLineView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IGBitmap & NewLineView::bitmap() const
{
  return _bitmap;
}


inline PlacedView::LineMode NewLineView::lineMode() const
{
  return forceNewLine;
}


inline PlacedView::FitMode NewLineView::fitMode() const
{
  return forceFit;
}


