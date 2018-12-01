/***************************************************************************
 * File...... HeaderGenerator.cpp
 * Author.... Mat
 *
 * HeaderGenerator implementation
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Standard C
#include <ctype.h>

// OpenClass
#include <iexcept.hpp>      // ICLibErrorInfo

// Generator
#include "HeaderGenerator.hpp"


//
// ---------- constructor ----------
//

HeaderGenerator::HeaderGenerator( const IString & filename ) :
    _output( filename )
{
  _output << "/* " << filename << " */\n";
  _output << "/* " << credits() << " */\n" << endl;
  check();
}


void HeaderGenerator::finish()
{
  check();
  _output.close();
}


// throw an exception if a file error is detected
void HeaderGenerator::check()
{
  // use "void *" operator to check error state
  if ( ! _output )
    ICLibErrorInfo::throwCLibError( "ofstream", IEXCEPTION_LOCATION() );
}


// if label name is a valid C constant, create a label
void HeaderGenerator::handleLabel( const LabelGin        & label )
{
  // if ID is 0, skip it
  if ( ! label.id() )
    return;

  // test all characters must by alphanumeric or "_"
  //      first character cannot be numeric
  char * buf = label.name();
  Boolean isFirst = true;
  while ( *buf )
  {
    if ( ! isalnum(*buf) && ( *buf != '_' ) )
      return;
    else if ( isFirst && isdigit(*buf) )
      return;
    buf++;
    isFirst = false;
  }

  // send a #define
  _output << "#define " << label.name() << " " << label.id() << endl;
  check();
}

