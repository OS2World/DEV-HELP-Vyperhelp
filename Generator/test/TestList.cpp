/***************************************************************************
 * File...... TestList.cpp
 * Author.... Mat
 * Date...... 12/2/95
 *
 * Test a GinList collection to see if it correctly buffers a series
 * of Gins.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop


void TestList( Generator & gen )
{
  gen << SectionGin( 1, true ).setTitle( "GinList" );

  GinList list;
  list.add( GinPtr( new PushGin(), IINIT ) );
  list.add( GinPtr( new FontGin( FontInfo::swiss, "" ), IINIT ) );
  list.add( GinPtr( new SizeGin( Distance( 1000, Distance::twips ) ), IINIT ) );
  list.add( GinPtr( new ColorGin( IColor( IColor::blue ) ), IINIT ) );
  list.add( GinPtr( new BackColorGin( IColor( IColor::yellow ) ), IINIT ) );
  list.add( GinPtr( new ItalicGin( true ), IINIT ) );

  gen << ParaGin()
      << "This word is "
      << list << "special" << PopGin()
      << ". So is "
      << list << "this" << PopGin()
      << "!";
}

