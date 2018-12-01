/***************************************************************************
 * File...... FontItem.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for FontItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IString & FontItem::name() const
{
  return _info.name();
}


inline unsigned long FontItem::pointSize() const
{
  return _info.pointSize();
}


