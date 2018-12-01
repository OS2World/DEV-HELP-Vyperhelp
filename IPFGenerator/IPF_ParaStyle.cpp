/***************************************************************************
 * File...... ParaStyle.cpp
 * Author.... Mat
 * Date...... 11/2/95
 *
 * Contains all Gin stream operators which are related to paragraph style.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "IPF_Common.pch"
#include "IPF_Common.hpp"
#pragma hdrstop


void IPFGenerator::handleRightMargin( const RightMarginGin  & rmargin )
{
  int marginChars;

  marginChars = charsFromDistance( rmargin.margin() );
  if ( rmargin.isRelative() )
    marginChars += _next->rightMarginChars;
  _next->rightMarginChars = marginChars;
}


void IPFGenerator::handleLeftMargin( const LeftMarginGin   & lmargin )
{
  int marginChars;

  marginChars = charsFromDistance( lmargin.margin() );
  if ( lmargin.isRelative() )
    marginChars += _next->leftMarginChars;
  _next->leftMarginChars = marginChars;
}


void IPFGenerator::handleBorder( const BorderGin       & border )
{
  _next->borders = border.borders();
}


void IPFGenerator::handleAlign( const AlignGin        & align )
{
  switch ( align.alignment() ) {
  case AlignGin::left:
  case AlignGin::justify:    // IPF doesn't support justified test
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
  } /* endswitch */
}


void IPFGenerator::handleSpacing( const SpacingGin      & spacing )
{
  _next->spacingLines = linesFromDistance( spacing.spacing() );
}


void IPFGenerator::handleRuler( const RulerGin        & ruler )
{
  RulerGin::Mode rulerMode = ruler.mode();

  // DON'T ALLOW NESTING WITHIN TABLES
  if ( ( _active->mode == table ) &&
       ( _next->level < _next->maxLevel ) &&
       ( rulerMode != RulerGin::normal ) &&
       ( rulerMode != RulerGin::quote ) )
  {
    return;
  }


  // SET IPF MODE

  switch ( rulerMode ) {

  case RulerGin::normal:
    _next->mode = paragraph;
    break;

  case RulerGin::list:
    _next->mode = singleList;
    _next->forceModeChange = true;
    break;

  case RulerGin::quote:
    _next->mode = paragraph;
    // indent both margins by 5 characters
    *this <<
        RightMarginGin( true, Distance( 5, Distance::chars ) ) <<
        LeftMarginGin( true, Distance( 5, Distance::chars ) );
    break;

  case RulerGin::preformat:
    _next->mode = lines;
    break;

  case RulerGin::dlist:
    switch ( ruler.lineBreak() ) {
    case RulerGin::none:
      _next->mode = doubleList;
      break;
    case RulerGin::fit:
      _next->mode = doubleListFit;
      break;
    case RulerGin::all:
      _next->mode = doubleListBreak;
      break;
    default:
      BADSWITCH;
      break;
    } /* endswitch */
    _next->forceModeChange = true;
    break;

  case RulerGin::table:
    _next->mode = table;
    _next->forceModeChange = true;
    break;

  default:
    BADSWITCH;
    break;
  } /* endswitch */

  // CONVERT & SAVE COLUMN WIDTHS
  _next->columnChars.removeAll();
  int i, n = ruler.numberOfColumns();
  for (i = 1; i <= n; i++) {
    _next->columnChars.enqueue( charsFromDistance( ruler.column( i ) ) );
  } /* endfor */

  // SET NESTING LEVEL
  if ( _next->mode == paragraph ) {
    // back up if possible
    if ( _next->level > _next->minLevel ) {
      _next->level--;
    } /* endif */
  } else {
    // nest if possible
    if ( _next->level < _next->maxLevel ) {
      _next->level++;
    } /* endif */
  } /* endif */
}


void IPFGenerator::handleOutline( const OutlineGin      & outline )
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
    _next->list = listOrdered;
    break;
  default:
    BADSWITCH;
    break;
  } /* endswitch */
}


void IPFGenerator::handleDivider( const DividerGin      & divider )
{
  // send a centered line with ten connected horizontal lines
  sendControl( "ce &bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.", isFormat );
}

