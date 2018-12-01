/***************************************************************************
 * File...... ColumnList.hpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Queue (list) of Distance which represent the columns in a table.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef COLUMNLIST_HPP
#define COLUMNLIST_HPP

// OpenClass
#include <iqueue.h>

// Generator
#include "Distance.hpp"


class ColumnList: public IQueue< Distance >
{
  public:
    // constructor
    ColumnList( INumber numberOfElements = 10 );
};


#endif

