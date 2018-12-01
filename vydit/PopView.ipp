/***************************************************************************
 * File...... PopView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for PopView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline PushView * PopView::push() const
{
  return _push;
}


inline const IGBitmap & PopView::bitmap() const
{
  return _bitmap;
}


inline void PopView::setOrphan()
{
  _push = NULL;  // become an orphan
}


inline Boolean PopView::isOrphan() const
{
  return ( _push == NULL );
}


