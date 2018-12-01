/***************************************************************************
 * File...... IPF_Control.cpp
 * Author.... Mat
 * Date...... 11/2/95
 *
 * Contains all Gin stream operators for document control.  Depending on
 * the current context's mode, these Gins are interpreted differently.
 * The following table lists IPF tags sent for each mode (rows), when the
 * Gin (column) is streamed.
 *
 *               ParaGin  LineGin  RowGin   ColGin   ItemGin
 * paragraph     :p       .br
 * singleList    :lp      .br                        :li
 * lines         CR/LF    CR/LF
 * doubleList    :p       .br      :dt      :dd
 * table         :p       .br      :row:c   :c
 *
 * Also: the SpacingGin determines the number of lines between paragraphs.
 * If spacing=0, then .br is used in place of :p.  If spacing >1 then
 * that many :p's will be used.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "IPF_Common.pch"
#include "IPF_Common.hpp"
#pragma hdrstop


void IPFGenerator::handleRow( const RowGin          & row )
{
  switch ( _next->mode ) {
  case table:
    nextCell( true );
    break;
  case doubleList:
  case doubleListBreak:
  case doubleListFit:
    sendTag( "dt", breakBefore | isFormat );
    break;
  } /* endswitch */
}


void IPFGenerator::handleColumn( const ColumnGin       & column )
{
  switch ( _next->mode ) {
  case table:
    nextCell( false );
    break;
  case doubleList:
  case doubleListBreak:
  case doubleListFit:
    sendTag( "dd", breakBefore | isFormat );
    break;
  } /* endswitch */
}


void IPFGenerator::handleItem( const ItemGin         & item )
{
  if ( _next->mode == singleList )
  {
    sendTag( "li", breakBefore | isFormat );
  }
  else if ( _next->mode == table )
  {
    // tables don't allow nested lists, but this will at least support
    // newlines for each list item
    sendTag( "p", breakBefore | isFormat );
  }
}


void IPFGenerator::handlePara( const ParaGin         & para )
{
  int numberOfLines = _next->spacingLines;

  // use LineGin if no space between lines
  if ( ! numberOfLines )
  {
    handleLine( LineGin() );
    return;
  }

  // if in "lines" mode, use outputLine()
  if ( _next->mode == lines )
  {
    prepareToSend();
    while ( numberOfLines-- )
      outputLine();
    return;
  }

  // otherwise, use :lp or :p
  IString tag = ( _next->mode == singleList )? "lp": "p";
  while ( numberOfLines-- )
    sendTag( tag, breakBefore | isFormat );
}


void IPFGenerator::handleLine( const LineGin         & line )
{
  if ( _next->mode == lines )
  {
    // use outputLine in lines (literal) mode
    prepareToSend();
    outputLine();
  }
  else if ( _next->mode != table )
  {
    sendControl( "br", isFormat );
  }
  else
  {
    // need to use :p. while in a table
    sendTag( "p", breakBefore | isFormat );
  }
}


