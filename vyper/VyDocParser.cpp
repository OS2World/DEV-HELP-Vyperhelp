/***************************************************************************
 * File...... VyDocParser.cpp
 * Author.... Mat
 * Date...... 5/20/99
 *
 * Implementation of VyDocParser.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Generator
#include "MekTek.hpp"
#include "Generator.hpp"
#include "Parser.hpp"
#include "ParserIndicator.hpp"
#include "SaveGenerator.hpp"

// Vyper
#include "VyDoc.hpp"
#include "VySection.hpp"
#include "VyDocParser.hpp"


VyDocParser::VyDocParser( VyDoc & doc, Boolean isSave, const IString & filename ):
    _doc( doc ),
    _isSave( isSave ),
    _filename( filename )
{}


void VyDocParser::generate( Generator & gen )
{
  // save docinfo
  gen.setCodePage( CodePage() );  // set default code page
  gen.setTitle( _doc.title() );
  gen.setSubject( _doc.subject() );
  gen.setVersion( _doc.version() );
  gen.setCopyright( _doc.copyright() );
  gen.setAuthor( _doc.author() );

  // generate art definitions
  _doc.addArtTo( gen );

  // determine number of sections
  MEKASSERT( indicator() );
  indicator()->setTotal( _doc.getSections()->numberOfElements() );

  // generate each section
  unsigned n = 0;
  IVSequence<VySection*>::Cursor cursor( *_doc.getSections() );
  forCursor( cursor )
  {
    // generate section
    VySection * section = cursor.element();
    section->generate( gen, true, false );

    // register new filename and offset
    if ( _isSave )
    {
      section->setSaveFilename( _filename );
      section->setSaveOffset( ((SaveGenerator &)gen).sectionOffset() );
    }

    // update progress indicator
    indicator()->setCurrent( n++ );
  }

  gen.finish();
}

