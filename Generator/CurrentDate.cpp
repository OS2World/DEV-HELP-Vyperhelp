/***************************************************************************
 * File...... CurrentDate.cpp
 * Author.... Mat
 * Date...... 3/30/99
 *
 * Current date derived from the system clock OR more recent file timestamps.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// OpenClass
#include <iprofile.hpp>

// Generator
#include "MekTek.hpp"
#include "CurrentDate.hpp"


/***************************************************************************
 * Procedure.. CurrentDate::CurrentDate
 * Author..... Mat
 * Date....... 3/30/99
 *
 * Defaults to the current date from the system clock, but also checks
 * certain filenames for a more recent date.
 ***************************************************************************/
CurrentDate::CurrentDate()
{
  // NOTE: default date is "today" which is usually the current date!
  ITRACE_ALL( IString("Today is ") + asString() );

  // check the date of the system profiles, they should be pretty recent
  validate( IProfile::userProfile().name() );
  validate( IProfile::systemProfile().name() );

  ITRACE_DEVELOP( IString("Current date is ") + asString() );
}



/***************************************************************************
 * Procedure.. CurrentDate::validate
 * Author..... Mat
 * Date....... 3/30/99
 *
 * Check the given filename to see if it has a date stamp more recent then
 * the current date.  If so, set the current date to match.
 ***************************************************************************/
void CurrentDate::validate( const IString & filename )
{
  // check the date of the user profile, it should be pretty recent
  struct stat filestat;
  if ( ! stat( filename, &filestat ) )
  {
    // get date/time of last modification
    struct tm * filetime = localtime( &filestat.st_mtime );
    if ( filetime )
    {
      // if file's date is later, set this date
      IDate date( filetime->tm_year + 1900, filetime->tm_yday + 1 );
      ITRACE_DEVELOP( filename + IString(" dated ") + date.asString() );
      if ( date > *this )
        initialize( date.monthOfYear(), date.dayOfMonth(), date.year() );
    }
  }
}


