/***************************************************************************
 * File...... TestSymbols.cpp
 * Author.... Mat
 * Date...... 11/17/95
 *
 * Lists all Generator symbols!
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop


void CharacterTable( Generator & gen, int cp )
{
  char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  int i, j;
  char ch;

  // set codepage (default font)
  FontGin font( FontInfo::system, IString());
  font.setCodepage( cp );
  gen << ParaGin()
      << font
      << IString("Character table for code page ") + IString(cp);

  // create a standard character set table
  Distance small( 3, Distance::chars );
  RulerGin table( RulerGin::table );
  for ( i = 0; i < 17; i++ )
    table.addColumn( small );
  gen << table;

  // header row
  gen << RowGin();
  for ( j = 0; j < 16; j++ )
    gen << ColumnGin() << TextGin( IString(hex[j]) + IString('-') );

  // data rows
  for ( i = 0; i < 16; i++ )
  {
    gen << RowGin() << TextGin( IString('-') + IString(hex[i]) );
    ch = i;
    for ( j = 0; j < 16; j++ )
    {
      gen << ColumnGin();
      if ( ch )
        gen << TextGin( ch );  // avoid ASCII 0
      ch += 16;  // characters are listed downward
    }
  }
  gen << RulerGin( RulerGin::normal );
}


void TestSymbols( Generator & gen )
{
  gen << SectionGin( 1, true ).setTitle( "List of symbols" )
      << RulerGin( RulerGin::dlist ).addColumn( Distance( 5, Distance::chars ) )
      << SpacingGin( Distance( 0, Distance::chars ) );

  for ( Symbol::Identifier id = Symbol::startOfList;
        id <= Symbol::endOfList;
        id = Symbol::Identifier( id + 1 ) )
  {
    Symbol symbol( id );

    gen << RowGin()
        << symbol
        << ColumnGin()
        << symbol.description();
  }

  // sample character sets
  gen << SectionGin( 1, true ).setTitle( "Codepages (ÄßÇ)" );  // text should look like "ABC" if translated correctly
  CharacterTable( gen, 0 );
  CharacterTable( gen, 437 );
  CharacterTable( gen, 850 );
  CharacterTable( gen, 865 );
  CharacterTable( gen, 1004 );
}


