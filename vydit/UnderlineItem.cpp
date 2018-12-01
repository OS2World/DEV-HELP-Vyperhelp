/***************************************************************************
 * File...... UnderlineItem.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implementation of UnderlineItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "UnderlineItem.hpp"
#include "UnderlineView.hpp"


UnderlineItem::UnderlineItem( Item * parent, Boolean isEnabled ):
    Item( parent ),
    _isEnabled( isEnabled )
{}


View * UnderlineItem::newView( View * parent )
{
  return new UnderlineView( this, parent );
}


void UnderlineItem::generate()
{
  parent()->asFlow()->sendGin( UnderlineGin( _isEnabled ) );
}


IString UnderlineItem::dumpString() const
{
  return IString( "UnderlineItem " ) + IString(_isEnabled);
}

