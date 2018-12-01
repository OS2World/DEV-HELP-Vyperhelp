/***************************************************************************
 * File...... WindowSet.hpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Window set is a keyed set of WindowDefs.  The key field is the
 * "id" element of the WindowDef.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef WINDOWSET_HPP
#define WINDOWSET_HPP

// OpenClass
#include <iksset.h>

// Generator
#include "WindowDef.hpp"


class WindowSet: public IKeySortedSet< WindowDef, int >
{
  public:
    // constructor
    WindowSet( INumber numberOfElements = 10 );
};


#endif

