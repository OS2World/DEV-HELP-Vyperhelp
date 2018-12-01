/***************************************************************************
 * File...... WordItem.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for WordItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline const IString & WordItem::string() const
{
  return _string;
}


inline WordItem * WordItem::asWord()
{
  return this;
}


