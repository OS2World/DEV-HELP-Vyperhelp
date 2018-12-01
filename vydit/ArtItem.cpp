/***************************************************************************
 * File...... ArtItem.cpp
 * Author.... Mat
 * Date...... 9/28/99
 *
 * Implementation of ArtItem.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Generator
#include "Gin.hpp"

// Editor
#include "Editor.hpp"
#include "FlowItem.hpp"
#include "ArtItem.hpp"
#include "ArtView.hpp"


ArtItem::ArtItem( Item * parent, const IString & id ):
    Item( parent ),
    _id( id )
{}


const IGBitmap * ArtItem::bitmap() const
{
  const ArtDef * art = parent()->asFlow()->getArt( _id );
  // return 0 if art definition is not found
  return art? &art->bitmap(): 0;
}

	
void ArtItem::setId( const IString & id )
{
  if ( _id != id )
  {
    _id = id;
    change();
  }
}


View * ArtItem::newView( View * parent )
{
  return new ArtView( this, parent );
}


INotificationId ArtItem::editPropertiesId() const
{
  return Editor::editArtId;
}


void ArtItem::generate()
{
  parent()->asFlow()->sendGin( ArtGin( _id, ArtGin::bottom ) );
}


IString ArtItem::dumpString() const
{
  return IString( "ArtItem " ) + IString( _id );
}

