/***************************************************************************
 * File...... EndItem.cpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * Implementation of EndItem
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// TextEditor
#include "EndView.hpp"
#include "EndItem.hpp"


EndItem::EndItem( Item * parent ):
    Item( parent )
{}


View * EndItem::newView( View * parent )
{
  return new EndView( this, parent );
}


IString EndItem::dumpString() const
{
  return IString( "EndItem" );
}

