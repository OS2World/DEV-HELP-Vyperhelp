/***************************************************************************
 * File...... ParserException.cpp
 * Author.... Mat
 * Date...... 5/12/98
 *
 * Implementation of ParserException
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Generator
#include "ParserException.hpp"


ParserException::ParserException( const SSException & ss ):
  IException( ss.text(), ss.errorId() )
{
  setErrorCodeGroup( "Parser" );
}


const char * ParserException::name() const
{
  return "ParserException";
}

