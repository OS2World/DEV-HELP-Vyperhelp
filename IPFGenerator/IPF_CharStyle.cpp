/***************************************************************************
 * File...... CharStyle.cpp
 * Author.... Mat
 * Date...... 11/2/95
 *
 * Contains all Gin stream operators which are related to character style.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// IPF Generator
#include "IPF_Common.hpp"


void IPFGenerator::handleFont( const FontGin         & font )
{
  switch ( font.family() ) {
  case FontInfo::system:
    _next->font = fontDefault;
    break;
  case FontInfo::roman:
    _next->font = fontTimesRoman;
    break;
  case FontInfo::swiss:
    _next->font = fontHelvetica;
    break;
  case FontInfo::mono:
    _next->font = fontCourier;
    break;
  default:
    BADSWITCH;
    break;
  } /* endswitch */

  // codepage mapping
  // only IBM codepages (>0) or active codepage (0) are supported
  CodePage cp = font.codepage();

#if 0
  /// IPF can accept most codepages, but there is a bug where the default codepage
  /// is always used if the font is default!
  switch ( cp )
  {
    case 437:  // only these codepages are supported by IPFC (?)
    case 850:
    case 860:
    case 863:
    case 865:
      _next->outputCodepage = cp;
      break;

    default:
      // choose 850 if an unknown codepage is requested
      if ( cp )
        _next->outputCodepage = 850;
      else
        _next->outputCodepage = 0;
      break;
  }
#endif

  // we assume that any IBM codepage is valid!
  if ( cp >= 0 )
    _next->outputCodepage = cp;
  else
    _next->outputCodepage = 850;  // use default codepage

  // save the actual input codepage for translation
  _next->inputCodepage = font.codepage();
}


void IPFGenerator::handleSize( const SizeGin         & size )
{
  _next->pointSize = pointsFromDistance( size.height() );
}


/***************************************************************************
 * Procedure.. GetColor
 * Author..... Mat
 * Date....... 10/31/95
 *
 * Given an IColor object, determine the closes IPF color, using the
 * IColor.index() function which maps to the standard 16 VGA colors.
 ***************************************************************************/
static IPFColor GetColor( const ColorGin & color )
{
  #define NUM_COLOR 18
  const IPFColor colorFromIndex[NUM_COLOR] =
  {
      colorDefault,  // -2 white
      colorBlack,    // -1 black
      colorDefault,  //  0 default (white)
      colorBlue,     //  1 blue
      colorRed,      //  2 red
      colorPink,     //  3 pink
      colorGreen,    //  4 green
      colorCyan,     //  5 cyan
      colorYellow,   //  6 yellow
      colorBlack,    //  7 black
      colorDarkGray, //  8 dark gray
      colorDarkBlue, //  9 dark blue
      colorDarkRed,  // 10 dark red
      colorDarkPink, // 11 dark pink
      colorDarkGreen,// 12 dark green
      colorDarkCyan, // 13 dark cyan
      colorBrown,    // 14 brown
      colorPaleGray  // 15 pale gray
  };
  unsigned long i;
  try
  {
    i = color.color().index() + 2;
  }
  catch ( IInvalidRequest & x )
  {
    return colorDefault;
  }
  return i < NUM_COLOR? colorFromIndex[i]: colorDefault;
}


void IPFGenerator::handleColor( const ColorGin        & color )
{
  _next->foregroundColor = GetColor( color );
}

void IPFGenerator::handleBackColor( const BackColorGin    & color )
{
  _next->backgroundColor = GetColor( color );
}


void IPFGenerator::handleBold( const BoldGin         & bold )
{
  _next->isBold = bold.isOn();
}


void IPFGenerator::handleItalic( const ItalicGin       & italic )
{
  _next->isItalic = italic.isOn();
}


void IPFGenerator::handleUnderline( const UnderlineGin    & uline )
{
  _next->isUnderline = uline.isOn();
}


void IPFGenerator::handleComment( const CommentGin      & comment )
{
  _next->isComment = comment.isOn();
}


void IPFGenerator::handleLink( const LinkGin         & link )
{
  if ( link.isOn() ) {
    _next->linkTarget = link.target();
    if ( link.isExternal() ) {
      _next->linkTargetFile = link.file();
    } else {
      // empty string flags "not external"
      _next->linkTargetFile = IString();
    } /* endif */
  } else {
    // empty string flags "link off"
    _next->linkTarget = IString();
  } /* endif */
}


void IPFGenerator::handleHide( const HideGin         & hide )
{
  IString nextKey;
    // default (empty string) flags "hide off"

  // update new hide state
  if ( hide.isOn() ) {
    nextKey = hide.key();
  } /* endif */

  // if hide state is going to change, we need to flush all changes
  // otherwise control Gins will not be properly (un)hidden
  if ( _active->key != nextKey )
    prepareToSend();

  _next->key = nextKey;
}


