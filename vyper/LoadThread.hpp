/***************************************************************************
 * File...... LoadThread.hpp
 * Author.... Mat
 * Date...... 5/18/99
 *
 * This thread will load (parse) and file into a VyDoc.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef LOADTHREAD_HPP
#define LOADTHREAD_HPP


// OpenClass
#include <ithread.hpp>      // IThreadFn
#include <istring.hpp>      // IString

// Generator
class Parser;

// Vyper
class VyDoc;
class VyIndicator;


class LoadThread: public IThreadFn
{
  public:
    // constructor
    LoadThread( VyDoc & doc, VyIndicator & indicator, Parser * parser, const IString & filename );

    // run function
    virtual void run();

  private:
    VyDoc &       _doc;
    VyIndicator & _indicator;
    Parser *      _parser;
    IString       _filename;
};


#endif

