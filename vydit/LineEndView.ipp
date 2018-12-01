/***************************************************************************
 * File...... LineEndView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for LineEndView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline Boolean LineEndView::isTransient() const
{
  return true;
}


inline const IGBitmap & LineEndView::bitmap() const
{
  return _bitmap;
}


// LineEndView is allocated as part of LineView, so it should not be deleted
// also, it does not need to be removed from any collection
inline void LineEndView::removeSelf()
{}


inline PlacedView::LineMode LineEndView::lineMode() const
{
  return forceNewLine;
}


inline PlacedView::FitMode LineEndView::fitMode() const
{
  return forceFit;
}


