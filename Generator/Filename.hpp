/***************************************************************************
 * File...... Filename.hpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Filename is an IString which is a filename.  It knows about paths, etc.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef FILENAME_HPP
#define FILENAME_HPP

// OpenClass
#include <ieqseq.h>
#include <istring.hpp>


class _Export Filename: public IString
{
  public:
    // constructor
    Filename();
    Filename( const IString & string );
    Filename( const char * string );
    enum AutoName
    {
      temporary
    };
    Filename( AutoName autoname );

    // case-insensitive compare
    int operator==( const Filename & filename ) const;
    int operator!=( const Filename & filename ) const;

    // enumerators
    enum StandardPath
    {
      include,
      tabpath,
      artwork,
      imbed
    };

    // locators
    void    locateTo( const Filename & dir );
    Boolean locateOnPath( const IString & path, Boolean checkCurrentFirst = false );
    Boolean locateOnStandardPath( StandardPath path, Boolean checkCurrentFirst = false );

    // filename parts
    Filename fullPath() const;    // "d:\path\file.ext"
    IString  drive() const;       // "d:"
    IString  dir() const;         // "d:\path\"
    IString  extension() const;   // ".ext"
    IString  base() const;        // "file"
    Filename name() const;        // "file.ext"
    IString  fullBase() const;    // "d:\path\file"

    // file operations
    enum AccessMode
    {
      // bits defined by standard C access() call
      readAccess = 0x04,
      writeAccess = 0x02
    };
    Boolean isAccessible( int mode = 0 ) const;
      // returns TRUE if the file is accessible
      // flags from AccessMode can be OR'd together
    void    remove() const;
    void    rename( const Filename & target ) const;
      // renames (copies contents) to the given filename
      // deletes target if it already exists
      // works across drives

    // system constants
    static unsigned int maxPathLength();

  private:
    static unsigned int _maxPathLength;
    static unsigned int computeMaxPathLength();
};


/***************************************************************************
 * Class...... FilenameList
 * Author..... Mat
 * Date....... 12/23/95
 *
 * FilenameList is a list of Filenames.
 ***************************************************************************/
class FilenameList: public IEqualitySequence< Filename >
{
  public:
    // constructor
    FilenameList( INumber numberOfElements = 100 );

    // locator
    Boolean locateFullPath( Filename & filename ) const;
};


#endif

