/***************************************************************************
 * File...... UserKey.cpp
 * Author.... Mat
 * Date...... 4/29/99
 *
 * Implementation for UserKey
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include <memory.h>
#include <ctype.h>

#include "UserKey.hpp"


// mask data to XOR key string
// NOTE: mask should change with a new release to invalidate old keys
static const unsigned long keyMask[2] = { 0xa9b8c7d6, 0xe5f40312 };


/***************************************************************************
 * Procedure.. UserKey::UserKey()
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Default constructor sets an empty user name and a registration date
 * 30 days into the future.
 ***************************************************************************/
UserKey::UserKey():
  _isValid( true )
{
  IDate demo;
  demo += 30;  // add 30 days to current date
  init( ExpirationDate( ENCODE_DATE( demo.year(), demo.monthOfYear(), demo.dayOfMonth() ) ) );
}


/***************************************************************************
 * Procedure.. UserKey::UserKey( const IString & key )
 * Author..... Mat
 * Date....... 4/29/99
 *
 * Takes a key string which was provided to the user.  The key string is
 * one that was generated with the key() function.  It should be a hex string
 * of 18 digits, where the first 16 digits are the masked data, and the
 * next two digits are a checksum byte, such that when all the unmasked data
 * bytes are XOR'd together along with the checksum byte, the result should
 * be 0.
 ***************************************************************************/
UserKey::UserKey( const IString & key ):
  _isValid( false )
{
  // check proper length
  if ( key.length() != 18 )
    return;

  // check for all hex digits
  if ( ! key.isHexDigits() )
    return;

  // load and unmask the data
  unsigned char checksum = 0;
  unsigned char * mask = (unsigned char *)keyMask;
  unsigned char * data = _data;
  for ( int i = 1; i < 16; i += 2 )
  {
    *data = (unsigned char)(key.subString( i, 2 ).x2d().asUnsigned()) ^ *mask;
    checksum ^= *data;
    data++;
    mask++;
  }

  // verify checksum
  checksum ^= (unsigned char)(key.subString( 17, 2 ).x2d().asUnsigned());
  if ( checksum )
    return;  // invalid key

  // valid key!
  _isValid = true;
}


/***************************************************************************
 * Procedure.. UserKey::UserKey( const IString & userName, const ExpirationDate & expiration )
 * Author..... Mat
 * Date....... 4/29/99
 *
 * This constructor creates a user key from a given user name and expiration.
 ***************************************************************************/
UserKey::UserKey( const IString & userName, const ExpirationDate & expiration ):
  _isValid( true )
{
  init( expiration, userName );
}


/***************************************************************************
 * Procedure.. UserKey::init
 * Author..... Mat
 * Date....... 4/30/99
 *
 * This function creates a user key from a given user name and expiration
 * date.  The two 32-bit values are combined by alternating bits into a
 * single 8 byte array.
 ***************************************************************************/
void UserKey::init( const ExpirationDate & expiration, const IString & userName )
{
  // clear data values
  memset( _data, 0, 8 );

  // loop and set bits
  unsigned long value1 = expiration.code();
  unsigned long value2 = encodeName( userName );
  unsigned long bit = 1;
  int i = 0;
  unsigned char dataBit = 1;
  while ( i < 8 )
  {
    // bits from value1
    if ( value1 & bit )
      _data[i] |= dataBit;
    dataBit <<= 1;

    // bits from value2
    if ( value2 & bit )
      _data[i] |= dataBit;
    dataBit <<= 1;

    bit <<= 1;

    if ( ! dataBit )
    {
      i++;
      dataBit = 1;
    }
  }
}


/***************************************************************************
 * Procedure.. UserKey::operator==
 * Author..... Mat
 * Date....... 5/4/99
 *
 * Returns true if both keys are valid and the key data is the same.
 ***************************************************************************/
int UserKey::operator==( const UserKey & other ) const
{
  return ( _isValid && other._isValid && ! memcmp( _data, other._data, sizeof(_data) ) );
}


/***************************************************************************
 * Procedure.. UserKey::extract
 * Author..... Mat
 * Date....... 5/4/99
 *
 * Extract a 32-bit encoded quantity from the key data.  The data contains
 * two 32-bit numbers, encoded by alternating bits.  If isOffset is true, then
 * the number starting with bit 1 is returned, otherwise the number starting
 * with bit 0 is returned.
 ***************************************************************************/
unsigned long UserKey::extract( Boolean isOffset ) const
{
  unsigned long result = 0;
  unsigned long bit = 1;
  int i = 0;
  unsigned char dataBit = isOffset? 2: 1;
  while ( i < 8 )
  {
    if ( _data[i] & dataBit )
    {
      result |= bit;  // set this bit in the result
    }
    bit <<= 1;
    dataBit <<= 2;
    if ( ! dataBit )
    {
      i++;
      dataBit = isOffset? 2: 1;
    }
  }

  return result;
}


/***************************************************************************
 * Procedure.. UserKey::encodeName
 * Author..... Mat
 * Date....... 5/4/99
 *
 * Hashes a user name string into an encoded 32-bit number.  The number is
 * obtained by XORing any alphabetic characters in the name into the
 * 32-bit number.
 ***************************************************************************/
unsigned long UserKey::encodeName( const IString & userName ) const
{
  // default name :-)
  char result[5] = "Ruby";

  // XOR in any alphabetic character found
  int n = 0;
  for ( int i = 1; i <= userName.length(); i++ )
  {
    char ch = userName[i];
    if ( isalpha(ch) )
      result[n++ % 4] ^= ch;
  }

  // return as an unsigned long
  return *(unsigned long *)result;
}


/***************************************************************************
 * Procedure.. UserKey::key
 * Author..... Mat
 * Date....... 5/4/99
 *
 * Returns the hex string representing the key. This is the string that the
 * user enters in the registration dialog, and it is the string that gets
 * stored in the INI file.  The string consists of 16-hex digits (the key
 * data with an XOR mask applied), followed by a 2 digit hex checksum.
 ***************************************************************************/
IString UserKey::key() const
{
  IString result;

  unsigned char checksum = 0;
  unsigned char * mask = (unsigned char *)keyMask;
  const unsigned char * data = _data;
  for ( int i = 0; i < 8; i++ )
  {
    result += IString( (int)(*data ^ *mask) ).d2x();
    checksum ^= *data;
    data++;
    mask++;
  }

  // add checksum byte
  result += IString( (int)checksum ).d2x();

  return result;
}


