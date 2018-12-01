/***************************************************************************
 * File...... TestMargins.cpp
 * Author.... Mat
 * Date...... 1/25/96
 *
 * Test Generator margins
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

void TestMargins( Generator & gen )
{
  const IString dummyPara(
      "This is a sample paragraph which is long enough so that it will "
      "probably wrap to another line." );

  // heading
  gen << SectionGin( 1, true ).setTitle( "Margin testing" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  // default margins
  gen << "Default margins.  "
      << dummyPara;

  // test left margin
  gen << ParaGin()
      << LeftMarginGin( false, Distance( 8, Distance::chars ) )
      << "Set left margin to 8 chars.  "
      << dummyPara;

  // test right margin
  gen << ParaGin()
      << RightMarginGin( false, Distance( 8, Distance::chars ) )
      << "Set right margin to 8 chars.  "
      << dummyPara
      << ParaGin();

  // test relative margins
  gen << RightMarginGin( true, Distance( 4, Distance::chars ) )
      << LeftMarginGin( true, Distance( 4, Distance::chars ) )
      << "Indent margins on both sides by 4 chars.  "
      << dummyPara
      << ParaGin();

  // test units=twips
  Distance oneInch( 1440, Distance::twips );  // 1440 twips/inch
  gen << RightMarginGin( false, oneInch )
      << LeftMarginGin( false, oneInch )
      << "Set margins to 1440 twips (1 inch).  "
      << dummyPara
      << ParaGin();

  // test units=pixels
  Distance hundredPixels( 100, Distance::pixels );  // 100 pixels
  gen << RightMarginGin( false, hundredPixels )
      << LeftMarginGin( false, hundredPixels )
      << "Set margins to 100 pixels.  "
      << dummyPara
      << ParaGin();

  // test units=percent
  Distance tenPercent( 10, Distance::percent );
  gen << RightMarginGin( false, tenPercent )
      << LeftMarginGin( false, tenPercent )
      << "Set margins to 10%.  "
      << dummyPara
      << ParaGin();
}

