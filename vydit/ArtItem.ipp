/***************************************************************************
 * File...... ArtItem.ipp
 * Author.... Mat
 * Date...... 9/28/99
 *
 * Inline functions for ArtItem
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline const IString & ArtItem::id() const
{
  return _id;
}


inline ArtItem * ArtItem::asArt()
{
  return this;
}


