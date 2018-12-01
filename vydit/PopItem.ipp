/***************************************************************************
 * File...... PopItem.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for PopItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline PushItem * PopItem::buddy()
{
  return _buddy;
}


inline PopItem * PopItem::asPop()
{
  return this;
}

