/***************************************************************************
 * File...... FlowView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for FlowView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


/***************************************************************************
 * Procedure.. FlowView::setEditor
 * Author..... Mat
 * Date....... 6/5/96
 *
 * Sets a pointer to the owning Editor object
 ***************************************************************************/
inline void FlowView::setEditor( Editor * editor )
{
  _editor = editor;
}


/***************************************************************************
 * Procedure.. FlowView::sizeTo
 * Author..... Mat
 * Date....... 8/27/96
 *
 * Sets the size of the FlowView.  Called by the FlowFormatter during
 * formatting.
 ***************************************************************************/
inline void FlowView::sizeTo( const ISize & size )
{
  _size = size;
}


inline Coord FlowView::width() const
{
  return _size.width();
}


inline Coord FlowView::height() const
{
  return _size.height();
}


