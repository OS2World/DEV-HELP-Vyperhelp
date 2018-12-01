/***************************************************************************
 * File...... OrderedViewCursor.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for OrderedViewCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline IOrderedCursor< View * > * OrderedViewCursor::cursor()
{
  return _cursor;
}


inline const IOrderedCursor< View * > * OrderedViewCursor::cursor() const
{
  return _cursor;
}


inline View * OrderedViewCursor::view()
{
  return _cursor->element();
}


inline const View * OrderedViewCursor::view() const
{
  return _cursor->element();
}


inline IBoolean OrderedViewCursor::setToLast ()
{
  return _cursor->setToLast();
}


inline IBoolean OrderedViewCursor::setToPrevious ()
{
  return _cursor->setToPrevious();
}


inline IBoolean OrderedViewCursor::setToFirst ()
{
  return _cursor->setToFirst();
}


inline IBoolean OrderedViewCursor::setToNext ()
{
  return _cursor->setToNext();
}


inline IBoolean OrderedViewCursor::isValid () const
{
  return _cursor->isValid();
}


inline void OrderedViewCursor::invalidate ()
{
  _cursor->invalidate();
}


inline IBoolean OrderedViewCursor::operator == ( OrderedViewCursor const & c ) const
{
  return ( *_cursor == *c._cursor );
}


