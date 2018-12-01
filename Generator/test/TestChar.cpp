/***************************************************************************
 * File...... TestChar.cpp
 * Author.... Mat
 * Date...... 11/9/95
 *
 * Test Generator character attributes
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "Generator.pch"
#include "Generator.hpp"
#pragma hdrstop

void TestChar( Generator & gen )
{
  gen << SectionGin( 1, true ).setTitle( "Character attributes testing" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  IString fontName;  // empty string, don't care about font name for now

  // test facenames
  gen << FontGin( FontInfo::mono, fontName )
      << "This uses a monospaced font. "
      << FontGin( FontInfo::swiss, fontName )
      << "This uses a swiss font. "
      << FontGin( FontInfo::roman, fontName )
      << "This uses a roman font. "
      << FontGin( FontInfo::system, fontName )
      << "This uses the system font.";

  // test different sizes
  Distance fivePoint( 100, Distance::twips );
  Distance fiftyPoint( 1000, Distance::twips );
  gen << PushGin()
      << ParaGin()
      << FontGin( FontInfo::system, fontName )
      << SizeGin( fivePoint )
      << "This is five point system font. "
      << FontGin( FontInfo::roman, fontName )
      << "This is five point roman font. "
      << SizeGin( fiftyPoint )
      << "This is fifty point roman font. "
      << FontGin( FontInfo::system, fontName )
      << "This is fifty point system font."
      << PopGin();

  // test standard colors
  gen << PushGin()
      << ParaGin()
      << ColorGin( IColor( IColor::white ) )
      << "white, "
      << ColorGin( IColor( IColor::blue ) )
      << "blue, "
      << ColorGin( IColor( IColor::red ) )
      << "red, "
      << ColorGin( IColor( IColor::pink ) )
      << "pink, "
      << ColorGin( IColor( IColor::green ) )
      << "green "
      << ColorGin( IColor( IColor::cyan ) )
      << "cyan, "
      << ColorGin( IColor( IColor::yellow ) )
      << "yellow, "
      << ColorGin( IColor( IColor::black ) )
      << "black, "
      << ColorGin( IColor( IColor::darkGray ) )
      << "dark gray, "
      << ColorGin( IColor( IColor::darkBlue ) )
      << "dark blue, "
      << ColorGin( IColor( IColor::darkRed ) )
      << "dark red, "
      << ColorGin( IColor( IColor::darkPink ) )
      << "dark pink, "
      << ColorGin( IColor( IColor::darkGreen ) )
      << "dark green, "
      << ColorGin( IColor( IColor::darkCyan ) )
      << "dark cyan, "
      << ColorGin( IColor( IColor::brown ) )
      << "brown, "
      << ColorGin( IColor( IColor::paleGray ) )
      << "pale gray."
      << PopGin();

  // RGB colors
  gen << PushGin()
      << ParaGin()
      << ColorGin( IColor( 255, 0,   0   ) )
      << "Rgb, "
      << ColorGin( IColor( 0,   255, 0   ) )
      << "rGb, "
      << ColorGin( IColor( 0,   0,   255 ) )
      << "rgB, "
      << ColorGin( IColor( 255, 255, 0   ) )
      << "RGb, "
      << ColorGin( IColor( 255, 0,   255 ) )
      << "RgB, "
      << ColorGin( IColor( 0,   255, 255 ) )
      << "rGB, "
      << ColorGin( IColor( 255, 255, 255 ) )
      << "RGB, "
      << ColorGin( IColor( 55,  155, 255 ) )
      << "R=55 G=155 B=255"
      << PopGin();

  // background color
  gen << PushGin()
      << ParaGin()
      << BackColorGin( IColor( IColor::yellow ) )
      << "Background color is yellow. "
      << ColorGin( IColor( IColor::blue ) )
      << "Now foreground color is blue. "
      << PopGin();

  // bold, italic, underline
  gen << PushGin()
      << ParaGin()
      << BoldGin( true )
      << "This is bold. "
      << BoldGin( false )
      << ItalicGin( true )
      << "This is italic. "
      << ItalicGin( false )
      << UnderlineGin( true )
      << "This is underline. "
      << ItalicGin( true )
      << "Now turn italic on. "
      << BoldGin( true )
      << "Now add bold. "
      << UnderlineGin( false )
      << "Now remove underline. "
      << ItalicGin( false )
      << "Now remove italic. "
      << BoldGin( false )
      << "Back to normal."
      << PopGin();

  // hidden text
  IString hideKey( "GENTEST" );
  gen << PushGin()
      << ParaGin()
      << "The text following this is hidden unless the GENTEST key is defined. "
      << ColorGin( IColor( IColor::red ) )
      << HideGin( hideKey )
      << "This is the hidden text (in red). "
      << HideGin()
      << "This text is not hidden, but it's still red."
      << PopGin();

  // links
  gen << PushGin()
      << ParaGin()
      << "Try the following links:"
      << RulerGin( RulerGin::list )
      << OutlineGin( OutlineGin::bullet )
      << ItemGin()
      <<   LinkGin( "363" )
      <<   "Jump to numeric label"
      <<   LinkGin()
      << ItemGin()
      <<   LinkGin( "Multi3" )
      <<   "Jump to multiple labels"
      <<   LinkGin()
      << ItemGin()
      <<   "This item isn't a link"
      << ItemGin()
      <<   LinkGin( "MidTopic" )
      <<   "Jump to mid-topic label"
      <<   LinkGin()
      << ItemGin()
      <<   LinkGin( "xtarget" ).setFile( "test2.inf" )
      <<   "Jump to an external target"
      << PopGin();

  // now combine them all!!
  gen << ParaGin()
      << PushGin()
      << FontGin( FontInfo::swiss, fontName )
      << "Start with a Swiss font. "
      << SizeGin( Distance( 240, Distance::twips ) )
      << "Size to 12 points. "
      << ColorGin( IColor( IColor::green ) )
      << "Set color to green. "
      << BackColorGin( IColor( IColor::yellow ) )
      << "Set background color to yellow. "
      << BoldGin( true )
      << "Turn bold on. "
      << ItalicGin( true )
      << "Turn italic on. "
      << UnderlineGin( true )
      << "Turn underline on. "
      << LinkGin( "gentest" )
      << "Link to 'gentest'. "
      << HideGin( hideKey )
      << "Hidden test (GENTEST). "
      << FontGin( FontInfo::roman, fontName )
      << "Font to Roman. "
      << SizeGin( Distance( 480, Distance::twips ) )
      << "Size to 24 points. "
      << ColorGin( IColor( IColor::red ) )
      << "Color to red. "
      << BackColorGin( IColor( IColor::cyan ) )
      << "Background color to cyan. "
      << BoldGin( false )
      << "Bold off. "
      << ItalicGin( false )
      << "Italic off. "
      << UnderlineGin( false )
      << "Underline off. "
      << LinkGin()
      << "Link off. "
      << HideGin()
      << "Hiding off."
      << PopGin();

  // comment text
  gen << ParaGin()
      << "Comment text follows: see generated IPF code. "
      << CommentGin( true )
      << "This is a comment. "
      << "It should not appear in the output text. "
      << BoldGin( true )
      << "This is still a comment, but a bold tag should precede it. "
      << CommentGin( false )
      << "This text should be visible and bold. "
      << CommentGin( true )
      << "End this section in comment mode, but new section should start OK.";

}

