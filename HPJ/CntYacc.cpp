/***************************************************************************
 * File...... CntYacc.cpp
 * Author.... Mat
 * Date...... 1/8/99
 *
 * Implementation for CNT parser.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C++
#include <iostream.h>

// MekTek
#include "MekTek.hpp"

// Generator
#include "Generator.hpp"

// WinHelpParser
#include "WinHelpParser.hpp"
#include "CntYacc.hpp"

#define ALexClass  CntLexUnused   // change default names, avoid multiple definitions
#define AYaccClass CntYaccUnused  // these are unused anyways
#include "cnt.yhh"       // generated from rule file



/***************************************************************************
 * Procedure.. CntYacc::CntYacc()
 * Author..... Mat
 *
 * CntYacc constructor loads Lex and Yacc tables and sets file to parse.
 ***************************************************************************/
CntYacc::CntYacc( const char* fileToParse, Boolean isPrimary ) :
    EnhancedYacc( "cnt", fileToParse ),
    _filename( fileToParse ),
    _isPrimary( isPrimary )
{}


/***************************************************************************
 * Procedure.. CntYacc::error
 * Author..... Mat
 *
 * Prints an error if a message occurs
 ***************************************************************************/
SSBooleanValue CntYacc::error( SSUnsigned32 qulState, SSLexLexeme & qLexeme)
{
  // print warning
  warning( "Parsing error", &qLexeme );

  // standard error recovery
  return errorToken();
}


/***************************************************************************
 * Procedure.. CntYacc::reduce()
 * Author..... Mat
 *
 * The reduce function is called for every production in the Yacc grammar.
 * The productions are defined in the .yhh created by the VP compiler.
 * When the rule file is changed, new cases should be added here from the
 * examples generated in the .yhh file.
 ***************************************************************************/
SSYaccStackElement* CntYacc::reduce( SSUnsigned32 production, SSUnsigned32 size)
{
  switch ( production )
  {
    case AYaccUnknown:
    // line -> Unknown
       warning( "Unknown line" );
       break;

    case AYaccInclude:
    // line -> Include Filename
       {
         // parse nested file
         Filename includeFilename = lexemeAsString( 1 );
         includeFilename.locateOnPath( _filename.dir() );
         CntYacc cnt( includeFilename, false );
         _whp->setActiveFile( includeFilename );
         cnt.parseTo( *_gen, *_whp );
         _whp->setActiveFile( _filename );
       }
       break;

    case AYaccTitle:
    // line -> Title Caption
       if ( _isPrimary )
         _whp->setTitle( lexemeAsString( 1 ).strip() );
       break;

    case AYaccSection:
    // line -> Section texts contents
       handleSection( lexemeAsULong( 0 ) );
       break;

    case AYaccTextLiteral:
    // text -> Literal
       // literal is in the form "\x" where x is the character to output
       MEKASSERT( lexemeAsString( 0 ).length() == 2 );
       _sectionTitle += lexemeAsString( 0 )[2];
       break;

    case AYaccText:
    // text -> Text
       _sectionTitle += lexemeAsString( 0 );
       break;

    case AYaccContents:
    // contents -> Equal Content
       _sectionTopic = lexemeAsString( 1 );
       break;
  }

  return stackElement();
}



/***************************************************************************
 * Procedure.. CntYacc::handleSection
 * Author..... Mat
 * Date....... 1/8/99
 *
 * Handle recognition of a section
 ***************************************************************************/
void CntYacc::handleSection( SSUnsigned32 level )
{
  // send the section
  _whp->sendSection( level, _sectionTitle.strip(), _sectionTopic.strip() );

  // update indicator
  if ( _whp->indicator() )
    _whp->indicator()->setCurrent( offset() );

  // reset strings
  _sectionTitle = _sectionTopic = IString();
}



/***************************************************************************
 * Procedure.. CntYacc::parseTo
 * Author..... Mat
 *
 * Sets the target Generator and WinHelpParser, then begins the parsing.
 * Returns true if an error occured, as per SSYacc::parse().
 ***************************************************************************/
SSBooleanValue CntYacc::parseTo( Generator & gen, WinHelpParser & whp )
{
  _gen = &gen;
  _whp = &whp;
  return parse();
}


/***************************************************************************
 * Procedure.. CntYacc::warning
 * Author..... Mat
 *
 * Log a warning condition.
 ***************************************************************************/
void CntYacc::warning( const IString & text, const SSLexLexeme * lexeme )
{
  unsigned line = 0;
  unsigned col = 0;

  // get current location, if possible
  if ( ! lexeme )
  {
    SSYaccStackElement * element = elementFromStack( 0 );
    MEKASSERT( element );
    lexeme = element->lexeme();
  }

  // determine line and column
  if ( lexeme )
  {
    line = lexeme->line();
    col = lexeme->offset();
  }

  _whp->warning( text, line, col );
}

