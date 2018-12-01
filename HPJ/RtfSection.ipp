/***************************************************************************
 * File...... RtfSection.ipp
 * Author.... Mat
 * Date...... 1/12/99
 *
 * Inline functions for RtfSection
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline void RtfSection::setTitle( const IString & title )
{
  _title = title;
}


inline void RtfSection::setLabel( LabelGin * label )
{
  _label = label;
}


inline const IString &  RtfSection::title() const
{
  return _title;
}


inline const LabelGin * RtfSection::label() const
{
  return _label;
}


