/***************************************************************************
 * File...... ParserIndicator.hpp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * A virtual class definition for displaying status and % done of a Parser.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef PARSERINDICATOR_HPP
#define PARSERINDICATOR_HPP


// OpenClass
#include <istring.hpp>      // IString


class _Export ParserIndicator
{
  public:
    // constructor
    ParserIndicator();
    virtual ~ParserIndicator();

    // set a total and current amount to completion
    //   (units can be whatever, they are mapped to % for display)
    //   default total is 100, initial current is 0
    void setTotal( unsigned total );
    void setCurrent( unsigned current );

    // set the name of the task being performed, default ignores
    virtual void setTask( const IString & task );

    // set a status string (current filename, etc), default ignores
    virtual void setStatus( const IString & status );

    // add a warning or error to the log output, default ignores
    virtual void warning( const IString & text );
    virtual void error( const IString & text );

  protected:
    // called when the % done changes
    virtual void meterUpdate() = 0;
    unsigned percentDone() const;

  private:
    void updatePercent();

    unsigned _total;
    unsigned _current;
    unsigned _percent;
};


// inline functions
#include "ParserIndicator.ipp"


#endif

