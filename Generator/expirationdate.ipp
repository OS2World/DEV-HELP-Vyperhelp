/***************************************************************************
 * File...... ExpirationDate.ipp
 * Author.... Mat
 * Date...... 7/7/98
 *
 * Inline functions for ExpirationDate.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


// returns non-zero if the date/code pair doesn't match
// always return 0 when _code is 0
inline int ExpirationDate::invalid() const
{
  return ( ENCODE_DATE( year(), monthOfYear(), dayOfMonth() ) - _code ) * _code;
}


// "int" operator returns non-zero if an expiration date is set
inline ExpirationDate::operator int() const
{
  return _code + invalid();
}


// return true if given date is beyond the expiration date
// use "int" operator to check if expiration is set
// use IDate to compare dates
inline Boolean ExpirationDate::operator<( const IDate & date ) const
{
  return IDate::operator<( date ) && operator int();
}


inline unsigned long ExpirationDate::code() const
{
  return _code;
}

