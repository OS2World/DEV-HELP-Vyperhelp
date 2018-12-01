/***************************************************************************
 * File...... TestSections.cpp
 * Author.... Mat
 * Date...... 1/25/96
 *
 * Routine for testing Generator sections
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

void TestSections( Generator & gen )
{
  int i;
  const IString dummyText( "Dummy section text." );

  // contents level testing (specifically for CNT exporting)
  gen << SectionGin( 1, true ).setTitle( "Test 1" );
  gen << SectionGin( 1, true ).setTitle( "Test 2" );
  gen << SectionGin( 2, true ).setTitle( "Test 2.1" );
  gen << SectionGin( 3, true ).setTitle( "Test 2.1.1" );
  gen << SectionGin( 2, true ).setTitle( "Test 2.2" );
  gen << SectionGin( 3, true ).setTitle( "Test 2.2.1" );
  gen << SectionGin( 1, true ).setTitle( "Test 3" );
  gen << SectionGin( 1, true ).setTitle( "Test 4" );
  gen << SectionGin( 2, true ).setTitle( "Test 4.1" );
  gen << SectionGin( 3, true ).setTitle( "Test 4.1.1" );
  gen << SectionGin( 2, true ).setTitle( "Test 4.2" );
  gen << SectionGin( 2, true ).setTitle( "Test 4.3" );
  gen << SectionGin( 1, true ).setTitle( "Test 5" );
  gen << SectionGin( 1, true ).setTitle( "Test 6" );

  // levels 1 to 6, with link ids and index entries
  for (i = 1; i <= 6; i++) {
    IString levelText = "Level " + IString( i );
    gen << SectionGin( i, true ).setTitle( levelText )
        << LabelGin( levelText )
        << KeywordGin( "Section", true )
        << "This is text for "
        << levelText
        ;
  } /* endfor */

  // root heading for section testing
  gen << SectionGin( 1, true ).setTitle( "Section testing" );

  // unlisted section
  gen << SectionGin( 2, false )
      << KeywordGin( "Unlisted section", true )
      << "This is an unlisted section with no title and no label."
      ;

  // section with external (public) label
  gen << SectionGin( 2, true ).setTitle( "External label" )
      << LabelGin( "gentest").setExternal( true )
      << dummyText
      ;

  // section with a numeric id
  gen << SectionGin( 2, true ).setTitle( "Numeric label" )
      << LabelGin( "363").setId( 363 )
      << dummyText
      ;

  // section with multiple labels
  gen << SectionGin( 2, true ).setTitle( "Multiple labels" )
      << LabelGin( "Multi1")
      << LabelGin( "Multi2")
      << LabelGin( "Multi3")
      << dummyText
      ;

  // section with mid-topic label
  gen << SectionGin( 2, true ).setTitle( "Mid-topic label" )
      << "The label follows this sentence.  "
      << LabelGin( "MidTopic")
      << "The label precedes this sentence."
      ;
}

