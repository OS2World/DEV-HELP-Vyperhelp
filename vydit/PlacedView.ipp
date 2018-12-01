/***************************************************************************
 * File...... PlacedView.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for PlacedView
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros


inline IString PlacedView::info() const
{
  return IString();
}


inline const PenState & PlacedView::state() const
{
  IASSERTSTATE( _state );
  return *_state;
}


inline PlacedView::LineMode PlacedView::lineMode() const
{
  return allowNewLine;
}


inline PlacedView::FitMode PlacedView::fitMode() const
{
  return allowFit;
}


inline Boolean PlacedView::isFormatted() const
{
  return ( _xPosition != -1 );
}


inline void PlacedView::clearFormatted()
{
  _xPosition = -1;
}


inline IPoint PlacedView::position() const
{
  IASSERTSTATE( _xPosition != -1 );
  return IPoint( _xPosition, 0 );
}


