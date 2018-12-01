/***************************************************************************
 * File...... RtfFont.cpp
 * Author.... Mat
 * Date...... 2/11/99
 *
 * Implementation of RtfFont.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Generator
#include "CodePage.hpp"
#include "RtfFont.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <iavlkss.c>
#include <ismkss.c>

// typedef ISortedMap <RtfFont, int> RtfFontTable;
#pragma define(IGAvlKeySortedSetNode<RtfFont,int,IEKCOps<RtfFont,int> >)
#pragma define(IGAvlKeySortedSetCursor<RtfFont,int,IEKCOps<RtfFont,int> >)
#pragma define(IGAvlKSS<RtfFont,int,IEKCOps<RtfFont,int> >)
#pragma define(IWSrtMapOnKSSetCursor<RtfFont,int,IEKCOps<RtfFont,int>,IGAvlKSS<RtfFont,int,IEKCOps<RtfFont,int> > >)
#pragma define(IWSMOnKSS<RtfFont,int,IEKCOps<RtfFont,int>,IGAvlKSS<RtfFont,int,IEKCOps<RtfFont,int> > >)



RtfFont::RtfFont( int fnum, int fcodepage, Familyname ffamily, Fprqtype ffprq, const IString & fname ):
        _fontfamily(ffamily),
        _codepage(fcodepage),
        _fprq(ffprq),
        _fontnumberKey(fnum),
        _facename(fname)
{}


int RtfFont::operator==( const RtfFont & font ) const
{
  return ( _fontfamily  == font._fontfamily )
      && ( _codepage    == font._codepage )
      && ( _fprq        == font._fprq )
      && ( _facename    == font._facename );
}

int RtfFont::operator!=( const RtfFont & font ) const
{
  return ! operator==( font );
}


/***************************************************************************
 * Procedure...... RtfFont::mapFontfamily
 * Author.... Marco
 * Date...... 12/31/1995
 *
 * maps rtf fontfamily names to FontInfo family names.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
FontInfo::Family RtfFont::mapFontfamily() const
{
  FontInfo::Family familyname;

  switch (_fontfamily)
  {
    case FROMAN:
      familyname = FontInfo::roman;
      break;
    case FSWISS:
      familyname = FontInfo::swiss;
      break;
    case FMODERN:
      familyname = FontInfo::mono;
      break;
    default:
      if ( _fprq == FIX )
        familyname = FontInfo::mono;
      else
      {
        // use FontInfo to determine family
        FontInfo font( _facename );
        familyname = font.family();
      }
      break;
  }
  return familyname;
}

IString RtfFont::familyWord() const
{
  switch ( _fontfamily )
  {
  case RtfFont::FROMAN:
    return IString( "\\froman" );
  case RtfFont::FSWISS:
    return IString( "\\fswiss" );
  case RtfFont::FMODERN:
    return IString( "\\fmodern" );
  case RtfFont::FSCRIPT:
    return IString( "\\fscript" );
  case RtfFont::FDECOR:
    return IString( "\\fdecor" );
  case RtfFont::FTECH:
    return IString( "\\ftech" );
  case RtfFont::FBIDI:
    return IString( "\\fbidi" );
  }

  return IString();
}


IString RtfFont::pitchWord() const
{
  switch ( _fprq )
  {
  case RtfFont::FIX:
    return IString( "\\fprq1" );
  case RtfFont::VARIABLE:
    return IString( "\\fprq2" );
  }

  return IString();
}


IString RtfFont::codepageWord() const
{
  switch( _codepage )
  {
  case CodePage::ansi:
    return IString( "\\fcharset0" );
  case CodePage::symbol:
    return IString( "\\fcharset2" );
  }

  if ( _codepage > 0 )
    return IString("\\cpg") + IString(_codepage);

  return IString();
}

