/***************************************************************************
 * File...... Registration.ipp
 * Author.... Mat
 * Date...... 4/30/99
 *
 * Inline functions for Registration class.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline IString Registration::name() const
{
  return IString( _name );
}

inline const UserKey & Registration::key() const
{
  return _key;
}


