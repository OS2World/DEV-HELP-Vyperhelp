/***************************************************************************
 * File...... Main.cpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Main program which converts WinHelp projects to Generator (IPF) output.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// Standard C
#include <stdlib.h>
#include <locale.h>
#include <malloc.h>

// iostreams
#include <fstream.h>

// OpenClass
#include <iexcept.hpp>

// Generator
#include "HtmlGenerator.hpp"
#include "TextParser.hpp"
#include "LoadParser.hpp"
#include "WinHelpParser.hpp"
#include "IpfParser.hpp"
#include "TraceGenerator.hpp"
#include "TextGenerator.hpp"
#include "IPFGenerator.hpp"
#include "SaveGenerator.hpp"
#include "HeaderGenerator.hpp"
#include "RtfGenerator.hpp"
#include "HpjGenerator.hpp"

// VYX
#include "ConsoleIndicator.hpp"

// version and copyright
#ifndef VVERSION
  #define VVERSION ""
#endif
#ifndef VCOPYRIGHT
  #define VCOPYRIGHT ""
#endif

// const strings
const char * nameText    = " (Text)";
const char * nameWinHelp = " (WinHelp)";
const char * nameVyper   = " (VyperHelp)";
const char * nameUnknown = " (Not recognized!)";
const char * nameIPF     = " (IPF)";
const char * nameHTM     = " (HTML Single)";
const char * nameHTML    = " (HTML Frames)";
const char * nameHHP     = " (HTMLHelp)";
const char * nameRtf     = " (RTF)";
const char * nameTrace   = " (VyperHelp Trace)";
const char * nameHeader  = " (C Header)";

// local functions
static void showHelp();


int main( int argc, char **argv )
{
  int         result = 0;
  Parser *    parser = 0;
  Generator * gen = 0;

  // setup for localization
  char * locale = getenv( "VLOCALE" );
  if ( locale )
    setlocale( LC_ALL, locale );

  // banner
  cout << "VyperHelp Converter " << VVERSION;
  if ( IString(VVERSION) != Generator::version() )
    cout << " (" << Generator::version() << ")";
  cout << "\n" << VCOPYRIGHT << "\n";

  // registration & expiration
  cout << Generator::registration().notice() << '\n';
  ExpirationDate expires = Generator::registration().key().expires();
  if ( (int)expires )
    cout << expires.notice() << "\n";

  cout << endl;

  // check arguments
  if ( argc < 3 )
  {
    showHelp();
    return 1;
  }

  ConsoleIndicator indicator;

  try
  {
    Filename infile( argv[1] );
    Filename outfile( argv[2] );
    IString description;

    // check for special '=' in outfile
    if ( outfile[1] == '=' )
      outfile = infile.fullBase() + outfile.subString( 2 );

    // open input file
    IString intype = infile.extension();
    intype.upperCase();
    description = IString("Input:  ") + infile;

    if ( intype == ".TXT" )
    {
      description += nameText;
      parser = new TextParser( infile );
    }
    else if ( intype == ".HPJ" )
    {
      description += nameWinHelp;
      parser = new WinHelpParser( infile );
    }
    else if ( intype == ".VYD" )
    {
      description += nameVyper;
      parser = new LoadParser( infile );
    }
    else if ( intype == ".IPF" )
    {
      description += nameIPF;
      parser = new IpfParser( infile );
    }
    else
    {
      description += nameUnknown;
    }
    indicator.setStatus( description );

    // open output file
    IString outtype = outfile.extension();
    outtype.upperCase();
    description = IString("Output: ") + outfile;

    if ( outtype == ".TXT" )
    {
      description += nameText;
      gen = new TextGenerator( outfile );
    }
    else if ( outtype == ".IPF" )
    {
      description += nameIPF;
      gen = new IPFGenerator( outfile, 80, outfile.dir() );
    }
    else if ( outtype == ".HTM" )
    {
      description += nameHTM;
      gen = new HtmlGenerator( singleFile, outfile, outfile.dir() );
    }
    else if ( outtype == ".HTML" )
    {
      description += nameHTML;
      gen = new HtmlGenerator( multiFile, outfile, outfile.dir() );
    }
    else if ( outtype == ".HHP" )
    {
      description += nameHHP;
      gen = new HtmlGenerator( htmlHelp, outfile, outfile.dir() );
    }
    else if ( outtype == ".VYD" )
    {
      description += nameVyper;
      gen = new SaveGenerator( outfile );
    }
    else if ( outtype == ".VYT" )
    {
      description += nameTrace;
      gen = new TraceGenerator( outfile );
    }
    else if ( outtype == ".RTF" )
    {
      description += nameRtf;
      gen = new RtfGenerator( outfile );
    }
    else if ( outtype == ".HPJ" )
    {
      description += nameWinHelp;
      gen = new HpjGenerator( outfile );
    }
    else if ( outtype == ".H" )
    {
      description += nameHeader;
      gen = new HeaderGenerator( outfile );
    }
    else
    {
      description += nameUnknown;
    }
    indicator.setStatus( description );

    // do the conversion!
    if ( parser && gen )
    {
      gen->setInputFilename( infile );
      parser->setIndicator( &indicator );
      parser->generate( *gen );
    }
  }

  catch ( IException & except )
  {
    result = except.errorId();
    if ( ! result )
      result = -1;
    else
      indicator.error( IString("ERROR ") + IString(result) + IString(" (") + except.errorCodeGroup() + IString(')') );
    indicator.error( except.text() );
  }

  delete gen;
  delete parser;

  // memory debugging
  _dump_allocated( 0 );

  return result;
}



static void showHelp()
{
  cout << "Usage: VYX infile outfile\n"
          "  Converts between text formats, file type is determined by extension\n"
          "  infile types: TXT, IPF, HPJ, VYD\n"
          "  outfile types: TXT, IPF, HTM, HTML, HHP, HPJ, VYD, RTF, H, VYT\n"
          "  outfile may use '=' for same root as infile (e.g., =.ipf)\n";
}

