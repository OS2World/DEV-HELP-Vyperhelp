/***************************************************************************
 * File...... TextGenerator.hpp
 * Author.... Mat
 *
 * Simple text generator.  Ignores almost everything except text and
 * newlines.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef TEXTGENERATOR_HPP
#define TEXTGENERATOR_HPP

// Standard C++
class ofstream;

// Generator
#include "Generator.hpp"


class _Export TextGenerator: public Generator
{
  public:
    // constructor
    TextGenerator( ostream & os, unsigned maxLineLength = 80 );
    TextGenerator( const IString & filename, unsigned maxLineLength = 80 );

    // from Generator
    virtual void handleSection( const SectionGin & );
    virtual void handlePara( const ParaGin & );
    virtual void handleLine( const LineGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );
    virtual void finish();

  private:
    ofstream * _file;
    unsigned   _sectionCount;
};


#endif

