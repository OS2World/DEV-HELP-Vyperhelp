/***************************************************************************
 * File...... Footnote.hpp
 * Author.... Mat
 * Date...... 10/27/98
 *
 * Support for IPF footnotes.  Footnotes can appear in the middle of a
 * section, so they must be queued and processed between sections.
 * The Footnote class holds a footnote id (string), and contents in a
 * GinList.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef FOOTNOTE_HPP
#define FOOTNOTE_HPP

// Generator
#include "Gin.hpp"
class Generator;


class Footnote
{
  public:
    // constructor
    Footnote( const IString & id, int level, int windowId );

    // list
    GinList & list();

    // Generator support
    void sendTo( Generator & gen );

  private:
    IString _id;
    int     _level;
    int     _windowId;
    GinList _list;
};


// inline functions
#include "Footnote.ipp"

#endif

