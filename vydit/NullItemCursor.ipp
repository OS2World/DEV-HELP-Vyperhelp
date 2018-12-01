/***************************************************************************
 * File...... NullItemCursor.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for NullItemCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline IBoolean NullItemCursor::setToLast()
{
  return false;
}


inline IBoolean NullItemCursor::setToPrevious()
{
  return false;
}


inline IBoolean NullItemCursor::setToFirst()
{
  return false;
}


inline IBoolean NullItemCursor::setToNext()
{
  return false;
}


inline IBoolean NullItemCursor::isValid() const
{
  return false;
}


inline void NullItemCursor::invalidate()
{}


