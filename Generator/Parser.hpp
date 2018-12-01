/***************************************************************************
 * File...... Parser.hpp
 * Author.... Mat
 *
 * Abstract Parser supports generation to a specified Generator.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PARSER_HPP
#define PARSER_HPP

// OpenClass
class IString;

// Generator
class Generator;
class ParserIndicator;


class _Export Parser
{
  public:
    // constructor
    Parser();
    virtual ~Parser();

    // generator
    virtual void generate( Generator & gen ) = 0;

    // for CDefineYacc (header file parser)
    virtual void setId( const IString & contextName, int id );

    // indicator support
    void setIndicator( ParserIndicator * indicator );
    ParserIndicator * indicator();

  private:
    ParserIndicator * _indicator;
};


// inline functions
#include "Parser.ipp"


#endif

