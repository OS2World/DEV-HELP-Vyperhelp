/***************************************************************************
 * File...... Filename.cpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Implementation of Filename, which is derived from IString.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// OS/2
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#include <os2.h>

// Standard C
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// OpenClass
#include <iexcept.hpp>

// Generator
#include "MekTek.hpp"
#include "Filename.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>
#include <iesseq.c>

// IEqualitySequence< Filename >
#pragma define(IGLnSqCrs<Filename,IEOps<Filename> >)
#pragma define(IGLinkedSequenceNode<Filename,IEOps<Filename> >)
#pragma define(IGLnSq<Filename,IEOps<Filename> >)
#pragma define(IWEqSeqOnSeqCursor<Filename,IEOps<Filename>,IGLnSq<Filename,IEOps<Filename> > >)
#pragma define(IWEqSeqOnSeq<Filename,IEOps<Filename>,IGLnSq<Filename,IEOps<Filename> > >)



unsigned int Filename::_maxPathLength = computeMaxPathLength();


Filename::Filename():
    IString()
{}

Filename::Filename( const IString & string ):
    IString( string )
{}

Filename::Filename( const char * string ):
    IString( string )
{}


Filename::Filename( AutoName autoname )
{
  // temporary is currently the only option
  // create a temporary file with a unique name
  char tempname[L_tmpnam];
  char * result = tmpnam( tempname );
  if ( result )
    overlayWith( tempname );
  else
    MEKTHROWCERROR();
}


// case-insensitive compare
int Filename::operator==( const Filename & filename ) const
{
  return upperCase( filename ) == upperCase( *this );
}

int Filename::operator!=( const Filename & filename ) const
{
  return ! operator==( filename );
}


/***************************************************************************
 * Procedure.. Filename::locateTo
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Get the full path, assuming the path is relative to the given directory.
 * This is done by temporarilly moving to the given directory and drive.
 ***************************************************************************/
void Filename::locateTo( const Filename & dir )
{
  // save current directory & drive
  char * homeDir = (char *) _alloca( _maxPathLength );
  if ( ! _getcwd( homeDir, _maxPathLength ) )
    MEKTHROWCERROR();
  int homeDrive = _getdrive();

  // change to the drive of the given filename
  int targetDrive = upperCase(dir.drive())[1];
  MEKASSERT( ( targetDrive >= 'A' ) && ( targetDrive <= 'Z' ) );
  targetDrive = targetDrive - 'A' + 1;  // A: = 1, B: = 2, etc
  int result = _chdrive( targetDrive );
  if ( result )
    MEKTHROWCERROR();

  // change to the directory of the given filename
  result = chdir( dir.subString( 1, dir.length() - 1 ) );
  if ( result )
  {
    _chdrive( homeDrive );
    MEKTHROWCERROR();
  }

  // determine the full path
  char * pathbuf = (char *) _alloca( _maxPathLength );
  if ( ! _fullpath( pathbuf, *this, _maxPathLength ) )
  {
    _chdrive( homeDrive );
    chdir( homeDir );
    MEKTHROWCERROR();
  }

  // return to the original drive and path
  _chdrive( homeDrive );
  chdir( homeDir );

  // set result
  overlayWith( pathbuf );
}


/***************************************************************************
 * Procedure.. Filename::locateOnPath
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Search the specified path for the file.  If found, prepend the path and
 * return true.  checkCurrentFirst is true if the current directory should
 * be checked first.
 ***************************************************************************/
Boolean Filename::locateOnPath( const IString & path, Boolean checkCurrentFirst )
{
  // allocate buffer from stack for result
  char * pathbuf = (char *) _alloca( _maxPathLength );

  // search the path
  APIRET rc = DosSearchPath(
      checkCurrentFirst? SEARCH_CUR_DIRECTORY: 0,
      path, *this, pathbuf, _maxPathLength );
  if ( rc ) {
    return false;
  }
  overlayWith( pathbuf );  // save full filename
  return true;
}


/***************************************************************************
 * Procedure.. Filename::locateOnStandardPath
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Search a standard path for the file.  If found, prepend the path and
 * return true.  checkCurrentFirst is true if the current directory should
 * be checked first.
 ***************************************************************************/
Boolean Filename::locateOnStandardPath( StandardPath path, Boolean checkCurrentFirst )
{
  // get path from the environment
  char * pathString = 0;
  switch ( path )
  {
    case include:
      pathString = getenv( "INCLUDE" );
      break;
    case tabpath:
      pathString = getenv( "TABPATH" );
      break;
    case artwork:
      pathString = getenv( "IPFCARTWORK" );
      break;
    case imbed:
      pathString = getenv( "IPFCIMBED" );
      break;
  }

  // create empty path if necessary
  if ( ! pathString )
  {
    if ( checkCurrentFirst )
      pathString = "";  // empty path list, still need to check current dir
    else
      return false;     // didn't find it
  }

  return locateOnPath( pathString, checkCurrentFirst );
}


/***************************************************************************
 * Procedure.. Filename::fullPath()
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Returns the fully-qualified filename.
 ***************************************************************************/
Filename Filename::fullPath() const
{
  // allocate buffer from stack for result
  char * pathbuf = (char *) _alloca( _maxPathLength );

  // call C run-time
  if ( _fullpath( pathbuf, *this, _maxPathLength ) == NULL )
    MEKTHROWCERROR();  // function failed!

  // return the result
  return IString( pathbuf );
}


/***************************************************************************
 * Procedure.. Filename::drive()
 * Author..... Mat
 *
 * Returns the drive letter of the filename (with colon).
 ***************************************************************************/
