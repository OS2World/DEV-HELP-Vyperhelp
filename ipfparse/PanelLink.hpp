/***************************************************************************
 * File...... PanelLink.hpp
 * Author.... Mat
 * Date...... 10/29/98
 *
 * A pair of strings, used in collections.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef PANELLINK_HPP
#define PANELLINK_HPP

// OpenClass
#include <istring.hpp>      // IString


class PanelLink
{
  public:
    // constructor
    PanelLink( const IString & name, int id );

    // accessors
    const IString & name() const;
    const int &     id() const;

  private:
    IString _name;
    int     _id;
};


// key function
const int & key( const PanelLink & link );


// inline functions
#include "PanelLink.ipp"

#endif

