/***************************************************************************
 * File...... TestIPF.cpp
 * Author.... Mat
 * Date...... 3/1/96
 *
 * Specific tests for IPF output, should still work under other generators
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#include "Generator.hpp"


static void InitSimpleTable( Generator & gen );


void TestIPF( Generator & gen )
{
  const Distance indent( 2880, Distance::twips );

  gen << SectionGin( 1, true ).setTitle( "IPF Tests" );
  gen << LabelGin("123#456");
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  gen << ParaGin()
      << ".Every .word .in .this .sentence .has .an .initial "
      << ".period .to .make .sure .that .none .are .interpreted "
      << ".as .control .words";

  // test embedded Gins in table
  gen << PushGin()
      << ParaGin();
  InitSimpleTable( gen );
  gen << RowGin()
      << BoldGin( true )
      << "Bold attribute"
      << ColumnGin()
      << FontGin( FontInfo::swiss, "" )
      << "Swiss font";
  gen << RowGin()
      << RightMarginGin( false, indent )
      << "Indent right margin"
      << ColumnGin()
      << LeftMarginGin( false, indent )
      << "Indent left margin";
  gen << RowGin()
      << ColorGin( IColor( IColor::darkBlue ) )
      << "Dark blue"
      << ColumnGin()
      << PushGin()
      << ColorGin( IColor( IColor::yellow ) )
      << ItalicGin( true )
      << "Yellow italic, this cell only"
      << PopGin();
  gen << RulerGin( RulerGin::normal )
      << ParaGin()
      << "Back to normal with attribute, font & margin changes."
      << PopGin()
      << ParaGin()
      << "This is a normal paragraph";

  // test reset after table done
  // also test that color carries through
  gen << PushGin()
      << ParaGin()
      << FontGin( FontInfo::roman, "" )
      << SizeGin( Distance( 20, Distance::twips ) )
      << ItalicGin( true )
      << LeftMarginGin( false, indent )
      << ColorGin( IColor( IColor::darkPink ) )
      << "Here is some tiny, roman, italic, purple indented (TRIPI) text."
      << ParaGin();
  InitSimpleTable( gen );
  gen << RowGin()
      << "West"
      << ColumnGin()
      << "East";
  gen << RulerGin( RulerGin::normal )
      << ParaGin()
      << "This text should also be TRIPI."
      << PopGin();

  // test table font size
  gen << PushGin()
      << ParaGin()
      << FontGin( FontInfo::roman, "" )
      << SizeGin( Distance( 2000, Distance::twips ) )
      << "Very big text"
      << ParaGin();
  InitSimpleTable( gen );
  gen << RowGin()
      << "Big"
      << ColumnGin()
      << "Table"
      << PopGin();

  // test nesting (not allowed within table)
  gen << PushGin();
  gen << RulerGin( RulerGin::list );
  gen << OutlineGin( OutlineGin::arabic );
  gen << ItemGin()
      << "First item (second item is a table)"
      << ItemGin();
  gen << PushGin();
  InitSimpleTable( gen );
  gen << RowGin()
      << "First cell"
      << ColumnGin()
      << "Second cell"
      << RowGin();
  gen << PushGin();
  gen << RulerGin( RulerGin::list );
  gen << OutlineGin( OutlineGin::arabic );
  gen << ItemGin()
      << "Alpha"
      << ItemGin()
      << "Beta"
      << PopGin();
  gen << ColumnGin()
      << "Fourth cell (3rd cell was a list)"
      << PopGin()
      << ItemGin()
      << "Third and last item."
      << PopGin();

  // link across a table
  gen << PushGin()
      << ParaGin()
      << LinkGin( "363" )
      << "This text and the following table are all in the same link:"
      << ParaGin();
  InitSimpleTable( gen );
  gen << RowGin()
      << "Told ya"
      << ColumnGin()
      << "So"
      << PopGin();

  gen << ParaGin()
      << "That's all for table testing.";

}


static void InitSimpleTable( Generator & gen )
{
  const Distance halfway( 50, Distance::percent );
  RulerGin simpleTable( RulerGin::table );
  simpleTable.addColumn( halfway );
  simpleTable.addColumn( halfway );
  gen << simpleTable << BorderGin(BorderGin::all);
}


void TestHTML( Generator & gen )
{
  const static char message[] = "Here is the sample HTML text. ";

  gen << SectionGin( 1, true ).setTitle( "HTML Tests" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  gen << ParaGin()
      << SizeGin( Distance( 160, Distance::twips ) )
      << "This entire section should use a small font."
      << ParaGin();

  // test cross-nesting of para vs char styles
  InitSimpleTable( gen );
  gen << RowGin()
      << "Next cell only is bold"
      << ColumnGin()
      << PushGin()
      << BoldGin(true)
      << message
      << PopGin();
  gen << RowGin()
      << "Next cell starts italic (should continue)"
      << ColumnGin()
      << ItalicGin(true)
      << message;
  gen << RowGin()
      << "Next cell is an ordered list"
      << ColumnGin()
      << PushGin();
      gen << RulerGin( RulerGin::list )
          << OutlineGin( OutlineGin::arabic )
          << ItemGin()
          << "First item in the ordered list"
          << ItemGin()
          << ColorGin( IColor( IColor::green ) )
          << "Second item in the ordered list is green, and so is the remainder of the ordered list."
          << ItemGin()
          << "Third item includes a bullet list:"
          << PushGin();
          gen << RulerGin( RulerGin::list )
              << OutlineGin( OutlineGin::bullet )
              << ItemGin()
              << "First item in bullet list"
              << ItemGin()
              << FontGin( FontInfo::mono, "" )
              << "Second item in bullt list is monospace font, and so is the remainder of the bullet list."
              << ItemGin()
              << "Third item in bullet list includes a quote from Shakespeare:"
              << PushGin();
              gen << RulerGin( RulerGin::quote )
                  << ParaGin()
                  << "To be or not to be. Whether 'tis nobler to suffer the slings and "
                  << "arrows of outragus "
                  << PushGin()
                  << UnderlineGin(true)
                  << "[sic]"
                  << PopGin()
                  << " fortune or whatever...";
              gen << PopGin();
          gen << PopGin();
      gen << PopGin();
  gen << RowGin()
      << "Next cell is absolutely normal, except for the "
      << LinkGin("123#456")
      << "link"
      << LinkGin()
      << "in the middle"
      << ColumnGin()
      << PushGin()
      << ItalicGin(false)
      << "Italic is turned off for this cell only"
      << PopGin();
  gen << RowGin()
      << "Next cell turns bold on and italic off (permanently)"
      << ColumnGin()
      << ItalicGin(false)
      << BoldGin(true)
      << message;
  gen << RowGin()
      << "Last cell makes no changes"
      << ColumnGin()
      << message;
}


