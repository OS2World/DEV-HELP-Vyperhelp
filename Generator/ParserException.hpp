/***************************************************************************
 * File...... ParserException.hpp
 * Author.... Mat
 * Date...... 5/12/98
 *
 * Maps a VisualParse++ "SSException" to an OpenClass "IException".
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef PARSEREXCEPTION_HPP
#define PARSEREXCEPTION_HPP

// VisualParse++
#include <ssexcept.hpp>

// OpenClass
#include <iexcept.hpp>


class ParserException: public IException
{
  public:
    // constructor
    ParserException( const SSException & ss );

    // identification
    virtual const char * name() const;
};


#endif

