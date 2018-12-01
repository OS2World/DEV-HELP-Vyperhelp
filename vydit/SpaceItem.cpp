/***************************************************************************
 * File...... SpaceItem.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of SpaceItem
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "FlowItem.hpp"
#include "SpaceView.hpp"
#include "SpaceItem.hpp"


SpaceItem::SpaceItem( Item * parent ):
    Item( parent )
{}


View * SpaceItem::newView( View * parent )
{
  return new SpaceView( this, parent );
}


void SpaceItem::generate()
{
  parent()->asFlow()->sendText( ' ' );
}


IString SpaceItem::dumpString() const
{
  return IString( "SpaceItem" );
}

