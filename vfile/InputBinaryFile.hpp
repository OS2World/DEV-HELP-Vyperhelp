/***************************************************************************
 * File...... BinaryFile.hpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * A BinaryFile implementation which support input-only from a file or a
 * generic istream.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef INPUTBINARYFILE_HPP
#define INPUTBINARYFILE_HPP

// Standard C++
class istream;
class ifstream;

// OpenClass
class IString;

// File Support
#include "BinaryFile.hpp"


class _Export InputBinaryFile: public BinaryFile
{
  public:
    // constructor
    InputBinaryFile( const IString & filename );
    InputBinaryFile( istream & stream );
    virtual ~InputBinaryFile();

    // file size
    unsigned size();

    // read routines
    Boolean  readBoolean();
    unsigned readVariable();
    short    readShort();
    long     readLong();
    IString  readString();
    void     readBinary( const IString & filename );

    // from BinaryFile
    virtual unsigned offset() const;
    virtual void     setOffset( unsigned offset );
    virtual void     close();

  private:
    // read helpers
    char get();
    void read( void * buffer, unsigned length );

    // support for combining Booleans into a byte
    unsigned char _bits;
    unsigned char _bitMask;

    // data stream
    ifstream * _file;
    istream *  _stream;
};



#endif


