/***************************************************************************
 * File...... EnhancedYacc.hpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * This subclass of SSYacc adds some common helper functions
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef ENHANCED_YACC
#define ENHANCED_YACC

// OpenClass
#include <istring.hpp>      // IString

// Visual Parse++
#include <ssyacc.hpp>
#include <sslex.hpp>



/***************************************************************************
 * Class...... EnhancedYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Adds helper functions to SSYacc for use during the reduce() function.
 ***************************************************************************/
class EnhancedYacc : public SSYacc
{
  public:
    // constructors
    SSConstr EnhancedYacc( const IString & tableName, const IString & consumer );
    SSConstr EnhancedYacc( const IString & tableName, SSLexConsumer & consumer );
    virtual ~EnhancedYacc();

    // set consumer file
    void setConsumerFile( const IString & consumer );

    // query file size and current offset (only valid for file consumers)
    unsigned size();
    unsigned offset();

  protected:
    // parsing helper functions
    SSUnsigned32 lexemeAsULong( SSSigned32 n, Boolean forceDecimal = false );
    IString      lexemeAsString( SSSigned32 n );

  private:
    void setTables( const IString & tableName );

    SSLex               _lex;
    SSLexFileConsumer * _consumer;
};


#endif

