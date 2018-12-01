/***************************************************************************
 * File...... BoldItem.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implementation of BoldItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "BoldItem.hpp"
#include "BoldView.hpp"


BoldItem::BoldItem( Item * parent, Boolean isEnabled ):
    Item( parent ),
    _isEnabled( isEnabled )
{}


View * BoldItem::newView( View * parent )
{
  return new BoldView( this, parent );
}


void BoldItem::generate()
{
  parent()->asFlow()->sendGin( BoldGin( _isEnabled ) );
}


IString BoldItem::dumpString() const
{
  return IString( "BoldItem " ) + IString(_isEnabled);
}

