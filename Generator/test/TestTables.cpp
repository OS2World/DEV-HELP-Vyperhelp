/***************************************************************************
 * File...... TestTables.cpp
 * Author.... Mat
 * Date...... 11/2/95
 *
 * Routines for testing Generator tables
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

static void SimpleTable( Generator & gen );

void TestTables( Generator & gen )
{
  // heading
  gen << SectionGin( 1, true ).setTitle( "Table testing" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  // single cell table
  Distance fourInches( 5760, Distance::twips );
  RulerGin oneCellTable( RulerGin::table );
  oneCellTable.addColumn( fourInches );
  gen << oneCellTable
      << RowGin()
      << "This is a single cell table, four inches wide.";

  // four column table, increasing sized columns
  RulerGin table( RulerGin::table );
  table.addColumn( Distance( 4, Distance::chars ) );
  table.addColumn( Distance( 8, Distance::chars ) );
  table.addColumn( Distance( 12, Distance::chars ) );
  table.addColumn( Distance( 16, Distance::chars ) );
  gen << table
      << RowGin()
      << "This column is 4 characters wide."
      << ColumnGin()
      << "This column is 8 characters wide."
      << ColumnGin()
      << "This column is 12 characters wide."
      << ColumnGin()
      << "This column is 16 characters wide."
      << RowGin()
      << "Second row."
      << ColumnGin()
      << "Second row."
      << ColumnGin()
      << "Second row."
      << ColumnGin()
      << "Second row.  This row has some more text, including a new paragraph"
      << ParaGin()
      << "This is the second paragraph in the last cell."
      << LineGin()
      << "This text follows a line break";

  // borders: top & bottom
  gen << RulerGin( RulerGin::normal )
      << ParaGin()
      << "Top and bottom borders only:"
      << BorderGin( BorderGin::top | BorderGin::bottom );
  SimpleTable( gen );

  // borders: left & right
  gen << RulerGin( RulerGin::normal )
      << ParaGin()
      << "Left and right borders only:"
      << BorderGin( BorderGin::left | BorderGin::right );
  SimpleTable( gen );

  // borders: inside only
  gen << RulerGin( RulerGin::normal )
      << ParaGin()
      << "Inside borders only:"
      << BorderGin( BorderGin::vertical | BorderGin::horizontal );
  SimpleTable( gen );
}


static void SimpleTable( Generator & gen )
{
  const Distance tenChars( 10, Distance::chars );
  RulerGin simpleTable( RulerGin::table );
  simpleTable.addColumn( tenChars );
  simpleTable.addColumn( tenChars );
  gen << simpleTable
      << RowGin()
      << "Northwest"
      << ColumnGin()
      << "Northeast"
      << RowGin()
      << "Southwest"
      << ColumnGin()
      << "Southeast";
}

