/***************************************************************************
 * File...... FontDefinition.cpp
 * Author.... Mat
 * Date...... 5/31/99
 *
 * Implementation of FontDefinition.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// OS/2
#define INCL_PM
#include <os2.h>

// OpenClass
#include <ifont.hpp>

// Generator
#include "FontDefinition.hpp"


//
// TEMPLATE INSTANCE
//
#include <ihshks.c>

#pragma define(IGHashKeySetCursor<FontDefinition,IString,IKEHOps<FontDefinition,IString> >)
#pragma define(IHashKeySet<FontDefinition,IString>)
#pragma define(IGHsKS<FontDefinition,IString,IKEHOps<FontDefinition,IString> >)


// static data
FontDefinitionSet FontDefinition::_set( 50 );


FontDefinition::FontDefinition( const IString & name ):
    _name( name )
{
  // save static font info
  IFont font( name );
  _isFixed = font.isFixed();
  _family = IString( font.fontmetrics()->szFamilyname );

  // determine if font is vector
  if ( name.length() )
  {
    // force vector type font
    // if name of the vector font is unchanged, then this font is available in vector form
    IFont vector( name, 0, false, true );
    _isVector = ( vector.name() == name );
  }
  else
  {
    // for default font, check state using isBitmap fuction
    _isVector = ! font.isBitmap();
  }
}


const FontDefinition * FontDefinition::locate( const IString & name )
{
  if ( ! _set.numberOfElements() )
  {
    // add a default font definition
    _set.add( FontDefinition( IString() ) );

    // load the static set
    IFont::FaceNameCursor faces;
    forCursor( faces )
      _set.add( FontDefinition( IFont::faceNameAt( faces ) ) );
  }

  // find the specified font
  FontDefinitionSet::Cursor cursor( _set );
  if ( _set.locateElementWithKey( name, cursor ) )
    return &cursor.element();

  // not found
  return 0;
}


const IString & key( const FontDefinition & fd )
{
  return fd.name();
}


