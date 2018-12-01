/***************************************************************************
 * File...... CDefineYacc.cpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Implementation for CDefineYacc -- a C pre-processor which recognizes
 * #defines and #includes.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "Filename.hpp"
#include "Parser.hpp"

#include "CDefineYacc.hpp"

#define ALexClass  CDefineLexUnused   // change default names, avoid multiple definitions
#define AYaccClass CDefineYaccUnused  // these are unused anyways
#include "cdefine.yhh"      // generated from rule file


/***************************************************************************
 * Procedure.. CDefineYacc::CDefineYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Constructor loads Lex and Yacc tables and links them.
 ***************************************************************************/
CDefineYacc::CDefineYacc( const char * fileToParse, const IString & homeDir ) :
    EnhancedYacc( "cdefine", fileToParse ),
    _value( 0 ),
    _homeDir( homeDir )
{}


/***************************************************************************
 * Procedure.. CDefineYacc::reduce
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Virtual function which is called for every production rule which is
 * recognized in the file being parsed.  The productions are defined
 * in the .ycc file.  This file is re-generated every time the rule
 * file is compiled.
 ***************************************************************************/
SSYaccStackElement* CDefineYacc::reduce( SSUnsigned32 production, SSUnsigned32 size)
{
  switch ( production ) {

    case AYaccDefineInt:
    // linePre -> #define Token int
    {
       _parser->setId( lexemeAsString( 1 ), _value );
       break;
    }

    case AYaccInclude:
    // linePre -> #include < fileName >
    case AYaccIncludeQuote:
    // linePre -> #include begQuote fileName endQuote
    {
       Boolean found = false;
       Filename filename = lexemeAsString( 2 );

       if ( production == AYaccIncludeQuote )
       {
         // search home directory first only if "" delimiters used
         found = filename.locateOnPath( _homeDir );
       }
       if ( ! found )
       {
         // try the standard include path
         found = filename.locateOnStandardPath( Filename::include );
       }
       if ( ! found)
       {
         // not found on the path, check relative path
         filename.locateTo( _homeDir );
       }

       CDefineYacc cdef( filename, _homeDir );
       cdef.parseTo( *_parser );
       break;
    }

    case AYaccIntOct:
    // int -> OCTALconstant
    case AYaccIntDec:
    // int -> INTEGERconstant
    case AYaccIntHex:
    // int -> HEXconstant
       _value = strtoul( lexemeAsString( 0 ), NULL, 0 );
       break;

  }

  return stackElement();
}


/***************************************************************************
 * Procedure.. CDefineYacc::parseTo
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Sets the target Parser, then begins the parsing.  The Parser
 * is used to save the IDs that are parsed form the #define statements.
 * Returns true if an error occured, as per SSYacc::parse().
 ***************************************************************************/
SSBooleanValue CDefineYacc::parseTo( Parser & parser )
{
  _parser = &parser;
  return parse();
}


