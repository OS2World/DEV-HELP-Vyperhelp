/***************************************************************************
 * File...... RtfColor.cpp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * Implementation for RtfColor
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include "RtfColor.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <iavlkss.c>
#include <ismkss.c>

// typedef ISortedMap <RtfColor, int> RtfColorTable;
#pragma define(IGAvlKeySortedSetNode<RtfColor,int,IEKCOps<RtfColor,int> >)
#pragma define(IGAvlKeySortedSetCursor<RtfColor,int,IEKCOps<RtfColor,int> >)
#pragma define(IGAvlKSS<RtfColor,int,IEKCOps<RtfColor,int> >)
#pragma define(IWSrtMapOnKSSetCursor<RtfColor,int,IEKCOps<RtfColor,int>,IGAvlKSS<RtfColor,int,IEKCOps<RtfColor,int> > >)
#pragma define(IWSMOnKSS<RtfColor,int,IEKCOps<RtfColor,int>,IGAvlKSS<RtfColor,int,IEKCOps<RtfColor,int> > >)



RtfColor::RtfColor(int cred, int cgreen, int cblue,int cnum) :
   _colorentry(cred,cgreen,cblue),
   _colornumberKey(cnum)
{
  _valid = ( ( cred != -1 ) && ( cgreen != -1 ) && ( cblue != -1 ) );
}


int RtfColor::operator==( const RtfColor & color ) const
{
  // TRUE if either (1) both are valid and colors match or (2) both are not valid
  return ( _valid && color._valid && ( _colorentry == color._colorentry )
      || ( ! _valid && ! color._valid ) );
}


int RtfColor::operator!=( const RtfColor & color ) const
{
  return ! operator==( color );
}

