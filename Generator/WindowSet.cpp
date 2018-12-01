/***************************************************************************
 * File...... WindowSet.cpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Implementation of WindowSet.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include "WindowSet.hpp"

//
// TEMPLATE INSTANCE
//
#include <iavlkss.c>

#pragma define(IGAvlKeySortedSetCursor<WindowDef,int,IKCOps<WindowDef,int> >)
#pragma define(IAvlKeySortedSet<WindowDef,int>)
#pragma define(IGAvlKSS<WindowDef,int,IKCOps<WindowDef,int> >)
#pragma define(IGAvlKeySortedSetNode<WindowDef,int,IKCOps<WindowDef,int> >)




WindowSet::WindowSet( INumber numberOfElements ):
    IKeySortedSet< WindowDef, int >( numberOfElements )
{}


