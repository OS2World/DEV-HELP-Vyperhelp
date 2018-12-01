/***************************************************************************
 * File...... GenTest.cpp
 *
 * Test Generator output
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

// Standard C
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <malloc.h>

// Standard C++
#include <fstream.h>

// OpenClass
#include <iexcept.hpp>

#include "HtmlGenerator.hpp"
#include "IPFGenerator.hpp"
#include "TraceGenerator.hpp"
#include "TextGenerator.hpp"
#include "SaveGenerator.hpp"
#include "RtfGenerator.hpp"
#include "HpjGenerator.hpp"


// EXTERNAL TEST ROUTINES
void TestSections( Generator & gen );
void TestKeywords( Generator & gen );
void TestMargins( Generator & gen );
void TestTables( Generator & gen );
void DefineWindows( Generator & gen );
void TestWindows( Generator & gen );
void TestChar( Generator & gen );
void TestPara( Generator & gen );
void TestSymbols( Generator & gen );
void DefineArt( Generator & gen );
void TestArt( Generator & gen );
void TestList( Generator & gen );
void TestNesting( Generator & gen );
void TestIPF( Generator & gen );
void TestHTML( Generator & gen );


Generator * createGenerator( char id, const IString & root )
{
  Generator * gen = 0;
  IString filename;

  switch ( id )
  {
    case 'H':
      filename = root + ".htm";
      gen = new HtmlGenerator(singleFile, filename);
      break;
    case 'L':
      filename = root + ".html";
      gen = new HtmlGenerator(multiFile, filename);
      break;
    case 'P':
      filename = root + ".hhp";
      gen = new HtmlGenerator(htmlHelp, filename);
      break;
    case 'I':
      filename = root + ".ipf";
      gen = new IPFGenerator( filename, 60 );
      break;
    case 'T':
      filename = root + ".vyt";
      gen = new TraceGenerator( filename );
      break;
    case 'X':
      filename = root + ".txt";
      gen = new TextGenerator( filename, 60 );
      break;
    case 'V':
      filename = root + ".vyd";
      gen = new SaveGenerator( filename );
      break;
    case 'R':
      filename = root + ".rtf";
      gen = new RtfGenerator( filename );
      break;
    case 'W':
      filename = root + ".hpj";
      gen = new HpjGenerator( filename );
      break;
  }

  if ( gen )
    cout << "Writing " << filename << endl;
  return gen;
}


void Test1( char id )
{
  try
  {
    Generator * gen = createGenerator( id, "test1" );
    if (! gen )
      return;

    // HEADER
    gen->setTitle( "Test Generator Document #1" )
       .setSubject( "Test document which was created by GenTest.cpp" )
       .setVersion( "1.04" )
       .setCopyright( "Copyright (c) 2000 MekTek" )
       .setAuthor( "Mat Kramer" )
       .setLanguage( Generator::english )
       .addKeyword( "test" )
       .addKeyword( "generator" );

    // DEFINITIONS FOR TESTS
    DefineWindows( *gen );
    DefineArt( *gen );

    // OUTPUT TESTS
    TestSections( *gen );
    TestKeywords( *gen );
    TestMargins( *gen );
    TestTables( *gen );
    TestWindows( *gen );
    TestChar( *gen );
    TestPara( *gen );
    TestArt( *gen );
    TestList( *gen );
    TestNesting( *gen );
    TestIPF( *gen );
    TestHTML( *gen );
    gen->finish();
    delete gen;
  }

  // catch OpenClass exceptions
  catch ( IException & except )
  {
    cerr << "\n>> " << except.text() << endl;
  }
}


void Test2( char id )
{
  try
  {
    Generator * gen = createGenerator( id, "test2" );
    if (! gen )
      return;
    gen->setCodePage( CodePage( CodePage::ansi ) );
    gen->setTitle( "Test Generator (ÄßÇ)" );  // text should look like "ABC" if translated correctly
    TestSymbols( *gen );
    gen->finish();
    delete gen;
  }

  // catch OpenClass exceptions
  catch ( IException & except )
  {
    cerr << "\n>> " << except.text() << endl;
  }
}


int main(int argc, char *argv[], char *envp[])
{
  int         result = 0;
  Generator * gen = 0;

  // setup for localization
  char * locale = getenv( "VLOCALE" );
  if ( locale )
    setlocale( LC_ALL, locale );

  // check which generator to use
  if ( argc < 2 )
  {
    cerr << "Usage: GENTEST h|l|p|i|x|v|t|r|w"
         << "\n\th: HTML single file"
         << "\n\tl: HTML multi file"
         << "\n\tp: HTMLHelp"
         << "\n\ti: IPF"
         << "\n\tx: text"
         << "\n\tv: vyper (VYD)"
         << "\n\tt: trace (VYT)"
         << "\n\tr: RTF"
         << "\n\tw: WinHelp (HPJ)\n";
    return 1;
  }
  char id = toupper( argv[1][0] );

  // create test files
  Test1( id );
  Test2( id );

  // memory debugging
  _dump_allocated( 0 );

  return 0;
}


