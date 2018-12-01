/***************************************************************************
 * File...... ItalicItem.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implementation of ItalicItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "ItalicItem.hpp"
#include "ItalicView.hpp"


ItalicItem::ItalicItem( Item * parent, Boolean isEnabled ):
    Item( parent ),
    _isEnabled( isEnabled )
{}


View * ItalicItem::newView( View * parent )
{
  return new ItalicView( this, parent );
}


void ItalicItem::generate()
{
  parent()->asFlow()->sendGin( ItalicGin( _isEnabled ) );
}


IString ItalicItem::dumpString() const
{
  return IString( "ItalicItem " ) + IString(_isEnabled);
}

