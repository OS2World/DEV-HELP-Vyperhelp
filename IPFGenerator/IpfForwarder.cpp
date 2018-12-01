/***************************************************************************
 * File...... IpfForwarder.cpp
 * Author.... Mat
 * Date...... 6/4/99
 *
 * Implementation of IpfForwarder.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


#include "IpfForwarder.hpp"


IpfForwarder::IpfForwarder( const LabelGin & label, int targetId ):
    _label( label ),
    _targetId( targetId )
{}


