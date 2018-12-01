/***************************************************************************
 * File...... IpfForwarder.hpp
 * Author.... Mat
 * Date...... 6/4/99
 *
 * Structure to hold information for forwarding IDs in IPF.  IPF only supports
 * up to one ID per section.  We use a trick with auto-links from a hidden
 * section to "forward" requests to the proper panel.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef IPFFORWARDER_HPP
#define IPFFORWARDER_HPP

// Generator
#include "Gin.hpp"


class IpfForwarder
{
  public:
    // constructor
    IpfForwarder( const LabelGin & label, int targetId );

    // getters
    const LabelGin & label() const;
    int              targetId() const;

  private:
    LabelGin _label;
    int      _targetId;
};


// inline functions
#include "IpfForwarder.ipp"


#endif

