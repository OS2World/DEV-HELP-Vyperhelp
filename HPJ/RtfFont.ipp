/***************************************************************************
 * File...... RtfFont.ipp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * Inline functions for RtfFont
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline RtfFont::Familyname RtfFont::family() const
{ return _fontfamily; }

inline RtfFont::Fprqtype RtfFont::pitch() const
{ return _fprq; }

inline const IString & RtfFont::getFacename() const
{return _facename;}

inline int RtfFont::getCodepage() const
{return _codepage;}

inline const int & RtfFont::getKey() const
{return _fontnumberKey;}

inline const int & key( const RtfFont & font )
{return font.getKey();}

