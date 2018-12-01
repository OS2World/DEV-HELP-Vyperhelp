/***************************************************************************
 * File...... BooleanString.cpp
 * Author.... Mat
 * Date...... 3/17/98
 *
 * Implementation of BooleanString
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Vyper
#include "BooleanString.hpp"


BooleanString::BooleanString( Boolean yes ):
    IString( yes? "Yes": "No" )
{}


