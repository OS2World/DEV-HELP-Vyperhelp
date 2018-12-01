/***************************************************************************
 * File...... RtfColor.ipp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * Inline functions for RtfColor
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline const int & RtfColor::getKey() const
{return _colornumberKey;}


inline const IColor * RtfColor::getColorentry() const
{
  return _valid? &_colorentry: 0;
}

inline const int & key( const RtfColor & color )
{ return color.getKey(); }

