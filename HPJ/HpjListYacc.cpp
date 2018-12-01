/***************************************************************************
 * File...... HpjListYacc.cpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * Implementation for the HpjListYacc parser, which adds to a filename
 * list.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "WinHelpParser.hpp"

#include "HpjListYacc.hpp"

#define ALexClass  HpjListLexUnused   // change default names, avoid multiple definitions
#define AYaccClass HpjListYaccUnused  // these are unused anyways
#include "hpjlist.yhh"      // generated from rule file


/***************************************************************************
 * Procedure.. HpjListYacc::HpjListYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Constructor loads Lex and Yacc tables and links them.
 ***************************************************************************/
HpjListYacc::HpjListYacc( const char * fileToParse ) :
    EnhancedYacc( "hpjlist", fileToParse )
{}


/***************************************************************************
 * Procedure.. HpjListYacc::reduce
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Virtual function which is called for every production rule which is
 * recognized in the file being parsed.  The productions are defined
 * in the .ycc file.  This file is re-generated every time the rule
 * file is compiled.
 ***************************************************************************/
SSYaccStackElement* HpjListYacc::reduce( SSUnsigned32 production, SSUnsigned32 size)
{
  if ( production == AYaccLineFile ) {
    // lineOpt -> Filename
    _whp->addRtfFile( lexemeAsString( 0 ) );
  }

  return stackElement();
}


/***************************************************************************
 * Procedure.. HpjListYacc::parseTo
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Sets the target WinHelpParser, then begins the parsing.  The WinHelpParser
 * is used to save the IDs that are parsed form the #define statements.
 * Returns true if an error occured, as per SSYacc::parse().
 ***************************************************************************/
SSBooleanValue HpjListYacc::parseTo( WinHelpParser & whp )
{
  _whp = &whp;
  return parse();
}


