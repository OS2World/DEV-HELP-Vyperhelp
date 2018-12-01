/***************************************************************************
 * File...... SaveThread.cpp
 * Author.... Mat
 * Date...... 5/18/99
 *
 * Implementation of SaveThread.  The "run" function saves the document.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


// Standard C
#include <limits.h>

// Vyper
#include "VyDoc.hpp"
#include "VyDocParser.hpp"
#include "VyIndicator.hpp"
#include "SaveThread.hpp"


SaveThread::SaveThread( VyDoc & doc, VyIndicator & indicator, Generator * gen, const IString & filename ):
    _doc( doc ),
    _indicator( indicator ),
    _gen( gen ),
    _filename( filename )
{}


/***************************************************************************
 * Procedure.. SaveThread::run
 * Author..... Mat
 * Date....... 5/18/99
 *
 * This function runs as a separate thread.  It saves the current VyDoc
 * document to the given file, using the Generator provided.
 ***************************************************************************/
void SaveThread::run()
{
  Boolean isSave = ( _gen == 0 );
  IString originalFilename( _doc.filename() );

  // turn of container refresh and show progress indicator
  _doc.refreshOff();
  _indicator.start();
  _indicator.setTask( "Save or Export File" );
  _indicator.setStatus( _filename );

  // resources allocated for saving
  Filename * tempfile = 0;

  try
  {
    // PREPARE FOR A SAVE
    if ( isSave )
    {
      // if there is a section being edited, save it
      if ( _doc.openedSection() )
        _doc.openedSection()->save();

      // get a temporary file and SaveGenerator
      tempfile = new Filename( Filename::temporary );
      _gen = new SaveGenerator( *tempfile );
    }

    // SAVE OR EXPORT
    // use VyDocParser to save or export
    VyDocParser parser( _doc, isSave, _filename );
    parser.setIndicator( &_indicator );
    parser.generate( *_gen );

    // FINALIZE SAVE
    if ( isSave )
    {
      // set filename in case of rename (Save As)
      _doc.setFilename( _filename );

      // commit new file
      tempfile->rename( _filename );
      delete tempfile;
      tempfile = 0;

      // commmit the save for all sections
      IVSequence<VySection*>::Cursor cursor( *_doc.getSections() );
      forCursor( cursor )
        cursor.element()->commitSave();

      // clear modified flag
      _doc.setModified( false );
    }

    // make sure meter shows 100% done
    _indicator.setCurrent( ULONG_MAX );
  }
  catch ( IException & x )
  {
    // set filename to original
    _doc.setFilename( originalFilename );

    // remove temporary file
    if ( tempfile )
      tempfile->remove();

    // display exception info
    if ( x.errorId() )
      _indicator.error( IString("ERROR ") + IString(x.errorId()) + IString(" (") + x.errorCodeGroup() + IString(')') );
    _indicator.error( x.text() );
  }

  // free resources
  if ( _gen )
  {
    delete _gen;
    _gen = 0;
  }
  if ( tempfile )
    delete tempfile;

  // hide  progress indicator and refresh container
  _indicator.finish();
  _doc.refreshOn();
}


