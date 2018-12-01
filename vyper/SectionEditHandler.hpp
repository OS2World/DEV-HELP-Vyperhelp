/***************************************************************************
 * File...... SectionEditHandler.hpp
 * Author.... Mat
 * Date...... 3/10/98
 *
 * Specialized ICnrEditHandler to allow direct editing of section titles.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef SECTIONEDITHANDLER_HPP
#define SECTIONEDITHANDLER_HPP

// OpenClass
#include <icnrehdr.hpp>     // ICnrEditHandler

class SectionEditHandler: public ICnrEditHandler
{
  public:
    // from ICnrEditHandler
    virtual Boolean endEdit( ICnrEndEditEvent & event );
};


#endif

