/***************************************************************************
 * File...... BooleanString.hpp
 * Author.... Mat
 * Date...... 3/17/98
 *
 * A string which describes the value of a Boolean switch: "yes" or "no"
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef BOOLEANSTRING_HPP
#define BOOLEANSTRING_HPP

// OpenClass
#include <istring.hpp>

class BooleanString: public IString
{
  public:
    // constructor
    BooleanString( Boolean yes );
};

#endif

