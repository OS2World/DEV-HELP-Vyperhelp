/***************************************************************************
 * File...... OutputBinaryFile.cpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Implementation of OutputBinaryFile
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <fstream.h>
#include <iexcept.hpp>
#include <istring.hpp>

// File Support
#include "MekTek.hpp"
#include "OutputBinaryFile.hpp"


OutputBinaryFile::OutputBinaryFile( const IString & filename ):
  _file( new ofstream( filename, ios::binary | ios::out ) ),
  _stream( _file ),
  _bits( 0x00 ),
  _bitMask( 0x01 )
{
  set( *_stream );
  check();  // verify state of the file stream
}


OutputBinaryFile::OutputBinaryFile( ostream & stream ):
  _file( 0 ),
  _stream( &stream ),
  _bits( 0x00 ),
  _bitMask( 0x01 )
{
  set( stream );
  check();
}


OutputBinaryFile::~OutputBinaryFile()
{
  close();
}


void OutputBinaryFile::close()
{
  // close and disassociate file
  if ( _file )
  {
    _file->close();
    delete _file;
    _file = 0;
  }

  // disassociate stream
  _stream = 0;
}


unsigned OutputBinaryFile::offset() const
{
  MEKASSERT( _stream );
  return _stream->tellp();
}


void OutputBinaryFile::setOffset( unsigned pos )
{
  MEKASSERT( _stream );
  _stream->seekp( pos );
  check();
}


void OutputBinaryFile::flush()
{
  writeBits();
}


void OutputBinaryFile::put( char c )
{
  MEKASSERT( _stream );

  writeBits();
  _stream->put( c );
  check();
}


void OutputBinaryFile::write( const void * buffer, unsigned length )
{
  MEKASSERT( _stream );

  writeBits();
  _stream->write( (const char *)buffer, length );
  check();
}


void OutputBinaryFile::writeBits()
{
  if ( _bitMask != 0x01 )
  {
    _bitMask = 0x01;  // do this before put() to avoid recursion
    put( _bits );
    _bits = 0x00;
  }
}


void OutputBinaryFile::writeBoolean( Boolean b )
{
  if ( b )
    _bits |= _bitMask;
  _bitMask <<= 1;
  if ( ! _bitMask )
    writeBits();
}


void OutputBinaryFile::writeVariable( unsigned n )
{
  // write 255 to extend, as necessary
  while ( n > 254 )
  {
    put( 255 );
    n -= 255;
  }

  // write the last byte
  put( n );
}


void OutputBinaryFile::writeShort( short n )
{
  write( &n, 2 );
}


void OutputBinaryFile::writeLong( long n )
{
  write( &n, 4 );
}


void OutputBinaryFile::writeString( const IString & string )
{
  unsigned length = string.length();
  writeVariable( length );
  write( (const char *)string, length );
}


void OutputBinaryFile::writeBinary( const IString & filename )
{
  MEKASSERT( _stream );

  writeBits();

  // write file size
  ifstream  stream( filename, ios::binary | ios::in );
  if ( ! stream )
    MEKTHROWCERROR();
  filebuf * file = stream.rdbuf();
  streampos size = file->seekoff( 0, ios::end, 0 );
  if ( size == EOF )
    MEKTHROWCERROR();
  writeLong( size );

  // write file contents
  streampos pos = file->seekoff( 0, ios::beg, 0 );
  if ( pos == EOF )
    MEKTHROWCERROR();
  *_stream << file;
  check();
}

