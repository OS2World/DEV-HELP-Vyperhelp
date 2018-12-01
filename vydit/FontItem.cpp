/***************************************************************************
 * File...... FontItem.cpp
 * Author.... Mat
 * Date...... 2/4/97
 *
 * Implementation of FontItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Generator
#include "MekTek.hpp"
#include "Generator.hpp"

// TextEditor
#include "Editor.hpp"
#include "FlowItem.hpp"
#include "FontInfo.hpp"
#include "FontItem.hpp"
#include "FontView.hpp"


FontItem::FontItem( Item * parent, const FontInfo & info ):
    Item( parent ),
    _info( info )
{}


View * FontItem::newView( View * parent )
{
  return new FontView( this, parent );
}


void FontItem::setInfo( const FontInfo & info )
{
  _info = info;
  change();
}


INotificationId FontItem::editPropertiesId() const
{
  return Editor::editFontId;
}


void FontItem::generate()
{
  FlowItem * flow = parent()->asFlow();
  flow->sendGin( FontGin( _info.family(), _info.name() ) );
  flow->sendGin( SizeGin( Distance( _info.pointSize() * 20, Distance::twips ) ) );
}


IString FontItem::dumpString() const
{
  return IString( "FontItem " ) + _info.name();
}

