/***************************************************************************
 * File...... UserKey.hpp
 * Author.... Mat
 * Date...... 4/29/99
 *
 * Hold a user registration key, which encodes a user name and an expiration
 * date.  The key consists of 8 bytes of data, alternating bits for an
 * ExpirationDate (unsigned long), and an unsigned long encoding of the user
 * name.  The data is also XORd to mask it.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef USERKEY_HPP
#define USERKEY_HPP


#include "ExpirationDate.hpp"

class _Export UserKey
{
  public:
    // constructors
    UserKey();
    UserKey( const IString & key );
    UserKey( const IString & userName, const ExpirationDate & expiration );

    // equality
    int operator==( const UserKey & other ) const;
    int operator!=( const UserKey & other ) const;

    // check validity and expiration
    Boolean        isValid() const;
    Boolean        isValidFor( const IString & userName ) const;
    ExpirationDate expires() const;

    // hex string to give to user
    IString        key() const;

  private:
    void          init( const ExpirationDate & expiration, const IString & userName = IString() );
    unsigned long encodeName( const IString & userName ) const;
    unsigned long name() const;
    unsigned long extract( Boolean isOffset ) const;

    Boolean       _isValid;
    unsigned char _data[8];
};


// inline functions
#include "UserKey.ipp"


#endif

