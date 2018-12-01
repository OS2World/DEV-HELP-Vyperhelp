/***************************************************************************
 * File...... RtfSection.cpp
 * Author.... Mat
 * Date...... 1/12/99
 *
 * Implementation for RtfSection
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// MekTek
#include "mektek.hpp"

// Generator
#include "Generator.hpp"
#include "SaveGenerator.hpp"
#include "LoadParser.hpp"

// WinHelpParser
#include "RtfSection.hpp"

//
// TEMPLATE DEFINITIONS
//

// RtfSectionList


RtfSection::RtfSection():
  _label( 0 ),
  _contents( Filename::temporary ),
  _save( 0 )
{}


RtfSection::~RtfSection()
{
  reset();

  // delete contents file
  _contents.remove();
}


/***************************************************************************
 * Procedure.. RtfSection::reset
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Reset the title, label and contents to empty.
 ***************************************************************************/
void RtfSection::reset()
{
  // title
  _title = IString();

  // label
  if ( _label )
  {
    delete _label;
    _label = 0;
  }

  // contents
  finish();
}


/***************************************************************************
 * Procedure.. RtfSection::finish
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Finishes and deletes the SaveGenerator, if it is active.  The temp file
 * will still be out there, and a LoadParser can be used to fetch the contents.
 ***************************************************************************/
void RtfSection::finish()
{
  if ( _save )
  {
    _save->finish();
    delete _save;
    _save = 0;
  }
}


/***************************************************************************
 * Procedure.. RtfSection::addGin
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Append a Gin to the temporary file.
 ***************************************************************************/
void RtfSection::addGin( const Gin & gin )
{
  // create the SaveGenerator if necessary
  if ( ! _save )
    _save = new SaveGenerator( _contents, true );

  // send the Gin to the temp file
  *_save << gin;
}


/***************************************************************************
 * Procedure.. RtfSection::generate
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Re-generate the contents of the temporary file to the given Generator.
 ***************************************************************************/
void RtfSection::generate( Generator & gen ) const
{
  // assume saving is done
  MEKASSERT( _save == 0 );

  // use LoadParser to fetch the contents from the file
  LoadParser load( _contents, 0 );
  load.generateSection( gen );
}


/***************************************************************************
 * RtfSectionList
 *
 * Sequence of RtfSections
 ***************************************************************************/

RtfSectionList::RtfSectionList( INumber numberOfElements ):
  ISequence< RtfSectionPtr >( numberOfElements )
{}


