/***************************************************************************
 * File...... InputBinaryFile.cpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Implementation of InputBinaryFile
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <fstream.h>
#include <istring.hpp>
#include <iexcept.hpp>

// File Support
#include "MekTek.hpp"
#include "InputBinaryFile.hpp"


InputBinaryFile::InputBinaryFile( const IString & filename ):
  _file( new ifstream( filename, ios::binary | ios::in ) ),
  _stream( _file ),
  _bits( 0x00 ),
  _bitMask( 0x01 )
{
  set( *_stream );
  check();  // verify state of the file stream
}


InputBinaryFile::InputBinaryFile( istream & stream ):
  _file( 0 ),
  _stream( &stream ),
  _bits( 0x00 ),
  _bitMask( 0x01 )
{
  set( stream );
  check();
}


InputBinaryFile::~InputBinaryFile()
{
  close();
}


void InputBinaryFile::close()
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


unsigned InputBinaryFile::offset() const
{
  MEKASSERT( _stream );
  return _stream->tellg();
}


void InputBinaryFile::setOffset( unsigned pos )
{
  MEKASSERT( _stream );
  _stream->seekg( pos );
  check();
}


unsigned InputBinaryFile::size()
{
  MEKASSERT( _stream );

  // save current position
  unsigned pos = _stream->tellg();

  // seek to end
  _stream->seekg( 0, ios::end );
  unsigned result = _stream->tellg();

  // return to current position
  _stream->seekg( pos );
  check();

  return result;
}


char InputBinaryFile::get()
{
  MEKASSERT( _stream );

  int c = _stream->get();
  if ( c == EOF )
    MEKTHROWCERROR();
  return (char)c;
}


void InputBinaryFile::read( void * buffer, unsigned length )
{
  MEKASSERT( _stream );

  _stream->read( (char *)buffer, length );
  check();
}


Boolean InputBinaryFile::readBoolean()
{
  // get next byte if necessary
  if ( _bitMask == 0x01 )
    _bits = get();

  Boolean b = ( ( _bits & _bitMask ) == _bitMask );
  _bitMask <<= 1;
  return b;
}


unsigned InputBinaryFile::readVariable()
{
  // reset bit count
  _bitMask = 0x01;

  // read bytes as necessary
  // (255 indicates more bytes to add in)
  unsigned n = 0;
  unsigned char c;
  do
  {
    c = get();
    n += c;
  } while ( c == 255 );

  return n;
}


short InputBinaryFile::readShort()
{
  // reset bit count
  _bitMask = 0x01;

  // read short (2 bytes)
  short n;
  read( &n, 2 );
  return n;
}


long InputBinaryFile::readLong()
{
  // reset bit count
  _bitMask = 0x01;

  // read long (4 bytes)
  long n;
  read( &n, 4 );
  return n;
}


IString InputBinaryFile::readString()
{
  // get string length
  unsigned n = readVariable();

  // allocate buffer for string data
  char * buffer = new char[n];

  // load the buffer
  read( buffer, n );

  // create string
  IString string( buffer, n );

  // free the buffer
  delete buffer;

  // return the string
  return string;
}


void InputBinaryFile::readBinary( const IString & filename )
{
  // read file size
  unsigned n = readLong();

  // allocate buffer for file data
  char * buffer = new char[n];

  // load the buffer
  read( buffer, n );

  // write file contents
  ofstream stream( filename, ios::binary | ios::out );
  if ( ! stream )
    MEKTHROWCERROR();
  stream.write( buffer, n );
  if ( ! stream )
    MEKTHROWCERROR();

  // free the buffer
  delete buffer;
}


