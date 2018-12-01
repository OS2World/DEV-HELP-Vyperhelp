/***************************************************************************
 * File...... LoadThread.cpp
 * Author.... Mat
 * Date...... 5/18/99
 *
 * Implementation of LoadThread.  The "run" function loads the document.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <limits.h>

// Generator
#include "Parser.hpp"

// Vyper
#include "VyDoc.hpp"
#include "VyDocGenerator.hpp"
#include "VyIndicator.hpp"
#include "LoadThread.hpp"


LoadThread::LoadThread( VyDoc & doc, VyIndicator & indicator, Parser * parser, const IString & filename ):
    _doc( doc ),
    _indicator( indicator ),
    _parser( parser ),
    _filename( filename )
{}


/***************************************************************************
 * Procedure.. LoadThread::run
 * Author..... Mat
 * Date....... 5/18/99
 *
 * This function runs as a separate thread.  It loads a new document into
 * the VyDoc.
 ***************************************************************************/
void LoadThread::run()
{
  // turn of container refresh and show progress indicator
  _doc.refreshOff();
  _indicator.start();
  _indicator.setTask( "Load or Import File" );

  try
  {
    _indicator.setStatus( "Closing current document..." );

    // make sure previous doc is cleared
    // reset filename if this is an import
    _doc.reset( _parser != 0 );

    _indicator.setStatus( _filename );

    // use a VyDocGenerator to populate the VyDoc
    VyDocGenerator gen( _doc );

    // check the file date
    gen.setInputFilename( _filename );

    // load or generate the contents
    if ( _parser )
    {
      // set associated indicator
      _parser->setIndicator( &_indicator );

      // import with the given Parser
      _parser->generate( gen );
      delete _parser;
    }
    else
    {
      // load with a VYD loader
      ITRACE_RUNTIME( IString( "Load document from ") + _doc.filename() );
      LoadParser loader( _doc.filename() );

      // set associated indicator
      loader.setIndicator( &_indicator );

      // load the VYD file (sections are scanned, not loaded)
      loader.load( gen );
    }

    // select first section (if there is one)
    if ( ! _doc.isEmpty() )
      _doc.setSelection( 1 );

    // clear modified flag, set opened flag
    _doc.setModified( false );
    _doc.setOpened();

    // make sure meter shows 100% done
    _indicator.setCurrent( ULONG_MAX );
  }
  catch ( IException & x )
  {
    // display exception info
    if ( x.errorId() )
      _indicator.error( IString("ERROR ") + IString(x.errorId()) + IString(" (") + x.errorCodeGroup() + IString(')') );
    _indicator.error( x.text() );
  }

  // hide  progress indicator and refresh container
  _indicator.finish();
  _doc.refreshOn();
}


