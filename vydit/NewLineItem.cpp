/***************************************************************************
 * File...... NewLineItem.cpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * Implementation of NewLineItem
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// TextEditor
#include "FlowItem.hpp"
#include "NewLineView.hpp"
#include "NewLineItem.hpp"


NewLineItem::NewLineItem( Item * parent ):
    Item( parent )
{}


View * NewLineItem::newView( View * parent )
{
  return new NewLineView( this, parent );
}


void NewLineItem::generate()
{
  parent()->asFlow()->sendGin( LineGin() );
}


IString NewLineItem::dumpString() const
{
  return IString( "NewLineItem" );
}

