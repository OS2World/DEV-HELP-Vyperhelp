/***************************************************************************
 * File...... VyDocParser.hpp
 * Author.... Mat
 * Date...... 5/20/99
 *
 * Parser interface for saving or exporting the current VyDoc contents.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef VYDOCPARSER_HPP
#define VYDOCPARSER_HPP


// Generator
#include "Parser.hpp"

// Vyper
class VyDoc;


class VyDocParser: public Parser
{
  public:
    // constructor
    VyDocParser( VyDoc & doc, Boolean isSave, const IString & filename );

    // from Parser
    virtual void generate( Generator & gen );

  private:
    VyDoc & _doc;
    Boolean _isSave;
    IString _filename;
};


#endif

