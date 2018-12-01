/***************************************************************************
 * File...... UserKey.ipp
 * Author.... Mat
 * Date...... 5/4/99
 *
 * Inline functions for UserKey
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


#include <memory.h>


inline Boolean UserKey::isValid() const
{
  return _isValid;
}


inline int UserKey::operator!=( const UserKey & other ) const
{
  return ! operator==( other );
}


inline unsigned long UserKey::name() const
{
  return extract( true );
}


inline Boolean UserKey::isValidFor( const IString & userName ) const
{
  return ( _isValid && encodeName( userName ) == name() );
}


inline ExpirationDate UserKey::expires() const
{
  return ExpirationDate( extract( false ) );
}



