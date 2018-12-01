/***************************************************************************
 * File...... FontInfo.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for FontInfo
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IString & FontInfo::name() const
{
  return _name;
}


inline unsigned long   FontInfo::pointSize() const
{
  return _pointSize;
}


inline Boolean         FontInfo::isBold() const
{
  return ( ( _flags & bold ) == bold );
}


inline Boolean         FontInfo::isItalic() const
{
  return ( ( _flags & italic ) == italic );
}


inline Boolean         FontInfo::isUnderline() const
{
  return ( ( _flags & underline ) == underline );
}


inline void FontInfo::setBold( Boolean isBold )
{
  _flags = isBold? ( _flags | bold ): ( _flags & ~bold );
}


inline void FontInfo::setItalic( Boolean isItalic )
{
  _flags = isItalic? ( _flags | italic ): ( _flags & ~italic );
}


inline void FontInfo::setUnderline( Boolean isUnderline )
{
  _flags = isUnderline? ( _flags | underline ): ( _flags & ~underline );
}


inline void FontInfo::setVector( Boolean isVector )
{
  _flags = isVector? ( _flags | vector ): ( _flags & ~vector );
}


