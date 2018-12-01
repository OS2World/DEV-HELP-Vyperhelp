/***************************************************************************
 * File...... HeaderGenerator.hpp
 * Author.... Mat
 *
 * HeaderGenerator is a generator which will create a C header file with
 * #define for each C-compatible label in the document
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

#ifndef HEADERGENERATOR_HPP
#define HEADERGENERATOR_HPP

// Standard C++
#include <fstream.h>

// Generator
#include "Generator.hpp"


class _Export HeaderGenerator: public Generator
{
  public:
    // constructor
    HeaderGenerator( const IString & filename );

    // Gin handlers
    virtual void handleLabel( const LabelGin & );
    virtual void finish();

  private:
    void check();

    ofstream _output;
};


#endif

