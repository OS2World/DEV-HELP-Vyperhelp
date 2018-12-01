/***************************************************************************
 * File...... OrderedItemCursor.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for OrderedItemCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline IOrderedCursor<Item*> * OrderedItemCursor::cursor()
{
  return _cursor;
}


inline const IOrderedCursor<Item*> * OrderedItemCursor::cursor() const
{
  return _cursor;
}


inline Item * OrderedItemCursor::item() const
{
  return _cursor->element();
}


inline IBoolean OrderedItemCursor::setToLast ()
{
  return _cursor->setToLast();
}


inline IBoolean OrderedItemCursor::setToPrevious ()
{
  return _cursor->setToPrevious();
}


inline IBoolean OrderedItemCursor::setToFirst ()
{
  return _cursor->setToFirst();
}


inline IBoolean OrderedItemCursor::setToNext ()
{
  return _cursor->setToNext();
}


inline IBoolean OrderedItemCursor::isValid () const
{
  return _cursor->isValid();
}


inline void OrderedItemCursor::invalidate ()
{
  _cursor->invalidate();
}


