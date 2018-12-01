/***************************************************************************
 * File...... HtmlParaStyle.cpp
 *
 * Contains all Gin stream operators which are related to paragraph style.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlCommon.hpp"


/***************************************************************************
 * Procedure.. NearestDivide
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Divides to integers and rounds to the nearest result.
 ***************************************************************************/
static int NearestDivide( int top, int bottom )
{
  div_t result = div( top, bottom );
  if ( result.rem > bottom / 2 )
    return result.quot + 1;
  else
    return result.quot;
}


/***************************************************************************
 * Procedure.. linesFromDistance
 *
 * Given a distance, determine the equivalent in text lines.  Return a
 * number >= 0, use 1 for default.
 ***************************************************************************/
static unsigned int linesFromDistance( const Distance & distance )
{
  int divisor;

  switch ( distance.units() )
  {
  case Distance::twips:
    divisor = 200;  // assume 10 pt text
    break;
  case Distance::chars:
    divisor = 1;
    break;
  case Distance::pixels:
  case Distance::percent:
    return 1;
  default:
    BADSWITCH;
    break;
  }

  int lines = NearestDivide( distance.length(), divisor );
  return lines < 0? 1: lines;  // use default if <0
}



void HtmlGenerator::handleSpacing( const SpacingGin      & spacing )
{
  _next->spacingLines = linesFromDistance( spacing.spacing() );
}


void HtmlGenerator::handleBorder( const BorderGin       & border )
{
  _next->hasBorder = (border.borders() != 0);
}


void HtmlGenerator::handleAlign( const AlignGin        & align )
{
  switch ( align.alignment() )
  {
  case AlignGin::left:
  case AlignGin::justify:    // HTML doesn't support justified test
    _next->align = alignLeft;
    break;

  case AlignGin::right:
    _next->align = alignRight;
    break;

  case AlignGin::center:
    _next->align = alignCenter;
    break;

  default:
    BADSWITCH;
    break;
  }
}


void HtmlGenerator::handleRuler( const RulerGin        & ruler )
{
  RulerGin::Mode rulerMode = ruler.mode();

  // SET MODE

  switch ( rulerMode ) {

  case RulerGin::normal:
    _next->mode = paragraph;
    break;

  case RulerGin::list:
    _next->mode = singleList;
    _next->forceModeChange = true;
    break;

  case RulerGin::quote:
    _next->mode = quotation;
    break;

  case RulerGin::preformat:
    _next->mode = lines;
    break;

  case RulerGin::dlist:
    _next->mode = doubleList;
    _next->forceModeChange = true;
    break;

  case RulerGin::table:
    _next->mode = table;
    _next->forceModeChange = true;
    break;

  default:
    BADSWITCH;
    break;
  }

  // RESET LIST/TABLE FLAG
  _next->hasItem = false;

  // CONVERT & SAVE COLUMN WIDTHS
  _next->columnPixels.removeAll();
  int i, n = ruler.numberOfColumns();
  for (i = 1; i <= n; i++)
  {
     _next->columnPixels.enqueue(NearestDivide( ruler.column(i).asTwips(), 20));
  }

  // SET NESTING LEVEL
  if ( _next->mode == paragraph )
  {
    // back up if possible
    if ( _next->level > _next->minLevel )
    {
      _next->level--;
    }
  }
  else
  {
    // nest if possible
    if ( _next->level < _next->maxLevel )
    {
      _next->level++;
    }
  }
}


void HtmlGenerator::handleOutline( const OutlineGin      & outline )
{
  switch ( outline.style() ) {
  case OutlineGin::none:
    _next->list = listSimple;
    break;
  case OutlineGin::bullet:
    _next->list = listUnordered;
    break;
  case OutlineGin::arabic:
  case OutlineGin::romanLower:
  case OutlineGin::romanUpper:
  case OutlineGin::alphaLower:
  case OutlineGin::alphaUpper:
    // Note: HTML supports different numbered lists, but none of our parsers currently do
    _next->list = listOrdered;
    break;
  default:
    BADSWITCH;
    break;
  } /* endswitch */
}


void HtmlGenerator::handleDivider( const DividerGin      & divider )
{
  // send a centered line with ten connected horizontal lines
  sendTag("hr");
}

