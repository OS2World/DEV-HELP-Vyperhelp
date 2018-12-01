/***************************************************************************
 * File...... EndView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for EndView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IGBitmap & EndView::bitmap() const
{
  return _bitmap;
}


inline PlacedView::LineMode EndView::lineMode() const
{
  return forceNewLine;
}


inline PlacedView::FitMode EndView::fitMode() const
{
  return forceFit;
}


