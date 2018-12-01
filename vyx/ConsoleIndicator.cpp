/***************************************************************************
 * File...... ConsoleIndicator.cpp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * Implementation for ConsoleIndicator
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <stdio.h>
#include <io.h>

// Standard C++
#include <iostream.h>

// Generator
#include "MekTek.hpp"
#include "ConsoleIndicator.hpp"


ConsoleIndicator::ConsoleIndicator()
{
  // determine if console output is being re-directed
  _isConsole = isatty( fileno( stdout ) );

  outputPercent();
}

ConsoleIndicator::~ConsoleIndicator()
{
  // overwrite the last percentage indicator
  if ( _isConsole )
    cout << IString("    ") << endl;
}


void ConsoleIndicator::setTask( const IString & task )
{
  outputLine( IString::upperCase( task ) );
}


void ConsoleIndicator::setStatus( const IString & status )
{
  outputLine( status );
}


void ConsoleIndicator::warning( const IString & text )
{
  outputLine( IString("> ") + text );
}


void ConsoleIndicator::error( const IString & text )
{
  outputLine( IString(">> ") + text );
}


void ConsoleIndicator::meterUpdate()
{
  outputPercent();
}


void ConsoleIndicator::outputLine( const IString & line )
{
  // make sure the line is at least 4 characters to overwrite the current %done
  IString extended( line );
  if ( extended.length() < 4 )
    extended.leftJustify( 4 );

  cout << extended << '\n';
  outputPercent();
}


void ConsoleIndicator::outputPercent()
{
  if ( _isConsole )
    cout << IString( percentDone() ).rightJustify( 3 ) << IString("%\r") << flush;
}




