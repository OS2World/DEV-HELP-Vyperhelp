/***************************************************************************
 * File...... LinkItem.cpp
 * Author.... Mat
 * Date...... 4/17/97
 *
 * Implementation of LinkItem
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Editor
#include "Editor.hpp"
#include "FlowItem.hpp"
#include "LinkItem.hpp"
#include "LinkView.hpp"


LinkItem::LinkItem( Item * parent, const LinkGin & gin ):
    Item( parent ),
    _gin( gin )
{}


View * LinkItem::newView( View * parent )
{
  return new LinkView( this, parent );
}


void LinkItem::setGin( const LinkGin & gin )
{
  _gin = gin;
  change();
}


INotificationId LinkItem::editPropertiesId() const
{
  return Editor::editLinkId;
}


void LinkItem::generate()
{
  parent()->asFlow()->sendGin( _gin );
}


IString LinkItem::dumpString() const
{
  IString dump = IString( "LinkItem " );
  if ( _gin.isExternal() )
    dump += _gin.file() + IString("/");
  dump += _gin.target();
  return dump;
}

