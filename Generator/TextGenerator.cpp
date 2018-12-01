/***************************************************************************
 * File...... TextGenerator.cpp
 * Author.... Mat
 *
 * TextGenerator implementation.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Standard C++
#include <fstream.h>
#include <iomanip.h>

// Generator
#include "TextGenerator.hpp"


//
// ---------- constructor ----------
//

TextGenerator::TextGenerator( ostream & os, unsigned maxLineLength ):
    Generator( false ),
    _file( 0 ),
    _sectionCount( 0 )
{
  setOutput( os, maxLineLength );
}


TextGenerator::TextGenerator( const IString & filename, unsigned maxLineLength ):
    Generator( true ),
    _file( new ofstream( filename ) ),
    _sectionCount( 0 )
{
  setOutput( *_file, maxLineLength );
}


void TextGenerator::finish()
{
  closeOutput();
  if ( _file )
  {
    _file->close();
    delete _file;
    _file = 0;
  }
}


void TextGenerator::handleSection( const SectionGin & section )
{
  if ( _sectionCount++ )
  {
    // paragraph between sections
    outputLine( false );
    outputLine();
  }

  if ( section.title().length() )
  {
    // section title with paragraph
    outputText( section.title(), breakAfter );
    outputLine();
  }
}


void TextGenerator::handlePara( const ParaGin & para )
{
  outputLine( false );
  outputLine();
}


void TextGenerator::handleLine( const LineGin & line )
{
  outputLine();
}


void TextGenerator::handleText( const TextGin & text )
{
  outputText( text.string(), 0 );
}


void TextGenerator::handleSymbol( const SymbolGin & symbol )
{
  unsigned char c = symbol.symbol().asChar();
  if ( ! c )
    c = CodePage::substitution;
  outputText( c, 0 );
}


