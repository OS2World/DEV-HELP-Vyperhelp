/***************************************************************************
 * File...... VyIndicator.hpp
 * Author.... Mat
 * Date...... 5/13/99
 *
 * This class is derived from ParserIndicator, and used to manage status and
 * and percent done for the GUI Vyper program.  It works closely with
 * the VyWait part, which is the "wait" dialog.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef VYINDICATOR_HPP
#define VYINDICATOR_HPP


// Generator
#include "ParserIndicator.hpp"

// Vyper
class VyWait;


class VyIndicator: public ParserIndicator
{
  public:
    // constructor
    VyIndicator();

    // set associated window
    void setWait( VyWait * wait );

    // control window
    void start();
    void finish();

    // from ParserIndicator
    virtual void setTask( const IString & task );
    virtual void setStatus( const IString & status );
    virtual void warning( const IString & text );
    virtual void error( const IString & text );

  protected:
    // from ParserIndicator
    virtual void meterUpdate();

  private:
    VyWait * _wait;
    unsigned _errorCount;
};


#endif

