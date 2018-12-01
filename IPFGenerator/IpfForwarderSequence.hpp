/***************************************************************************
 * File...... IpfForwarderSequence.hpp
 * Author.... Mat
 * Date...... 6/4/99
 *
 * Sequence of IpfForwarders
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef IPFFORWARDERSEQUENCE_HPP
#define IPFFORWARDERSEQUENCE_HPP

// OpenClass
#include <iseq.h>           // ISequence

// Generator
#include "IpfForwarder.hpp"


class IpfForwarderSequence: public ISequence< IpfForwarder >
{
  public:
    IpfForwarderSequence( INumber numberOfElements = 100 );
};


#endif