IString Filename::drive() const
{
  char drive[ _MAX_DRIVE ];
  _splitpath( fullPath(), drive, NULL, NULL, NULL );
  return IString( drive );
}


/***************************************************************************
 * Procedure.. Filename::extension()
 * Author..... Mat
 * Date....... 12/2/97
 *
 * Returns the extension of the filename (with leading period).
 ***************************************************************************/
IString Filename::extension() const
{
  char ext[ _MAX_EXT ];
  _splitpath( fullPath(), NULL, NULL, NULL, ext );
  return IString( ext );
}


/***************************************************************************
 * Procedure.. Filename::base()
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Returns the base name portion of the filename.
 ***************************************************************************/
IString Filename::base() const
{
  char fname[ _MAX_FNAME ];
  _splitpath( fullPath(), NULL, NULL, fname, NULL );
  return IString( fname );
}


/***************************************************************************
 * Procedure.. Filename::name()
 * Author..... Mat
 * Date....... 12/2/97
 *
 * Returns the base filename plus extension.
 ***************************************************************************/
Filename Filename::name() const
{
  char fname[ _MAX_FNAME ];
  char ext[ _MAX_EXT ];
  _splitpath( fullPath(), NULL, NULL, fname, ext );
  return IString( fname ) + IString( ext );
}


/***************************************************************************
 * Procedure.. Filename::fullBase()
 * Author..... Mat
 * Date....... 3/5/96
 *
 * Retunrs drive + dir + base portion of filename.
 ***************************************************************************/
IString Filename::fullBase() const
{
  char drive[ _MAX_DRIVE ];
  char dir[ _MAX_DIR ];
  char fname[ _MAX_FNAME ];
  _splitpath( fullPath(), drive, dir, fname, NULL );
  return IString( drive ) + IString( dir ) + IString( fname );
}


/***************************************************************************
 * Procedure.. Filename::dir()
 * Author..... Mat
 * Date....... 3/5/96
 *
 * Returns the drive + directory (with trailing backslash)
 ***************************************************************************/
IString Filename::dir() const
{
  char drive[ _MAX_DRIVE ];
  char dir[ _MAX_DIR ];
  _splitpath( fullPath(), drive, dir, NULL, NULL );
  return IString( drive ) + IString( dir );
}


/***************************************************************************
 * Procedure.. Filename::maxPathLength
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Return system-wide constant max path\file length (including 0 terminator.
 ***************************************************************************/
unsigned int Filename::maxPathLength()
{
  return _maxPathLength;
}



/***************************************************************************
 * Procedure.. FilenameList::FilenameList
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Constructor for FilenameList
 ***************************************************************************/
FilenameList::FilenameList( INumber numberOfElements ):
    IEqualitySequence< Filename >( numberOfElements )
{}


/***************************************************************************
 * Procedure.. FilenameList::locateFullPath
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Assuming that filename is a filename without a path, this function will
 * search the filename list for a match.  If a match is found, filename is
 * modified to include the entire filespec found and true is returned.
 ***************************************************************************/
Boolean FilenameList::locateFullPath( Filename & filename ) const
{
  // check every filename in the list for a match
  Cursor cursor( *this );
  forCursor( cursor )
  {
    if ( cursor.element().name() == filename )
    {
      // found it!  return entry from list
      filename = cursor.element();
      return true;
    }
  }

  return false;  // coudn't find it!
}


/***************************************************************************
 * Procedure.. Filename::computeMaxPathLength
 * Author..... Mat
 * Date....... 12/26/95
 *
 * sets system-wide max path\file length
 ***************************************************************************/
unsigned int Filename::computeMaxPathLength()
{
  // get max path\file length or use default of 255
  ULONG maxPathLength = 255;
  DosQuerySysInfo( QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH,
      &maxPathLength, sizeof( maxPathLength ) );
  return ++maxPathLength;  // allow for zero-terminator
}


/***************************************************************************
 * Procedure.. Filename::isAccessible
 * Author..... Mat
 * Date....... 3/6/98
 *
 * Returns true if the file exists and is accessible in the specified mode.
 * mode values are specified by the standard C access() call.
 ***************************************************************************/
Boolean Filename::isAccessible( int mode ) const
{
  return ( access( *this, mode ) == 0 );
}


/***************************************************************************
 * Procedure.. Filename::remove
 * Author..... Mat
 * Date....... 1/15/98
 *
 * Remove (delete) this file.
 ***************************************************************************/
void Filename::remove() const
{
  if ( isAccessible() )
  {
    int result = ::remove( *this );
    if ( result )
      MEKTHROWCERROR();
  }
}



/***************************************************************************
 * Procedure.. Filename::rename
 * Author..... Mat
 * Date....... 3/5/98
 *
 * Rename (copy) this file to the specified name.  Doesn't actually change
 * this string, but copies the contents to the given string.  If the
 * target already exists, it will be deleted.  This function works across
 * drives by physically copying the file contents and then deleting the
 * source file.
 *
 * On return, the previous source filename is deleted.
 ***************************************************************************/
void Filename::rename( const Filename & target ) const
{
  // delete target if it exists
  target.remove();

  if ( drive() == target.drive() )
  {
    // use rename (faster) if on the same drive
    int result = ::rename( *this, target );
    if ( result )
      MEKTHROWCERROR();
  }
  else
  {
    // need to physically copy the file
    int result = system( IString("copy ") + *this + IString(" ") + target + IString(" >nul 2>nul") );
    if ( result )
      MEKTHROWCERROR();
    // after successful copy, delete source file
    remove();
  }
}

