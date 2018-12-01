/***************************************************************************
 * File...... WordViewCursor.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for WordViewCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // ASSERT macros


inline unsigned WordViewCursor::index() const
{
  return _index;
}


inline void WordViewCursor::setIndex( unsigned index )
{
  _index = index;
}


inline CharView * WordViewCursor::view()
{
  IASSERTSTATE( _index );  // check for valid cursor
  return &_charView;
}


inline const CharView * WordViewCursor::view() const
{
  IASSERTSTATE( _index );  // check for valid cursor
  return &_charView;
}


inline IBoolean WordViewCursor::isFirst() const
{
  return ( _index == 1 );
}


inline IBoolean WordViewCursor::isValid () const
{
  return ( _index > 0 );
}


inline void WordViewCursor::invalidate ()
{
  _index = 0;
}


