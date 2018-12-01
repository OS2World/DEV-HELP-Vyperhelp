/***************************************************************************
 * File...... ConsoleIndicator.hpp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * Derived from ConsoleIndicator to show status and percent done to the
 * console.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef CONSOLEINDICATOR_HPP
#define CONSOLEINDICATOR_HPP


// Generator
#include "ParserIndicator.hpp"


class ConsoleIndicator: public ParserIndicator
{
  public:
    // constructor
    ConsoleIndicator();
    virtual ~ConsoleIndicator();

    // from ParserIndicator
    virtual void setTask( const IString & task );
    virtual void setStatus( const IString & status );
    virtual void warning( const IString & text );
    virtual void error( const IString & text );

  protected:
    // from ParserIndicator
    virtual void meterUpdate();

  private:
    void outputLine( const IString & line );
    void outputPercent();

    Boolean _isConsole;  // true if not redirected to a file
};


#endif

