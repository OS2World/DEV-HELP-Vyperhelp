/***************************************************************************
 * File...... PopItem.cpp
 * Author.... Mat
 * Date...... 4/22/97
 *
 * Implements PopItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "PopView.hpp"
#include "PopItem.hpp"


PopItem::PopItem( Item * parent, PushItem * buddy ):
  Item( parent ),
  _buddy( buddy )
{}


View * PopItem::newView( View * parent )
{
  return new PopView( this, parent );
}


void PopItem::generate()
{
  parent()->asFlow()->sendGin( PopGin() );
}


IString PopItem::dumpString() const
{
  return IString( "PopItem" );
}


