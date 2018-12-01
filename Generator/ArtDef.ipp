/***************************************************************************
 * File...... ArtDef.ipp
 * Author.... Mat
 * Date...... 12/9/97
 *
 * Inline functions for ArtDef
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// query functions

inline const IString & ArtDef::id() const
{ return _id; }

inline const IGBitmap & ArtDef::bitmap() const
{ MEKASSERT( _bitmap ); return *_bitmap; }


// global key function for ArtSet

inline const IString & key( const ArtDef & art )
{ return art.id(); }

