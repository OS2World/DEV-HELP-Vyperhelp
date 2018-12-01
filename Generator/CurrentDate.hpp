/***************************************************************************
 * File...... CurrentDate.hpp
 * Author.... Mat
 * Date...... 3/30/99
 *
 * Current date derived from the system clock, but verified from some file
 * timestamps, to try to avoid users simply setting their clock back
 * to get around the timeout.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef CURRENTDATE_HPP
#define CURRENTDATE_HPP

// OpenClass
#include <idate.hpp>        // IDate


class CurrentDate: public IDate
{
  public:
    // constructor
    CurrentDate();

    void validate( const IString & filename );
};


#endif

