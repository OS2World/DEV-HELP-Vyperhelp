/***************************************************************************
 * File...... RtfGeneratorContext.cpp
 * Author.... Mat
 * Date...... 2/19/99
 *
 * Implementation for RtfGeneratorContext and RtfGeneratorContextStack.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include "RtfGeneratorContext.hpp"


RtfGeneratorContextStack::RtfGeneratorContextStack( INumber numberOfElements ):
  IStack< RtfGeneratorContext >( numberOfElements )
{}


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>
#include <istkseq.c>

// RtfGeneratorContextStack
#pragma define(IGLinkedSequenceNode<RtfGeneratorContext,IStdOps<RtfGeneratorContext> >)
#pragma define(IGLnSqCrs<RtfGeneratorContext,IStdOps<RtfGeneratorContext> >)
#pragma define(IGLnSq<RtfGeneratorContext,IStdOps<RtfGeneratorContext> >)
#pragma define(IWStackOnSeqCursor<RtfGeneratorContext,IStdOps<RtfGeneratorContext>,IGLnSq<RtfGeneratorContext,IStdOps<RtfGeneratorContext> > >)
#pragma define(IWStOnSq<RtfGeneratorContext,IStdOps<RtfGeneratorContext>,IGLnSq<RtfGeneratorContext,IStdOps<RtfGeneratorContext> > >)



/***************************************************************************
 * Procedure.. RtfGeneratorContext::reset
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Reset context settings after a ruler change or after a new section.  For
 * new section, all settings get reset.
 ***************************************************************************/
void RtfGeneratorContext::reset( Boolean all )
{
  mode = RulerGin::normal;
  wrap = RulerGin::none;
  itemNumber = 0;

  if ( all )
  {
    borders = 0;
    itemStyle = OutlineGin::none;
    spacing = 0;
    leftMargin = 0;
    rightMargin = 0;
    tab = 0;
    baseLeftMargin = 0;
    baseRightMargin = 0;
    baseTab = 0;
    cp = RTF_CODEPAGE;
    tableNested = 0;
    isComment = false;
  }
}


/***************************************************************************
 * Procedure.. RtfGeneratorContext::changedFrom
 * Author..... Mat
 * Date....... 3/18/99
 *
 * Called to determine if the paragraph styles have changed from one context
 * to the next.
 ***************************************************************************/
Boolean RtfGeneratorContext::changedFrom( const RtfGeneratorContext & other ) const
{
  return (
       ( mode != other.mode )
    || ( wrap != other.wrap )
    || ( borders != other.borders )
    || ( itemStyle != other.itemStyle )
    || ( spacing != other.spacing )
    || ( leftMargin != other.leftMargin )
    || ( rightMargin != other.rightMargin )
    || ( tab != other.tab )
    || ( alignment != other.alignment )
    );
}


