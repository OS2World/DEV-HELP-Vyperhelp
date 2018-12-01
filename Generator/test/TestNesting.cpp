/***************************************************************************
 * File...... TestNesting.cpp
 * Author.... Mat
 * Date...... 1/25/96
 *
 * Test Generator nesting (PushGin and PopGin)
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

void TestNesting( Generator & gen )
{
  RulerGin   list( RulerGin::list );
  RulerGin   normal( RulerGin::normal );
  OutlineGin simple( OutlineGin::none );
  OutlineGin ordered( OutlineGin::arabic );
  OutlineGin unordered( OutlineGin::bullet );
  TextGin    text( "Dummy text." );

  // section header
  gen << SectionGin( 1, true ).setTitle( "Nesting" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  // nested lists with no item text
  gen << normal << ParaGin()
      << "Lists with no item text:"
      << list << unordered
      << PushGin()
      << list << ordered
      << PushGin()
      << list << simple
      << PopGin()
      << PopGin();

  // nested lists with item text
  gen << normal << ParaGin()
      << "Lists with item text:"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << list << ordered
      << ItemGin() << text
      << PushGin()
      << list << simple
      << ItemGin() << text
      << PopGin()
      << PopGin();

  // nested lists with inner item text
  gen << normal << ParaGin()
      << "Lists with inner text only:"
      << list << unordered
      << PushGin()
      << list << ordered
      << ItemGin() << text
      << PopGin();

  // nested lists with item text
  gen << normal << ParaGin()
      << "Lists with one nesting level:"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << list << ordered
      << ItemGin() << text
      << list << simple
      << ItemGin() << text
      << PopGin();

  // nested lists with outer item text
  gen << normal << ParaGin()
      << "Lists with outer text only:"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << list << ordered
      << PopGin();

  // list with nested character styles
  gen << normal << ParaGin()
      << "List with nested bold:"
      << list << unordered
      << PushGin()
      << BoldGin( true )
      << ItemGin() << text
      << PopGin();

  // list with nested paragraph style
  gen << normal << ParaGin()
      << "List with nested paragraph:"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << normal
      << ParaGin() << text
      << PopGin();

  // alternating nested paragraphs/lists
  gen << normal << ParaGin()
      << "Alternating paragraphs and lists:"
      << normal
      << ParaGin() << text
      << PushGin()
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << normal
      << ParaGin() << text
      << PopGin()
      << PopGin();

  // sequential lists (no nesting)
  gen << normal << ParaGin()
      << "Sequential lists (no nesting):"
      << list << unordered
      << ItemGin() << text
      << list << ordered
      << ItemGin() << text;

  // sequential nested lists
  gen << normal << ParaGin()
      << "Sequential nested lists:"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << list << ordered
      << ItemGin() << text
      << PopGin()
      << PushGin()
      << list << simple
      << ItemGin() << text
      << PopGin();

  // same list nested 3x, text at inner only
  gen << normal << ParaGin()
      << "Nest same list 3x, text inner only:"
      << list << unordered
      << PushGin()
      << list << unordered
      << PushGin()
      << list << unordered
      << PushGin()
      << ItemGin() << text
      << PopGin()
      << PopGin()
      << PopGin();

  // nested list with paragraph between lists
  gen << normal << ParaGin()
      << "Nested list with paragraph between lists"
      << list << unordered
      << ItemGin() << text
      << PushGin()
      << list << simple
      << ItemGin() << text
      << normal
      << ParaGin() << text
      << list << unordered
      << ItemGin() << text
      << PopGin()
      << ItemGin() << text;

  // push immediately following section
  gen << SectionGin( 2, true ).setTitle( "Push right after Section" )
      << PushGin()
      << ParaGin()
      << text
      << PopGin();
}

