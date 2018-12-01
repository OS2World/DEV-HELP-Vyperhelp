/***************************************************************************
 * File...... StringPair.hpp
 * Author.... Mat
 * Date...... 10/29/98
 *
 * A pair of strings, used in collections.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef STRINGPAIR_HPP
#define STRINGPAIR_HPP

// OpenClass
#include <istring.hpp>      // IString


class StringPair
{
  public:
    // constructor
    StringPair( const IString & key, const IString & other );

    // accessors
    const IString & key() const;
    const IString & other() const;

  private:
    IString _key;
    IString _other;
};


// key function
const IString & key( const StringPair & pair );


// inline functions
#include "StringPair.ipp"

#endif

