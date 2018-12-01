/***************************************************************************
 * File...... TextParser.cpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Implementation of TextParser
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Standard C++
#include <iostream.h>
#include <fstream.h>

// OpenClass
#include <istring.hpp>      // IString
#include <iexcept.hpp>      // ICLibErrorInfo

// Generator
#include "Generator.hpp"
#include "ParserIndicator.hpp"
#include "TextParser.hpp"


TextParser::TextParser( istream & input ):
    _file( 0 ),
    _input( input )
{}


TextParser::TextParser( const IString & filename ):
    _file( new ifstream( filename ) ),
    _input( *_file )
{}


TextParser::~TextParser()
{
  close();
}


void TextParser::close()
{
  if ( _file )
  {
    _file->close();
    delete _file;
    _file = 0;
  }
}


void TextParser::generate( Generator & gen )
{
  // set total size
  if ( indicator() )
  {
    _input.seekg( 0, ios::end );
    indicator()->setTotal( _input.tellg() );
    _input.seekg( 0, ios::beg );
  }

  // create a section to hold all text
  gen << SectionGin( 1, true );

  // set line spacing so that a paragraph gets an extra line
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  int next = _input.peek();
  while ( ( next != EOF ) && _input )
  {
    // send text portion of next line
    gen << IString::lineFrom( _input );

    // check for newline (\n) or paragraph (\n\n)
    next = _input.peek();
    if ( next == '\n' )
    {
      _input.ignore();
      next = _input.peek();
      gen << ParaGin();
    }
    else
    {
      gen << LineGin();
    }

    // update percent done
    if ( indicator() )
      indicator()->setCurrent( _input.tellg() );
  }

  // close the generator
  gen.finish();

  // close the file if opened by TextParser
  close();

  // throw an exception if there is a file error
  if ( ! _input )
    ICLibErrorInfo::throwCLibError( "istream", IEXCEPTION_LOCATION() );
}


