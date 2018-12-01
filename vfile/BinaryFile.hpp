/***************************************************************************
 * File...... BinaryFile.hpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Generic support for a binary file which can read and write different
 * basic field types:
 *   boolean   bits are combined when consecutive booleans are written
 *   variable  usually a byte, automatically extended if necessary
 *   short     16 bit number (2 bytes)
 *   long      32 bit number (4 bytes)
 *   string    variable length string
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef BINARYFILE_HPP
#define BINARYFILE_HPP

// Standard C++
class ios;

// OpenClass
#include <istring.hpp>


class _Export BinaryFile
{
  public:
    // constructor
    BinaryFile();
    virtual ~BinaryFile();

    // offset control
    virtual unsigned offset() const = 0;
    virtual void     setOffset( unsigned offset ) = 0;

    // close the file
    virtual void close() = 0;

  protected:
    // error checking
    void set( ios & io );
    void check();

  private:
    // i/o stream
    ios * _io;
};



#endif


