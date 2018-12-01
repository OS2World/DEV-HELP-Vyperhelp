/***************************************************************************
 * File...... ContainerHelpHandler.hpp
 * Author.... Mat
 * Date...... 5/27/99
 *
 * Handles help requests for a container.  Simply returns the window's
 * help ID.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef CONTAINERHELPHANDLER_HPP
#define CONTAINERHELPHANDLER_HPP


// OpenClass
#include <icnrhdr.hpp>


class ContainerHelpHandler: public ICnrHandler
{
  public:
    // constructor
    ContainerHelpHandler( unsigned long helpId );

    // from ICnrHandler
    virtual Boolean help( ICnrHelpEvent & evt );

  private:
    unsigned long _helpId;
};


#endif

