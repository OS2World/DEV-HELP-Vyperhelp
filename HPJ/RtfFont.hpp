/***************************************************************************
 * File...... RtfFont.hpp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * RTF font table element.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef RTFFONT_HPP
#define RTFFONT_HPP

// OpenClass
#include <istring.hpp>      // IString
#include <isrtmap.h>        // ISortedMap

#include "FontInfo.hpp"


class _Export RtfFont
{
   public:
     enum Familyname   {FNIL,FROMAN,FSWISS,FMODERN,FSCRIPT,FDECOR,FTECH,FBIDI};
     enum Fprqtype     {DEFAULT,FIX,VARIABLE};

     RtfFont( int fnum, int fcodepage, Familyname ffamily, Fprqtype ffprq, const IString & fname );

     // equality
     int operator==( const RtfFont & font ) const;
     int operator!=( const RtfFont & font ) const;

     Familyname family() const;
     Fprqtype   pitch() const;
     const IString & getFacename() const;
     FontInfo::Family mapFontfamily() const;
     int getCodepage() const;
     const int & getKey() const;

     // RTF control word generation
     IString familyWord() const;
     IString pitchWord() const;
     IString codepageWord() const;

   private:
     Familyname     _fontfamily;
     int            _codepage;
     Fprqtype       _fprq;
     int            _fontnumberKey;
     IString        _facename;
};


typedef ISortedMap <RtfFont, int> RtfFontTable;

const int & key( const RtfFont & font );


// inline functions
#include "RtfFont.ipp"


#endif


