/***************************************************************************
 * File...... StringPair.ipp
 * Author.... Mat
 * Date...... 10/29/98
 *
 * Inline functions for StringPair
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline const IString & StringPair::key() const
{
  return _key;
}


inline const IString & StringPair::other() const
{
  return _other;
}


inline const IString & key( const StringPair & pair )
{
  return pair.key();
}

