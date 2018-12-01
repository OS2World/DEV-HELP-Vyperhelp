/***************************************************************************
 * File...... PushView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for PushView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

inline PopView * PushView::pop() const
{
  return _pop;
}


inline void PushView::setPop( PopView * pop )
{
  _pop = pop;
}


inline const IGBitmap & PushView::bitmap() const
{
  return _bitmap;
}


