/***************************************************************************
 * File...... ArtSet.hpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Art set is a keyed set of ArtDefs.  The key field is the
 * "id" element of the ArtDef.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef ARTSET_HPP
#define ARTSET_HPP

// OpenClass
#include <iksset.h>

// Generator
#include "ArtDef.hpp"


class _Export ArtSet: public IKeySortedSet< ArtDef, IString >
{
  public:
    // constructor
    ArtSet( INumber numberOfElements = 100 );
};


#endif

