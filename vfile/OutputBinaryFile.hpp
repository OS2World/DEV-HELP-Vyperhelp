/***************************************************************************
 * File...... OutputBinaryFile.hpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * A OutputBinaryFile implementation which supports output-only from a file or a
 * generic istream.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef OUTPUTBINARYFILE_HPP
#define OUTPUTBINARYFILE_HPP

// Standard C++
class ofstream;
class ostream;

// OpenClass
class IString;

// File Support
#include "BinaryFile.hpp"


class _Export OutputBinaryFile: public BinaryFile
{
  public:
    // constructor
    OutputBinaryFile( const IString & filename );
    OutputBinaryFile( ostream & stream );
    ~OutputBinaryFile();

    // write functions
    void writeBoolean( Boolean b );
    void writeVariable( unsigned n );
    void writeShort( short n );
    void writeLong( long n );
    void writeString( const IString & string );
    void writeBinary( const IString & filename );
    void flush();

    // from BinaryFile
    virtual unsigned offset() const;
    virtual void     setOffset( unsigned offset );
    virtual void close();

  private:
    // write helpers
    void put( char c );
    void write( const void * buffer, unsigned length );
    void writeBits();

    // support for combining Booleans into a byte
    unsigned char _bits;
    unsigned char _bitMask;

    // data stream
    ofstream * _file;
    ostream *  _stream;
};



#endif


