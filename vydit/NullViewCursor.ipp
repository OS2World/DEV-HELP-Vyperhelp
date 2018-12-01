/***************************************************************************
 * File...... NullViewCursor.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for NullViewCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline IBoolean NullViewCursor::isFirst() const
{
  return false;
}


inline IBoolean NullViewCursor::isLast() const
{
  return false;
}


inline IBoolean NullViewCursor::setToLast()
{
  return false;
}


inline IBoolean NullViewCursor::setToPrevious()
{
  return false;
}


inline IBoolean NullViewCursor::setToFirst()
{
  return false;
}


inline IBoolean NullViewCursor::setToNext()
{
  return false;
}


inline IBoolean NullViewCursor::isValid() const
{
  return false;
}


inline void NullViewCursor::invalidate()
{}


