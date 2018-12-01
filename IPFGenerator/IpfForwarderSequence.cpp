/***************************************************************************
 * File...... IpfForwarderSequence.cpp
 * Author.... Mat
 * Date...... 6/5/99
 *
 * Implementation of IpfForwarderSequence.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


#include "IpfForwarderSequence.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>

#pragma define(IGLnSqCrs<IpfForwarder,IStdOps<IpfForwarder> >)
#pragma define(IGLinkedSequenceNode<IpfForwarder,IStdOps<IpfForwarder> >)
#pragma define(ILinkedSequence<IpfForwarder>)
#pragma define(IGLnSq<IpfForwarder,IStdOps<IpfForwarder> >)



IpfForwarderSequence::IpfForwarderSequence( INumber numberOfElements ):
    ISequence< IpfForwarder >( numberOfElements )
{}


