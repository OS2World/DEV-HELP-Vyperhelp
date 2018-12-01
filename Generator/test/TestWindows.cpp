/***************************************************************************
 * File...... TestWindows.cpp
 * Author.... Mat
 * Date...... 11/9/95
 *
 * Test Generator window placement.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

static const int idUpperLeft = 1;
static const int idLowerLeft = 2;
static const int idUpperRight = 3;
static const int idLowerRight = 4;
static const int idSmallCenter = 5;
static const int idSmallMax = 6;


void DefineWindows( Generator & gen )
{
  // define a variety of windows
  Distance halfDistance( 50, Distance::percent );
  WindowDef upperLeft(
      idUpperLeft,
      "Upper Left",
      Position( Position::min ),
      Position( halfDistance ),
      halfDistance,
      halfDistance,
      WindowDef::noBorder,
      WindowDef::noScroll,
      false,
      false,
      false,
      false
  );
  WindowDef lowerLeft(
      idLowerLeft,
      "Lower Left",
      Position( Position::min ),
      Position( Position::min ),
      halfDistance,
      halfDistance,
      WindowDef::staticBorder,
      WindowDef::vertScroll,
      true,
      false,
      false,
      false
  );
  WindowDef upperRight(
      idUpperRight,
      "Upper Right",
      Position( halfDistance ),
      Position( halfDistance ),
      halfDistance,
      halfDistance,
      WindowDef::sizeBorder,
      WindowDef::horizScroll,
      true,
      true,
      true,
      false
  );
  WindowDef lowerRight(
      idLowerRight,
      "Lower Right",
      Position( halfDistance ),
      Position( Position::min ),
      halfDistance,
      halfDistance,
      WindowDef::staticBorder,
      WindowDef::allScroll,
      true,
      false,
      false,
      true
  );
  Distance twoInches( 2880, Distance::twips );
  WindowDef smallCenter(
      idSmallCenter,
      "Small Center",
      Position( Position::center ),
      Position( Position::center ),
      twoInches,
      twoInches,
      WindowDef::staticBorder,
      WindowDef::noScroll,
      true,
      false,
      false,
      false
  );
  WindowDef smallMax(
      idSmallMax,
      "Small Max",
      Position( Position::max ),
      Position( Position::max ),
      twoInches,
      twoInches,
      WindowDef::staticBorder,
      WindowDef::noScroll,
      true,
      false,
      false,
      false
  );

  // add window definitions to generator
  gen.defineWindow( upperLeft )
     .defineWindow( lowerLeft )
     .defineWindow( upperRight )
     .defineWindow( lowerRight )
     .defineWindow( smallCenter )
     .defineWindow( smallMax );
}


void TestWindows( Generator & gen )
{
  // main heading
  gen << SectionGin( 1, true ).setTitle( "Window testing" );

  gen << SectionGin( 2, true ).setWindow( idUpperLeft )
      << "Upper left, no border, no scroll, no titlebar";

  gen << SectionGin( 2, true ).setWindow( idLowerLeft )
      << "Lower left, static border, vertical scroll, titlebar";

  gen << SectionGin( 2, true ).setWindow( idUpperRight )
      << "Upper right, size border, horizontal scroll, titlebar, min/max";

  gen << SectionGin( 2, true ).setWindow( idLowerRight )
      << "Lower right, static border, all scroll, titlebar, sysmenu";

  gen << SectionGin( 2, true ).setWindow( idSmallCenter )
      << "Centered 2x2in";

  gen << SectionGin( 2, true ).setWindow( idSmallMax )
      << "Maxed 2x2in";
}

