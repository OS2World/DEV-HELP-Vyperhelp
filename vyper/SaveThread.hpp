/***************************************************************************
 * File...... SaveThread.hpp
 * Author.... Mat
 * Date...... 5/18/99
 *
 * This thread will save (generate) a VyDoc into a file.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef SAVETHREAD_HPP
#define SAVETHREAD_HPP


// OpenClass
#include <ithread.hpp>      // IThreadFn
#include <istring.hpp>      // IString

// Generator
class Generator;

// Vyper
class VyDoc;
class VyIndicator;


class SaveThread: public IThreadFn
{
  public:
    // constructor
    SaveThread( VyDoc & doc, VyIndicator & indicator, Generator * gen, const IString & filename );
      // if no Generator is given, a "save" is assumed
      // if filename != doc.filename(), a rename is assumed

    // run function
    virtual void run();

  private:
    VyDoc &       _doc;
    VyIndicator & _indicator;
    Generator *   _gen;
    IString       _filename;
};


#endif

