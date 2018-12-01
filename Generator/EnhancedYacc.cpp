/***************************************************************************
 * File...... EnhancedYacc.cpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Implements EnhancedYacc, which adds helper functions to SSYacc.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// Standard C
#include <stdlib.h>
#include <stdio.h>

// Generator
#include "MekTek.hpp"
#include "Filename.hpp"
#include "EnhancedYacc.hpp"


/***************************************************************************
 * Procedure.. EnhancedYacc::EnhancedYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * The tableName provided is the base part of the filename only.  An LLR
 * extension is used to get the YaccTable, and a DFA extension is used to
 * get the LexTable.  The environment variable TABPATH is used to search
 * for the table files.
 *
 ***************************************************************************/
EnhancedYacc::EnhancedYacc( const IString & tableName, const IString & consumer ):
  _consumer( 0 )
{
  // set tables
  setTables( tableName );

  // set consumer, if provided
  if ( consumer.length() )
  {
    _consumer = new SSLexFileConsumer( consumer );
    _lex.setConsumer( *_consumer );
  }
}


/***************************************************************************
 * Procedure.. EnhancedYacc::EnhancedYacc
 * Author..... Mat
 *
 * The tableName provided is the base part of the filename only.  An LLR
 * extension is used to get the YaccTable, and a DFA extension is used to
 * get the LexTable.  The environment variable TABPATH is used to search
 * for the table files.
 *
 * In this case, the consumer is not a file, so _consumer is set to 0.
 *
 ***************************************************************************/
EnhancedYacc::EnhancedYacc( const IString & tableName, SSLexConsumer & consumer ):
  _consumer( 0 )
{
  // set tables
  setTables( tableName );

  // set consumer
  _lex.setConsumer( consumer );
}


EnhancedYacc::~EnhancedYacc()
{
  if ( _consumer )
    delete _consumer;
}


/***************************************************************************
 * Procedure.. EnhancedYacc::setConsumerFile
 * Author..... Mat
 * Date....... 5/11/99
 *
 * Set a new file consumer.  This function wraps SSLex::setConsumer, because
 * we need to keep the SSLexFileConsumer around so we can query file size
 * and offset.
 ***************************************************************************/
void EnhancedYacc::setConsumerFile( const IString & consumer )
{
  // delete previous consumer if one exists
  if ( _consumer )
  {
    delete _consumer;
    _consumer = 0;
  }

  // set new consumer file
  if ( consumer.length() )
  {
    _consumer = new SSLexFileConsumer( consumer );
    _lex.setConsumer( *_consumer );
  }
}


/***************************************************************************
 * Procedure.. EnhancedYacc::size
 * Author..... Mat
 * Date....... 5/11/99
 *
 * Returns the size of the file being consumed.  Assumes that a file consumer
 * is active, not some other consumer.
 ***************************************************************************/
unsigned EnhancedYacc::size()
{
  MEKASSERT( _consumer );

  // assumes that the buffer length is the same as the file length, which is true
  //   since the default SSLexFileConsumer constructor is used.
  return _consumer->bufferLength();
}


/***************************************************************************
 * Procedure.. EnhancedYacc::offset
 * Author..... Mat
 * Date....... 5/11/99
 *
 * Return the current offset of the file.
 ***************************************************************************/
unsigned EnhancedYacc::offset()
{
  MEKASSERT( _consumer );

  // assumes the entire file will fit in a single buffer, which is true
  //   since the default SSLexFileConsumer constructor is used.
  return _consumer->lexemeBuffer() - _consumer->buffer();
}



/***************************************************************************
 * Procedure.. EnhancedYacc::setTables
 * Author..... Mat
 * Date....... 10/20/98
 *
 * Locate the LLR and DFA tables in the current directory or on the TABPATH.
 ***************************************************************************/
void EnhancedYacc::setTables( const IString & tableName )
{
  // locate yacc table
  Filename yaccTable = tableName + ".llr";
  yaccTable.locateOnStandardPath( Filename::tabpath, true );
  setTable( yaccTable );

  // locate lex table
  Filename lexTable = tableName + ".dfa";
  lexTable.locateOnStandardPath( Filename::tabpath, true );
  _lex.setTable( lexTable );
  setLex( _lex );
}


/***************************************************************************
 * Procedure.. EnhancedYacc::lexemeAsULong
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Get the nth lexeme from a reduction and convert it to an unsigned long.
 * If forceDecimal is false, then octal and hex are also supported with normal
 * C notation:
 *   "0" prefix specified octal
 *   "0x" prefix specifies hex
 ***************************************************************************/
SSUnsigned32 EnhancedYacc::lexemeAsULong( SSSigned32 n, Boolean forceDecimal )
{
  SSYaccStackElement *element = elementFromProduction( n );
  IASSERTSTATE( element != NULL );
  SSLexLexeme *lexeme = element->lexeme();
  IASSERTSTATE( lexeme != NULL );
  if ( forceDecimal )
  {
    SSBooleanValue isOverflow;
    return lexeme->asUnsigned32( isOverflow );
  }
  else
  {
    // use strtoul instead of lexeme->asUnsigned32() for support of hex and octal
    return strtoul( lexeme->asChar(), NULL, 0 );
  }
}


/***************************************************************************
 * Procedure.. EnhancedYacc::lexemeAsString
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Get the nth lexeme from a reduction and convert it to a string.
 ***************************************************************************/
IString EnhancedYacc::lexemeAsString( SSSigned32 n )
{
  SSYaccStackElement *element = elementFromProduction( n );
  IASSERTSTATE( element != NULL );
  SSLexLexeme *lexeme = element->lexeme();
  IASSERTSTATE( lexeme != NULL );
  return lexeme->asChar();
}


