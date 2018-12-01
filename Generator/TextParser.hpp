/***************************************************************************
 * File...... TextParser.hpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Simple text parser takes a stream of text and parses it into Gins for the
 * generator.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef TEXTPARSER_HPP
#define TEXTPARSER_HPP

// Standard C++
class istream;
class ifstream;

// Generator
#include "Parser.hpp"


class _Export TextParser: public Parser
{
  public:
    // constructor
    TextParser( istream & input );
    TextParser( const IString & filename );
    virtual ~TextParser();

    // from Parser
    virtual void generate( Generator & gen );

  private:
    void close();

    ifstream * _file;
    istream &  _input;
};


#endif

