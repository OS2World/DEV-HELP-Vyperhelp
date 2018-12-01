/***************************************************************************
 * File...... ParagraphItem.cpp
 * Author.... Mat
 *
 * Implementation of ParagraphItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "ParagraphView.hpp"
#include "ParagraphItem.hpp"


ParagraphItem::ParagraphItem( Item * parent ):
    Item( parent )
{}


View * ParagraphItem::newView( View * parent )
{
  return new ParagraphView( this, parent );
}


void ParagraphItem::generate()
{
  parent()->asFlow()->sendGin( ParaGin() );
}


IString ParagraphItem::dumpString() const
{
  return IString( "ParagraphItem" );
}

