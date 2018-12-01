/***************************************************************************
 * File...... ParagraphView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for ParagraphView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IGBitmap & ParagraphView::bitmap() const
{
  return _bitmap;
}


inline PlacedView::LineMode ParagraphView::lineMode() const
{
  return forceNewLine;
}


inline PlacedView::FitMode ParagraphView::fitMode() const
{
  return forceFit;
}


