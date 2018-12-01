/***************************************************************************
 * File...... TestPara.cpp
 * Author.... Mat
 * Date...... 11/10/95
 *
 * Test Generator paragraph styles
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop


// PROTOTYPES
static void Divider( Generator & gen, const IString & text );
static void SingleList(
    Generator & gen,
    OutlineGin::Style style,
    Boolean compact,
    Boolean needPop );
static void DoubleList(
    Generator & gen,
    const Distance & width,
    Boolean compact,
    RulerGin::LineBreak lineBreak );


void TestPara( Generator & gen )
{
  static const IString sentence( "This is a sample sentence. " );

  gen << SectionGin( 1, true ).setTitle( "Paragraph style testing" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  // normal paragraph
  gen << "This is a normal paragraph. "
      << "The \\n after this sentence should be ignored.  \n"
      << "Was it?  "
      << sentence << sentence << sentence << sentence << sentence;

  // quoted paragraph
  gen << RulerGin( RulerGin::quote )
      << ParaGin()
      << "This is a quoted paragraph. "
      << sentence << sentence << sentence << sentence << sentence;

  // paragraph spacing
  gen << RulerGin( RulerGin::normal )
      << PushGin()
      << ParaGin()
      << SpacingGin( Distance( 2, Distance::chars ) )
      << "This is the first paragraph. There should be two lines after this paragraph."
      << ParaGin()
      << SpacingGin( Distance( 0, Distance::chars ) )
      << "This is the second paragraph. There should be "
      << ItalicGin( true ) << "no" << ItalicGin( false )
      << " blank line after this paragraph."
      << ParaGin()
      << "This is the third paragraph."
      << PopGin();

  // centered text
  gen << RulerGin( RulerGin::preformat )
      << AlignGin( AlignGin::center )
      << ParaGin()
      << "This section is centered\n"
      << "and preformatted\n"
      << "each\nword\non\na\nseparate\nline\n";

  // right-justified text
  gen << RulerGin( RulerGin::preformat )
      << AlignGin( AlignGin::right )
      << ParaGin()
      << "This section is right-justified\n"
      << "and preformatted\n"
      << "each\nword\non\na\nseparate\nline\n";

  // preformated text, mono font
  gen << RulerGin( RulerGin::preformat )
      << AlignGin( AlignGin::left )
      << ParaGin()
      << PushGin()
      << FontGin( FontInfo::mono, "" )
      << "This section is left-justified\n"
      << "and preformatted\n"
      << "also    monospaced   so\n"
      << "every   word         lines\n"
      << "        up"
      << PopGin();

  // divider lines
  gen << AlignGin( AlignGin::center )
      << ParaGin()
      << PushGin()
      << FontGin( FontInfo::swiss, "" )  // 20 pt Helvetica
      << SizeGin( Distance( 400, Distance::twips ) )
      << "Dividers are used to separate sections below:"
      << PopGin()
      << RulerGin( RulerGin::normal )
      << AlignGin( AlignGin::left );

  // single-column lists
  Divider( gen, "Simple list" );
  SingleList( gen, OutlineGin::none, false, true );
  Divider( gen, "Unordered list" );
  SingleList( gen, OutlineGin::bullet, false, true );
  Divider(gen, "Ordered list" );
  SingleList( gen, OutlineGin::arabic, false, true );

  // nested single-column lists
  Divider( gen, "Nested simple list" );
  SingleList( gen, OutlineGin::none, false, false );
  SingleList( gen, OutlineGin::none, true, false );
  SingleList( gen, OutlineGin::none, false, false );
  gen << PopGin() << PopGin() << PopGin();
  Divider( gen, "Nested unordered list" );
  SingleList( gen, OutlineGin::bullet, false, false );
  SingleList( gen, OutlineGin::bullet, true, false );
  SingleList( gen, OutlineGin::bullet, false, false );
  gen << PopGin() << PopGin() << PopGin();
  Divider( gen, "Nested ordered list" );
  SingleList( gen, OutlineGin::arabic, false, false );
  SingleList( gen, OutlineGin::arabic, true, false );
  SingleList( gen, OutlineGin::arabic, false, false );
  gen << PopGin() << PopGin() << PopGin();
  Divider( gen, "Nested lists (simple, unordered, ordered)" );
  SingleList( gen, OutlineGin::none, false, false );
  SingleList( gen, OutlineGin::bullet, true, false );
  SingleList( gen, OutlineGin::arabic, false, false );
  gen << PopGin() << PopGin() << PopGin();

  // double-column lists
  Distance fifteen( 15, Distance::chars );
  Distance five( 5, Distance::chars );
  Divider( gen, "Double list, 15 char indent" );
  DoubleList( gen, fifteen, false, RulerGin::none );
  Divider( gen, "Double list, break columns" );
  DoubleList( gen, five, false, RulerGin::all );
  Divider( gen, "Double list, break if needed" );
  DoubleList( gen, five, false, RulerGin::fit );
  Divider( gen, "Double list, compact" );
  DoubleList( gen, fifteen, true, RulerGin::none );
}


static void Divider( Generator & gen, const IString & text )
{
  gen << DividerGin()
      << ParaGin()
      << text
      << ":";
}


static void SingleList(
    Generator & gen,
    OutlineGin::Style style,
    Boolean compact,
    Boolean needPop )
{
  gen << PushGin()
      << RulerGin( RulerGin::list )
      << OutlineGin( style )
      << SpacingGin( Distance( compact? 0: 1, Distance::chars ) )
      << ItemGin()
      << "First item."
      << ItemGin()
      << "Second item."
      << ItemGin()
      << "Third item.";

  if ( needPop ) {
    gen << PopGin();
  } /* endif */
}


static void DoubleList(
    Generator & gen,
    const Distance & width,
    Boolean compact,
    RulerGin::LineBreak lineBreak )
{
  gen << PushGin()
      << RulerGin( RulerGin::dlist ).addColumn( width ).setLineBreak( lineBreak )
      << SpacingGin( Distance( compact? 0: 1, Distance::chars ) )
      << RowGin()
      << "First term"
      << ColumnGin()
      << "First definition goes here."
      << RowGin()
      << "Second term"
      << ColumnGin()
      << "Second definition goes here."
      << PopGin();
}


