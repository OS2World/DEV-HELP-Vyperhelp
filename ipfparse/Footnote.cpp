/***************************************************************************
 * File...... Footnote.cpp
 * Author.... Mat
 * Date...... 10/27/98
 *
 * Implementation of Footnote.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Generator
#include "Generator.hpp"

// IpfParser
#include "Footnote.hpp"


Footnote::Footnote( const IString & id, int level, int windowId ):
    _id( id ),
    _level( level ),
    _windowId( windowId )
{}


/***************************************************************************
 * Procedure.. Footnote::sendTo
 * Author..... Mat
 * Date....... 10/27/98
 *
 * Send the footnote to the specified Generator.
 ***************************************************************************/
void Footnote::sendTo( Generator & gen )
{
  // send section and label
  SectionGin section( _level, false );
  section.setWindow( _windowId );
  section.setTitle( "Footnote" );
  gen << section << LabelGin( _id );

  // send contents
  gen << _list;
}


