/***************************************************************************
 * File...... RtfColor.hpp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * RTF color table entry.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef RTFCOLOR_HPP
#define RTFCOLOR_HPP

// OpenClass
#include <isrtmap.h>        // ISortedMap
#include <icolor.hpp>       // IColor


class _Export RtfColor
{
   public:
     // constructor
     RtfColor(int cred, int cgreen, int cblue,int cnum);
       // set any color to -1 for an invalid (default) entry

     // equality
     int operator==( const RtfColor & color ) const;
     int operator!=( const RtfColor & color ) const;

     // queries
     const IColor * getColorentry() const;
       // returns 0 if the entry is invalid (default)
     const int & getKey() const;

   private:
     int      _colornumberKey;
     IColor   _colorentry;
     Boolean  _valid;

};


typedef ISortedMap <RtfColor, int> RtfColorTable;

const int & key( const RtfColor & color );


// inline functions
#include "RtfColor.ipp"


#endif
