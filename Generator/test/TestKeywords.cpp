/***************************************************************************
 * File...... TestKeywords.cpp
 * Author.... Mat
 * Date...... 1/25/96
 *
 * Routines for testing Generator keywords
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

void TestKeywords( Generator & gen )
{
  const IString dummyText( "Dummy keyword text" );

  // root heading for keyword testing
  gen << SectionGin( 1, true ).setTitle( "Keyword testing" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  // single (simple) keyword
  gen << SectionGin( 2, true ).setTitle( "Single keyword" )
      << KeywordGin( "Keyword", true )
      << dummyText
      ;

  // re-use of a single keyword
  gen << SectionGin( 2, true ).setTitle( "Keyword re-use" )
      << KeywordGin( "Keyword", true )
      << dummyText
      ;

  // unlisted keyword
  gen << SectionGin( 2, true ).setTitle( "Unlisted keyword" )
      << KeywordGin( "Unlisted keyword", false )
      << dummyText
      ;

  // external keyword
  gen << SectionGin( 2, true ).setTitle( "External keyword" )
      << KeywordGin( "External", true ).setExternal( true )
      << dummyText
      ;
}

