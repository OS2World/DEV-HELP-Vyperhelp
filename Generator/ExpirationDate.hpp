/***************************************************************************
 * File...... ExpirationDate.hpp
 * Author.... Mat
 * Date...... 7/7/98
 *
 * Handling and checking of expiration date for demo timeouts.  Expiration
 * date is set with a coded value, where
 *   code = ( year * 35 + day ) * 15 + month
 * code of 0 means "no expiration".
 *
 * Use inline operators and functions to make it tougher to hack.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef EXPIRATIONDATE_HPP
#define EXPIRATIONDATE_HPP


// OpenClass
#include <istring.hpp>      // IString
#include <idate.hpp>        // IDate


// encoding macro
#define ENCODE_DATE( year, month, day ) (((year) * 35 + (day) ) * 15 + (month))


class _Export ExpirationDate: public IDate
{
  public:
    // constructor
    ExpirationDate( unsigned long code = 0 );

    // expiration notice
    IString notice() const;

    // "int" operator used to check if expiration is set
    operator int() const;

    // comparison
    Boolean operator<( const IDate & date ) const;

    // code extraction
    unsigned long code() const;

  private:
    int invalid() const;

    unsigned long _code;
};


// inline functions
#include "ExpirationDate.ipp"


#endif

