/***************************************************************************
 * File...... SymbolItem.cpp
 * Author.... Mat
 * Date...... 4/1/97
 *
 * Implementation of SymbolItem.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "SymbolView.hpp"
#include "SymbolItem.hpp"


SymbolItem::SymbolItem( const Symbol & symbol, Item * parent ):
    Item( parent ),
    _symbol( symbol )
{}


View * SymbolItem::newView( View * parent )
{
  return new SymbolView( this, parent );
}


void SymbolItem::generate()
{
  parent()->asFlow()->sendGin( SymbolGin( _symbol ) );
}


IString SymbolItem::dumpString() const
{
  return IString( IString("SymbolItem(") + IString(_symbol.id()) + IString(")") );
}


