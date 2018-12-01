/***************************************************************************
 * File...... VyIndicator.cpp
 * Author.... Mat
 * Date...... 5/13/99
 *
 * Implementation for VyIndicator.  These methods simple pass the data on
 * to the associated dialog.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


#include "VyWait.hpp"
#include "VyIndicator.hpp"


VyIndicator::VyIndicator():
  _wait( 0 )
{}


void VyIndicator::setWait( VyWait * wait )
{
  _wait = wait;
  if ( _wait )
  {
    _wait->setIndicator( this );
    setTask( "VyperHelp" );
    setStatus( "Ready" );
    setCurrent( 0 );
    meterUpdate();
  }
}


void VyIndicator::start()
{
  _errorCount = 0;
  setCurrent( 0 );
  if ( _wait )
  {
    _wait->getOK()->enable( false );
    _wait->show();
    _wait->getOK()->setFocus();
  }
}


void VyIndicator::finish()
{
  if ( _wait )
  {
    // enable OK and wait for user if there were any errors
    if ( _errorCount )
      _wait->getOK()->enable();
    else
      _wait->close();
    _wait = 0;
  }
}


void VyIndicator::setTask( const IString & task )
{
  if ( _wait )
    _wait->getTask()->setText( task );
}


void VyIndicator::setStatus( const IString & status )
{
  if ( _wait )
    _wait->getStatus()->setText( status );
}


void VyIndicator::warning( const IString & text )
{
  if ( _wait )
  {
    _errorCount++;
    _wait->getErrorList()->addLineAsLast( text );
  }
}


void VyIndicator::error( const IString & text )
{
  if ( _wait )
  {
    _errorCount++;
    _wait->getErrorList()->addLineAsLast( IString(">> ") + text );
  }
}


void VyIndicator::meterUpdate()
{
  if ( _wait )
    _wait->getProgressIndicator()->moveArmToTick( percentDone() );
}


