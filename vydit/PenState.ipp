/***************************************************************************
 * File...... PenState.ipp
 * Author.... Mat
 * Date...... 1/3/97
 *
 * Inline functions for PenState
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

inline int PenState::operator==( const PenState & state ) const
{
  // check _maxAscender to see if metrics have been set yet
  return ( _font        == state._font )
      && ( _color       == state._color )
      && ( _maxAscender == state._maxAscender );
}


inline int PenState::operator!=( const PenState & state ) const
{
  return ! operator==( state );
}


inline const FontInfo & PenState::font() const
{
  return _font;
}


inline const IColor & PenState::color() const
{
  return _color;
}


inline void PenState::setColor( const IColor & color )
{
  _color = color;
}


inline unsigned long PenState::charWidth( unsigned char c ) const
{
  return _charWidths[c];
}


inline unsigned long PenState::maxAscender() const
{
  return _maxAscender;
}


inline unsigned long PenState::maxDescender() const
{
  return _maxDescender;
}


inline int PenState::count() const
{
  return _count;
}


inline void PenState::setCount( int count )
{
  _count = count;
}


inline Boolean PenState::decrementCount()
{
  return ( --_count == 0 );
}


inline void PenState::incrementCount()
{
  _count++;
}


inline unsigned long hash( const PenState & state, unsigned long modulo )
{
  return state.hash( modulo );
}


