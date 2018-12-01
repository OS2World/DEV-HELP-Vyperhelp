/***************************************************************************
 * File...... ArtSet.cpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Implementation of ArtSet.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include "ArtSet.hpp"

//
// TEMPLATE INSTANCE
//
#include <iavlkss.c>

#pragma define(IGAvlKeySortedSetCursor<ArtDef,IString,IKCOps<ArtDef,IString> >)
#pragma define(IAvlKeySortedSet<ArtDef,IString>)
#pragma define(IGAvlKSS<ArtDef,IString,IKCOps<ArtDef,IString> >)
#pragma define(IGAvlKeySortedSetNode<ArtDef,IString,IKCOps<ArtDef,IString> >)




ArtSet::ArtSet( INumber numberOfElements ):
    IKeySortedSet< ArtDef, IString >( numberOfElements )
{}


