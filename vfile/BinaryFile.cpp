/***************************************************************************
 * File...... BinaryFile.cpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Implementation of BinaryFile
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Standard C++
#include <iostream.h>

// OpenClass
#include <iexcept.hpp>

// File Support
#include "MekTek.hpp"
#include "BinaryFile.hpp"


BinaryFile::BinaryFile():
  _io( 0 )
{}


// null destructor declared virtual
BinaryFile::~BinaryFile()
{}


void BinaryFile::set( ios & io )
{
  _io = &io;
}


void BinaryFile::check()
{
  MEKASSERT( _io );
  if ( ! *_io )
    MEKTHROWCERROR();
}


