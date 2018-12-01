/***************************************************************************
 * File...... PushItem.cpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Implements PushItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "PushView.hpp"
#include "PushItem.hpp"


PushItem::PushItem( Item * parent ):
  Item( parent )
{}


View * PushItem::newView( View * parent )
{
  return new PushView( this, parent );
}


void PushItem::generate()
{
  parent()->asFlow()->sendGin( PushGin() );
}


IString PushItem::dumpString() const
{
  return IString( "PushItem" );
}



